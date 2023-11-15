#ifndef sat_midi_file_included
#define sat_midi_file_included
//----------------------------------------------------------------------

#include <vector>

//----------------------------------------------------------------------
//
// event
//
//----------------------------------------------------------------------

struct SAT_MidiEvent {

  uint8_t   type    = 0; // 0:midi, 1:sysex, 2:meta
  uint8_t   msg1    = 0;
  uint8_t   msg2    = 0;
  uint8_t   msg3    = 0;
  uint32_t  delta   = 0;
  float     time    = 0.0;
  uint32_t  offset  = 0;
  uint32_t  datalen = 0;
  uint8_t*  data    = nullptr;
  
  //----------

  SAT_MidiEvent() {
    type    = 0;
    msg1    = 0;
    msg2    = 0;
    msg3    = 0;
    delta   = 0;
    datalen = 0;
    data    = nullptr;
  }

  //----------

  SAT_MidiEvent(uint32_t d, uint8_t m1, uint8_t m2, uint8_t m3)  {
    type    = 0;
    msg1    = m1;
    msg2    = m2;
    msg3    = m3;
    delta   = d;
    delta   = d;
    datalen = 0;
    data    = nullptr;
  }

  //----------

  ~SAT_MidiEvent() {
    if (data) free(data);
  }

  //----------

  void setData(uint8_t* ptr, uint32_t length) {
    if (data) free(data);
    data = (uint8_t*)malloc(length);
    datalen = length;
    memcpy(data,ptr,length);
  }

  //----------

  void setText(uint8_t* ptr, uint32_t length) {
    if (data) free(data);
    data = (uint8_t*)malloc(length+1);
    datalen = length;
    memcpy(data,ptr,length);
    data[length] = 0;
  }

  //----------

  void setData(uint8_t v1,uint8_t v2=0,uint8_t v3=0,uint8_t v4=0,uint8_t v5=0) {
    if (data) free(data);
    data = (uint8_t*)malloc(5);
    datalen = 5;
    data[0] = v1;
    data[1] = v2;
    data[2] = v3;
    data[3] = v4;
    data[4] = v5;
  }

};

//----------

typedef std::vector<SAT_MidiEvent*> SAT_MidiEvents;

//----------------------------------------------------------------------
//
// track
//
//----------------------------------------------------------------------

struct SAT_MidiTrack {

  char*           name        = nullptr;
  uint32_t        num_events  = 0;
  SAT_MidiEvents  events      = {};
  //uint32_t        next_event  = 0;
  //bool            active      = 0;
  float           length      = 0.0;

  //----------

  SAT_MidiTrack() {
    name = nullptr;
    num_events = 0;
    events = {};
  }

  //----------

  ~SAT_MidiTrack() {
    if (name) free(name);
    for (uint32_t i=0; i<events.size(); i++) {
      delete events[i];
    }
  }

  //----------

  // assume non-zero terminated
  void setName(const char* ptr, uint32_t length) {
    if (name) free(name);
    name = (char*)malloc(length+1);
    memcpy(name,ptr,length);
    name[length] = 0;
  }

};

//----------

typedef std::vector<SAT_MidiTrack*> SAT_MidiTracks;

//----------------------------------------------------------------------
//
// sequence
//
//----------------------------------------------------------------------

struct SAT_MidiSequence {

  char*           name          = nullptr;
  uint32_t        format        = 0;
  uint32_t        num_tracks    = 0;
  uint32_t        tpq           = 0;
  SAT_MidiTracks  tracks        = {};
  float           length        = 0.0;

  uint32_t        tempo         = 0.0;
  uint32_t        timesig_num   = 0;
  uint32_t        timesig_denom = 0;

  //----------

  SAT_MidiSequence() {
    name = nullptr;
    format      = 0;
    num_tracks  = 0;
    tpq         = 0;
    tracks      = {};
  }

  //----------

  ~SAT_MidiSequence() {
    if (name) free(name);
    for (uint32_t i=0; i<tracks.size(); i++) {
      delete tracks[i];
    }
  }

  //----------

  void setName(const char* ptr, uint32_t length) {
    if (name) free(name);
    name = (char*)malloc(length+1);
    memcpy(name,ptr,length);
    name[length] = 0;
  }

  //----------

  void calc_time() {
    uint32_t  us_per_qnote    = tempo; // <Tempo in latest Set Tempo event>
    uint32_t  ticks_per_qnote = tpq; // <PPQ from the header>
    float     us_per_tick     = (float)us_per_qnote / (float)ticks_per_qnote;
    float     s_per_tick      = us_per_tick / 1000000.0;
    length = 0.0;
    for (uint32_t t=0; t<num_tracks; t++) {
      SAT_MidiTrack* track = tracks[t];
      track->length = 0.0;
      uint32_t num_events = track->num_events;
      uint32_t delta = 0;
      for (uint32_t e=0; e<num_events; e++) {
        SAT_MidiEvent* event = track->events[e];
        delta += event->delta;
        float time = (float)delta * (float)s_per_tick;
        event->time = time;
        if (time > track->length) track->length = time;
        if (time > length) length = time;
      }
    }
  }

};



//----------------------------------------------------------------------
//
// file
//
//----------------------------------------------------------------------

class SAT_MidiFile {

//------------------------------
private:
//------------------------------

  uint8_t*          MBuffer          = nullptr;
  uint8_t*          MBufferPtr       = nullptr;
  uint32_t          MBufferSize      = 0;

