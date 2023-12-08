#ifndef sa_reverb_included
#define sa_reverb_included
//----------------------------------------------------------------------

// TODO

// signalsmith reverb (see /extern/sigmalsmith)
// https://github.com/Signalsmith-Audio

#include "sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"

#include "extern/signalsmith/dsp/delay.h"

//#include <cstdlib>
//#include <cmath>

//----------

#define PLUGIN_NAME "sa_reverb"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template<int channels=8, int stepCount=4, bool averageOutputs=true>
class SignalsmithReverb {

  // This is a simple delay class which rounds to a whole number of samples.
  using Delay = signalsmith::delay::Delay<double, signalsmith::delay::InterpolatorNearest>;

  //----------

  // struct SingleChannelFeedback {
  //   double delayMs = 80;
  //   double decayGain = 0.85;
  //   int delaySamples;
  //   Delay delay;
  //   void configure(double sampleRate) {
  //     delaySamples = delayMs*0.001*sampleRate;
  //     delay.resize(delaySamples + 1);
  //     delay.reset(); // Start with all 0s
  //   }
  //   double process(double input) {
  //     double delayed = delay.read(delaySamples);
  //     double sum = input + delayed*decayGain;
  //     delay.write(sum);
  //     return delayed;
  //   }
  // };

  //----------

  // struct MultiChannelFeedback {
  //   using Array = std::array<double, channels>;
  //   double delayMs = 150;
  //   double decayGain = 0.85;
  //   std::array<int, channels> delaySamples;
  //   std::array<Delay, channels> delays;
  //   void configure(double sampleRate) {
  //     double delaySamplesBase = delayMs*0.001*sampleRate;
  //     for (int c = 0; c < channels; ++c) {
  //       // Distribute delay times exponentially between delayMs and 2*delayMs
  //       double r = c*1.0/channels;
  //       delaySamples[c] = std::pow(2, r)*delaySamplesBase;
  //       delays[c].resize(delaySamples[c] + 1);
  //       delays[c].reset();
  //     }
  //   }
  //   Array process(Array input) {
  //     Array delayed;
  //     for (int c = 0; c < channels; ++c) {
  //       delayed[c] = delays[c].read(delaySamples[c]);
  //     }
  //     for (int c = 0; c < channels; ++c) {
  //       double sum = input[c] + delayed[c]*decayGain;
  //       delays[c].write(sum);
  //     }
  //     return delayed;
  //   }
  // };

  //----------

  struct MultiChannelMixedFeedback {
    using Array = std::array<double, channels>;
    double delayMs = 150;
    double decayGain = 0.85;
    std::array<int, channels> delaySamples;
    std::array<Delay, channels> delays;
    void configure(double sampleRate) {
      double delaySamplesBase = delayMs*0.001*sampleRate;
      for (int c = 0; c < channels; ++c) {
        double r = c*1.0/channels;
        delaySamples[c] = std::pow(2, r)*delaySamplesBase;
        delays[c].resize(delaySamples[c] + 1);
        delays[c].reset();
      }
    }
    Array process(Array input) {
      Array delayed;
      for (int c = 0; c < channels; ++c) {
        delayed[c] = delays[c].read(delaySamples[c]);
      }
      // Mix using a Householder matrix
      Array mixed = delayed;
      Householder<double, channels>::inPlace(mixed.data());
      for (int c = 0; c < channels; ++c) {
        double sum = input[c] + mixed[c]*decayGain;
        delays[c].write(sum);
      }
      return delayed;
    }
  };

  //----------

  struct DiffusionStep {
    using Array = std::array<double, channels>;
    double delayMsRange = 50;
    std::array<int, channels> delaySamples;
    std::array<Delay, channels> delays;
    std::array<bool, channels> flipPolarity;
    void configure(double sampleRate) {
      double delaySamplesRange = delayMsRange*0.001*sampleRate;
      for (int c = 0; c < channels; ++c) {
        double rangeLow = delaySamplesRange*c/channels;
        double rangeHigh = delaySamplesRange*(c + 1)/channels;
        //delaySamples[c] = randomInRange(rangeLow, rangeHigh);
        delaySamples[c] = SAT_RandomRange(rangeLow, rangeHigh);
        delays[c].resize(delaySamples[c] + 1);
        delays[c].reset();
        flipPolarity[c] = rand()%2;
      }
    }
    Array process(Array input) {
      // Delay
      Array delayed;
      for (int c = 0; c < channels; ++c) {
        delays[c].write(input[c]);
        delayed[c] = delays[c].read(delaySamples[c]);
      }
      // Mix with a Hadamard matrix
      Array mixed = delayed;
      Hadamard<double, channels>::inPlace(mixed.data());
      // Flip some polarities
      for (int c = 0; c < channels; ++c) {
        if (flipPolarity[c]) mixed[c] *= -1;
      }
      return mixed;
    }
  };

  //----------

