#ifndef sat_widget_included
#define sat_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_painter.h"
#include "gui/sat_paint_context.h"
#include "gui/sat_widget_listener.h"
#include "gui/sat_widget_owner.h"

#define SAT_WIDGET_DIRTY_QUEUE_SIZE 1024

//----------------------------------------------------------------------

//class SAT_TimerListener {
//public:
//  virtual void do_timerListener_update() {}
//};

//class SAT_Painter {
//public:
//  virtual void setClipRect(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
//  virtual void resetClip() {}
//  virtual void pushClip(SAT_Rect ARect) {}
//  virtual void pushOverlappingClip(SAT_Rect ARect) {}
//  virtual void popClip() {}
//};

//struct SAT_PaintContext {
//  SAT_Painter*  painter     = nullptr;
//  SAT_Rect      updaterect  = {};
//  uint32_t      counter     = 0;
//};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Widget;
typedef SAT_Array<SAT_Widget*> SAT_WidgetArray;
typedef SAT_LockFreeQueue<SAT_Widget*,SAT_WIDGET_DIRTY_QUEUE_SIZE> SAT_WidgetQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Widget
: public SAT_WidgetListener {

//------------------------------
private:
//------------------------------

  SAT_Rect              MRect                               = {};       // on screen, after alignment & scaling
  SAT_Rect              MBasisRect                          = {};       // basis for alignment & scaling (unscaled, untransformed)
  SAT_Rect              MInitialRect                        = {};       // as created, constructor

  SAT_WidgetOwner*      MOwner                              = nullptr;  // aka window
  SAT_WidgetListener*   MListener                           = nullptr;  // event parent
  SAT_Widget*           MParentWidget                       = nullptr;  // hierarchial parent
  SAT_WidgetArray       MChildWidgets                       = {};       // sub widgets
  int32_t               MIndex                              = -1;       // index (parent)
  
  uint32_t              MAlignment                          = SAT_WIDGET_ALIGN_PARENT;
  uint32_t              MStretching                         = SAT_WIDGET_STRETCH_NONE;
  SAT_Point             MSpacing                            = SAT_Point();
  SAT_Rect              MInnerBorder                        = SAT_Rect();
  
  const char*           MName                               = "";
  const char*           MHint                               = "";
  int32_t               MMouseCursor                        = 0;
  double                MValues[SAT_WIDGET_MAX_VALUES]      = {0};
  double                MModulations[SAT_WIDGET_MAX_VALUES] = {0};
  void*                 MConnections[SAT_WIDGET_MAX_VALUES] = {0};
  
  bool                  MAutoClip                           = false;
  bool                  MAutoHoverCursor                    = false;
  bool                  MAutoHoverHint                      = false;
  
  bool                  MIsActive                           = true;
  bool                  MIsVisible                          = true;
  bool                  MIsDisabled                         = false;
  bool                  MIsInteracting                      = false;
  
  //
  
  
//------------------------------
public:
//------------------------------

  SAT_Widget(SAT_Rect ARect) {
  }
  
  //----------

  virtual ~SAT_Widget() {
    #ifndef SAT_NO_AUTODELETE
      deleteChildWidgets();
    #endif
  }

//------------------------------
public:
//------------------------------

  virtual bool                autoClip()                        { return MAutoClip; }
  virtual bool                autoHoverCursor()                 { return MAutoHoverCursor; }
  virtual bool                autoHoverHint()                   { return MAutoHoverHint; }

  virtual SAT_Rect            getBasisRect()                    { return MBasisRect; }
  virtual SAT_Widget*         getChildWidget(uint32_t AIndex)   { return MChildWidgets[AIndex]; }
  virtual SAT_WidgetArray*    getChildWidgets()                 { return &MChildWidgets; }
  virtual void*               getConnection(uint32_t AIndex=0)  { return MConnections[AIndex]; }
  virtual SAT_Rect            getContentRect()                  { return SAT_Rect(); }
  virtual double              getHeight()                       { return MRect.h; }
  virtual const char*         getHint()                         { return MHint; }
  virtual int32_t             getIndex()                        { return MIndex; }
  virtual SAT_Rect            getInitialRect()                  { return MInitialRect; }
  virtual SAT_WidgetListener* getListener()                     { return MListener; }
  virtual int32_t             getMouseCursor()                  { return MMouseCursor; }
  virtual uint32_t            getNumChildWidgets()              { return MChildWidgets.size(); }
  virtual SAT_WidgetOwner*    getOwner()                        { return MOwner; }
  virtual SAT_Widget*         getParentWidget()                 { return MParentWidget; }
  virtual SAT_Rect            getRect()                         { return MRect; }
  virtual double              getModulation(uint32_t AIndex=0)  { return MModulations[AIndex]; }
  virtual double              getValue(uint32_t AIndex=0)       { return MValues[AIndex]; }
  virtual double              getWidth()                        { return MRect.w; }
  virtual double              getWindowScale()                  { return MOwner->do_widget_get_scale(); }

  virtual bool                isActive()                        { return MIsActive; }
  virtual bool                isVisible()                       { return MIsVisible; }
  virtual bool                isDisabled()                      { return MIsDisabled; }
  virtual bool                isInteracting()                   { return MIsInteracting; }

  //
  
  
//------------------------------
public:
//------------------------------

  virtual void setActive(bool AState=true)                      { MIsActive = AState; }
  virtual void setAlignment(uint32_t AAlignment)                { MAlignment = AAlignment; }
  virtual void setAutoClip(bool AState=true)                    { MAutoClip = AState; }
  virtual void setAutoHoverCursor(bool AState=true)             { MAutoHoverCursor = AState; }
  virtual void setAutoHoverHint(bool AState=true)               { MAutoHoverHint = AState; }
  virtual void setBasisHeight(double AHeight)                   { MBasisRect.h = AHeight; } /*todo: scale*/
  virtual void setBasisRect(SAT_Rect ARect)                     { MBasisRect = ARect; } /*todo: scale*/
  virtual void setBasisWidth(double AWidth)                     { MBasisRect.w = AWidth; } /*todo: scale*/
  virtual void setCursor(int32_t ACursor)                       { MMouseCursor = ACursor; }
  virtual void setDisabled(bool AState=true)                    { MIsDisabled = AState; }
  virtual void setHeight(double AHeight)                        { MRect.h = AHeight; }
  virtual void setIndex(int32_t AIndex)                         { MIndex = AIndex; }
  virtual void setInnerBorder(SAT_Rect ARect)                   { MInnerBorder = ARect; }
  virtual void setInteracting(bool AState=true)                 { MIsInteracting = AState; }
  virtual void setLayout(uint32_t AAlign, uint32_t AStretch)    { MAlignment = AAlign; MStretching = AStretch; }
  virtual void setListener(SAT_WidgetListener* AListener)       { MListener = AListener; }
  virtual void setModulation(double AValue, uint32_t AIndex=0)  { MValues[AIndex] = AValue; }
  virtual void setName(const char* AName)                       { MName = AName; }
  virtual void setParentWidget(SAT_Widget* AParent)             { MParentWidget = AParent; }
  virtual void setRect(SAT_Rect ARect)                          { MRect = ARect; }
  virtual void setRectAndBasis(SAT_Rect ARect)                  { MRect = ARect; MBasisRect = ARect; } /*todo: scale*/
  virtual void setSpacing(SAT_Point ASpacing)                   { MSpacing = ASpacing; }
  virtual void setStretching(uint32_t AStretching)              { MStretching = AStretching; }
  virtual void setValue(double AValue, uint32_t AIndex=0)       { MValues[AIndex] = AValue; }
  virtual void setVisible(bool AState=true)                     { MIsVisible = AState; }
  virtual void setWidth(double AWidth)                          { MRect.w = AWidth; }

//------------------------------
public: // hierarchy
//------------------------------

  virtual SAT_Widget* appendChildWidget(SAT_Widget* AChild, SAT_WidgetListener* AListener=nullptr) {
    int32_t index = MChildWidgets.size();
    AChild->setIndex(index);
    if (AListener) AChild->setListener(AListener);
    else AChild->setListener(this);
    return AChild;
  }

  //----------
  
  //virtual void removeChildWidget(int32_t AIndex) {
  //  SAT_Widget* widget = MChildWidgets[AIndex];
  //  MChildWidgets.remove(AIndex);
  //  widget->setIndex(-1);
  //  widget->setListener(nullptr);
  //}

  //----------

  virtual void deleteChildWidgets(){
    for (uint32_t i=0; i<MChildWidgets.size(); i++) delete MChildWidgets[i];
    MChildWidgets.clear();
  }

  //----------
  
  virtual void prepareWidgets(SAT_WidgetOwner* AOwner) {
    on_widget_prepare();
    MOwner = AOwner;
    for (uint32_t i=0; i<MChildWidgets.size(); i++) {
      MChildWidgets[i]->setParentWidget(this);
      MChildWidgets[i]->prepareWidgets(MOwner);
    }
  }

  //----------
  
  virtual SAT_Widget* findChildWidget(double AXpos, double AYpos) {
    for (uint32_t i=0; i<MChildWidgets.size(); i++) {
      SAT_Widget* widget = MChildWidgets[i];
      if (widget->isActive()) {
        SAT_Rect rect = widget->getRect();
        if (rect.contains(AXpos,AYpos)) {
          SAT_Widget* child = widget->findChildWidget(AXpos,AYpos);
          return child;
        }
      }
    }
    if (MIsActive) return this;
    else return nullptr;
  }

  //----------
  
  virtual void paintChildWidgets(SAT_PaintContext* AContext) {
    SAT_Painter* painter = AContext->painter;
    SAT_Rect mrect = getRect();
    for (uint32_t i=0; i<MChildWidgets.size(); i++) {
      SAT_Widget* widget = MChildWidgets[i];
      SAT_Rect widgetrect = widget->getRect();
      widgetrect.overlap(mrect);
      if (widgetrect.isNotEmpty()) {
        painter->pushOverlappingClip(widgetrect);
        widget->on_widget_paint(AContext);
        painter->popClip();
      }
    }
  }

  //----------
  
  virtual void scrollChildWidgets(double ADeltaX, double ADeltaY) {
  }
  
  //----------
  
  /*
    scale
    border
    - percent
    - align
    - stretch
    - limit
    - combine
  */

  virtual void realignChildWidgets() {
  }
  
  //----------
  
  virtual void scaleWidget(double AScale, bool ARecursive=true) {
  }

//------------------------------
public: // base
//------------------------------

  virtual void on_widget_prepare() {
  }
  
  virtual void on_widget_move(double AXpos, double AYpos) {
  }
  
  virtual void on_widget_resize(double AWidth, double AHeight) {
  }
  
  virtual void on_widget_paint(SAT_PaintContext* AContext) {
    paintChildWidgets(AContext);
  }
  
  virtual void on_widget_timer(uint32_t AId, double ADelta) {
  }

  virtual void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATimestamp) {
  }

  virtual void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATimestamp) {
  }

  virtual void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) {
  }

  virtual void on_widget_mouse_dblclick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) {
  }

  virtual void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) {
  }

  virtual void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATimestamp) {
  }

  virtual void on_widget_mouse_enter(SAT_Widget* AFrom, double AXpos, double AYpos, uint32_t ATimestamp) {
  }

  virtual void on_widget_mouse_leave(SAT_Widget* ATo, double AXpos, double AYpos, uint32_t ATimestamp) {
  }

  virtual void on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ACount, double* AValue) {
  }

