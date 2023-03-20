#ifndef sat_win32_window_included
#define sat_win32_window_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/win32/sat_win32.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Win32Window {

//------------------------------
private:
//------------------------------

  bool                        MIsEmbedded           = false;
  bool                        MIsCursorHidden       = false;

//------------------------------
protected:
//------------------------------

   int32_t                    MWindowXpos           = 0;
   int32_t                    MWindowYpos           = 0;
  uint32_t                    MWindowWidth          = 0;
  uint32_t                    MWindowHeight         = 0;

//------------------------------
public:
//------------------------------

  SAT_Win32Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent) {

    MWindowXpos   = 0;
    MWindowYpos   = 0;
    MWindowWidth  = AWidth;
    MWindowHeight = AHeight;

    if (AParent) {
      MIsEmbedded = true;
      //..
    }
    else {
      MIsEmbedded = false;
      //..
    }

  }

  //----------

  virtual ~SAT_Win32Window() {
    if (MIsMapped) hide();
  }

//------------------------------
public:
//------------------------------

  //HWND  getWin32Window()  { return MWindow;}

//------------------------------
public:
//------------------------------

  virtual void on_window_open() {}
  virtual void on_window_close() {}
  virtual void on_window_move(int32_t AXpos, int32_t AYpos) {}
  virtual void on_window_resize(int32_t AWidth, int32_t AHeight) {}
  virtual void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void on_window_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_client_message(uint32_t AData) {}

//------------------------------
public:
//------------------------------

  virtual void setPos(uint32_t AXpos, uint32_t AYpos) {
    MWindowXpos = AXpos;
    MWindowYpos = AYpos;
  }

  //----------

  virtual void setSize(uint32_t AWidth, uint32_t AHeight) {
    MWindowWidth = AWidth;
    MWindowHeight = AHeight;
  }

  //----------

  virtual void setTitle(const char* ATitle) {
  }

  //----------

  virtual void show() {
  }

  //----------

  virtual void hide() {
  }

//------------------------------
public:
//------------------------------

  void reparent(intptr_t AParent) {
    if (AParent != 0) {
      MIsEmbedded = true;
    }
  }

  //----------

  void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
  }

  //----------

  void sendClientMessage(uint32_t AData, uint32_t AType) {
  }

  //----------


  //----------

  uint32_t eventLoop() {
    return 0;
  }

//------------------------------
private:
//------------------------------

  void startEventThread() {
    MIsEventThreadActive = true;
  }

  //----------

  void stopEventThread() {
    MIsEventThreadActive = false;
  }

  //----------

  void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) {
  }

  //----------

  void fill(uint32_t AColor) {
  }

//------------------------------
public: // mouse
//------------------------------

  void setCursor(int32_t ACursor) {
  }

  //----------

  void setCursorPos(int32_t AXpos, int32_t AYpos) {
  }

  //----------

  void hideCursor(void) {
    if (!MIsCursorHidden) {
      MIsCursorHidden = true;
      //..
    }
  }

  //----------

  void showCursor(void) {
    if (MIsCursorHidden) {
      MIsCursorHidden = false;
      //..
    }
  }

  //----------

  void grabCursor(void) {
  }

  //----------

  void releaseCursor(void) {
  }

};

//----------------------------------------------------------------------
#endif





