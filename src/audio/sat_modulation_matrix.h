#ifndef sat_modulation_matrix_included
#define sat_modulation_matrix_included
//----------------------------------------------------------------------

// hard-coded modulation matrix, x rows, source -> dest
// dst = ((amount * scale) + offset
// then use dst as amount in calculations

#include "base/sat.h"

//----------------------------------------------------------------------

class SAT_ModulationMatrix {

//------------------------------
private:
//------------------------------

  uint32_t    MSrcCount = 0;
  uint32_t    MDstCount = 0;
  uint32_t    MModCount = 0;

  const char* MSrcNames[SAT_MODMATRIX_MAX_SRC_COUNT]     = {0};
  sat_param_t MSrcValues[SAT_MODMATRIX_MAX_SRC_COUNT]    = {0};
  uint32_t    MSrcUpdate[SAT_MODMATRIX_MAX_SRC_COUNT]    = {0};

  const char* MDstNames[SAT_MODMATRIX_MAX_DST_COUNT]     = {0};
  sat_param_t MDstValues[SAT_MODMATRIX_MAX_DST_COUNT]    = {0};
  uint32_t    MDstUpdate[SAT_MODMATRIX_MAX_SRC_COUNT]    = {0};

  uint32_t    MModSrcIndex[SAT_MODMATRIX_MAX_MOD_COUNT]  = {0};
  uint32_t    MModDstIndex[SAT_MODMATRIX_MAX_MOD_COUNT]  = {0};
  sat_param_t MModAmount[SAT_MODMATRIX_MAX_MOD_COUNT]    = {0};
  sat_param_t MModScale[SAT_MODMATRIX_MAX_MOD_COUNT]     = {0};
  sat_param_t MModOffset[SAT_MODMATRIX_MAX_MOD_COUNT]    = {0};
  uint32_t    MModMode[SAT_MODMATRIX_MAX_MOD_COUNT]      = {0};

//------------------------------
public:
//------------------------------

  SAT_ModulationMatrix() {
  }

  //----------

  ~SAT_ModulationMatrix() {
  }

//------------------------------
public:
//------------------------------

  uint32_t addSrc(const char* AName, uint32_t AUpdate) {
    //SAT_Trace("src %i %s\n",MSrcCount,AName);
    MSrcNames[MSrcCount] = AName;
    MSrcUpdate[MSrcCount] = AUpdate;
    MSrcCount++;
    return MSrcCount;
  }

  //----------

  uint32_t addDst(const char* AName, uint32_t AUpdate) {
    //SAT_Trace("dst %i %s\n",MDstCount,AName);
    MDstNames[MDstCount] = AName;
    MDstUpdate[MDstCount] = AUpdate;
    MDstCount++;
    return MDstCount;
  }

  //----------

  uint32_t addMod(uint32_t AModSrcIndex, uint32_t AModDstIndex, sat_param_t AModAmount, sat_param_t AModScale, sat_param_t AModOffset, uint32_t AModMode) {
    //SAT_Trace("mod %i src %i dst %i\n",MModCount,AModSrcIndex,AModDstIndex);
    MModSrcIndex[MModCount] = AModSrcIndex;
    MModDstIndex[MModCount] = AModDstIndex;
    MModAmount[MModCount]   = AModAmount;
    MModScale[MModCount]    = AModScale;
    MModOffset[MModCount]   = AModOffset;
    MModMode[MModCount]     = AModMode;
    MModCount++;
    return MModCount;
  }

  //----------

  void setSrcValue(uint32_t AIndex, sat_param_t AValue)   { MSrcValues[AIndex] = AValue; }
  void setDstValue(uint32_t AIndex, sat_param_t AValue)   { MDstValues[AIndex] = AValue; }
  void setModSrcIndex(uint32_t AIndex, uint32_t AValue)   { MModSrcIndex[AIndex] = AValue;  }
  void setModDstIndex(uint32_t AIndex, uint32_t AValue)   { MModDstIndex[AIndex] = AValue;  }
  void setModAmount(uint32_t AIndex, sat_param_t AValue)  { MModAmount[AIndex] = AValue;  }
  void setModScale(uint32_t AIndex, sat_param_t AValue)   { MModScale[AIndex] = AValue;  }
  void setModOffset(uint32_t AIndex, sat_param_t AValue)  { MModOffset[AIndex] = AValue;  }
  void setModMode(uint32_t AIndex, uint32_t AValue)       { MModMode[AIndex] = AValue;  }

  uint32_t    getSrcCount()                   { return MSrcCount; }
  uint32_t    getDstCount()                   { return MDstCount; }
  uint32_t    getModCount()                   { return MModCount; }

  sat_param_t getSrcValue(uint32_t AIndex)    { return MSrcValues[AIndex]; }
  sat_param_t getDstValue(uint32_t AIndex)    { return MDstValues[AIndex]; }
  uint32_t    getModSrcIndex(uint32_t AIndex) { return MModSrcIndex[AIndex];  }
  uint32_t    getModDstIndex(uint32_t AIndex) { return MModDstIndex[AIndex];  }
  sat_param_t getModAmount(uint32_t AIndex)   { return MModAmount[AIndex];  }
  sat_param_t getModScale(uint32_t AIndex)    { return MModScale[AIndex];  }
  sat_param_t getModOffset(uint32_t AIndex)   { return MModOffset[AIndex];  }
  uint32_t    getModMode(uint32_t AIndex)     { return MModMode[AIndex];  }

  //----------

  void process(uint32_t AMode, uint32_t AUpdate) {
    for (uint32_t i=0; i<MDstCount; i++) {
      //if (MModMode[i] == AMode) MDstValues[i] = 0;
      MDstValues[i] = 0.0;
    }
    for (uint32_t i=0; i<MModCount; i++) {
      uint32_t src_index  = MModSrcIndex[i];
      uint32_t dst_index  = MModDstIndex[i];
      uint32_t mode       = MModMode[i];
      if (mode == AMode) {
        if (dst_index > 0) {
          if (src_index > 0) {
            sat_param_t result = MSrcValues[src_index];
            result *= ( MModAmount[i] * MModScale[i] );
            result += MModOffset[i];
            MDstValues[dst_index] += result;
          }
        }
      }
    }
  }

};

//----------------------------------------------------------------------
#endif
