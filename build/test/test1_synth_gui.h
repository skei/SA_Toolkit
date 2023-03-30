
#define SAT_PLUGIN_CLAP
#define SAT_PLUGIN_VST3

//

#include "base/sat.h"
#include "audio/sat_audio_math.h"
#include "audio/sat_audio_utils.h"
#include "audio/sat_voice_manager.h"
#include "gui/sat_widgets.h"
#include "plugin/sat_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef SAT_DEBUG
  #define PLUGIN_NAME "myPlugin (debug)"
#else
  #define PLUGIN_NAME "myPlugin"
#endif

#define EDITOR_WIDTH  720 // (50 + 200 + 10 + 200 + 10 + 200 + 50)
#define EDITOR_HEIGHT 450 // (50 + 200 + (5 * (10 + 20)) + 50)
#define EDITOR_SCALE  1.5

const char* buttontext[5] = { "1", "2", "3", "IV", "five" };

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myWidget
: public SAT_MovableWidget {

//------------------------------
private:
//------------------------------

  SAT_Window* MWindow = nullptr;
  
  double x1 = 0.50;
  double y1 = 0.50;
  double x2 = 0.50;
  double y2 = 0.50;
  double x3 = 0.50;
  double y3 = 0.50;
  double x4 = 0.50;
  double y4 = 0.50;

  double x1_add = -0.0012;
  double y1_add = 0.0083;
  double x2_add = 0.007;
  double y2_add = -0.0025;
  double x3_add = 0.003;
  double y3_add = -0.0067;
  double x4_add = -0.0054;
  double y4_add = 0.0039;

//------------------------------
public:
//------------------------------

  myWidget(SAT_Rect ARect)
  : SAT_MovableWidget(ARect) {
  }
  
  //----------

  virtual ~myWidget() {
  }
  
  //----------

  void prepare(SAT_WidgetListener* AWindow, bool ARecursive=true) override {
    SAT_MovableWidget::prepare(AWindow,ARecursive);
    MWindow = (SAT_Window*)AWindow;
    MWindow->registerTimerWidget(this);
  }
  
  //----------

  void on_widget_timer() override {
    //SAT_PRINT;
    redraw();
  };

  //----------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    
    SAT_Painter* painter = AContext->painter;
    double S = getWindowScale();
    SAT_Rect R = getRect();
    R.shrink(S*2.0);
    
    drawDropShadow(AContext);
    fillBackground(AContext);
    
    painter->setDrawColor(SAT_DarkOrange);
    painter->setLineWidth(2.5 * S);
    
    double _x1 = R.x + (x1 * R.w);
    double _y1 = R.y + (y1 * R.h);
    double _x2 = R.x + (x2 * R.w);
    double _y2 = R.y + (y2 * R.h);
    double _x3 = R.x + (x3 * R.w);
    double _y3 = R.y + (y3 * R.h);
    double _x4 = R.x + (x4 * R.w);
    double _y4 = R.y + (y4 * R.h);
    
    painter->drawLine(_x1,_y1,_x2,_y2);
    painter->drawLine(_x2,_y2,_x3,_y3);
    painter->drawLine(_x3,_y3,_x4,_y4);
    painter->drawLine(_x4,_y4,_x1,_y1);
    //painter->drawCurveBezier(_x1,_y1,_x4,_y4,_x2,_y2,_x3,_y3);
    
    drawBorder(AContext);
    
    x1 += x1_add;
    y1 += y1_add;
    x2 += x2_add;
    y2 += y2_add;
    x3 += x3_add;
    y3 += y3_add;
    x4 += x4_add;
    y4 += y4_add;
    
    if (x1 >= 1)  { x1 = 1; x1_add = -x1_add; }
    if (y1 >= 1)  { y1 = 1; y1_add = -y1_add; }
    if (x1 <  0)  { x1 = 0; x1_add = -x1_add; }
    if (y1 <  0)  { y1 = 0; y1_add = -y1_add; }
    
    if (x2 >= 1)  { x2 = 1; x2_add = -x2_add; }
    if (y2 >= 1)  { y2 = 1; y2_add = -y2_add; }
    if (x2 <  0)  { x2 = 0; x2_add = -x2_add; }
    if (y2 <  0)  { y2 = 0; y2_add = -y2_add; }
    
    if (x3 >= 1)  { x3 = 1; x3_add = -x3_add; }
    if (y3 >= 1)  { y3 = 1; y3_add = -y3_add; }
    if (x3 <  0)  { x3 = 0; x3_add = -x3_add; }
    if (y3 <  0)  { y3 = 0; y3_add = -y3_add; }
    
    if (x4 >= 1)  { x4 = 1; x4_add = -x4_add; }
    if (y4 >= 1)  { y4 = 1; y4_add = -y4_add; }
    if (x4 <  0)  { x4 = 0; x4_add = -x4_add; }
    if (y4 <  0)  { y4 = 0; y4_add = -y4_add; }
    
  }

};

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/" PLUGIN_NAME "/" SAT_VERSION,
  .name         = PLUGIN_NAME,
  .vendor       = SAT_VENDOR,
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "...",
  .features     = (const char* []) {
    //CLAP_PLUGIN_FEATURE_AUDIO_EFFECT    
    CLAP_PLUGIN_FEATURE_INSTRUMENT,
    nullptr
  }
};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

