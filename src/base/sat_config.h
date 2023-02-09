#ifndef sat_config_included
#define sat_config_included
//----------------------------------------------------------------------

#ifdef __gnu_linux__
  #define SAT_LINUX
#endif

//----------

#if defined _WIN32 || defined __CYGWIN__
  #define  SAT_WIN32
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define SAT_VERSION "0.4.0"
#define SAT_VENDOR  "skei.audio"
#define SAT_URL     "https://github.com/skei/SA_Toolkit"

//----------------------------------------------------------------------
// base
//----------------------------------------------------------------------

//TODO
//#define SAT_LOG_FILE_ENABLE

// append to existing log file, or create new
#define SAT_LOG_FILE_APPEND

//--------------------
// debug
//--------------------

// trace memcalls (malloc/new vs free/delete)
//#define SAT_DEBUG_MEMTRACE

// prints through socket.
// output to consile: nc -U -l -k /tmp/sat.socket
//#define SAT_DEBUG_PRINT_SOCKET

// print thread id
//#define SAT_DEBUG_PRINT_THREAD

// print time (in seconds, with microsecond granularity)
//#define SAT_DEBUG_PRINT_TIME

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

// delete plugin ('delete this') in SAT_Plugin.destroy()
// otherwise, override destroy() and do it manually
#define SAT_DELETE_PLUGIN_IN_DESTROY

//----------------------------------------------------------------------
// gui
//----------------------------------------------------------------------

// removes all traces of gui/editor in the plugin
//#define SAT_NO_GUI

// waits for notify event before returning when opening windows..
#define MIP_X11_WAIT_FOR_MAPNOTIFY




//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------





//--------------------
// base
//--------------------

#ifdef SAT_EXE
  #define  SAT_DEBUG_MEMTRACE
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
