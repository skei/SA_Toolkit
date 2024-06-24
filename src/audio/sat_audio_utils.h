#ifndef sat_audio_utils_included
#define sat_audio_utils_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/util/sat_math.h"

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

/*

//------------------------------
// mono
//------------------------------

void SAT_CopyMonoBuffer(float** ADst, float** ASrc, uint32_t ALength);
void SAT_ClearMonoBuffer(float** ADst, uint32_t ALength);
void SAT_AddMonoBuffer(float** ADst, float** AAdd, uint32_t ALength);
void SAT_ScaleMonoBuffer(float** ADst, float AScale, uint32_t ALength);

void SAT_CopyMonoBuffer(float* ADst, float* ASrc, uint32_t ALength);
void SAT_ClearMonoBuffer(float* ADst, uint32_t ALength);
void SAT_AddMonoBuffer(float* ADst, float* AAdd, uint32_t ALength);
void SAT_ScaleMonoBuffer(float* ADst, float AScale, uint32_t ALength);

// SAT_PLUGIN_PROCESS_TICKSIZE

//void SAT_CopyMonoBuffer(float** ADst, float** ASrc);
//7void SAT_ClearMonoBuffer(float** ADst);
//void SAT_AddMonoBuffer(float** ADst, float** AAdd);
//void SAT_ScaleMonoBuffer(float** ADst, float AScale);

//void SAT_CopyMonoBuffer(float* ADst, float* ASrc);
//void SAT_ClearMonoBuffer(float* ADst);
//void SAT_AddMonoBuffer(float* ADst, float* AAdd);
//void SAT_ScaleMonoBuffer(float* ADst, float AScale);

//------------------------------
// stereo
//------------------------------

void SAT_CopyStereoBuffer(float** ADst, float** ASrc, uint32_t ALength);
void SAT_ClearStereoBuffer(float** ADst, uint32_t ALength);
void SAT_AddStereoBuffer(float** ADst, float** AAdd, uint32_t ALength);
void SAT_ScaleStereoBuffer(float** ADst, float AScale, uint32_t ALength);
void SAT_ScaleStereoBuffer(float** ADst, float ALeft, float ARight, uint32_t ALength);
void SAT_ClampStereoBuffer(float** ADst, float AMin, float AMax, uint32_t ALength);

// SAT_PLUGIN_PROCESS_TICKSIZE

//void SAT_CopyStereoBuffer(float** ADst, float** ASrc);
//void SAT_ClearStereoBuffer(float** ADst);
//void SAT_AddStereoBuffer(float** ADst, float** AAdd);
//void SAT_ScaleStereoBuffer(float** ADst, float AScale);
//void SAT_ScaleStereoBuffer(float** ADst, float ALeft, float ARight);

//------------------------------
// mono -> stereo
//------------------------------

void SAT_CopyMonoToStereoBuffer(float** ADst, float** ASrc, uint32_t ALength);
void SAT_AddMonoToStereoBuffer(float** ADst, float** ASrc, uint32_t ALength);
void SAT_CopyMonoToStereoBuffer(float** ADst, float* ASrc, uint32_t ALength);
void SAT_AddMonoToStereoBuffer(float** ADst, float* ASrc, uint32_t ALength);

// SAT_PLUGIN_PROCESS_TICKSIZE

//void SAT_CopyMonoToStereoBuffer(float** ADst, float** ASrc);
//void SAT_AddMonoToStereoBuffer(float** ADst, float** ASrc);
//void SAT_CopyMonoToStereoBuffer(float** ADst, float* ASrc);
//void SAT_AddMonoToStereoBuffer(float** ADst, float* ASrc);

*/

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

//------------------------------
// mono
//------------------------------

void SAT_CopyMonoBuffer(float** ADst, float** ASrc, uint32_t ALength) {
  memcpy(ADst[0],ASrc[0],ALength*sizeof(float));
}

//----------

void SAT_ClearMonoBuffer(float** ADst, uint32_t ALength) {
  memset(ADst[0],0,ALength*sizeof(float));
}

//----------

void SAT_AddMonoBuffer(float** ADst, float** AAdd, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* add0 = AAdd[0];
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void SAT_ScaleMonoBuffer(float** ADst, float AScale, uint32_t ALength) {
  float* dst0 = ADst[0];
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ *= AScale;
  }
}

//------------------------------

void SAT_CopyMonoBuffer(float* ADst, float* ASrc, uint32_t ALength) {
  memcpy(ADst,ASrc,ALength*sizeof(float));
}

//----------

void SAT_ClearMonoBuffer(float* ADst, uint32_t ALength) {
  memset(ADst,0,ALength*sizeof(float));
}

//----------

void SAT_AddMonoBuffer(float* ADst, float* AAdd, uint32_t ALength) {
  float* dst0 = ADst;
  float* add0 = AAdd;
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void SAT_ScaleMonoBuffer(float* ADst, float AScale, uint32_t ALength) {
  float* dst0 = ADst;
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ *= AScale;
  }
}

//------------------------------
// mono, ticksize
//------------------------------