  SAT_MidiSequence* MSequence        = nullptr;

  uint8_t*          MTrackStartPtr   = nullptr;
  uint32_t          MRunningStatus   = 0;
  bool              MEndOfTrack      = false;
  uint32_t          MNumTrackEvents  = 0;
  SAT_MidiTrack*    MCurrentTrack    = nullptr;
//uint32_t          MSplitTracks     = 0;

//------------------------------
public:
//------------------------------

  SAT_MidiFile() {
    MSequence = new SAT_MidiSequence();
  }

  //----------

  ~SAT_MidiFile() {
    delete MSequence;
  }

//------------------------------
public:
//------------------------------

  uint32_t load(uint8_t* ABuffer, uint32_t ASize) {
    MBuffer     = ABuffer;
    MBufferPtr  = ABuffer;
    MBufferSize = ASize;
    read_header();
    read_tracks();
    return 0;
  }

  //----------

  uint32_t load(const char* AFilename) {
    MSequence->setName(AFilename,strlen(AFilename));
    FILE* fp = fopen(AFilename,"rb");
    fseek(fp,0,SEEK_END);
    uint32_t size = ftell(fp);
    fseek(fp,0L,SEEK_SET);
    uint8_t* buffer = (uint8_t*)malloc(size); // deleted in unload (as MBuffer)
    __attribute__ ((unused)) uint32_t result; // shut up, gcc!
    result = fread(buffer,1,size,fp);
    fclose(fp);
    result = load(buffer,size);
    //free(buffer);
    MSequence->calc_time();
    return result;
  }

  //----------

  void unload() {
    if (MBuffer) free(MBuffer);
    MBuffer = nullptr;
    if (MSequence) delete MSequence;
    MSequence = nullptr;
  }

  //----------

  SAT_MidiSequence* getMidiSequence() {
    return MSequence;
  }

  //----------

  void print() {
    if (MSequence) {
      printf("name: '%s'\n",MSequence->name);
      printf("format: %i\n",MSequence->format);
      printf("tpq: %i\n",MSequence->tpq);
      printf("num_tracks: %i\n",MSequence->num_tracks);
      if (MSequence->tracks.size() > 0) {
        for (uint32_t t=0; t<MSequence->tracks.size(); t++) {
          printf("Track %i\n",t);
          SAT_MidiTrack* track = MSequence->tracks[t];
          if (track) {
            printf("  name: '%s'\n",track->name);
            printf("  num_events: %i\n",track->num_events);
            if (track->events.size() > 0) {
              for (uint32_t e=0; e<track->events.size(); e++) {
                SAT_MidiEvent* event = track->events[e];
                print_event(e,event);
              } // for e
            } // events > 0
            else { printf("events.size <= 0\n"); }
          } // track
          else  { printf("track(%i) == null\n",t); }
        } // for t
      } // tracks > 0
      else { printf("tracks.size <= 0\n"); }

    } // sequence
    else { printf("sequence == null\n"); }
  }

  //----------

