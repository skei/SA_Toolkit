#ifndef mip_table_waveform_included
#define mip_table_waveform_included
//----------------------------------------------------------------------

/*
  based on:
  http://www.earlevel.com/main/category/digital-audio/oscillators/wavetable-oscillators/?order=ASC
  http://www.earlevel.com/main/2013/03/03/replicating-wavetables/

  more:
  http://www.kvraudio.com/forum/viewtopic.php?p=5903494#p5903494
  http://www.kvraudio.com/forum/viewtopic.php?p=5909834#p5909834

  "I know that it is possible to calculate and inverse fft from a forward fft
  implementation(not a specific inverse fft like those available in fftw) by
  swapping the real and imaginary arrays. i.e putting the saw spectrum into the
  real array (cosine ?) will result in the fft filling the imaginary/ai array
  with the real valued x[n] time domain components leaving the ar/real array as
  zero values afterwards ? "

  "Yes, you have it right - the saw is an odd function, hence the negative symmetry.
  And yes, the imaginary part corresponds to sine; if you want to specify a phase
  other than sine or cosine, you need a combination or value in real and
  imaginary."

  "is it possible to add index array for wavetables for scanning synthesis so that
  the wavetable can change over time
  like the ppg system"

  "Yes, absolutely. The oscillator has one dimension of wavetables, and you would
  add another, to implement multiple sets of wavetables per oscillator. If you
  want to avoid clicks as you shift through wavetables, you could crossfade
  (interpolate) between sets."

  -

  I understand the concepts of the harmonic content or the individual waveforms
  as far as fourier theory and sinusoidal basis functions are concerned but am at
  a bit of a loss as to how to calculate the specturm/arguments to pass to the FFT
  routine.
  Do you have any advice or articles you might point me towards to get a better
  understanding of this?
  i.e how to work out the following
  freqWaveRe[idx] = 1.0 / idx;
  Is it simply the amplitude value/signal components strength that is calculated
  here?
  Basically id like to start experimenting with other wave shapes similar to
  modern Soft Synths like SuperSaw's etc etc.
  Many thanks again for the brilliant tutorials.

  -

  We're just setting the amplitude of a given harmonic here. For a sawtooth wave,
  the harmonics drop off as the reciprocal of the harmonic number:
    1st harmonic: 1.0 (1/1)
    2nd harmonic: 0.5 (1/2)
    3rd harmonic: 0.33333.. (1/3)
    4th harmonic: 0.25 (1/4)
    ...
*/

//----------

/*
  . osc
    frequency of note decies which table to use
    interpolate between tables
    one (or more) tables per octavle

  - wavetable
    multiple waveforms that can be scrolled/modulated through..
    resampling from table
    one table per voice/sound, movement of sound

  - scanned synthesis

  - create tables
  - interpolation / resampling

*/

//----------------------------------------------------------------------

#include <math.h>

#define MIP_WAVETABLE_LINEAR_INTERPOLATION
#define MIP_WAVETABLE_MAX_TABLES 32

/*
  2k * 32 = 64k * sizeof(float) = 256k
  scanned = *256 = up to 64mb :-/
*/

//----------

typedef struct {
  float   topFreq;
  int32_t   len;
  float   *table;
} MIP_WaveTable;

//----------------------------------------------------------------------

class MIP_TableWaveform {

  private:

    int32_t         MNumTables;
    MIP_WaveTable  MTables[MIP_WAVETABLE_MAX_TABLES];

  //------------------------------
  //
  //------------------------------

  public:

    MIP_TableWaveform() {
      MNumTables = 0;
      for (int32_t i=0; i<MIP_WAVETABLE_MAX_TABLES; i++) {
        MTables[i].topFreq = 0;
        MTables[i].len = 0;
        MTables[i].table = nullptr;
      }
    }

    //----------

    ~MIP_TableWaveform() {
      for (int32_t i=0; i<MIP_WAVETABLE_MAX_TABLES; i++) {
        float *temp = MTables[i].table;
        if (temp) MIP_Delete [] temp;
      }
    }

  //------------------------------
  //
  //------------------------------

  public:

    int32_t numTables(void) { return MNumTables; }

  //------------------------------
  //
  //------------------------------

  private:

    /*
      fft
      I grabbed (and slightly modified) this Rabiner & Gold translation...
      (could modify for real data, could use a template version, blah blah--just keeping it short)
    */

