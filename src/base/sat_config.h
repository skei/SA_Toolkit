#ifndef sat_config_included
#define sat_config_included
//----------------------------------------------------------------------


#define SAT_VERSION "0.4.1"
#define SAT_VENDOR  "skei.audio"
#define SAT_URL     "https://github.com/skei/SA_Toolkit"





//----------------------------------------------------------------------
//
// base
//
//----------------------------------------------------------------------


/*
  append to existing log file, or create new
*/

  #define SAT_LOG_FILE_APPEND

/*
  additional precision (for math approximations)
*/

  //#define SAT_MATH_EXTRA_PRECISION

//--------------------
// debug
//--------------------

/*
  trace memcalls (malloc/new vs free/delete)
*/

  #define SAT_DEBUG_ASSERT

/*
  trace memcalls (malloc/new vs free/delete)
*/

  //#define SAT_DEBUG_MEMTRACE

/*
  SAT_Print outputs through socket.
  output to console: nc -U -l -k /tmp/sat.socket
  automatically defined if compiling as plugin
  automatically undefined if compiling as exe
*/

  //#define SAT_DEBUG_PRINT_SOCKET

/*
  print thread id (in SAT_Print)
*/

  //#define SAT_DEBUG_PRINT_THREAD

/*
  print time (in SAT_Print)
*/

  //#define SAT_DEBUG_PRINT_TIME

/*
  crash handler
*/

  #define SAT_DEBUG_CRASH_HANDLER

/*
  call stack
*/

  #define SAT_DEBUG_CALL_STACK

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------


/*
  delete plugin ('delete this') in SAT_Plugin.destroy()
  otherwise, override destroy() and do it manually
*/

  #define SAT_DELETE_PLUGIN_IN_DESTROY

/*
  parameter changes not meant for a specific voice, will be sent to ALL voices
*/

  #define SAT_VOICE_MANAGER_SEND_GLOBAL_PARAMS_TO_ALL_VOICES

/*
  modulation not meant for a specific voice, will be sent to ALL voices
*/

  #define SAT_VOICE_MANAGER_SEND_GLOBAL_MODS_TO_ALL_VOICES

/*
  convert regular MIDI to note events
*/

  #define SAT_VOICE_MANAGER_CONVERT_MIDI


//----------------------------------------------------------------------
//
// gui
//
//----------------------------------------------------------------------


/*
  removes all traces of gui/editor in the plugin
*/

  //#define SAT_NO_GUI

/*
  waits for notify event before returning when opening windows..
*/

  #define SAT_X11_WAIT_FOR_MAPNOTIFY


//----------------------------------------------------------------------
//
// defaults
//
//----------------------------------------------------------------------



// HACK? FIXME

//--------------------
// base
//--------------------

#ifdef SAT_EXE
  //#define  SAT_DEBUG_MEMTRACE
  #undef SAT_DEBUG_PRINT_SOCKET
#endif

//----------

#ifdef SAT_PLUGIN
  //#undef  SAT_DEBUG_MEMTRACE
  #define SAT_DEBUG_PRINT_SOCKET
#endif

//--------------------
// plugin
//--------------------

/*
  if no plugin format is specificed, we use CLAP for plugins, and EXE for standalone
*/

#undef SAT_PLUGIN_DEFINED

//----------

#ifdef SAT_PLUGIN_CLAP
  #define SAT_PLUGIN_DEFINED
#endif

#ifdef SAT_PLUGIN_EXE
  #define SAT_PLUGIN_DEFINED
#endif

//----------

#ifndef SAT_PLUGIN_DEFINED
  #ifdef SAT_EXE
    #define SAT_PLUGIN_EXE
  #endif
  #ifdef SAT_PLUGIN
    #define SAT_PLUGIN_CLAP
  #endif
#endif

//--------------------
// gui
//--------------------



//----------------------------------------------------------------------
#endif
