#ifndef sat_tween_manager_included
#define sat_tween_manager_included
//----------------------------------------------------------------------

#include "base/utils/sat_easing.h"
#include "gui/sat_widget.h"

//----------------------------------------------------------------------

#define SAT_TWEEN_MAX_VALUES        4
#define SAT_TWEEN_CHAIN_QUEUE_SIZE  32

//#define SAT_TWEEN_CALLBACK  1
//#define SAT_TWEEN_PAUSE     2
//#define SAT_TWEEN_RECT      3

// delete, end, repeat, ..

//----------

class SAT_TweenNode;
class SAT_TweenChain;

typedef SAT_Array<SAT_TweenNode*> SAT_TweenNodeArray;
typedef SAT_Array<SAT_TweenChain*> SAT_TweenChainArray;
typedef SAT_Queue<SAT_TweenChain*,SAT_TWEEN_CHAIN_QUEUE_SIZE> SAT_TweenChainQueue;

//----------------------------------------------------------------------
//
// tween node
//
//----------------------------------------------------------------------

class SAT_TweenNode {
  
//------------------------------
public:
//------------------------------

  uint32_t  MType                               = 0;        // tween, pause, signal ..
  uint32_t  MId                                 = 0;        // id
  double    MDuration                           = 0.0;      // in seconds
  void*     MTarget                             = nullptr;  // target widget
  uint32_t  MTweenType                          = 0;        // tween type (easing)
  uint32_t  MNumValues                          = 0;        // number of coords/vriables
  double    MStartValues[SAT_TWEEN_MAX_VALUES]  = {0};      // start values
  double    MDeltaValues[SAT_TWEEN_MAX_VALUES]  = {0};      // values change to react target in 'duration' time
  
//------------------------------
public:
//------------------------------

  SAT_TweenNode(void* ATarget, uint32_t AId, /*double AStart, */double ADuration, uint32_t AType, uint32_t ANumValues=0, double* AStartValues=nullptr, double* AEndValues=nullptr, uint32_t ATween=0) {
    //MActive       = false;
    MType         = AType;
    MId           = AId;
    //MStartTime    = AStart;
    MDuration     = ADuration;
    MTarget       = ATarget;
    MTweenType    = ATween;
    MNumValues    = ANumValues;
    for (uint32_t i=0; i<ANumValues; i++) {
      MStartValues[i] = AStartValues[i];
      MDeltaValues[i] = AEndValues[i] - AStartValues[i];
    }
  }
  
  //----------
  
  ~SAT_TweenNode() {
  }
  
};

//----------------------------------------------------------------------
//
// tween chain
//
//----------------------------------------------------------------------

class SAT_TweenChain {
  
//------------------------------
public:
//------------------------------

  bool                MActive       = true;
  uint32_t            MCurrentNode  = 0;
  double              MCurrentTime  = 0.0;
  SAT_TweenNodeArray  MNodes        = {};
  
//------------------------------
public:
//------------------------------

  SAT_TweenChain() {
  }
  
  //----------
  
  ~SAT_TweenChain() {
    #if !defined (SAT_NO_AUTODELETE)
      deleteNodes();
    #endif
  }
  
  //----------
  
  SAT_TweenNode* appendNode(SAT_TweenNode* ANode) {
    MNodes.append(ANode);
    return ANode;
  }

  //----------

  void deleteNodes() {
    for (uint32_t i=0; i<MNodes.size(); i++) delete MNodes[i];
  }
  
  //----------
  
  void start(void) {
    MActive = true;
    MCurrentNode = 0;
    MCurrentTime = 0.0;
  }
  
  //----------
  
  void stop(void) {
    MActive = false;
  }
  
  //----------
  
  bool isActive() {
    return MActive;
  }

};

//----------------------------------------------------------------------
//
// tween manager
//
//----------------------------------------------------------------------

class SAT_TweenManager {
  
//------------------------------
private:
//------------------------------

  SAT_TweenChainArray MChains   = {};
  SAT_TweenChainQueue MPending  = {};

//------------------------------
public:
//------------------------------

  SAT_TweenManager() {
  }

  //----------

  ~SAT_TweenManager() {
    #if !defined (SAT_NO_AUTODELETE)
      deleteChains();
    #endif
  }

//------------------------------
public:
//------------------------------

  //void appendTween(SAT_TweenNode* ATween) {
  //}
  
  //----------
  
  SAT_TweenChain* appendChain(SAT_TweenChain* AChain) {
    //SAT_PRINT;
    //MChains.append(AChain);
    MPending.write(AChain);
    return AChain;
  }
  
  //----------

  void deleteChains() {
    for (uint32_t i=0; i<MChains.size(); i++) delete MChains[i];
    SAT_TweenChain* chain;
    while (MPending.read(&chain)) { delete chain; }
  }
  
  //----------

  virtual void process(double ADelta/*, double AScale*/) {

    SAT_TweenChain* chain;
    while (MPending.read(&chain)) {
      MChains.append(chain);
    }

    uint32_t num_chains = MChains.size();
    for (uint32_t i=0; i<num_chains; i++) {
      if (MChains[i]->MActive) {

        uint32_t index = MChains[i]->MCurrentNode;
        SAT_TweenNode* node = MChains[i]->MNodes[index];
        if (node) {// && node->MActive) {
          SAT_Widget* target = (SAT_Widget*)node->MTarget;
          double data[16] = {0};
          for (uint32_t j=0; j<node->MNumValues; j++) {
            data[j] = SAT_Easing(
              node->MTweenType,
              MChains[i]->MCurrentTime,
              node->MStartValues[j],
              node->MDeltaValues[j],
              node->MDuration
            );
          }
          //switch (node->MType) {
          //  case SAT_WIDGET_TWEEN_PAUSE:
          //    break;
          //  default:

              target->on_widget_tween(node->MId,node->MType,node->MNumValues,data);

          //    break;
          //}
          MChains[i]->MCurrentTime += ADelta;
          if (MChains[i]->MCurrentTime >= node->MDuration) {
            MChains[i]->MCurrentTime = 0.0;
            MChains[i]->MCurrentNode += 1;
            if (MChains[i]->MCurrentNode >= MChains[i]->MNodes.size()) {
              //SAT_Print("finished %i\n",i);
              MChains[i]->MActive = false;
              // //TODO: if looping
              // MChains[i]->MCurrentNode = 0;
              // MChains[i]->MCurrentTime = 0.0;
            }
          } // duration
        } // node.active
      } // tween.active
    } // for

    // delete finished chains
    uint32_t i = 0;
    while (i < MChains.size()) {
      SAT_TweenChain* chain = MChains[i];
      if (!chain->MActive) {
        //for (uint32_t j=0; j<chain->MNodes.size(); j++) {
        //  SAT_TweenNode* node = chain->MNodes[j];
        //  SAT_Print("deleting node %i\n",j);
        //  delete node;
        //}
        //SAT_Print("deleting chain %i\n",i);
        delete chain;
        MChains.remove(i);
      }
      i += 1;
    }
    
  }
  
  //----------

};

//----------------------------------------------------------------------

#endif
