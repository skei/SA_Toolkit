#ifndef sat_config_included
#define sat_config_included
//----------------------------------------------------------------------

#define SAT_VERSION                             "1.0.0"
#define SAT_VENDOR                              "skei.audio"
#define SAT_URL                                 "https://github.com/skei/SA_Toolkit"
#define SAT_LOG_DIRECTORY                       "skei.audio"

// #define SAT_BUILD_COUNT 0 // set by compiler script

//----------------------------------------------------------------------
//
// 'magic' values
//
//----------------------------------------------------------------------

#define SAT_AUDIO_QUANTIZED_SIZE                32
#define SAT_DEBUG_MAX_OBSERVABLES               64
#define SAT_MAX_NAME_LENGTH                     256
#define SAT_MAX_PATH_LENGTH                     512
#define SAT_MODMATRIX_MAX_DST_COUNT             64
#define SAT_MODMATRIX_MAX_MOD_COUNT             64
#define SAT_MODMATRIX_MAX_SRC_COUNT             64
#define SAT_PAINTER_CLIP_RECT_STACK_SIZE        256
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
#define SAT_RENDERER_MAJOR_VERSION              3
#define SAT_RENDERER_MINOR_VERSION              2
#define SAT_VOICE_MAX_EVENTS_PER_BLOCK          4096
#define SAT_VOICE_MANAGER_MAX_EVENTS_PER_BLOCK  4096
#define SAT_VOICE_MANAGER_STEAL_VOICES          true
#define SAT_WIDGET_MAX_VALUES                   16
#define SAT_WINDOW_DBLCLICK_MS                  250
#define SAT_WINDOW_MAX_DIRTY_WIDGETS            1024
#define SAT_WINDOW_TIMER_MS                     20

//----------------------------------------------------------------------
//
// options / switches
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
  #define SAT_DEBUG_PRINT_THREAD
  #define SAT_DEBUG_PRINT_TIME
//#define SAT_DEBUG_PRINT_PRETTY_FUNCTION
//#define SAT_DEBUG_PRINT_SOCKET
//#define SAT_DEBUG_WINDOW

//#define SAT_FAST_MATH_EXTRA_PRECISION

  #define SAT_GUI_DEFAULT_EDITOR

//#define SAT_GUI_NOGUI
//#define SAT_GUI_WAYLAND
//#define SAT_GUI_WIN32
//#define SAT_GUI_X11

  #define SAT_INCLUDE_INTERFACES
  #define SAT_INCLUDE_IMPLEMENTATIONS

//#define SAT_LV2_EXPORT_TTL

//#define SAT_MOUSE_DOUBLECLICK

  #define SAT_NANOVG_PATCH

//#define SAT_NO_AUTODELETE
//#define SAT_NO_ENTRY
//#define SAT_NO_LOG
//#define SAT_NO_STDLIB

//#define SAT_NO_BITMAP_DRAW
//#define SAT_NO_BITMAP_FONT
//#define SAT_NO_BITMAP_GEN
//#define SAT_NO_BITMAP_PNG
//#define SAT_NO_BITMAP_PNG_FILE
//#define SAT_NO_BITMAP_PROC

//#define SAT_PLUGIN_ALL
//#define SAT_PLUGIN_CLAP
//#define SAT_PLUGIN_DSSI
//#define SAT_PLUGIN_EXE
//#define SAT_PLUGIN_LADSPA
//#define SAT_PLUGIN_LV2
//#define SAT_PLUGIN_VST2
//#define SAT_PLUGIN_VST3

//#define SAT_PLUGIN_EXECUTABLE_LIBRARY
//#define SAT_PLUGIN_VST2_FST
//#define SAT_PLUGIN_VST2_VESTIGE
//#define SAT_PLUGIN_VST2_XAYMAR
//#define SAT_PLUGIN_VST2_NO_SHELL
//#define SAT_PLUGIN_VST3_TRAVESTY

  #define SAT_PLUGIN_CONVERT_MIDI_TO_CLAP
  #define SAT_PLUGIN_DELETE_IN_DESTROY
  
//#define SAT_PLUGIN_USE_ARA_FACTORY
//#define SAT_PLUGIN_USE_INVALIDATION_FACTORY
  #define SAT_PLUGIN_USE_PLUGIN_FACTORY
  #define SAT_PLUGIN_USE_PRESET_DISCOVERY_FACTORY

//#define SAT_TESTS
  #define SAT_TESTS_EXIT_ON_FAILURE

//#define SAT_VOICE_MANAGER_CONVERT_MIDI
  #define SAT_VOICE_MANAGER_SEND_GLOBAL_MODS_TO_ALL_VOICES
  #define SAT_VOICE_MANAGER_SEND_GLOBAL_PARAMS_TO_ALL_VOICES

//#define SAT_WAVETABLE_LINEAR_INTERPOLATION

//#define SAT_WIN32_GDIPLUS
//#define SAT_WIN32_INVALIDATE_WHEN_RESIZING

  #define SAT_X11_WAIT_FOR_MAPNOTIFY

//----------------------------------------------------------------------
//
// defaults
//
//----------------------------------------------------------------------