    void fft(int32_t N, float *ar, float *ai) {
      /*
        in-place complex fft
        After Cooley, Lewis, and Welch; from Rabiner & Gold (1975)
        program adapted from FORTRAN
        by K. Steiglitz  (ken@princeton.edu)
        Computer Science Dept.
        Princeton University 08544
      */
      int32_t i, j, k, L;             // indexes
      int32_t M, TEMP, LE, LE1, ip;   // M = log N
      int32_t NV2, NM1;
      float t;                    // temp
      float Ur, Ui, Wr, Wi, Tr, Ti;
      float Ur_old;
      // if ((N > 1) && !(N & (N - 1)))   // make sure we have a power of 2
      NV2 = N >> 1;
      NM1 = N - 1;
      TEMP = N; // get M = log N
      M = 0;
      while (TEMP >>= 1) ++M;
      // shuffle
      j = 1;
      for (i = 1; i <= NM1; i++) {
        if(i<j) {             // swap a[i] and a[j]
          t = ar[j-1];
          ar[j-1] = ar[i-1];
          ar[i-1] = t;
          t = ai[j-1];
          ai[j-1] = ai[i-1];
          ai[i-1] = t;
        }
        k = NV2;             // bit-reversed counter
        while(k < j) {
          j -= k;
          k /= 2;
        }
        j += k;
      }
      LE = 1.;
      for (L = 1; L <= M; L++) {            // stage L
        LE1 = LE;                         // (LE1 = LE/2)
        LE *= 2;                          // (LE = 2^L)
        Ur = 1.0;
        Ui = 0.;
        Wr = cos(M_PI/(float)LE1);
        Wi = -sin(M_PI/(float)LE1); // Cooley, Lewis, and Welch have "+" here
        for (j = 1; j <= LE1; j++) {
          for (i = j; i <= N; i += LE) { // butterfly
            ip = i+LE1;
            Tr = ar[ip-1] * Ur - ai[ip-1] * Ui;
            Ti = ar[ip-1] * Ui + ai[ip-1] * Ur;
            ar[ip-1] = ar[i-1] - Tr;
            ai[ip-1] = ai[i-1] - Ti;
            ar[i-1]  = ar[i-1] + Tr;
            ai[i-1]  = ai[i-1] + Ti;
          }
          Ur_old = Ur;
          Ur = Ur_old * Wr - Ui * Wi;
          Ui = Ur_old * Wi + Ui * Wr;
        }
      }
    }

  //------------------------------
  //
  //------------------------------

  public:

    /*
      add wavetables in order of lowest frequency to highest
      topFreq is the highest frequency supported by a wavetable
      wavetables within an oscillator can be different lengths
      returns 0 upon success,
      or the number of wavetables if no more room is available

      skei:
      allocates memory & copies content of tableIn
      deleted in destructor
    */

    int32_t addTable(int32_t len, float *tableIn, float topFreq) {
      if (MNumTables < MIP_WAVETABLE_MAX_TABLES) {
        float* T = MTables[MNumTables].table = MIP_New float[len];
        MTables[MNumTables].len = len;
        MTables[MNumTables].topFreq = topFreq;
        ++MNumTables;
        for (long idx = 0; idx < len; idx++) T[idx] = tableIn[idx];
        return 0;
      }
      return MNumTables;
    }

    //----------

    /*
      if scale is 0, auto-scales
      returns scaling factor (0.0 if failure), and wavetable in ai array

      skei:
      allocates & copies memory..
    */

    float makeTable(int32_t len, float *ar, float *ai, float scale, float topFreq) {
      fft(len, ar, ai);
      if (scale == 0.0) {
        // calc normal
        float max = 0;
        for (int32_t idx = 0; idx < len; idx++) {
          float temp = fabs(ai[idx]);
          if (max < temp) max = temp;
        }
        scale = 1.0 / max * .999;
      }
      // normalize
      float* wave = MIP_New float[len];
      for (int32_t idx = 0; idx < len; idx++) wave[idx] = ai[idx] * scale;
      if (addTable(len,wave,topFreq)) scale = 0.0;
      // skei: delete wave ???
      MIP_Delete [] wave;
      return scale;
    }

    //----------

    /*
      The main function of interest here; Fills the oscillator with all
      wavetables necessary for full-bandwidth operation.
    */

