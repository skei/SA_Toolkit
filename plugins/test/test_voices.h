#ifndef sa_test_voices_included
#define sa_test_voices_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_voice_processor.h"
#include "audio/sat_audio_math.h"
//#include "audio/sat_audio_utils.h"

#define MAX_VOICES 32

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_test_voices_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_test_voices/v0",
  .name         = "sa_test_voices",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_INSTRUMENT, nullptr }
};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class sa_test_voices_voice {

//------------------------------
private:
//------------------------------

  uint32_t          MIndex    = 0;
  SAT_VoiceContext* MContext  = nullptr;
  float*            MBuffer   = nullptr;

  double ph     = 0.0;
  double phadd  = 0.0;

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    MIndex = AIndex;
    MContext = AContext;
    MBuffer = MContext->voice_buffer;
    MBuffer += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
  }

  //----------

  uint32_t noteOn(uint32_t AIndex, double AValue) {
    SAT_PRINT("AIndex %i AValue %.3f\n",AIndex,AValue);
    ph = 0.0;
    phadd = SAT_NoteToHz(AIndex) / MContext->sample_rate;
    return SAT_VOICE_PLAYING;
  }

  uint32_t noteOff(uint32_t AIndex, double AValue) {
    SAT_PRINT("AIndex %i AValue %.3f\n",AIndex,AValue);
    return SAT_VOICE_FINISHED;
  }

  void noteChoke(uint32_t AIndex, double AValue) {
    SAT_PRINT("AIndex %i AValue %.3f\n",AIndex,AValue);
  }

  void noteExpression(uint32_t AIndex, double AValue) {
    SAT_PRINT("AIndex %i AValue %.3f\n",AIndex,AValue);
  }

  void parameter(uint32_t AIndex, double AValue) {
    SAT_PRINT("AIndex %i AValue %.3f\n",AIndex,AValue);
  }

  void modulation(uint32_t AIndex, double AValue) {
    SAT_PRINT("AIndex %i AValue %.3f\n",AIndex,AValue);
  }

  //----------

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {
    //SAT_PRINT("AOffset %i ALength %i\n",AOffset,ALength);
    float* buffer = MBuffer + AOffset;
    for (uint32_t i=0; i<ALength; i++) {
      ph = SAT_Fract(ph);
      sat_sample_t s = (ph * 2.0) - 1.0;
      buffer[i] = s * 0.25; // vol 0.25
      ph += phadd;
    }
    return AState;
  }

  uint32_t processSlice(uint32_t AState, uint32_t AOffset) {
    return process(AState,AOffset,SAT_AUDIO_QUANTIZED_SIZE);
  }

  //----------

  sat_sample_t getVoiceLevel() {
    return 1.0;
  }

};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

typedef SAT_VoiceProcessor<sa_test_voices_voice,MAX_VOICES> sa_test_voices_processor;

#if 0

class sa_test_voices_processor
: public SAT_VoiceProcessor<sa_test_voices_voice,MAX_VOICES> {

//------------------------------
public:
//------------------------------

  sa_test_voices_processor(SAT_ProcessorOwner* AOwner)
  : SAT_VoiceProcessor(AOwner) {
    SAT_TRACE;
  }

  //----------

  virtual ~sa_test_voices_processor() {
    SAT_TRACE;
  }

  void process(SAT_ProcessContext* AContext) override {
    const clap_process_t* process = AContext->process;
    float** output = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;
    // SAT_VoiceProcessor::mixActiveVoices(output,length);
    SAT_ClearStereoBuffer(output,length);
    processEvents(process->in_events,process->out_events);
    processAudio(AContext);
    mixActiveVoices(output,length);
  }

};

#endif // 0

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_test_voices_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  sa_test_voices_processor* MProcessor = nullptr;

//------------------------------
public:
//------------------------------

  sa_test_voices_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_test_voices_plugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    registerSynthExtensions();    
    appendClapNoteInputPort("In");
    appendStereoAudioOutputPort("Out");
    uint32_t flags = CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID;
    appendParameter(new SAT_Parameter( "Gain",   "", 1, 0,1, flags ));
    MProcessor = new sa_test_voices_processor(this);
    setProcessor(MProcessor);
    MProcessor->init(getClapPlugin(),getClapHost());
    MProcessor->setProcessThreaded(true);
    return SAT_Plugin::init();
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MProcessor->activate(sample_rate,min_frames_count,max_frames_count);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  bool voice_info_get(clap_voice_info_t *info) override {
    //SAT_TRACE;
    info->voice_count     = MAX_VOICES;
    info->voice_capacity  = MAX_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

  void thread_pool_exec(uint32_t task_index) override {
    MProcessor->threadPoolExec(task_index);
  }

};

//----------------------------------------------------------------------
//
// entry point
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_test_voices_descriptor,sa_test_voices_plugin)
#endif

#undef MAX_VOICES

//----------------------------------------------------------------------
#endif
