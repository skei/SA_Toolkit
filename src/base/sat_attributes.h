#ifndef sat_attributes_included
#define sat_attributes_included
//----------------------------------------------------------------------

#if defined (SAT_LINUX)
  #define __SAT_IMPORT
  #define __SAT_EXPORT __SAT_PUBLIC
  #define __SAT_CDECL
  #define __SAT_STDCALL
  #define __SAT_PUBLIC __attribute__ ((visibility ("default")))
  #define __SAT_LOCAL  __attribute__ ((visibility ("hidden")))
#endif

#if defined (SAT_MAC)
  #define __SAT_IMPORT
  #define __SAT_EXPORT __SAT_PUBLIC
  #define __SAT_CDECL
  #define __SAT_STDCALL
  #define __SAT_PUBLIC __attribute__ ((visibility ("default")))
  #define __SAT_LOCAL  __attribute__ ((visibility ("hidden")))
#endif

#if defined (SAT_WIN32)
  #define __SAT_IMPORT  __attribute__ ((dllimport))
  #define __SAT_EXPORT  __attribute__ ((dllexport))
  #define __SAT_CDECL   __attribute__ ((cdecl))
  #define __SAT_STDCALL __attribute__ ((stdcall))
  #define __SAT_PUBLIC
  #define __SAT_LOCAL
#endif

//------------------------------
// attributes
//------------------------------

#define __SAT_ALIAS(name)         __attribute__ ((alias (name)))
#define __SAT_ALIGNED(x)          __attribute__ ((aligned (x)))
#define __SAT_ALIGNEDMAX          __attribute__ ((aligned))
#define __SAT_ALLOC_ALIGN(x)      __attribute__ ((alloc_align(x)))
#define __SAT_ALLOC_SIZE1(x)      __attribute__ ((alloc_size(x)))
#define __SAT_ALLOC_SIZE2(x,y)    __attribute__ ((alloc_size(x,y)))
#define __SAT_ALWAYS_INLINE       inline __attribute__ ((always_inline))
#define __SAT_ASM_VOLATILE        __asm__ __volatile__
#define __SAT_CLEANUP(x)          __attribute__ ((__cleanup__(x)))
#define __SAT_COLD                __attribute__ ((cold))
#define __SAT_CONSTEXPR           constexpr
#define __SAT_CONSTRUCTOR         __attribute__ ((constructor))
#define __SAT_DEPRECATED          __attribute__ ((deprecated))
#define __SAT_DESTRUCTOR          __attribute__ ((destructor))
#define __SAT_ERROR(message)      __attribute__ ((error(message)))
#define __SAT_EXTERNC             extern "C"
#define __SAT_FLATTEN             __attribute__ ((flatten))
#define __SAT_HOT                 __attribute__ ((hot))
#define __SAT_HOTINLINE           ___always_inline __hot __optimize(3)
#define __SAT_INIT_PRIORITY(x)    __attribute__ ((init_priority(x)))
#define __SAT_LIKELY(expr)        __builtin_expect(!!(expr),1)
#define __SAT_MALLOC              __attribute__ ((malloc))
#define __SAT_MAY_ALIAS           __attribute__ ((may_alias))
#define __SAT_NO_REORDER          __attribute__ ((no_reorder))
#define __SAT_NOINLINE            __attribute__ ((noinline))
#define __SAT_NORETURN            __attribute__ ((noreturn))
#define __SAT_OPTIMIZE(level)     __attribute__ ((optimize (level)))
#define __SAT_PACKED              __attribute__ ((packed))
#define __SAT_PURE                __attribute__ ((pure))
#define __SAT_SHARED              __attribute__ ((shared))
#define __SAT_SECTION(name)       __attribute__ ((section (name)))
#define __SAT_SELECTANY           __attribute__ ((selectany))
#define __SAT_TARGET(value)       __attribute__ ((target(value)))
#define __SAT_TRANSPARENT_UNION   __attribute__ ((transparent_union))
#define __SAT_UNLIKELY(expr)      __builtin_expect(!!(expr),0)
#define __SAT_USED                __attribute__ ((used))
#define __SAT_UNUSED              __attribute__ ((unused))
#define __SAT_VECTOR(x)           __attribute__ ((vector_size (x)))
#define __SAT_VOLATILE            __volatile__
#define __SAT_VISIBILITY(type)    __attribute__ ((visibility(type)))
#define __SAT_WARNING(message)    __attribute__ ((warning(message)))
#define __SAT_WEAK                __attribute__ ((weak))
#define __SAT_WEAKREF(name)       __attribute__ ((weakref(name)))

//----------------------------------------------------------------------
#endif









