#ifndef sat_base_window_included
#define sat_base_window_included
//----------------------------------------------------------------------

#include "sat.h"

class SAT_Editor;

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

  //virtual intptr_t getHandle() = 0;
  virtual uint32_t getWidth()  = 0;
  virtual uint32_t getHeight() = 0;
  virtual uint32_t getDepth()  = 0;

  //

  virtual bool setupOverlay() { return false; }

//------------------------------
public:
//------------------------------

  virtual void show() = 0;
  virtual void hide() = 0;
  virtual void setPos(int32_t AXpos, int32_t AYpos) = 0;
  virtual void setSize(uint32_t AWidth, uint32_t AHeight) = 0;
  virtual void setTitle(const char* ATitle) = 0;
  virtual void showMouseCursor() = 0;
  virtual void hideMouseCursor() = 0;
  virtual void grabMouseCursor() = 0;
  virtual void releaseMouseCursor() = 0;
  virtual void setMouseCursorPos(int32_t AXpos, int32_t AYpos) = 0;
  virtual void setMouseCursorShape(int32_t ACursor) = 0;
  virtual void reparent(intptr_t AParent) = 0;
  virtual void invalidate(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) = 0;
  virtual void sendClientMessage(uint32_t AData, uint32_t AType) = 0;
  virtual void eventLoop() = 0;
  virtual void startEventThread() = 0;
  virtual void stopEventThread() = 0;

//------------------------------
public:
//------------------------------

  virtual void on_window_show() {}
  virtual void on_window_hide() {}
  virtual void on_window_move(int32_t AXpos, int32_t AYpos) {}
  virtual void on_window_resize(uint32_t AWidth, uint32_t AHeight) {}
  virtual void on_window_paint(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) {}
  //virtual void on_window_realign() {}
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