  void print_event(uint32_t i, SAT_MidiEvent* event) {
    if (event->msg1 < 0xF0) {
      switch (event->msg1 & 0xF0) {
        case 0x80: printf("%i : %02X %02X %02X note off [delta %i time %f]\n",               i,event->msg1,event->msg2,event->msg3,event->delta,event->time); break;
        case 0x90: printf("%i : %02X %02X %02X note on [delta %i time %f]\n",                i,event->msg1,event->msg2,event->msg3,event->delta,event->time); break;
        case 0xA0: printf("%i : %02X %02X %02X polyphonic key pressure [delta %i time %f]\n",i,event->msg1,event->msg2,event->msg3,event->delta,event->time); break;
        case 0xB0: printf("%i : %02X %02X %02X control change [delta %i time %f]\n",         i,event->msg1,event->msg2,event->msg3,event->delta,event->time); break;
        case 0xC0: printf("%i : %02X %02X -- program change [delta %i time %f]\n",           i,event->msg1,event->msg2,event->delta,event->time); break;
        case 0xD0: printf("%i : %02X %02X -- channel pressuer [delta %i time %f]\n",         i,event->msg1,event->msg2,event->delta,event->time); break;
        case 0xE0: printf("%i : %02X %02X %02X pitch wheel [delta %i time %f]\n",            i,event->msg1,event->msg2,event->msg3,event->delta,event->time); break;
        default:   printf("%i : unknown event [delta %i time %f]\n",i,event->delta,event->time); break;
      }
    } // < f0
    else {
      if (event->msg1 < 0xFF) {
        //printf("%i : %02X %02X %02X [%i]\n",i,event->msg1,event->msg2,event->msg3,event->delta,event->time);
        switch (event->msg1) {
          case 0xF0: printf("%i : F0 -- -- [delta %i time %f] sysex (size %i)\n",i, event->delta,event->time,event->datalen); break;
          case 0xF1: printf("%i : F1 -- -- [delta %i time %f] ?\n",              i, event->delta,event->time); break;
          case 0xF2: printf("%i : F2 -- -- [delta %i time %f] ?\n",              i, event->delta,event->time); break;
          case 0xF3: printf("%i : F3 -- -- [delta %i time %f] ?\n",              i, event->delta,event->time); break;
          case 0xF4: printf("%i : F4 -- -- [delta %i time %f] ?\n",              i, event->delta,event->time); break;
          case 0xF5: printf("%i : F5 -- -- [delta %i time %f] ?\n",              i, event->delta,event->time); break;
          case 0xF6: printf("%i : F6 -- -- [delta %i time %f] ?\n",              i, event->delta,event->time); break;
          case 0xF7: printf("%i : F7 -- -- [delta %i time %f] sysex (size %i)\n",i, event->delta,event->time,event->datalen); break;
          case 0xF8: printf("%i : F8 -- -- [delta %i time %f] timing clock\n",   i, event->delta,event->time); break;
          case 0xF9: printf("%i : F9 -- -- [delta %i time %f] undefined\n",      i, event->delta,event->time); break;
          case 0xFA: printf("%i : FA -- -- [delta %i time %f] start\n",          i, event->delta,event->time); break;
          case 0xFB: printf("%i : FB -- -- [delta %i time %f] continue\n",       i, event->delta,event->time); break;
          case 0xFC: printf("%i : FC -- -- [delta %i time %f] stop\n",           i, event->delta,event->time); break;
          case 0xFD: printf("%i : FD -- -- [delta %i time %f] undefined\n",      i, event->delta,event->time); break;
          case 0xFE: printf("%i : FE -- -- [delta %i time %f] active sensing\n", i, event->delta,event->time); break;
          default:   printf("%i : ? [delta %i time %f]\n",i,event->delta,event->time); break;
        } // switch
      } // < ff
      else {
        //printf("%i : FF %02X %02X [%i]\n",i,event->msg2,event->msg3,event->delta,event->time);
        uint8_t v1,v2,v3,v4,v5;
        uint32_t v;
        switch(event->msg2) {
          case 0x00:
            v1 = event->data[0];
            v2 = event->data[1];
            v = (v1 << 8) + v2;
            printf("%i : FF%02X [delta %i time %f] seq number: %i (size %i)\n",i, event->msg2,event->delta,event->time,v,event->datalen);
            break;
          case 0x01: printf("%i : FF%02X [delta %i time %f] text: '%s' (size %i)\n",i, event->msg2,event->delta,event->time,event->data,event->datalen); break;
          case 0x02: printf("%i : FF%02X [delta %i time %f] copyright: '%s' (size %i)\n",i, event->msg2,event->delta,event->time,event->data,event->datalen); break;
          case 0x03: printf("%i : FF%02X [delta %i time %f] seq/track name: '%s' (size %i)\n",i, event->msg2,event->delta,event->time,event->data,event->datalen); break;
          case 0x04: printf("%i : FF%02X [delta %i time %f] instrument name: '%s' (size %i)\n",i, event->msg2,event->delta,event->time,event->data,event->datalen); break;
          case 0x05: printf("%i : FF%02X [delta %i time %f] lyric: '%s' (size %i)\n",i, event->msg2,event->delta,event->time,event->data,event->datalen); break;
          case 0x06: printf("%i : FF%02X [delta %i time %f] marker: '%s' (size %i)\n",i, event->msg2,event->delta,event->time,event->data,event->datalen); break;
          case 0x07: printf("%i : FF%02X [delta %i time %f] cue point: '%s' (size %i)\n",i, event->msg2,event->delta,event->time,event->data,event->datalen); break;
          case 0x08: printf("%i : FF%02X [delta %i time %f] program name: '%s' (size %i)\n",i, event->msg2,event->delta,event->time,event->data,event->datalen); break;
          case 0x09: printf("%i : FF%02X [delta %i time %f] device name: '%s' (size %i)\n",i, event->msg2,event->delta,event->time,event->data,event->datalen); break;
          case 0x20:
            v1 = event->data[0];
            printf("%i : FF%02X [delta %i time %f] midi channel: %i (size %i)\n",i, event->msg2,event->delta,event->time,v1,event->datalen);
            break;
          case 0x21:
            v1 = event->data[0];
            printf("%i : FF%02X [delta %i time %f] midi port: %i (size %i)\n",i, event->msg2,event->delta,event->time,v1,event->datalen);
            break;
          case 0x2f:
            printf("%i : FF%02X [delta %i time %f] end of track (size %i)\n",i, event->msg2,event->delta,event->time,event->datalen);
            break;
          case 0x51:
            v1 = event->data[0];
            v2 = event->data[1];
            v3 = event->data[2];
            v = (v1 << 16) + (v2 < 8) + v3;
            printf("%i : FF%02X [delta %i time %f] tempo: %i (size %i)\n",i, event->msg2,event->delta,event->time,v,event->datalen);
            break;
          case 0x54:
            v1 = event->data[0];
            v2 = event->data[1];
            v3 = event->data[2];
            v4 = event->data[3];
            v5 = event->data[4];
            printf("%i : FF%02X [delta %i time %f] SMPTE: %i %i %i %i %i (size %i)\n",i, event->msg2,event->delta,event->time,v1,v2,v3,v4,v5,event->datalen);
            break;
          case 0x58:
            v1 = event->data[0];
            v2 = event->data[1];
            v3 = event->data[2];
            v4 = event->data[3];
            printf("%i : FF%02X [delta %i time %f] time signature: %i %i %i %i (size %i)\n",i, event->msg2,event->delta,event->time,v1,v2,v3,v4,event->datalen);
            break;
          case 0x59:
            v1 = event->data[0];
            v2 = event->data[1];
            printf("%i : FF%02X [delta %i time %f] key signature: %i %i (size %i)\n",i, event->msg2,event->delta,event->time,v1,v2,event->datalen);
            break;
          case 0x7F:
            printf("%i : FF%02X [delta %i time %f] seq specific (size %i)\n",i, event->msg2,event->delta,event->time,event->datalen);
            break;
          default:
            printf("%i : FF%02X [delta %i time %f] unknown event\n",i, event->msg2,event->delta,event->time);
            break;
        } // switch msg2
      } // ! < ff
    } // ! < f0
  }

//------------------------------
private:
//------------------------------

