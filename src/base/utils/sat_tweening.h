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
//
//
//
//----------------------------------------------------------------------

#define SAT_MAX_TWEENS      32

//#define SAT_TWEEN_CALLBACK  1
//#define SAT_TWEEN_PAUSE     2
//#define SAT_TWEEN_RECT      3

struct SAT_TweenNode {
  bool        active;
  uint32_t    type;           // tween, pause, signal ..
//  uint32_t    id;             // id
  uint32_t    count;          // number of coords/vriables
  uint32_t    tween;          // tween type (easing)
  void*       target;         // target widget
  double      current_time;   // 0..duration
  double      duration;       // in seconds
  double      start[4];       // start values
  double      delta[4];       // values change to react target in 'duration' time
};

struct SAT_TweenList {
  union {
    SAT_TweenNode tween;
    double        duration;
    uint32_t      signal;
  };
};

typedef SAT_Array<SAT_TweenList> SAT_TweenArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Tweening {
  
//------------------------------
private:
//------------------------------

  //SAT_TweenNodeArray  MTweens = {};
  SAT_TweenNode   MTweenNodes[SAT_MAX_TWEENS] = {0};
  SAT_TweenArray  MTweenArray;

//------------------------------
public:
//------------------------------

  virtual void appendTween(uint32_t AType, void* ATarget, uint32_t ATween, uint32_t ACount, double* AStart, double* AEnd, double ADuration) {
    int32_t i = findFreeTween();
    //SAT_Print("free tween: %i\n",i);
    if (i >= 0) {
      MTweenNodes[i].active        = true;
//      MTweenNodes[i].type          = SAT_TWEEN_RECT;
      MTweenNodes[i].type            = AType;
      //MTweenNodes[i].type          = AType;
      MTweenNodes[i].count         = ACount;
      MTweenNodes[i].tween         = ATween;
      MTweenNodes[i].target        = ATarget;
      MTweenNodes[i].current_time  = 0.0;
      MTweenNodes[i].duration      = ADuration;
      for (uint32_t j=0; j<ACount; j++) {
        MTweenNodes[i].start[j]    = AStart[j];
        MTweenNodes[i].delta[j]    = (AEnd[j] - AStart[j]);
      }
    }
  };

  //----------

  virtual int32_t findFreeTween() {
    for (uint32_t i=0; i<SAT_MAX_TWEENS; i++) {
      if (!MTweenNodes[i].active) return i;
    }
    return -1;
  }
  
  //----------

  virtual void processAllNodes(double ADelta, double AScale) {
    for (uint32_t i=0; i<SAT_MAX_TWEENS; i++) {
      if (MTweenNodes[i].active) {
        SAT_Widget* widget = (SAT_Widget*)MTweenNodes[i].target;
        double t = MTweenNodes[i].current_time;
        double d = MTweenNodes[i].duration;
        double data[16] = {0};
        for (uint32_t j=0; j<MTweenNodes[i].count; j++) {
          data[j] = SAT_Easing(MTweenNodes[i].tween,t,MTweenNodes[i].start[j],MTweenNodes[i].delta[j],d);
        }
        widget->on_widget_tween(MTweenNodes[i].type,MTweenNodes[i].count,data);
        
        MTweenNodes[i].current_time += ADelta;
        if (MTweenNodes[i].current_time >= MTweenNodes[i].duration) {
          MTweenNodes[i].active = false;
        }
        
      }
    }
  };
  
  //----------

  virtual void processTweenList(double ADelta, double AScale) {
  }

  //----------


//  virtual void appendTween(void* ATarget, uint32_t AType, uint32_t ATween, double AStartValue, double AEndValue, double ADuration) {
//    int32_t i = findFreeTween();
//    //SAT_Print("free tween: %i\n",i);
//    if (i >= 0) {
//      MTweenNodes[i].active        = true;
//      MTweenNodes[i].type          = AType;
//      MTweenNodes[i].tween         = ATween;
//      MTweenNodes[i].target        = ATarget;
//      MTweenNodes[i].current_time  = 0.0;
//      MTweenNodes[i].duration      = ADuration;
//      MTweenNodes[i].start[0]      = AStartValue;
//      MTweenNodes[i].delta[0]      = (AEndValue - AStartValue);// / ADuration;
//    }
//  };
//  
//  //----------
//
//  virtual void appendTween(void* ATarget, uint32_t AType, uint32_t ATween, SAT_Rect AStartRect, SAT_Rect AEndRect, double ADuration) {
//    int32_t i = findFreeTween();
//    //SAT_Print("free tween: %i\n",i);
//    if (i >= 0) {
//      MTweenNodes[i].active        = true;
//      MTweenNodes[i].type          = AType;
//      MTweenNodes[i].tween         = ATween;
//      MTweenNodes[i].target        = ATarget;
//      MTweenNodes[i].current_time  = 0.0;
//      MTweenNodes[i].duration      = ADuration;
//      MTweenNodes[i].start[0]    = AStartRect.x;
//      MTweenNodes[i].start[1]    = AStartRect.y;
//      MTweenNodes[i].start[2]    = AStartRect.w;
//      MTweenNodes[i].start[3]    = AStartRect.h;
//      MTweenNodes[i].delta[0]    = AEndRect.x - AStartRect.x;
//      MTweenNodes[i].delta[1]    = AEndRect.y - AStartRect.y;
//      MTweenNodes[i].delta[2]    = AEndRect.w - AStartRect.w;
//      MTweenNodes[i].delta[3]    = AEndRect.h - AStartRect.h;
//    }
//  };
//  
//  //----------
//
//  virtual void appendTween(void* ATarget, uint32_t AType, uint32_t ATween, SAT_Color AStartColor, SAT_Color AEndColor, double ADuration) {
//    int32_t i = findFreeTween();
//    //SAT_Print("free tween: %i\n",i);
//    if (i >= 0) {
//      MTweenNodes[i].active        = true;
//      MTweenNodes[i].type          = AType;
//      MTweenNodes[i].tween         = ATween;
//      MTweenNodes[i].target        = ATarget;
//      MTweenNodes[i].current_time  = 0.0;
//      MTweenNodes[i].duration      = ADuration;
//      //MTweenNodes[i].start_color   = AStartColor;
//      //MTweenNodes[i].delta_color   = AEndColor;
//      //MTweenNodes[i].delta_color.sub(AStartColor);
//      MTweenNodes[i].start[0]    = AStartColor.r;
//      MTweenNodes[i].start[1]    = AStartColor.g;
//      MTweenNodes[i].start[2]    = AStartColor.b;
//      MTweenNodes[i].start[3]    = AStartColor.a;
//      MTweenNodes[i].delta[0]    = AEndColor.r - AStartColor.r;
//      MTweenNodes[i].delta[1]    = AEndColor.g - AStartColor.g;
//      MTweenNodes[i].delta[2]    = AEndColor.b - AStartColor.b;
//      MTweenNodes[i].delta[3]    = AEndColor.a - AStartColor.a;
//    }
//  };
  
//------------------------------
private:
//------------------------------
  
};

#undef MAX_TWEENS

//----------------------------------------------------------------------

#endif