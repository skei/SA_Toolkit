#ifndef mip_rochebois_waveform_included
#define mip_rochebois_waveform_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// obscure antialiased saw code
// http://www.kvraudio.com/forum/viewtopic.php?p=1710116#p1710116
//----------------------------------------------------------------------

// is it the same as this?
// http://www.musicdsp.org/showArchiveComment.php?ArchiveID=215

//float sr      = 44100.0f;
//float freq    = 440.0f;
//float phase   = 0.0f;
//float dphase  = freq * (2.0f/sr);
//float phase += dphase;
//if (phase>=1.0) phase-=2.0;

//----------

class MIP_RocheboisSawWaveform {
  private:
    float aw0, ax0, ay0, az0;
    float aw1, ax1, ay1, az1;
    float dpw0, dpx0, dpy0, dpz0;
    float dpw1, dpx1, dpy1, dpz1;
  public:
    MIP_RocheboisSawWaveform() {
      aw0 = 0.0f;   ax0 = 0.0f;   ay0 = 0.0f;   az0 = 0.0f;
      aw1 = 0.0f;   ax1 = 0.0f;   ay1 = 0.0f;   az1 = 0.0f;
      dpw0 = 1.0f;  dpx0 = 1.0f;  dpy0 = 1.0f;  dpz0 = 1.0f;
      dpw1 = 1.0f;  dpx1 = 1.0f;  dpy1 = 1.0f;  dpz1 = 1.0f;
    }
    float process(float t, float dt) {
      t           = t * 2.0f - 1.0f; // 0..1 -> -1..1 -> phasor -> naive saw
      aw0         = t * ( 7.0f/360.0f + t*t * ( -1.0f/36.0f + t*t * (1.0f/120.0f)));
      dpw0        = dt;
      ax0         = (aw1-aw0) / dpw0;   dpx0 = 0.5f * (dpw0+dpw1);  // ax0 is calculated on the short integration segment dpw0 = dphase.
      ay0         = (ax1-ax0) / dpx0;   dpy0 = 0.5f * (dpx0+dpx1);  // ay0 is calculated on the short integration segment dpx0.
      az0         = (ay1-ay0) / dpy0;   dpz0 = 0.5f * (dpy0+dpy1);  // az0 is calculated on the short integration segment dpy0.
      float out   = (az1-az0) / dpz0;                               // fout is calculated on the short integration segment dpz0.
      //float compensation = 1.0f / (dt*dt*dt*dt);
      //out /= compensation;
      out *= dt; // --> hmmm... -600db signal??? but looks ok in fft analyzer..
      aw1  = aw0;   ax1  = ax0;   ay1  = ay0;    az1 = az0;
      dpw1 = dpw0;  dpx1 = dpx0;  dpy1 = dpy0;  dpz1 = dpz0;
      return out;
    }
    float operator () (float t, float dt) {
      return process(t,dt);
    }
};


//----------------------------------------------------------------------
#endif
