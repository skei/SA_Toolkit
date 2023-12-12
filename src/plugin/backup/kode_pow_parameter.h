#ifndef kode_pow_parameter_included
#define kode_pow_parameter_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_parameter.h"
#include "plugin/parameters/kode_float_parameter.h"

// value^n

// appendParameter( new KODE_ParamPow( "pow/post", 440,  2,true,     20, 20000, 1 )); // n^2 = 20.20000, default = 440
// appendParameter( new KODE_ParamPow( "pow",      5,    2,false,    1,  10,    1 ));    // 1^2..10^2, default = 5^2

class KODE_PowParameter
: public KODE_FloatParameter {

private:

  float   MPower  = 1.0f;
  bool    MPost   = true; // if true, calc ^n after range/scale conversion, else calc before..

public:

  KODE_PowParameter(const char* AName, float AVal, float APower, bool APost=false, float AMin=0.0f, float AMax=1.0f, float AStep=0.0f)
  : KODE_FloatParameter(AName,AVal,AMin,AMax,AStep) {
    MPower = APower;
    MPost = APost;
    setup(AVal);
//    //MDefValue = to01(MDefValue);
//    if (APost) {
//      float n = ((AVal - MMinValue) * MInvRange);
//      if (MPower > 0) MDefValue = powf(n,1.0f/MPower);
//      else MDefValue = 0.0f;
//    }
//    else {
//      //if (MPower > 0) AVal = powf(AVal,1.0f/MPower);
//      MDefValue = ((AVal - MMinValue) * MInvRange);
//    }
  }

  KODE_PowParameter(const char* AName, const char* ALabel, float AVal, float APower, bool APost=false, float AMin=0.0f, float AMax=1.0f, float AStep=0.0f)
  : KODE_FloatParameter(AName,ALabel,AVal,AMin,AMax,AStep) {
    MPower = APower;
    MPost = APost;
    setup(AVal);
  }
private:

  void setup(float AVal) {
    //MDefValue = to01(MDefValue);
    if (MPost) {
      float n = ((AVal - MMinValue) * MInvRange);
      if (MPower > 0) MDefValue = powf(n,1.0f/MPower);
      else MDefValue = 0.0f;
    }
    else {
      //if (MPower > 0) AVal = powf(AVal,1.0f/MPower);
      MDefValue = ((AVal - MMinValue) * MInvRange);
    }
  }

public:

  float to01(float AValue) override {
    float result = 0.0f;
    if (MPost) {
      float n = KODE_FloatParameter::to01(AValue);
      if (MPower > 0) result = powf(n,1.0f/MPower);
    }
    else {
      if (MPower > 0) AValue = powf(AValue,1.0f/MPower);
      result = KODE_FloatParameter::to01(AValue);
    }
    return result;
  }

  //----------

  float from01(float AValue) override {
    float result = 0.0f;
    if (MPost) {
      float n = powf(AValue,MPower);
      result = KODE_FloatParameter::from01(n);
    }
    else {
      float n = KODE_FloatParameter::from01(AValue);
      result = powf(n,MPower);
    }
    return result;
  }

};

//----------------------------------------------------------------------
#endif