#define NUM_VOICES 32

class myVoice {

//------------------------------
private:
//------------------------------

  uint32_t          MIndex    = 0;
  SAT_VoiceContext* MContext  = nullptr;

  float             srate     = 0.0;
  float             ph        = 0.0;
  float             phadd     = 0.0;

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    MIndex = AIndex;
    MContext = AContext;
    srate = AContext->sample_rate;
  }

  //----------

  sat_sample_t getEnvLevel() {
    return 0.0;
  }

  //----------

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {
    float* buffer = MContext->voice_buffer;
    buffer += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
    buffer += AOffset;
    if ((AState == SAT_VOICE_PLAYING) || (AState == SAT_VOICE_RELEASED)) {
      for (uint32_t i=0; i<ALength; i++) {
        ph = SAT_Fract(ph);
        float v = (ph * 2.0) - 1.0;  // 0..1 -> -1..1
        //v = sin(v * SAT_PI2);
        *buffer++ = v * 0.1;
        ph += phadd;
      }
    } // playing
    else {
      memset(buffer,0,ALength * sizeof(float));
    }
    return AState;
  }

  //----------

  uint32_t processSlice(uint32_t AState, uint32_t AOffset) {
    return process(AState,AOffset,SAT_AUDIO_QUANTIZED_SIZE);
  }

  //----------

  uint32_t noteOn(uint32_t AKey, double AVelocity) {
    //SAT_Print("\n");
    ph = 0.0;
    float hz = SAT_NoteToHz(AKey);
    phadd = 1.0 / SAT_HzToSamples(hz,srate);
    return SAT_VOICE_PLAYING;
  }

  //----------

  uint32_t noteOff(uint32_t AKey, double AVelocity) {
    //SAT_Print("\n");
    return SAT_VOICE_FINISHED;
  }

  //----------

  void noteChoke(uint32_t AKey, double AVelocity) {
    //SAT_Print("\n");
  }

  //----------

  void noteExpression(uint32_t AExpression, double AValue) {
    //SAT_Print("\n");
  }

  //----------

  void parameter(uint32_t AIndex, double AValue) {
    //SAT_Print("\n");
  }

  //----------

  void modulation(uint32_t AIndex, double AValue) {
    //SAT_Print("\n");
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
private:
//------------------------------

  SAT_VoiceManager<myVoice,NUM_VOICES>  MVoiceManager = {};
  SAT_PanelWidget*    MRootPanel      = nullptr;
  SAT_VoicesWidget*   MVoicesWidget   = nullptr;
  SAT_WaveformWidget* MWaveformWidget = nullptr;
  
//------------------------------
public:
//------------------------------

  //SAT_PLUGIN_DEFAULT_CONSTRUCTOR(myPlugin);

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
    //SAT_Print("sizeof(myPlugin) : %i\n",sizeof(myPlugin));
  }

  //----------

  //virtual ~myPlugin() {
  //  SAT_Print("yepp, we are being deleted.. ptr: %p\n",this);
  //  //SAT_PRINT;
  //}

