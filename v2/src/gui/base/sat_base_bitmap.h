#ifndef sat_base_bitmap_included
#define sat_base_bitmap_included
//----------------------------------------------------------------------

class SAT_BaseBitmap {

//------------------------------
public:
//------------------------------

  SAT_BaseBitmap() {
  }

  //----------

  SAT_BaseBitmap(uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=32) {
  }

  //----------

  virtual ~SAT_BaseBitmap() {
  }

//------------------------------
public:
//------------------------------

  virtual uint32_t  getWidth()  { return 0; }
  virtual uint32_t  getHeight() { return 0; }
  virtual uint32_t  getDepth()  { return 0; }
  virtual uint32_t  getStride() { return 0; }
  virtual uint32_t* getBuffer() { return nullptr; }

};

//----------------------------------------------------------------------
#endif
