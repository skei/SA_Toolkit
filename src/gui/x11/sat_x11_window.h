#ifndef sat_x11_window_included
#define sat_x11_window_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_opengl.h"
#include "gui/x11/sat_x11.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_X11Window {

//------------------------------
private:
//------------------------------

  SAT_OpenGL  MOpenGL = {};

//------------------------------
public:
//------------------------------

  SAT_X11Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent) {
  }

  //----------

  virtual ~SAT_X11Window() {
  }

//------------------------------
public:
//------------------------------

  virtual void setPos(uint32_t AXpos, uint32_t AYpos) {
  }

  virtual void setSize(uint32_t AWidth, uint32_t AHeight) {
  }

  virtual void setTitle(const char* ATitle) {
  }

  virtual void setParent(intptr_t AParent) {
  }

  virtual void show() {
  }

  virtual void hide() {
  }

};

//----------------------------------------------------------------------
#endif
