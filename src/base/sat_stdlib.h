#ifndef sat_stdlib_included
#define sat_stdlib_included
//----------------------------------------------------------------------

#ifndef SAT_NO_STDLIB

  #include <cassert>
  #ifdef SAT_MAC
    #include <stdlib.h>
  #else
    #include <malloc.h>
  #endif
  #include <math.h>
  #include <memory.h>
  #include <stdarg.h>
  #include <stdint.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <time.h>
  #include <unistd.h>

  #include <array>
  #include <atomic>
  #include <vector>
  //#include <unordered_map>

#else
#endif

//----------------------------------------------------------------------
#endif