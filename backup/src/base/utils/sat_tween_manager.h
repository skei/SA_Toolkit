#ifndef sat_tweening_included
#define sat_tweening_included
//----------------------------------------------------------------------

/*
  todo;
    chain of tweens instead of single one
    including pause
    
    startTween(), endTween()
    
*/

#include "base/utils/sat_easing.h"
#include "gui/sat_widget.h"

//----------------------------------------------------------------------

#define SAT_TWEEN_MAX_VALUES  4

//#define SAT_TWEEN_CALLBACK  1
//#define SAT_TWEEN_PAUSE     2
//#define SAT_TWEEN_RECT      3

//----------------------------------------------------------------------
//
//
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

  SAT_TweenNode(void* ATarget, uint32_t AId, /*double AStart, */double ADuration, uint32_t AType, uint32_t ACount=0, double* AStartValues=nullptr, double* AEndValues=nullptr, uint32_t ATween=0) {
    //MActive       = false;
    MType         = AType;
    MId           = AId;
    //MStartTime    = AStart;
    MDuration     = ADuration;
    MTarget       = ATarget;
    MTweenType    = ATween;
    MNumValues    = ACount;
    for (uint32_t i=0; i<ACount; i++) {
      MStartValues[i] = AStartValues[i];
      MDeltaValues[i] = AEndValues[i] - AStartValues[i];
    }
  }
  
  //----------
  
  ~SAT_TweenNode() {
  }
  
};

//------------------------------
//
//------------------------------

typedef SAT_Array<SAT_TweenNode*> SAT_TweenNodes;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_TweenChain {
  
//------------------------------
public:
//------------------------------

  bool            MActive       = true;
  uint32_t        MCurrentNode  = 0;
  double          MCurrentTime  = 0.0;
  SAT_TweenNodes  MNodes        = {};
  
//------------------------------
public:
//------------------------------

  SAT_TweenChain() {
  }
  
  //----------
  
  ~SAT_TweenChain() {
  }
  
  //----------
  
  SAT_TweenNode* appendNode(SAT_TweenNode* ANode) {
    MNodes.append(ANode);
    return ANode;
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

//------------------------------
//
//------------------------------

typedef SAT_Array<SAT_TweenChain*> SAT_TweenChains;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_TweenManager {
  
//------------------------------
private:
//------------------------------

  SAT_TweenChains   MTweens;

//------------------------------
public:
//------------------------------

  //void appendTween(SAT_TweenNode* ATween) {
  //}
  
  //----------
  
  SAT_TweenChain* appendChain(SAT_TweenChain* ATweens) {
    MTweens.append(ATweens);
    return ATweens;
  }
  
  //----------
  
  virtual void process(double ADelta, double AScale) {
    uint32_t num_tweens = MTweens.size();
    for (uint32_t i=0; i<num_tweens; i++) {
      if (MTweens[i]->MActive) {
        uint32_t index = MTweens[i]->MCurrentNode;
        SAT_TweenNode* node = MTweens[i]->MNodes[index];
        if (node) {// && node->MActive) {
          SAT_Widget* target = (SAT_Widget*)node->MTarget;
          double data[16] = {0};
          for (uint32_t j=0; j<node->MNumValues; j++) {
            data[j] = SAT_Easing(
              node->MTweenType,
              MTweens[i]->MCurrentTime,
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
          
          MTweens[i]->MCurrentTime += ADelta;
          if (MTweens[i]->MCurrentTime >= node->MDuration) {
            
            MTweens[i]->MCurrentTime = 0.0;
            MTweens[i]->MCurrentNode += 1;
            if (MTweens[i]->MCurrentNode >= MTweens[i]->MNodes.size()) {
              
              SAT_Print("finished %i\n",i);
              MTweens[i]->MActive = false;
              
//              //TODO: if looping
//              MTweens[i]->MCurrentNode = 0;
//              MTweens[i]->MCurrentTime = 0.0;
              
            }
            
          } // duration
        } // node.active
      } // tween.active
    } // for
    
    uint32_t i = 0;
    while (i < MTweens.size()) {
      if (!MTweens[i]->MActive) {
        
        for (uint32_t j=0; j<MTweens[i]->MNodes.size(); j++) {
          SAT_Print("deleting node %i\n",j);
          delete MTweens[i]->MNodes[j];
        }
        
        SAT_Print("deleting chain %i\n",i);
        delete MTweens[i];
        MTweens.remove(i);
      }
      i += 1;
    }
    
  }
  
//------------------------------
private:
//------------------------------
  
};

//----------------------------------------------------------------------

#endif