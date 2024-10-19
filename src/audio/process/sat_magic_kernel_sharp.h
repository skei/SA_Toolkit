#ifndef sat_magic_kernel_sharp_included
#define sat_magic_kernel_sharp_included
//----------------------------------------------------------------------

// Magic Kernel Sharp
// https://johncostella.com/magic/#code
// MIT-0
// see bottom of file

//----------------------------------------------------------------------

template <typename T>
class SAT_MagicKernelSharp {

//------------------------------
private:
//------------------------------

  const T m1 = 31927.0 / 61440;
  const T m2 = 6719.0  / 20480;
  const T m3 = 15349.0 / 122880;
  const T m4 = 3119.0  / 122880;
  const T m5 = 81.0    / 40960;
  const T m6 = 1.0     / 122880;

  const T s0 =  8415120629.0  / 2952708336;
  const T s1 = -3913360945.0  / 2952708336;
  const T s2 =  20369223277.0 / 35432500032;
  const T s3 = -4387620835.0  / 17716250016;
  const T s4 =  1887357215.0  / 17716250016;
  const T s5 = -808049411.0   / 17716250016;
  const T s6 =  674477135.0   / 35432500032;
  const T s7 = -3769012.0     / 553632813;

  T halver[12]    = {};
  T doubler[2][6] = {};
  T sharp[15]     = {};

  T temp_samples[65536] = {};

//------------------------------
public:
//------------------------------

  SAT_MagicKernelSharp() {
    halver[0]     = halver[11]    = m6 / 2.0;
    halver[1]     = halver[10]    = m5 / 2.0;
    halver[2]     = halver[9]     = m4 / 2.0;
    halver[3]     = halver[8]     = m3 / 2.0;
    halver[4]     = halver[7]     = m2 / 2.0;
    halver[5]     = halver[6]     = m1 / 2.0;
    doubler[0][0] = doubler[1][5] = m5;
    doubler[0][1] = doubler[1][4] = m3;
    doubler[0][2] = doubler[1][3] = m1;
    doubler[0][3] = doubler[1][2] = m2;
    doubler[0][4] = doubler[1][1] = m4;
    doubler[0][5] = doubler[1][0] = m6;
    sharp[0]      = sharp[14]     = s7;
    sharp[1]      = sharp[13]     = s6;
    sharp[2]      = sharp[12]     = s5;
    sharp[3]      = sharp[11]     = s4;
    sharp[4]      = sharp[10]     = s3;
    sharp[5]      = sharp[9]      = s2;
    sharp[6]      = sharp[8]      = s1;
    sharp[7]      = s0;
  }

  //----------

  // 2 -> 1

  T downsample(uint32_t  count, T *in_samples, T *out_samples) {
    // uint32_t rate_out = rate_in / 2;
    uint32_t num_temp = (count - 10) / 2;
    for (uint32_t u=0; u<num_temp; u++) {
      T signal = 0;
      for (uint32_t v=0; v<12; v++) {
        signal += halver[v] * in_samples[(u*2) + v];
      }
      temp_samples[u] = signal;
    }
    
    uint32_t num_out = num_temp - 14;
    for (uint32_t u=0; u<num_out; u++) {
      T signal = 0.0;
      for (uint32_t v=0; v<15; v++) {
        signal += sharp[v] * temp_samples[u + v];
      }
      out_samples[u] = signal;
    }

  }
  
  //----------

  // 1 -> 2
  
  void upsample(uint32_t  count, T *in_samples, T *out_samples) {
    // uint32_t rate_out = rate_in * 2;
    uint32_t num_temp = count - 14;
    for (uint32_t u=0; u<num_temp; u++) {
      T signal = 0.0;
      for (uint32_t v=0; v<15; v++) {
        signal += sharp[v] * in_samples[u + v];
      }
      temp_samples[u] = signal;
    }
    uint32_t num_out = (2 * num_temp) - 10;
    for (uint32_t u = 0; u < num_out;) {
      for (uint32_t o=0; o<2; o++, u++) {
        T signal = 0.0;
        for (uint32_t v=0; v<6; v++) {
          signal += doubler[o][v] * temp_samples[u / 2 + v];
        }
        out_samples[u] = signal;
      }
    }
  }

};

//----------------------------------------------------------------------
#endif




#if 0

/*
  Copyright (c) 1989-2024 John Costella

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
  of the Software, and to permit persons to whom the Software is furnished to do
  so, subject to the following conditions:

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "wav_double_halve.h"
#include <math.h>

int main(int argc, char *argv[]) {

  // args_description("Upsample or downsample a 16-bit PCM WAV file by a factor of 2");
  // args_string("input-filename", "Input 16-bit PCM WAV file");
  // args_string("output-filename", "Output WAV file to create");
  // args_flag("halve", 'H', "Halve, i.e. downsample (e.g. from 96kHz to 48kHz)");
  // args_flag("double", 'd', "Double, i.e. upsample (e.g. from 48kHz to 96kHz)");
  // args_parse(argc, argv);
  //
  // if (args_set_veto("halve", "double")) {
  //   halve = true;
  // } else if (args_set("double")) {
  //   halve = false;
  // } else {
  //   throw("Must specify either --halve or --double");
  // }

  halve = true; // false;

  //double_audio_new_load_wav(&_in, args_get_string("input-filename"));

  n_in = _in->n;
  rate_in = _in->rate;
  in_samples = _in->_samples;

  if (halve) {
    // if (rate_in < 96000) {
    //   throw("Program not intended to downsample to less than 48kHz");
    // }
    n_temp = (n_in - 10) / 2;
    rate_out = rate_in / 2;

    double_audio_new(&_temp, n_temp, rate_out);
    temp_samples = _temp->_samples;

    for (u = 0; u < n_temp; u++) {
      for (signal = 0, v = 0; v < 12; v++) {
        signal += halver[v] * in_samples[2 * u + v];
      }
      temp_samples[u] = signal;
    }
    n_out = n_temp - 14;

    double_audio_new(&_out, n_out, rate_out);
    out_samples = _out->_samples;

    for (u = 0; u < n_out; u++) {
      for (signal = 0, v = 0; v < 15; v++) {
        signal += sharp[v] * temp_samples[u + v];
      }
      out_samples[u] = signal;
    }

  } else {
    n_temp = n_in - 14;

    double_audio_new(&_temp, n_temp, rate_in);
    temp_samples = _temp->_samples;

    for (u = 0; u < n_temp; u++) {
      for (signal = 0, v = 0; v < 15; v++) {
        signal += sharp[v] * in_samples[u + v];
      }
      temp_samples[u] = signal;
    }
    n_out = 2 * n_temp - 10;
    rate_out = rate_in * 2;

    double_audio_new(&_out, n_out, rate_out);
    out_samples = _out->_samples;

    for (u = 0; u < n_out;) {
      for (o = 0; o < 2; o++, u++) {
        for (signal = 0, v = 0; v < 6; v++) {
          signal += doubler[o][v] * temp_samples[u / 2 + v];
        }
        out_samples[u] = signal;
      }
    }
  }

  // double_audio_save_wav(_out, args_get_string("output-filename"));
  // double_audio_delete(&_out);
  // double_audio_delete(&_temp);
  // double_audio_delete(&_in);

  return 0;
}

#endif // 0

