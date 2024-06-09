#ifndef sat_processor_owner_included
#define sat_processor_owner_included
//----------------------------------------------------------------------
// aka plugin

#include "sat.h"
#include "plugin/sat_audio_port.h"
#include "plugin/sat_note_port.h"
#include "plugin/sat_parameter.h"

//----------------------------------------------------------------------

class SAT_ProcessorOwner {
public:
  virtual SAT_AudioPortArray* on_processorOwner_getAudioInputPorts()  { return nullptr; }
  virtual SAT_AudioPortArray* on_processorOwner_getAudioOutputPorts() { return nullptr; }
  virtual SAT_NotePortArray*  on_processorOwner_getNoteInputPorts()   { return nullptr; }
  virtual SAT_NotePortArray*  on_processorOwner_getNoteOutputPorts()  { return nullptr; }
  virtual SAT_ParameterArray* on_processorOwner_getParameters()       { return nullptr; }

  // should be processorListener?

  virtual void  on_processorOwner_updateParamFromHostToGui(uint32_t AIndex, sat_param_t AValue)  {}
  virtual void  on_processorOwner_updateModFromHostToGui(uint32_t AIndex, sat_param_t AValue)  {}
  virtual void  on_processorOwner_outputEvent()  {}

//virtual void  on_processorOwner_queueParamFromHostToGui(uint32_t AIndex, sat_param_t AValue)  {}
//virtual void  on_processorOwner_queueModFromHostToGui(uint32_t AIndex, sat_param_t AValue)    {}
//virtual void  on_processorOwner_queueNoteEndFromAudioToHost(SAT_Note ANote) {}

  virtual ~SAT_ProcessorOwner() {}
};

//----------------------------------------------------------------------
#endif