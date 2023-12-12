#ifndef kode_hosted_ladspa_plugin_included
#define kode_hosted_ladspa_plugin_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "host/kode_hosted_plugin.h"

class KODE_HostedLadspaPlugin
: public KODE_HostedPlugin {
};

//----------------------------------------------------------------------
#endif




#if 0

#ifndef kode_ladspa_included
#define kode_ladspa_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

#include <dlfcn.h>
#include "base/kode_library.h"
//#include "base/kode_memory_stdlib.h"
#include "extern/ladspa/ladspa.h"

#define KODE_MAX_LADSPA_AUDIO_INPUTS     32
#define KODE_MAX_LADSPA_AUDIO_OUTPUTS    32
#define KODE_MAX_LADSPA_CONTROL_INPUTS   512
#define KODE_MAX_LADSPA_CONTROL_OUTPUTS  512
#define KODE_MAX_LADSPA_PORTS            512

//----------

struct KODE_Ladspa_Port {
  const char* name;
  uint32      flags;
  float       val;
  float       minval;
  float       maxval;
};

//----------

class KODE_Ladspa : public KODE_Library {
  protected:
    bool MInstantiated;
    bool MActivated;
  protected:
    uint32                      MLadspaFlags;
    LADSPA_Handle               MLadspaHandle;
    const LADSPA_Descriptor*    MLadspaDescriptor;
    LADSPA_Descriptor_Function  MGetLadspaDescriptor;
  protected:
    uint32                      MNumAudioInputs;
    uint32                      MNumAudioOutputs;
    uint32                      MNumControlInputs;
    uint32                      MNumControlOutputs;
    KLadspa_Port                MLadspaPorts[KODE_MAX_LADSPA_PORTS];
    int32                       MAudioInputIndex[KODE_MAX_LADSPA_AUDIO_INPUTS];
    int32                       MAudioOutputIndex[KODE_MAX_LADSPA_AUDIO_OUTPUTS];
    int32                       MControlInputIndex[KODE_MAX_LADSPA_CONTROL_INPUTS];
    int32                       MControlOutputIndex[KODE_MAX_LADSPA_CONTROL_OUTPUTS];
  public:
    //bool              instantiated(void)            { return MInstantiated; }
    //bool              activated(void)               { return MActivated; }
    const char*     name(void)                    { return MLadspaDescriptor->Name; }
    const char*     label(void)                   { return MLadspaDescriptor->Label; }
    const char*     maker(void)                   { return MLadspaDescriptor->Maker; }
    const char*     copyright(void)               { return MLadspaDescriptor->Copyright; }
    uint32          uniqueId(void)                { return MLadspaDescriptor->UniqueID; }
    KLadspa_Port*   port(int i)                   { return &MLadspaPorts[i]; }
    KLadspa_Port*   audioInputPort(int i)         { return &MLadspaPorts[MAudioInputIndex[i]]; }
    KLadspa_Port*   audioOutputPort(int i)        { return &MLadspaPorts[MAudioOutputIndex[i]]; }
    KLadspa_Port*   controlInputPort(int i)       { return &MLadspaPorts[MControlInputIndex[i]]; }
    KLadspa_Port*   controlOutputPort(int i)      { return &MLadspaPorts[MControlOutputIndex[i]]; }
    uint32          numAudioInputs(void)          { return MNumAudioInputs; }
    uint32          numAudioOutputs(void)         { return MNumAudioOutputs; }
    uint32          numControlInputs(void)        { return MNumControlInputs; }
    uint32          numControlOutputs(void)       { return MNumControlOutputs; }
    uint32          audioInputIndex(uint32 i)     { return MAudioInputIndex[i]; }
    uint32          audioOutputIndex(uint32 i)    { return MAudioOutputIndex[i]; }
    uint32          controlInputIndex(uint32 i)   { return MControlInputIndex[i]; }
    uint32          controlOutputIndex(uint32 i)  { return MControlOutputIndex[i]; }
  public:
    KLadspa();
    virtual ~KLadspa();
  public:
    virtual bool    load(const char* AFilename) override;
  public:
    virtual uint32  countDescriptors(void);
    virtual bool    initDescriptor(uint32 AIndex);
    virtual void    initPorts(void);
    virtual void    dumpLadspaDescriptor(void);
  public:
    virtual void    instantiate(uint32 ASampleRate);
    virtual void    connect_port(uint32 APort, float* APtr);
    virtual void    activate(void);
    virtual void    run(uint32 ANum);
    virtual void    run_adding(uint32 ANum);
    virtual void    set_run_adding_gain(float AGain);
    virtual void    deactivate(void);
    virtual void    cleanup(void);
  };

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

