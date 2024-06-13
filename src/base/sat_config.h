#ifndef sat_config_included
#define sat_config_included
//----------------------------------------------------------------------

#define SAT_VENDOR                            "skei.audio"
#define SAT_URL                               "https://github.com/skei/SA_Toolkit"
#define SAT_VERSION                           "2.0.0"
#define SAT_LOG_DIRECTORY                     "skei.audio"
#define SAT_LOG_NAME                          "LOG"

//----------------------------------------------------------------------
//
// 'magic' constants
//
//----------------------------------------------------------------------

#define SAT_AUDIO_QUANTIZED_SIZE                  32
#define SAT_MAX_NAME_LENGTH                       64
#define SAT_MAX_STRING_LENGTH                     256
#define SAT_MAX_PATH_LENGTH                       512
#define SAT_PAINTER_CLIP_RECT_STACK_SIZE          32
#define SAT_PLUGIN_MAX_BLOCK_SIZE                 4096
#define SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK     1024
#define SAT_PLUGIN_MAX_MOD_EVENTS_PER_BLOCK       1024
#define SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK       32
#define SAT_PLUGIN_MAX_NOTE_ENDS_PER_BLOCK        1024
#define SAT_RENDERER_MAJOR_VERSION                3
#define SAT_RENDERER_MINOR_VERSION                2
#define SAT_TWEEN_MAX_VALUES                      4
#define SAT_TWEEN_CHAIN_QUEUE_SIZE                32
#define SAT_VOICE_PROCESSOR_MAX_EVENTS_PER_BLOCK  1024
#define SAT_WIDGET_NUM_VALUES                     16
#define SAT_WINDOW_THREAD_KILL                    666
#define SAT_WINDOW_TIMER_MS                       20

//----------------------------------------------------------------------
//
// options
//
//----------------------------------------------------------------------

//#define SAT_DEBUG
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
//#define SAT_INCLUDE_TESTS
//#define SAT_INCLUDE_ANALYTICS
//#define SAT_INCLUDE_LOG
  #define SAT_LOG_THREAD
  #define SAT_LOG_TIME
  #define SAT_NANOVG_TEXT_PATCH
//#define SAT_NO_BITMAP_PNG
//#define SAT_NO_BITMAP_PNG_FILE
//#define SAT_NO_STDLIB
//#define SAT_NO_GUI
//#define SAT_PLUGIN_DEFAULT_EDITOR
//#define SAT_PLUGIN_EXE_NO_MAIN
//#define SAT_PLUGIN_INCLUDE_CLAP_ARA_FACTORY
//#define SAT_PLUGIN_INCLUDE_CLAP_INVALIDATION_FACTORY
  #define SAT_PLUGIN_INCLUDE_CLAP_PLUGIN_FACTORY
//#define SAT_PLUGIN_INCLUDE_CLAP_PRESET_DISCOVERY_FACTORY
//#define SAT_PLUGIN_INCLUDE_CLAP_STATE_CONVERTER_FACTORY
//#define SAT_PRINT_SOCKET
  #define SAT_PRINT_THREAD
  #define SAT_PRINT_TIME
//#define SAT_PRINT_PRETTY_FUNCTION
//#define SAT_TESTS_EXIT_ON_FAILURE
//#define SAT_VOICE_MANAGER_CONVERT_MIDI
//#define SAT_WINDOW_BUFFERED
//#define SAT_WINDOW_NO_TIMER_AUTOSTART
  #define SAT_WINDOW_TIMER_REFRESH_WIDGETS
  #define SAT_X11_WAIT_FOR_MAPNOTIFY

// automatically (compile script, command line):

