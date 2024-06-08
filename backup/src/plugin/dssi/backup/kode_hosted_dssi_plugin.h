#ifndef kode_hosted_dssi_plugin_included
#define kode_hosted_dssi_plugin_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "host/kode_hosted_plugin.h"

class KODE_HostedDssiPlugin
: public KODE_HostedPlugin {
};

//----------------------------------------------------------------------
#endif












#if 0

#ifndef kode_dssi_included
#define kode_dssi_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

#define KODE_DSSI_MAX_MIDI_EVENTS   256
#define KODE_DSSI_MAX_NUM_PROGRAMS  1024

#include "audio/wrappers/kode_wrap_ladspa.h"
#include "base/kode_array.h"
#include "extern/dssi/dssi.h"

//----------

//typedef KArray<const DSSI_Program_Descriptor*> KDssiPrograms;

struct KODE_DssiProgram {
//char    name[25];
  char    name[56];   // 56 = 64 - sizeof(int32)*2 -> align/cache, etc..
  uint32  bank;
  uint32  program;
};

//typedef KArray<KDssiProgram> KDssiPrograms;

//----------

class KODE_Dssi : public KLadspa {
  protected:
    const DSSI_Descriptor*    MDssiDescriptor;
    DSSI_Descriptor_Function  MGetDssiDescriptor;
    uint32                    MNumMidiEvents;
    snd_seq_event_t           MMidiEvents[KODE_DSSI_MAX_MIDI_EVENTS];
  //KDssiPrograms             MPrograms;
    uint32                    MNumPrograms;
    KDssiProgram              MPrograms[KODE_DSSI_MAX_NUM_PROGRAMS];
  public:
    KDssi();
    virtual ~KDssi();
  public:
    virtual bool        load(const char* AFilename) override;
    virtual uint32      countDescriptors(void) override;
    virtual bool        initDescriptor(uint32 AIndex) override;
  public:
    virtual void        initPrograms(void);
    virtual uint32      numPrograms(void);
    virtual const char* getProgramName(uint32 AIndex);
    virtual void        setProgram(uint32 AIndex);
    virtual void        dumpDssiDescriptor(void);
    virtual void        midiEvent(uint32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3);
    virtual void        resetMidi(void);
  public:
    virtual char*       configure(const char *Key, const char *Value);
    virtual const DSSI_Program_Descriptor* get_program(uint32 Index);
    virtual void        set_program(uint32 Bank, uint32 Program);
    virtual int32       get_midi_controller_for_port(uint32 Port);
    virtual void        run_synth(uint32 ANumSamples);
    virtual void        run_synth_adding(uint32 ANumSamples);
  //virtual void        run_multiple_synths(uint32 InstanceCount, LADSPA_Handle* Instances, uint32 ANumSamples) final;
  //virtual void        run_multiple_synths_adding(uint32 InstanceCount, LADSPA_Handle* Instances, uint32 ANumSamples) final;
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

KDssi::KDssi()
: KLadspa() {
  MDssiDescriptor = KODE_NULL;
  MGetDssiDescriptor = KODE_NULL;
  MNumMidiEvents = 0;
  KMemset(MMidiEvents,0,sizeof(MMidiEvents));
}

//----------

KDssi::~KDssi() {
}

//----------------------------------------
//
//----------------------------------------

// we don't check for ladspa_descriptor..

bool KDssi::load(const char* AFilename) {
  //KDTrace("KDssi::load %s\n",AFilename);
  if (KLibrary::load(AFilename)) {
    MGetDssiDescriptor = (DSSI_Descriptor_Function)getSymbol("dssi_descriptor");
    return (MGetDssiDescriptor);
  }
  return false;
}

//bool KDssi::init(void) {
//  MGetDssiDescriptor = (DSSI_Descriptor_Function)getLibrarySymbol("dssi_descriptor");
//  return (MGetDssiDescriptor);
//}

//----------

uint32 KDssi::countDescriptors(void) {
  //KDTrace("KDssi::countDescriptors\n");
  uint32 num = 0;
  if (MGetDssiDescriptor) {
    const DSSI_Descriptor* desc = MGetDssiDescriptor(num);
    while (desc) {
      num++;
      desc = MGetDssiDescriptor(num);
    }
  }
  return num;
}

//----------

bool KDssi::initDescriptor(uint32 AIndex) {
  if (MGetDssiDescriptor) {
    MDssiDescriptor = MGetDssiDescriptor(AIndex);
    if (MDssiDescriptor) {
      MLadspaDescriptor = MDssiDescriptor->LADSPA_Plugin;
      return true;
    }
  }
  return false;
}

//----------

//void KDssi::initPorts(void) {
//}

//----------

void KDssi::initPrograms(void) {
  const DSSI_Program_Descriptor* prog_descr;
  MNumPrograms = 0;
  prog_descr = get_program(MNumPrograms);
  while (prog_descr) {
    KStrncpy( MPrograms[MNumPrograms].name, prog_descr->Name, 24 );
    MPrograms[MNumPrograms].bank = prog_descr->Bank;
    MPrograms[MNumPrograms].program = prog_descr->Program;
    MNumPrograms++;
    prog_descr = get_program(MNumPrograms);
  }
}

//----------

uint32 KDssi::numPrograms(void) {
  return MNumPrograms;
}

//----------

const char* KDssi::getProgramName(uint32 AIndex) {
  return MPrograms[AIndex].name;
}

//----------

void KDssi::setProgram(uint32 AIndex) {
  uint32 bank = MPrograms[AIndex].bank;
  uint32 program = MPrograms[AIndex].program;
  set_program(bank,program);
}

//----------------------------------------
//
//----------------------------------------

void KDssi::dumpDssiDescriptor(void) {
  KDTrace("DSSI_Descriptor:\n");
  KDTrace("  api version:                   %i\n",     MDssiDescriptor->DSSI_API_Version);
  KDTrace("  ladspa descriptor:             0x%08x\n", MDssiDescriptor->LADSPA_Plugin);
  KDTrace("  configure:                     0x%08x\n", MDssiDescriptor->configure);
  KDTrace("  get_program:                   0x%08x\n", MDssiDescriptor->get_program);
  KDTrace("  select_program:                0x%08x\n", MDssiDescriptor->select_program);
  KDTrace("  get_midi_controller_for_port:  0x%08x\n", MDssiDescriptor->get_midi_controller_for_port);
  KDTrace("  run_synth:                     0x%08x\n", MDssiDescriptor->run_synth);
  KDTrace("  run_synth_adding:              0x%08x\n", MDssiDescriptor->run_synth_adding);
  KDTrace("  run_multiple_synths:           0x%08x\n", MDssiDescriptor->run_multiple_synths);
  KDTrace("  run_multiple_synths_adding:    0x%08x\n", MDssiDescriptor->run_multiple_synths_adding);
}

//----------

void KDssi::midiEvent(uint32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
  //KTrace("midi.. offset %i : %i %i %i\n",AOffset,AMsg1,AMsg2,AMsg3);
  if (MNumMidiEvents < KODE_DSSI_MAX_MIDI_EVENTS) {
    uint32 chan = AMsg1 & 0x0f;
    uint32 msg  = AMsg1 & 0xf0;
    switch(msg) {
      case 0x80: // note off
        MMidiEvents[MNumMidiEvents].type                = SND_SEQ_EVENT_NOTEOFF;
        MMidiEvents[MNumMidiEvents].time.tick           = AOffset;
        MMidiEvents[MNumMidiEvents].data.note.channel   = chan;
        MMidiEvents[MNumMidiEvents].data.note.note      = AMsg2;
        MMidiEvents[MNumMidiEvents].data.note.velocity  = AMsg3;
        break;
      case 0x90: // note on
        MMidiEvents[MNumMidiEvents].type                = SND_SEQ_EVENT_NOTEON;
        MMidiEvents[MNumMidiEvents].time.tick           = AOffset;
        MMidiEvents[MNumMidiEvents].data.note.channel   = chan;
        MMidiEvents[MNumMidiEvents].data.note.note      = AMsg2;
        MMidiEvents[MNumMidiEvents].data.note.velocity  = AMsg3;
        break;
      //case 0xa0: // polyphonic key pressure (aftertouch)
      //  break;
      case 0xb0: // control change
        MMidiEvents[MNumMidiEvents].type                  = SND_SEQ_EVENT_CONTROLLER;
        MMidiEvents[MNumMidiEvents].time.tick             = AOffset;
        MMidiEvents[MNumMidiEvents].data.control.channel  = chan;
        MMidiEvents[MNumMidiEvents].data.control.param    = AMsg2;
        MMidiEvents[MNumMidiEvents].data.control.value    = AMsg3;
        break;
      //case 0xc0: // program change
      //  break;
      //case 0xd0: // channel pressure (aftertouch)
      //  break;
      //case 0xe0: // pitch bend
      //  break;
    }
    MNumMidiEvents++;
  }
}

void KDssi::resetMidi(void) {
  MNumMidiEvents = 0;
  KMemset(MMidiEvents,0,sizeof(MMidiEvents));
}

//----------------------------------------
//
//----------------------------------------

char* KDssi::configure(const char *Key, const char *Value) {
  //KDTrace("KDssi::configure %s %s\n",Key,Value);
  if (MInstantiated) {
    if (MDssiDescriptor) {
      if (MDssiDescriptor->configure) {
        return MDssiDescriptor->configure(MLadspaHandle,Key,Value);
      }
    }
  }
  return KODE_NULL;
}

//----------

const DSSI_Program_Descriptor* KDssi::get_program(uint32 Index) {
  if (MInstantiated) {
    if (MDssiDescriptor) {
      if (MDssiDescriptor->get_program) {
        return MDssiDescriptor->get_program(MLadspaHandle,Index);
      }
    }
  }
  return KODE_NULL;
}

//----------

void KDssi::set_program(uint32 Bank, uint32 Program) {
  //KDTrace("KDssi::set_program %i %i\n",Bank,Program);
  if (MInstantiated) {
    if (MDssiDescriptor) {
      if (MDssiDescriptor->select_program) {
        MDssiDescriptor->select_program(MLadspaHandle,Bank,Program);
      }
    }
  }
}

//----------

int32 KDssi::get_midi_controller_for_port(uint32 Port) {
  if (MInstantiated) {
    if (MDssiDescriptor) {
      if (MDssiDescriptor->get_midi_controller_for_port) {
        return MDssiDescriptor->get_midi_controller_for_port(MLadspaHandle,Port);
      }
    }
  }
  return -1;
}

//----------

void KDssi::run_synth(uint32 ANumSamples) {
  if (MActivated) {
    if (MDssiDescriptor) {
      if (MDssiDescriptor->run_synth) {
        MDssiDescriptor->run_synth(MLadspaHandle,ANumSamples,MMidiEvents,MNumMidiEvents);
        MNumMidiEvents = 0;
      }
    }
  }
}

//----------

void KDssi::run_synth_adding(uint32 ANumSamples) {
  if (MActivated) {
    if (MDssiDescriptor) {
      if (MDssiDescriptor->run_synth_adding) {
        MDssiDescriptor->run_synth_adding(MLadspaHandle,ANumSamples,MMidiEvents,MNumMidiEvents);
        MNumMidiEvents = 0;
      }
    }
  }
}

//----------

//void KDssi::run_multiple_synths(uint32 InstanceCount, LADSPA_Handle* Instances, uint32 ANumSamples) {
//  if (MDssiDescriptor) {
//    if (MDssiDescriptor->run_multiple_synths) {
//      MDssiDescriptor->run_multiple_synths(InstanceCount,Instances,ANumSamples,MMidiEvents,MNumMidiEvents);
//      MNumMidiEvents = 0;
//    }
//  }
//}

//----------

//void KDssi::run_multiple_synths_adding(uint32 InstanceCount, LADSPA_Handle* Instances, uint32 ANumSamples) {
//  if (MDssiDescriptor) {
//    if (MDssiDescriptor->run_multiple_synths_adding) {
//      MDssiDescriptor->run_multiple_synths_adding(InstanceCount,Instances,ANumSamples,MMidiEvents,MNumMidiEvents);
//      MNumMidiEvents = 0;
//    }
//  }
//}

//----------


//----------------------------------------------------------------------
#endif

#endif // 0