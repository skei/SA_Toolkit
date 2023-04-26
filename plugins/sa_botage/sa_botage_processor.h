#ifndef sa_botage_processor_included
#define sa_botage_processor_included
//----------------------------------------------------------------------

#include "base/sat.h"
//#include "sa_botage_params.h"

#include "audio/old/filters/sat_svf_filter.h"

//----------------------------------------------------------------------

#define BUFFER_SIZE       (1024 * 1024 * 16)
#define MIN_LOOP_LENGTH   64
#define MIN_READ_SPEED    (1.0 / 8.0)
#define MAX_READ_SPEED    8.0

//----------------------------------------------------------------------

class sa_botage_processor {

  friend class sa_botage_plugin;
  friend class sa_botage_editor;

//------------------------------
private:
//------------------------------

  double    MSampleRate               = 0.0;
  float     MLeftBuffer[BUFFER_SIZE]  = {0};
  float     MRightBuffer[BUFFER_SIZE] = {0};
  uint32_t  MBufferLength             = 0;
  double    MBufferLengthF            = 0;
  bool      MBufferWrapped            = false;

  double    MReadPos                  = 0.0; // vari pitch
  uint32_t  MWritePos                 = 0;
  bool      MIsPlaying                = false;
  bool      MWasPlaying               = false;

  int32_t   MCurrentSlice             = -1;
  int32_t   MPrevSlice                = -1;

  double    MReadSpeed                = 1.0;

  double    MSlice                    = 0;
  double    MSliceFract               = 0.0;
  double    MSliceStart               = 0.0;
  double    MSliceLength              = 0.0;

  bool      MRange                    = false;
  //double    MRangeFract               = 0.0;
  double    MRangeStart               = 0.0;
  double    MRangeLength              = 0.0;
  uint32_t  MRangeCount               = 0;

  bool      MLoop                     = false;
  //double    MLoopFract                = 0.0;
  double    MLoopStart                = 0;
  double    MLoopLength               = 0;
  uint32_t  MLoopCount                = 0;

  double    MLoopFract                = 0.0;

  bool      MLoopWrapped              = false;
  double    MCurrentRangeOffset       = 0.0;

//------------------------------
private:
//------------------------------

  uint32_t      par_num_beats               = 0;
  uint32_t      par_num_slices              = 0;

  double        par_range_prob              = 0.0;
  uint32_t      par_range_slice_count       = 0;
  uint32_t      par_range_loop_count        = 0;

  double        par_loop_env_attack         = 0.0;
  double        par_loop_env_decay          = 0.0;
  double        par_slice_env_attack        = 0.0;
  double        par_slice_env_decay         = 0.0;

  double        par_fx_filter_prob          = 0.0;
  uint32_t      par_fx_filter_type          = 0;
  double        par_fx_filter_freq          = 0.0;
  double        par_fx_filter_bw            = 0.0;

  double        par_prob_size_prob_range    = 0.0;
  double        par_prob_size_min_range     = 0.0;
  double        par_prob_size_max_range     = 0.0;
  double        par_prob_size_prob_loop     = 0.0;
  double        par_prob_size_min_loop      = 0.0;
  double        par_prob_size_max_loop      = 0.0;

  double        par_prob_speed_prob_range   = 0.0;
  double        par_prob_speed_min_range    = 0.0;
  double        par_prob_speed_max_range    = 0.0;
  double        par_prob_speed_prob_loop    = 0.0;
  double        par_prob_speed_min_loop     = 0.0;
  double        par_prob_speed_max_loop     = 0.0;

  double        par_prob_offset_prob_range  = 0.0;
  double        par_prob_offset_min_range   = 0.0;
  double        par_prob_offset_max_range   = 0.0;
  double        par_prob_offset_prob_loop   = 0.0;
  double        par_prob_offset_min_loop    = 0.0;
  double        par_prob_offset_max_loop    = 0.0;

  double        par_prob_reverse_prob_range = 0.0;
  double        par_prob_reverse_prob_loop  = 0.0;

  double        par_prob_fx_prob_range      = 0.0;
  double        par_prob_fx_min_range       = 0.0;
  double        par_prob_fx_max_range       = 0.0;
  double        par_prob_fx_prob_loop       = 0.0;
  double        par_prob_fx_min_loop        = 0.0;
  double        par_prob_fx_max_loop        = 0.0;

  uint32_t      par_fx_mode                 = 0;

  double        par_fx1_prob                = 0.0;
  uint32_t      par_fx1_type                = 0.0;
  double        par_fx1_par1                = 0.0;
  double        par_fx1_par2                = 0.0;
  double        par_fx1_par3                = 0.0;

  double        par_fx2_prob                = 0.0;
  uint32_t      par_fx2_type                = 0.0;
  double        par_fx2_par1                = 0.0;
  double        par_fx2_par2                = 0.0;
  double        par_fx2_par3                = 0.0;

  double        par_fx3_prob                = 0.0;
  uint32_t      par_fx3_type                = 0.0;
  double        par_fx3_par1                = 0.0;
  double        par_fx3_par2                = 0.0;
  double        par_fx3_par3                = 0.0;

