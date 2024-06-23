#ifndef mip_phasor_included
#define mip_phasor_included
//----------------------------------------------------------------------

// 0..1

//----------------------------------------------------------------------

#include "base/mip_math.h"
#include "audio/mip_audio_math.h"

//----------

class MIP_Phasor {

  private:
    float MSampleRate = 0.0f;

  public:
    float t   = 0.0f;
    float dt  = 0.0f;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //KPhasor() {
    //  t = 0;
    //  dt = 0;
    //  MSampleRate = 0;
    //}

  //----------------------------------------
  // get/set
  //----------------------------------------

  public:

    void setSampleRate(float ASampleRate) {
      MSampleRate = ASampleRate;
    }

    //----------

    void setHz(float AFreq) {
      dt = AFreq / MSampleRate;
      //KTrace("AFreq %.2f MSampleRate %.2f dt %.2f\n",AFreq,MSampleRate,dt);
    }

    //----------

    void addHz(float AFreq) {
      dt += AFreq / MSampleRate;
    }

    //----------

    void setHz(float AFreq, float ASampleRate) {
      dt = AFreq / ASampleRate;
    }

    //----------

    void addHz(float AFreq, float ASampleRate) {
      dt += AFreq / ASampleRate;
    }

  //----------------------------------------
  // internal
  //----------------------------------------

  private:

    float tick_sync(void) {
      float out_sync = -2.0f;
      //float t_ = t;
      t += dt;
      if (t >= 1.0f) {
        t -= 1.0f;
        out_sync = t / dt;
      }
      else if (t > (1.0f - dt)) {
        out_sync = -(1.0f - t) / dt;
      }
      return out_sync;
    }

    //----------

    float tick_sync(float input_sync) {
      float out_sync = 0.0f;
      //float t_ = t;
      t += dt;
      if (input_sync >= 0.0) {
        t = input_sync * dt;
        out_sync = t / dt;
      }
      else if (t >= 1.0f) {
        t -= 1.0f;
        out_sync = t / dt;
      }
      else if (t > (1.0f - dt)) {
        out_sync = -(1.0f - t) / dt;
      }
      else {
        out_sync = input_sync;
      }
      return out_sync;
    }

    //----------

    void tick(void) {
      t += dt;
      t = MIP_Modulo(t,1.0f);
    }

    //----------

    void tick(float a) {
      t += a;
      t = MIP_Modulo(t,1.0f);
    }

  //----------------------------------------
  // process
  //----------------------------------------

  public:

    float process(void) {
      //MIP_Trace("t %.3f dt %.3f\n",t,dt);
      float result = t;
      //t += dt;
      //t = KModulo(t,1.0f);
      tick();
      return result;
    }

    //----------

    float process(float AMod) {
      float result = t;
      //t += (dt + AMod);
      //t = KModulo(t,1.0f);
      tick(dt + AMod);
      return result;
    }

    //----------

    void skip(int32_t ASteps) {
      //t += (dt*ASteps);
      //t = KModulo(t,1.0f);
      tick(dt + ASteps);
    }

};

//----------------------------------------------------------------------
#endif
