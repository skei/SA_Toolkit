#ifndef sat_debug_breakpoint_included
#define sat_debug_breakpoint_included
//----------------------------------------------------------------------

#ifdef SAT_DEBUG_BREAKPOINT

  #define SAT_Breakpoint asm("int $0x3\n")
  //#define SAT_Error(name, fmt, ...) fprintf( stderr, "SAT_ERROR " #name ": " fmt "\n", __VA_ARGS__ )

//----------

#else

  #define SAT_Breakpoint {}
  //#define SAT_Error(name, fmt, ...) {}

#endif

//----------------------------------------------------------------------
#endif
