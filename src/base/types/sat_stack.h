#ifndef sat_stack_included
#define sat_stack_included
//----------------------------------------------------------------------

template <typename T, int SIZE>
class SAT_Stack {

//------------------------------
private:
//------------------------------

  T       MStack[SIZE]  = {0};
  int32_t MStackTop     = 0;
  T       MNullItem     = (T)0;

//------------------------------
public:
//------------------------------

  SAT_Stack() {
  }

  //----------

  ~SAT_Stack() {
  }

//------------------------------
public:
//------------------------------

  void setNullItem(T AItem) {
    MNullItem = AItem;
  }

  //----------

  void reset() {
    MStackTop = 0;
  }

  //----------

  int getSize() {
    return SIZE;
  }

  //----------

  int getNumItems() {
    return MStackTop;
  }

  //----------

  bool isEmpty() {
    return (MStackTop > 0);
  }

  //----------

  T* getTop(void) {
    return &MStack[MStackTop-1];
  }

  //----------

  // c  ->  a b -  ->  c a b -

  void push(T AItem) {
    if (MStackTop < SIZE) MStack[MStackTop++] = AItem;
  }

  //----------

  // a b -  ->  b - -  -> [a]

  T pop() {
    if (MStackTop > 0) return MStack[--MStackTop];
    else {
      //printf("ouch! trying to pop from empty stack!\n");
      return MNullItem;
    }
  }

  //----------

  // a b -  ->  a a b -

  void dup(void) {
    MStack[MStackTop] = MStack[MStackTop-1];
    MStackTop++;
  }

  //----------

  // a b -  ->  b a b -

  void dup2(void) {
    MStack[MStackTop] = MStack[MStackTop-2];
    MStackTop++;
  }

  //----------

  // a b -  ->  b - -  ->

  void drop(void) {
    MStackTop--;
  }

  //----------

};

//----------------------------------------------------------------------
#endif