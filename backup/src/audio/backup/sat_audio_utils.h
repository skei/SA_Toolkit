#ifndef mip_audio_utils_included
#define mip_audio_utils_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/utils/mip_math.h"

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

//------------------------------
// mono
//------------------------------

void MIP_CopyMonoBuffer(float** ADst, float** ASrc, uint32_t ALength);
void MIP_ClearMonoBuffer(float** ADst, uint32_t ALength);
void MIP_AddMonoBuffer(float** ADst, float** AAdd, uint32_t ALength);
void MIP_ScaleMonoBuffer(float** ADst, float AScale, uint32_t ALength);

void MIP_CopyMonoBuffer(float* ADst, float* ASrc, uint32_t ALength);
void MIP_ClearMonoBuffer(float* ADst, uint32_t ALength);
void MIP_AddMonoBuffer(float* ADst, float* AAdd, uint32_t ALength);
void MIP_ScaleMonoBuffer(float* ADst, float AScale, uint32_t ALength);

// MIP_PLUGIN_PROCESS_TICKSIZE

//void MIP_CopyMonoBuffer(float** ADst, float** ASrc);
//7void MIP_ClearMonoBuffer(float** ADst);
//void MIP_AddMonoBuffer(float** ADst, float** AAdd);
//void MIP_ScaleMonoBuffer(float** ADst, float AScale);

//void MIP_CopyMonoBuffer(float* ADst, float* ASrc);
//void MIP_ClearMonoBuffer(float* ADst);
//void MIP_AddMonoBuffer(float* ADst, float* AAdd);
//void MIP_ScaleMonoBuffer(float* ADst, float AScale);

//------------------------------
// stereo
//------------------------------

void MIP_CopyStereoBuffer(float** ADst, float** ASrc, uint32_t ALength);
void MIP_ClearStereoBuffer(float** ADst, uint32_t ALength);
void MIP_AddStereoBuffer(float** ADst, float** AAdd, uint32_t ALength);
void MIP_ScaleStereoBuffer(float** ADst, float AScale, uint32_t ALength);
void MIP_ScaleStereoBuffer(float** ADst, float ALeft, float ARight, uint32_t ALength);
void MIP_ClampStereoBuffer(float** ADst, float AMin, float AMax, uint32_t ALength);

// MIP_PLUGIN_PROCESS_TICKSIZE

//void MIP_CopyStereoBuffer(float** ADst, float** ASrc);
//void MIP_ClearStereoBuffer(float** ADst);
//void MIP_AddStereoBuffer(float** ADst, float** AAdd);
//void MIP_ScaleStereoBuffer(float** ADst, float AScale);
//void MIP_ScaleStereoBuffer(float** ADst, float ALeft, float ARight);

//------------------------------
// mono -> stereo
//------------------------------

void MIP_CopyMonoToStereoBuffer(float** ADst, float** ASrc, uint32_t ALength);
void MIP_AddMonoToStereoBuffer(float** ADst, float** ASrc, uint32_t ALength);
void MIP_CopyMonoToStereoBuffer(float** ADst, float* ASrc, uint32_t ALength);
void MIP_AddMonoToStereoBuffer(float** ADst, float* ASrc, uint32_t ALength);

// MIP_PLUGIN_PROCESS_TICKSIZE

//void MIP_CopyMonoToStereoBuffer(float** ADst, float** ASrc);
//void MIP_AddMonoToStereoBuffer(float** ADst, float** ASrc);
//void MIP_CopyMonoToStereoBuffer(float** ADst, float* ASrc);
//void MIP_AddMonoToStereoBuffer(float** ADst, float* ASrc);

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

//------------------------------
// mono
//------------------------------

void MIP_CopyMonoBuffer(float** ADst, float** ASrc, uint32_t ALength) {
  memcpy(ADst[0],ASrc[0],ALength*sizeof(float));
}

//----------

void MIP_ClearMonoBuffer(float** ADst, uint32_t ALength) {
  memset(ADst[0],0,ALength*sizeof(float));
}

//----------

void MIP_AddMonoBuffer(float** ADst, float** AAdd, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* add0 = AAdd[0];
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void MIP_ScaleMonoBuffer(float** ADst, float AScale, uint32_t ALength) {
  float* dst0 = ADst[0];
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ *= AScale;
  }
}

//------------------------------

void MIP_CopyMonoBuffer(float* ADst, float* ASrc, uint32_t ALength) {
  memcpy(ADst,ASrc,ALength*sizeof(float));
}

//----------

