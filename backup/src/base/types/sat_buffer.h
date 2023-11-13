#ifndef sat_buffer_included
#define sat_buffer_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/utils/sat_integer_math.h"

//----------------------------------------------------------------------
//
// static buffer
//
//----------------------------------------------------------------------

template <class TYPE, int SIZE>
class SAT_StaticBuffer {

//------------------------------
protected:
//------------------------------

  TYPE  MBuffer[SIZE] = {0};

//------------------------------
public:
//------------------------------

  SAT_StaticBuffer() {
  }

  SAT_StaticBuffer(const SAT_StaticBuffer& ABuffer) {
    //SAT_StaticBuffer* buf = (SAT_StaticBuffer*)&ABuffer;
    //memcpy(MBuffer,buf->ptr());
  }

  SAT_StaticBuffer(void* ABuffer, uint32_t ASize) {
    //strcpy(MBuffer,ABuffer,ASize);
  }

  virtual ~SAT_StaticBuffer() {
  }

//------------------------------
public:
//------------------------------

  TYPE& operator [] (const uint32_t AIndex) {
    return MBuffer[AIndex];
  }

  operator TYPE* () {
    return MBuffer;
  }

  operator const TYPE* ()	const {
    return MBuffer;
  }

  SAT_StaticBuffer& operator = (TYPE* ABuffer) {
    memcpy(MBuffer,ABuffer,SIZE * sizeof(TYPE));
    return *this;
  }

  SAT_StaticBuffer& operator = (/*const*/ SAT_StaticBuffer& ABuffer) {
    memcpy(MBuffer,ABuffer,SIZE * sizeof(TYPE));
    return *this;
  }

  //SAT_StaticBuffer& operator += (TYPE* ABuffer) {
  //  //strcat(MBuffer,str);
  //  return *this;
  //}

  //bool operator == (const TYPE* str) {
  //  //return (strcmp(MBuffer,str) == 0) ? true : false;
  //  return false;
  //}

//------------------------------
public:
//------------------------------

  TYPE* ptr() {
    return MBuffer;
  }

  uint32_t size() {
    return SIZE;
  }

  void clearBuffer() {
    memset(MBuffer,0,SIZE*sizeof(TYPE));
  }

  void fillBuffer(TYPE AItem) {
    for (uint32_t i=0; i<SIZE; i++) {
      MBuffer[i] = AItem;
    }
  }

  void fillBuffer() {
    for (uint32_t i=0; i<SIZE; i++) {
      MBuffer[i] = i;
    }
  }

  /*

  todo: cap src/dst to biffer size

  void insertSpace(uint32_t APos, uint32_t ACount) {
    uint32_t num = (MBufferSize - APos);
    resizeBuffer(MBufferSize + ACount);
    TYPE* dst = &MBuffer[APos + ACount];
    TYPE* src = &MBuffer[APos];
    memmove(dst,src,num * sizeof(TYPE));
  }

  void removeSpace(uint32_t APos, uint32_t ACount) {
    TYPE* dst = &MBuffer[APos];
    TYPE* src = &MBuffer[APos + ACount];
    uint32_t num = (MBufferSize - APos);
    memmove(dst,src,num * sizeof(TYPE));
    resizeBuffer(MBufferSize - ACount);
  }

  */

//------------------------------
public:
//------------------------------

  int32_t find(TYPE AItem) {
    for (uint32_t i=0; i<SIZE; i++) {
      if (MBuffer[i] == AItem) return i;
    }
    return -1;
  }

};

//----------------------------------------------------------------------

//template <int SIZE, class T>
//SAT_StaticString<SIZE> operator + (T var, const SAT_StaticString<SIZE>& str)  {
//  SAT_StaticString<SIZE> svar = var;
//  return svar += str;
//}


//----------------------------------------------------------------------
//
// dynamic buffer
//
//----------------------------------------------------------------------

template <class TYPE>
class SAT_DynamicBuffer {

//------------------------------
protected:
//------------------------------

  TYPE*     MBuffer     = nullptr;
  uint32_t  MSize       = 0;
  uint32_t  MAllocated  = 0;

//------------------------------
public:
//------------------------------

  //SAT_DynamicBuffer() {
  //}

  //----------

  virtual ~SAT_DynamicBuffer() {
    deleteBuffer();
  }

//------------------------------
public:
//------------------------------

  TYPE& operator [] (const uint32_t AIndex) {
    return MBuffer[AIndex];
  }

  //----------

  operator TYPE* () {
    return MBuffer;
  }

  //----------

  operator const TYPE* ()	const {
    return MBuffer;
  }

//------------------------------
public:
//------------------------------

  TYPE* ptr() {
    return MBuffer;
  }

  //----------

  uint32_t size() {
    return MSize;
  }

  //----------

  void clearBuffer() {
    memset(MBuffer,0,MSize * sizeof(TYPE));
  }

  //----------

  void fillBuffer(TYPE AItem) {
    for (uint32_t i=0; i<MSize; i++) {
      MBuffer[i] = AItem;
    }
  }

  //----------

  void fillBuffer() {
    for (uint32_t i=0; i<MSize; i++) {
      MBuffer[i] = i;
    }
  }

//------------------------------
public:
//------------------------------

  void createBuffer(uint32_t ASize) {
    uint32_t size = SAT_NextPowerOfTwo(ASize);
    //if (size != MAllocated) {
      MBuffer = (TYPE*)malloc(size * sizeof(TYPE));
      MAllocated = size;
      MSize = ASize;
    //}
  }

  //----------

  void deleteBuffer() {
    if (MBuffer) free(MBuffer);
    MBuffer = nullptr;
    MAllocated = 0;
    MSize = 0;
  }

  //----------

  void resizeBuffer(uint32_t ASize) {
    //if (MAllocated == 0) createBuffer(ASize);
    uint32_t size2 = SAT_NextPowerOfTwo(ASize);
    if (size2 != MAllocated) {
      MBuffer = (TYPE*)realloc(MBuffer,size2 * sizeof(TYPE));
      MAllocated = size2;
    }
    MSize = ASize;
  }

  //----------

  void insertSpace(uint32_t APos, uint32_t ACount) {
    uint32_t num = (MSize - APos);
    resizeBuffer(MSize + ACount);
    TYPE* dst = &MBuffer[APos + ACount];
    TYPE* src = &MBuffer[APos];
    memmove(dst,src,num * sizeof(TYPE));
  }

  //----------

  //TODO
  void insertHead(uint32_t ACount) {
    //insertSpace(0,ACount);
  }

  //----------

  //TODO
  void insertTail(uint32_t ACount) {
    //resizeBuffer(MSize+ACount);
  }

  //----------

  void removeSpace(uint32_t APos, uint32_t ACount) {
    TYPE* dst = &MBuffer[APos];
    TYPE* src = &MBuffer[APos + ACount];
    uint32_t num = (MSize - APos);
    memmove(dst,src,num * sizeof(TYPE));
    resizeBuffer(MSize - ACount);
  }

  //----------

  //TODO
  void removeHead(uint32_t ACount) {
    //removeSpace(0,ACount);
  }

  //----------

  //TODO
  void removeTail(uint32_t ACount) {
    //resizeBuffer(MSize-ACount);
  }

};

//----------------------------------------------------------------------
#endif
