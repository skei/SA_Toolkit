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
#define SAT_MODMATRIX_MAX_SRC_COUNT               32
#define SAT_MODMATRIX_MAX_DST_COUNT               32
#define SAT_MODMATRIX_MAX_MOD_COUNT               32
#define SAT_PAINTER_CLIP_RECT_STACK_SIZE          256
#define SAT_PARAMETER_NUM_VALUES                  16
#define SAT_PLUGIN_MAX_BLOCK_SIZE                 4096
#define SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK     1024
#define SAT_PLUGIN_MAX_MOD_EVENTS_PER_BLOCK       1024
#define SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK       32
#define SAT_PLUGIN_MAX_NOTE_ENDS_PER_BLOCK        1024
#define SAT_RENDERER_MAJOR_VERSION                3
#define SAT_RENDERER_MINOR_VERSION                2
#define SAT_THREAD_POOL_MAX_THREADS               32
#define SAT_THREAD_POOL_MAX_VOICES                4096
#define SAT_TWEEN_MAX_VALUES                      4
#define SAT_TWEEN_CHAIN_QUEUE_SIZE                32
#define SAT_VOICE_MAX_EVENTS_PER_BLOCK            1024
#define SAT_VOICE_PROCESSOR_MAX_EVENTS_PER_BLOCK  1024
#define SAT_VOICE_PROCESSOR_NUM_THREADS           12
#define SAT_WIDGET_NUM_VALUES                     16
#define SAT_WINDOW_DBL_CLICK_MS                   200
#define SAT_WINDOW_THREAD_KILL                    666
#define SAT_WINDOW_THREAD_TIMER                   333
#define SAT_WINDOW_BUFFER_MIN_SIZE                256
#define SAT_WINDOW_DIRTY_QUEUE_SIZE               1024
#define SAT_WINDOW_TIMER_MS                       20

//----------------------------------------------------------------------
//
// options
//
//----------------------------------------------------------------------

  // enable debugging features
  // defined by compile script

//#define SAT_DEBUG

  // enable SAT_Assert

#define SAT_DEBUG_ASSERT

  // enable SAT_BreakPoint
  
#define SAT_DEBUG_BREAKPOINT

  // enable dumping the call stack when/if crashing

#define SAT_DEBUG_CALLSTACK

  // capture crashes.. print some info, dump call srack..

#define SAT_DEBUG_CRASH_HANDLER

  // enable mem leak tracker..
  // (unreliable in a plugin, so default is disabled)

//#define SAT_DEBUG_MEMTRACE

  // enable debug observers

//#define SAT_DEBUG_OBSERVER

  // enable SAT_PRINT even in non-debug mode

//#define SAT_DEBUG_PRINT_ALWAYS

  // enbald debug window

//#define SAT_DEBUG_WINDOW

  // select embedded (editor reparented to host window)
  // or remote (handle everything yourself) editor

#define SAT_EDITOR_EMBEDDED
//#define SAT_EDITOR_REMOTE

  // include (debug) 'analytics'

//#define SAT_INCLUDE_ANALYTICS

  // include logging

#define SAT_INCLUDE_LOG

  // include unit testing

//#define SAT_INCLUDE_TESTS

  // print thread in when logging

#define SAT_LOG_THREAD

  // print time in when logging

#define SAT_LOG_TIME

  // remove bitmap decoding from SAT_Bitmap

//#define SAT_NO_BITMAP_PNG

  // remove file loading & parsoing from SAT_Bitmap

//#define SAT_NO_BITMAP_PNG_FILE

  // (not used)
  // remove all stdlib stuff..
  // (handle everything yourself!)

//#define SAT_NO_STDLIB

  // compiule wothout a gui
  // removes all traces of gui code!
  // defined by compile script

//#define SAT_NO_GUI

  // set up a default editor if you don't provide one yourself

#define SAT_PLUGIN_DEFAULT_EDITOR

  // don't include a main() function for SAT_PLUGIN_EXE

//#define SAT_PLUGIN_EXE_NO_MAIN

  // select which clap factories to include

