#ifndef sat_debug_assert_included
#define sat_debug_assert_included
//----------------------------------------------------------------------

#ifdef SAT_DEBUG_ASSERT

  //#ifdef H_DEBUG
  //
  //  unsigned int _h_Assert () __attribute__ (( noinline ));
  //  unsigned int _h_Assert(const unsigned int e, const char* file, const char* func, const unsigned int line, const char* e_str) {
  //    if (!e) {
  //      dtrace("### H_ASSERT: " << file << ", " << func << ", " << line << ", (" << e_str << ")");
  //      exit(1);
  //    }
  //    return 0;
  //  }
  //  #define h_Assert(e) _h_Assert((e), __FILE__, __FUNCTION__, __LINE__, #e)
  //
  //  #if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 4)
  //    _H_SA_INLINE unsigned int __H_SA_TRUE (void) { return 1; }
  //    unsigned int _H_SA(void)
  //      __attribute__ (( error("### STATIC ASSERTION at this point!") ))
  //      __attribute__ (( optimize(0) ));
  //    #define h_StaticAssert(e) ( (!(e)) ? _H_SA(): __H_SA_TRUE() )
  //  #else
  //    // switch can also be used. it does not generate extra code and is much more portable, but not very useful at the same time.
  //    #define h_StaticAssert(e) switch(0) { case 0: case (e): ; }
  //  #endif
  //
  //#else
  //
  //  _H_SA_INLINE unsigned int __H_SA (void) { return 0; }
  //  #define h_Assert(e)       __H_SA()
  //  #define h_StaticAssert(e) __H_SA()
  //
  //#endif

  #define SAT_Assert(x) {                                                               \
    if (!(x)) {                                                                         \
      SAT_Print("SAT_Assert( " SAT_TERM_FG_RED "%s" SAT_TERM_RESET " ) failed!\n",#x);  \
      /*SAT_Print("SAT_Assert(%s) failed!\n",#x);*/                                     \
      exit(1);                                                                          \
    }                                                                                   \
  }
  
#else

  #define SAT_Assert(x) {}
  
#endif

//----------------------------------------------------------------------
#endif
