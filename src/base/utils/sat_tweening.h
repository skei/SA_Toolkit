#ifndef sat_tweening_included
#define sat_tweening_included
//----------------------------------------------------------------------

// todo; chain of tweens instead of single one
// including pause

#include "base/utils/sat_easing.h"

#define MAX_TWEEN_NODES 32

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct SAT_TweenNode {
  bool        active;
  uint32_t    id;
  //uint32_t    type; // remove
  uint32_t    count;
  uint32_t    tween;
  void*       target;
  double      current_time;
  double      duration;
  double      start[4];
  double      delta[4];
};

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
  SAT_TweenNode MTweens[MAX_TWEEN_NODES] = {0};

//------------------------------
public:
//------------------------------

  virtual void appendTween(uint32_t AId, void* ATarget, uint32_t ATween, uint32_t ACount, double* AStart, double* AEnd, double ADuration) {
    int32_t i = findFreeTween();
    //SAT_Print("free tween: %i\n",i);
    if (i >= 0) {
      MTweens[i].active        = true;
      MTweens[i].id            = AId;
      //MTweens[i].type          = AType;
      MTweens[i].count         = ACount;
      MTweens[i].tween         = ATween;
      MTweens[i].target        = ATarget;
      MTweens[i].current_time  = 0.0;
      MTweens[i].duration      = ADuration;
      for (uint32_t j=0; j<ACount; j++) {
        MTweens[i].start[j]    = AStart[j];
        MTweens[i].delta[j]    = (AEnd[j] - AStart[j]);
      }
    }
  };

  //----------

  virtual int32_t findFreeTween() {
    for (uint32_t i=0; i<MAX_TWEEN_NODES; i++) {
      if (!MTweens[i].active) return i;
    }
    return -1;
  }
  
  //----------

  virtual void process(double ADelta, double AScale) {
    for (uint32_t i=0; i<MAX_TWEEN_NODES; i++) {
      if (MTweens[i].active) {
        SAT_Widget* widget = (SAT_Widget*)MTweens[i].target;
        double t = MTweens[i].current_time;
        double d = MTweens[i].duration;
        double data[16] = {0};
        for (uint32_t j=0; j<MTweens[i].count; j++) {
          data[j] = SAT_Easing(MTweens[i].tween,t,MTweens[i].start[j],MTweens[i].delta[j],d);
        }
        widget->on_widget_tween(MTweens[i].id,MTweens[i].count,data);
        
        MTweens[i].current_time += ADelta;
        if (MTweens[i].current_time >= MTweens[i].duration) {
          MTweens[i].active = false;
        }
        
      }
    }
  };
  
  //----------










//  virtual void appendTween(void* ATarget, uint32_t AType, uint32_t ATween, double AStartValue, double AEndValue, double ADuration) {
//    int32_t i = findFreeTween();
//    //SAT_Print("free tween: %i\n",i);
//    if (i >= 0) {
//      MTweens[i].active        = true;
//      MTweens[i].type          = AType;
//      MTweens[i].tween         = ATween;
//      MTweens[i].target        = ATarget;
//      MTweens[i].current_time  = 0.0;
//      MTweens[i].duration      = ADuration;
//      MTweens[i].start[0]      = AStartValue;
//      MTweens[i].delta[0]      = (AEndValue - AStartValue);// / ADuration;
//    }
//  };
//  
//  //----------
//
//  virtual void appendTween(void* ATarget, uint32_t AType, uint32_t ATween, SAT_Rect AStartRect, SAT_Rect AEndRect, double ADuration) {
//    int32_t i = findFreeTween();
//    //SAT_Print("free tween: %i\n",i);
//    if (i >= 0) {
//      MTweens[i].active        = true;
//      MTweens[i].type          = AType;
//      MTweens[i].tween         = ATween;
//      MTweens[i].target        = ATarget;
//      MTweens[i].current_time  = 0.0;
//      MTweens[i].duration      = ADuration;
//      MTweens[i].start[0]    = AStartRect.x;
//      MTweens[i].start[1]    = AStartRect.y;
//      MTweens[i].start[2]    = AStartRect.w;
//      MTweens[i].start[3]    = AStartRect.h;
//      MTweens[i].delta[0]    = AEndRect.x - AStartRect.x;
//      MTweens[i].delta[1]    = AEndRect.y - AStartRect.y;
//      MTweens[i].delta[2]    = AEndRect.w - AStartRect.w;
//      MTweens[i].delta[3]    = AEndRect.h - AStartRect.h;
//    }
//  };
//  
//  //----------
//
//  virtual void appendTween(void* ATarget, uint32_t AType, uint32_t ATween, SAT_Color AStartColor, SAT_Color AEndColor, double ADuration) {
//    int32_t i = findFreeTween();
//    //SAT_Print("free tween: %i\n",i);
//    if (i >= 0) {
//      MTweens[i].active        = true;
//      MTweens[i].type          = AType;
//      MTweens[i].tween         = ATween;
//      MTweens[i].target        = ATarget;
//      MTweens[i].current_time  = 0.0;
//      MTweens[i].duration      = ADuration;
//      //MTweens[i].start_color   = AStartColor;
//      //MTweens[i].delta_color   = AEndColor;
//      //MTweens[i].delta_color.sub(AStartColor);
//      MTweens[i].start[0]    = AStartColor.r;
//      MTweens[i].start[1]    = AStartColor.g;
//      MTweens[i].start[2]    = AStartColor.b;
//      MTweens[i].start[3]    = AStartColor.a;
//      MTweens[i].delta[0]    = AEndColor.r - AStartColor.r;
//      MTweens[i].delta[1]    = AEndColor.g - AStartColor.g;
//      MTweens[i].delta[2]    = AEndColor.b - AStartColor.b;
//      MTweens[i].delta[3]    = AEndColor.a - AStartColor.a;
//    }
//  };
  
//------------------------------
private:
//------------------------------
  
};

#undef MAX_NODES

//----------------------------------------------------------------------

#endif