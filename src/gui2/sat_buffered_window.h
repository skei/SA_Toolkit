#ifndef sat_buffered_window_included
#define sat_buffered_window_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui2/sat_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_BufferedWindow
: public SAT_Window {

//------------------------------
private:
//------------------------------

  void*             MBuffer                 = nullptr;
  uint32_t          MBufferWidth            = 0;
  uint32_t          MBufferHeight           = 0;
  uint32_t          MAllocatedBufferWidth   = 0;
  uint32_t          MAllocatedBufferHeight  = 0;

//------------------------------
public:
//------------------------------

  SAT_BufferedWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent, SAT_WindowListener* AListener)
  : SAT_Window(AWidth,AHeight,AParent,AListener) {
  }

  //----------

  virtual ~SAT_BufferedWindow() {
  }

//------------------------------
public: // buffer
//------------------------------

  virtual void createBuffer(uint32_t AWidth, uint32_t AHeight) {
    MBufferWidth = AWidth;
    MBufferHeight = AHeight;
    MAllocatedBufferWidth = SAT_NextPowerOfTwo(AWidth);
    MAllocatedBufferHeight = SAT_NextPowerOfTwo(AHeight);
  }
  
  //----------

  virtual void deleteBuffer() {
    MBufferWidth = 0;
    MBufferHeight = 0;
    MAllocatedBufferWidth = 0;
    MAllocatedBufferHeight = 0;
  }

  //----------

  virtual bool resizeBuffer(uint32_t AWidth, uint32_t AHeight) {
    // create new buffer
    // copy old to new buffer
    // delete old buffer
    // set new buffer
    return false;
  }

  //----------

  virtual bool checkBuffer(uint32_t AWidth, uint32_t AHeight) {
    uint32_t w2 = SAT_NextPowerOfTwo(AWidth);
    uint32_t h2 = SAT_NextPowerOfTwo(AHeight);
    if ((w2 != MAllocatedBufferWidth) || (h2 != MAllocatedBufferHeight)) {
      resizeBuffer(w2,h2);
      return true;
    }
    return false;
  }

  //----------
  
  virtual bool activateBuffer() {
    return false;
  }

  //----------

  virtual bool deactivateBuffer() {
    return false;
  }

//------------------------------
public: // window
//------------------------------

  //void on_window_open() override {
  //  SAT_Window::on_window_open();
  //}

  //----------

  //void on_window_close() override {
  //  SAT_Window::on_window_close();
  //}

  //----------

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    bool resized = checkBuffer(AWidth,AHeight);
    if (resized) {}
    SAT_Window::on_window_resize(AWidth,AHeight);
  }

  //----------

  //virtual void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
  //  SAT_Window::on_window_paint(AXpos,AYpos,AWidth,AHeight);
  //}
  
};

//----------------------------------------------------------------------
#endif




