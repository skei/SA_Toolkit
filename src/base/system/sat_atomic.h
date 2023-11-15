#ifndef sat_atomic_included
#define sat_atomic_included
//----------------------------------------------------------------------

/*
  * assumption:
    simple 32-bit reads/writes can be considered atomic
    float, uint32, (32-bit ptr)
  * gcc might use 32-bit operators for moving/setting/etc 64 bit values
*/


// https://gcc.gnu.org/onlinedocs/gcc-4.7.2/gcc/_005f_005fsync-Builtins.html#g_t_005f_005fsync-Builtins

//----------------------------------------------------------------------

struct SAT_Atomic {
  volatile int counter;
};

//----------------------------------------------------------------------

//#define SAT_AtomicInit(i)  {(i)}

//----------

/*
  Read atomic variable
  v :  pointer of type SAT_Atomic
  Atomically reads the value of v.
*/

//#define sat_atomic_read(v) ((v)->counter)

//----------

/*
  Set atomic variable
  v: pointer of type SAT_Atomic
  i: required value
*/

//#define sat_atomic_set(v,i) (((v)->counter)=(i))

//----------------------------------------------------------------------

/*
  Add to the atomic variable
  i: integer value to add
  v: pointer of type SAT_Atomic
*/

//----------

static inline void SAT_AtomicAdd(int i, SAT_Atomic* v) {
  (void)__sync_add_and_fetch(&v->counter,i);
}

//static inline int SAT_AtomicAdd(int i, SAT_Atomic* v) {
//  return __sync_add_and_fetch(&v->counter, i);
//}

//----------

/*
  Subtract the atomic variable
  i: integer value to subtract
  v: pointer of type SAT_Atomic
  Atomically subtracts i from v.
*/

static inline void SAT_AtomicSub(int i, SAT_Atomic* v) {
  (void)__sync_sub_and_fetch(&v->counter,i);
}

//----------

/*
  Subtract value from variable and test result
  i: integer value to subtract
  v: pointer of type SAT_Atomic

  Atomically subtracts i from v and returns true if the result is zero,
  or false for all other cases.
*/

static inline int SAT_AtomicSubAndTest(int i, SAT_Atomic* v) {
  return !(__sync_sub_and_fetch(&v->counter, i));
}

//----------

/*
  Increment atomic variable
  v: pointer of type SAT_Atomic
  Atomically increments v by 1.
*/

static inline void SAT_AtomicInc(SAT_Atomic* v) {
  (void)__sync_fetch_and_add(&v->counter,1);
}

//----------

/*
  decrement atomic variable
  v: pointer of type SAT_Atomic
  Atomically decrements v by 1.
  Note that the guaranteed useful range of an SAT_Atomic is only 24 bits.
*/

static inline void SAT_AtomicDec(SAT_Atomic* v) {
  (void)__sync_fetch_and_sub(&v->counter,1);
}

//----------

/*
  Decrement and test
  v: pointer of type SAT_Atomic

  Atomically decrements @v by 1 and
  returns true if the result is 0, or false for all other
  cases.
*/

static inline int SAT_AtomicDecAndTest(SAT_Atomic* v) {
  return !(__sync_sub_and_fetch(&v->counter,1));
}

//----------

/*
  Increment and test
  v; pointer of type SAT_Atomic
  Atomically increments v by 1
  returns true if the result is zero, or false for all other cases.
*/

static inline int SAT_AtomicIncAndTest( SAT_Atomic *v ) {
  return !(__sync_add_and_fetch(&v->counter, 1));
}

//----------

/*
  add and test if negative
  v: pointer of type SAT_Atomic
  i: integer value to add
  Atomically adds i to v
  returns true if the result is negative,
  or false when result is greater than or equal to zero.
*/

static inline int SAT_AtomicAddNegative(int i, SAT_Atomic *v) {
  return (__sync_add_and_fetch(&v->counter, i) < 0);
}

//----------------------------------------------------------------------

/*
  compare and swap
  v: pointer of type SAT_Atomic
  If the current value of v is oldval, then write newval into v.
  Returns #TRUE if the comparison is successful and newval was written.
*/

static inline int SAT_AtomicCompareAndSwap(SAT_Atomic* v, int oldval, int newval) {
  return __sync_bool_compare_and_swap(&v->counter,oldval,newval);
}

//----------------------------------------------------------------------
#endif