  double        par_fx4_prob                = 0.0;
  uint32_t      par_fx4_type                = 0.0;
  double        par_fx4_par1                = 0.0;
  double        par_fx4_par2                = 0.0;
  double        par_fx4_par3                = 0.0;

  double        par_fx5_prob                = 0.0;
  uint32_t      par_fx5_type                = 0.0;
  double        par_fx5_par1                = 0.0;
  double        par_fx5_par2                = 0.0;
  double        par_fx5_par3                = 0.0;

//------------------------------
private:
//------------------------------

  double        rnd_main_prob            = 0.0;
  double        rnd_main_slices          = 0.0;
  double        rnd_main_subdiv          = 0.0;

  double        rnd_range_size           = 0.0;
  double        rnd_range_speed          = 0.0;
  double        rnd_range_offset         = 0.0;
  double        rnd_range_reverse        = 0.0;
  double        rnd_range_fx             = 0.0;

  double        rnd_loop_size            = 0.0;
  double        rnd_loop_speed           = 0.0;
  double        rnd_loop_offset          = 0.0;
  double        rnd_loop_reverse         = 0.0;
  double        rnd_loop_fx              = 0.0;

  double        rnd_range_size_value     = 0.0;
  double        rnd_range_speed_value    = 0.0;
  double        rnd_range_offset_value   = 0.0;
  double        rnd_range_reverse_value  = 0.0;
  double        rnd_range_fx_value       = 0.0;

  double        rnd_loop_size_value      = 0.0;
  double        rnd_loop_speed_value     = 0.0;
  double        rnd_loop_offset_value    = 0.0;
  double        rnd_loop_reverse_value   = 0.0;
  double        rnd_loop_fx_value        = 0.0;

  double        rnd_fx_value             = 0.0;

  bool          rnd_slice_on             = false;

  bool          rnd_range_size_on        = false;
  bool          rnd_range_speed_on       = false;
  bool          rnd_range_offset_on      = false;
  bool          rnd_range_reverse_on     = false;
  bool          rnd_range_fx_on          = false;

  bool          rnd_loop_size_on         = false;
  bool          rnd_loop_speed_on        = false;
  bool          rnd_loop_offset_on       = false;
  bool          rnd_loop_reverse_on      = false;
  bool          rnd_loop_fx_on           = false;

  //

  double        rnd_fx1        = 0.0;
  bool          rnd_fx1_on     = false;
  double        rnd_fx2        = 0.0;
  bool          rnd_fx2_on     = false;
  double        rnd_fx3        = 0.0;
  bool          rnd_fx3_on     = false;
  double        rnd_fx4        = 0.0;
  bool          rnd_fx4_on     = false;
  double        rnd_fx5        = 0.0;
  bool          rnd_fx5_on     = false;

//------------------------------
private:
//------------------------------

  MIP_SvfFilter MFX_LP0  = {};
  MIP_SvfFilter MFX_LP1  = {};

  MIP_SvfFilter MFX_HP0  = {};
  MIP_SvfFilter MFX_HP1  = {};

  MIP_SvfFilter MFX_BP0  = {};
  MIP_SvfFilter MFX_BP1 = {};

  double        MFX_LP_Input    = 0.0;
  double        MFX_LP_Prev     = 0.0;
  double        MFX_LP_Freq     = 0.0;
  double        MFX_LP_BW       = 0.0;
  double        MFX_LP_Movement = 0.0;
  double        MFX_LP_Current  = 0.0;

  double        MFX_HP_Input    = 0.0;
  double        MFX_HP_Prev     = 0.0;
  double        MFX_HP_Freq     = 0.0;
  double        MFX_HP_BW       = 0.0;
  double        MFX_HP_Movement = 0.0;
  double        MFX_HP_Current  = 0.0;

  double        MFX_BP_Input    = 0.0;
  double        MFX_BP_Prev     = 0.0;
  double        MFX_BP_Freq     = 0.0;
  double        MFX_BP_BW       = 0.0;
  double        MFX_BP_Movement = 0.0;
  double        MFX_BP_Current  = 0.0;

  double        MFX_Dist_Input  = 0.0;
  double        MFX_Dist_Amount = 0.0;
  double        MFX_Dist_Limit  = 0.0;

//------------------------------
public:
//------------------------------

  sa_botage_processor() {
  }

  //----------

  ~sa_botage_processor() {
  }

//------------------------------
public:
//------------------------------

  bool hasWrapped() { return MLoopWrapped; }

//------------------------------
public:
//------------------------------

  void activate(double sample_rate) {
    MSampleRate = sample_rate;
  }

  //----------