  // struct DiffuserEqualLengths {
  //   using Array = std::array<double, channels>;
  //   using Step = DiffusionStep;
  //   std::array<Step, stepCount> steps;
  //   DiffuserEqualLengths(double totalDiffusionMs) {
  //     for (auto &step : steps) {
  //       step.delayMsRange = totalDiffusionMs/stepCount;
  //     }
  //   }
  //   void configure(double sampleRate) {
  //     for (auto &step : steps) step.configure(sampleRate);
  //   }
  //   Array process(Array samples) {
  //     for (auto &step : steps) {
  //       samples = step.process(samples);
  //     }
  //     return samples;
  //   }
  // };

  //----------

  struct DiffuserHalfLengths {
    using Array = std::array<double, channels>;
    using Step = DiffusionStep;
    std::array<Step, stepCount> steps;
    DiffuserHalfLengths(double diffusionMs) {
      for (auto &step : steps) {
        diffusionMs *= 0.5;
        step.delayMsRange = diffusionMs;
      }
    }
    void configure(double sampleRate) {
      for (auto &step : steps) step.configure(sampleRate);
    }
    Array process(Array samples) {
      for (auto &step : steps) {
        samples = step.process(samples);
      }
      return samples;
    }
  };

  //----------

  struct BasicReverb {
    using Array = std::array<double, channels>;
    MultiChannelMixedFeedback feedback;
    DiffuserHalfLengths diffuser;
    double dry, wet;
    BasicReverb(double roomSizeMs, double rt60, double dry=0, double wet=1) : diffuser(roomSizeMs), dry(dry), wet(wet) {
      feedback.delayMs = roomSizeMs;
      // How long does our signal take to go around the feedback loop?
      double typicalLoopMs = roomSizeMs*1.5;
      // How many times will it do that during our RT60 period?
      double loopsPerRt60 = rt60/(typicalLoopMs*0.001);
      // This tells us how many dB to reduce per loop
      double dbPerCycle = -60/loopsPerRt60;
      feedback.decayGain = std::pow(10, dbPerCycle*0.05);
    }
    void configure(double sampleRate) {
      feedback.configure(sampleRate);
      diffuser.configure(sampleRate);
    }
    Array process(Array input) {
      Array diffuse = diffuser.process(input);
      Array longLasting = feedback.process(diffuse);
      Array output;
      for (int c = 0; c < channels; ++c) {
        output[c] = dry*input[c] + wet*longLasting[c];
      }
      return output;
    }
  };

//------------------------------
private:
//------------------------------

  BasicReverb* processor = nullptr;

//------------------------------
public:
//------------------------------

  SignalsmithReverb(double roomSizeMs, double rt60, double dry=0, double wet=1) {
    processor = new BasicReverb(roomSizeMs,rt60,dry,wet);
  }

  //----------

  ~SignalsmithReverb() {
    delete processor;
  }

//------------------------------
public:
//------------------------------

  void configure(double sample_rate) {
    processor->configure(sample_rate);
  }

  //----------

  void process(sat_sample_t** inputs, sat_sample_t** outputs, uint32_t num_samples) {
    //srand(1);
    //int num_channels = 2;
    int durationSamples = num_samples;// / num_channels;
    // outputWav.samples.resize(durationSamples*2);
    for (int i = 0; i < durationSamples; ++i) {
      // Duplicate input channels as many times as needed
      std::array<double, channels> array;
      //double array[channels];
      for (int c=0; c<channels; ++c) {
        int inputChannel = c % 2;//channels;
        //array[c] = inputs[i*channels + inputChannel];
        sat_sample_t* input = inputs[inputChannel];
        array[c] = input[i];
      }
      array = processor->process(array);
      sat_sample_t* output0 = outputs[0];
      sat_sample_t* output1 = outputs[1];
      if (averageOutputs) {
        // Mix down into stereo for example output
        double left = 0, right = 0;
        for (int c = 0; c < channels; c += 2) {
          left += array[c];
          right += array[c + 1];
        }
        constexpr double scaling = 2.0/channels;
        output0[i] = left*scaling;
        output1[i] = right*scaling;
      } else {
        output0[i] = array[0];
        output1[i] = array[1];
      }
    }
    // outputWav.write(outputPrefix + name + ".wav");
    // if (!outputWav.result) {
    //   std::cerr << outputWav.result.reason << "\n";
    //   std::exit(1);
    // }
  }

//------------------------------
private:
//------------------------------

  // double randomInRange(double low, double high) {
  //   // There are better randoms than this, and you should use them instead 😛
  //   double unitRand = rand()/double(RAND_MAX);
  //   return low + unitRand*(high - low);
  // }

  //----------

  // Use like `Householder<double, 8>::inPlace(data)` - size must be ≥ 1
  template<typename Sample, int size>
  class Householder {
    static constexpr Sample multiplier{-2.0/size};
  public:
    static void inPlace(Sample *arr) {
      double sum = 0;
      for (int i = 0; i < size; ++i) {
        sum += arr[i];
      }
      sum *= multiplier;
      for (int i = 0; i < size; ++i) {
        arr[i] += sum;
      }
    };
  };