//------------------------------
public:
//------------------------------

  bool init() final {
    
    //SAT_GLOBAL.DEBUG.print_callstack();

    //int* ptr = nullptr;
    //int a = *ptr;
    
    SAT_PRINT;
    SAT_Print("Hello world!\n");
    SAT_DPrint( SAT_TERM_RESET SAT_TERM_NORMAL "normal " SAT_TERM_RESET SAT_TERM_BOLD "bold " SAT_TERM_RESET SAT_TERM_FAINT "faint " SAT_TERM_RESET SAT_TERM_ITALICS "italics " SAT_TERM_RESET SAT_TERM_UNDERLINE "underline\n" SAT_TERM_RESET );

    SAT_DPrint( SAT_TERM_FG_RED "hello" SAT_TERM_FG_YELLOW " world2\n" SAT_TERM_RESET);
    
   
    registerDefaultSynthExtensions();
    appendClapNoteInputPort();
    appendStereoOutputPort();
    
    /*SAT_Parameter* par1 =*/ appendParameter( new SAT_Parameter("Param1",0.1) );
    /*SAT_Parameter* par2 =*/ appendParameter( new SAT_Parameter("Param2",0.4) );
      SAT_Parameter* par3 =   appendParameter( new SAT_Parameter("Param3",0.7) );
    par3->setFlag(CLAP_PARAM_IS_MODULATABLE);
    
    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE);
    
    //setProcessThreaded(false);
    //setEventMode(SAT_PLUGIN_EVENT_MODE_BLOCK);
    
    SAT_Host* host = getHost();
    const clap_plugin_t*  clapplugin = getPlugin();
    const clap_host_t* claphost = host->getHost();
    
    MVoiceManager.init(clapplugin,claphost);
    MVoiceManager.setProcessThreaded(false);
    MVoiceManager.setEventMode(SAT_PLUGIN_EVENT_MODE_INTERLEAVED);
    
    return SAT_Plugin::init();
  }

  //----------

  //void destroy() final {
  //  // root widget is not automatically deleted
  //  if (MRootPanel) delete MRootPanel;
  //  SAT_Plugin::destroy();
  //}

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MVoiceManager.activate(sample_rate,min_frames_count,max_frames_count);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  void thread_pool_exec(uint32_t task_index) final {
    MVoiceManager.thread_pool_exec(task_index);
  }

  //----------

  bool voice_info_get(clap_voice_info_t *info) final {
    info->voice_count     = NUM_VOICES;
    info->voice_capacity  = NUM_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

//------------------------------
public:
//------------------------------

  //SAT_Editor* createEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight) final {
  //  //SAT_PRINT;
  //  SAT_Editor* editor = new SAT_Editor(AListener,AWidth,AHeight);
  //  return editor;
  //}

  //----------

  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    //SAT_PRINT;

    // root

    MRootPanel = new SAT_PanelWidget( SAT_Rect(0,0,EDITOR_WIDTH,EDITOR_HEIGHT) );
    AWindow->appendRootWidget(MRootPanel);
    MRootPanel->setFillBackground(true);
    MRootPanel->setDrawBorder(false);
    MRootPanel->setInnerBorder(SAT_Rect(5,5,5,5));

    // menu

    //SAT_MenuWidget* menu = new SAT_MenuWidget(SAT_Rect(260,70,150,150));
    SAT_MenuWidget* menu = new SAT_MenuWidget(SAT_Rect(0,0,150,150));
    menu->setRightClickClose(true);

    SAT_MenuItemWidget* i1 = new SAT_MenuItemWidget(SAT_Rect(5,5,140,15),"Item 1");
    menu->appendChildWidget(i1);
    i1->setDrawBorder(false);

    SAT_MenuItemWidget* i2 = new SAT_MenuItemWidget(SAT_Rect(5,20,140,15),"Item 2");
    menu->appendChildWidget(i2);
    i2->setDrawBorder(false);

    SAT_MenuItemWidget* i3 = new SAT_MenuItemWidget(SAT_Rect(5,35,140,15),"Item 3");
    menu->appendChildWidget(i3);
    i3->setDrawBorder(false);

    // widgets
    
    const char* format = getPluginFormat();
    SAT_PluginHeaderWidget* plugin_header = new SAT_PluginHeaderWidget(SAT_Rect(0,0,EDITOR_WIDTH,40),"toolkit",format);
    MRootPanel->appendChildWidget(plugin_header);

    SAT_TextWidget* text = new SAT_TextWidget(SAT_Rect(50,50,200,20),"Hello world!");
    MRootPanel->appendChildWidget(text);
    text->setTextSize(12);
    text->setFillGradient(true);
    text->setGradientColors(SAT_Grey,SAT_DarkerGrey);
    text->setRoundedCorners(true);
    text->setCornerSize(10);
    
    text->setAlignment(/*SAT_EDGE_LEFT |*/ SAT_EDGE_BOTTOM);
    text->setStretching(SAT_EDGE_LEFT | SAT_EDGE_RIGHT);
    
    SAT_ValueWidget* val = new SAT_ValueWidget(SAT_Rect(50,80,200,20),"Param 1", 0.0);
    MRootPanel->appendChildWidget(val);
    val->setTextSize(12);
    val->setRoundedCorners(true);
    val->setCornerSize(5);

    SAT_DragValueWidget* dragval = new SAT_DragValueWidget(SAT_Rect(50,110,200,20),"Param 2", 0.0);
    MRootPanel->appendChildWidget(dragval);
    dragval->setTextSize(7);
    dragval->setValueSize(7);
    dragval->setSnap(true);
    dragval->setSnapPos(0.5);
    dragval->setAutoHideCursor(false);
    dragval->setAutoLockCursor(true);
    dragval->setRoundedCorners(true);
    dragval->setCornerSize(2);
    
    dragval->setDrawBorder(true);
    dragval->setValueSize(9);
    dragval->setValueColor(SAT_Black);
    dragval->setValueOffset(SAT_Rect(0,0,8,0));
    dragval->setBorderWidth(0.5);
    dragval->setBorderColor(SAT_LightGrey);
    dragval->setDrawDropShadow(true);
    dragval->setDropShadowFeather(6);
    dragval->setDropShadowOffset(1,1);

    SAT_SliderWidget* slider = new SAT_SliderWidget(SAT_Rect(50,140,200,20),"Param 3", 0.0);
    MRootPanel->appendChildWidget(slider);
    slider->setTextSize(12);
    slider->setBipolar(true);
    slider->setBipolarCenter(0.5);
    slider->setSnap(true);
    slider->setSnapPos(0.5);
    slider->setAutoHideCursor(true);
    slider->setAutoLockCursor(true);
    //slider->setModulation(0.25);
    slider->setDrawModulation(true);
    slider->setModulationColor( SAT_Color(1,1,1,0.25) );
    slider->setDragDirection(SAT_DIRECTION_RIGHT);

    SAT_ButtonWidget* button1 = new SAT_ButtonWidget(SAT_Rect(50,170,95,20));
    MRootPanel->appendChildWidget(button1);
    
    SAT_ButtonWidget* button2 = new SAT_ButtonWidget(SAT_Rect(155,170,95,20));
    MRootPanel->appendChildWidget(button2);
    button2->setIsToggle(true);

    //button1->setAlignment(SAT_EDGE_LEFT);
    //button2->setAnchors(SAT_EDGE_BOTTOM);
    
    SAT_TabsWidget* tabs = new SAT_TabsWidget(SAT_Rect(50,200,200,100),3);
    MRootPanel->appendChildWidget(tabs);
    
      SAT_PanelWidget* page1 = new SAT_PanelWidget(SAT_Rect(0,0,200,80));
      SAT_PanelWidget* page2 = new SAT_PanelWidget(SAT_Rect(0,0,200,80));
      SAT_PanelWidget* page3 = new SAT_PanelWidget(SAT_Rect(0,0,200,80));
      page1->setBackgroundColor(SAT_Color(0.40,0.35,0.35));
      page2->setBackgroundColor(SAT_Color(0.35,0.40,0.35));
      page3->setBackgroundColor(SAT_Color(0.35,0.35,0.40));
      tabs->appendPage("page1",page1);
      tabs->appendPage("page2",page2);
      tabs->appendPage("page3",page3);
      tabs->selectPage(0);
    
    MVoicesWidget = new SAT_VoicesWidget(SAT_Rect(50,310,200,10),NUM_VOICES);
    MRootPanel->appendChildWidget(MVoicesWidget);
    
    MWaveformWidget = new SAT_WaveformWidget(SAT_Rect(50,330,200,30));
    MRootPanel->appendChildWidget(MWaveformWidget);
    MWaveformWidget->setNumGrid(4);
    MWaveformWidget->setNumSubGrid(2);

    // column 2

    SAT_SelectorWidget* selector = new SAT_SelectorWidget(SAT_Rect(260,50,200,20),"Select",menu);
    MRootPanel->appendChildWidget(selector);

    SAT_GridWidget* grid = new SAT_GridWidget(SAT_Rect(260,80,200,100),16,8);
    MRootPanel->appendChildWidget(grid);

    SAT_ButtonRowWidget* buttonrow = new SAT_ButtonRowWidget(SAT_Rect(260,190,200,20),5,buttontext,SAT_BUTTON_ROW_MULTI);
    MRootPanel->appendChildWidget(buttonrow);
    buttonrow->setValueIsBits(true,8);
    buttonrow->setRoundedCorners(true);
    buttonrow->setCornerSizes(8,8,8,8);
    

    SAT_KnobWidget* knob = new SAT_KnobWidget(SAT_Rect(260,220,50,50),"%",0.0);
    MRootPanel->appendChildWidget(knob);
    knob->setBipolar(true);
    knob->setBipolarCenter(0.5);
    knob->setSnap(true);
    knob->setSnapPos(0.5);
    knob->setValue(0.25);
    
    SAT_SymbolWidget* symbol1 = new SAT_SymbolWidget(SAT_Rect(260,280,10,10),SAT_SYMBOL_RECT);
    MRootPanel->appendChildWidget(symbol1);

    SAT_SymbolWidget* symbol2 = new SAT_SymbolWidget(SAT_Rect(275,280,10,10),SAT_SYMBOL_TRI_DOWN);
    MRootPanel->appendChildWidget(symbol2);

    SAT_SymbolWidget* symbol3 = new SAT_SymbolWidget(SAT_Rect(290,280,10,10),SAT_SYMBOL_CIRCLE);
    MRootPanel->appendChildWidget(symbol3);

    SAT_SymbolWidget* symbol4 = new SAT_SymbolWidget(SAT_Rect(305,280,10,10),SAT_SYMBOL_FILLED_RECT);
    MRootPanel->appendChildWidget(symbol4);

    SAT_MenuItemWidget* menuitem = new SAT_MenuItemWidget(SAT_Rect(320,220,140,20),"MenuItem");
    MRootPanel->appendChildWidget(menuitem);
    
    SAT_PanelWidget* pp2 = new SAT_PanelWidget(SAT_Rect(320,250,140,50));
    MRootPanel->appendChildWidget(pp2);
    pp2->setDrawBorder(true);
    pp2->setBorderColor(SAT_DarkestGrey);
    pp2->setBorderWidth(0.5);
    pp2->setRoundedCorners(true);
    pp2->setCornerSize(5);
    pp2->setFillBackground(false);
    pp2->setDrawDropShadow(true);
    pp2->setDropShadowFeather(5);
    pp2->setDropShadowOffset(3,3);
    pp2->setDropShadowColors( SAT_Grey, SAT_DarkerGrey );
    //pp2->setDropShadowOffset(1,1);
    pp2->setDropShadowInner(true);

    // column 3

    SAT_GraphWidget* graph = new SAT_GraphWidget(SAT_Rect(470,50,200,230));
    MRootPanel->appendChildWidget(graph);
    graph->setFillBackground(true);
    
    //graph->setAlignment(SAT_EDGE_LEFT);
    graph->setStretching(SAT_EDGE_RIGHT);

    for (uint32_t i=0; i<5; i++) {
      SAT_GraphModule* module = new SAT_GraphModule();
      module->numInputs = 2;
      module->inputs[0] = SAT_PIN_SIGNAL;
      module->outputs[0] = SAT_PIN_SIGNAL;
      module->numOutputs = 2;
      graph->addModule(module,i*10,i*10,"module");
    }
    
    SAT_PanelWidget* ppp = new SAT_PanelWidget(SAT_Rect(470,290,200,100));
    MRootPanel->appendChildWidget(ppp);
    ppp->setFillBackground(true);
    ppp->setFillGradient(true);
    ppp->setGradientColors(SAT_Grey,SAT_DarkerGrey);
    ppp->setDrawBorder(true);
    ppp->setBorderColor(SAT_LightGrey);
    ppp->setBorderWidth(0.5);
    ppp->setRoundedCorners(true);
    ppp->setCornerSize(4);
    ppp->setDrawDropShadow(true);
    ppp->setDropShadowFeather(10);
    //ppp->setDropShadowColors( SAT_Color(0,0,0,0.75), SAT_Color(0,0,0,0) );
    ppp->setDropShadowColors( SAT_DarkestGrey, SAT_DarkGrey );
    ppp->setDropShadowOffset(1,1);
    
    ppp->setAlignment(SAT_EDGE_RIGHT );
    ppp->setStretching(/* SAT_EDGE_RIGHT | */ SAT_EDGE_BOTTOM );
    
    SAT_ScrollBarWidget* scrollbar = new SAT_ScrollBarWidget(SAT_Rect(260,310,200,20));
    MRootPanel->appendChildWidget(scrollbar);
    scrollbar->setValue(0.3);
    //scrollbar->setThumbSize(0.3);
    
    //
    
    // menus etc have to be appended last, because they need to be drawn on
    // top of other widgets (when visible)..

    MRootPanel->appendChildWidget(menu);
    
    //
    
    SAT_MovableWidget* movable = new SAT_MovableWidget(SAT_Rect(260,340,100,50));
    MRootPanel->appendChildWidget(movable);
    //movable->setBackgroundColor(SAT_Color(0.40,0.35,0.35));
    movable->setFillBackground(true);
    movable->setBackgroundColor(SAT_Green2);
    //movable->setFillGradient(true);
    //movable->setGradientColors(SAT_Grey,SAT_DarkerGrey);
    movable->setDrawBorder(true);
    movable->setBorderColor(SAT_LightGrey);
    movable->setBorderWidth(1);
    movable->setRoundedCorners(true);
    movable->setCornerSize(6);
    //movable->setDrawDropShadow(true);
    //movable->setDropShadowFeather(10);
    //movable->setDropShadowOffset(1,1);
    
    //
    
    myWidget* animated = new myWidget(SAT_Rect(370,340,140,80));
    MRootPanel->appendChildWidget(animated);
    AWindow->registerTimerWidget(animated);
    
    //
    
    AEditor->connect(val,     getParameter(0));
    AEditor->connect(dragval, getParameter(1));
    AEditor->connect(slider,  getParameter(2));

    return true;
  }

