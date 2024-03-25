#ifndef sa_botage_processor_included
#define sa_botage_processor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_utils.h"

#define MAX_BUFFER_SIZE (1024*1024)

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_botage_processor {

//------------------------------
private:
//------------------------------

  float     MBuffer[MAX_BUFFER_SIZE * 2]  = {0};
  uint32_t  MBufferSize                   = MAX_BUFFER_SIZE;
  uint32_t  MReadPos                      = 0;
  uint32_t  MWritePos                     = 0;

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

  void setBufferSize(uint32_t ASize) { MBufferSize = ASize; }

  uint32_t getReadPos()     { return MReadPos; }
  uint32_t getWritePos()    { return MWritePos; }
  float*   getBuffer()      { return MBuffer; }
  uint32_t getBufferSize()  { return MBufferSize; }

//------------------------------
public:
//------------------------------

  void process(SAT_ProcessContext* AContext) {

    float**  inputs  = AContext->process->audio_inputs[0].data32;
    float**  outputs = AContext->process->audio_outputs[0].data32;
    uint32_t length  = AContext->process->frames_count;

    float* input0 = inputs[0];
    float* input1 = inputs[1];
    float* output0 = outputs[0];
    float* output1 = outputs[1];

    for (uint32_t i=0; i<length; i++) {

      // input
      float in0 = *input0++;
      float in1 = *input1++;

      // read from buffer
      float out0 = MBuffer[(MReadPos*2)  ];
      float out1 = MBuffer[(MReadPos*2)+1];
      MReadPos = ((MReadPos + 1) % MBufferSize);

      // write to buffer
      MBuffer[(MWritePos*2)  ] = in0;
      MBuffer[(MWritePos*2)+1] = in1;
      MWritePos = ((MWritePos + 1) % MBufferSize);

      // output
      *output0++ = out0;
      *output1++ = out1;
    }

  }

};

//----------------------------------------------------------------------

#undef MAX_BUFFER_SIZE

//----------------------------------------------------------------------
#endif
