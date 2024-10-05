#ifndef sat_linux_cpu_included
#define sat_linux_cpu_included
//----------------------------------------------------------------------

#include "sat.h"
#include <sys/sysinfo.h>

//----------------------------------------------------------------------
//
// linux
//
//----------------------------------------------------------------------

int32_t SAT_GetNumProcessors() {
  return get_nprocs();    // available processors
  //return get_nprocs_conf() // configured processors
}

//----------

/*
int main() {
  long nprocs = -1;
  long nprocs_max = -1;
  #ifdef _WIN32
    #ifndef _SC_NPROCESSORS_ONLN
      SYSTEM_INFO info;
      GetSystemInfo(&info);
      #define sysconf(a) info.dwNumberOfProcessors
      #define _SC_NPROCESSORS_ONLN
    #endif
  #endif
  #ifdef _SC_NPROCESSORS_ONLN
    nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    if (nprocs < 1) {
      fprintf(stderr, "Could not determine number of CPUs online:\n%s\n", strerror (errno));
      exit (EXIT_FAILURE);
    }
    nprocs_max = sysconf(_SC_NPROCESSORS_CONF);
    if (nprocs_max < 1) {
      fprintf(stderr, "Could not determine number of CPUs configured:\n%s\n", strerror (errno));
      exit (EXIT_FAILURE);
    }
    printf ("%ld of %ld processors online\n",nprocs, nprocs_max);
    exit (EXIT_SUCCESS);
  #else
    fprintf(stderr, "Could not determine number of CPUs");
    exit (EXIT_FAILURE);
  #endif
}
*/

//----------

// int sysinfo (struct sysinfo *__info) __THROW;

// __kernel_long_t uptime;		  // Seconds since boot
// __kernel_ulong_t loads[3];	  // 1, 5, and 15 minute load averages
// __kernel_ulong_t totalram;	  // Total usable main memory size
// __kernel_ulong_t freeram;	  // Available memory size
// __kernel_ulong_t sharedram;	// Amount of shared memory
// __kernel_ulong_t bufferram;	// Memory used by buffers
// __kernel_ulong_t totalswap;	// Total swap space size
// __kernel_ulong_t freeswap;	  // swap space still available
// __u16 procs;		   	          // Number of current processes
// __u16 pad;		   	            // Explicit padding for m68k
// __kernel_ulong_t totalhigh;	// Total high memory size
// __kernel_ulong_t freehigh;	  // Available high memory size
// __u32 mem_unit;			        // Memory unit size in bytes

//----------

uint32_t SAT_GetUptime() {
  struct sysinfo info;
  if (sysinfo(&info)) return info.uptime;
  return 0;
}

//----------------------------------------------------------------------
#endif






#if 0

// https://www.kvraudio.com/forum/viewtopic.php?p=8886124#p8886124

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <x86intrin.h>

int main() {
  uint32_t cw = _mm_getcsr();
  cw &= 0b11111111111111110111111111111111; // FTZ OFF
  //cw |= 0b00000000000000001000000000000000; // FTZ ON
  cw &= 0b11111111111111111111111110111111; // DAZ OFF
  //cw |= 0b00000000000000000000000001000000; // DAZ ON
  _mm_setcsr(cw);

  //float zero = 0.f;
  uint32_t addend = 0b00000000111111111111111111111111;
  uint32_t one = 1;
  //float a = 0.f;
  float a = *(float*)&addend;
  float res = 0.f;
  float disp = 0.f;

  uint32_t c = addend;

  while(addend >= 0) {
    //a = *(float*)&addend;
    //res = zero + a;
    disp = res;
    res = a - (*(float*)&one);
    a = res;
    addend = *(uint32_t*)&res;
    if (res == 0.f) { break; }
    //addend -= 0x00000001U;
    c--;
  }

  printf("%u\n", addend);
  printf("%u\n", c);
  printf("%.25e\n", disp);
  printf("%.25e\n", res);
  printf("%.25e\n", a);
  //system("pause");
  return 0;
}

#endif // 0

//----------------------------------------------------------------------

#if 0

// http://bits.stephan-brumme.com/endianess.html
// see also: SAT_LITTLE_ENDIAN