KLadspa::KLadspa()
: KLibrary() {
  MInstantiated       = false;;
  MActivated          = false;;
  MLadspaFlags        = klf_none;
  MLadspaHandle       = KODE_NULL;
  MLadspaDescriptor   = KODE_NULL;
  MNumAudioInputs     = 0;
  MNumAudioOutputs    = 0;
  MNumControlInputs   = 0;
  MNumControlOutputs  = 0;
  KMemset(MLadspaPorts,       0,sizeof(MLadspaPorts));
  KMemset(MAudioInputIndex,   0,sizeof(MAudioInputIndex));
  KMemset(MAudioOutputIndex,  0,sizeof(MAudioOutputIndex));
  KMemset(MControlInputIndex, 0,sizeof(MControlInputIndex));
  KMemset(MControlOutputIndex,0,sizeof(MControlOutputIndex));
}

//----------

KLadspa::~KLadspa() {
}

//----------------------------------------
//
//----------------------------------------

bool KLadspa::load(const char* AFilename) {
  //KDTrace("KLadspa::load %s\n",AFilename);
  if (KLibrary::load(AFilename)) {
    MGetLadspaDescriptor = (LADSPA_Descriptor_Function)getSymbol("ladspa_descriptor");
    return true;
  }
  return false;
}

//----------------------------------------
//
//----------------------------------------

uint32 KLadspa::countDescriptors(void) {
  //KDTrace("KLadspa::countDescriptors\n");
  uint32 num = 0;
  if (MGetLadspaDescriptor) {
    const LADSPA_Descriptor* desc = MGetLadspaDescriptor(num);
    while (desc) {
      num++;
      desc = MGetLadspaDescriptor(num);
    }
  }
  return num;
}

//----------

bool KLadspa::initDescriptor(uint32 AIndex) {
  //KDTrace("KLadspa::InitDescriptor %i\n",AIndex);
  if (MGetLadspaDescriptor) {
    MLadspaDescriptor = MGetLadspaDescriptor(AIndex);
    return (MLadspaDescriptor);
  }
  return false;
}

//----------