//------------------------------
public: // widget listener
//------------------------------

  void do_widget_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (MListener) MListener->do_widget_update(ASender,AMode,AIndex);
  }

  //----------

  void do_widget_redraw(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (MListener) MListener->do_widget_redraw(ASender,AMode,AIndex);
  }

  //----------

  void do_widget_realign(SAT_Widget* ASender) override {
    if (MListener) MListener->do_widget_realign(ASender);
  }

  //----------

  void do_widget_close(SAT_Widget* ASender) override {
    if (MListener) MListener->do_widget_close(ASender);
  }

  //----------

  void do_widget_select(SAT_Widget* ASender, int32_t AIndex) override {
    if (MListener) MListener->do_widget_select(ASender,AIndex);
  }

  //----------

  void do_widget_set_cursor(SAT_Widget* ASender, int32_t ACursor) override {
    if (MListener) MListener->do_widget_set_cursor(ASender,ACursor);
  }

  //----------

  void do_widget_set_hint(SAT_Widget* ASender, uint32_t AMode, const char* AHint) override {
    if (MListener) MListener->do_widget_set_hint(ASender,AMode,AHint);
  }

  //----------

  void do_widget_set_modal(SAT_Widget* ASender) override {
    if (MListener) MListener->do_widget_set_modal(ASender);
  }
  
  //void do_widget_notify(SAT_Widget* ASender, uint32_t AMessage, int32_t AValue)  override {
  //  if (MListener) MListener->do_widget_notify(ASender,AMessage,AValue);
  //}

};

//----------------------------------------------------------------------
#endif
