#ifndef sat_base_window_included
#define sat_base_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_window_listener.h"

//----------------------------------------------------------------------

class SAT_BaseWindow
: public SAT_WindowListener {

//------------------------------
private:
//------------------------------

  SAT_WindowListener* MListener = this;

//------------------------------
public:
//------------------------------

  SAT_BaseWindow() {
  }

  //----------

  virtual ~SAT_BaseWindow() {
  }

//------------------------------
public:
//------------------------------

  void setListener(SAT_WindowListener* AListener) {
    if (MListener) MListener = AListener;
    else MListener = this;
  }

//------------------------------
public:
//------------------------------

  virtual void open() {}
  virtual void close() {}
  virtual void setPos(int32_t AXpos, int32_t AYpos) {}
  virtual void setSize(int32_t AWidth, int32_t AHeight) {}
  virtual void setTitle(const char* ATitle) {}
  virtual void setMousePos() {}
  virtual void setMouseCursor() {}
  virtual void hideMouse() {}
  virtual void showMouse() {}
  virtual void eventLoop() {}
  virtual void startEventThread() {}
  virtual void stopEventThread() {}
  virtual void startTimer(uint32_t AMSInterval, bool AOneShot) {}
  virtual void stopTimer() {}
  virtual void setParent(intptr_t AParent) {}
  virtual void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void sendUserMessage(intptr_t AMessage) {}

};

//----------------------------------------------------------------------
#endif