void KLadspa::initPorts(void) {
  //KDTrace("KLadspa::initPorts\n");
  MNumAudioInputs     = 0;
  MNumAudioOutputs    = 0;
  MNumControlInputs   = 0;
  MNumControlOutputs  = 0;
  MLadspaFlags        = klf_none;
  if (MLadspaDescriptor) {
    if (MLadspaDescriptor->Properties & LADSPA_PROPERTY_REALTIME)         { MLadspaFlags |= klf_realtime; }
    if (MLadspaDescriptor->Properties & LADSPA_PROPERTY_INPLACE_BROKEN)   { MLadspaFlags |= klf_inplaceBroken; }
    if (MLadspaDescriptor->Properties & LADSPA_PROPERTY_HARD_RT_CAPABLE)  { MLadspaFlags |= klf_hardRTCapable; }
    for (uint32 i=0; i<MLadspaDescriptor->PortCount; i++) {
      MLadspaPorts[i].name    = MLadspaDescriptor->PortNames[i];
      MLadspaPorts[i].flags   = klp_none;
      MLadspaPorts[i].val     = 0;
      MLadspaPorts[i].minval  = 0;
      MLadspaPorts[i].maxval  = 1;
      LADSPA_PortDescriptor pd = MLadspaDescriptor->PortDescriptors[i];
      if (pd & LADSPA_PORT_AUDIO)   { MLadspaPorts[i].flags |= klp_audio; }
      if (pd & LADSPA_PORT_CONTROL) { MLadspaPorts[i].flags |= klp_control; }
      if (pd & LADSPA_PORT_INPUT)   { MLadspaPorts[i].flags |= klp_input; }
      if (pd & LADSPA_PORT_OUTPUT)  { MLadspaPorts[i].flags |= klp_output; }
      uint32 hints = MLadspaDescriptor->PortRangeHints[i].HintDescriptor;
      if (hints&LADSPA_HINT_BOUNDED_BELOW)  { MLadspaPorts[i].minval = MLadspaDescriptor->PortRangeHints[i].LowerBound; }
      if (hints&LADSPA_HINT_BOUNDED_ABOVE)  { MLadspaPorts[i].maxval = MLadspaDescriptor->PortRangeHints[i].UpperBound; }
      if (hints&LADSPA_HINT_TOGGLED)        { MLadspaPorts[i].flags |= klp_toggled; }
      if (hints&LADSPA_HINT_SAMPLE_RATE)    { MLadspaPorts[i].flags |= klp_samplerate; }
      if (hints&LADSPA_HINT_LOGARITHMIC)    { MLadspaPorts[i].flags |= klp_logarithmic; }
      if (hints&LADSPA_HINT_INTEGER)        { MLadspaPorts[i].flags |= klp_integer; }
      uint32 defmask = hints & LADSPA_HINT_DEFAULT_MASK;
      if (defmask == LADSPA_HINT_DEFAULT_MINIMUM) MLadspaPorts[i].val =  MLadspaPorts[i].minval;
      if (defmask == LADSPA_HINT_DEFAULT_LOW)     MLadspaPorts[i].val = (MLadspaPorts[i].minval * 0.75 + MLadspaPorts[i].maxval * 0.25);
      if (defmask == LADSPA_HINT_DEFAULT_MIDDLE)  MLadspaPorts[i].val = (MLadspaPorts[i].minval * 0.50 + MLadspaPorts[i].maxval * 0.50);
      if (defmask == LADSPA_HINT_DEFAULT_HIGH)    MLadspaPorts[i].val = (MLadspaPorts[i].minval * 0.25 + MLadspaPorts[i].maxval * 0.75);
      if (defmask == LADSPA_HINT_DEFAULT_MAXIMUM) MLadspaPorts[i].val =  MLadspaPorts[i].maxval;
      if (defmask == LADSPA_HINT_DEFAULT_0)       MLadspaPorts[i].val = 0;
      if (defmask == LADSPA_HINT_DEFAULT_1)       MLadspaPorts[i].val = 1;
      if (defmask == LADSPA_HINT_DEFAULT_100)     MLadspaPorts[i].val = 100;
      if (defmask == LADSPA_HINT_DEFAULT_440)     MLadspaPorts[i].val = 440;
      if ( (MLadspaPorts[i].flags&klp_audio)   && (MLadspaPorts[i].flags&klp_input)  ) MAudioInputIndex[   MNumAudioInputs++]    = i;
      if ( (MLadspaPorts[i].flags&klp_audio)   && (MLadspaPorts[i].flags&klp_output) ) MAudioOutputIndex[  MNumAudioOutputs++]   = i;
      if ( (MLadspaPorts[i].flags&klp_control) && (MLadspaPorts[i].flags&klp_input)  ) MControlInputIndex[ MNumControlInputs++]  = i;
      if ( (MLadspaPorts[i].flags&klp_control) && (MLadspaPorts[i].flags&klp_output) ) MControlOutputIndex[MNumControlOutputs++] = i;
    }
  }
}

//----------

void KLadspa::dumpLadspaDescriptor(void) {
  KDTrace("LADSPA_Descriptor:\n");
  KDTrace("  name:               '%s'\n",       MLadspaDescriptor->Name);
  KDTrace("  label:              '%s'\n",       MLadspaDescriptor->Label);
  KDTrace("  maker:              '%s'\n",       MLadspaDescriptor->Maker);
  KDTrace("  copyright:          '%s'\n",       MLadspaDescriptor->Copyright);
  KDTrace("  unique id:           %i\n",        MLadspaDescriptor->UniqueID);
  KDTrace("  num ports:           %i\n",        MLadspaDescriptor->PortCount);
  KDTrace("  flags:               0x%08x\n",    MLadspaFlags);
  KDTrace("  instantiate:         0x%08x\n",    MLadspaDescriptor->instantiate);
  KDTrace("  connect_port:        0x%08x\n",    MLadspaDescriptor->connect_port);
  KDTrace("  activate:            0x%08x\n",    MLadspaDescriptor->activate);
  KDTrace("  run:                 0x%08x\n",    MLadspaDescriptor->run);
  KDTrace("  run_adding:          0x%08x\n",    MLadspaDescriptor->run_adding);
  KDTrace("  set_run_adding_gain: 0x%08x\n",    MLadspaDescriptor->set_run_adding_gain);
  KDTrace("  deactivate:          0x%08x\n",    MLadspaDescriptor->deactivate);
  KDTrace("  cleanup:             0x%08x\n",    MLadspaDescriptor->cleanup);
  KDTrace("  num audio input ports:     %i\n",  MNumAudioInputs );
  KDTrace("  num audio output ports:    %i\n",  MNumAudioOutputs );
  KDTrace("  num control input ports:   %i\n",  MNumControlInputs );
  KDTrace("  num control output ports:  %i\n",  MNumControlOutputs );
}