// -> sat_cpu.h
//bool SAT_IsLittleEndian(void) {
//  uint16_t pattern = 0x0001;
//  return *(char*)&pattern == 0x01;
//}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// #ifdef SAT_WIN32
//   #include <windows.h>
// #endif

// #elif SAT_MAC
//   #include <sys/param.h>
//   #include <sys/sysctl.h>
// #endif

#include <unistd.h>
#include <emmintrin.h>
//----------

int32_t SAT_NumCpuCores(void ) {
  return sysconf(_SC_NPROCESSORS_ONLN);

  //#ifdef SAT_WIN32
  //  SYSTEM_INFO sysinfo;
  //  GetSystemInfo(&sysinfo);
  //  return sysinfo.dwNumberOfProcessors;
  //#endif

  //#ifdef SAT_MAC
  //  int nm[2];
  //  size_t len = 4;
  //  uint32_t count;
  //  nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
  //  sysctl(nm, 2, &count, &len, NULL, 0);
  //  if(count < 1) {
  //    nm[1] = HW_NCPU;
  //    sysctl(nm, 2, &count, &len, NULL, 0);
  //    if(count < 1) { count = 1; }
  //  }
  //  return count;
  //#endif

}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  https://github.com/revivalizer/64klibs/blob/master/base/util/fpustate.h

  RAII FPU state class, sets FTZ and DAZ and rounding, no exceptions
  Adapted from code by mystran @ kvraudio
  http://www.kvraudio.com/forum/viewtopic.php?t=312228&postdays=0&postorder=asc&start=0
*/

class SAT_FpuState {
  private:
    unsigned int sse_control_store;
  public:
    enum Rounding {
      kRoundNearest = 0,
      kRoundNegative,
      kRoundPositive,
      kRoundToZero,
    };
    SAT_FpuState(Rounding mode = kRoundToZero) {
      sse_control_store = _mm_getcsr();
      // bits: 15 = flush to zero | 6 = denormals are zero
      // bitwise-OR with exception masks 12:7 (exception flags 5:0)
      // rounding 14:13, 00 = nearest, 01 = neg, 10 = pos, 11 = to zero
      // The enum above is defined in the same order so just shift it up
      _mm_setcsr(0x8040 | 0x1f80 | ((unsigned int)mode << 13));
    }
    ~SAT_FpuState() {
      // clear exception flags, just in case (probably pointless)
      _mm_setcsr(sse_control_store & (~0x3f));
    }
};


//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// parts of the following code by lubomir i ivanov (for axonlib)
// used with permission

//----------------------------------------------------------------------


// fPIC compatible
#define SAT_CPU_EBX_REG "r"

#define SAT_CPU_EBX_STORE   \
  "pushl %%ebx;"            \
  "cpuid;"                  \
  "movl %%ebx, %1;"         \
  "popl %%ebx;"

//----------

class MIP_CpuInfo {

  private:

    unsigned char isCalled = 0;
    unsigned int  _caps = 0;
    char          cpustringbuf[256];
    unsigned char _SSE3, _SSSE3, _FPU, _CMOV, _SSE, _SSE2, _SSE4A, _SSE5, _MMX,
                  _MMXEXT, _3DNOW, _3DNOWEXT;

  public:

    MIP_CpuInfo(void) {
      isCalled=0;
      _caps=0;
    }

    //----------

    ~MIP_CpuInfo(void) {
    }

    //----------

    // get cpu specifiction