void MIP_ClearMonoBuffer(float* ADst, uint32_t ALength) {
  memset(ADst,0,ALength*sizeof(float));
}

//----------

void MIP_AddMonoBuffer(float* ADst, float* AAdd, uint32_t ALength) {
  float* dst0 = ADst;
  float* add0 = AAdd;
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void MIP_ScaleMonoBuffer(float* ADst, float AScale, uint32_t ALength) {
  float* dst0 = ADst;
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ *= AScale;
  }
}

//------------------------------
// mono, ticksize
//------------------------------

/*

void MIP_CopyMonoBuffer(float** ADst, float** ASrc) {
  memcpy(ADst[0],ASrc[0],KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void MIP_ClearMonoBuffer(float** ADst) {
  memset(ADst[0],0,KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void MIP_AddMonoBuffer(float** ADst, float** AAdd) {
  float* dst0 = ADst[0];
  float* add0 = AAdd[0];
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void MIP_ScaleMonoBuffer(float** ADst, float AScale) {
  float* dst0 = ADst[0];
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ *= AScale;
  }
}

//------------------------------

void MIP_CopyMonoBuffer(float* ADst, float* ASrc) {
  memcpy(ADst,ASrc,KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void MIP_ClearMonoBuffer(float* ADst) {
  memset(ADst,0,KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void MIP_AddMonoBuffer(float* ADst, float* AAdd) {
  float* dst0 = ADst;
  float* add0 = AAdd;
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void MIP_ScaleMonoBuffer(float* ADst, float AScale) {
  float* dst0 = ADst;
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ *= AScale;
  }
}

*/

//------------------------------
// stereo
//------------------------------

void MIP_CopyStereoBuffer(float** ADst, float** ASrc, uint32_t ALength) {
  memcpy(ADst[0],ASrc[0],ALength*sizeof(float));
  memcpy(ADst[1],ASrc[1],ALength*sizeof(float));
}

//----------

void MIP_ClearStereoBuffer(float** ADst, uint32_t ALength) {
  memset(ADst[0],0,ALength*sizeof(float));
  memset(ADst[1],0,ALength*sizeof(float));
}

//----------

void MIP_AddStereoBuffer(float** ADst, float** AAdd, uint32_t ALength) {
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

void MIP_ScaleStereoBuffer(float** ADst, float AScale, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ *= AScale;
    *dst1++ *= AScale;
  }
}

//----------

void MIP_ScaleStereoBuffer(float** ADst, float ALeft, float ARight, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ *= ALeft;
    *dst1++ *= ARight;
  }
}

//----------

void MIP_ClampStereoBuffer(float** ADst, float AMin, float AMax, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  for(uint32_t i=0; i<ALength; i++) {
    float spl0 = *dst0;
    float spl1 = *dst1;
    spl0 = MIP_Clamp(spl0,AMin,AMax);
    spl1 = MIP_Clamp(spl1,AMin,AMax);
    *dst0++ = spl0;
    *dst1++ = spl1;
  }
}

//------------------------------
// stereo, ticksize
//------------------------------

/*

void MIP_CopyStereoBuffer(float** ADst, float** ASrc) {
  memcpy(ADst[0],ASrc[0],KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
  memcpy(ADst[1],ASrc[1],KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void MIP_ClearStereoBuffer(float** ADst) {
  memset(ADst[0],0,KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
  memset(ADst[1],0,KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void MIP_AddStereoBuffer(float** ADst, float** AAdd) {
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

void MIP_ScaleStereoBuffer(float** ADst, float AScale) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ *= AScale;
    *dst1++ *= AScale;
  }
}

//----------

void MIP_ScaleStereoBuffer(float** ADst, float ALeft, float ARight) {
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

void MIP_CopyMonoToStereoBuffer(float** ADst, float** ASrc, uint32_t ALength) {
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

void MIP_AddMonoToStereoBuffer(float** ADst, float** ASrc, uint32_t ALength) {
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

void MIP_CopyMonoToStereoBuffer(float** ADst, float* ASrc, uint32_t ALength) {
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

void MIP_AddMonoToStereoBuffer(float** ADst, float* ASrc, uint32_t ALength) {
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

void MIP_CopyMonoToStereoBuffer(float** ADst, float** ASrc) {
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

void MIP_AddMonoToStereoBuffer(float** ADst, float** ASrc) {
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

void MIP_CopyMonoToStereoBuffer(float** ADst, float* ASrc) {
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

void MIP_AddMonoToStereoBuffer(float** ADst, float* ASrc) {
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
