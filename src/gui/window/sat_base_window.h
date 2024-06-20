#ifndef sat_base_window_included
#define sat_base_window_included
//----------------------------------------------------------------------

#include "sat.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_BaseWindow {

//------------------------------
public:
//------------------------------

  SAT_BaseWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0) {}
  virtual ~SAT_BaseWindow() {}

//------------------------------
public:
//------------------------------

  virtual bool setupOverlay() { return false; }

  virtual void      show() {}
  virtual void      hide() {}

  virtual void      setPos(int32_t AXpos, int32_t AYpos) {}
  virtual void      setSize(uint32_t AWidth, uint32_t AHeight) {}
  virtual void      setTitle(const char* ATitle) {}

  virtual void      setMousePos(int32_t AXpos, int32_t AYpos) {}
  virtual void      setMouseCursor(int32_t ACursor) {}
  virtual void      hideMouse() {}
  virtual void      showMouse() {}
  virtual void      grabMouse() {}
  virtual void      releaseMouse() {}

  virtual void      reparent(intptr_t AParent) {}
  virtual void      invalidate(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) {}
  virtual void      sendClientMessage(uint32_t AData, uint32_t AType) {}

  virtual uint32_t  eventLoop() { return 0; }
  virtual void      startEventThread() {}
  virtual void      stopEventThread() {}

//------------------------------
public:
//------------------------------

  virtual void on_window_show() {}
  virtual void on_window_hide() {}
  virtual void on_window_move(int32_t AXpos, int32_t AYpos) {}
  virtual void on_window_resize(uint32_t AWidth, uint32_t AHeight) {}

  // virtual void on_window_prerender(uint32_t AWidth, uint32_t AHeight) {}
  // virtual void on_window_prepaint(uint32_t AWidth, uint32_t AHeight) {}
  virtual void on_window_paint(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) {}
  // virtual void on_window_postpaint() {}
  // virtual void on_window_postrender() {}

  virtual void on_window_realign() {}
  
  virtual void on_window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouseRelease(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouseMove(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_keyPress(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_keyRelease(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) {}

  virtual void on_window_mouseEnter(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_mouseLeave(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}

  virtual void on_window_clientMessage(uint32_t AData) {}

};

//----------------------------------------------------------------------
#endif