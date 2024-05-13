#ifndef sat_config_included
#define sat_config_included
//----------------------------------------------------------------------



//----------------------------------------------------------------------
//
// 'magic' constants
//
//----------------------------------------------------------------------

#define SAT_VENDOR                            "skei.audio"
#define SAT_URL                               "https://github.com/skei/SA_Toolkit"
#define SAT_VERSION                           "0.0.0"
#define SAT_LOG_DIRECTORY                     "skei.audio"
#define SAT_LOG_NAME                          "LOG"

#define SAT_AUDIO_QUANTIZED_SIZE              32
#define SAT_MAX_NAME_LENGTH                   64
#define SAT_MAX_STRING_LENGTH                 256
#define SAT_MAX_PATH_LENGTH                   512
#define SAT_PAINTER_CLIP_RECT_STACK_SIZE      32
#define SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK 1024
#define SAT_PLUGIN_MAX_MOD_EVENTS_PER_BLOCK   1024
#define SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK   32
#define SAT_RENDERER_MAJOR_VERSION            3
#define SAT_RENDERER_MINOR_VERSION            2
#define SAT_WIDGET_NUM_VALUES                 16
#define SAT_WINDOW_THREAD_KILL                666
#define SAT_WINDOW_TIMER_MS                   20

//----------------------------------------------------------------------
//
// options
//
//----------------------------------------------------------------------

  #define SAT_DEBUG_ASSERT
  #define SAT_DEBUG_BREAKPOINT
  #define SAT_DEBUG_CALLSTACK
  #define SAT_DEBUG_CRASH_HANDLER
//#define SAT_DEBUG_MEMTRACE
//#define SAT_DEBUG_OBSERVER
//#define SAT_DEBUG_PRINT_ALWAYS
//#define SAT_DEBUG_WINDOW

  #define SAT_EDITOR_EMBEDDED
//#define SAT_EDITOR_REMOTE

//#define SAT_GUI_WAYLAND
//#define SAT_GUI_WIN32
//#define SAT_GUI_X11

//#define SAT_INCLUDE_TESTS
//#define SAT_INCLUDE_LOG
//#define SAT_INCLUDE_ANALYTICS

  #define SAT_LOG_THREAD
  #define SAT_LOG_TIME

//#define SAT_NANOVG_PATCH

//#define SAT_NO_STDLIB

//#define SAT_NO_GUI
//#define SAT_NO_PAINTER
//#define SAT_NO_RENDERER
//#define SAT_NO_SURFACE
//#define SAT_NO_WINDOW

//#define SAT_PAINTER_CAIRO
//#define SAT_PAINTER_NANOVG
//#define SAT_PAINTER_WIN32
//#define SAT_PAINTER_X11

//#define SAT_PLUGIN_DEFAULT_EDITOR
//#define SAT_PLUGIN_EXE_NO_MAIN

//#define SAT_PLUGIN_INCLUDE_ARA_FACTORY
//#define SAT_PLUGIN_INCLUDE_INVALIDATION_FACTORY
  #define SAT_PLUGIN_INCLUDE_PLUGIN_FACTORY
//#define SAT_PLUGIN_INCLUDE_PRESET_DISCOVERY_FACTORY
//#define SAT_PLUGIN_INCLUDE_STATE_CONVERTER_FACTORY

//#define SAT_PLUGIN_INCLUDE_DSSI_WRAPPER
//#define SAT_PLUGIN_INCLUDE_EXE_WRAPPER
//#define SAT_PLUGIN_INCLUDE_LADSPA_WRAPPER
//#define SAT_PLUGIN_INCLUDE_LV2_WRAPPER
//#define SAT_PLUGIN_INCLUDE_VST2_WRAPPER
//#define SAT_PLUGIN_INCLUDE_VST3_WRAPPER

//#define SAT_PRINT_SOCKET
  #define SAT_PRINT_THREAD
  #define SAT_PRINT_TIME
