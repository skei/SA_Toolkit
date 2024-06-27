#ifndef mip_oscillator_included
#define mip_oscillator_included
//----------------------------------------------------------------------

/*
  todo: phase mod & sync -> KPhasor
*/

class MIP_Oscillator {

private:

    float   t           = 0.0f;
    //float   t2          = 0.0f;
    float   hz          = 440.0f     ;
    float   sample_rate = 44100.0f;

    float   pm          = 0.0f;
    bool    sync        = false;
    float*  pm_buffer   = nullptr;
    float*  sync_buffer = nullptr;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void setFreq(float freq) {
      hz = freq;
    }

    void setModulation(float* _pm_buf, float* _sync_buf, float _pm, bool _sync) {
      pm_buffer   = _pm_buf;
      sync_buffer = _sync_buf;
      pm          = _pm;
      sync        = _sync;
    }

    void setSamplerate(float r) {
      sample_rate = r;
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //void clear() {
    //  t   = 0; // start;
    //  //t2  = 0; // start;
    //  hz  = 440.0;
    //}

    //----------

    void reset() {
      t   = 0; // start;
      //t2  = 0; // start;
    }

    //----------

    // 0..1 -> -1..1

    float to_signed(float x) {
      return 2.0f * x - 1.0f;
    }

    //----------

    // phase mod

    // t2 -> t_

    float phase_mod(float t, int i) {
      float t_ = t + pm * pm_buffer[i];
      if (t_ < 0.0f) t_ = 1.0f - fmod(fabs(t_), 1.0f);
      else if (t_ >= 1.0f) t_ = fmod(t_, 1.0f);
      return t_;
    }

    //----------

    float polyblep(float t) {
      if (t > 0.0f) return t - (t*t)/2.0f - 0.5f;
      else return (t*t)/2.0f + t + 0.5f;
    }

    //----------

    float saw_sync(float t, float t_, float dt, float sync) {
      float mod = 0.0f;
      if (sync >= 0.0f) mod = (t_ + dt - t) * polyblep(sync); // sync start
      else if (sync > -1.0f) mod = (t + (1.0 + sync) * dt) * polyblep(sync); // sync end
      else if (t < dt) mod = polyblep(t / dt); // start
      else if (t > (1.0f - dt)) mod = polyblep((t - 1.0f) / dt); // end
      return mod;
    }

    //----------

    float saw_polyblep(float t, float dt) {
      float mod = 0.0f;
      if (t < dt) mod = polyblep(t / dt); // start
      else if (t > (1.0f - dt)) mod = polyblep((t - 1.0f) / dt); // end
      return mod;
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void process_saw(float* output, float* out_sync, int samples) {
      float dt = hz / sample_rate;
      for (int32_t i = 0; i < samples; i++) {
        //t2 = t;
        t += dt;
        if (t >= 1.0f) {
          t -= 1.0f;
          out_sync[i] = t / dt;
        } else if (t > (1.0 - dt)) {
          out_sync[i] = -(1.0 - t) / dt;
        } else {
          out_sync[i] = -2.0;
        }
        float mod = saw_polyblep(t, dt);
        output[i] = to_signed(t - mod);
      }
    }

    void process_saw_sync(float* output, float* out_sync, int samples) {
      float dt = hz / sample_rate;
      for (int32_t i = 0; i < samples; i++) {
        float t2 = t;
        t += dt;
        if (sync_buffer[i] >= 0.0) {
          t = sync_buffer[i] * dt;
          out_sync[i] = t / dt;
        }  else if (t >= 1.0f) {
          t -= 1.0f;
          out_sync[i] = t / dt;
        } else if (t > (1.0 - dt)) {
          out_sync[i] = -(1.0 - t) / dt;
        } else {
          out_sync[i] = sync_buffer[i];
        }
        float mod = saw_sync(t, t2, dt, sync_buffer[i]);
        output[i] = to_signed(t - mod);
      }
    }

    void process_saw_pm(float* output, float* out_sync, int samples) {
      float dt = hz / sample_rate;
      for (int32_t i = 0; i < samples; i++) {
        //t2 = t;
        t += dt;
        if (t >= 1.0f) {
          t -= 1.0f;
          out_sync[i] = t / dt;
        } else if (t > (1.0 - dt)) {
          out_sync[i] = -(1.0 - t) / dt;
        } else {
          out_sync[i] = -2.0;
        }
        float t = phase_mod(this->t, i);
        output[i] = to_signed(t);
      }
    }

    void process_saw_pm_sync(float* output, float* out_sync, int samples) {
      float dt = hz / sample_rate;
      for (int32_t i = 0; i < samples; i++) {
        //t2 = t;
        t += dt;
        if (sync_buffer[i] >= 0.0) {
          t = sync_buffer[i] * dt;
          out_sync[i] = t / dt;
        }  else if (t >= 1.0f) {
          t -= 1.0f;
          out_sync[i] = t / dt;
        } else if (t > (1.0 - dt)) {
          out_sync[i] = -(1.0 - t) / dt;
        } else {
          out_sync[i] = sync_buffer[i];
        }
        float t = phase_mod(this->t, i);
        output[i] = to_signed(t);
      }
    }

    //----------

    void process(float* output, float* out_sync, int samples) {
      if (pm > 0.0f) {
        if (sync) process_saw_pm_sync(output,out_sync,samples);
        else process_saw_pm(output,out_sync,samples);
      } else if (sync) process_saw_sync(output,out_sync,samples);
      else process_saw(output,out_sync,samples);
    }


};

//----------------------------------------------------------------------
#endif
