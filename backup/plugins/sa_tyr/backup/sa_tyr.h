#ifndef sa_tyr_included
#define sa_tyr_included
//----------------------------------------------------------------------

//#define SAT_PLUGIN_CLAP
//#define SAT_PLUGIN_VST3

#include "audio/sat_audio_utils.h"
#include "audio/sat_voice.h"
#include "audio/sat_voice_manager.h"
#include "audio/backup/filters/sat_decimator_filter.h"
#include "plugin/sat_note.h"
#include "plugin/sat_plugin.h"
#include "plugin/sat_editor.h"

//----------------------------------------------------------------------

#define SA_TYR_EDITOR_WIDTH     836
#define SA_TYR_EDITOR_HEIGHT    703
#define SA_TYR_NUM_VOICES       128
#define SA_TYR_PLUGIN_AUTHOR    "skei.audio"
#define SA_TYR_PLUGIN_NAME      "sa_tyr"
#define SA_TYR_PLUGIN_VERSION   "0.3.1"
#define SA_TYR_PLUGIN_WWW       "https://torhelgeskei.com"

// ugh.. offsets, etc should be multiplied by oversampling tii...
#define SA_TYR_OVERSAMPLING     4

//----------

#include "sa_tyr/sa_tyr_parameters.h"
#include "sa_tyr/sa_tyr_voice.h"
#include "sa_tyr/sa_tyr_editor.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

#define CHAR_ARRAY (const char*[])

const clap_plugin_descriptor_t sa_tyr_descriptor = {
  .clap_version  = CLAP_VERSION,
  .id            = SA_TYR_PLUGIN_AUTHOR "/" SA_TYR_PLUGIN_NAME "/" SA_TYR_PLUGIN_VERSION,
  .name          = SA_TYR_PLUGIN_NAME,
  .vendor        = SA_TYR_PLUGIN_AUTHOR,
  .url           = SA_TYR_PLUGIN_WWW,
  .manual_url    = "",
  .support_url   = "",
  .version       = SA_TYR_PLUGIN_VERSION,
  .description   = "",
  //.features      = (const char*[]){ CLAP_PLUGIN_FEATURE_INSTRUMENT, nullptr }
  .features      = CHAR_ARRAY { CLAP_PLUGIN_FEATURE_INSTRUMENT, nullptr }
};

#undef CHAR_ARRAY

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_tyr_plugin
: public SAT_Plugin {

//------------------------------
private:
//------------------------------

  sa_tyr_voice_manager  MVoiceManager       = {};

  SAT_Decimator59Filter MDecimator_l0       = {};
  SAT_Decimator59Filter MDecimator_r0       = {};
  SAT_Decimator59Filter MDecimator_l        = {};
  SAT_Decimator59Filter MDecimator_r        = {};

  __SAT_ALIGNED(SAT_ALIGNMENT_CACHE) float left_buffer[16384] = {0};
  __SAT_ALIGNED(SAT_ALIGNMENT_CACHE) float right_buffer[16384] = {0};
  __SAT_ALIGNED(SAT_ALIGNMENT_CACHE) float* block_buffer[2] = { left_buffer, right_buffer };

//------------------------------
public:
//------------------------------

  sa_tyr_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
    setInitialEditorSize(SA_TYR_EDITOR_WIDTH,SA_TYR_EDITOR_HEIGHT);
    MVoiceManager.setProcessThreaded(true);
    MVoiceManager.setEventMode(SAT_VOICE_EVENT_MODE_INTERLEAVED);
    //MVoiceManager.setEventVoiceSplit(true);
  }

  //----------

  virtual ~sa_tyr_plugin() {
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    bool result = SAT_Plugin::init();
    if (result) {
      appendStereoInput();
      appendStereoOutput();
      appendNoteInput();
      appendNoteOutput();
      for (uint32_t i=0; i<PARAM_COUNT; i++) {
        const char* name = sa_tyr_parameters[i].name;
        double val       = sa_tyr_parameters[i].default_value;
        double minval    = sa_tyr_parameters[i].min_value;
        double maxval    = sa_tyr_parameters[i].max_value;
        uint32_t flags   = sa_tyr_parameters[i].flags;
        SAT_Parameter* parameter = nullptr;
        if (flags & CLAP_PARAM_IS_STEPPED) parameter = new SAT_IntParameter(name,val,minval,maxval);
        else parameter = new SAT_Parameter(name,val,minval,maxval);
        parameter->setFlags(flags);
        appendParameter(parameter);
      }
      setDefaultParameterValues();
      MVoiceManager.init(getClapPlugin(),getClapHost());
    }
    return result;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    uint32_t srate = sample_rate * SA_TYR_OVERSAMPLING;
    SAT_Plugin::activate(srate,min_frames_count,max_frames_count);
    MVoiceManager.activate(srate,min_frames_count,max_frames_count,&MParameters);
    return true;
  }

//------------------------------
public: // gui
//------------------------------

#ifndef SAT_NO_GUI

  bool gui_create(const char *api, bool is_floating) final {
    //SAT_Plugin::gui_create(api,is_floating);
//    MIsEditorOpen = false;
    //MEditor = new sa_tyr_editor(this,MInitialEditorWidth,MInitialEditorHeight,&MParameters,getClapDescriptor());
    //return (MEditor);
    SAT_Editor* editor = new sa_tyr_editor(this,MInitialEditorWidth,MInitialEditorHeight,&MParameters,getClapDescriptor());
    setEditor(editor);
    return (editor);
    //MEditor->setCanResizeEditor(true);
    //MEditor->setResizeProportional(true);
    //MEditor->setProportionalSize(EDITOR_WIDTH,EDITOR_HEIGHT);
    //return true;
  }

  //----------

  void on_editor_timer() final {
    sa_tyr_editor* editor = (sa_tyr_editor*)getEditor();
    //if (MEditor && MEditor->isEditorOpen()) {
    if (editor && editor->isEditorOpen()) {
      //sa_tyr_editor* editor = (sa_tyr_editor*)MEditor;
      editor->timer_update(&MVoiceManager);
    }
    SAT_Plugin::on_editor_timer();

  }