//------------------------------
public:
//------------------------------

  void do_editor_listener_timer() override {
   
    SAT_Plugin::do_editor_listener_timer();
    
    #ifndef SAT_EXE
    for (uint32_t voice=0; voice<NUM_VOICES; voice++) {
      uint32_t state = MVoiceManager.getVoiceState(voice);
      MVoicesWidget->setVoiceState(voice,state);
    }
    MVoicesWidget->redraw();
    #endif
    
  }

//------------------------------
public:
//------------------------------

  void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    MVoiceManager.postProcessEvents(in_events,out_events);
  }

  //----------

  bool handleNoteOn(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteOn(event);
    return true;
  }

  //----------

  bool handleNoteOff(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteOff(event);
    return true;
  }

  //----------

  bool handleNoteChoke(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteChoke(event);
    return true;
  }

  //----------

  bool handleNoteExpression(const clap_event_note_expression_t* event) final {
    MVoiceManager.handleNoteExpression(event);
    return true;
  }

  //----------

  bool handleParamValue(const clap_event_param_value_t* event) final {
    MVoiceManager.handleParamValue(event);
    return true;
  }

  //----------

  bool handleParamMod(const clap_event_param_mod_t* event) final {
    MVoiceManager.handleParamMod(event);
    return true;
  }

  //----------

  bool handleTransport(const clap_event_transport_t* event) final {
    return false;
  }

  //----------

  bool handleMidi(const clap_event_midi_t* event) final {
    MVoiceManager.handleMidi(event);
    return true;
  }

  //----------

  bool handleMidiSysex(const clap_event_midi_sysex_t* event) final {
    MVoiceManager.handleMidiSysex(event);
    return true;
  }

  //----------

  bool handleMidi2(const clap_event_midi2_t* event) final {
    MVoiceManager.handleMidi2(event);
    return true;
  }

