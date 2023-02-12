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

  void push(T AItem) {
    if (MStackTop < SIZE) MStack[MStackTop++] = AItem;
  }

  //----------

  T pop() {
    if (MStackTop > 0) return MStack[--MStackTop];
    else return MNullItem;
  }

};

//----------------------------------------------------------------------
#endif
