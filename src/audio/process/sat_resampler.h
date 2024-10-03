#ifndef sat_resampler_included
#define sat_resampler_included
//----------------------------------------------------------------------

/*
  based on SampleRateConverter by Wenting Chang
  https://github.com/zephray/SampleRateConverter
  sett bottom of file for license (MIT)
*/

// TODO

//----------------------------------------------------------------------

template <typename T>
class SAT_Resampler {

  typedef enum {
    FIR_LOWPASS,
    FIR_HIGHPASS,
    FIR_BANDPASS,
    FIR_BANDSTOP
  } FIR_Response;

  typedef struct {
    T*  pfb;
    int num_phases;
    int taps_per_phase;
    int interpolation;
    int decimation;
    int phase_index;
    int input_deficit;
    T*  history;
    int history_length;
  } SAT_FirFilter;

//------------------------------
private:
//------------------------------

#define sinc(x)  (x == 0.0) ? (1.0) : (sin(SAT_PI * x)/(SAT_PI * x))
#define sincf(x) (x == 0.0) ? (1.0) : (sinf(SAT_PI * x)/(SAT_PI * x))

//----------

// n/d rounding up

int iceil(int n, int d) {
  int rounding = (n % d != 0) ? 1 : 0;
  return n / d + rounding;
}

//----------

// Dot product of two vectors

T dot(T* a, int a_length, T* history, T* b, int b_last_index) {
  T dotprod = 0.0;
  int i = 0;
  for (; i < (a_length - b_last_index - 1); i++) {
    dotprod += a[i] * history[b_last_index + i];
  }
  for (; i<a_length; i++) {
    dotprod += a[i] * b[b_last_index - a_length + 1 + i];
  }
  return dotprod;
}

//----------

// Shift b into a

void shiftin(T* a, int a_length, T* b, int b_length) {
  if (b_length > a_length) {
    memcpy(a, &b[b_length - a_length], a_length * sizeof(T));
  }
  else {
    for (int i = 0; i < (a_length - b_length); i++) {
      a[i] = a[i + b_length];
    }
    for (int i = 0; i < b_length; i++) {
      a[i + a_length - b_length] = b[i];
    }
  }
}

//----------

// Hamming Window

T* hamming(int num_taps) {
  T* window = malloc(num_taps * sizeof(T));
  for (int i = 0; i < num_taps; i++) {
    T alpha = 0.54;
    T beta = 0.46;
    window[i] = alpha - beta * cos(2.0 * SAT_PI * (T)i / (T)(num_taps - 1));
  }
  return window;
}

//----------

// Hann Window

T* hann(int num_taps) {
  T* window = malloc(num_taps * sizeof(T));
  for (int i = 0; i < num_taps; i++) {
    T alpha = 0.5;
    T beta = 0.5;
    window[i] = alpha - beta * cos(2.0 * SAT_PI * (T)i / (T)(num_taps - 1));
  }
  return window;
}

//----------

// Blackman Window

T* blackman(int num_taps) {
  T* window = malloc(num_taps * sizeof(T));
  for (int i = 0; i < num_taps; i++) {
    T alpha = 0.42;
    T beta = 0.5;
    window[i] = alpha
              - beta * cos(2.0 * SAT_PI * (T)i / (T)(num_taps - 1))
              + (beta - alpha)
              * cos(4.0 * SAT_PI * (T)i / (T)(num_taps - 1));
  }
  return window;
}

//----------

// Generate an FIR prototype
// For LPF and HPF, feed cutoff_low with the cutoff frequency

T* fir_prototype(int num_taps, T cutoff_low, T cutoff_high, FIR_Response response) {
  T* proto = malloc(num_taps * sizeof(T));
  // some alias to help make the code cleaner
  T f = cutoff_low;
  T f1 = cutoff_low;
  T f2 = cutoff_high;
  int m = num_taps - 1;
  switch (response) {
    case FIR_LOWPASS:
      for (int i=0; i<num_taps; i++) 
        proto[i] = 2.0 * f * sinc(2.0 * f * (i - m / 2.0));
      break;
    case FIR_HIGHPASS:
      for (int i=0; i<num_taps; i++)
        proto[i] = sinc(i - m / 2.0) - 2.0 * f * sinc(2.0 * f * (i - m / 2.0));
      break;
    case FIR_BANDPASS:
      for (int i = 0; i < num_taps; i++)
        proto[i] = 2.0 * (f1 * sinc(2.0 * f1 * (i - m / 2.0))
                        - f2 * sinc(2.0 * f2 * (i - m / 2.0)));
      break;
    case FIR_BANDSTOP:
      for (int i=0; i<num_taps; i++)
        proto[i] = 2.0 * (f2 * sinc(2.0 * f2 * (i - m / 2.0))
                        - f1 * sinc(2.0 * f1 * (i - m / 2.0)));
      break;
    default:
      free(proto);
      proto = nullptr;
      SAT_PRINT("Invalid response type!\n");
      break;
  }
  return proto;
}

//----------

// Generate Polyphase filter bank for interpolator and rational

void taps_to_pfb(T* coefficients, int num_taps, int interpolation, T** p_pfb, int* p_num_phases, int* p_taps_per_phase) {
  int num_phases = interpolation;
  int taps_per_phase = iceil(num_taps, num_phases); // iceil(a/b)
  int pfb_size = taps_per_phase * num_phases;
  T* pfb = malloc(pfb_size * sizeof(T));
  int c_index = 0;
  for (int phase=0; phase<num_phases; phase++) {
    for (int tap=0; tap<taps_per_phase; tap++) {
      pfb[phase * taps_per_phase + taps_per_phase - 1 - tap] = coefficients[tap * num_phases + phase];
    }
    // *p_pfb = pfb;
    // *p_num_phases = num_phases;
    // *p_taps_per_phase = taps_per_phase;
  }
  *p_pfb = pfb;
  *p_num_phases = num_phases;
  *p_taps_per_phase = taps_per_phase;
}

//------------------------------
public:
//------------------------------

// Design an FIR filter with windowing

T* generate_fir_coeffs(int num_taps, T cutoff) {
  T* proto = fir_prototype(num_taps, cutoff, 0, FIR_LOWPASS);
  T* window = hann(num_taps);
  for (int i = 0; i < num_taps; i++) {
    proto[i] *= window[i];
  }
  free(window);
  return proto;
}

//----------

// Generate a FIR filter with given coefficients
// It will make a local copy of coefficients

SAT_FirFilter* generate_fir_filter(T* coefficients, int num_taps, int interpolation, int decimation) {
  SAT_FirFilter* flt = malloc(sizeof(SAT_FirFilter));
  taps_to_pfb(coefficients, num_taps, interpolation, &flt->pfb, &flt->num_phases, &flt->taps_per_phase);
  flt->interpolation = interpolation;
  flt->decimation = decimation;
  flt->phase_index = 0;
  flt->input_deficit = 0;
  flt->history_length = flt->taps_per_phase - 1;
  flt->history = malloc(flt->history_length * sizeof(T));
  memset(flt->history, 0, flt->history_length * sizeof(T));
  return flt;
}

//----------

// Generate a filter

SAT_FirFilter* generate(int interpolation, int decimation) {
  int num_taps = 24;
  T cutoff_freq = 0.5 / num_taps;
  T* coefficients = generate_fir_coeffs(num_taps, cutoff_freq);
  SAT_FirFilter* flt = generate_fir_filter(coefficients, num_taps, interpolation, decimation);
  free(coefficients);
  return flt;
}

//----------

// Filt incoming sample block

int filter(SAT_FirFilter* flt, T* samples, int count, T* output) {
  int phase_index_step = flt->decimation % flt->interpolation;
  int phase = flt->phase_index;
  int i = flt->input_deficit;
  int input_step;
  int j = 0;
  while (i < count) {
    output[j++] = dot(
      &flt->pfb[phase * flt->taps_per_phase],
      flt->taps_per_phase, flt->history, samples, i
    );
    i += (phase + flt->decimation) / flt->interpolation;
    phase = (phase + phase_index_step) % flt->interpolation;
  }
  flt->input_deficit = i - count;
  flt->phase_index = phase;
  shiftin(flt->history, flt->history_length, samples, count);
  return j;
}

};