  // Use like `Hadamard<double, 8>::inPlace(data)` - size must be a power of 2
  template<typename Sample, int size>
  class Hadamard {
  public: 
    static inline void recursiveUnscaled(Sample * data) {
      if (size <= 1) return;
      constexpr int hSize = size/2;
      // Two (unscaled) Hadamards of half the size
      Hadamard<Sample, hSize>::recursiveUnscaled(data);
      Hadamard<Sample, hSize>::recursiveUnscaled(data + hSize);
      // Combine the two halves using sum/difference
      for (int i = 0; i < hSize; ++i) {
        double a = data[i];
        double b = data[i + hSize];
        data[i] = (a + b);
        data[i + hSize] = (a - b);
      }
    }
    static inline void inPlace(Sample * data) {
      recursiveUnscaled(data);
      Sample scalingFactor = std::sqrt(1.0/size);
      for (int c = 0; c < size; ++c) {
        data[c] *= scalingFactor;
      }
    }
  };

};

typedef SignalsmithReverb<8,4,true> SAT_SignalsmithReverb;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_reverb_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/" PLUGIN_NAME,
  .name         = PLUGIN_NAME,
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "signalsmith reverb",
  .features     = (const char* []) {
                    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    CLAP_PLUGIN_FEATURE_PITCH_SHIFTER,
                    nullptr
                  }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_reverb_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  SAT_SignalsmithReverb* reverb = nullptr;//SAT_SignalsmithReverb(100,3,1,0.25);

  double p_roomsize = 0.0;
  double p_rt60 = 0.0;
  double p_dry = 0.0;
  double p_wet = 0.0;

//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(sa_reverb_plugin)

  //----------

  /*
    extendDuration(3);
    monoProcessor(SingleChannelFeedback());                     // "single-channel-feedback", 
    multiProcessor<4>(MultiChannelFeedback<4>());               // "multi-channel-feedback-4", 
    multiProcessor<8>(MultiChannelFeedback<8>());               // "multi-channel-feedback-8", 
    multiProcessor<4>(MultiChannelMixedFeedback<4>());          // "multi-channel-feedback-householder-4", 
    multiProcessor<8>(MultiChannelMixedFeedback<8>());          // "multi-channel-feedback-householder-8", 
    multiProcessor<4,false>(DiffuserHalfLengths<4, 3>(200));    // "diffuser-equal-4-3", 
    multiProcessor<8,false>(DiffuserHalfLengths<8, 4>(200));    // "diffuser-equal-8-4", 
    multiProcessor<8,false>(DiffuserEqualLengths<8, 6>(3000));  // "diffuser-equal-8-6-long", 
    multiProcessor<8,false>(DiffuserHalfLengths<8, 6>(3000));   // "diffuser-halves-8-6-long", 
    extendDuration(1);
    multiProcessor<8,true>(BasicReverb<8, 4>(50, 2.5));         // "reverb-basic-8-short", 
    extendDuration(2);
    multiProcessor<8,true>(BasicReverb<8, 4>(100, 3, 1, 0.25)); // "reverb-basic-8-mix", 
    extendDuration(2);
    multiProcessor<8,true>(BasicReverb<8, 4>(100, 10));         // "reverb-basic-8-long", 
  */

  bool init() final {
    registerDefaultExtensions();    
    appendStereoAudioInputPort("In");
    appendStereoAudioOutputPort("Out");

    appendParameter( new SAT_Parameter( "RoomSize", 100,  1,    1000  ));
    appendParameter( new SAT_Parameter( "rt60",     1,    0.1,  10    ));
    appendParameter( new SAT_Parameter( "dry",      1,    0,    1     ));
    appendParameter( new SAT_Parameter( "wet",      0.5,  0,    1     ));
    //setAllParameterFlags(CLAP_PARAM_IS_AUTOMATABLE);
    reverb = new SAT_SignalsmithReverb(100,10,1,0.75);
    return SAT_Plugin::init();
  }

  void destroy() final {
    delete reverb;
    SAT_Plugin::destroy();
  }
  
  //----------
  
  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {

    reverb->configure(sample_rate);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }
  
  //----------
  
  // bool start_processing() final {
  //   // MStretch.reset();
  //   return SAT_Plugin::start_processing();
  // }
  
  //----------
  
  bool on_plugin_paramValue(const clap_event_param_value_t* event) final {
    double  v  = event->value;
    int32_t iv = SAT_Trunc(event->value);
    switch (event->param_id) {
      case 0: p_roomsize = v; return true;
      case 1: p_rt60 = v;     return true;
      case 2: p_dry = v;      return true;
      case 3: p_wet = v;      return true;
    }
    return false;
  }
  
  //----------

  void processAudio(SAT_ProcessContext* AContext) final {
    float** inputs  = AContext->process->audio_inputs[0].data32;
    float** outputs = AContext->process->audio_outputs[0].data32;
    uint32_t length = AContext->process->frames_count;
    //SAT_CopyStereoBuffer(outputs,inputs,length);
    // MStretch.process(inputs,length,outputs,length);

    reverb->process(inputs,outputs,length);

  }
  
  //----------

};

//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY

  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_reverb_descriptor,sa_reverb_plugin)
  
#endif

//----------

#undef PLUGIN_NAME

//----------------------------------------------------------------------
#endif

