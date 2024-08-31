#ifndef sat_analyrics_included
#define sat_analyrics_included
//----------------------------------------------------------------------

/*
  todo:
    ANALYTICS.addItem("QueueMaxSize");
    ANALYTICS.report("QueueMaxSize",5);
    ..
*/

//#include "sat.h"
#include "base/sat_print.h"

//----------------------------------------------------------------------

class SAT_Analytics {

//------------------------------
private:
//------------------------------

  SAT_Print* MPrint = nullptr;

  #ifdef SAT_INCLUDE_ANALYTICS
    uint32_t ParamFromHostToGui_count     = 0;
    uint32_t ModFromHostToGui_count       = 0;
    uint32_t ParamFromGuiToHost_count     = 0;
    uint32_t ParamFromGuiToAudio_count    = 0;
    uint32_t NoteEndFromAudioToHost_count = 0;

    uint32_t RealignQueue_num_queued = 0;
    uint32_t RealignQueue_num_realigned = 0;
    uint32_t PaintQueue_num_queued = 0;
    uint32_t PaintQueue_num_realigned = 0;
    uint32_t PendingResizeQueue_count = 0;

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
      MPrint->print("  ParamFromHostToGui count     %i\n",ParamFromHostToGui_count);
      MPrint->print("  ModFromHostToGui count       %i\n",ModFromHostToGui_count);
      MPrint->print("  ParamFromGuiToHost count     %i\n",ParamFromGuiToHost_count);
      MPrint->print("  ParamFromGuiToAudio count    %i\n",ParamFromGuiToAudio_count);
      MPrint->print("  NoteEndFromAudioToHost count %i\n",NoteEndFromAudioToHost_count);
      MPrint->print("  RealignQueue num queued      %i\n",RealignQueue_num_queued);
      MPrint->print("  RealignQueue num realigned   %i\n",RealignQueue_num_realigned);
      MPrint->print("  PaintQueue num queued        %i\n",PaintQueue_num_queued);
      MPrint->print("  PaintQueue num realigned     %i\n",PaintQueue_num_realigned);
      MPrint->print("  PendingResizeQueue count     %i\n",PendingResizeQueue_count);
    #endif
  }

//------------------------------
public: // queues
//------------------------------

  // max queue sizes

  #ifdef SAT_INCLUDE_ANALYTICS

    // plugin

    void report_ParamFromHostToGui_count(uint32_t count) {
      if (count > ParamFromHostToGui_count) ParamFromHostToGui_count = count;
    }

    void report_ModFromHostToGui_count(uint32_t count) {
      if (count > ModFromHostToGui_count) ModFromHostToGui_count = count;
    }

    void report_ParamFromGuiToHost_count(uint32_t count) {
      if (count > ParamFromGuiToHost_count) ParamFromGuiToHost_count = count;
    }

    void report_ParamFromGuiToAudio_count(uint32_t count) {
      if (count > ParamFromGuiToAudio_count) ParamFromGuiToAudio_count = count;
    }
    
    void report_NoteEndFromAudioToHost_count(uint32_t count) {
      if (count > NoteEndFromAudioToHost_count) NoteEndFromAudioToHost_count = count;
    }

    // gui

    void report_RealignQueue_num_queued(uint32_t count) {
      if (count > RealignQueue_num_queued) RealignQueue_num_queued = count;
    }

    void report_RealignQueue_num_realigned(uint32_t count) {
      if (count > RealignQueue_num_realigned) RealignQueue_num_realigned = count;
    }

    void report_PaintQueue_num_queued(uint32_t count) {
      if (count > PaintQueue_num_queued) PaintQueue_num_queued = count;
    }

    void report_PaintQueue_num_realigned(uint32_t count) {
      if (count > PaintQueue_num_realigned) PaintQueue_num_realigned = count;
    }

    void report_PendingResizeQueue_count(uint32_t count) {
      if (count > PendingResizeQueue_count) PendingResizeQueue_count = count;
    }

  #else

    void report_ParamFromHostToGui_count(uint32_t count) {}
    void report_ModFromHostToGui_count(uint32_t count) {}
    void report_ParamFromGuiToHost_count(uint32_t count) {}
    void report_ParamFromGuiToAudio_count(uint32_t count) {}
    void report_NoteEndFromAudioToHost_count(uint32_t count) {}

    void report_RealignQueue_num_queued(uint32_t count) {}
    void report_RealignQueue_num_realigned(uint32_t count) {}
    void report_PaintQueue_num_queued(uint32_t count) {}
    void report_PaintQueue_num_realigned(uint32_t count) {}
    void report_PendingResizeQueue_count(uint32_t count) {}


  #endif

};

//----------------------------------------------------------------------
#endif