//#define SAT_GUI_WAYLAND
//#define SAT_GUI_WIN32
//#define SAT_GUI_X11
//#define SAT_NO_PAINTER
//#define SAT_NO_RENDERER
//#define SAT_NO_SURFACE
//#define SAT_NO_WINDOW
//#define SAT_PAINTER_CAIRO
//#define SAT_PAINTER_NANOVG
//#define SAT_PAINTER_WIN32
//#define SAT_PAINTER_X11
//#define SAT_PLUGIN_CLAP
//#define SAT_PLUGIN_DSSI
//#define SAT_PLUGIN_EXE
//#define SAT_PLUGIN_LADSPA
//#define SAT_PLUGIN_LV2
//#define SAT_PLUGIN_VST2
//#define SAT_PLUGIN_VST3
//#define SAT_RENDERER_EGL
//#define SAT_RENDERER_GLX
//#define SAT_RENDERER_SOFTWARE
//#define SAT_RENDERER_WGL
//#define SAT_RENDERER_WIN32
//#define SAT_RENDERER_X11
//#define SAT_SURFACE_BITMAP
//#define SAT_SURFACE_CAIRO
//#define SAT_SURFACE_NANOVG
//#define SAT_SURFACE_WIN32
//#define SAT_SURFACE_X11
//#define SAT_WINDOW_WAYLAND
//#define SAT_WINDOW_WIN32
//#define SAT_WINDOW_X11

//----------------------------------------------------------------------
//
// debug
//
//----------------------------------------------------------------------

#ifdef SAT_WIN32
  #undef SAT_DEBUG
#endif

//----------

#ifndef SAT_DEBUG
  //#undef SAT_DEBUG_ASSERT
  #undef SAT_DEBUG_BREAKPOINT
  #undef SAT_DEBUG_CALLSTACK
  #undef SAT_DEBUG_CRASH_HANDLER
  #undef SAT_DEBUG_MEMTRACE
  #undef SAT_DEBUG_OBSERVER
  #undef SAT_DEBUG_PRINT_ALWAYS
  #undef SAT_DEBUG_WINDOW
#endif

//----------------------------------------------------------------------
//
// default / plugin
//
//----------------------------------------------------------------------

#undef SAT_PLUGIN_DEFINED

  #ifdef SAT_PLUGIN_CLAP
    #define SAT_PLUGIN
    #define SAT_PLUGIN_DEFINED
    #define SAT_PLUGIN
  #endif

  #ifdef SAT_PLUGIN_DSSI
    #define SAT_PLUGIN_DEFINED
    #define SAT_PLUGIN
  #endif

  #ifdef SAT_PLUGIN_EXE
    #define SAT_PLUGIN_DEFINED
    #define SAT_EXE
  #endif

  #ifdef SAT_PLUGIN_EXELIB
    #define SAT_PLUGIN_DEFINED
    #define SAT_PLUGIN
  #endif

  #ifdef SAT_PLUGIN_LADSPA
    #define SAT_PLUGIN_DEFINED
    #define SAT_PLUGIN
  #endif

  #ifdef SAT_PLUGIN_LV2
    #define SAT_PLUGIN_DEFINED
    #define SAT_PLUGIN
  #endif

  #ifdef SAT_PLUGIN_VST2
    #define SAT_PLUGIN_DEFINED
    #define SAT_PLUGIN
  #endif

  #ifdef SAT_PLUGIN_VST3
    #define SAT_PLUGIN_DEFINED
    #define SAT_PLUGIN
  #endif


#ifdef SAT_PLUGIN_DEFINED
  #define SAT_PLUGIN
#else
  #define SAT_PLUGIN_CLAP
#endif

//#undef SAT_PLUGIN_DEFINED

//----------------------------------------------------------------------
//
// default / gui
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
  #ifdef SAT_GUI_COCOA
    #define SAT_GUI_DEFINED
  #endif
#endif

#ifndef SAT_GUI_DEFINED
  #ifdef SAT_LINUX
    #define SAT_GUI_X11
  #endif
  #ifdef SAT_MAC
    #define SAT_GUI_COCOA
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
  #undef SAT_EDITOR_EMBEDDED
  #undef SAT_EDITOR_REMOTE
  
#else

  #ifdef SAT_GUI_WAYLAND
    #define SAT_WINDOW_WAYLAND
    #define SAT_SURFACE_WAYLAND
    #define SAT_RENDERER_EGL
    #define SAT_PAINTER_NANOVG
    // #define SAT_EDITOR_REMOTE
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
    //#define SAT_RENDERER_X11
    //#define SAT_SURFACE_X11
    //#define SAT_PAINTER_X11
  #endif

#endif


//----------------------------------------------------------------------
#endif