  void setParamValue(uint32_t param_id,double value) {
    switch (param_id) {
      case PAR_NUM_BEATS                : par_num_beats               = value; break;
      case PAR_NUM_SLICES               : par_num_slices              = value; break;
      case PAR_RANGE_PROB               : par_range_prob              = value; break;
      case PAR_RANGE_SLICE_COUNT        : par_range_slice_count       = value; break;
      case PAR_RANGE_LOOP_COUNT         : par_range_loop_count        = value; break;
      case PAR_LOOP_ENV_ATTACK          : par_loop_env_attack         = value; break;
      case PAR_LOOP_ENV_DECAY           : par_loop_env_decay          = 25.0 - value; break;
      case PAR_SLICE_ENV_ATTACK         : par_slice_env_attack        = value; break;
      case PAR_SLICE_ENV_DECAY          : par_slice_env_decay         = 1.0 - value; break;
      //case PAR_FX_FILTER_PROB           : par_fx_filter_prob          = value; break;
      //case PAR_FX_FILTER_TYPE           : par_fx_filter_type          = value; break;
      //case PAR_FX_FILTER_FREQ           : par_fx_filter_freq          = value; break;
      //case PAR_FX_FILTER_BW             : par_fx_filter_bw            = value; break;
      case PAR_PROB_SIZE_PROB_RANGE     : par_prob_size_prob_range    = value; break;
      case PAR_PROB_SIZE_MIN_RANGE      : par_prob_size_min_range     = value; break;
      case PAR_PROB_SIZE_MAX_RANGE      : par_prob_size_max_range     = value; break;
      case PAR_PROB_SIZE_PROB_LOOP      : par_prob_size_prob_loop     = value; break;
      case PAR_PROB_SIZE_MIN_LOOP       : par_prob_size_min_loop      = value; break;
      case PAR_PROB_SIZE_MAX_LOOP       : par_prob_size_max_loop      = value; break;
      case PAR_PROB_SPEED_PROB_RANGE    : par_prob_speed_prob_range   = value; break;
      case PAR_PROB_SPEED_MIN_RANGE     : par_prob_speed_min_range    = value; break;
      case PAR_PROB_SPEED_MAX_RANGE     : par_prob_speed_max_range    = value; break;
      case PAR_PROB_SPEED_PROB_LOOP     : par_prob_speed_prob_loop    = value; break;
      case PAR_PROB_SPEED_MIN_LOOP      : par_prob_speed_min_loop     = value; break;
      case PAR_PROB_SPEED_MAX_LOOP      : par_prob_speed_max_loop     = value; break;
      case PAR_PROB_OFFSET_PROB_RANGE   : par_prob_offset_prob_range  = value; break;
      case PAR_PROB_OFFSET_MIN_RANGE    : par_prob_offset_min_range   = value; break;
      case PAR_PROB_OFFSET_MAX_RANGE    : par_prob_offset_max_range   = value; break;
      case PAR_PROB_OFFSET_PROB_LOOP    : par_prob_offset_prob_loop   = value; break;
      case PAR_PROB_OFFSET_MIN_LOOP     : par_prob_offset_min_loop    = value; break;
      case PAR_PROB_OFFSET_MAX_LOOP     : par_prob_offset_max_loop    = value; break;
      case PAR_PROB_REVERSE_PROB_RANGE  : par_prob_reverse_prob_range = value; break;
      case PAR_PROB_REVERSE_PROB_LOOP   : par_prob_reverse_prob_loop  = value; break;
      case PAR_PROB_FX_PROB_RANGE       : par_prob_fx_prob_range      = value; break;
      case PAR_PROB_FX_MIN_RANGE        : par_prob_fx_min_range       = value; break;
      case PAR_PROB_FX_MAX_RANGE        : par_prob_fx_max_range       = value; break;
      case PAR_PROB_FX_PROB_LOOP        : par_prob_fx_prob_loop       = value; break;
      case PAR_PROB_FX_MIN_LOOP         : par_prob_fx_min_loop        = value; break;
      case PAR_PROB_FX_MAX_LOOP         : par_prob_fx_max_loop        = value; break;

      case PAR_FX_MODE                  : par_fx_mode                 = value; break; //

      case PAR_FX1_PROB                 : par_fx1_prob                = value; break;
      case PAR_FX1_TYPE                 : par_fx1_type                = value; break;
      case PAR_FX1_PAR1                 : par_fx1_par1                = value; break;
      case PAR_FX1_PAR2                 : par_fx1_par2                = value; break;
      case PAR_FX1_PAR3                 : par_fx1_par3                = value; break;

      case PAR_FX2_PROB                 : par_fx2_prob                = value; break;
      case PAR_FX2_TYPE                 : par_fx2_type                = value; break;
      case PAR_FX2_PAR1                 : par_fx2_par1                = value; break;
      case PAR_FX2_PAR2                 : par_fx2_par2                = value; break;
      case PAR_FX2_PAR3                 : par_fx2_par3                = value; break;

      case PAR_FX3_PROB                 : par_fx3_prob                = value; break;
      case PAR_FX3_TYPE                 : par_fx3_type                = value; break;
      case PAR_FX3_PAR1                 : par_fx3_par1                = value; break;
      case PAR_FX3_PAR2                 : par_fx3_par2                = value; break;
      case PAR_FX3_PAR3                 : par_fx3_par3                = value; break;

      case PAR_FX4_PROB                 : par_fx4_prob                = value; break;
      case PAR_FX4_TYPE                 : par_fx4_type                = value; break;
      case PAR_FX4_PAR1                 : par_fx4_par1                = value; break;
      case PAR_FX4_PAR2                 : par_fx4_par2                = value; break;
      case PAR_FX4_PAR3                 : par_fx4_par3                = value; break;

      case PAR_FX5_PROB                 : par_fx5_prob                = value; break;
      case PAR_FX5_TYPE                 : par_fx5_type                = value; break;
      case PAR_FX5_PAR1                 : par_fx5_par1                = value; break;
      case PAR_FX5_PAR2                 : par_fx5_par2                = value; break;
      case PAR_FX5_PAR3                 : par_fx5_par3                = value; break;

    }
  }

