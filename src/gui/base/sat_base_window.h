#ifndef sat_base_window_included
#define sat_base_window_included
//----------------------------------------------------------------------

#include "sat.h"

class SAT_Painter;
class SAT_Renderer;
class SAT_Timer;

//----------------------------------------------------------------------

class SAT_BaseWindow {

//------------------------------
public:
//------------------------------

  SAT_BaseWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0) {
  }

  //----------

  virtual ~SAT_BaseWindow() {
  }

//------------------------------
public:
//------------------------------

  virtual void          on_window_open() {}
  virtual void          on_window_close() {}
  virtual void          on_window_move(int32_t AXpos, int32_t AYpos) {}
  virtual void          on_window_resize(int32_t AWidth, int32_t AHeight) {}
  virtual void          on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void          on_window_keyPress(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) {}
  virtual void          on_window_keyRelease(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) {}
  virtual void          on_window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void          on_window_mouseRelease(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void          on_window_mouseMove(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) {}
  virtual void          on_window_mouseEnter(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void          on_window_mouseLeave(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void          on_window_clientMessage(uint32_t AData) {}
  virtual void          on_window_timer(SAT_Timer* ATimer, double AElapsed) {}

//------------------------------
public:
//------------------------------

  virtual uint32_t      getWindowType() { return 0; }
  virtual const char*   getWindowTypeName() { return ""; }

  virtual SAT_Renderer* getRenderer() { return nullptr; }
  virtual SAT_Painter*  getPainter() { return nullptr; }

  virtual uint32_t      getWidth() { return 0; }
  virtual uint32_t      getHeight() { return 0; }
  virtual uint32_t      getScreenWidth() { return 0; }
  virtual uint32_t      getScreenHeight() { return 0; }
  virtual uint32_t      getScreenDepth() { return 0; }

//------------------------------
public:
//------------------------------

  virtual void          open() {}
  virtual void          close() {}
  virtual void          setPos(int32_t AXpos, int32_t AYpos) {}
  virtual void          setSize(int32_t AWidth, int32_t AHeight) {}
  virtual void          setTitle(const char* ATitle) {}
  virtual void          setMousePos(int32_t AXpos, int32_t AYpos) {}
  virtual void          setMouseCursor(int32_t ACursor) {}
  virtual void          hideMouse() {}
  virtual void          showMouse() {}
  virtual void          grabMouse() {}
  virtual void          releaseMouse() {}
  virtual uint32_t      eventLoop() { return 0; }
  virtual void          startEventThread() {}
  virtual void          stopEventThread() {}
  virtual void          setParent(intptr_t AParent) {}
  virtual void          invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void          sendClientMessage(uint32_t AData, uint32_t AType) {}

//virtual void          sync() {}
//virtual void          flush() {}

};

//----------------------------------------------------------------------
#endif