#endif

//------------------------------
public: // voice into
//------------------------------

  bool voice_info_get(clap_voice_info_t *info) final {
    info->voice_count     = SA_TYR_NUM_VOICES;
    info->voice_capacity  = SA_TYR_NUM_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

//------------------------------
public: // thread pool
//------------------------------

  void thread_pool_exec(uint32_t task_index) final {
    MVoiceManager.thread_pool_exec(task_index);
  }

//------------------------------
public: // events
//------------------------------

  void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    MVoiceManager.preProcessEvents(in_events,out_events);
  }

  void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    MVoiceManager.postProcessEvents(in_events,out_events);
  }

  //----------

  void processNoteOn(const clap_event_note_t* event) final {
    MVoiceManager.processNoteOn(event);
  }

  void processNoteOff(const clap_event_note_t* event) final {
    MVoiceManager.processNoteOff(event);
  }

  void processNoteChoke(const clap_event_note_t* event) final {
    MVoiceManager.processNoteChoke(event);
  }

  void processNoteExpression(const clap_event_note_expression_t* event) final {
    MVoiceManager.processNoteExpression(event);
  }

  void processParamValue(const clap_event_param_value_t* event) final {
    MVoiceManager.processParamValue(event);
  }

  void processParamMod(const clap_event_param_mod_t* event) final {
    MVoiceManager.processParamMod(event);
  }

  void processMidi(const clap_event_midi_t* event) final {
    MVoiceManager.processMidi(event);
  }

  void processMidiSysex(const clap_event_midi_sysex_t* event) final {
    MVoiceManager.processMidiSysex(event);
  }

  void processMidi2(const clap_event_midi2_t* event) final {
    MVoiceManager.processMidi2(event);
  }

//------------------------------
public: // audio
//------------------------------

  void processAudioBlock(SAT_ProcessContext* AContext) final {

    float**  output = AContext->process->audio_outputs[0].data32;
    uint32_t length = AContext->process->frames_count;

    SAT_ClearStereoBuffer(output,length);

    // set to defaults in SAT_Plugin.process()

    if (SA_TYR_OVERSAMPLING > 1) {
      //MProcessContext.oversampling = SA_TYR_OVERSAMPLING;
      //MProcessContext.block_buffer = block_buffer;
      //MProcessContext.block_length = length * SA_TYR_OVERSAMPLING;
      SAT_ProcessContext* context = getProcessContext();
      context->oversampling = SA_TYR_OVERSAMPLING;
      context->block_buffer = block_buffer;
      context->block_length = length * SA_TYR_OVERSAMPLING;
    }

    // process the voices

    MVoiceManager.processAudioBlock(AContext);

    // downsample
    // todo.. do better than this!

//    if (SA_TYR_OVERSAMPLING == 1) {
//      //SAT_CopyStereoBuffer(output,block_buffer,block_length);
//    }
//
//    else {

    if (SA_TYR_OVERSAMPLING > 1) {

      float* src0 = left_buffer;
      float* src1 = right_buffer;
      float* dst0 = output[0];
      float* dst1 = output[1];

      if (SA_TYR_OVERSAMPLING == 2) {
        for (uint32_t i=0; i<length; i++) {
          float l0  = *src0++;
          float l1  = *src0++;
          //float l   = (l0 + l1) * 0.5;
          float l   = MDecimator_l.process(l0,l1);
          float r0  = *src1++;
          float r1  = *src1++;
          //float r   = (r0 + r1) * 0.5;
          float r   = MDecimator_r.process(r0,r1);
          *dst0++ = l;
          *dst1++ = r;
        }
      }

      else if (SA_TYR_OVERSAMPLING == 4) {
        for (uint32_t i=0; i<length; i++) {
          float l00 = *src0++;
          float l01 = *src0++;
          float l10 = *src0++;
          float l11 = *src0++;
          float l0  = MDecimator_l0.process(l00,l01);
          //float l1  = MDecimator_l1.process(l10,l11);
          float l1  = MDecimator_l0.process(l10,l11);
          float l   = MDecimator_l.process(l0,l1);
          float r00 = *src1++;
          float r01 = *src1++;
          float r10 = *src1++;
          float r11 = *src1++;
          float r0  = MDecimator_r0.process(r00,r01);
          //float r1  = MDecimator_r1.process(r10,r11);
          float r1  = MDecimator_r0.process(r10,r11);
          float r   = MDecimator_r.process(r0,r1);
          *dst0++ = l;
          *dst1++ = r;
        }
      }

    } // os 1

    double gain     = MParameters[PAR_MASTER_VOL]->getValue();
    double pan      = MParameters[PAR_MASTER_PAN]->getValue();
    double left     = (1.0 - pan) * gain;
    double right    = (0.0 + pan) * gain;

    SAT_ScaleStereoBuffer(output,left,right,length);

  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_DEFAULT_ENTRY(sa_tyr_descriptor,sa_tyr_plugin);
#endif

//----------------------------------------------------------------------
#endif
