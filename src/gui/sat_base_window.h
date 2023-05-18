#ifndef sat_base_window_included
#define sat_base_window_included
//----------------------------------------------------------------------

// TODO
#if 0

#include "base/sat.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_BaseWindow {

//------------------------------
public:
//------------------------------

  SAT_BaseWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent) {
  }

  //----------

  virtual ~SAT_BaseWindow() {
  }

//------------------------------
public:
//------------------------------

  virtual void on_window_open() {}
  virtual void on_window_close() {}
  virtual void on_window_move(int32_t AXpos, int32_t AYpos) {}
  virtual void on_window_resize(int32_t AWidth, int32_t AHeight) {}
  virtual void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void on_window_key_press(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_key_release(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouse_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_mouse_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_client_message(uint32_t AData) {}
  
//------------------------------
public:
//------------------------------
  
  virtual void      setPos(uint32_t AXpos, uint32_t AYpos) {}
  virtual void      setSize(uint32_t AWidth, uint32_t AHeight) {}
  virtual void      setTitle(const char* ATitle) {}
  virtual void      show() {}
  virtual void      hide() {}
  virtual void      reparent(intptr_t AParent) {}
  virtual void      invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void      sendClientMessage(uint32_t AData, uint32_t AType) {}
  virtual uint32_t  eventLoop() { return 0; }
  virtual void      startEventThread() {}
  virtual void      stopEventThread() {}

  virtual void      fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) {}
  virtual void      fill(uint32_t AColor) {}

  virtual void      setMouseCursor(int32_t ACursor) {}
  virtual void      setMouseCursorPos(int32_t AXpos, int32_t AYpos) {}
  virtual void      hideMouseCursor(void) {}
  virtual void      showMouseCursor(void) {}
  virtual void      grabMouseCursor(void) {}
  virtual void      releaseMouseCursor(void) {}

};

#endif // 0

//----------------------------------------------------------------------
#endif