  uint8_t peek_byte() {
    assert(MBufferPtr);
    uint8_t v = *MBufferPtr;
    return v;
  }

  //----------

  uint8_t read_byte() {
    assert(MBufferPtr);
    uint8_t v = *MBufferPtr++;
    return v;
  }

  //----------

  uint16_t read_short() {
    assert(MBufferPtr);
    uint8_t v1 = read_byte();
    uint8_t v2 = read_byte();
    return (v1<<8) + v2;
  }

  //----------

  uint32_t read_uint() {
    assert(MBufferPtr);
    uint8_t v1 = read_byte();
    uint8_t v2 = read_byte();
    uint8_t v3 = read_byte();
    uint8_t v4 = read_byte();
    uint32_t sum = (v1<<24) + (v2<<16) + (v3<<8) + v4;
    return sum;
  }

  //----------

  //uint32_t read_variable() {
  //  uint32_t shift = 7; // 0;
  //  uint8_t byte = read_byte();
  //  uint32_t sum = (byte & 0x7f);
  //  while (byte & 0x80) {
  //    byte = read_byte();
  //    //sum += (byte & 0x7f) << shift;
  //    sum = (sum << shift) + (byte & 0x7f);
  //    shift += 7;
  //  }
  //  return sum;
  //}

  //----------

  uint32_t read_variable() {
    uint8_t   c;
    uint32_t value = read_byte();
    if (value & 0x80) {
      value &= 0x7F;
      do {
        c = read_byte();
        value = (value << 7) + (c & 0x7F);
      } while (c & 0x80);
    }
    return value;
  }

  //----------

  uint8_t* read_data(uint32_t length) {
    assert(MBufferPtr);
    uint8_t* result = MBufferPtr;
    MBufferPtr += length;
    return result;
  }

//------------------------------
private:
//------------------------------

  /*
    The header chunk at the beginning of the file specifies some basic
    information about the data in the file. Here's the syntax of the complete
    chunk:
    <Header Chunk> = <chunk type><length><format><ntrks><division>
    As described above, <chunk type> is the four ASCII characters 'MThd';
    <length> is a 32-bit representation of the number 6 (high byte first).
    The data section contains three 16-bit words, stored most-significant byte first.
    The first word, <format>, specifies the overall organisation of the file.
    Only three values of <format> are specified:

      0 - the file contains a single multi-channel track
      1 - the file contains one or more simultaneous tracks (or MIDI outputs)
          of a sequence
      2 - the file contains one or more sequentially independent single-track
          patterns

    The next word, <ntrks>, is the number of track chunks in the file.
    It will always be 1 for a format 0 file.
    The third word, <division>, specifies the meaning of the delta-times.
    It has two formats, one for metrical time, and one for time-code-based time:

    bit 15	  bits 14 thru 8	          bits 7 thru 0
    0	        ticks per quarter-note
    1	        negative SMPTE format     ticks per frame

    If bit 15 of <division> is zero, the bits 14 thru 0 represent the number of
    delta time "ticks" which make up a quarter-note. For instance, if division
    is 96, then a time interval of an eighth-note between two events in the
    file would be 48.

    If bit 15 of <division> is a one, delta times in a file correspond to
    subdivisions of a second, in a way consistent with SMPTE and MIDI Time
    Code. Bits 14 thru 8 contain one of the four values -24, -25, -29, or -30,
    corresponding to the four standard SMPTE and MIDI Time Code formats (-29
    corresponds to 30 drop frame), and represents the number of frames per
    second. These negative numbers are stored in two's compliment form. The
    second byte (stored positive) is the resolution within a frame: typical
    values may be 4 (MIDI Time Code resolution), 8, 10, 80 (bit resolution), or
    100. This stream allows exact specifications of time-code-based tracks, but
    also allows millisecond-based tracks by specifying 25 frames/sec and a
    resolution of 40 units per frame. If the events in a file are stored with a
    bit resolution of thirty-frame time code, the division word would be E250
    hex.

  */

  void read_header() {
    uint32_t id = read_uint(); // "MThd" (4D,54,68,64)
    if (id != 0x4d546864) { printf("wrong file header id\n"); return; }
    uint32_t size = read_uint();
    if (size != 6) { printf("wrong file header size\n"); return; }
    uint16_t v1 = read_short();
    uint16_t v2 = read_short();
    uint16_t v3 = read_short();
    //printf("\n  format %04x ntrks %04x division %04x\n\n",v1,v2,v3);
    MSequence->format     = v1;
    MSequence->num_tracks = v2;
    //if (v3 & 0x8000) {}
    MSequence->tpq        = v3;
  }

  //----------

  /*
    The track chunks (type MTrk) are where actual song data is stored. Each
    track chunk is simply a stream of MIDI events (and non-MIDI events),
    preceded by delta-time values. The format for Track Chunks (described
    below) is exactly the same for all three formats (0, 1, and 2: see "Header
    Chunk" above) of MIDI Files.

    Here is the syntax of an MTrk chunk (the + means "one or more": at least
    one MTrk event must be present):
      <Track Chunk> = <chunk type><length><MTrk event>+
*/

