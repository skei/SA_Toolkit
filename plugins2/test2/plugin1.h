
#include "plugin/sat_plugin.h"

#include "plugin/processor/sat_block_processor.h"
#include "plugin/processor/sat_interleaved_processor.h"
#include "plugin/processor/sat_quantized_processor.h"
#include "plugin/processor/sat_voice_processor.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "myPlugin-v0",
  .name         = "myPlugin",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "my plugin..",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

class myProcessor
: public SAT_InterleavedProcessor {

//------------------------------
private:
//------------------------------

  sat_param_t   param0 = 0.0;
  sat_param_t   param1 = 0.0;
  sat_param_t   param2 = 0.0;
  //sat_param_t   params[3] = {0,0,0};

//------------------------------
public:
//------------------------------

  myProcessor(SAT_ProcessorOwner* AOwner)
  : SAT_InterleavedProcessor(AOwner) {
  }

  //----------

  virtual ~myProcessor() {
  }

//------------------------------
public:
//------------------------------

  void paramValueEvent(const clap_event_param_value_t* event) final {
    uint32_t id = event->param_id;
    sat_param_t value = event->value;
    //SAT_PRINT("%i = %.3f\n",id,value);
    //params[id] = value;
    switch(id) {
      case 0: param0 = value; break;
      case 1: param1 = value; break;
      case 2: param2 = value; break;
    }
  }

  //----------

  void processStereoSample(sat_sample_t* spl0, sat_sample_t* spl1) final {
    SAT_ParameterArray* parameters = getParameters();
    //*spl0 *= (params[0] * params[2]);
    //*spl1 *= (params[1] * params[2]);
    //sat_param_t param0 = getParameterValue(0);
    //sat_param_t param1 = getParameterValue(1);
    //sat_param_t param2 = getParameterValue(2);
    *spl0 *= (param0 * param2);
    *spl1 *= (param1 * param2);
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public SAT_Plugin {

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~myPlugin() {
  }

//------------------------------
public: // init
//------------------------------

  bool init() final {
    registerDefaultExtensions();
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();
    appendParameter(new SAT_Parameter("Left", "",1.0));
    appendParameter(new SAT_Parameter("Right","",1.0));
    appendParameter(new SAT_Parameter("Gain", "",1.0));
    setProcessor( new myProcessor(this) );
    setInitialEditorSize(640,480,1.0,false);
    return SAT_Plugin::init();
  }

  //----------

  //#if 0
  #ifndef SAT_NO_GUI

    bool setupEditor(SAT_Editor* AEditor) final {
      SAT_TRACE;

      SAT_Window* window = AEditor->getWindow();
      SAT_RootWidget* root = new SAT_RootWidget( window, SAT_Rect() );
      window->setRootWidget(root);

        root->Layout.innerBorder = SAT_Rect(10,10,10,10);

        SAT_VisualWidget* wdg1 = new SAT_VisualWidget(SAT_Rect(10,10,100,50));
        root->appendChild(wdg1);
        wdg1->setBackgroundColor(SAT_Green);

        SAT_TextWidget* text1 = new SAT_TextWidget(SAT_Rect(120,10,100,50));
        root->appendChild(text1);

        SAT_ValueWidget* value1 = new SAT_ValueWidget(SAT_Rect(230,10,100,50));
        root->appendChild(value1);

        SAT_ButtonWidget* button1 = new SAT_ButtonWidget(SAT_Rect(10,70,100,50));
        root->appendChild(button1);
        AEditor->connect(button1,getParameter(0));

        SAT_DragValueWidget* drag1 = new SAT_DragValueWidget(SAT_Rect(120,70,100,50));
        root->appendChild(drag1);
        AEditor->connect(drag1,getParameter(1));

        //SAT_SliderWidget* slider1 = new SAT_SliderWidget(SAT_Rect(100,50)); // (230,70,100,50));
        SAT_SliderWidget* slider1 = new SAT_SliderWidget(SAT_Rect(50,20));
        root->appendChild(slider1);
        AEditor->connect(slider1,getParameter(2));
        slider1->Layout.flags =  SAT_WIDGET_LAYOUT_RIGHT_CENTER;
        slider1->Layout.flags |= SAT_WIDGET_LAYOUT_PERCENT_PARENT;

      return true;
    }

  #endif // gui
  //#endif // 0

  //----------

  // void on_editorListener_timer(SAT_Timer* ATimer, double ADelta) final {
  //   SAT_Plugin::on_editorListener_timer(ATimer,ADelta);
  //   SAT_PRINT("delta %.3f\n",ADelta);
  // }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);

