#ifndef sat_widget2_included
#define sat_widget2_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_painter.h"

//----------------------------------------------------------------------

#define SAT_WIDGET_UPDATE_VALUE       0

#define SAT_WIDGET_REDRAW_ALL         1
#define SAT_WIDGET_REDRAW_VALUE       2
#define SAT_WIDGET_REDRAW_MODULATION  3

#define SAT_WIDGET_REALIGN_ALL        0

//----------

struct SAT_WidgetOwner2 {
  // window
};

struct SAT_PaintContext2 {
  SAT_Painter*  painter;
  double        scale;
  //SAT_Window*   owner_window;
};

class SAT_Timer2;

class SAT_Widget2;
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

  // hierarchy

  SAT_WidgetOwner2*   MOwner                          = nullptr;
  SAT_Widget2*        MParent                         = nullptr;
  SAT_WidgetArray2    MChildren                       = {};
  uint32_t            MIndex                          = 0;

  // value

  uint32_t            MNumValues                      = 1;
  double              MValues[SAT_WIDGET_MAX_VALUES]  = {0};

  // pos

  SAT_Rect            MCreateRect                     = {0,0,0,0};
  SAT_Rect            MBaseRect                       = {0,0,0,0};
  SAT_Rect            MRect                           = {0,0,0,0};
  SAT_Point           MLayoutOffset                   = {0,0};
  SAT_Rect            MClipOffset                     = {0,0,0,0};
//SAT_Point           MSnapDistance                   = {0,0};

  // layout

  uint32_t            MLayoutFlags                    = SAT_WIDGET_LAYOUT_DEFAULT;
  SAT_Rect            MInnerBorder                    = {0,0,0,0};
  SAT_Rect            MOuterBorder                    = {0,0,0,0};
  SAT_Point           MWidgetSpacing                  = {0,0};
  SAT_Rect            MSizeLimit                      = {-1,-1,-1,-1};
  bool                MVertical                       = false;
  bool                MOpaque                         = false;

  // state

  bool                MIsActive                       = true;
  bool                MIsVisible                      = true;
  bool                MIsDisabled                     = false;
  bool                MIsInteracting                  = false;

  // options

  bool                MAutoClip                       = true;
  bool                MAutoHoverCursor                = true;
  bool                MAutoHoverHint                  = true;
  bool                MRealignIfNotVisible            = false;

//------------------------------
public:
//------------------------------

  SAT_Widget2() {
  }

  //----------

  virtual ~SAT_Widget2() {
    #ifndef SAT_NO_AUTODELETE
      deleteChildWidgets();
    #endif
  }

//------------------------------
public: // set
//------------------------------

  void setIndex(uint32_t AIndex) { MIndex = AIndex; }

//------------------------------
public: // get
//------------------------------

  bool isActive()             { return MIsActive; }
  bool isVisible()            { return MIsVisible; }
  bool isDisabled()           { return MIsDisabled; }
  bool isInteracting()        { return MIsInteracting; }

  bool autoClip()             { return MAutoClip; }
  bool autoHoverCursor()      { return MAutoHoverCursor; }
  bool autoHoverHint()        { return MAutoHoverHint; }
  bool realignIfNotVisible()  { return MRealignIfNotVisible; }

  bool isVertical()           { return MVertical; }
  bool isOpaque()             { return MOpaque; }

//------------------------------
public: // hierarchy
//------------------------------

  virtual void appendChildWidget(SAT_Widget2* AWidget) {
    uint32_t index = MChildren.size();
    MChildren.append(AWidget);
    AWidget->setIndex(index);
  }
  
  //----------

  virtual void removeChildWidget(SAT_Widget2* AWidget) {
    MChildren.remove(AWidget);
  }

  //----------
  
  virtual void deleteChildWidgets() {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      if (MChildren[i]) {
        //MChildren[i]->on_widget_delete
        delete MChildren[i];
        MChildren[i] = nullptr;
      }
    }
  }

  //----------
  
  virtual SAT_Widget2* findOpaqueParent() {
    if (MParent) {
      if (MParent->isOpaque()) return MParent;
      else return MParent->findOpaqueParent();
    }
    return nullptr;
  }

  //----------
  
  // virtual void scrollChildWidgets(double AXdelta, double AYdelta) {
  // }

//------------------------------
public: // value
//------------------------------

  // virtual int32_t findClosestValue(double AXpos, double AYpos) {
  //   return 0;
  // }

  //----------
  
  // virtual double calcPosFromValue(uint32_t AIndex=0) {
  //   return 0.0;
  // }