  void start_new_track() {
    SAT_MidiTrack* track = new SAT_MidiTrack();
    MSequence->tracks.push_back(track);
    MCurrentTrack = track;
    MCurrentTrack->setName("",0);
  }

  //----------

  void read_tracks() {
    //MSplitTracks = 0;
    for (uint32_t i=0; i<MSequence->num_tracks; i++) {
      uint32_t id = read_uint(); // "MTrk" (4D,54,72,6B)
      if (id != 0x4d54726b) { printf("  Error: track %i wrong track header id 0x%08x (wants 0x4d54726b)\n",i,id); return; }
      uint32_t size = read_uint();
      start_new_track();
      uint32_t num_events = read_track_events(size);
      MCurrentTrack->num_events = num_events;
      //i += MSplitTracks;
      //MSplitTracks = 0;
    }
  }

  //----------

  /*
    The syntax of an MTrk event is very simple:
      <MTrk event> = <delta-time><event>

    <delta-time> is stored as a variable-length quantity. It represents the
    amount of time before the following event. If the first event in a track
    occurs at the very beginning of a track, or if two events occur
    simultaneously, a delta-time of zero is used. Delta-times are always present.
    (Not storing delta-times of 0 requires at least two bytes for any other
    value, and most delta-times aren't zero.) Delta-time is in some fraction of a
    beat (or a second, for recording a track with SMPTE times), as specified in
    the header chunk.
  */

  uint32_t read_track_events(uint32_t ASize) {
    MNumTrackEvents = 0;
    MTrackStartPtr = MBufferPtr;
    MEndOfTrack = false;
    while (MEndOfTrack == false) {
      uint32_t delta_time = read_variable();
      read_event(delta_time);
      MNumTrackEvents += 1;
    }
    return MNumTrackEvents;
  }

//------------------------------
private:
//------------------------------

  /*
    <MIDI event> is any MIDI channel message See Appendix 1 - MIDI Messages.
    Running status is used: status bytes of MIDI channel messages may be
    omitted if the preceding event is a MIDI channel message with the same
    status. The first event in each MTrk chunk must specify status. Delta-
    time is not considered an event itself: it is an integral part of the
    syntax for an MTrk event. Notice that running status occurs across delta-
    times.
  */

  void read_event(uint32_t ADeltaTime) {

    uint8_t        index       = 0;
    uint8_t        value       = 0;
    uint8_t        event       = 0;
    SAT_MidiEvent* midi_event  = nullptr;

    // running status

    if (peek_byte() < 0x80) {
      event = MRunningStatus;
    }
    else {
      event = read_byte();
      //MRunningStatus = event;
    }
    MRunningStatus = event;

    // midi events

    switch (event & 0xf0) {
      case 0x80:
        index = read_byte();
        value = read_byte();
        midi_event = new SAT_MidiEvent(ADeltaTime,event,index,value);
        MCurrentTrack->events.push_back(midi_event);
        break;
      case 0x90:
        index = read_byte();
        value = read_byte();
        midi_event = new SAT_MidiEvent(ADeltaTime,event,index,value);
        MCurrentTrack->events.push_back(midi_event);
        break;
      case 0xA0:
        index = read_byte();
        value = read_byte();
        midi_event = new SAT_MidiEvent(ADeltaTime,event,index,value);
        MCurrentTrack->events.push_back(midi_event);
        break;
      case 0xB0:
        index = read_byte();
        value = read_byte();
        midi_event = new SAT_MidiEvent(ADeltaTime,event,index,value);
        MCurrentTrack->events.push_back(midi_event);
        break;
      case 0xC0:
        index = read_byte();
        value = read_byte();
        midi_event = new SAT_MidiEvent(ADeltaTime,event,index,value);
        MCurrentTrack->events.push_back(midi_event);
        break;
      case 0xD0:
        index = read_byte();
        value = read_byte();
        midi_event = new SAT_MidiEvent(ADeltaTime,event,index,value);
        MCurrentTrack->events.push_back(midi_event);
        break;
      case 0xE0:
        index = read_byte();
        value = read_byte();
        //printf("  0x%02x %i %i pitch wheel (delta %i)\n",event,index,value,ADeltaTime);
        midi_event = new SAT_MidiEvent(ADeltaTime,event,index,value);
        MCurrentTrack->events.push_back(midi_event);
        break;
      case 0xF0: // system
        parse_event_system(event,ADeltaTime);
        break;
      //default:
      //  // 0x..7x (running status?)
      //  printf("  0x%02x unknown event (delta %i)\n",event,ADeltaTime);
      //  break;
    } // switch

  }

  //----------