//----------------------------------------
//
//----------------------------------------

void KLadspa::instantiate(uint32 ASampleRate) {
  if (MLibLoaded) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->instantiate) {
        MLadspaHandle = MLadspaDescriptor->instantiate(MLadspaDescriptor,ASampleRate);
        if (MLadspaHandle) MInstantiated = true;
      }
    }
  }
}

//----------

void KLadspa::connect_port(uint32 APort, float* APtr) {
  if (MInstantiated) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->connect_port) {
        MLadspaDescriptor->connect_port(MLadspaHandle,APort,APtr);
      }
    }
  }
}

//----------

void KLadspa::activate(void) {
  if (MInstantiated) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->activate) {
        MLadspaDescriptor->activate(MLadspaHandle);
        MActivated = true;
      }
    }
  }
}

//----------

void KLadspa::run(uint32 ANum) {
  if (MActivated) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->run) {
        MLadspaDescriptor->run(MLadspaHandle,ANum);
      }
    }
  }
}

//----------

void KLadspa::run_adding(uint32 ANum) {
  //KDTrace("KLadspa::run_adding %i\n",ANum);
  if (MActivated) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->run_adding) {
        MLadspaDescriptor->run_adding(MLadspaHandle,ANum);
      }
    }
  }
}

//----------

void KLadspa::set_run_adding_gain(float AGain) {
  //KDTrace("KLadspa::set_run_adding_gain %f\n",AGain);
  if (MActivated) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->set_run_adding_gain) {
        MLadspaDescriptor->set_run_adding_gain(MLadspaHandle,AGain);
      }
    }
  }
}

//----------

void KLadspa::deactivate(void) {
  //KDTrace("KLadspa::deactivate\n");
  if (MActivated) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->deactivate) {
        MLadspaDescriptor->deactivate(MLadspaHandle);
      }
    }
  }
}

//----------

void KLadspa::cleanup(void) {
  //KDTrace("KLadspa::cleanup\n");
  if (MInstantiated) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->cleanup) {
        MLadspaDescriptor->cleanup(MLadspaHandle);
        MInstantiated = false;
      }
    }
  }
}

//----------------------------------------------------------------------
#endif












/*
  https://ccrma.stanford.edu/mirrors/lalists/lad/2001/Sep/0014.html

  Hi,
  I would like to know if there is any agreed upon logarithmic function to
  use for LADSPA control ports when the LADSPA_HINT_LOGARITHMIC is set. In
  practice, the function should map display coordinates (the slider) to
  control values.
  The best I have come up with myself is this:
    Control port value, v
    Control port low limit, v_low
    Control port high limit, v_high
    Display coordinates, d
    Display coordinates low limit, d_low
    Display coordinates high limit, d_high
    Transposition to positive range, log_offset
    Setup:
      log_offset = -v_low+1;
      d_low = log(v_low + log_offset);
      d_high = log(v_high + log_offset);
      d = log(v + log_offset);
    Slider moved:
      v = exp(d) - log_offset;
  Marcus
*/

//----------

/*
  http://nwoeanhinnogaehr.github.io/ladspa.rs/ladspa/enum.DefaultValue.html
  Enum ladspa::DefaultValue
  Minimum
    Equal to the lower_bound of the Port.
  Low
    For ports with LADSPA_HINT_LOGARITHMIC, this should be
      exp(log(lower_bound) * 0.75 + log(upper_bound) * 0.25)
    Otherwise, this should be
      (lower_bound * 0.75 + upper_bound * 0.25)
  Middle
    For ports with CONTROL_HINT_LOGARITHMIC, this should be
      exp(log(lower_bound) * 0.5 + log(upper_bound) * 0.5)
    Otherwise, this should be
      (lower_bound * 0.5 + upper_bound * 0.5)
  High
    For ports with LADSPA_HINT_LOGARITHMIC, this should be
      exp(log(lower_bound) * 0.25 + log(upper_bound) * 0.75)
      Otherwise, this should be
      (lower_bound * 0.25 + upper_bound * 0.75)
*/

//----------

/*
  http://lists.linuxaudio.org/pipermail/linux-audio-dev/2006-April/015714.html
  if a is the users parameter UI control value in [0,1], u is the lower
  bound, v is the upper bound, and b is the log base, then:
  port val  = ( (1-a) log(u) + a log(v) )^b
            = ( -a log(u) + log(u) + a log(v) )^b
            = log( ( u+v^a ) / u^a )^b
            = (u + v^a) / u^a
*/

#endif // 0