#if defined (SAT_PLUGIN_EXE)
  //#undef SAT_DEBUG_PRINT_SOCKET
  //#define SAT_DEBUG_MEMTRACE
#else
  #define SAT_DEBUG_PRINT_SOCKET
  //#undef SAT_DEBUG_MEMTRACE
#endif

//------------------------------
// debug
//------------------------------

#if defined (SAT_PLUGIN)
  #define SAT_DEBUG_PRINT_SOCKET
#endif

#if defined (SAT_WIN32)
  #undef SAT_DEBUG_CALLSTACK
  #undef SAT_DEBUG_CRASH_HANDLER
  #undef SAT_DEBUG_PRINT_SOCKET
#endif

#if !defined (SAT_DEBUG)
  #undef SAT_DEBUG_ASSERT
  #undef SAT_DEBUG_BREAKPOINT
  #undef SAT_DEBUG_CALLSTACK
  #undef SAT_DEBUG_CRASH_HANDLER
  #undef SAT_DEBUG_MEMTRACE
  #undef SAT_DEBUG_OBSERVER
  #undef SAT_DEBUG_WINDOW
#endif

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
  #define SAT_PLUGIN

#elif defined (SAT_PLUGIN_DSSI)
  #define SAT_PLUGIN_FORMAT_DEFINED
  #define SAT_PLUGIN_CLAP
  #define SAT_PLUGIN

#elif defined (SAT_PLUGIN_EXE)
  #define SAT_PLUGIN_FORMAT_DEFINED
  #define SAT_PLUGIN_CLAP
  #define SAT_EXE

#elif defined (SAT_PLUGIN_LADSPA)
  #define SAT_PLUGIN_FORMAT_DEFINED
  #define SAT_PLUGIN_CLAP
  #define SAT_PLUGIN

#elif defined (SAT_PLUGIN_LV2)
  #define SAT_PLUGIN_FORMAT_DEFINED
  #define SAT_PLUGIN_CLAP
  #define SAT_PLUGIN

#elif defined (SAT_PLUGIN_VST2)
  #define SAT_PLUGIN_FORMAT_DEFINED
  #define SAT_PLUGIN_CLAP
  #define SAT_PLUGIN

#elif defined (SAT_PLUGIN_VST3)
  #define SAT_PLUGIN_FORMAT_DEFINED
  #define SAT_PLUGIN_CLAP
  #define SAT_PLUGIN

//#else
//  #error no PLUGIN type defined

#endif

//----------

#if !defined (SAT_PLUGIN_FORMAT_DEFINED)
  //#define SAT_PLUGIN_ALL
  #define SAT_PLUGIN_CLAP
  #define SAT_PLUGIN
#endif

//#ifdef SAT_PLUGIN
//  #define SAT_DEBUG_PRINT_SOCKET
//#endif

//------------------------------
// gui
//------------------------------

// SAT_GUI_NOGUI
// SAT_GUI_SOFTWARE
// SAT_GUI_WAYLAND
// SAT_GUI_WIN32
// SAT_GUI_X11

//

// SAT_WINDOW_WAYLAND
// SAT_WINDOW_WIN32
// SAT_WINDOW_X11

// SAT_RENDERER_EGL
// SAT_RENDERER_GLX
// SAT_RENDERER_WGL
// SAT_RENDERER_WIN32
// SAT_RENDERER_X11

// SAT_PAINTER_CAIRO
// SAT_PAINTER_NANOVG
// SAT_PAINTER_WIN32
// SAT_PAINTER_X11

//----------

#undef SAT_GUI_FORMAT_DEFINED

//----------

#if defined (SAT_GUI_NOGUI)
  //#define SAT_GUI_WINDOW_NONE
  //#define SAT_GUI_RENDERER_NONE
  //#define SAT_GUI_PAINTER_NONE
  #define SAT_GUI_FORMAT_DEFINED

#elif defined (SAT_GUI_WIN32)
  //#define SAT_GUI_WINDOW_WIN32
  //#define SAT_GUI_RENDERER_WGL
  //#define SAT_GUI_PAINTER_NANOVG
  #define SAT_GUI_FORMAT_DEFINED

#elif defined (SAT_GUI_X11)
  //#define SAT_GUI_WINDOW_X11
  //#define SAT_GUI_RENDERER_GLX
  //#define SAT_GUI_PAINTER_NANOVG
  #define SAT_GUI_FORMAT_DEFINED

#elif defined (SAT_GUI_WAYLAND)
  //#define SAT_GUI_WINDOW_WAYLAND
  //#define SAT_GUI_RENDERER_EGL
  //#define SAT_GUI_PAINTER_NANOVG
  #define SAT_GUI_FORMAT_DEFINED

//#else
//  #error no GUI type defined

#endif

//----------

#if !defined (SAT_GUI_FORMAT_DEFINED)

  #if defined (SAT_LINUX)
    #define SAT_GUI_X11

  #elif defined (SAT_WIN32)
    #define SAT_GUI_WIN32
  #endif

#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


//----------------------------------------------------------------------
#endif