  void parse_event_system(uint8_t AEvent, uint32_t ADeltaTime) {
    SAT_MidiEvent* midi_event = nullptr;
    uint32_t length;
    switch (AEvent) {

      /*
        <sysex event> is used to specify a MIDI system exclusive message,
        either as one unit or in packets, or as an "escape" to specify any
        arbitrary bytes to be transmitted. See Appendix 1 - MIDI Messages.
        A normal complete system exclusive message is stored in a MIDI File in
        this way:
        F0 <length> <bytes to be transmitted after F0>

        An F0 sysex event is used for the first packet in a series -- it is a
        message in which the F0 should be transmitted. An F7 sysex event is
        used for the remainder of the packets, which do not begin with F0.
        (Of course, the F7 is not considered part of the system exclusive
        message).
        A syntactic system exclusive message must always end with an F7, even
        if the real-life device didn't send one, so that you know when you've
        reached the end of an entire sysex message without looking ahead to the
        next event in the MIDI File. If it's stored in one complete F0 sysex
        event, the last byte must be an F7. There also must not be any
        transmittable MIDI events in between the packets of a multi-packet
        system exclusive message.
      */

      case 0xF0:
        length = read_variable();
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,0,0);
        midi_event->setData(MBufferPtr,length);
        MCurrentTrack->events.push_back(midi_event);
        MBufferPtr += length;
        break;

      /*
        Another form of sysex event is provided which does not imply that an F0
        should be transmitted. This may be used as an "escape" to provide for
        the transmission of things which would not otherwise be legal,
        including system realtime messages, song pointer or select, MIDI Time
        Code, etc. This uses the F7 code:
        F7 <length> <all bytes to be transmitted>
        ...
        When reading a MIDI File, and an F7 sysex event is encountered without
        a preceding F0 sysex event to start a multi-packet system exclusive
        message sequence, it should be presumed that the F7 event is being used
        as an "escape". In this case, it is not necessary that it end with an
        F7, unless it is desired that the F7 be transmitted.
      */

      case 0xF7:
        // F0 <length> <bytes to be transmitted after F0>
        length = read_variable();
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,0,0);
        midi_event->setData(MBufferPtr,length);
        MCurrentTrack->events.push_back(midi_event);
        MBufferPtr += length;
        break;

      /*
        Timing Clock.
        Sent 24 times per quarter note when synchronisation is required.
      */