//#define SAT_PRINT_PRETTY_FUNCTION

//#define SAT_RENDERER_EGL
//#define SAT_RENDERER_GLX
//#define SAT_RENDERER_WGL

//#define SAT_TESTS_EXIT_ON_FAILURE

//#define SAT_SURFACE_BITMAP
//#define SAT_SURFACE_CAIRO
//#define SAT_SURFACE_NANOVG
//#define SAT_SURFACE_WIN32
//#define SAT_SURFACE_X11

//#define SAT_WINDOW_NO_TIMER_AUTOSTART
//#define SAT_WINDOW_TIMER_REFRESH  

//#define SAT_WINDOW_WAYLAND
//#define SAT_WINDOW_WIN32
//#define SAT_WINDOW_X11

  #define SAT_X11_WAIT_FOR_MAPNOTIFY

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

#undef SAT_PLUGIN_DEFINED

  #ifdef SAT_PLUGIN_CLAP
    #define SAT_PLUGIN_DEFINED
  #endif

  #ifdef SAT_PLUGIN_DSSI
    #define SAT_PLUGIN_DEFINED
  #endif

  #ifdef SAT_PLUGIN_EXE
    #define SAT_PLUGIN_DEFINED
  #endif

  #ifdef SAT_PLUGIN_EXELIB
    #define SAT_PLUGIN_DEFINED
  #endif

  #ifdef SAT_PLUGIN_LADSPA
    #define SAT_PLUGIN_DEFINED
  #endif

  #ifdef SAT_PLUGIN_LV2
    #define SAT_PLUGIN_DEFINED
  #endif

  #ifdef SAT_PLUGIN_VST2
    #define SAT_PLUGIN_DEFINED
  #endif

  #ifdef SAT_PLUGIN_VST3
    #define SAT_PLUGIN_DEFINED
  #endif

#ifndef SAT_PLUGIN_DEFINED
  #define SAT_PLUGIN_CLAP
#endif

#undef SAT_PLUGIN_DEFINED

//----------------------------------------------------------------------
//
// gui
//
//----------------------------------------------------------------------

#undef SAT_GUI_DEFINED

#ifdef SAT_NO_GUI
  #define SAT_GUI_DEFINED
#else
  #ifdef SAT_GUI_WAYLAND
    #define SAT_GUI_DEFINED
  #endif
  #ifdef SAT_GUI_WIN32
    #define SAT_GUI_DEFINED
  #endif
  #ifdef SAT_GUI_X11
    #define SAT_GUI_DEFINED
  #endif
#endif

#ifndef SAT_GUI_DEFINED
  #ifdef SAT_LINUX
    #define SAT_GUI_X11
  #endif
  #ifdef SAT_WIN32
    #define SAT_GUI_WIN32
  #endif
#endif

#undef SAT_GUI_DEFINED

//----------

#ifdef SAT_NO_GUI

  #define SAT_NO_WINDOW
  #define SAT_NO_SURFACE
  #define SAT_NO_RENDERER
  #define SAT_NO_PAINTER
  
#else

  #ifdef SAT_GUI_WAYLAND
    #define SAT_WINDOW_WAYLAND
    #define SAT_SURFACE_WAYLAND
    #define SAT_RENDERER_EGL
    #define SAT_PAINTER_NANOVG
  #endif

  #ifdef SAT_GUI_WIN32
    #define SAT_WINDOW_WIN32
    #define SAT_SURFACE_WIN32
    #define SAT_RENDERER_WGL
    #define SAT_PAINTER_NANOVG
  #endif

  #ifdef SAT_GUI_X11
    #define SAT_WINDOW_X11
    #define SAT_RENDERER_GLX
    #define SAT_SURFACE_NANOVG
    #define SAT_PAINTER_NANOVG
    // #define SAT_RENDERER_X11
    // #define SAT_SURFACE_X11
    // #define SAT_PAINTER_X11
  #endif

#endif


//----------------------------------------------------------------------
#endif