/*

void SAT_CopyMonoBuffer(float** ADst, float** ASrc) {
  memcpy(ADst[0],ASrc[0],KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void SAT_ClearMonoBuffer(float** ADst) {
  memset(ADst[0],0,KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void SAT_AddMonoBuffer(float** ADst, float** AAdd) {
  float* dst0 = ADst[0];
  float* add0 = AAdd[0];
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void SAT_ScaleMonoBuffer(float** ADst, float AScale) {
  float* dst0 = ADst[0];
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ *= AScale;
  }
}

//------------------------------

void SAT_CopyMonoBuffer(float* ADst, float* ASrc) {
  memcpy(ADst,ASrc,KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void SAT_ClearMonoBuffer(float* ADst) {
  memset(ADst,0,KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void SAT_AddMonoBuffer(float* ADst, float* AAdd) {
  float* dst0 = ADst;
  float* add0 = AAdd;
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void SAT_ScaleMonoBuffer(float* ADst, float AScale) {
  float* dst0 = ADst;
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ *= AScale;
  }
}

*/

//------------------------------
// stereo
//------------------------------

void SAT_CopyStereoBuffer(float** ADst, float** ASrc, uint32_t ALength) {
  memcpy(ADst[0],ASrc[0],ALength*sizeof(float));
  memcpy(ADst[1],ASrc[1],ALength*sizeof(float));
}

//----------

void SAT_ClearStereoBuffer(float** ADst, uint32_t ALength) {
  memset(ADst[0],0,ALength*sizeof(float));
  memset(ADst[1],0,ALength*sizeof(float));
}

//----------

void SAT_AddStereoBuffer(float** ADst, float** AAdd, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* add0 = AAdd[0];
  float* dst1 = ADst[1];
  float* add1 = AAdd[1];
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ += *add0++;
    *dst1++ += *add1++;
  }
}

//----------

void SAT_ScaleStereoBuffer(float** ADst, float AScale, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ *= AScale;
    *dst1++ *= AScale;
  }
}

//----------

void SAT_ScaleStereoBuffer(float** ADst, float ALeft, float ARight, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ *= ALeft;
    *dst1++ *= ARight;
  }
}

//----------

void SAT_ClampStereoBuffer(float** ADst, float AMin, float AMax, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  for(uint32_t i=0; i<ALength; i++) {
    float spl0 = *dst0;
    float spl1 = *dst1;
    spl0 = SAT_Clamp(spl0,AMin,AMax);
    spl1 = SAT_Clamp(spl1,AMin,AMax);
    *dst0++ = spl0;
    *dst1++ = spl1;
  }
}

//------------------------------
// stereo, ticksize
//------------------------------

/*

void SAT_CopyStereoBuffer(float** ADst, float** ASrc) {
  memcpy(ADst[0],ASrc[0],KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
  memcpy(ADst[1],ASrc[1],KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void SAT_ClearStereoBuffer(float** ADst) {
  memset(ADst[0],0,KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
  memset(ADst[1],0,KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void SAT_AddStereoBuffer(float** ADst, float** AAdd) {
  float* dst0 = ADst[0];
  float* add0 = AAdd[0];
  float* dst1 = ADst[1];
  float* add1 = AAdd[1];
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ += *add0++;
    *dst1++ += *add1++;
  }
}

//----------

void SAT_ScaleStereoBuffer(float** ADst, float AScale) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ *= AScale;
    *dst1++ *= AScale;
  }
}

//----------

void SAT_ScaleStereoBuffer(float** ADst, float ALeft, float ARight) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ *= ALeft;
    *dst1++ *= ARight;
  }
}

*/

//------------------------------
// mono -> stereo
//------------------------------

void SAT_CopyMonoToStereoBuffer(float** ADst, float** ASrc, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc[0];
  for (uint32_t i=0; i<ALength; i++) {
    float spl0 = *src0++;
    *dst0++ = spl0;
    *dst1++ = spl0;
  }
}

//----------

void SAT_AddMonoToStereoBuffer(float** ADst, float** ASrc, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc[0];
  for(uint32_t i=0; i<ALength; i++) {
    float spl0 = *src0++;
    *dst0++ += spl0;
    *dst1++ += spl0;
  }
}

//------------------------------

void SAT_CopyMonoToStereoBuffer(float** ADst, float* ASrc, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc;
  for (uint32_t i=0; i<ALength; i++) {
    float spl0 = *src0++;
    *dst0++ = spl0;
    *dst1++ = spl0;
  }
}

//----------

void SAT_AddMonoToStereoBuffer(float** ADst, float* ASrc, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc;
  for(uint32_t i=0; i<ALength; i++) {
    float spl0 = *src0++;
    *dst0++ += spl0;
    *dst1++ += spl0;
  }
}

//------------------------------
// mono -> stereo, ticksize
//------------------------------

/*

void SAT_CopyMonoToStereoBuffer(float** ADst, float** ASrc) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc[0];
  for (uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    float spl0 = *src0++;
    *dst0++ = spl0;
    *dst1++ = spl0;
  }
}

//----------

void SAT_AddMonoToStereoBuffer(float** ADst, float** ASrc) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc[0];
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    float spl0 = *src0++;
    *dst0++ += spl0;
    *dst1++ += spl0;
  }
}

//------------------------------

void SAT_CopyMonoToStereoBuffer(float** ADst, float* ASrc) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc;
  for (uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    float spl0 = *src0++;
    *dst0++ = spl0;
    *dst1++ = spl0;
  }
}

//----------

void SAT_AddMonoToStereoBuffer(float** ADst, float* ASrc) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc;
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    float spl0 = *src0++;
    *dst0++ += spl0;
    *dst1++ += spl0;
  }
}

*/

//----------------------------------------------------------------------
#endif
