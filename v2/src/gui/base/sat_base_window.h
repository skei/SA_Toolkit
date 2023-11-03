#ifndef sat_base_window_included
#define sat_base_window_included
//----------------------------------------------------------------------

#include "sat.h"
//#include "gui/sat_renderer.h"
#include "gui/base/sat_base_renderer.h"
#include "gui/base/sat_base_painter.h"

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

  virtual uint32_t          getWindowType()                                                                                           { return 0; }
  virtual const char*       getWindowTypeName()                                                                                       { return ""; }
  virtual SAT_BaseRenderer* getRenderer()                                                                                             { return nullptr; }
  virtual SAT_BasePainter*  getPainter()                                                                                              { return nullptr; }

//------------------------------
public:
//------------------------------

  virtual void              on_window_open()                                                                                          {}
  virtual void              on_window_close()                                                                                         {}
  virtual void              on_window_move(int32_t AXpos, int32_t AYpos)                                                              {}
  virtual void              on_window_resize(int32_t AWidth, int32_t AHeight)                                                         {}
  virtual void              on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight)                            {}
  virtual void              on_window_key_press(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime)                     {}
  virtual void              on_window_key_release(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime)                   {}
  virtual void              on_window_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime)    {}
  virtual void              on_window_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime)  {}
  virtual void              on_window_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime)                       {}
  virtual void              on_window_mouse_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime)                                       {}
  virtual void              on_window_mouse_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime)                                       {}
  virtual void              on_window_client_message(uint32_t AData) {}

//------------------------------
public:
//------------------------------

  virtual uint32_t          getWidth()                                                                                                { return 0; }
  virtual uint32_t          getHeight()                                                                                               { return 0; }
  virtual uint32_t          getScreenWidth()                                                                                          { return 0; }
  virtual uint32_t          getScreenHeight()                                                                                         { return 0; }
  virtual uint32_t          getScreenDepth()                                                                                          { return 0; }

//------------------------------
public:
//------------------------------

  virtual void              open()                                                                                                    {}
  virtual void              close()                                                                                                   {}
  virtual void              setPos(int32_t AXpos, int32_t AYpos)                                                                      {}
  virtual void              setSize(int32_t AWidth, int32_t AHeight)                                                                  {}
  virtual void              setTitle(const char* ATitle)                                                                              {}
  virtual void              setMousePos(int32_t AXpos, int32_t AYpos)                                                                 {}
  virtual void              setMouseCursor(int32_t ACursor)                                                                           {}
  virtual void              hideMouse()                                                                                               {}
  virtual void              showMouse()                                                                                               {}
  virtual void              grabMouse()                                                                                               {}
  virtual void              releaseMouse()                                                                                            {}
  virtual uint32_t          eventLoop()                                                                                               { return 0; }
  virtual void              startEventThread()                                                                                        {}
  virtual void              stopEventThread()                                                                                         {}
  //virtual void              startTimer(uint32_t AMSInterval, bool AOneShot)                                                           {}
  //virtual void              stopTimer()                                                                                               {}
  virtual void              setParent(intptr_t AParent)                                                                               {}
  virtual void              invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight)                                 {}
  virtual void              sendClientMessage(uint32_t AData, uint32_t AType)                                                         {}
//virtual void              sync()                                                                                                    {}
//virtual void              flush()                                                                                                   {}

};

//----------------------------------------------------------------------
#endif
