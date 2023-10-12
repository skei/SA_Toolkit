#ifndef sat_x11_window_included
#define sat_x11_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_window.h"
#include "gui/sat_renderer_owner.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_X11Window
: public SAT_RendererOwner
, public SAT_BaseWindow {

//------------------------------
private:
//------------------------------

  Display*  MDisplay = nullptr;

//------------------------------
public:
//------------------------------

  SAT_X11Window()
  : SAT_BaseWindow() {
  }

  //----------

  virtual ~SAT_X11Window() {
  }

//------------------------------
public: // SAT_RendererOwner
//------------------------------

  Display* getX11Display() override {
    return MDisplay;
  }

//------------------------------
public: // SAT_BaseWindow
//------------------------------

  void open() override {
  }

  //----------

  void close() override {
  }

  //----------
  
  void setPos(int32_t AXpos, int32_t AYpos) override {
  }

  //----------
  
  void setSize(int32_t AWidth, int32_t AHeight) override {
  }

  //----------
  
  void setTitle(const char* ATitle) override {
  }

  //----------
  
  void setMousePos() override {
  }

  //----------
  
  void setMouseCursor() override {
  }

  //----------
  
  void hideMouse() override {
  }

  //----------
  
  void showMouse() override {
  }

  //----------
  
  void eventLoop() override {
  }

  //----------
  
  void startEventThread() override {
  }

  //----------
  
  void stopEventThread() override {
  }

  //----------
  
  void startTimer(uint32_t AMSInterval, bool AOneShot) override {
  }

  //----------
  
  void stopTimer() override {
  }

  //----------
  
  void setParent(intptr_t AParent) override {
  }

  //----------
  
  void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
  }

  //----------
  
  void sendUserMessage(intptr_t AMessage) override {
  }

//------------------------------
private:
//------------------------------

  //...

//------------------------------
private:
//------------------------------

  void eventHandler() {
    // call on_windowListener..
  }

//------------------------------
private:
//------------------------------

  static
  void event_thread() {
    // call eventHandler
  }

  //----------
  
  static
  void timer_callback() {
  }

};

//----------------------------------------------------------------------
#endif