    void id(const int fcall=33139, int* eax=NULL, int* ebx=NULL, int* ecx=NULL, int* edx=NULL) {
      if (fcall == 33139)  { // 33139 = some default number
        isCalled = 1;
        int a, b, c, d;
        // -----------------
        // 0x00000001
        __asmv (
          SAT_CPU_EBX_STORE
          : "=a" (a), "=" SAT_CPU_EBX_REG "" (b),
            "=c" (c), "=d" (d) : "a" (0x00000001) : "cc"
        );
        _SSE3   = SBitGet(c, 0);
        _SSSE3  = SBitGet(c, 9);
        _FPU    = SBitGet(d, 0);
        _CMOV   = SBitGet(d, 15);
        _SSE    = SBitGet(d, 25);
        _SSE2   = SBitGet(d, 26);
        // -----------------
        // 0x80000001
        __asmv (
          SAT_CPU_EBX_STORE
          : "=a" (a), "=" SAT_CPU_EBX_REG "" (b),
            "=c" (c), "=d" (d) : "a" (0x80000001) : "cc"
        );
        _SSE4A    = SBitGet(c, 4);
        _SSE5     = SBitGet(c, 11);
        _MMX      = SBitGet(d, 23);
        _MMXEXT   = SBitGet(d, 22);
        _3DNOW    = SBitGet(d, 31);
        _3DNOWEXT = SBitGet(d, 30);
      }
      // user defined call
      // -----------------
      else
        __asmv (
          SAT_CPU_EBX_STORE
          : "=a" (*eax), "=" SAT_CPU_EBX_REG "" (*ebx),
            "=c" (*ecx), "=d" (*edx) : "a" (fcall) : "cc"
        );
    }

    //----------

    // get cpu caps

    uint32_t caps(void) {
      if (!isCalled) id();
      if (_SSE3)      _caps |= 0x0001;
      if (_SSSE3)     _caps |= 0x0002;
      if (_FPU)       _caps |= 0x0004;
      if (_CMOV)      _caps |= 0x0008;
      if (_SSE)       _caps |= 0x0010;
      if (_SSE2)      _caps |= 0x0020;
      if (_SSE4A)     _caps |= 0x0040;
      if (_SSE5)      _caps |= 0x0080;
      if (_MMX)       _caps |= 0x0100;
      if (_MMXEXT)    _caps |= 0x0200;
      if (_3DNOW)     _caps |= 0x0400;
      if (_3DNOWEXT)  _caps |= 0x0800;
      return _caps;
    }

    //----------

    // h_CpuCapsString
    char* capsString(void) {
      if (!isCalled) id();
      cpustringbuf[0] = 0;
      if (_SSE3)     SStrcat(cpustringbuf,(char*)"sse3 "); // space at the end make them easier to string together
      if (_SSSE3)    SStrcat(cpustringbuf,(char*)"ssse3 ");
      if (_FPU)      SStrcat(cpustringbuf,(char*)"fpu ");
      if (_CMOV)     SStrcat(cpustringbuf,(char*)"cmov ");
      if (_SSE)      SStrcat(cpustringbuf,(char*)"sse ");
      if (_SSE2)     SStrcat(cpustringbuf,(char*)"sse2 ");
      if (_SSE4A)    SStrcat(cpustringbuf,(char*)"sse4a ");
      if (_SSE5)     SStrcat(cpustringbuf,(char*)"sse5 ");
      if (_MMX)      SStrcat(cpustringbuf,(char*)"mmx ");
      if (_MMXEXT)   SStrcat(cpustringbuf,(char*)"mmxext ");
      if (_3DNOW)    SStrcat(cpustringbuf,(char*)"3dnow ");
      if (_3DNOWEXT) SStrcat(cpustringbuf,(char*)"3dnowext ");
      return cpustringbuf;
    }

    //----------

    // call rdtsc()

    #ifdef SAT_64BIT
    inline unsigned long rdtsc(void) {

      //unsigned int low, high;
      //__asmv ( "rdtsc;" : "=a" (low), "=d" (high) );
      //return ( (low) | ( (unsigned long)(high) << 32 ) );

      unsigned long val;
      __asmv ( "rdtsc;" : "=A" (val) );
      return val;
      // ^ check if problems occur with 64bit return
    }
    #endif
    #ifdef SAT_32BIT
    inline unsigned long rdtsc(void) {
      unsigned long long val;
      __asmv (
        // ## serialization causes delayed segmentation faults !
        //"cpuid;"
        "rdtsc;"
        //"leal %0, %%ecx;"
        //"movl %%eax, (%%ecx);"
        //"movl %%edx, 4(%%ecx);"
        : "=A" (val)
        //:: "%eax", "%ebx", "%ecx", "%edx"
      );
      return (unsigned long)val;
    }
    #endif

    //----------

};

#endif // 0