  //----------

  void transport(uint32_t flags) {

    if (flags & CLAP_TRANSPORT_IS_PLAYING) {
      MIsPlaying = true;
    }
    else {
      MIsPlaying = false;
      MRange = false;
      MLoop = false;
      MReadPos = 0.0;
      MWritePos = 0;
    }

    if (MIsPlaying && !MWasPlaying) {
      //MIP_Print("play\n");
      MPrevSlice = -1;
      MReadPos = 0.0;
      MWritePos = 0;
      MReadSpeed = 1.0;
      MBufferWrapped = false;
    }
    else if (MWasPlaying && !MIsPlaying) {
      //MIP_Print("stop\n");
      // reset waveform areas/markers..
    }
    MWasPlaying = MIsPlaying;
  }

//------------------------------
public:
//------------------------------

  void processAudioBlock(MIP_ProcessContext* AContext) {

    const clap_process_t* process = AContext->process;

    float* audioin0  = process->audio_inputs[0].data32[0];
    float* audioin1  = process->audio_inputs[0].data32[1];
    float* audioout0 = process->audio_outputs[0].data32[0];
    float* audioout1 = process->audio_outputs[0].data32[1];

    // buffer length

    MIP_Assert( process->transport->tempo > 0.0);
    MIP_Assert( par_num_beats > 0 );
    MIP_Assert( par_num_slices > 0);

    double seconds_per_beat = 60.0 / process->transport->tempo;
    double samples_per_beat = seconds_per_beat * MSampleRate;
    MBufferLengthF = (double)par_num_beats * samples_per_beat;
    MBufferLength = ceil(MBufferLengthF);

    uint32_t len = process->frames_count;

    if (MIsPlaying) {

      double slice_length = MBufferLengthF / (double)(par_num_beats * par_num_slices);

      for (uint32_t i=0; i<len; i++) {

        // get input

        float in0 = *audioin0++;
        float in1 = *audioin1++;

        // write to buffer

        MLeftBuffer[MWritePos]  = in0;
        MRightBuffer[MWritePos] = in1;
        MWritePos += 1;
        if (MWritePos >= MBufferLength) { // will wrap..
          handleBufferWrapping();
        }
        MWritePos %= MBufferLength;

        // (re-) calc slice length

        MIP_Assert( slice_length > 0.0 );

        double slice_pos = (double)MWritePos / slice_length;
        MSlice = MIP_Trunc(slice_pos);
        MSliceFract = slice_pos - MSlice; // 0..1
        MSliceStart = MSlice * slice_length;
        MSliceLength = slice_length;

        // loop fract

        //if (MLoop) {
        //  if (MLoopLength > 0) {
        //    MLoopFract = (MReadPos - MLoopStart) / MLoopLength;
        //  }
        //}
        //else {
        //  MLoopFract = 0.0;
        //}

        // count slices, samples

        MCurrentSlice = MSlice;
        if (MCurrentSlice != MPrevSlice) nextSlice();
        MPrevSlice = MCurrentSlice;
        countSamples();

        // read from buffer

        MReadPos = fmod(MReadPos,MBufferLength);
        if (MReadPos < 0.0) MReadPos += MBufferLength;

        uint32_t pos = MIP_Trunc(MReadPos);
        float buf0 = MLeftBuffer[pos];
        float buf1 = MRightBuffer[pos];
        MReadPos += MReadSpeed;

        MReadPos = fmod(MReadPos,MBufferLength);
        if (MReadPos < 0.0) MReadPos += MBufferLength;

        // out

        float out0 = in0;
        float out1 = in1;

        if (MLoop) {
          //if (MLoopWrapped) {
            // only after we have wrapped around, so the content has changed
            // first time around, the buffer might be empty..
            out0 = buf0;
            out1 = buf1;
          //}
        }

        process_effects(&out0,&out1);

        float env = process_envelopes();
        out0 *= env;
        out1 *= env;

        // finat output

        *audioout0++ = out0;
        *audioout1++ = out1;
      }
    }

    else { // playing
      for (uint32_t i=0; i<len; i++) {
        float spl0 = *audioin0++;
        float spl1 = *audioin1++;
        *audioout0++ = spl0;
        *audioout1++ = spl1;
      }
    }

  }

//------------------------------
private:
//------------------------------

  // in = uint32-t bitfield
  // return = index of selected bit

  uint32_t randomBit(uint32_t AValue) {
    uint32_t numbits = 0;
    uint32_t bit = 1;
    for (uint32_t i=0; i<32; i++) {
      if (AValue & bit) numbits += 1;
      bit <<= 1;
    }
    if (numbits == 0) return 0;
    uint32_t rnd = MIP_RandomRangeInt(0,numbits-1);
    bit = 1;
    for (uint32_t i=0; i<32; i++) {
      if (AValue & bit) {
        if (rnd == 0) {
          //MIP_Print("i %i bit %i\n",i,bit);
          return (i + 1);
        }
        rnd -= 1;
      }
      bit <<= 1;
    }
    return 0;
  }

//------------------------------
private:
//------------------------------

