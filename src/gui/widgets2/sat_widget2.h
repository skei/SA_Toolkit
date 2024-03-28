#ifndef sat_widget2_included
#define sat_widget2_included
//----------------------------------------------------------------------

#include "sat.h"

//----------------------------------------------------------------------

#define SAT_WIDGET_UPDATE_VALUE       0

#define SAT_WIDGET_REDRAW_ALL         0
#define SAT_WIDGET_REDRAW_VALUE       1

#define SAT_WIDGET_REALIGN_ALL        0

//----------

class SAT_Widget2;
class SAT_WidgetOwner2;
class SAT_PaintContext2;
class SAT_Timer2;

typedef SAT_Array<SAT_Widget2*> SAT_WidgetArray2;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Widget2 {

//------------------------------
private:
//------------------------------

  const char*         MName                           = "SAT_Widget";
  const char*         MHint                           = "";
  int32_t             MCursor                         = SAT_CURSOR_DEFAULT;

  // value

  uint32_t            MNumValues                      = 1;
  double              MValues[SAT_WIDGET_MAX_VALUES]  = {0};

  // pos

  SAT_Rect            MRect                           = {0,0,0,0};
  SAT_Rect            MBaseRect                       = {0,0,0,0};
  SAT_Rect            MCreateRect                     = {0,0,0,0};

  SAT_Rect            MClipOffset                     = {0,0,0,0};
  SAT_Point           MLayoutOffset                   = {0,0};
  SAT_Point           MSnapDistance                   = {0,0};

  // layout

  uint32_t            MLayoutFlags                    = SAT_WIDGET_LAYOUT_DEFAULT;
  SAT_Rect            MLayoutInnerBorder              = {0,0,0,0};
  SAT_Rect            MLayoutOuterBorder              = {0,0,0,0};
  SAT_Point           MLayoutWidgetSpacing            = {0,0};
  SAT_Rect            MLayoutSizeLimit                = {-1,-1,-1,-1};
  bool                MLayoutVertical                 = false;
  bool                MLayoutOaque                    = false;

  // state

  bool                MIsActive                       = true;
  bool                MIsVisible                      = true;
  bool                MIsInteracting                  = false;
  bool                MIsEnabled                      = true;

  // options

  bool                MAutoClip                       = true;
  bool                MAutoHoverCursor                = true;
  bool                MAutoHoverHint                  = true;
  bool                MRealignIfNotVisible            = false;

  // hierarchy

  SAT_Widget2*        MParent                         = nullptr;
  SAT_WidgetArray2    MChildren                       = {};

//------------------------------
public:
//------------------------------

  SAT_Widget2() {
  }

  //----------

  virtual ~SAT_Widget2() {
  }

//------------------------------
public: // get,set
//------------------------------

//------------------------------
public: // hierarchy
//------------------------------

  virtual void appendChildWidget(SAT_Widget2* AWidget) {}
  virtual void removeChildWidget(SAT_Widget2* AWidget) {}
  virtual void deleteChildWidgets() {}

  virtual void scrollChildWidgets(double AXdelta, double AYdelta) {}
  virtual SAT_Widget2* findOpaqueParent() { return nullptr; }

//------------------------------
public: // value
//------------------------------

  virtual int32_t findClosestValue(double AXpos, double AYpos) { return 0; }
  virtual double calcPosFromValue(uint32_t AIndex=0) { return 0.0; }

//------------------------------
public:
//------------------------------

  virtual void updateWidget(uint32_t AMode) {}
  virtual void redrawWidget(uint32_t AMode) {}
  virtual void redrawParent(uint32_t AMode) {}
  virtual void realignWidget(uint32_t AMode) {}
  virtual void realignParent(uint32_t AMode) {}

//------------------------------
public: // on_ (downwards)
//------------------------------

  virtual void on_widget_open(SAT_WidgetOwner2* AOwner) {}
  virtual void on_widget_close(SAT_WidgetOwner2* AOwner) {}
  virtual void on_widget_prealign() {}
  virtual void on_widget_postalign() {}
  virtual void on_widget_paint(SAT_PaintContext2* AContext) {}
  virtual void on_widget_timer(SAT_Timer2* ATimer, double AElapsed) {}
  virtual void on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ANum, double* AData) {}
  virtual void on_widget_mouseClick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_widget_mouseDblClick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_widget_mouseRelease(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_widget_mouseMove(double AXpos, double AYpos, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_widget_keyPress(char AChar, uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_widget_keyRelease(char AChar, uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_widget_enter(SAT_Widget2* AFrom, double AXpos, double AYpos) {}
  virtual void on_widget_leave(SAT_Widget2* ATo, double AXpos, double AYpos) {}

//------------------------------
public: // do_ (upwards)
//------------------------------

  virtual void do_widget_update(SAT_Widget2* AWidget, uint32_t AIndex, uint32_t AMode) {}
  virtual void do_widget_redraw(SAT_Widget2* AWidget, uint32_t AIndex, uint32_t AMode) {}
  virtual void do_widget_realign(SAT_Widget2* AWidget, uint32_t AMode) {}
  virtual void do_widget_setCursor(SAT_Widget2* AWidget, int32_t ACursor) {}
  virtual void do_widget_setHint(SAT_Widget2* AWidget, const char* AHint) {}
  virtual void do_widget_setModal(SAT_Widget2* AWidget) {}
  virtual void do_widget_captureKeys(SAT_Widget2* AWidget) {}
  virtual void do_widget_close(SAT_Widget2* AWidget) {}
  virtual void do_widget_select(SAT_Widget2* AWidget, int32_t AIndex, int32_t ASubIndex=-1) {}
  virtual void do_widget_resized(SAT_Widget2* ASender, double ADeltaX, double ADeltaY) {}

};


//----------------------------------------------------------------------
#endif