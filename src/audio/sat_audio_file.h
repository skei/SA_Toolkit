#ifndef sat_audio_file_included
#define sat_audio_file_included
//----------------------------------------------------------------------


//----------------------------------------------------------------------
#ifdef SAT_WIN32
//----------------------------------------------------------------------

  //TODO.. can we use sndfile in windows?

  struct SF_INFO {
    int64_t	frames;
    int32_t samplerate;
    int32_t channels;
    int32_t format;
    int32_t sections;
    int32_t seekable;
  } ;

  class SAT_AudioFile {
  public:
    SF_INFO* getInfo() { return nullptr; }
    bool open(const char *APath, uint32_t AMode=SAT_AUDIO_FILE_READ, uint32_t ASampleRate=44100, uint32_t AChannels=2) { return true; }
    void close() {}
    int32_t read(uint32_t AChannels, uint32_t ABlockSize, float **ABuffers) { return 0; }
    void write(uint32_t AChannels, uint32_t ABlockSize, float **ABuffers) {}
    void printInfo() {}
  };

//----------------------------------------------------------------------
#else
//----------------------------------------------------------------------

  #include <sndfile.h>

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_AudioFile {

//------------------------------
private:
//------------------------------

  SNDFILE*    MHandle     = nullptr;
  SF_INFO     MInfo       = {0};

  const char* MPath       = nullptr;
  uint32_t    MMode       = SAT_AUDIO_FILE_READ;
  uint32_t    MSapleRate  = 44100;
  uint32_t    MChannels   = 2;


//------------------------------
public:
//------------------------------

  SAT_AudioFile() {
  }

  //----------

  ~SAT_AudioFile() {
  }

  //----------

  SF_INFO* getInfo() {
    return &MInfo;
  }

//------------------------------
public:
//------------------------------

  bool open(const char *APath, uint32_t AMode=SAT_AUDIO_FILE_READ, uint32_t ASampleRate=44100, uint32_t AChannels=2) {

    MPath       = APath;
    MMode       = AMode;
    MSapleRate  = ASampleRate;
    MChannels   = AChannels;

    switch (MMode) {

      case SAT_AUDIO_FILE_READ:

        MHandle = sf_open(APath,SFM_READ,&MInfo);
        if (!MHandle) {
          printf("couldn't open \"%s\": %s\n", MPath, sf_strerror(MHandle));
          return false;
        }
        return true;

      case SAT_AUDIO_FILE_WRITE:

        MInfo.frames     = 0;
        MInfo.samplerate = ASampleRate;
        MInfo.channels   = AChannels;
        MInfo.format     = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
        MInfo.sections   = 0;
        MInfo.seekable   = 0;
        MHandle = sf_open(APath,SFM_WRITE,&MInfo);
        if (!MHandle) {
          printf("couldn't open \"%s\": %s\n", MPath, sf_strerror(MHandle));
          return false;
        }
        sf_command(MHandle,SFC_SET_ADD_PEAK_CHUNK,nullptr,SF_FALSE);
        return true;

      //case SAT_AUDIO_FILE_RDWR:
      //
      //  return false;

    } // switch

    return false;
  }

  //----------

  void close() {
    if (MHandle) sf_close(MHandle);
    MHandle = nullptr;
  }

  //----------

  int32_t read(uint32_t AChannels, uint32_t ABlockSize, float **ABuffers) {
    float read_buf[AChannels];
    uint32_t f;
    for (f=0; f<ABlockSize; f++) {
      if (!sf_readf_float(MHandle,read_buf,1)) break;
      for (uint32_t c=0; c<AChannels; c++) ABuffers[c][f] = read_buf[c];
    }
    uint32_t ret = f;
    for (; f<ABlockSize; f++) {
      for (uint32_t c=0; c<AChannels; c++) {
        ABuffers[c][f] = 0.0f;
      }
    }
    return ret;
  }

  //----------

  void write(uint32_t AChannels, uint32_t ABlockSize, float **ABuffers) {
    float write_buf[AChannels];
    uint32_t f;
    for (f = 0; f < ABlockSize; f++) {
      for (uint32_t c=0; c<AChannels; c++) {
        write_buf[c] = ABuffers[c][f];
      }
      sf_writef_float(MHandle,write_buf,1);
    }
  }

  //----------

  void printInfo() {
    printf("  path:               '%s'\n",MPath);
    printf("  - MInfo.frames      %i\n",(int)MInfo.frames);
    printf("  - MInfo.samplerate  %i\n",MInfo.samplerate);
    printf("  - MInfo.channels    %i\n",MInfo.channels);
    printf("  - MInfo.format      %i\n",MInfo.format);
    printf("  - MInfo.sections    %i\n",MInfo.sections);
    printf("  - MInfo.seekable    %i\n",MInfo.seekable);
    printf("\n");
  }

};

//----------------------------------------------------------------------
#endif // ! WIN32
//----------------------------------------------------------------------
#endif
