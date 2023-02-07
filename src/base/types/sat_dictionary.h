#ifndef sat_assoc_array_included
#define sat_assoc_array_included
//----------------------------------------------------------------------

/*
  based on:
    http://www.codeproject.com/Articles/149879/Associative-Array-in-C
    by Arnav Mukhopadhyay
*/

//----------------------------------------------------------------------

#include "base/types/sat_array.h"

//----------

template <class T>
class SAT_Dictionary {

//------------------------------
private:
//------------------------------

  struct SAT_DictionaryItem {
    const char* name;
    T value;
  };

  //----------

  SAT_Array<SAT_DictionaryItem> MItems;

//------------------------------
public:
//------------------------------

  uint32_t size() {
    return MItems.size();
  }

  //----------

  bool hasItem(const char* name) {
    for (uint32_t i=0; i<size(); i++) {
      if (strcmp(name,MItems[i].name) == 0) return true; // strcmp
    }
    return false;
  }

  //----------

  T getItem(uint32_t index) {
    return MItems[index].value;
  }

  //----------

  T getItem(const char* name) {
    for (uint32_t i=0; i<size(); i++) {
      if (strcmp(name,MItems[i].name) == 0) return MItems[i].value;
    }
    return nullptr;
  }

  //----------

  int32_t getItemIndex(const char* name) {
    for (uint32_t i=0; i<size(); i++) {
      if (strcmp(name,MItems[i].name) == 0) return i; // strcmp
    }
    return -1;
  }

  //----------

  const char* getItemName(uint32_t index) {
    return MItems[index].name;
  }

  //----------

  T& operator [] (uint32_t index) {
    return MItems[index].value;
  }

  //----------

  T& operator [] (const char* name) {
    for (uint32_t i=0; i<size(); i++) {
      if (strcmp(name,MItems[i].name) == 0) return MItems[i].value; // strcmp
    }
    uint32_t idx = size();
    SAT_DictionaryItem d;
    d.name = name;
    d.value = 0;  // ???
    MItems.append(d);
    return MItems[idx].value;
  }

  //----------

  bool addItem(const char* name, T value) {
    if (hasItem(name)) return false;
    SAT_DictionaryItem d;
    d.name = name;
    d.value = value;
    MItems.append(d);
    return true;
  }

  //----------

  bool removeItem(uint32_t index) {
    MItems.remove(index);
    return true;
  }

  //----------

  bool removeItem(const char* name) {
    for (uint32_t i=0; i<size(); i++) {
      if (strcmp(name,MItems[i].name) == 0) { // strcmp
        MItems.remove(i);
        return true;
      }
    }
    return false;
  }

  //----------

};

//----------------------------------------------------------------------
#endif
