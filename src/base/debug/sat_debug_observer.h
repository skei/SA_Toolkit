#ifndef sat_debug_observer_included
#define sat_debug_observer_included
//----------------------------------------------------------------------

#ifdef SAT_DEBUG_OBSERVER

  #include "base/sat.h"

  //----------------------------------------------------------------------

  struct SAT_Observable {
    uint32_t    type;
    void*       ptr;
    const char* desc;
  };

  typedef SAT_Array<SAT_Observable> SAT_Observables;

  //----------------------------------------------------------------------
  //
  //
  //
  //----------------------------------------------------------------------

  //#ifdef SAT_DEBUG_MEMTRACE


  class SAT_Observer {

  //------------------------------
  private:
  //------------------------------

    //uint32_t        MNumObservables                         = 0;
    //SAT_Observable  MObservables[SAT_DEBUG_MAX_OBSERVABLES] = {0};
    SAT_Observables  MObservables = {};

  //------------------------------
  public:
  //------------------------------

    SAT_Observer() {
    }
    
    //----------

    ~SAT_Observer() {
    }
    
  //------------------------------
  public: // observer
  //------------------------------

    void observe(uint32_t AType, void* APtr, const char* ADesc) {
      //MObservables[MNumObservables].type = AType;
      //MObservables[MNumObservables].ptr  = APtr;
      //MObservables[MNumObservables].desc = ADesc;
      SAT_Observable obs;
      obs.type = AType;
      obs.ptr  = APtr;
      obs.desc = ADesc;
      MObservables.append(obs);
    }
    
    //----------
    
    void unobserve(void* APtr) {
      for (uint32_t i=0; i<MObservables.size(); i++) {
        if (MObservables[i].ptr == APtr) {
          MObservables.remove(i);
          return;
        }
      }
    }
    
    //----------

    void print_observers() {
      if (MObservables.size() > 0) {
        SAT_DPrint("\nObserved:\n");
        for (uint32_t i=0; i<MObservables.size(); i++) {
          switch (MObservables[i].type) {
            
            case SAT_OBSERVE_DOUBLE: {
              SAT_DPrint("  %i. %s = %.3f\n",i,MObservables[i].desc,*(double*)MObservables[i].ptr);
              break;
            }
            
          }
        }
        //print("\n");
      }
    }
    
    //----------
    
    uint32_t getNumObservers() {
      return MObservables.size();
    }
    
    //----------
    
    SAT_Observable* getObserver(uint32_t AIndex) {
      return &MObservables[AIndex];
    }

  };

  //------------------------------

  SAT_Observer SAT_GLOBAL_OBSERVER = {};
  
  //----------

  void SAT_PrintObservers() {
    SAT_GLOBAL_OBSERVER.print_observers();
  }
  
//------------------------------

#else // !SAT_DEBUG_OBSERVER

  void SAT_PrintObservers() {
  }

#endif // SAT_DEBUG_OBSERVER

//----------------------------------------------------------------------
#endif
