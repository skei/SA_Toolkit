#ifndef sat_test_included
#define sat_test_included
//----------------------------------------------------------------------

#ifdef SAT_UNIT_TESTING

/*
  based on utest.h by Neil Henning (sheredom)
  see /include/extern/utest for more info & license, etc..
  and https://github.com/sheredom/utest.h
*/

//----------------------------------------------------------------------


#ifdef _MSC_VER
/*
   Disable warning about not inlining 'inline' functions.
*/
#pragma warning(disable : 4710)

/*
   Disable warning about inlining functions that are not marked 'inline'.
*/
#pragma warning(disable : 4711)

/*
   Disable warning for alignment padding added
*/
#pragma warning(disable : 4820)

#if _MSC_VER > 1900
/*
  Disable warning about preprocessor macros not being defined in MSVC headers.
*/
#pragma warning(disable : 4668)

/*
  Disable warning about no function prototype given in MSVC headers.
*/
#pragma warning(disable : 4255)

/*
  Disable warning about pointer or reference to potentially throwing function.
*/
#pragma warning(disable : 5039)
#endif

#pragma warning(push, 1)
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1920)
typedef __int64 sat_test_int64_t;
typedef unsigned __int64 sat_test_uint64_t;
typedef unsigned __int32 sat_test_uint32_t;
#else
#include <stdint.h>
typedef int64_t sat_test_int64_t;
typedef uint64_t sat_test_uint64_t;
typedef uint32_t sat_test_uint32_t;
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if defined(__cplusplus)
#include <stdexcept>
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#if defined(__cplusplus)
#define SAT_TEST_C_FUNC extern "C"
#else
#define SAT_TEST_C_FUNC
#endif

#define SAT_TEST_TEST_PASSED (0)
#define SAT_TEST_TEST_FAILURE (1)
#define SAT_TEST_TEST_SKIPPED (2)

#if defined(_MSC_VER) || defined(__MINGW64__) || defined(__MINGW32__)

#if defined(__MINGW64__) || defined(__MINGW32__)



#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif

// define SAT_TEST_USE_OLD_QPC before #include "sat_test.h" to use old
// QueryPerformanceCounter
#ifndef SAT_TEST_USE_OLD_QPC

// SKEI: removed pragma warning push/pop
//       changed from Windows.h to windows.h

//#pragma warning(push, 0)
//#include <Windows.h>
//#pragma warning(pop)

#include <windows.h>

typedef LARGE_INTEGER sat_test_large_integer;
#else
// use old QueryPerformanceCounter definitions (not sure is this needed in some
// edge cases or not) on Win7 with VS2015 these extern declaration cause "second
// C linkage of overloaded function not allowed" error
typedef union {
  struct {
    unsigned long LowPart;
    long HighPart;
  } DUMMYSTRUCTNAME;
  struct {
    unsigned long LowPart;
    long HighPart;
  } u;
  sat_test_int64_t QuadPart;
} sat_test_large_integer;

SAT_TEST_C_FUNC __declspec(dllimport) int __stdcall QueryPerformanceCounter(
    sat_test_large_integer *);
SAT_TEST_C_FUNC __declspec(dllimport) int __stdcall QueryPerformanceFrequency(
    sat_test_large_integer *);

#if defined(__MINGW64__) || defined(__MINGW32__)
#pragma GCC diagnostic pop
#endif
#endif




#elif defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) ||    \
    defined(__NetBSD__) || defined(__DragonFly__) || defined(__sun__) ||       \
    defined(__HAIKU__)
/*
   slightly obscure include here - we need to include glibc's features.h, but
   we don't want to just include a header that might not be defined for other
   c libraries like musl. Instead we include limits.h, which we know on all
   glibc distributions includes features.h
*/
#include <limits.h>

#if defined(__GLIBC__) && defined(__GLIBC_MINOR__)
#include <time.h>

#if ((2 < __GLIBC__) || ((2 == __GLIBC__) && (17 <= __GLIBC_MINOR__)))
/* glibc is version 2.17 or above, so we can just use clock_gettime */
#define SAT_TEST_USE_CLOCKGETTIME
#else
#include <sys/syscall.h>
#include <unistd.h>
#endif
#else // Other libc implementations
#include <time.h>
#define SAT_TEST_USE_CLOCKGETTIME
#endif

#elif defined(__APPLE__)
#include <mach/mach_time.h>
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1920)
#define SAT_TEST_PRId64 "I64d"
#define SAT_TEST_PRIu64 "I64u"
#else
#include <inttypes.h>

#define SAT_TEST_PRId64 PRId64
#define SAT_TEST_PRIu64 PRIu64
#endif

#if defined(__cplusplus)
#define SAT_TEST_INLINE inline

#if defined(__clang__)
#define SAT_TEST_INITIALIZER_BEGIN_DISABLE_WARNINGS                               \
  _Pragma("clang diagnostic push")                                             \
      _Pragma("clang diagnostic ignored \"-Wglobal-constructors\"")

#define SAT_TEST_INITIALIZER_END_DISABLE_WARNINGS _Pragma("clang diagnostic pop")
#else
#define SAT_TEST_INITIALIZER_BEGIN_DISABLE_WARNINGS
#define SAT_TEST_INITIALIZER_END_DISABLE_WARNINGS
#endif

#define SAT_TEST_INITIALIZER(f)                                                   \
  struct f##_cpp_struct {                                                      \
    f##_cpp_struct();                                                          \
  };                                                                           \
  SAT_TEST_INITIALIZER_BEGIN_DISABLE_WARNINGS static f##_cpp_struct               \
      f##_cpp_global SAT_TEST_INITIALIZER_END_DISABLE_WARNINGS;                   \
  f##_cpp_struct::f##_cpp_struct()
#elif defined(_MSC_VER)
#define SAT_TEST_INLINE __forceinline

#if defined(_WIN64)
#define SAT_TEST_SYMBOL_PREFIX
#else
#define SAT_TEST_SYMBOL_PREFIX "_"
#endif

#if defined(__clang__)
#define SAT_TEST_INITIALIZER_BEGIN_DISABLE_WARNINGS                               \
  _Pragma("clang diagnostic push")                                             \
      _Pragma("clang diagnostic ignored \"-Wmissing-variable-declarations\"")

#define SAT_TEST_INITIALIZER_END_DISABLE_WARNINGS _Pragma("clang diagnostic pop")
#else
#define SAT_TEST_INITIALIZER_BEGIN_DISABLE_WARNINGS
#define SAT_TEST_INITIALIZER_END_DISABLE_WARNINGS
#endif

