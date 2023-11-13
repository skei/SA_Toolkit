#ifndef sat_win32_shm_included
#define sat_win32_shm_included
//----------------------------------------------------------------------

//#include <sys/shm.h>

class SAT_SharedMemory {

//------------------------------
private:
//------------------------------

//  int32_t MId   = -1;
//  int32_t MKey  = -1;

//------------------------------
public:
//------------------------------

  /*
    ftok() converts a pathname and a project identifier to a IPC key


    shmget() returns the identifier of the shared memory segment associated
    with the value of the argument key.  It may be used either to obtain the
    identifier of a previously created shared memory segment (when shmflg is
    zero and key does not have the value IPC_PRIVATE), or to create a new set.

    A new shared memory segment, with size equal to the value of size rounded
    up to a multiple of PAGE_SIZE, is created if key has the value IPC_PRIVATE
    or key isn't IPC_PRIVATE, no shared memory segment corresponding to key
    exists, and IPC_CREAT is specified in shmflg.
  */

  // AFile must exist

  SAT_SharedMemory(uint32_t ASize, const char* AFile) {
//    MKey = ftok(AFile,0xff); //,'R');
//    if (MKey == -1) {
//      perror("ftok");
//      exit(1);
//    }
//    MId = shmget(MKey,ASize,IPC_CREAT|0644);
//    if (MId == -1) {
//      perror("shmget");
//      exit(1);
//    }
  }

  //----------

  ~SAT_SharedMemory() {
  }

//------------------------------
public:
//------------------------------

//  int32_t getId(void)   { return MId; }
//  int32_t getKey(void)  { return MKey; }

//------------------------------
public:
//------------------------------

  /*
    shmat() attaches the System V shared memory segment identified by shmid
    to the address space of the calling process.  The attaching address is
    specified by shmaddr with one of the following criteria:
    * If shmaddr is NULL, the system chooses a suitable (unused) page-aligned
      address to attach the segment.
  */

  void* attach(void) {
//    void* ptr = shmat(MId,nullptr,0);
//    if ((intptr_t)ptr == -1) {
//      perror("shmat");
//      exit(1);
//    }
//    return ptr;
    return nullptr;
  }

  //----------

  /*
    shmdt() detaches the shared memory segment located at the address specified
    by shmaddr from the address space of the calling process.
  */

  void detach(void* ptr) {
//    shmdt(&ptr);
  }

};

//----------------------------------------------------------------------
#endif
