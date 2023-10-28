#ifndef sat_parameter_manager_included
#define sat_parameter_manager_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_editor.h"
#include "plugin/sat_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ParameterManager {

//------------------------------
private:
//------------------------------

  SAT_ParameterArray      MParameters = {};
  SAT_Array<sat_param_t>  MStateBlock = {};

//------------------------------
public:
//------------------------------

  // void appendParameter(const char* AName, sat_param_t AValue) {}
  // void appendParameter(clap_param_info_t AParamInfo) {}
  // void deleteParameters() {}

  uint32_t getNumParameters() {
    return MParameters.size();
  }

  //----------

  SAT_Parameter* getParameter(uint32_t AIndex) {
    return MParameters[AIndex];
  }

  //----------

  SAT_Parameter* findParameter(uint32_t AId) {
    return nullptr;
  }

  //----------

  SAT_Parameter* findParameter(const char* ANAme) {
    return nullptr;
  }

//------------------------------
public:
//------------------------------

  void updateEditorValues(SAT_Editor* AEditor) {
  }

  //----------

  sat_param_t* getStateBuffer(uint32_t* ANum) {
    //todo: fill stte buffer w param values
    *ANum = MStateBlock.size();
    return &MStateBlock[0];
  }

  //----------

  void setStateBuffer(sat_param_t* AParams, uint32_t ANum) {
    //todo: copy param values from buffer
    for (uint32_t i=0; i<ANum; i++) {
      //MParameters[i]->setValue();
    }
  }

};

//----------------------------------------------------------------------
#endif
