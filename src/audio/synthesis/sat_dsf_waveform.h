#ifndef sat_dsf_waveform_included
#define sat_dsf_waveform_included
//----------------------------------------------------------------------

#include <math.h>
#include "base/utils/sat_fast_math.h"
//#include "base/sat_math_table.h"

/*
  * pitch 'offset' between t1 & t2 = "distance" between partials
  * mod amt..
    0.5 = partials decrease by 6 db
    0.25 = -12 db
    0.125 = 18db, etc..
*/

//----------------------------------------------------------------------

/*
  http://www.jamminpower.com/PDF/Sine%20Summation.pdf
  http://www.verklagekasper.de/synths/dsfsynthesis/dsfsynthesis.html
*/

//----------

/*
  http://www.musicdsp.org/showArchiveComment.php?ArchiveID=68http://www.kvraudio.com/forum/viewtopic.php?p=5152613#p5152613
  Discrete Summation Formula (DSF)
  References : Stylson, Smith and others... (posted by Alexander Kritov)
  Notes :
    Buzz uses this type of synth.
    For cool sounds try to use variable,
    for example a=exp(-x/12000)*0.8 // x- num.samples
  Code :
    double DSF(
      double x,   // input
      double a,   // a < 1.0
      double N,   // N
      double fi)  // phase
    {
      double s1 = pow(a,N-1.0)*sin((N-1.0)*x+fi);
      double s2 = pow(a,N)*sin(N*x+fi);
      double s3 = a*sin(x+fi);
      double s4 =1.0 - (2*a*cos(x)) +(a*a);
      if (s4==0) return 0;
      else return (sin(fi) - s3 - s2 +s1)/s4;
    }
*/

//----------------------------------------------------------------------

template <typename T>
class SAT_DsfWaveform {

  private:

    //KMathTable<16384,kmt_sin>  MSinTable;

  public:

    SAT_DsfWaveform() {
    }

    //----------

    // calculate max N

    // float findMaxPartials(float dt1,float dt2) {
    //   float n = 0;
    //   float t = dt1 + dt2;
    //   while (t <= 0.5) {
    //     t += dt2;
    //     n += 1;
    //   }
    //   return n;
    // }

    uint32_t findMaxPartials(T dt1, T dt2) {
      uint32_t n = 0;
      T t = dt1 + dt2;
      while (t <= 0.5) {
        t += dt2;
        n += 1;
      }
      return n;
    }

    //----------

    T process(T t1, T t2, T a, uint32_t N) {
      t1           *= SAT_PI2;
      t2           *= SAT_PI2;
      T N1          = N + 1;
      T aa          = a * a;
      T a2          = a * 2;
      T sin_t1      = sin(t1);
      T cos_t2      = cos(t2);
      T sin_t2t1    = sin(t2 -    t1);
      T sin_t1nt2   = sin(t1 + N *t2);
      T sin_t1n1t2  = sin(t1 + N1*t2);
      T pow_an1     = pow(a,N1);
      T result      = ((a*sin_t2t1+sin_t1) + pow_an1 * (a*sin_t1nt2-sin_t1n1t2)) / (1+aa-a2*cos_t2);
      /*
      T result      = (a*sin_t2t1  + sin_t1)
      result           += pow_an1 * (a*sin_t1nt2 - sin_t1n1t2)
      result           /= (1+aa-a2*cos_t2);
      */
      /*
      // normalization (1)
      result *= sqrt( (1-a*a) / (1-pow(a,2*N+2)) );
      // normalization (2)
      result /= ((1.0f-pow_an1) / (1.0f-a));
      */
      return result;
    }

    //----------

    /*
    T processF(T t1, T t2, T a, uint32_t N) {
      t1               *= SAT_PI2;
      t2               *= SAT_PI2;
      T N1          = N + 1.0f;
      T aa          = a * a;
      T a2          = a * 2.0f;
      T w_t1      = KWrap(t1,         -SAT_PI,SAT_PI);
      T w_t2      = KWrap(t2,         -SAT_PI,SAT_PI);
      T w_t2t1    = KWrap((t2-t1),    -SAT_PI,SAT_PI);
      T w_t1nt2   = KWrap((t1+N*t2),  -SAT_PI,SAT_PI);
      T w_t1n1t2  = KWrap((t1+N1*t2), -SAT_PI,SAT_PI);
      T sin_t1      = KSinF2(w_t1);
      T cos_t2      = KCosF2(w_t2);
      T sin_t2t1    = KSinF2(w_t2t1);
      T sin_t1nt2   = KSinF2(w_t1nt2);
      T sin_t1n1t2  = KSinF2(w_t1n1t2);
      //int32 iN1         = (int32)KTrunc(N1);
      //T pow_an1     = KPowF(a,iN1);
      T pow_an1     = pow(a,N1);
      return ((a*sin_t2t1+sin_t1) + pow_an1 * (a*sin_t1nt2-sin_t1n1t2)) / (1+aa-a2*cos_t2);
    }
    */

    //----------

    // the following didn't work properly?

    /*
    T process2(T t1, T t2, T a, uint32_t N) {
      t1 *= SAT_PI2;
      t2 *= SAT_PI2;
      T N1          = N + 1;
      T aa          = a * a;
      T a2          = a * 2;
      T sin_t1      = sin(t1);
      T cos_t2      = cos(t2);
      T cos_nt2     = cos(N*t2);
      T cos_n1t2    = cos(N1*t2);
      T pow_a2n1    = pow(a2,N1);
      T result      = sin_t1 * ( 1 - aa - pow_a2n1 * ( cos_n1t2 - a * cos_nt2 ))  /  (1 + aa - a2 * cos_t2);
      // normalization
      //result *= sqrt( (1-a*a) / (1+a*a) );
      return result;
    }
    */

};

/*
  According to Stilson + Smith, this should be
  float s1 = pow(a,N+1.0)*sin((N-1.0)*x+fi);
  Could be a typo though?

  ---

  input x should be the phase, and fi is the initial phase I guess?

  ---

  I'm not so sure that there is a sign typo.
  The author of this code just seems to have an off-by-one definition of N.
  If you expand it all out, it looks like Stilson & Smith's paper, except you
  have N here where S&S had N+1, and you have N-1 where S&S had N.
  I think the code is equivalent. You just have to understand how to choose N
  to avoid aliasing.


*/

//----------------------------------------------------------------------
#endif