//------------------------------
public:
//------------------------------

  // block

  void processAudio(SAT_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    float** outputs = process->audio_outputs[0].data32;
    AContext->voice_buffer = outputs;
    AContext->voice_length = length;
    MVoiceManager.processAudio(AContext);
    
    sat_param_t scale = getParameterValue(2) + getModulationValue(2);
    scale = SAT_Clamp(scale,0,1);
    
    SAT_ScaleStereoBuffer(outputs,scale,length);
  }

  //----------

  // interleaved

  /*
  void processAudio(SAT_ProcessContext* AContext, uint32_t offset, uint32_t length) final {
    const clap_process_t* process = AContext->process;
    //uint32_t length = process->frames_count;
    float* inputs[2];
    float* outputs[2];
    inputs[0]  = process->audio_inputs[0].data32[0] + offset;
    inputs[1]  = process->audio_inputs[0].data32[1] + offset;
    outputs[0] = process->audio_outputs[0].data32[0] + offset;
    outputs[1] = process->audio_outputs[0].data32[1] + offset;
    SAT_CopyStereoBuffer(outputs,inputs,length);
    sat_param_t scale = getParameterValue(2) + getModulationValue(2);
    scale = SAT_Clamp(scale,0,1);
    SAT_ScaleStereoBuffer(outputs,scale,length);
  }
  */

  //----------

  // quantized

  /*
  void processAudio(SAT_ProcessContext* AContext, uint32_t offset) final {
  }
  */

  };

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY

  #include "plugin/sat_entry.h"

  SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);

  /*
  
  void SAT_Register(SAT_Registry* ARegistry) {
    //SAT_PRINT;
    uint32_t index = ARegistry->getNumDescriptors();
    SAT_Log("SAT_Register -> id %s index %i\n",myDescriptor.id,index);
    ARegistry->registerDescriptor(&myDescriptor);
  }

  const clap_plugin_t* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
    SAT_Log("SAT_CreatePlugin (index %i)\n",AIndex);
    if (AIndex == 0) {
      myPlugin* plugin = new myPlugin(ADescriptor,AHost); // deleted in SAT_Plugin.destroy
      return plugin->getPlugin();
    }
    return nullptr;
  }
  
  */

#endif // SAT_NO_ENTRY

//----------------------------------------------------------------------