  /*
    called every time we enter a new slice
  */

  void nextSlice() {
    if (!MRange) {
      // assume off
      MRange = false;
      MLoop = false;
      rnd_slice_on = true;
      // range
      //if (MIP_RandomRange(0.0, 0.999) < par_range_prob) {

      //if (MIP_Random() < par_range_prob) {
      //  startRange();
      //}

      rnd_main_prob = MIP_Random();

      if (rnd_main_prob < par_range_prob) {
        rnd_slice_on = true;
        startRange();
      }

    }
  }

  //----------

  // called every sample

  void countSamples() {

    if (MRange) {
      MRangeCount += 1;
      if (MRangeCount >= MRangeLength) { // range finished?
        endRange();
      }
    }

    if (MLoop) {
      MLoopCount += 1;
      //MLoopCounter += 1.0;
      //MLoopCounter += MReadSpeed;
      //MLoopCounter += fabs(MReadSpeed);
      //if (MLoopCounter >= MLoopLength) {
      if (MLoopCount >= MLoopLength) {
        handleLoopWrapping();
      }
    }

  }

//------------------------------
private:
//------------------------------

  /*
    called from nextSlice, if prob resulted in new range
  */

  void startRange() {
    uint32_t remain = (par_num_beats * par_num_slices) - MSlice;
    uint32_t num_slices = randomBit(par_range_slice_count);
    //num_slices = MIP_MaxI(num_slices,1);
    //num_slices = MIP_MinI(num_slices,remain);
    num_slices = MIP_ClampI(num_slices,1,remain);
    uint32_t num_loops = randomBit(par_range_loop_count);
    num_loops = MIP_MaxI(num_loops,1);
    MRange        = true;
    MRangeCount   = 0;
    MRangeStart   = MSliceStart;
    MRangeLength  = MSliceLength * num_slices;
    MLoop         = true;
    MLoopCount    = 0;
    MLoopStart    = MSliceStart;
    MLoopLength   = MRangeLength / num_loops;
    MLoopWrapped  = false;
    MReadSpeed    = 1.0;
    handleLoopStarting();

    rnd_main_slices = num_slices;
    rnd_main_subdiv = num_loops;

    MFX_LP0.reset();
    MFX_LP1.reset();

    MFX_HP0.reset();
    MFX_HP1.reset();

    MFX_BP0.reset();
    MFX_BP1.reset();

  }

  //----------

  /*
    called from countSamples, after range has ended
  */

  void endRange() {
    MRange          = false;
    MLoop           = false;
    MPrevSlice      = -1;
    MReadPos        = MWritePos;
    MReadSpeed      = 1.0;
    rnd_range_fx_on = false;
    //rnd_fx_value    = 1.0;
  }

//------------------------------
private:
//------------------------------

  /*
    called from startRange
    (after range/loop has been defined)
  */