//#define SAT_PLUGIN_INCLUDE_CLAP_ARA_FACTORY
//#define SAT_PLUGIN_INCLUDE_CLAP_INVALIDATION_FACTORY
#define SAT_PLUGIN_INCLUDE_CLAP_PLUGIN_FACTORY
//#define SAT_PLUGIN_INCLUDE_CLAP_PRESET_DISCOVERY_FACTORY
//#define SAT_PLUGIN_INCLUDE_CLAP_STATE_CONVERTER_FACTORY

  // disable shell plugin functionality from vst2 wrapper
  // unreliable (need to rethink strategy)..

#define SAT_PLUGIN_VST2_NO_SHELL

  // use vestige header instead of vst2 sdk

//#define SAT_PLUGIN_VST2_VESTIGE

  // send SAT_PRINT output through a socked, so it can be captured
  // in another console with 'nc -U -l -k /tmp/sat.socket'

//#define SAT_PRINT_SOCKET

  // print thread id with SAT_PRINT

#define SAT_PRINT_THREAD

  // print time with SAT_PRINT

#define SAT_PRINT_TIME

  // print function names 'prettified' in SAT_PRINT

//#define SAT_PRINT_PRETTY_FUNCTION

  // unit testing will exit on failing a test

//#define SAT_TESTS_EXIT_ON_FAILURE

  // convert midi events to note events

//#define SAT_VOICE_PROCESSOR_CONVERT_MIDI

  // (try to) process voices using thread pool (clap ext or our own)

#define SAT_VOICE_PROCESSOR_THREADED
#define SAT_VOICE_PROCESSOR_CLAP_THREAD_POOL

  // if events have no recipients (note_id, pck = -1), meaning they're global,
  // send them (individually) to all voices
  /* .. we always do, so not used at the moment.. (see sat_voice_processor.h) .. */

// #define SAT_VOICE_PROCESSOR_SEND_GLOBAL_MODS_TO_ALL_VOICES
// #define SAT_VOICE_PROCESSOR_SEND_GLOBAL_PARAMS_TO_ALL_VOICES

  // draw (and keep around) to a buffer, and copy from it to the screen,
  // instead of drawing directly to the screen (or opewngl backbuffer)
  // 'retained mode' - ish
  // still a few things to do, and no performance testing have been done..

//#define SAT_WINDOW_BUFFERED

  // if this is defined, double click events will be sent to the widgets
  // (in addition to the regular single click events)

#define SAT_WINDOW_SUPPORT_DBL_CLICK

  // autostart the window timer

#define SAT_WINDOW_TIMER_AUTOSTART

  // if this is not defined, updated widgets will not be queued
  // or flushed from the dirty widget queue.. not redrawn..
  // (do we still need this?)

#define SAT_WINDOW_QUEUE_WIDGETS

  // waits for (x11) window to actually appear on the screen
  
#define SAT_X11_WAIT_FOR_MAPNOTIFY

//----------

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
  //#define SAT_PLUGIN
#else
  #define SAT_PLUGIN_CLAP
  #define SAT_PLUGIN
#endif

// #undef SAT_PLUGIN_DEFINED

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
#endif

#ifndef SAT_GUI_DEFINED
  #ifdef SAT_LINUX
    #define SAT_GUI_X11
  #endif
  #ifdef SAT_WIN32
    #define SAT_GUI_WIN32
  #endif
#endif

// #undef SAT_GUI_DEFINED

//----------

#ifdef SAT_NO_GUI

  #define SAT_NO_WINDOW
  #define SAT_NO_SURFACE
  #define SAT_NO_RENDERER
  #define SAT_NO_PAINTER
  #undef  SAT_EDITOR_EMBEDDED
  #undef  SAT_EDITOR_REMOTE
  #undef  SAT_PLUGIN_DEFAULT_EDITOR
  
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
    #define SAT_RENDERER_WGL
    #define SAT_SURFACE_NANOVG
    #define SAT_PAINTER_NANOVG
    //#define SAT_RENDERER_WIN32
    //#define SAT_SURFACE_WIN32
    //#define SAT_PAINTER_WIN32
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