//------------------------------
public:
//------------------------------

  virtual void updateWidget(uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) {
    do_widget_update(this, AIndex, AMode);
  }

  //----------
  
  virtual void redrawWidget(uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_ALL) {
    do_widget_redraw(this, AIndex, AMode);
  }

  //----------
  
  virtual void realignWidget(uint32_t AMode=SAT_WIDGET_REALIGN_ALL) {
    do_widget_realign(this, AMode);
  }

  //----------
  
  virtual void redrawParent(uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_ALL) {
    if (MParent) MParent->do_widget_redraw(MParent,AIndex,AMode);
  }
  
  //----------
  
  virtual void realignParent(uint32_t AMode=SAT_WIDGET_REALIGN_ALL) {
    if (MParent) MParent->do_widget_realign(MParent, AMode);
  }

  //----------
  
  virtual void paintChildren(SAT_PaintContext2* AContext) {
    // culling
    // clipping
    // children
  }

  //----------
  
  virtual void realignChildren() {
    // reset scroll, movement
    // realign
    // set scroll, movement
  }

//------------------------------
public: // on_ (downwards)
//------------------------------

  // called when owned window opens, before initial realigning & drawing
  // (on_window_open)

  virtual void on_widget_open(SAT_WidgetOwner2* AOwner) {
    MOwner = AOwner;
  }

  // called when owned window closes
  // (on_window_close)

  virtual void on_widget_close(SAT_WidgetOwner2* AOwner) {
  }

  // called just before widget is aligned (moved/sized)

  virtual void on_widget_prealign() {
  }

  // called after widget has been aligned (just before realigning children)

  virtual void on_widget_postalign() {
  }

  // called when (part of) the widget needs to be painted

  virtual void on_widget_paint(SAT_PaintContext2* AContext) {
    paintChildren(AContext);
  }

  // called when (window) timer ticks

  virtual void on_widget_timer(SAT_Timer2* ATimer, double AElapsed) {
  }

  // ..todo..

  virtual void on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ANum, double* AData) {
  }

  virtual void on_widget_mouseClick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) {
  }

  virtual void on_widget_mouseDblClick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) {
  }

  virtual void on_widget_mouseRelease(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) {
  }

  virtual void on_widget_mouseMove(double AXpos, double AYpos, uint32_t AState, uint32_t ATimeStamp) {
  }

  virtual void on_widget_keyPress(char AChar, uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) {
  }

  virtual void on_widget_keyRelease(char AChar, uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) {
  }

  // called when mouse cursor enters widget

  virtual void on_widget_enter(SAT_Widget2* AFrom, double AXpos, double AYpos/*, uint32_t ATimeStamp*/) {
    if (autoHoverHint()) { do_widget_setHint(this,MHint); }
    if (autoHoverCursor()) { do_widget_setCursor(this,MCursor); }
    // update hover
  }

  // called when mouse cursor leaves widget

  virtual void on_widget_leave(SAT_Widget2* ATo, double AXpos, double AYpos/*, uint32_t ATimeStamp*/) {
    if (autoHoverHint()) { do_widget_setHint(this,""); }
    if (autoHoverCursor()) { do_widget_setCursor(this,SAT_CURSOR_DEFAULT); }
  }

//------------------------------
public: // do_ (upwards)
//------------------------------

  virtual void do_widget_update(SAT_Widget2* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) {
    if (MParent) MParent->do_widget_update(AWidget,AIndex,AMode);
  }

  virtual void do_widget_redraw(SAT_Widget2* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_ALL) {
    if (MParent) MParent->do_widget_redraw(AWidget,AIndex,AMode);
  }

  virtual void do_widget_realign(SAT_Widget2* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_ALL) {
    if (MParent) MParent->do_widget_realign(AWidget,AMode);
  }

  virtual void do_widget_setCursor(SAT_Widget2* AWidget, int32_t ACursor) {
    if (MParent) MParent->do_widget_setCursor(AWidget,ACursor);
  }

  virtual void do_widget_setHint(SAT_Widget2* AWidget, const char* AHint) {
    if (MParent) MParent->do_widget_setHint(AWidget,AHint);
  }

  virtual void do_widget_setModal(SAT_Widget2* AWidget) {
    if (MParent) MParent->do_widget_setModal(AWidget);
  }

  virtual void do_widget_captureKeys(SAT_Widget2* AWidget) {
    if (MParent) MParent->do_widget_captureKeys(AWidget);
  }

  virtual void do_widget_close(SAT_Widget2* AWidget) {
    if (MParent) MParent->do_widget_close(AWidget);
  }

  virtual void do_widget_select(SAT_Widget2* AWidget, int32_t AIndex, int32_t ASubIndex=-1) {
    if (MParent) MParent->do_widget_select(AWidget,AIndex,ASubIndex);
  }

  virtual void do_widget_resized(SAT_Widget2* ASender, double ADeltaX, double ADeltaY) {
    if (MParent) MParent->do_widget_resized(ASender,ADeltaX,ADeltaY);
  }

};


//----------------------------------------------------------------------
#endif