      case 0xF8:
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,0,0);
        MCurrentTrack->events.push_back(midi_event);
        break;

      case 0xF9: // undefined
        break;

      /*
        Start.
        Start the current sequence playing. (This message will be followed with
        Timing Clocks).
      */

      case 0xFA:
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,0,0);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
        Continue.
        Continue at the point the sequence was Stopped.
      */

      case 0xFB:
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,0,0);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
        Stop.
        Stop the current sequence.
      */

      case 0xFC:
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,0,0);
        MCurrentTrack->events.push_back(midi_event);
        break;

      case 0xFD: // undefined
        break;

      /*
        Use of this message is optional. When initially sent, the receiver will
        expect to receive another Active Sensing message each 300ms (max), or
        it will be assume that the connection has been terminated. At
        termination, the receiver will turn off all voices and return to normal
        (non-active sensing) operation.
      */

      case 0xFE: // active sensing
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,0,0);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
        Reset all receivers in the system to power-up status. This should be
        used sparingly, preferably under manual control. In particular,
        it should not be sent on power-up.
        In a MIDI file this is used as an escape to introduce <meta events>.
      */

      case 0xFF:
        parse_event_meta(AEvent,ADeltaTime);
        break;

      default:
        printf("  0x%02x unknown system event (delta %i)\n",AEvent,ADeltaTime);
        midi_event = new SAT_MidiEvent(ADeltaTime,0,0,0);
        MCurrentTrack->events.push_back(midi_event);
        break;
    } // switch
  }

  //----------

  // meta

  /*

    <meta-event> specifies non-MIDI information useful to this format or to
    sequencers, with this syntax:

    FF <type> <length> <bytes>

    All meta-events begin with FF, then have an event type byte (which is
    always less than 128), and then have the length of the data stored as a
    variable-length quantity, and then the data itself. If there is no data,
    the length is 0. As with chunks, future meta-events may be designed which
    may not be known to existing programs, so programs must properly ignore
    meta-events which they do not recognise, and indeed should expect to see
    them. Programs must never ignore the length of a meta-event which they do
    not recognise, and they shouldn't be surprised if it's bigger than
    expected. If so, they must ignore everything past what they know about.
    However, they must not add anything of their own to the end of the meta-
    event. Sysex events and meta events cancel any running status which was in
    effect. Running status does not apply to and may not be used for these
    messages.

    A few meta-events are defined herein. It is not required for every program
    to support every meta-event.

    In the syntax descriptions for each of the meta-events a set of conventions
    is used to describe parameters of the events. The FF which begins each
    event, the type of each event, and the lengths of events which do not have
    a variable amount of data are given directly in hexadecimal. A notation
    such as dd or se, which consists of two lower-case letters, mnemonically
    represents an 8-bit value. Four identical lower-case letters such as wwww
    mnemonically refer to a 16-bit value, stored most-significant-byte first.
    Six identical lower-case letters such as tttttt refer to a 24-bit value,
    stored most-significant-byte first. The notation len refers to the length
    portion of the meta-event syntax, that is, a number, stored as a variable-
    length quantity, which specifies how many bytes (possibly text) data were
    just specified by the length.

    In general, meta-events in a track which occur at the same time may occur
    in any order. If a copyright event is used, it should be placed as early as
    possible in the file, so it will be noticed easily.
    Sequence Number and Sequence/Track Name events, if present, must appear at
    time 0. An end-of-track event must occur as the last event in the track.

    Meta-event types 01 through 0F are reserved for various types of text
    events, each of which meets the specification of text events (00) but
    is used for a different purpose:
  */

  void parse_event_meta(uint8_t AEvent, uint32_t ADeltaTime) {
    SAT_MidiEvent* midi_event = nullptr;
    uint8_t v1,v2,v3,v4,v5;
    char* ptr = nullptr;

    uint8_t   meta = read_byte();
    uint32_t  length;

    switch(meta) {

      /*
        FF 00 02 Sequence Number

        This optional event, which must occur at the beginning of a track,
        before any nonzero delta-times, and before any transmittable MIDI
        events, specifies the number of a sequence. In a format 2 MIDI File,
        it is used to identify each "pattern" so that a "song" sequence using
        the Cue message can refer to the patterns. If the ID numbers are
        omitted, the sequences' locations in order in the file are used as
        defaults. In a format 0 or 1 MIDI File, which only contain one
        sequence, this number should be contained in the first (or only) track.
        If transfer of several multitrack sequences is required, this must be
        done as a group of format 1 files, each with a different sequence
        number.
      */

      // Sequence Number and Sequence/Track Name events, if present, must appear at time 0

      case 0x00:

        length = read_variable();
        assert(length == 2);
        v1 = read_byte();
        v2 = read_byte();
        //v = (v1<<8) + v2;
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setData(v1,v2);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
        FF 01 len text Text Event
        Any amount of text describing anything. It is a good idea to put a text
        event right at the beginning of a track, with the name of the track,
        a description of its intended orchestration, and any other information
        which the user wants to put there. Text events may also occur at other
        times in a track, to be used as lyrics, or descriptions of cue points.
        The text in this event should be printable ASCII characters for maximum
        interchange. However, other character codes using the high-order bit
        may be used for interchange of files between different programs on the
        same computer which supports an extended character set. Programs on a
        computer which does not support non-ASCII characters should ignore
        those characters.
      */

      case 0x01:
        length = read_variable();
        ptr = (char*)read_data(length);
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setText((uint8_t*)ptr,length);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
        FF 02 len text Copyright Notice
        Contains a copyright notice as printable ASCII text. The notice should
        contain the characters (C), the year of the copyright, and the owner of
        the copyright. If several pieces of music are in the same MIDI File,
        all of the copyright notices should be placed together in this event so
        that it will be at the beginning of the file. This event should be the
        first event in the track chunk, at time 0.
      */

      case 0x02:
        length = read_variable();
        ptr = (char*)read_data(length);
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setText((uint8_t*)ptr,length);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
        FF 03 len text Sequence/Track Name
        If in a format 0 track, or the first track in a format 1 file, the name
        of the sequence. Otherwise, the name of the track.
      */

      // Sequence Number and Sequence/Track Name events, if present, must appear at time 0

      case 0x03:

        length = read_variable();
        ptr = (char*)read_data(length);
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setText((uint8_t*)ptr,length);
        MCurrentTrack->events.push_back(midi_event);
        MCurrentTrack->setName(ptr,length);
        break;

      /*
        FF 04 len text Instrument Name
        A description of the type of instrumentation to be used in that track.
        May be used with the MIDI Prefix meta-event to specify which MIDI
        channel the description applies to, or the channel may be specified as
        text in the event itself.
      */

      case 0x04:
        length = read_variable();
        ptr = (char*)read_data(length);
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setText((uint8_t*)ptr,length);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
        FF 05 len text Lyric
        A lyric to be sung. Generally, each syllable will be a separate lyric
        event which begins at the event's time.
      */

      case 0x05:
        length = read_variable();
        ptr = (char*)read_data(length);
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setText((uint8_t*)ptr,length);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
        FF 06 len text Marker
        Normally in a format 0 track, or the first track in a format 1 file.
        The name of that point in the sequence, such as a rehearsal letter or
        section name ("First Verse", etc.)
      */

      case 0x06:
        length = read_variable();
        ptr = (char*)read_data(length);
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setText((uint8_t*)ptr,length);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
        FF 07 len text Cue Point
        A description of something happening on a film or video screen or stage
        at that point in the musical score ("Car crashes into house", "curtain
        opens", "she slaps his face", etc.)
      */

      case 0x07:
        length = read_variable();
        ptr = (char*)read_data(length);
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setText((uint8_t*)ptr,length);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
      */

      case 0x08:
        length = read_variable();
        ptr = (char*)read_data(length);
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setText((uint8_t*)ptr,length);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
      */

      case 0x09:
        length = read_variable();
        ptr = (char*)read_data(length);
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setText((uint8_t*)ptr,length);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
        FF 20 01 cc MIDI Channel Prefix
        The MIDI channel (0-15) contained in this event may be used to
        associate a MIDI channel with all events which follow, including System
        exclusive and meta-events. This channel is "effective" until the next
        normal MIDI event (which contains a channel) or the next MIDI Channel
        Prefix meta-event. If MIDI channels refer to "tracks", this message may
        be put into a format 0 file, keeping their non-MIDI data associated
        with a track. This capability is also present in Yamaha's ESEQ file
        format.
      */

      //  case TINYSMF_META_TYPE_MIDI_CHANNEL:
      //    ev->cooked.midi_channel = ev->raw[0];
      //    break;

      case 0x20:
        length = read_variable();
        assert(length == 1);
        v1 = read_byte();
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setData(v1);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
        midi port FF 21 01 pp
        pp is a byte specifying the MIDI port (0-127).
      */

      case 0x21:
        length = read_variable();
        assert(length == 1);
        v1 = read_byte();
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setData(v1);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
        FF 2F 00 End of Track
        This event is not optional. It is included so that an exact ending
        point may be specified for the track, so that an exact length is
        defined, which is necessary for tracks which are looped or concatenated
      */

      case 0x2f:
        length = read_variable();
        assert(length == 0);
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        //midi_event->setData((uint8_t*)ptr,length);
        MCurrentTrack->events.push_back(midi_event);
        MEndOfTrack = true;
        break;

      /*
        FF 51 03 tttttt Set Tempo (in microseconds per MIDI quarter-note)
        This event indicates a tempo change. Another way of putting
        "microseconds per quarter-note" is "24ths of a microsecond per MIDI
        clock". Representing tempos as time per beat instead of beat per time
        allows absolutely exact long-term synchronisation with a time-based
        sync protocol such as SMPTE time code or MIDI time code. The amount of
        accuracy provided by this tempo resolution allows a four-minute piece
        at 120 beats per minute to be accurate within 500 usec at the end of
        the piece. Ideally, these events should only occur where MIDI clocks
        would be located -- this convention is intended to guarantee, or at
        least increase the likelihood, of compatibility with other
        synchronisation devices so that a time signature/tempo map stored in
        this format may easily be transferred to another device.
      */


      case 0x51:
        length = read_variable();
        assert(length == 3);
        v1 = read_byte();
        v2 = read_byte();
        v3 = read_byte();
        MSequence->tempo = (v1 << 16) + (v2 << 8) + v3;
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setData(v1,v2,v3);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
        FF 54 05 hr mn se fr ff SMPTE Offset
        This event, if present, designates the SMPTE time at which the track
        chunk is supposed to start. It should be present at the beginning of
        the track, that is, before any nonzero delta-times, and before any
        transmittable MIDI events. the hour must be encoded with the SMPTE
        format, just as it is in MIDI Time Code. In a format 1 file, the SMPTE
        Offset must be stored with the tempo map, and has no meaning in any of
        the other tracks. The ff field contains fractional frames, in 100ths of
        a frame, even in SMPTE-based tracks which specify a different frame
        subdivision for delta-times.
      */

      case 0x54:
        length = read_variable();
        assert(length == 5);
        v1 = read_byte();
        v2 = read_byte();
        v3 = read_byte();
        v4 = read_byte();
        v5 = read_byte();
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setData(v1,v2,v3,v4,v5);
        MCurrentTrack->events.push_back(midi_event);
        //MSequence->smpte_offset =
        break;

      /*
        FF 58 04 nn dd cc bb Time Signature
        The time signature is expressed as four numbers. nn and dd represent
        the numerator and denominator of the time signature as it would be
        notated. The denominator is a negative power of two: 2 represents a
        quarter-note, 3 represents an eighth-note, etc. The cc parameter
        expresses the number of MIDI clocks in a metronome click. The bb
        parameter expresses the number of notated 32nd-notes in a MIDI quarter-
        note (24 MIDI clocks). This was added because there are already
        multiple programs which allow a user to specify that what MIDI thinks
        of as a quarter-note (24 clocks) is to be notated as, or related to in
        terms of, something else.
        Therefore, the complete event for 6/8 time, where the metronome clicks
        every three eighth-notes, but there are 24 clocks per quarter-note, 72
        to the bar, would be (in hex):
        FF 58 04 06 03 24 08
        That is, 6/8 time (8 is 2 to the 3rd power, so this is 06 03), 36 MIDI
        clocks per dotted-quarter (24 hex!), and eight notated 32nd-notes per
        quarter-note.
      */

      case 0x58:
        length = read_variable();
        assert(length == 4);
        v1 = read_byte();
        v2 = read_byte();
        v3 = read_byte();
        v4 = read_byte();
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setData(v1,v2,v3,v4);
        MCurrentTrack->events.push_back(midi_event);
        MSequence->timesig_num = v1;
        MSequence->timesig_denom = v2;
        //MSequence->midi_clocks_in_metronome_click = v3;
        //MSequence->num_notated_32nd_notes_in_midi_qnote_(24_midi_clocks) = v4;
        break;

      /*
        FF 59 02 sf mi Key Signature
        sf = -7: 7 flats
        sf = -1: 1 flat
        sf = 0: key of C
        sf = 1: 1 sharp
        sf = 7: 7 sharps
        mi = 0: major key
        mi = 1: minor key
      */

      case 0x59:
        length = read_variable();
        assert(length == 2);
        v1 = read_byte(); // -7 7 flats 0 key of C 7 7 sharps
        v2 = read_byte();  // 0 major 1 minor
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setData(v1,v2);
        MCurrentTrack->events.push_back(midi_event);
        //MSequence->keysig =
        break;


      /*
        FF 7F len data Sequencer Specific Meta-Event
        Special requirements for particular sequencers may use this event type:
        the first byte or bytes of data is a manufacturer ID (these are one
        byte, or if the first byte is 00, three bytes). As with MIDI System
        Exclusive, manufacturers who define something using this meta-event
        should publish it so that others may be used by a sequencer which
        elects to use this as its only file format; sequencers with their
        established feature-specific formats should probably stick to the
        standard features when using this format.
      */

      case 0x7F:
        length = read_variable();
        ptr = (char*)read_data(length);
        midi_event = new SAT_MidiEvent(ADeltaTime,AEvent,meta,0);
        midi_event->setData((uint8_t*)ptr,length);
        MCurrentTrack->events.push_back(midi_event);
        break;

      /*
      */

      default:
        length = read_variable();
        midi_event = new SAT_MidiEvent(ADeltaTime,0,0,0);
        midi_event->setData((uint8_t*)ptr,length);
        MCurrentTrack->events.push_back(midi_event);
        MBufferPtr += length;
        printf("  0xff%02x %i <..> unknown meta event (delta %i)\n",meta,length,ADeltaTime);
        break;

    } // switch meta
  }

};

//----------------------------------------------------------------------
#endif
