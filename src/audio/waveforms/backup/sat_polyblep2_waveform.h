#ifndef sat_polyblep2_waveform_included
#define sat_polyblep2_waveform_included
//----------------------------------------------------------------------

float SAT_PolyBlep(float t) {
  if (t > 0.0f) {
    return t - (t*t)/2.0f - 0.5f;
  } else {
    return (t*t)/2.0f + t + 0.5f;
  }
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

float SAT_PolyBlepSawWaveform(float t, float dt) {
  float mod = 0.0f;
  if (t < dt) { // start
    mod = SAT_PolyBlep(t/dt);
  } else if (t > (1.0f - dt)) { // end
    mod = SAT_PolyBlep((t - 1.0f) / dt);
  }
  return mod;
}

//----------

// w = width

float SAT_PolyBlepPulseWaveform(float t, float dt, float w) {
  float mod = 0.0f;
  if (t < w) {
    if (t < dt) { // start
      mod = SAT_PolyBlep(t / dt);
    } else if (t > (w - dt)) {
      mod = -SAT_PolyBlep( (t - w) / dt);
    }
  } else {
    if (t > (1.0f - dt)) { // end
      mod = SAT_PolyBlep( (t - 1.0f) / dt);
    } else if (t < (w + dt)) {
      mod = -SAT_PolyBlep( (t - w) / dt);
    }
  }
  return mod;
}

//----------------------------------------------------------------------
// sync variants
//----------------------------------------------------------------------

// s = sync

float SAT_PolyBlepSawSyncWaveform(float t, float t_, float dt, float s) {
  float mod = 0.0f;
  if (s >= 0.0f) { // sync start
    mod = (t_ + dt - t) * SAT_PolyBlep(s);
  } else if (s > -1.0f) { // sync end
    mod = (t + (1.0 + s) * dt) * SAT_PolyBlep(s);
  } else if (t < dt) { // start
    mod = SAT_PolyBlep(t / dt);
  } else if (t > (1.0f - dt)) { // end
    mod = SAT_PolyBlep((t - 1.0f) / dt);
  }
  return mod;
}

//----------

float SAT_PolyBlepPulseSyncWaveform(float t, float t_, float dt, float w, float s) {
  float mod = 0.0f;
  if (t < w) {
    if (t < dt) { // start
      if (s < 0.0f || t_ >= w) {
        mod = SAT_PolyBlep(t / dt);
      }
    } else if (t > (w - dt)) {
      if (s < -1.0f) {
        mod = -SAT_PolyBlep( (t - w) / dt);
      }
    }
  } else {
    if (t > (1.0f - dt)) { // end
      mod = SAT_PolyBlep( (t - 1.0f) / dt);
    } else if (t < (w + dt)) {
      mod = -SAT_PolyBlep( (t - w) / dt);
    } else if (s > -1.0f) {
      mod = SAT_PolyBlep(s);
    }
  }
  return mod;
}

//----------------------------------------------------------------------



//----------------------------------------------------------------------
#endif