  void handleLoopStarting() {

    rnd_range_size_on     = false;
    rnd_range_speed_on    = false;
    rnd_range_offset_on   = false;
    rnd_range_reverse_on  = false;
    rnd_range_fx_on       = false;

    rnd_fx1_on            = false;
    rnd_fx2_on            = false;
    rnd_fx3_on            = false;
    rnd_fx4_on            = false;
    rnd_fx5_on            = false;

    MFX_LP_Current        = 0.0;
    MFX_HP_Current        = 0.0;
    MFX_BP_Current        = 0.0;

    // size
    rnd_range_size = MIP_Random();
    if (rnd_range_size < par_prob_size_prob_range) {
      rnd_range_size_on = true;
      double s = par_prob_size_max_range - par_prob_size_min_range;
      rnd_range_size_value = MIP_Random();
      s *= rnd_range_size_value;
      s += par_prob_size_min_range;
      float n = powf(0.5,-s);
      MLoopLength *= n;
      MLoopLength = MIP_MaxI(MLoopLength,MIN_LOOP_LENGTH);
    }

    // speed
    rnd_range_speed = MIP_Random();
    if (rnd_range_speed < par_prob_speed_prob_range) {
      rnd_range_speed_on = true;
      double s = par_prob_speed_max_range - par_prob_speed_min_range;
      rnd_range_speed_value = MIP_Random();
      s *= rnd_range_speed_value;
      s += par_prob_speed_min_range;
      float n = powf(0.5,-s);
      MReadSpeed *= n;
      MReadSpeed = MIP_Clamp(MReadSpeed,-8.0,8.0);
    }

    // offset

    MCurrentRangeOffset = 0.0;

    rnd_range_offset = MIP_Random();
    if (rnd_range_offset < par_prob_offset_prob_range) {
      rnd_range_offset_on = true;
      double s = par_prob_offset_max_range - par_prob_offset_min_range;
      rnd_range_offset_value = MIP_Random();
      s *= rnd_range_offset_value;
      s += par_prob_offset_min_range;
      s *= 8.9; //(double)(par_num_beats * par_num_slices);
      s = MIP_Trunc(s);
      MCurrentRangeOffset = (s * MSliceLength);
      //MReadPos += (s * MSliceLength);
      MReadPos += MCurrentRangeOffset;
    }

    // reverse
    rnd_range_reverse = MIP_Random();
    if (rnd_range_reverse < par_prob_reverse_prob_range) {
      rnd_range_reverse_on = true;
      MReadSpeed *= -1.0;
      if (MReadSpeed < 0.0) MReadPos += MLoopLength;
      else MReadPos -= MLoopLength;
    }

    // fx
    rnd_range_fx = MIP_Random();
    if (rnd_range_fx < par_prob_fx_prob_range) {
      rnd_range_fx_on = true;
      rnd_range_fx_value = MIP_Random();
      double s = par_prob_fx_max_range - par_prob_fx_min_range;
      s *= rnd_range_fx_value;
      s += par_prob_fx_min_range;
      float n = powf(0.5,-s);
      rnd_fx_value = n;
      //MLoopLength *= n;
      //MLoopLength = MIP_MaxI(MLoopLength,MIN_LOOP_LENGTH);
    }

    //
    //
    //

    // fx

    if (rnd_range_fx_on) {
      switch (par_fx_mode) {
        case 0: { // single
          double s1  =      par_fx1_prob;
          double s2  = s1 + par_fx2_prob;
          double s3  = s2 + par_fx3_prob;
          double s4  = s3 + par_fx4_prob;
          double s5  = s4 + par_fx5_prob;
          if (s5 > 0) {
            double r = MIP_RandomRange(0,s5);
            if ((par_fx1_prob > 0) && (r <= s1)) {  rnd_fx1_on = true; break; }
            if ((par_fx2_prob > 0) && (r <= s2)) {  rnd_fx2_on = true; break; }
            if ((par_fx3_prob > 0) && (r <= s3)) {  rnd_fx3_on = true; break; }
            if ((par_fx4_prob > 0) && (r <= s4)) {  rnd_fx4_on = true; break; }
            if ((par_fx5_prob > 0)             ) {  rnd_fx5_on = true; break; }
          }
          break;
        }
        case 1: { // multi
          rnd_fx1 = MIP_Random();
          rnd_fx2 = MIP_Random();
          rnd_fx3 = MIP_Random();
          rnd_fx4 = MIP_Random();
          rnd_fx5 = MIP_Random();
          if (rnd_fx1 < par_fx1_prob) rnd_fx1_on = true;
          if (rnd_fx2 < par_fx2_prob) rnd_fx2_on = true;
          if (rnd_fx3 < par_fx3_prob) rnd_fx3_on = true;
          if (rnd_fx4 < par_fx4_prob) rnd_fx4_on = true;
          if (rnd_fx5 < par_fx5_prob) rnd_fx5_on = true;
          break;
        }
        case 2: { // all
          rnd_fx1 = 1;
          rnd_fx2 = 1;
          rnd_fx3 = 1;
          rnd_fx4 = 1;
          rnd_fx5 = 1;
          rnd_fx1_on = true;
          rnd_fx2_on = true;
          rnd_fx3_on = true;
          rnd_fx4_on = true;
          rnd_fx5_on = true;
          break;
        }
      } // fx

    } // switch range fx

  }

  //------------------------------

  /*
    called from countSamples, after loop has ended
    (wraparound)
  */

