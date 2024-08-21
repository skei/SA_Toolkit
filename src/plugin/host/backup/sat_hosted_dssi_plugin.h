#if 0

#ifndef sat_hosted_dssi_plugin_included
#define sat_hosted_dssi_plugin_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "host/sat_hosted_plugin.h"

class SAT_HostedDssiPlugin
: public SAT_HostedPlugin {
};

//----------------------------------------------------------------------
#endif

#endif // 0










#if 0

#ifndef sat_dssi_included
#define sat_dssi_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

#define SAT_DSSI_MAX_MIDI_EVENTS   256
#define SAT_DSSI_MAX_NUM_PROGRAMS  1024

#include "audio/wrappers/sat_wrap_ladspa.h"
#include "base/sat_array.h"
#include "extern/dssi/dssi.h"

//----------

//typedef KArray<const DSSI_Program_Descriptor*> SAT_DssiPrograms;

struct SAT_DssiProgram {
//char    name[25];
  char    name[56];   // 56 = 64 - sizeof(int32)*2 -> align/cache, etc..
  uint32  bank;
  uint32  program;
};

//typedef KArray<SAT_DssiProgram> SAT_DssiPrograms;

//----------

class SAT_Dssi
: public SAT_Ladspa {
  protected:
    const DSSI_Descriptor*    MDssiDescriptor;
    DSSI_Descriptor_Function  MGetDssiDescriptor;
    uint32                    MNumMidiEvents;
    snd_seq_event_t           MMidiEvents[SAT_DSSI_MAX_MIDI_EVENTS];
  //SAT_DssiPrograms             MPrograms;
    uint32                    MNumPrograms;
    SAT_DssiProgram              MPrograms[SAT_DSSI_MAX_NUM_PROGRAMS];
  public:
    SAT_Dssi();
    virtual ~SAT_Dssi();
  public:
    virtual bool        load(const char* AFilename) override;
    virtual uint32      countDescriptors(void) override;
    virtual bool        initDescriptor(uint32 AIndex) override;
  public:
    virtual void        initPrograms(void);
    virtual uint32      numPrograms(void);
    virtual const char* getProgramName(uint32 AIndex);
    virtual void        setProgram(uint32 AIndex);
    virtual void        dumpDssiDescriptor(void);
    virtual void        midiEvent(uint32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3);
    virtual void        resetMidi(void);
  public:
    virtual char*       configure(const char *Key, const char *Value);
    virtual const DSSI_Program_Descriptor* get_program(uint32 Index);
    virtual void        set_program(uint32 Bank, uint32 Program);
    virtual int32       get_midi_controller_for_port(uint32 Port);
    virtual void        run_synth(uint32 ANumSamples);
    virtual void        run_synth_adding(uint32 ANumSamples);
  //virtual void        run_multiple_synths(uint32 InstanceCount, LADSPA_Handle* Instances, uint32 ANumSamples) final;
  //virtual void        run_multiple_synths_adding(uint32 InstanceCount, LADSPA_Handle* Instances, uint32 ANumSamples) final;
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

SAT_Dssi::SAT_Dssi()
: SAT_Ladspa() {
  MDssiDescriptor = nullptr;
  MGetDssiDescriptor = nullptr;
  MNumMidiEvents = 0;
  KMemset(MMidiEvents,0,sizeof(MMidiEvents));
}

//----------

SAT_Dssi::~SAT_Dssi() {
}

//----------------------------------------
//
//----------------------------------------

// we don't check for ladspa_descriptor..

bool SAT_Dssi::load(const char* AFilename) {
  //KDTrace("SAT_Dssi::load %s\n",AFilename);
  if (KLibrary::load(AFilename)) {
    MGetDssiDescriptor = (DSSI_Descriptor_Function)getSymbol("dssi_descriptor");
    return (MGetDssiDescriptor);
  }
  return false;
}

//bool SAT_Dssi::init(void) {
//  MGetDssiDescriptor = (DSSI_Descriptor_Function)getLibrarySymbol("dssi_descriptor");
//  return (MGetDssiDescriptor);
//}

//----------

uint32 SAT_Dssi::countDescriptors(void) {
  //KDTrace("SAT_Dssi::countDescriptors\n");
  uint32 num = 0;
  if (MGetDssiDescriptor) {
    const DSSI_Descriptor* desc = MGetDssiDescriptor(num);
    while (desc) {
      num++;
      desc = MGetDssiDescriptor(num);
    }
  }
  return num;
}

//----------

bool SAT_Dssi::initDescriptor(uint32 AIndex) {
  if (MGetDssiDescriptor) {
    MDssiDescriptor = MGetDssiDescriptor(AIndex);
    if (MDssiDescriptor) {
      MLadspaDescriptor = MDssiDescriptor->LADSPA_Plugin;
      return true;
    }
  }
  return false;
}

//----------

//void SAT_Dssi::initPorts(void) {
//}

//----------

void SAT_Dssi::initPrograms(void) {
  const DSSI_Program_Descriptor* prog_descr;
  MNumPrograms = 0;
  prog_descr = get_program(MNumPrograms);
  while (prog_descr) {
    KStrncpy( MPrograms[MNumPrograms].name, prog_descr->Name, 24 );
    MPrograms[MNumPrograms].bank = prog_descr->Bank;
    MPrograms[MNumPrograms].program = prog_descr->Program;
    MNumPrograms++;
    prog_descr = get_program(MNumPrograms);
  }
}

//----------

uint32 SAT_Dssi::numPrograms(void) {
  return MNumPrograms;
}

//----------

const char* SAT_Dssi::getProgramName(uint32 AIndex) {
  return MPrograms[AIndex].name;
}

//----------

void SAT_Dssi::setProgram(uint32 AIndex) {
  uint32 bank = MPrograms[AIndex].bank;
  uint32 program = MPrograms[AIndex].program;
  set_program(bank,program);
}

//----------------------------------------
//
//----------------------------------------

void SAT_Dssi::dumpDssiDescriptor(void) {
  KDTrace("DSSI_Descriptor:\n");
  KDTrace("  api version:                   %i\n",     MDssiDescriptor->DSSI_API_Version);
  KDTrace("  ladspa descriptor:             0x%08x\n", MDssiDescriptor->LADSPA_Plugin);
  KDTrace("  configure:                     0x%08x\n", MDssiDescriptor->configure);
  KDTrace("  get_program:                   0x%08x\n", MDssiDescriptor->get_program);
  KDTrace("  select_program:                0x%08x\n", MDssiDescriptor->select_program);
  KDTrace("  get_midi_controller_for_port:  0x%08x\n", MDssiDescriptor->get_midi_controller_for_port);
  KDTrace("  run_synth:                     0x%08x\n", MDssiDescriptor->run_synth);
  KDTrace("  run_synth_adding:              0x%08x\n", MDssiDescriptor->run_synth_adding);
  KDTrace("  run_multiple_synths:           0x%08x\n", MDssiDescriptor->run_multiple_synths);
  KDTrace("  run_multiple_synths_adding:    0x%08x\n", MDssiDescriptor->run_multiple_synths_adding);
}

//----------

void SAT_Dssi::midiEvent(uint32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
  //KTrace("midi.. offset %i : %i %i %i\n",AOffset,AMsg1,AMsg2,AMsg3);
  if (MNumMidiEvents < SAT_DSSI_MAX_MIDI_EVENTS) {
    uint32 chan = AMsg1 & 0x0f;
    uint32 msg  = AMsg1 & 0xf0;
    switch(msg) {
      case 0x80: // note off
        MMidiEvents[MNumMidiEvents].type                = SND_SEQ_EVENT_NOTEOFF;
        MMidiEvents[MNumMidiEvents].time.tick           = AOffset;
        MMidiEvents[MNumMidiEvents].data.note.channel   = chan;
        MMidiEvents[MNumMidiEvents].data.note.note      = AMsg2;
        MMidiEvents[MNumMidiEvents].data.note.velocity  = AMsg3;
        break;
      case 0x90: // note on
        MMidiEvents[MNumMidiEvents].type                = SND_SEQ_EVENT_NOTEON;
        MMidiEvents[MNumMidiEvents].time.tick           = AOffset;
        MMidiEvents[MNumMidiEvents].data.note.channel   = chan;
        MMidiEvents[MNumMidiEvents].data.note.note      = AMsg2;
        MMidiEvents[MNumMidiEvents].data.note.velocity  = AMsg3;
        break;
      //case 0xa0: // polyphonic key pressure (aftertouch)
      //  break;
      case 0xb0: // control change
        MMidiEvents[MNumMidiEvents].type                  = SND_SEQ_EVENT_CONTROLLER;
        MMidiEvents[MNumMidiEvents].time.tick             = AOffset;
        MMidiEvents[MNumMidiEvents].data.control.channel  = chan;
        MMidiEvents[MNumMidiEvents].data.control.param    = AMsg2;
        MMidiEvents[MNumMidiEvents].data.control.value    = AMsg3;
        break;
      //case 0xc0: // program change
      //  break;
      //case 0xd0: // channel pressure (aftertouch)
      //  break;
      //case 0xe0: // pitch bend
      //  break;
    }
    MNumMidiEvents++;
  }
}

void SAT_Dssi::resetMidi(void) {
  MNumMidiEvents = 0;
  KMemset(MMidiEvents,0,sizeof(MMidiEvents));
}

//----------------------------------------
//
//----------------------------------------

char* SAT_Dssi::configure(const char *Key, const char *Value) {
  //KDTrace("SAT_Dssi::configure %s %s\n",Key,Value);
  if (MInstantiated) {
    if (MDssiDescriptor) {
      if (MDssiDescriptor->configure) {
        return MDssiDescriptor->configure(MLadspaHandle,Key,Value);
      }
    }
  }
  return nullptr;
}

//----------

const DSSI_Program_Descriptor* SAT_Dssi::get_program(uint32 Index) {
  if (MInstantiated) {
    if (MDssiDescriptor) {
      if (MDssiDescriptor->get_program) {
        return MDssiDescriptor->get_program(MLadspaHandle,Index);
      }
    }
  }
  return nullptr;
}

//----------

void SAT_Dssi::set_program(uint32 Bank, uint32 Program) {
  //KDTrace("SAT_Dssi::set_program %i %i\n",Bank,Program);
  if (MInstantiated) {
    if (MDssiDescriptor) {
      if (MDssiDescriptor->select_program) {
        MDssiDescriptor->select_program(MLadspaHandle,Bank,Program);
      }
    }
  }
}

//----------

int32 SAT_Dssi::get_midi_controller_for_port(uint32 Port) {
  if (MInstantiated) {
    if (MDssiDescriptor) {
      if (MDssiDescriptor->get_midi_controller_for_port) {
        return MDssiDescriptor->get_midi_controller_for_port(MLadspaHandle,Port);
      }
    }
  }
  return -1;
}

//----------

void SAT_Dssi::run_synth(uint32 ANumSamples) {
  if (MActivated) {
    if (MDssiDescriptor) {
      if (MDssiDescriptor->run_synth) {
        MDssiDescriptor->run_synth(MLadspaHandle,ANumSamples,MMidiEvents,MNumMidiEvents);
        MNumMidiEvents = 0;
      }
    }
  }
}

//----------

void SAT_Dssi::run_synth_adding(uint32 ANumSamples) {
  if (MActivated) {
    if (MDssiDescriptor) {
      if (MDssiDescriptor->run_synth_adding) {
        MDssiDescriptor->run_synth_adding(MLadspaHandle,ANumSamples,MMidiEvents,MNumMidiEvents);
        MNumMidiEvents = 0;
      }
    }
  }
}

//----------

//void SAT_Dssi::run_multiple_synths(uint32 InstanceCount, LADSPA_Handle* Instances, uint32 ANumSamples) {
//  if (MDssiDescriptor) {
//    if (MDssiDescriptor->run_multiple_synths) {
//      MDssiDescriptor->run_multiple_synths(InstanceCount,Instances,ANumSamples,MMidiEvents,MNumMidiEvents);
//      MNumMidiEvents = 0;
//    }
//  }
//}

//----------

//void SAT_Dssi::run_multiple_synths_adding(uint32 InstanceCount, LADSPA_Handle* Instances, uint32 ANumSamples) {
//  if (MDssiDescriptor) {
//    if (MDssiDescriptor->run_multiple_synths_adding) {
//      MDssiDescriptor->run_multiple_synths_adding(InstanceCount,Instances,ANumSamples,MMidiEvents,MNumMidiEvents);
//      MNumMidiEvents = 0;
//    }
//  }
//}

//----------


//----------------------------------------------------------------------
#endif

#endif // 0