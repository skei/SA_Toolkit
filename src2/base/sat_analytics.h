#ifndef sat_analyrics_included
#define sat_analyrics_included
//----------------------------------------------------------------------

//#include "sat.h"
#include "base/sat_print.h"


//----------------------------------------------------------------------

class SAT_Analytics {

//------------------------------
private:
//------------------------------

  SAT_Print* MPrint = nullptr;

  #ifdef SAT_INCLUDE_ANALYTICS
    uint32_t _ParamFromHostToGui  = 0;
    uint32_t _ModFromHostToGui    = 0;
    uint32_t _ParamFromGuiToHost  = 0;
    uint32_t _ParamFromGuiToAudio = 0;
  #endif

//------------------------------
public:
//------------------------------

  SAT_Analytics() {
  }

  //----------

  ~SAT_Analytics() {
  }

//------------------------------
public:
//------------------------------

  bool initialize(SAT_Print* APrint) {
    MPrint = APrint;
    #ifdef SAT_INCLUDE_ANALYTICS
    #endif
    return true;
  }

  void cleanup() {
    #ifdef SAT_INCLUDE_ANALYTICS
      dumpAnalytics();
    #endif
  }

//------------------------------
public:
//------------------------------

  void dumpAnalytics() {
    #ifdef SAT_INCLUDE_ANALYTICS
      MPrint->print("ANALYTICS:\n");
      MPrint->print("  _ParamFromHostToGui  %i\n",_ParamFromHostToGui);
      MPrint->print("  _ModFromHostToGui    %i\n",_ModFromHostToGui);
      MPrint->print("  _ParamFromGuiToHost  %i\n",_ParamFromGuiToHost);
      MPrint->print("  _ParamFromGuiToAudio %i\n",_ParamFromGuiToAudio);
    #endif
  }

//------------------------------
public: // queues
//------------------------------

  void set_ParamFromHostToGui(uint32_t count) {
    #ifdef SAT_INCLUDE_ANALYTICS
      if (count > _ParamFromHostToGui) _ParamFromHostToGui = count;
    #endif
  }

  void set_ModFromHostToGui(uint32_t count) {
    #ifdef SAT_INCLUDE_ANALYTICS
      if (count > _ModFromHostToGui) _ModFromHostToGui = count;
    #endif
  }

  void set_ParamFromGuiToHost(uint32_t count) {
    #ifdef SAT_INCLUDE_ANALYTICS
      if (count > _ParamFromGuiToHost) _ParamFromGuiToHost = count;
    #endif
  }

  void set_ParamFromGuiToAudio(uint32_t count) {
    #ifdef SAT_INCLUDE_ANALYTICS
      if (count > _ParamFromGuiToAudio) _ParamFromGuiToAudio = count;
    #endif
  }

};

//----------------------------------------------------------------------
#endif