  void handleLoopWrapping() {
    MLoopWrapped = true;
    //MLoopCounter = 0.0;
    MLoopCount = 0;

//    MFX_LP_Current        = 0.0;
//    MFX_HP_Current        = 0.0;
//    MFX_BP_Current        = 0.0;

    if (MReadSpeed < 0.0) MReadPos = MLoopStart + MLoopLength;
    //else MReadPos = MLoopStart;
    else MReadPos = MLoopStart + MCurrentRangeOffset;

    //float rnd = 0.0;

    rnd_loop_size_on    = false;
    rnd_loop_speed_on   = false;
    rnd_loop_offset_on  = false;
    rnd_loop_reverse_on = false;
    rnd_loop_fx_on      = false;
    //rnd_fx_value        = 1.0;

    // size
    rnd_loop_size = MIP_Random();
    if (rnd_loop_size < par_prob_size_prob_loop) {
      rnd_loop_size_on = true;
      double s = par_prob_size_max_loop - par_prob_size_min_loop;
      rnd_loop_size_value = MIP_Random();
      s *= rnd_loop_size_value;
      s += par_prob_size_min_loop;
      float n = powf(0.5,-s);
      MLoopLength *= n;
      MLoopLength = MIP_MaxI(MLoopLength,MIN_LOOP_LENGTH);
    }

    // speed
    rnd_loop_speed = MIP_Random();
    if (rnd_loop_speed < par_prob_speed_prob_loop) {
      rnd_loop_speed_on = true;
      double s = par_prob_speed_max_loop - par_prob_speed_min_loop;
      rnd_loop_speed_value = MIP_Random();
      s *= rnd_loop_speed_value;
      s += par_prob_speed_min_loop;
      float n = powf(0.5,-s);
      MReadSpeed *= n;
      MReadSpeed = MIP_Clamp(MReadSpeed,-8.0,8.0);
    }

    // offset
    rnd_loop_offset = MIP_Random();
    if (rnd_loop_offset < par_prob_offset_prob_loop) {
      rnd_loop_offset_on = true;
      double s = par_prob_offset_max_loop - par_prob_offset_min_loop;
      rnd_loop_offset_value = MIP_Random();
      s *= rnd_loop_offset_value;
      s += par_prob_offset_min_loop;
      s *= 8.9; // (double)(par_num_beats * par_num_slices);
      s = MIP_Trunc(s);
      //MIP_Print("s %.3f\n",s);
      MReadPos += (s * MSliceLength);
    }

    // reverse
    rnd_loop_reverse = MIP_Random();
    //MIP_Print("rnd %.3f (par_prob_offset_prob_range %.3f)\n",rnd,par_prob_reverse_prob_range);
    if (rnd_loop_reverse < par_prob_reverse_prob_loop) {
      rnd_loop_reverse_on = true;
      //MIP_Print("Reverse!\n");
      MReadSpeed *= -1.0;
      if (MReadSpeed < 0.0) MReadPos += MLoopLength;
      else MReadPos -= MLoopLength;

    }

    // fx
    rnd_loop_fx = MIP_Random();
    if (rnd_loop_fx < par_prob_fx_prob_loop) {
      rnd_loop_fx_on = true;
      rnd_loop_fx_value = MIP_Random();
      double s = par_prob_fx_max_loop - par_prob_fx_min_loop;
      s *= rnd_loop_fx_value;
      s += par_prob_fx_min_loop;
      float n = powf(0.5,-s);
      rnd_fx_value *= n;
      //MIP_Print("rnd_fx_value: %.3f\n",rnd_loop_value);
      //MLoopLength *= n;
      //MLoopLength = MIP_MaxI(MLoopLength,MIN_LOOP_LENGTH);
    }

  }

//------------------------------
private:
//------------------------------

  void handleBufferWrapping() {
    MBufferWrapped = true;
    //MIP_Print("buffer finished\n");
    MPrevSlice = -1;
  }

//------------------------------
private:
//------------------------------

  //float handleEnvelopes(float ASliceFract, float ALoopFract) {
  float process_envelopes() {
    float env = 1.0f;

    // slice env

    float sa = par_slice_env_attack;        // * MSliceLength;
    float sd = (1.0 - par_slice_env_decay); // * MSliceLength;

    if ((sa > 0) && (MSliceFract < sa)) {
      env *= (MSliceFract / sa);
    }

    if ((sd > 0) && (MSliceFract >= (1.0 - sd))) {
      env *= ((1.0 - MSliceFract) / sd);
    }

    // loop env

    if (/*MLoopWrapped &&*/ MLoop && (MLoopLength > 0.0)) {
      double loop_count = (double)MLoopCount;
      double la = par_loop_env_attack * (MSampleRate * 0.001); // samples
      double ld = (25.0 - par_loop_env_decay)  * (MSampleRate * 0.001);
      //float loop_pos = ALoopFract * MLoopLength;
      if ((la > 0) && (loop_count < la)) {
        env *= (loop_count / la);
      }
      if ((ld > 0) && (loop_count >= (MLoopLength - ld))) {
        env *= ((MLoopLength - loop_count) / ld);
      }
    }

    return env;
  }

//------------------------------
private:
//------------------------------

  void prepare_effect(uint32_t type, double par1, double par2, double par3, float* in0, float* in1) {
    switch (type) {
      case FX_OFF: {
        break;
      }
      case FX_LP: {
        MFX_LP_Input    = 1.0;
        double freq     = MIP_Curve(par1,0.2);
        double f2       = freq * rnd_fx_value; // * 0.5 .. 2
        double diff     = freq - f2;
        MFX_LP_Freq     = f2;
        MFX_LP_BW       = 1.0 - MIP_Curve(par2,0.9);
        MFX_LP_Movement = diff / MRangeLength;
        MFX_LP_Movement *= par3;

        break;
      }
      case FX_HP: {
        MFX_HP_Input    = 1.0;
        double freq     = MIP_Curve(par1,0.2);
        double f2       = freq * rnd_fx_value; // * 0.5 .. 2
        double diff     = freq - f2;
        MFX_HP_Freq     = f2;
        MFX_HP_BW       = 1.0 - MIP_Curve(par2,0.9);
        MFX_HP_Movement = diff / MRangeLength;
        MFX_HP_Movement *= par3;

        break;
      }
      case FX_BP: {
        MFX_BP_Input    = 1.0;
        double freq     = MIP_Curve(par1,0.2);
        double f2       = freq * rnd_fx_value; // * 0.5 .. 2
        double diff     = freq - f2;
        MFX_BP_Freq     = f2;
        MFX_BP_BW       = 1.0 - MIP_Curve(par2,0.9);
        MFX_BP_Movement = diff / MRangeLength;
        MFX_BP_Movement *= par3;

        break;
      }
      case FX_DIST: {
        MFX_Dist_Input  = 1.0;
        MFX_Dist_Amount   = MIP_Clamp(par1 * rnd_fx_value, 0,1);
        MFX_Dist_Limit    = 1.0 - par2;

        break;
      }
      case FX_5: {
        break;
      }
      case FX_6: {
        break;
      }
      case FX_7: {
        break;
      }
    } // switch
  }

