#ifndef sat_array_included
#define sat_array_included
//----------------------------------------------------------------------

//#include <vector>
//#define SAT_Array std::vector

#include "base/utils/sat_integer_math.h" // SAT_NextPowerOfTwo
//#include "base/sat_stdlib.h"

//----------------------------------------------------------------------

template<class _T>
class SAT_Array {

//------------------------------
public:
//------------------------------

  SAT_Array() {
    //MBuffer     = nullptr;
    //MBufferSize = 0;
    //MSize       = 0;
  }

  //----------

  SAT_Array(const SAT_Array& AArray) {
    MBuffer = (_T*)malloc(MTypeSize*AArray.MBufferSize);
    MBufferSize = AArray.MBufferSize;
    MSize = AArray.MSize;
    memcpy(MBuffer, AArray.MBuffer, MTypeSize*AArray.MBufferSize);
  }

  //----------

  ~SAT_Array() {
    if (MBuffer) {
      free(MBuffer);
    }
  }

//------------------------------
public:
//------------------------------

  uint32_t  size(void)                    { return MSize; }
  void*     buffer(void)                  { return MBuffer; }
  _T&       item(const uint32_t AIndex)   { return MBuffer[AIndex]; }
  uint32_t  bufferSize(void)              { return MBufferSize; }

  void      resize(const uint32_t ASize)  { resizeBuffer(ASize); }

//------------------------------
public:
//------------------------------

  _T& getItem(const uint32_t AIndex) {
    return MBuffer[AIndex];
  }

  //----------

  _T& operator [] (const uint32_t AIndex) {
    return MBuffer[AIndex];
  }

  //----------

  SAT_Array<_T>& operator = (const SAT_Array<_T> &AArray) {
    if (this == &AArray) return *this;
//      if (AArray.MSize == 0) clear();
//      setSize(AArray.MSize);
    memcpy(MBuffer, AArray.MBuffer, MTypeSize*AArray.MSize);
    return *this;
  }

//------------------------------
private:
//------------------------------

  void freeItems(void) {
    for (uint32_t i=0; i<MSize; i++) free( MBuffer[i] );
  }

  //----------

  void deleteItems(void) {
    for (uint32_t i=0; i<MSize; i++) delete MBuffer[i];
  }

  //----------

  void clearBuffer(bool AErase=false) {
    if (AErase) {
      if (MBuffer) free(MBuffer);
      MBuffer = nullptr;
      MBufferSize = 0;
    }
    MSize = 0;
  }

  //----------

  void resizeBuffer(const uint32_t ABufferSize) {
    uint32_t size = SAT_NextPowerOfTwo(ABufferSize);
    if (size != MBufferSize) {
      MBuffer = (_T*)realloc(MBuffer, MTypeSize*size);
      MBufferSize = size;
      //MSize = size;
      //if (MSize > MBufferSize) MSize = MBufferSize;
    }
  }

  //----------

  void growBuffer(const uint32_t ANum) {
    uint32_t newsize = MSize + ANum;
    resizeBuffer(newsize);
  }

  //----------

  void shrinkBuffer(const uint32_t ANum) {
    uint32_t newsize = MSize - ANum;
    resizeBuffer(newsize);
  }

//------------------------------
public:
//------------------------------

  //int32_t findItem(_T& AItem) {
  //  for (uint32_t i=0; i<MSize; i++) {
  //    if (MBuffer[i] == AItem) return i;
  //  }
  //  return -1;
  //}

  void clear(bool AErase=false) {
    clearBuffer(AErase);
  }

  //----------

  void insert(const _T& AItem, uint32_t APos) {
    growBuffer(1);
    memmove( &MBuffer[APos+1], &MBuffer[APos], (MSize-APos) * MTypeSize);
    MBuffer[APos] = AItem;
    MSize++;
  }

  //----------

  void insert(SAT_Array<_T>& AArray, uint32_t APos) {
    uint32_t num = AArray.size();
    growBuffer(num);
    memmove( &MBuffer[APos+num], &MBuffer[APos], (MSize-APos) * MTypeSize);
    memcpy( &MBuffer[APos], AArray.buffer(), num * MTypeSize );
    MSize += num;
  }

  //----------

  void insertHead(const _T& AItem) {
    insert(AItem,0);
  }

  //----------

  void insertTail(const _T& AItem) {
    growBuffer(1);
    MBuffer[MSize] = AItem;
    MSize++;
  }

  //------------------------------

  void remove(const uint32_t APos) {
    memmove( &MBuffer[APos], &MBuffer[APos+1], (MSize-APos-1) * MTypeSize);
    shrinkBuffer(1);
    MSize--;
  }

  //----------

  void remove(const uint32_t APos, uint32_t ANum) {
    //moveBufferItems(APos+ANum,APos,ANum);
    memmove( &MBuffer[APos], &MBuffer[APos+ANum], (MSize-APos-ANum) * MTypeSize);
    shrinkBuffer(ANum);
    MSize -= ANum;
  }

  //----------
  
  void remove(const _T& AItem) {
    int32_t index = find(AItem);
    if (index >= 0) remove(index);
  }
  
  //----------

  void removeHead(void) {
    remove(0);
  }

  //----------

  void removeTail(void) {
    MSize--;
    shrinkBuffer(1);
  }

//------------------------------
public:
//------------------------------

  int32_t find(const _T& AItem) {
    for (uint32_t i=0; i<MSize; i++) {
      if (MBuffer[i] == AItem) return i;
    }
    return -1;
  }

  //----------

  void append(_T& AItem) {
    insertTail(AItem);
  }

  void push_back(_T& AItem) {
    insertTail(AItem);
  }

  //----------

  _T& head(void) {
    return MBuffer[0];
  }

  //----------

  _T& tail(void) {
    return MBuffer[MSize-1];
  }

  //----------

  void push(_T AItem) {
    insertTail(AItem);
  }

  //----------

  _T pop(void) {
    removeTail();
    return MBuffer[MSize];
  }

//------------------------------
private:
//------------------------------

  const uint32_t	MTypeSize   = sizeof(_T);
  _T*             MBuffer     = nullptr;
  uint32_t	      MBufferSize = 0;
  uint32_t	      MSize       = 0;

};

//----------------------------------------------------------------------

typedef SAT_Array<char>          SAT_CharArray;
typedef SAT_Array<char*>         SAT_CharPtrArray;
typedef SAT_Array<const char>    SAT_ConstCharArray;
typedef SAT_Array<const char*>   SAT_ConstCharPtrArray;
typedef SAT_Array<void*>         SAT_PtrArray;
typedef SAT_Array<void**>        SAT_PtrPtrArray;
typedef SAT_Array<float*>        SAT_FloatPtrArray;
typedef SAT_Array<float>         SAT_FloatArray;
typedef SAT_Array<bool>          SAT_BoolArray;
typedef SAT_Array<int32_t>       SAT_Int32Array;
typedef SAT_Array<uint32_t>      SAT_Uint32Array;

//typedef SAT_Array<SAT_String>   SAT_StringArray;
//typedef SAT_Array<SAT_Sample>   SAT_SampleArray;
//typedef SAT_Array<SAT_Sample*>  SAT_SamplePtrArray;

//----------------------------------------------------------------------
#endif