#pragma section(".CRT$XCU", read)
#define SAT_TEST_INITIALIZER(f)                                                   \
  static void __cdecl f(void);                                                 \
  SAT_TEST_INITIALIZER_BEGIN_DISABLE_WARNINGS                                     \
  __pragma(comment(linker, "/include:" SAT_TEST_SYMBOL_PREFIX #f "_"))            \
      SAT_TEST_C_FUNC __declspec(allocate(".CRT$XCU")) void(__cdecl *             \
                                                         f##_)(void) = f;      \
  SAT_TEST_INITIALIZER_END_DISABLE_WARNINGS                                       \
  static void __cdecl f(void)
#else
#if defined(__linux__)
#if defined(__clang__)
#if __has_warning("-Wreserved-id-macro")
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#endif

#define __STDC_FORMAT_MACROS 1

#if defined(__clang__)
#if __has_warning("-Wreserved-id-macro")
#pragma clang diagnostic pop
#endif
#endif
#endif

#define SAT_TEST_INLINE inline

#define SAT_TEST_INITIALIZER(f)                                                   \
  static void f(void) __attribute__((constructor));                            \
  static void f(void)
#endif

#if defined(__cplusplus)
#define SAT_TEST_CAST(type, x) static_cast<type>(x)
#define SAT_TEST_PTR_CAST(type, x) reinterpret_cast<type>(x)
#define SAT_TEST_EXTERN extern "C"
#define SAT_TEST_NULL NULL
#else
#define SAT_TEST_CAST(type, x) ((type)(x))
#define SAT_TEST_PTR_CAST(type, x) ((type)(x))
#define SAT_TEST_EXTERN extern
#define SAT_TEST_NULL 0
#endif

#ifdef _MSC_VER
/*
    io.h contains definitions for some structures with natural padding. This is
    uninteresting, but for some reason MSVC's behaviour is to warn about
    including this system header. That *is* interesting
*/
#pragma warning(disable : 4820)
#pragma warning(push, 1)
#include <io.h>
#pragma warning(pop)
#define SAT_TEST_COLOUR_OUTPUT() (_isatty(_fileno(stdout)))
#else
#if defined(__EMSCRIPTEN__)
#include <emscripten/html5.h>
#define SAT_TEST_COLOUR_OUTPUT() false
#else
#include <unistd.h>
#define SAT_TEST_COLOUR_OUTPUT() (isatty(STDOUT_FILENO))
#endif
#endif

static SAT_TEST_INLINE void *sat_test_realloc(void *const pointer, size_t new_size) {
  void *const new_pointer = realloc(pointer, new_size);

  if (SAT_TEST_NULL == new_pointer) {
    free(new_pointer);
  }

  return new_pointer;
}

static SAT_TEST_INLINE sat_test_int64_t sat_test_ns(void) {
#if defined(_MSC_VER) || defined(__MINGW64__) || defined(__MINGW32__)
  sat_test_large_integer counter;
  sat_test_large_integer frequency;
  QueryPerformanceCounter(&counter);
  QueryPerformanceFrequency(&frequency);
  return SAT_TEST_CAST(sat_test_int64_t,
                    (counter.QuadPart * 1000000000) / frequency.QuadPart);
#elif defined(__linux__) && defined(__STRICT_ANSI__)
  return SAT_TEST_CAST(sat_test_int64_t, clock()) * 1000000000 / CLOCKS_PER_SEC;
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) ||    \
    defined(__NetBSD__) || defined(__DragonFly__) || defined(__sun__) ||       \
    defined(__HAIKU__)
  struct timespec ts;
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) &&              \
    !defined(__HAIKU__)
  timespec_get(&ts, TIME_UTC);
#else
  const clockid_t cid = CLOCK_REALTIME;
#if defined(SAT_TEST_USE_CLOCKGETTIME)
  clock_gettime(cid, &ts);
#else
  syscall(SYS_clock_gettime, cid, &ts);
#endif
#endif
  return SAT_TEST_CAST(sat_test_int64_t, ts.tv_sec) * 1000 * 1000 * 1000 + ts.tv_nsec;
#elif __APPLE__
  return SAT_TEST_CAST(sat_test_int64_t, mach_absolute_time());
#elif __EMSCRIPTEN__
  return emscripten_performance_now() * 1000000.0;
#else
#error Unsupported platform!
#endif
}

typedef void (*sat_test_testcase_t)(int *, size_t);

struct sat_test_test_state_s {
  sat_test_testcase_t func;
  size_t index;
  char *name;
};

struct sat_test_state_s {
  struct sat_test_test_state_s *tests;
  size_t tests_length;
  FILE *output;
};

/* extern to the global state sat_test needs to execute */
SAT_TEST_EXTERN struct sat_test_state_s sat_test_state;

#if defined(_MSC_VER)
#define SAT_TEST_WEAK __forceinline
#else
#define SAT_TEST_WEAK __attribute__((weak))
#endif

#if defined(_MSC_VER)
#define SAT_TEST_UNUSED
#else
#define SAT_TEST_UNUSED __attribute__((unused))
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvariadic-macros"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif
#define SAT_TEST_PRINTF(...)                                                      \
  if (sat_test_state.output) {                                                    \
    fprintf(sat_test_state.output, __VA_ARGS__);                                  \
  }                                                                            \
  printf(__VA_ARGS__)
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvariadic-macros"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif

#ifdef _MSC_VER
#define SAT_TEST_SNPRINTF(BUFFER, N, ...) _snprintf_s(BUFFER, N, N, __VA_ARGS__)
#else
#define SAT_TEST_SNPRINTF(...) snprintf(__VA_ARGS__)
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#if defined(__cplusplus)
/* if we are using c++ we can use overloaded methods (its in the language) */
#define SAT_TEST_OVERLOADABLE
#elif defined(__clang__)
/* otherwise, if we are using clang with c - use the overloadable attribute */
#define SAT_TEST_OVERLOADABLE __attribute__((overloadable))
#endif

#if defined(SAT_TEST_OVERLOADABLE)
SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(float f);
SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(float f) {
  SAT_TEST_PRINTF("%f", SAT_TEST_CAST(double, f));
}

SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(double d);
SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(double d) {
  SAT_TEST_PRINTF("%f", d);
}

SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(long double d);
SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(long double d) {
  SAT_TEST_PRINTF("%Lf", d);
}

SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(int i);
SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(int i) {
  SAT_TEST_PRINTF("%d", i);
}

SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(unsigned int i);
SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(unsigned int i) {
  SAT_TEST_PRINTF("%u", i);
}

SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(long int i);
SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(long int i) {
  SAT_TEST_PRINTF("%ld", i);
}

SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(long unsigned int i);
SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(long unsigned int i) {
  SAT_TEST_PRINTF("%lu", i);
}

SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(const void *p);
SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(const void *p) {
  SAT_TEST_PRINTF("%p", p);
}

/*
   long long is a c++11 extension
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) ||              \
    defined(__cplusplus) && (__cplusplus >= 201103L)

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif

SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(long long int i);
SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(long long int i) {
  SAT_TEST_PRINTF("%lld", i);
}

SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void sat_test_type_printer(long long unsigned int i);
SAT_TEST_WEAK SAT_TEST_OVERLOADABLE void
sat_test_type_printer(long long unsigned int i) {
  SAT_TEST_PRINTF("%llu", i);
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#define sat_test_type_printer(val)                                             \
  SAT_TEST_PRINTF(_Generic((val), signed char                                  \
                        : "%d", unsigned char                                  \
                        : "%u", short                                          \
                        : "%d", unsigned short                                 \
                        : "%u", int                                            \
                        : "%d", long                                           \
                        : "%ld", long long                                     \
                        : "%lld", unsigned                                     \
                        : "%u", unsigned long                                  \
                        : "%lu", unsigned long long                            \
                        : "%llu", float                                        \
                        : "%f", double                                         \
                        : "%f", long double                                    \
                        : "%Lf", default                                       \
                        : _Generic((val - val), ptrdiff_t                      \
                                   : "%p", default                             \
                                   : "undef")),                                \
               (val))
#else
/*
   we don't have the ability to print the values we got, so we create a macro
   to tell our users we can't do anything fancy
*/
#define sat_test_type_printer(...) SAT_TEST_PRINTF("undef")
#endif

#ifdef _MSC_VER
#define SAT_TEST_SURPRESS_WARNING_BEGIN                                        \
  __pragma(warning(push)) __pragma(warning(disable : 4127))                    \
      __pragma(warning(disable : 4571)) __pragma(warning(disable : 4130))
#define SAT_TEST_SURPRESS_WARNING_END __pragma(warning(pop))
#else
#define SAT_TEST_SURPRESS_WARNING_BEGIN
#define SAT_TEST_SURPRESS_WARNING_END
#endif

#if defined(__cplusplus) && (__cplusplus >= 201103L)
#define SAT_TEST_AUTO(x) auto
#elif !defined(__cplusplus)

#if defined(__clang__)
/* clang-format off */
/* had to disable clang-format here because it malforms the pragmas */
#define SAT_TEST_AUTO(x)                                                       \
  _Pragma("clang diagnostic push")                                             \
      _Pragma("clang diagnostic ignored \"-Wgnu-auto-type\"") __auto_type      \
          _Pragma("clang diagnostic pop")
/* clang-format on */
#else
#define SAT_TEST_AUTO(x) __typeof__(x + 0)
#endif

#else
#define SAT_TEST_AUTO(x) typeof(x + 0)
#endif

#if defined(__clang__)
#define SAT_TEST_STRNCMP(x, y, size)                                           \
  _Pragma("clang diagnostic push")                                             \
      _Pragma("clang diagnostic ignored \"-Wdisabled-macro-expansion\"")       \
          strncmp(x, y, size) _Pragma("clang diagnostic pop")
#else
#define SAT_TEST_STRNCMP(x, y, size) strncmp(x, y, size)
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define SAT_TEST_SKIP(msg)                                                     \
  do {                                                                         \
    SAT_TEST_PRINTF("   Skipped : '%s'\n", (msg));                             \
    *sat_test_result = SAT_TEST_TEST_SKIPPED;                                  \
    return;                                                                    \
  } while (0)

#if defined(__clang__)
#define SAT_TEST_EXPECT(x, y, cond)                                            \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                         \
    _Pragma("clang diagnostic push")                                           \
        _Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"")     \
            _Pragma("clang diagnostic ignored \"-Wc++98-compat-pedantic\"")    \
                _Pragma("clang diagnostic ignored \"-Wfloat-equal\"")          \
                    SAT_TEST_AUTO(x) xEval = (x);                              \
    SAT_TEST_AUTO(y) yEval = (y);                                              \
    if (!((xEval)cond(yEval))) {                                               \
      _Pragma("clang diagnostic pop")                                          \
          SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);             \
      SAT_TEST_PRINTF("  Expected : (");                                       \
      SAT_TEST_PRINTF(#x ") " #cond " (" #y);                                  \
      SAT_TEST_PRINTF(")\n");                                                  \
      SAT_TEST_PRINTF("    Actual : ");                                        \
      sat_test_type_printer(xEval);                                            \
      SAT_TEST_PRINTF(" vs ");                                                 \
      sat_test_type_printer(yEval);                                            \
      SAT_TEST_PRINTF("\n");                                                   \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END
#elif defined(__GNUC__)
#define SAT_TEST_EXPECT(x, y, cond)                                            \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                         \
    SAT_TEST_AUTO(x) xEval = (x);                                              \
    SAT_TEST_AUTO(y) yEval = (y);                                              \
    if (!((xEval)cond(yEval))) {                                               \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                 \
      SAT_TEST_PRINTF("  Expected : (");                                       \
      SAT_TEST_PRINTF(#x ") " #cond " (" #y);                                  \
      SAT_TEST_PRINTF(")\n");                                                  \
      SAT_TEST_PRINTF("    Actual : ");                                        \
      sat_test_type_printer(xEval);                                            \
      SAT_TEST_PRINTF(" vs ");                                                 \
      sat_test_type_printer(yEval);                                            \
      SAT_TEST_PRINTF("\n");                                                   \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END
#else
#define SAT_TEST_EXPECT(x, y, cond)                                            \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                         \
    if (!((x)cond(y))) {                                                       \
      SAT_TEST_PRINTF("%s:%u: Failure (Expected " #cond " Actual)\n", __FILE__,\
                   __LINE__);                                                  \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END
#endif

#define SAT_TEST_EXPECT_TRUE(x)                                                         \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                         \
    if (!(x)) {                                                                \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                 \
      SAT_TEST_PRINTF("  Expected : true\n");                                  \
      SAT_TEST_PRINTF("    Actual : %s\n", (x) ? "true" : "false");            \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END

#define SAT_TEST_EXPECT_FALSE(x)                                                        \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                         \
    if (x) {                                                                   \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                 \
      SAT_TEST_PRINTF("  Expected : false\n");                                 \
      SAT_TEST_PRINTF("    Actual : %s\n", (x) ? "true" : "false");            \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END

#define SAT_TEST_EXPECT_EQ(x, y) SAT_TEST_EXPECT(x, y, ==)
#define SAT_TEST_EXPECT_NE(x, y) SAT_TEST_EXPECT(x, y, !=)
#define SAT_TEST_EXPECT_LT(x, y) SAT_TEST_EXPECT(x, y, <)
#define SAT_TEST_EXPECT_LE(x, y) SAT_TEST_EXPECT(x, y, <=)
#define SAT_TEST_EXPECT_GT(x, y) SAT_TEST_EXPECT(x, y, >)
#define SAT_TEST_EXPECT_GE(x, y) SAT_TEST_EXPECT(x, y, >=)

#define SAT_TEST_EXPECT_STREQ(x, y)                                                     \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                         \
    if (SAT_TEST_NULL == x || SAT_TEST_NULL == y || 0 != strcmp(x, y)) {       \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                 \
      SAT_TEST_PRINTF("  Expected : \"%s\"\n", x);                             \
      SAT_TEST_PRINTF("    Actual : \"%s\"\n", y);                             \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END

#define SAT_TEST_EXPECT_STRNE(x, y)                                                     \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                         \
    if (SAT_TEST_NULL == x || SAT_TEST_NULL == y || 0 == strcmp(x, y)) {       \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                 \
      SAT_TEST_PRINTF("  Expected : \"%s\"\n", x);                             \
      SAT_TEST_PRINTF("    Actual : \"%s\"\n", y);                             \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END

#define SAT_TEST_EXPECT_STRNEQ(x, y, n)                                                 \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                         \
    if (SAT_TEST_NULL == x || SAT_TEST_NULL == y || 0 != SAT_TEST_STRNCMP(x, y, n)) {   \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                 \
      SAT_TEST_PRINTF("  Expected : \"%.*s\"\n", SAT_TEST_CAST(int, n), x);    \
      SAT_TEST_PRINTF("    Actual : \"%.*s\"\n", SAT_TEST_CAST(int, n), y);    \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END

#define SAT_TEST_EXPECT_STRNNE(x, y, n)                                                 \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                         \
    if (SAT_TEST_NULL == x || SAT_TEST_NULL == y || 0 == SAT_TEST_STRNCMP(x, y, n)) {   \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                 \
      SAT_TEST_PRINTF("  Expected : \"%.*s\"\n", SAT_TEST_CAST(int, n), x);    \
      SAT_TEST_PRINTF("    Actual : \"%.*s\"\n", SAT_TEST_CAST(int, n), y);    \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END

#define SAT_TEST_EXPECT_NEAR(x, y, epsilon)                                             \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                         \
    const double diff =                                                        \
        SAT_TestFabs(SAT_TEST_CAST(double, x) - SAT_TEST_CAST(double, y));    \
    if (diff > SAT_TEST_CAST(double, epsilon) || SAT_TestIsNan(diff)) {       \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                 \
      SAT_TEST_PRINTF("  Expected : %f\n", SAT_TEST_CAST(double, x));          \
      SAT_TEST_PRINTF("    Actual : %f\n", SAT_TEST_CAST(double, y));          \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END

#if defined(__cplusplus)
#define SAT_TEST_EXPECT_EXCEPTION(x, exception_type)                                    \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                         \
    int exception_caught = 0;                                                  \
    try {                                                                      \
      x;                                                                       \
    } catch (const exception_type &) {                                         \
      exception_caught = 1;                                                    \
    } catch (...) {                                                            \
      exception_caught = 2;                                                    \
    }                                                                          \
    if (exception_caught != 1) {                                               \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                 \
      SAT_TEST_PRINTF("  Expected : %s exception\n", #exception_type);         \
      SAT_TEST_PRINTF("    Actual : %s\n", (exception_caught == 2)             \
                                            ? "Unexpected exception"           \
                                            : "No exception");                 \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END
#endif

#if defined(__clang__)
#define SAT_TEST_ASSERT(x, y, cond)                                            \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                         \
    _Pragma("clang diagnostic push")                                           \
        _Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"")     \
            _Pragma("clang diagnostic ignored \"-Wc++98-compat-pedantic\"")    \
                _Pragma("clang diagnostic ignored \"-Wfloat-equal\"")          \
                    SAT_TEST_AUTO(x) xEval = (x);                              \
    SAT_TEST_AUTO(y) yEval = (y);                                              \
    if (!((xEval)cond(yEval))) {                                               \
      _Pragma("clang diagnostic pop")                                          \
          SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);             \
      SAT_TEST_PRINTF("  Expected : (");                                       \
      SAT_TEST_PRINTF(#x ") " #cond " (" #y);                                  \
      SAT_TEST_PRINTF(")\n");                                                  \
      SAT_TEST_PRINTF("    Actual : ");                                        \
      sat_test_type_printer(xEval);                                            \
      SAT_TEST_PRINTF(" vs ");                                                 \
      sat_test_type_printer(yEval);                                            \
      SAT_TEST_PRINTF("\n");                                                   \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                \
      return;                                                                  \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END
#elif defined(__GNUC__)
#define SAT_TEST_ASSERT(x, y, cond)                                               \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                            \
    SAT_TEST_AUTO(x) xEval = (x);                                                 \
    SAT_TEST_AUTO(y) yEval = (y);                                                 \
    if (!((xEval)cond(yEval))) {                                               \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
      SAT_TEST_PRINTF("  Expected : (");                                          \
      SAT_TEST_PRINTF(#x ") " #cond " (" #y);                                     \
      SAT_TEST_PRINTF(")\n");                                                     \
      SAT_TEST_PRINTF("    Actual : ");                                           \
      sat_test_type_printer(xEval);                                               \
      SAT_TEST_PRINTF(" vs ");                                                    \
      sat_test_type_printer(yEval);                                               \
      SAT_TEST_PRINTF("\n");                                                      \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                      \
      return;                                                                  \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END
#else
#define SAT_TEST_ASSERT(x, y, cond)                                               \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                            \
    if (!((x)cond(y))) {                                                       \
      SAT_TEST_PRINTF("%s:%u: Failure (Expected " #cond " Actual)\n", __FILE__,   \
                   __LINE__);                                                  \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                      \
      return;                                                                  \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END
#endif

#define SAT_TEST_ASSERT_TRUE(x)                                                         \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                            \
    if (!(x)) {                                                                \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
      SAT_TEST_PRINTF("  Expected : true\n");                                     \
      SAT_TEST_PRINTF("    Actual : %s\n", (x) ? "true" : "false");               \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                      \
      return;                                                                  \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END

#define SAT_TEST_ASSERT_FALSE(x)                                                        \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                            \
    if (x) {                                                                   \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
      SAT_TEST_PRINTF("  Expected : false\n");                                    \
      SAT_TEST_PRINTF("    Actual : %s\n", (x) ? "true" : "false");               \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                      \
      return;                                                                  \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END

#define SAT_TEST_ASSERT_EQ(x, y) SAT_TEST_ASSERT(x, y, ==)
#define SAT_TEST_ASSERT_NE(x, y) SAT_TEST_ASSERT(x, y, !=)
#define SAT_TEST_ASSERT_LT(x, y) SAT_TEST_ASSERT(x, y, <)
#define SAT_TEST_ASSERT_LE(x, y) SAT_TEST_ASSERT(x, y, <=)
#define SAT_TEST_ASSERT_GT(x, y) SAT_TEST_ASSERT(x, y, >)
#define SAT_TEST_ASSERT_GE(x, y) SAT_TEST_ASSERT(x, y, >=)

#define SAT_TEST_ASSERT_STREQ(x, y)                                                     \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                            \
    if (SAT_TEST_NULL == x || SAT_TEST_NULL == y || 0 != strcmp(x, y)) {             \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
      SAT_TEST_PRINTF("  Expected : \"%s\"\n", x);                                \
      SAT_TEST_PRINTF("    Actual : \"%s\"\n", y);                                \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                      \
      return;                                                                  \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END

#define SAT_TEST_ASSERT_STRNE(x, y)                                                     \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                            \
    if (SAT_TEST_NULL == x || SAT_TEST_NULL == y || 0 == strcmp(x, y)) {             \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
      SAT_TEST_PRINTF("  Expected : \"%s\"\n", x);                                \
      SAT_TEST_PRINTF("    Actual : \"%s\"\n", y);                                \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                      \
      return;                                                                  \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END

#define SAT_TEST_ASSERT_STRNEQ(x, y, n)                                                 \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                            \
    if (SAT_TEST_NULL == x || SAT_TEST_NULL == y || 0 != SAT_TEST_STRNCMP(x, y, n)) {   \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
      SAT_TEST_PRINTF("  Expected : \"%.*s\"\n", SAT_TEST_CAST(int, n), x);          \
      SAT_TEST_PRINTF("    Actual : \"%.*s\"\n", SAT_TEST_CAST(int, n), y);          \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                      \
      return;                                                                  \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END

#define SAT_TEST_ASSERT_STRNNE(x, y, n)                                                 \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                            \
    if (SAT_TEST_NULL == x || SAT_TEST_NULL == y || 0 == SAT_TEST_STRNCMP(x, y, n)) {   \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
      SAT_TEST_PRINTF("  Expected : \"%.*s\"\n", SAT_TEST_CAST(int, n), x);          \
      SAT_TEST_PRINTF("    Actual : \"%.*s\"\n", SAT_TEST_CAST(int, n), y);          \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                      \
      return;                                                                  \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END

#define SAT_TEST_ASSERT_NEAR(x, y, epsilon)                                             \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                            \
    const double diff =                                                        \
        SAT_TestFabs(SAT_TEST_CAST(double, x) - SAT_TEST_CAST(double, y));             \
    if (diff > SAT_TEST_CAST(double, epsilon) || SAT_TestIsNan(diff)) {             \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
      SAT_TEST_PRINTF("  Expected : %f\n", SAT_TEST_CAST(double, x));                \
      SAT_TEST_PRINTF("    Actual : %f\n", SAT_TEST_CAST(double, y));                \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                      \
      return;                                                                  \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END

#if defined(__cplusplus)
#define SAT_TEST_ASSERT_EXCEPTION(x, exception_type)                                    \
  SAT_TEST_SURPRESS_WARNING_BEGIN do {                                            \
    int exception_caught = 0;                                                  \
    try {                                                                      \
      x;                                                                       \
    } catch (const exception_type &) {                                         \
      exception_caught = 1;                                                    \
    } catch (...) {                                                            \
      exception_caught = 2;                                                    \
    }                                                                          \
    if (exception_caught != 1) {                                               \
      SAT_TEST_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
      SAT_TEST_PRINTF("  Expected : %s exception\n", #exception_type);            \
      SAT_TEST_PRINTF("    Actual : %s\n", (exception_caught == 2)                \
                                            ? "Unexpected exception"           \
                                            : "No exception");                 \
      *sat_test_result = SAT_TEST_TEST_FAILURE;                                      \
      return;                                                                  \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  SAT_TEST_SURPRESS_WARNING_END
#endif

#define SAT_TEST(SET, NAME)                                                           \
  SAT_TEST_EXTERN struct sat_test_state_s sat_test_state;                             \
  static void sat_test_run_##SET##_##NAME(int *sat_test_result);                      \
  static void sat_test_##SET##_##NAME(int *sat_test_result, size_t sat_test_index) {  \
    (void)sat_test_index;                                                             \
    sat_test_run_##SET##_##NAME(sat_test_result);                                     \
  }                                                                                   \
  SAT_TEST_INITIALIZER(sat_test_register_##SET##_##NAME) {                            \
    const size_t index = sat_test_state.tests_length++;                               \
    const char *name_part = #SET "." #NAME;                                           \
    const size_t name_size = strlen(name_part) + 1;                                   \
    char *name = SAT_TEST_PTR_CAST(char *, malloc(name_size));                        \
    sat_test_state.tests = SAT_TEST_PTR_CAST(                                         \
        struct sat_test_test_state_s *,                                               \
        sat_test_realloc(SAT_TEST_PTR_CAST(void *, sat_test_state.tests),             \
                      sizeof(struct sat_test_test_state_s) *                          \
                          sat_test_state.tests_length));                              \
    if (sat_test_state.tests) {                                                       \
      sat_test_state.tests[index].func = &sat_test_##SET##_##NAME;                   \
      sat_test_state.tests[index].name = name;                                    \
      sat_test_state.tests[index].index = 0;                                      \
    }                                                                          \
    SAT_TEST_SNPRINTF(name, name_size, "%s", name_part);                          \
  }                                                                            \
  void sat_test_run_##SET##_##NAME(int *sat_test_result)

#define SAT_TEST_F_SETUP(FIXTURE)                                                 \
  static void sat_test_f_setup_##FIXTURE(int *sat_test_result,                       \
                                      struct FIXTURE *sat_test_fixture)

#define SAT_TEST_F_TEARDOWN(FIXTURE)                                              \
  static void sat_test_f_teardown_##FIXTURE(int *sat_test_result,                    \
                                         struct FIXTURE *sat_test_fixture)

#if defined(__GNUC__) && __GNUC__ >= 8 && defined(__cplusplus)
#define SAT_TEST_FIXTURE_SURPRESS_WARNINGS_BEGIN                                  \
  _Pragma("GCC diagnostic push")                                               \
      _Pragma("GCC diagnostic ignored \"-Wclass-memaccess\"")
#define SAT_TEST_FIXTURE_SURPRESS_WARNINGS_END _Pragma("GCC diagnostic pop")
#else
#define SAT_TEST_FIXTURE_SURPRESS_WARNINGS_BEGIN
#define SAT_TEST_FIXTURE_SURPRESS_WARNINGS_END
#endif

#define SAT_TEST_F(FIXTURE, NAME)                                                 \
  SAT_TEST_FIXTURE_SURPRESS_WARNINGS_BEGIN                                        \
  SAT_TEST_EXTERN struct sat_test_state_s sat_test_state;                               \
  static void sat_test_f_setup_##FIXTURE(int *, struct FIXTURE *);                \
  static void sat_test_f_teardown_##FIXTURE(int *, struct FIXTURE *);             \
  static void sat_test_run_##FIXTURE##_##NAME(int *, struct FIXTURE *);           \
  static void sat_test_f_##FIXTURE##_##NAME(int *sat_test_result,                    \
                                         size_t sat_test_index) {                 \
    struct FIXTURE fixture;                                                    \
    (void)sat_test_index;                                                         \
    memset(&fixture, 0, sizeof(fixture));                                      \
    sat_test_f_setup_##FIXTURE(sat_test_result, &fixture);                           \
    if (SAT_TEST_TEST_PASSED != *sat_test_result) {                                  \
      return;                                                                  \
    }                                                                          \
    sat_test_run_##FIXTURE##_##NAME(sat_test_result, &fixture);                      \
    sat_test_f_teardown_##FIXTURE(sat_test_result, &fixture);                        \
  }                                                                            \
  SAT_TEST_INITIALIZER(sat_test_register_##FIXTURE##_##NAME) {                       \
    const size_t index = sat_test_state.tests_length++;                           \
    const char *name_part = #FIXTURE "." #NAME;                                \
    const size_t name_size = strlen(name_part) + 1;                            \
    char *name = SAT_TEST_PTR_CAST(char *, malloc(name_size));                    \
    sat_test_state.tests = SAT_TEST_PTR_CAST(                                        \
        struct sat_test_test_state_s *,                                           \
        sat_test_realloc(SAT_TEST_PTR_CAST(void *, sat_test_state.tests),               \
                      sizeof(struct sat_test_test_state_s) *                      \
                          sat_test_state.tests_length));                          \
    sat_test_state.tests[index].func = &sat_test_f_##FIXTURE##_##NAME;               \
    sat_test_state.tests[index].name = name;                                      \
    SAT_TEST_SNPRINTF(name, name_size, "%s", name_part);                          \
  }                                                                            \
  SAT_TEST_FIXTURE_SURPRESS_WARNINGS_END                                          \
  void sat_test_run_##FIXTURE##_##NAME(int *sat_test_result,                         \
                                    struct FIXTURE *sat_test_fixture)

#define SAT_TEST_I_SETUP(FIXTURE)                                                 \
  static void sat_test_i_setup_##FIXTURE(                                         \
      int *sat_test_result, struct FIXTURE *sat_test_fixture, size_t sat_test_index)

#define SAT_TEST_I_TEARDOWN(FIXTURE)                                              \
  static void sat_test_i_teardown_##FIXTURE(                                      \
      int *sat_test_result, struct FIXTURE *sat_test_fixture, size_t sat_test_index)

#define SAT_TEST_I(FIXTURE, NAME, INDEX)                                          \
  SAT_TEST_EXTERN struct sat_test_state_s sat_test_state;                               \
  static void sat_test_run_##FIXTURE##_##NAME##_##INDEX(int *, struct FIXTURE *); \
  static void sat_test_i_##FIXTURE##_##NAME##_##INDEX(int *sat_test_result,          \
                                                   size_t index) {             \
    struct FIXTURE fixture;                                                    \
    memset(&fixture, 0, sizeof(fixture));                                      \
    sat_test_i_setup_##FIXTURE(sat_test_result, &fixture, index);                    \
    if (SAT_TEST_TEST_PASSED != *sat_test_result) {                                  \
      return;                                                                  \
    }                                                                          \
    sat_test_run_##FIXTURE##_##NAME##_##INDEX(sat_test_result, &fixture);            \
    sat_test_i_teardown_##FIXTURE(sat_test_result, &fixture, index);                 \
  }                                                                            \
  SAT_TEST_INITIALIZER(sat_test_register_##FIXTURE##_##NAME##_##INDEX) {             \
    size_t i;                                                                  \
    sat_test_uint64_t iUp;                                                        \
    for (i = 0; i < (INDEX); i++) {                                            \
      const size_t index = sat_test_state.tests_length++;                         \
      const char *name_part = #FIXTURE "." #NAME;                              \
      const size_t name_size = strlen(name_part) + 32;                         \
      char *name = SAT_TEST_PTR_CAST(char *, malloc(name_size));                  \
      sat_test_state.tests = SAT_TEST_PTR_CAST(                                      \
          struct sat_test_test_state_s *,                                         \
          sat_test_realloc(SAT_TEST_PTR_CAST(void *, sat_test_state.tests),             \
                        sizeof(struct sat_test_test_state_s) *                    \
                            sat_test_state.tests_length));                        \
      sat_test_state.tests[index].func = &sat_test_i_##FIXTURE##_##NAME##_##INDEX;   \
      sat_test_state.tests[index].index = i;                                      \
      sat_test_state.tests[index].name = name;                                    \
      iUp = SAT_TEST_CAST(sat_test_uint64_t, i);                                     \
      SAT_TEST_SNPRINTF(name, name_size, "%s/%" SAT_TEST_PRIu64, name_part, iUp);    \
    }                                                                          \
  }                                                                            \
  void sat_test_run_##FIXTURE##_##NAME##_##INDEX(int *sat_test_result,               \
                                              struct FIXTURE *sat_test_fixture)

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

SAT_TEST_WEAK
double SAT_TestFabs(double d);
SAT_TEST_WEAK
double SAT_TestFabs(double d) {
  union {
    double d;
    sat_test_uint64_t u;
  } both;
  both.d = d;
  both.u &= 0x7fffffffffffffffu;
  return both.d;
}

SAT_TEST_WEAK
int SAT_TestIsNan(double d);
SAT_TEST_WEAK
int SAT_TestIsNan(double d) {
  union {
    double d;
    sat_test_uint64_t u;
  } both;
  both.d = d;
  both.u &= 0x7fffffffffffffffu;
  return both.u > 0x7ff0000000000000u;
}

SAT_TEST_WEAK
int sat_test_should_filter_test(const char *filter, const char *testcase);
SAT_TEST_WEAK int sat_test_should_filter_test(const char *filter,
                                        const char *testcase) {
  if (filter) {
    const char *filter_cur = filter;
    const char *testcase_cur = testcase;
    const char *filter_wildcard = SAT_TEST_NULL;

    while (('\0' != *filter_cur) && ('\0' != *testcase_cur)) {
      if ('*' == *filter_cur) {
        /* store the position of the wildcard */
        filter_wildcard = filter_cur;

        /* skip the wildcard character */
        filter_cur++;

        while (('\0' != *filter_cur) && ('\0' != *testcase_cur)) {
          if ('*' == *filter_cur) {
            /*
               we found another wildcard (filter is something like *foo*) so we
               exit the current loop, and return to the parent loop to handle
               the wildcard case
            */
            break;
          } else if (*filter_cur != *testcase_cur) {
            /* otherwise our filter didn't match, so reset it */
            filter_cur = filter_wildcard;
          }

          /* move testcase along */
          testcase_cur++;

          /* move filter along */
          filter_cur++;
        }

        if (('\0' == *filter_cur) && ('\0' == *testcase_cur)) {
          return 0;
        }

        /* if the testcase has been exhausted, we don't have a match! */
        if ('\0' == *testcase_cur) {
          return 1;
        }
      } else {
        if (*testcase_cur != *filter_cur) {
          /* test case doesn't match filter */
          return 1;
        } else {
          /* move our filter and testcase forward */
          testcase_cur++;
          filter_cur++;
        }
      }
    }

    if (('\0' != *filter_cur) ||
        (('\0' != *testcase_cur) &&
         ((filter == filter_cur) || ('*' != filter_cur[-1])))) {
      /* we have a mismatch! */
      return 1;
    }
  }

  return 0;
}

static SAT_TEST_INLINE FILE *sat_test_fopen(const char *filename, const char *mode) {
#ifdef _MSC_VER
  FILE *file;
  if (0 == fopen_s(&file, filename, mode)) {
    return file;
  } else {
    return SAT_TEST_NULL;
  }
#else
  return fopen(filename, mode);
#endif
}

//  static
//  int SAT_TestMain(int argc, char** argv) { return 0; }

int SAT_TestMain(int argc, char**argv) {
//static SAT_TEST_INLINE int SAT_TestMain(int argc, const char *const argv[]);
  SAT_PRINT;
  sat_test_uint64_t failed = 0;
  sat_test_uint64_t skipped = 0;
  size_t index = 0;
  size_t *failed_testcases = SAT_TEST_NULL;
  size_t failed_testcases_length = 0;
  size_t *skipped_testcases = SAT_TEST_NULL;
  size_t skipped_testcases_length = 0;
  const char *filter = SAT_TEST_NULL;
  sat_test_uint64_t ran_tests = 0;
  int enable_mixed_units = 0;
  int random_order = 0;
  sat_test_uint32_t seed = 0;

  enum colours { RESET, GREEN, RED, YELLOW };

  const int use_colours = SAT_TEST_COLOUR_OUTPUT();
  const char *colours[] = {"\033[0m", "\033[32m", "\033[31m", "\033[33m"};

  if (!use_colours) {
    for (index = 0; index < sizeof colours / sizeof colours[0]; index++) {
      colours[index] = "";
    }
  }
  /* loop through all arguments looking for our options */
  for (index = 1; index < SAT_TEST_CAST(size_t, argc); index++) {
    /* Informational switches */
    const char help_str[] = "--help";
    const char list_str[] = "--list-tests";
    /* Test config switches */
    const char filter_str[] = "--filter=";
    const char output_str[] = "--output=";
    const char enable_mixed_units_str[] = "--enable-mixed-units";
    const char random_order_str[] = "--random-order";
    const char random_order_with_seed_str[] = "--random-order=";

    if (0 == SAT_TEST_STRNCMP(argv[index], help_str, strlen(help_str))) {
      printf("sat_test.h - the single file unit testing solution for C/C++!\n"
             "Command line Options:\n"
             "  --help                  Show this message and exit.\n"
             "  --filter=<filter>       Filter the test cases to run (EG. "
             "MyTest*.a would run MyTestCase.a but not MyTestCase.b).\n"
             "  --list-tests            List testnames, one per line. Output "
             "names can be passed to --filter.\n");
      printf("  --output=<output>       Output an xunit XML file to the file "
             "specified in <output>.\n"
             "  --enable-mixed-units    Enable the per-test output to contain "
             "mixed units (s/ms/us/ns).\n"
             "  --random-order[=<seed>] Randomize the order that the tests are "
             "ran in. If the optional <seed> argument is not provided, then a "
             "random starting seed is used.\n");
      goto cleanup;
    } else if (0 ==
               SAT_TEST_STRNCMP(argv[index], filter_str, strlen(filter_str))) {
      /* user wants to filter what test cases run! */
      filter = argv[index] + strlen(filter_str);
    } else if (0 ==
               SAT_TEST_STRNCMP(argv[index], output_str, strlen(output_str))) {
      sat_test_state.output = sat_test_fopen(argv[index] + strlen(output_str), "w+");
    } else if (0 == SAT_TEST_STRNCMP(argv[index], list_str, strlen(list_str))) {
      for (index = 0; index < sat_test_state.tests_length; index++) {
        SAT_TEST_PRINTF("%s\n", sat_test_state.tests[index].name);
      }
      /* when printing the test list, don't actually run the tests */
      return 0;
    } else if (0 == SAT_TEST_STRNCMP(argv[index], enable_mixed_units_str,
                                  strlen(enable_mixed_units_str))) {
      enable_mixed_units = 1;
    } else if (0 == SAT_TEST_STRNCMP(argv[index], random_order_with_seed_str,
                                  strlen(random_order_with_seed_str))) {
      seed =
          SAT_TEST_CAST(sat_test_uint32_t,
                     strtoul(argv[index] + strlen(random_order_with_seed_str),
                             SAT_TEST_NULL, 10));
      random_order = 1;
    } else if (0 == SAT_TEST_STRNCMP(argv[index], random_order_str,
                                  strlen(random_order_str))) {
      const sat_test_int64_t ns = sat_test_ns();

      // Some really poor pseudo-random using the current time. I do this
      // because I really want to avoid using C's rand() because that'd mean our
      // random would be affected by any srand() usage by the user (which I
      // don't want).
      seed = SAT_TEST_CAST(sat_test_uint32_t, ns >> 32) * 31 +
             SAT_TEST_CAST(sat_test_uint32_t, ns & 0xffffffff);
      random_order = 1;
    }
  }

  if (random_order) {
    // Use Fisher-Yates with the Durstenfield's version to randomly re-order the
    // tests.
    for (index = sat_test_state.tests_length; index > 1; index--) {
      // For the random order we'll use PCG.
      const sat_test_uint32_t state = seed;
      const sat_test_uint32_t word =
          ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
      const sat_test_uint32_t next =
          ((word >> 22u) ^ word) % SAT_TEST_CAST(sat_test_uint32_t, index);

      // Swap the randomly chosen element into the last location.
      const struct sat_test_test_state_s copy = sat_test_state.tests[index - 1];
      sat_test_state.tests[index - 1] = sat_test_state.tests[next];
      sat_test_state.tests[next] = copy;

      // Move the seed onwards.
      seed = seed * 747796405u + 2891336453u;
    }
  }

  for (index = 0; index < sat_test_state.tests_length; index++) {
    if (sat_test_should_filter_test(filter, sat_test_state.tests[index].name)) {
      continue;
    }

    ran_tests++;
  }

  printf("%s[==========]%s Running %" SAT_TEST_PRIu64 " test cases.\n",
         colours[GREEN], colours[RESET], SAT_TEST_CAST(sat_test_uint64_t, ran_tests));

  if (sat_test_state.output) {
    fprintf(sat_test_state.output, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(sat_test_state.output,
            "<testsuites tests=\"%" SAT_TEST_PRIu64 "\" name=\"All\">\n",
            SAT_TEST_CAST(sat_test_uint64_t, ran_tests));
    fprintf(sat_test_state.output,
            "<testsuite name=\"Tests\" tests=\"%" SAT_TEST_PRIu64 "\">\n",
            SAT_TEST_CAST(sat_test_uint64_t, ran_tests));
  }

  for (index = 0; index < sat_test_state.tests_length; index++) {
    int result = SAT_TEST_TEST_PASSED;
    sat_test_int64_t ns = 0;

    if (sat_test_should_filter_test(filter, sat_test_state.tests[index].name)) {
      continue;
    }

    printf("%s[ RUN      ]%s %s\n", colours[GREEN], colours[RESET],
           sat_test_state.tests[index].name);

    if (sat_test_state.output) {
      fprintf(sat_test_state.output, "<testcase name=\"%s\">",
              sat_test_state.tests[index].name);
    }

    ns = sat_test_ns();
    errno = 0;
#if defined(__cplusplus)
    SAT_TEST_SURPRESS_WARNING_BEGIN
    try {
      sat_test_state.tests[index].func(&result, sat_test_state.tests[index].index);
    } catch (const std::exception &err) {
      printf(" Exception : %s\n", err.what());
      result = SAT_TEST_TEST_FAILURE;
    } catch (...) {
      printf(" Exception : Unknown\n");
      result = SAT_TEST_TEST_FAILURE;
    }
    SAT_TEST_SURPRESS_WARNING_END
#else
    sat_test_state.tests[index].func(&result, sat_test_state.tests[index].index);
#endif
    ns = sat_test_ns() - ns;

    if (sat_test_state.output) {
      fprintf(sat_test_state.output, "</testcase>\n");
    }

    // Record the failing test.
    if (SAT_TEST_TEST_FAILURE == result) {
      const size_t failed_testcase_index = failed_testcases_length++;
      failed_testcases = SAT_TEST_PTR_CAST(
          size_t *, sat_test_realloc(SAT_TEST_PTR_CAST(void *, failed_testcases),
                                  sizeof(size_t) * failed_testcases_length));
      if (SAT_TEST_NULL != failed_testcases) {
        failed_testcases[failed_testcase_index] = index;
      }
      failed++;
    } else if (SAT_TEST_TEST_SKIPPED == result) {
      const size_t skipped_testcase_index = skipped_testcases_length++;
      skipped_testcases = SAT_TEST_PTR_CAST(
          size_t *, sat_test_realloc(SAT_TEST_PTR_CAST(void *, skipped_testcases),
                                  sizeof(size_t) * skipped_testcases_length));
      if (SAT_TEST_NULL != skipped_testcases) {
        skipped_testcases[skipped_testcase_index] = index;
      }
      skipped++;
    }

    {
      const char *const units[] = {"ns", "us", "ms", "s", SAT_TEST_NULL};
      unsigned int unit_index = 0;
      sat_test_int64_t time = ns;

      if (enable_mixed_units) {
        for (unit_index = 0; SAT_TEST_NULL != units[unit_index]; unit_index++) {
          if (10000 > time) {
            break;
          }

          time /= 1000;
        }
      }

      if (SAT_TEST_TEST_FAILURE == result) {
        printf("%s[  FAILED  ]%s %s (%" SAT_TEST_PRId64 "%s)\n", colours[RED],
               colours[RESET], sat_test_state.tests[index].name, time,
               units[unit_index]);
      } else if (SAT_TEST_TEST_SKIPPED == result) {
        printf("%s[  SKIPPED ]%s %s (%" SAT_TEST_PRId64 "%s)\n", colours[YELLOW],
               colours[RESET], sat_test_state.tests[index].name, time,
               units[unit_index]);
      } else {
        printf("%s[       OK ]%s %s (%" SAT_TEST_PRId64 "%s)\n", colours[GREEN],
               colours[RESET], sat_test_state.tests[index].name, time,
               units[unit_index]);
      }
    }
  }

  printf("%s[==========]%s %" SAT_TEST_PRIu64 " test cases ran.\n", colours[GREEN],
         colours[RESET], ran_tests);
  printf("%s[  PASSED  ]%s %" SAT_TEST_PRIu64 " tests.\n", colours[GREEN],
         colours[RESET], ran_tests - failed - skipped);

  if (0 != skipped) {
    printf("%s[  SKIPPED ]%s %" SAT_TEST_PRIu64 " tests, listed below:\n",
           colours[YELLOW], colours[RESET], skipped);
    for (index = 0; index < skipped_testcases_length; index++) {
      printf("%s[  SKIPPED ]%s %s\n", colours[YELLOW], colours[RESET],
             sat_test_state.tests[skipped_testcases[index]].name);
    }
  }

  if (0 != failed) {
    printf("%s[  FAILED  ]%s %" SAT_TEST_PRIu64 " tests, listed below:\n",
           colours[RED], colours[RESET], failed);
    for (index = 0; index < failed_testcases_length; index++) {
      printf("%s[  FAILED  ]%s %s\n", colours[RED], colours[RESET],
             sat_test_state.tests[failed_testcases[index]].name);
    }
  }

  if (sat_test_state.output) {
    fprintf(sat_test_state.output, "</testsuite>\n</testsuites>\n");
  }

cleanup:
  for (index = 0; index < sat_test_state.tests_length; index++) {
    free(SAT_TEST_PTR_CAST(void *, sat_test_state.tests[index].name));
  }

  free(SAT_TEST_PTR_CAST(void *, skipped_testcases));
  free(SAT_TEST_PTR_CAST(void *, failed_testcases));
  free(SAT_TEST_PTR_CAST(void *, sat_test_state.tests));

  if (sat_test_state.output) {
    fclose(sat_test_state.output);
  }

  return SAT_TEST_CAST(int, failed);
}

/*
   we need, in exactly one source file, define the global struct that will hold
   the data we need to run sat_test. This macro allows the user to declare the
   data without having to use the SAT_TEST_MAIN macro, thus allowing them to write
   their own main() function.
*/
#define SAT_TEST_STATE() struct sat_test_state_s sat_test_state = {0, 0, 0}

/*
   define a main() function to call into sat_test.h and start executing tests! A
   user can optionally not use this macro, and instead define their own main()
   function and manually call sat_test_main. The user must, in exactly one source
   file, use the SAT_TEST_STATE macro to declare a global struct variable that
   sat_test requires.
*/
#define SAT_TEST_MAIN()                                                           \
  SAT_TEST_STATE();                                                               \
  int main(int argc, const char *const argv[]) {                               \
    return sat_test_main(argc, argv);                                             \
  }

//----------------------------------------------------------------------

#else

//----------------------------------------------------------------------

  int SAT_TestMain(int argc, char** argv) { return 0; }

  #define SAT_TEST_STATE(x)

  //

  #define SAT_TEST_EXPECT(x, y, cond)
  #define SAT_TEST_EXPECT_TRUE(x)
  #define SAT_TEST_EXPECT_FALSE(x)

  #define SAT_TEST_EXPECT_STREQ(x, y)
  #define SAT_TEST_EXPECT_STRNE(x, y)
  #define SAT_TEST_EXPECT_STRNEQ(x, y, n)
  #define SAT_TEST_EXPECT_STRNNE(x, y, n)
  #define SAT_TEST_EXPECT_NEAR(x, y, epsilon)
  #define SAT_TEST_EXPECT_EXCEPTION(x, exception_type)
  #define SAT_TEST_ASSERT(x, y, cond)

  #define SAT_TEST_ASSERT_TRUE(x)

  #define SAT_TEST_ASSERT_FALSE(x)
  #define SAT_TEST_ASSERT_STREQ(x, y)
  #define SAT_TEST_ASSERT_STRNE(x, y)
  #define SAT_TEST_ASSERT_STRNEQ(x, y, n)
  #define SAT_TEST_ASSERT_STRNNE(x, y, n)
  #define SAT_TEST_ASSERT_NEAR(x, y, epsilon)
  #define SAT_TEST_ASSERT_EXCEPTION(x, exception_type)

  #define SAT_TEST(SET,NAME)                static __SAT_UNUSED void sat_test_register_##SET##_##NAME()
  #define SAT_TEST_F_SETUP(FIXTURE)         static __SAT_UNUSED void sat_test_f_setup_##SET##_##NAME()
  #define SAT_TEST_F_TEARDOWN(FIXTURE)      static __SAT_UNUSED void sat_test_f_teardown_##SET##_##NAME()
  #define SAT_TEST_F(FIXTURE, NAME)         static __SAT_UNUSED void sat_test_f_##SET##_##NAME()
  #define SAT_TEST_I_SETUP(FIXTURE)         static __SAT_UNUSED void sat_test_i_setup_##SET##_##NAME()
  #define SAT_TEST_I_TEARDOWN(FIXTURE)      static __SAT_UNUSED void sat_test_i_teardown_##SET##_##NAME()
  #define SAT_TEST_I(FIXTURE, NAME, INDEX)  static __SAT_UNUSED void sat_test_i_##SET##_##NAME()

#endif // SAT_UNIT_TESTING

//----------------------------------------------------------------------
#endif

