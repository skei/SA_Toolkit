#ifndef sat_bitmap_included
#define sat_bitmap_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_bitmap.h"

//----------------------------------------------------------------------

class SAT_Bitmap
: public class SAT_BaseBitmap {

//------------------------------
private:
//------------------------------

  uint32_t  MBufferSize = 0;
  uint32_t* MBuffer     = nullptr;

  uint32_t  MWidth      = 0;
  uint32_t  MHeight     = 0;
  uint32_t  MDepth      = 0;
  uint32_t  MStride     = 0;

//------------------------------
public:
//------------------------------

  SAT_Bitmap()
  : SAT_BaseBitmap() {
  }

  //----------

  SAT_Bitmap(uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=32)
  : SAT_BaseBitmap(AWidth,AHeight,ADepth) {
  }

  //----------

  virtual ~SAT_Bitmap() {
  }

//------------------------------
public:
//------------------------------

  uint32_t  getWidth()  override { return 0; }
  uint32_t  getHeight() override { return 0; }
  uint32_t  getDepth()  override { return 0; }
  uint32_t  getStride() override { return 0; }
  uint32_t* getBuffer() override { return nullptr; }

};

//----------------------------------------------------------------------
#endif