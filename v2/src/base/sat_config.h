#ifndef sat_config_included
#define sat_config_included
//----------------------------------------------------------------------

#define SAT_VERSION                             "0.5.0"
#define SAT_VENDOR                              "skei.audio"
#define SAT_URL                                 "https://github.com/skei/SA_Toolkit" // -> SAT2

//----------------------------------------------------------------------
//
// 'magic' values
//
//----------------------------------------------------------------------

//------------------------------
// audio
//------------------------------

#define SAT_AUDIO_QUANTIZED_SIZE                32
#define SAT_MODMATRIX_MAX_DST_COUNT             64
#define SAT_MODMATRIX_MAX_MOD_COUNT             64
#define SAT_MODMATRIX_MAX_SRC_COUNT             64
#define SAT_VOICE_MAX_EVENTS_PER_BLOCK          4096
#define SAT_VOICE_MANAGER_MAX_EVENTS_PER_BLOCK  4096
#define SAT_VOICE_MANAGER_STEAL_VOICES          true

//------------------------------
// base
//------------------------------

#define SAT_DEBUG_MAX_OBSERVABLES               64
#define SAT_MAX_NAME_LENGTH                     256
#define SAT_MAX_PATH_LENGTH                     512

//------------------------------
// plugin
//------------------------------

#define SAT_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH  64
#define SAT_PLUGIN_MAX_BLOCK_SIZE               4096
#define SAT_PLUGIN_MAX_EVENT_SIZE               256
#define SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK     32
#define SAT_PLUGIN_MAX_MOD_EVENTS_PER_BLOCK     4096
#define SAT_PLUGIN_MAX_NOTE_ENDS_PER_BLOCK      1024
#define SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK   4096
#define SAT_PLUGIN_MAX_PARAMETERS               1024
#define SAT_PLUGIN_VST3_MAX_NOTE_IDS            32
#define SAT_PLUGIN_VST3_TIMER_MS                20

//------------------------------
// gui
//------------------------------

//#define SAT_RENDERER_MAJOR_VERSION              3
//#define SAT_RENDERER_MINOR_VERSION              2
//#define SAT_PAINTER_CLIP_RECT_STACK_SIZE        256
#define SAT_WIDGET_MAX_VALUES                   16
#define SAT_WINDOW_DBLCLICK_MS                  250
#define SAT_WINDOW_MAX_DIRTY_WIDGETS            1024
#define SAT_WINDOW_TIMER_MS                     20

//----------------------------------------------------------------------
//
// options / switches
//
//----------------------------------------------------------------------

//------------------------------
// audio
//------------------------------

//------------------------------
// base
//------------------------------

//#define SAT_NO_AUTODELETE
//#define SAT_NO_TESTS

//------------------------------
// plugin
//------------------------------

//#define SAT_PLUGIN_ALL
//#define SAT_PLUGIN_CLAP
//#define SAT_PLUGIN_DSSI
//#define SAT_PLUGIN_EXE
//#define SAT_PLUGIN_LADSPA
//#define SAT_PLUGIN_LV2
//#define SAT_PLUGIN_VST2
//#define SAT_PLUGIN_VST3

//------------------------------
// gui
//------------------------------

//----------------------------------------------------------------------
//
// defaults
//
//----------------------------------------------------------------------

//------------------------------
// audio
//------------------------------

//------------------------------
// base
//------------------------------

//------------------------------
// plugin
//------------------------------

#undef SAT_PLUGIN_FORMAT_DEFINED

//----------

#if defined (SAT_PLUGIN_ALL)

  #define SAT_PLUGIN_FORMAT_DEFINED
  #define SAT_PLUGIN_CLAP
  #define SAT_PLUGIN_DSSI
  #define SAT_PLUGIN_EXE
  #define SAT_PLUGIN_LADSPA
  #define SAT_PLUGIN_LV2
  #define SAT_PLUGIN_VST2
  #define SAT_PLUGIN_VST3

#elif defined (SAT_PLUGIN_CLAP)

  #define SAT_PLUGIN_FORMAT_DEFINED

#elif defined (SAT_PLUGIN_DSSI)

  #define SAT_PLUGIN_FORMAT_DEFINED
  #define SAT_PLUGIN_CLAP

#elif defined (SAT_PLUGIN_EXE)

  #define SAT_PLUGIN_FORMAT_DEFINED
  #define SAT_PLUGIN_CLAP

#elif defined (SAT_PLUGIN_LADSPA)

  #define SAT_PLUGIN_FORMAT_DEFINED
  #define SAT_PLUGIN_CLAP

#elif defined (SAT_PLUGIN_LV2)

  #define SAT_PLUGIN_FORMAT_DEFINED
  #define SAT_PLUGIN_CLAP

#elif defined (SAT_PLUGIN_VST2)

  #define SAT_PLUGIN_FORMAT_DEFINED
  #define SAT_PLUGIN_CLAP

#elif defined (SAT_PLUGIN_VST3)

  #define SAT_PLUGIN_FORMAT_DEFINED
  #define SAT_PLUGIN_CLAP

//#else
//
//  #error no PLUGIN type defined

#endif

//----------

#if !defined (SAT_PLUGIN_FORMAT_DEFINED)

  //#define SAT_PLUGIN_ALL
  #define SAT_PLUGIN_CLAP

#endif

//------------------------------
// gui
//------------------------------

#if defined (SAT_LINUX)

  #define SAT_GUI_X11
  //#define SAT_WINDOW_X11
  //#define SAT_RENDERER_GL
  //#define SAT_PAINTER_NANOVG
  ;

#elif defined (SAT_WIN32)

  #define SAT_GUI_WIN32
  //#define SAT_WINDOW_WIN32
  //#define SAT_RENDERER_GL
  //#define SAT_PAINTER_NANOVG
  ;

#endif

//----------------------------------------------------------------------
#endif