//----------------------------------------------------------------------

/* 
 * Sample Rate Converter
 *
 * Copyright 2019 Wenting Zhang
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE. 
 */

//----------

/*
  Sample Rate Converter
  This is a C implementation of an audio sample rate convertor based on
  Polyphase FIR filter. It can be used to up or downconverting the sample rate
  of a raw audio stream with any fractional ratio. However this may not
  suitable as an arbitrary resampler as memory space consumption goes up
  linearly as the numerator of the ratio goes up. The stream should be in
  de-interleaved 32bit float format. The filter is stateful, capable of
  processing audio stream with arbitrary block size. The code does not depend
  on libraries other than standard math library (-lm). The state is decoupled
  from the algorithm, allowing code re-entrant and multi-instancing.

  Usage
  Simple 44.1kHz to 192kHz upsampler:

    #include "src.h"

    // 44.1kHz * 640 / 147 = 192kHz
    FIR_Filter* filter = src_generate(640, 147); 

    float* input_buffer[IN_SIZE];       // Incoming stream
    float* output_buffer[IN_SIZE * 5];  // Outcoming stream
    int output_length = src_filt(filter, input_buffer, input_length, output_buffer);

  Manually pick the number of taps, cutoff frequency, and run mutiple instances:

    #include "src.h"

    int interpolation = 2;        // x 2
    int decimation = 1;           // / 1
    float cutoff_frequency = 0.5; // half the sampling frequency
    int num_taps = 24 * interpolation;
    float* coefficients = src_generate_fir_coeffs(num_taps, cutoff_frequency / interpolation);
    FIR_Filter* filter_left = src_generate_fir_filter(coefficients, num_taps, interpolation, decimation);
    FIR_Filter* filter_right = src_generate_fir_filter(coefficients, num_taps, interpolation, decimation);
    free(coefficients);
    
    float* input_buffer_left[IN_SIZE];                                    // Incoming stream
    float* output_buffer_left[IN_SIZE * interpolation / decimation + 1];  // Outcoming stream
    int output_length = src_filt(filter_left, input_buffer, input_length, output_buffer);
  
  By default it uses 24 taps per phase and hanning window to construct the
  filter. This might not be the best configuration. For better filtering
  quality, higher taps count and kaiser window may be used.

*/

//----------------------------------------------------------------------
#endif