    void fillTables(float *freqRe, float *freqIm, int32_t numSamples) {
      int32_t idx;
      // zero DC offset and Nyquist
      freqRe[0] = freqIm[0] = 0.0;
      freqRe[numSamples >> 1] = freqIm[numSamples >> 1] = 0.0;
      // determine maxHarmonic, the highest non-zero harmonic in the wave
      int32_t maxHarmonic = numSamples >> 1;
      const float minVal = 0.000001; // -120 dB
      while ((fabs(freqRe[maxHarmonic]) + fabs(freqIm[maxHarmonic]) < minVal) && maxHarmonic) --maxHarmonic;
      // calculate topFreq for the initial wavetable
      // maximum non-aliasing playback rate is 1 / (2 * maxHarmonic), but we allow aliasing up to the
      // point where the aliased harmonic would meet the next octave table, which is an additional 1/3
      float topFreq = 2.0 / 3.0 / maxHarmonic;
      // for subsquent tables, float topFreq and remove upper half of harmonics
      float *ar = MIP_New float [numSamples];
      float *ai = MIP_New float [numSamples];
      float scale = 0.0;
      while (maxHarmonic) {
        // fill the table in with the needed harmonics
        for (idx = 0; idx < numSamples; idx++) ar[idx] = ai[idx] = 0.0;
        for (idx = 1; idx <= maxHarmonic; idx++) {
          ar[idx] = freqRe[idx];
          ai[idx] = freqIm[idx];
          ar[numSamples - idx] = freqRe[numSamples - idx];
          ai[numSamples - idx] = freqIm[numSamples - idx];
        }
        // make the wavetable
        scale = makeTable(numSamples,ar,ai,scale,topFreq);
        // prepare for next table
        topFreq *= 2;
        maxHarmonic >>= 1;
      }
      // skei: delete ar/ai ???
      MIP_Delete [] ar;
      MIP_Delete [] ai;
    }

  //------------------------------
  //
  //------------------------------

  public:

    // builds a sawtooth oscillator via frequency domain

    //WaveTableOsc*
    void makeSaw(int32_t tableLen=2048) {
      //int32_t tableLen = 2048;    // to give full bandwidth from 20 Hz
      int32_t idx;
      float *freqRe = MIP_New float [tableLen];
      float *freqIm = MIP_New float [tableLen];
      // make a sawtooth
      for (idx = 0; idx < tableLen; idx++) { freqIm[idx] = 0.0; }
      freqRe[0] = freqRe[tableLen >> 1] = 0.0;
      for (idx = 1; idx < (tableLen >> 1); idx++) {
        freqRe[idx] = 1.0 / idx;                  // sawtooth spectrum
        freqRe[tableLen - idx] = -freqRe[idx];    // mirror
      }
      // build a wavetable oscillator
      fillTables(freqRe,freqIm,tableLen);
      // skei: free freqRe/Im ???
      MIP_Delete [] freqRe;
      MIP_Delete [] freqIm;
    }

    //----------

    // creates an oscillator from an arbitrary time domain wave

    void makeWave(float *waveSamples, int32_t tableLen) {
      float *freqRe = MIP_New float [tableLen];
      float *freqIm = MIP_New float [tableLen];
      // take FFT
      for (int32_t idx = 0; idx < tableLen; idx++) {
        freqIm[idx] = waveSamples[idx];
        freqRe[idx] = 0.0;
      }
      fft(tableLen, freqRe, freqIm);
      // build a wavetable oscillator
      fillTables(freqRe,freqIm,tableLen);
      // skei: free freqRe/Im ???
      //MIP_Delete [] freqRe;
      //MIP_Delete [] freqIm;
    }

  //------------------------------
  //
  //------------------------------

  public:

    /*
      * the 'while 'searching for the correct wavetable could be done
        elsewhere, to avoid doing it per sample.. when note is pressed,
        or per block ???

      * better interpolation
      * bilinear interpolation between tables?
    */

    float process(float t, float dt=0) {
      // grab the appropriate wavetable
      int32_t tableIdx = 0;
      while ((dt >= MTables[tableIdx].topFreq) && (tableIdx < (MNumTables - 1))) { ++tableIdx; }
      MIP_WaveTable* wavetable = &MTables[tableIdx];

      #ifdef MIP_WAVETABLE_LINEAR_INTERPOLATION

        float temp = t * wavetable->len;
        int32_t intPart = temp;
        float fracPart = temp - intPart;
        float samp0 = wavetable->table[intPart];
        if (++intPart >= wavetable->len) intPart = 0;
        float samp1 = wavetable->table[intPart];
        return samp0 + (samp1 - samp0) * fracPart;

      #else // truncate

        return wavetable->table[ int32_t(t * wavetable->len) ];

      #endif

    }

    //----------

};

//#undef MIP_WAVETABLE_LINEAR_INTERPOLATION
#undef MIP_WAVETABLE_MAX_TABLES

//----------------------------------------------------------------------
#endif