  //----------

  void process_effects(float* in0, float* in1) {

    if (rnd_range_fx_on) {

      MFX_LP_Input    = 0.0;
      MFX_HP_Input    = 0.0;
      MFX_BP_Input    = 0.0;
      MFX_Dist_Input  = 0.0;

      //

      if (rnd_fx1_on) prepare_effect(par_fx1_type,par_fx1_par1,par_fx1_par2,par_fx1_par3,in0,in1);
      if (rnd_fx2_on) prepare_effect(par_fx2_type,par_fx2_par1,par_fx2_par2,par_fx2_par3,in0,in1);
      if (rnd_fx3_on) prepare_effect(par_fx3_type,par_fx3_par1,par_fx3_par2,par_fx3_par3,in0,in1);
      if (rnd_fx4_on) prepare_effect(par_fx4_type,par_fx4_par1,par_fx4_par2,par_fx4_par3,in0,in1);
      if (rnd_fx5_on) prepare_effect(par_fx5_type,par_fx5_par1,par_fx5_par2,par_fx5_par3,in0,in1);

//      switch (fx_mode) {
//        case FX_MODE_SINGLE: {
//          break;
//        }
//        case FX_MODE_MULTI: {
//          break;
//        }
//        case FX_MODE_ALL: {
//          break;
//        }
//      }

      // process

      if (MFX_LP_Input != MFX_LP_Prev) {
        MFX_LP0.reset();
        MFX_LP1.reset();
      }
      MFX_LP_Prev = MFX_LP_Input;

      if (MFX_LP_Input > 0.0) {
        double f = MIP_Clamp(MFX_LP_Freq + MFX_LP_Current,0,1);
        MFX_LP0.setMode(1); // lp
        MFX_LP0.setFreq(f);
        MFX_LP0.setBW(MFX_LP_BW);
        MFX_LP1.setMode(1); // lp
        MFX_LP1.setFreq(f);
        MFX_LP1.setBW(MFX_LP_BW);
        *in0 = MFX_LP0.process(*in0);
        *in1 = MFX_LP1.process(*in1);
        MFX_LP_Current += MFX_LP_Movement;
        //MFX_LP_Freq = MIP_Clamp(MFX_LP_Freq,0,1);
      }

      //

      if (MFX_HP_Input != MFX_HP_Prev) {
        MFX_HP0.reset();
        MFX_HP1.reset();
      }
      MFX_HP_Prev = MFX_HP_Input;

      if (MFX_HP_Input > 0.0) {
        double f = MIP_Clamp(MFX_HP_Freq + MFX_HP_Current,0,1);
        MFX_HP0.setMode(2); // lp
        MFX_HP0.setFreq(f);
        MFX_HP0.setBW(MFX_HP_BW);
        MFX_HP1.setMode(2); // lp
        MFX_HP1.setFreq(f);
        MFX_HP1.setBW(MFX_HP_BW);
        *in0 = MFX_HP0.process(*in0);
        *in1 = MFX_HP1.process(*in1);
        MFX_HP_Current += MFX_HP_Movement;
        //MFX_HP_Freq = MIP_Clamp(MFX_HP_Freq,0,1);
      }

      //

      if (MFX_BP_Input != MFX_BP_Prev) {
        MFX_BP0.reset();
        MFX_BP1.reset();
      }
      MFX_BP_Prev = MFX_BP_Input;

      if (MFX_BP_Input > 0.0) {
        double f = MIP_Clamp(MFX_BP_Freq + MFX_BP_Current,0,1);
        MFX_BP0.setMode(3); // lp
        MFX_BP0.setFreq(f);
        MFX_BP0.setBW(MFX_BP_BW);
        MFX_BP1.setMode(3); // lp
        MFX_BP1.setFreq(f);
        MFX_BP1.setBW(MFX_BP_BW);
        *in0 = MFX_BP0.process(*in0);
        *in1 = MFX_BP1.process(*in1);
        MFX_BP_Current += MFX_BP_Movement;
        //MFX_BP_Freq = MIP_Clamp(MFX_BP_Freq,0,1);
      }

      //

      if (MFX_Dist_Input > 0.0) {
        float s = MIP_Sign(*in0);
        float a = abs(*in0);
        a *= 1.0 + (MFX_Dist_Amount * 2.0);
        if (a > 1.0) {
          a = 1.0 + (1.0 - a);
        }
        *in0 = (a * s * MFX_Dist_Limit);
        s = MIP_Sign(*in1);
        a = abs(*in1);
        a *= 1.0 + (MFX_Dist_Amount * 2.0);
        if (a > 1.0) {
          a = 1.0 + (1.0 - a);
        }
        *in1 = (a * s * MFX_Dist_Limit);
      }

    } // fx on
  }

};

//----------------------------------------------------------------------
#endif
