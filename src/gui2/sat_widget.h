#ifndef sat_widget_included
#define sat_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"

#define SAT_WIDGET_DIRTY_QUEUE_SIZE 1024

//----------------------------------------------------------------------

class SAT_TimerListener {
public:
  virtual void do_timerListener_update() {}
};

class SAT_Painter {
public:
  virtual void setClipRect(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void resetClip() {}
  virtual void pushClip(SAT_Rect ARect) {}
  virtual void pushOverlappingClip(SAT_Rect ARect) {}
  virtual void popClip() {}
};

struct SAT_PaintContext {
  SAT_Painter*  painter     = nullptr;
  SAT_Rect      updaterect  = {};
  uint32_t      counter     = 0;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Widget;
typedef SAT_Array<SAT_Widget*> SAT_WidgetArray;
typedef SAT_LockFreeQueue<SAT_Widget*,SAT_WIDGET_DIRTY_QUEUE_SIZE> SAT_WidgetQueue;

//----------


class SAT_WidgetOwner {
public:
  virtual double        do_widgetOwner_getWidth()   { return 0; }
  virtual double        do_widgetOwner_getHeight()  { return 0; } 
  virtual double        do_widgetOwner_getScale()   { return 1.0; }
  virtual SAT_Painter*  do_widgetOwner_getPainter() { return 0; } 
};

//----------

class SAT_WidgetListener {
public:
  virtual bool  do_widgetListener_update(SAT_Widget* ASender)                                     { return true; }
  virtual bool  do_widgetListener_redraw(SAT_Widget* ASender)                                     { return true; }
  virtual bool  do_widgetListener_realign(SAT_Widget* ASender)                                    { return true; }
  virtual bool  do_widgetListener_close(SAT_Widget* ASender)                                      { return true; }
  virtual bool  do_widgetListener_select(SAT_Widget* ASender)                                     { return true; }
  virtual bool  do_widgetListener_setCursor(SAT_Widget* ASender, int32_t ACursor)                 { return true; }
  virtual bool  do_widgetListener_setHint(SAT_Widget* ASender, uint32_t AMode, const char* AHint) { return true; }
  virtual bool  do_widgetListener_setModal(SAT_Widget* ASender)                                   { return true; }
  virtual bool  do_widgetListener_notify(SAT_Widget* ASender, uint32_t AMessage, int32_t AValue)  { return true; }

};

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

  SAT_Rect              MRect               = {};       // on screen, after alignment & scaling
  SAT_Rect              MBasisRect          = {};       // basis for alignment & scaling (unscaled, untransformed)
  SAT_Rect              MInitialRect        = {};       // as created, constructor

  SAT_WidgetOwner*      MOwner              = nullptr;  // aka window
  SAT_WidgetListener*   MListener           = nullptr;  // event parent
  SAT_Widget*           MParent             = nullptr;  // hierarchial parent
  SAT_WidgetArray       MChildren           = {};       // sub widgets
  int32_t               MIndex              = -1;       // index (parent)
  
  uint32_t              MAlignment          = SAT_WIDGET_ALIGN_PARENT;
  uint32_t              MStretching         = SAT_WIDGET_STRETCH_NONE;
  SAT_Point             MSpacing            = SAT_Point();
  SAT_Rect              MInnerBorder        = SAT_Rect();
  
  const char*           MHint               = "";
  int32_t               MMouseCursor        = 0;
  
  bool                  MAutoHoverCursor    = false;
  bool                  MAutoHoverHint      = false;
  
  bool                  MIsActive           = true;
  bool                  MIsVisible          = true;
  bool                  MIsDisabled         = false;
  bool                  MIsInteracting      = false;
  
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

  virtual void setIndex(int32_t AIndex)                   { MIndex = AIndex; }
  virtual void setListener(SAT_WidgetListener* AListener) { MListener = AListener; }
  virtual void setParent(SAT_Widget* AParent)             { MParent = AParent; }

  virtual void setAutoHoverCursor(bool AState=true)       { MAutoHoverCursor = AState; }
  virtual void setAutoHoverHint(bool AState=true)         { MAutoHoverHint = AState; }

  virtual void setIsActive(bool AState=true)              { MIsActive = AState; }
  virtual void setIsVisible(bool AState=true)             { MIsVisible = AState; }
  virtual void setIsDisabled(bool AState=true)            { MIsDisabled = AState; }
  virtual void setIsInteracting(bool AState=true)         { MIsInteracting = AState; }

//------------------------------
public:
//------------------------------

  virtual SAT_Rect            getRect()                 { return MRect; }
  virtual SAT_Rect            getBasisRect()            { return MBasisRect; }
  virtual SAT_Rect            getInitialRect()          { return MInitialRect; }

  virtual SAT_WidgetOwner*    getOwner()                { return MOwner; }
  virtual SAT_WidgetListener* getListener()             { return MListener; }
  virtual SAT_Widget*         getParent()               { return MParent; }
  virtual SAT_WidgetArray*    getChildren()             { return &MChildren; }
  virtual uint32_t            getNumChildren()          { return MChildren.size(); }
  virtual SAT_Widget*         getChild(uint32_t AIndex) { return MChildren[AIndex]; }
  virtual int32_t             getIndex()                { return MIndex; }

  virtual const char*         getHint()                 { return MHint; }
  virtual int32_t             getMouseCursor()          { return MMouseCursor; }

  virtual bool                autoHoverCursor()         { return MAutoHoverCursor; }
  virtual bool                autoHoverHint()           { return MAutoHoverHint; }

  virtual bool                isActive()                { return MIsActive; }
  virtual bool                isVisible()               { return MIsVisible; }
  virtual bool                isDisabled()              { return MIsDisabled; }
  virtual bool                isInteracting()           { return MIsInteracting; }

//------------------------------
public: // hierarchy
//------------------------------

  virtual int32_t appendChildWidget(SAT_Widget* AChild, SAT_WidgetListener* AListener=nullptr) {
    int32_t index = MChildren.size();
    AChild->setIndex(index);
    if (AListener) AChild->setListener(AListener);
    else AChild->setListener(this);
    return index;
  }

  //----------
  
  virtual void removeChildWidget(int32_t AIndex) {
    SAT_Widget* widget = MChildren[AIndex];
    MChildren.remove(AIndex);
    widget->setIndex(-1);
    widget->setListener(nullptr);
  }

  //----------

  virtual void deleteChildWidgets(){
    for (uint32_t i=0; i<MChildren.size(); i++) delete MChildren[i];
    MChildren.clear();
  }

  //----------
  
  virtual void prepareWidgets(SAT_WidgetOwner* AOwner) {
    on_widget_prepare();
    MOwner = AOwner;
    for (uint32_t i=0; i<MChildren.size(); i++) {
      MChildren[i]->setParent(this);
      MChildren[i]->prepareWidgets(MOwner);
    }
  }

  //----------
  
  virtual SAT_Widget* findChildWidget(double AXpos, double AYpos) {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* widget = MChildren[i];
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
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* widget = MChildren[i];
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

  virtual void on_widget_mouse_enter(double AXpos, double AYpos, SAT_Widget* AFrom) {
  }

  virtual void on_widget_mouse_leave(double AXpos, double AYpos, SAT_Widget* ATo) {
  }

//------------------------------
public: // widget listener
//------------------------------

  bool do_widgetListener_update(SAT_Widget* ASender) {
    if (MListener) return MListener->do_widgetListener_update(ASender);
    return false;
  }

  //----------

  bool do_widgetListener_redraw(SAT_Widget* ASender) {
    if (MListener) return MListener->do_widgetListener_redraw(ASender);
    return false;
  }

  //----------

  bool do_widgetListener_realign(SAT_Widget* ASender) {
    if (MListener) return MListener->do_widgetListener_realign(ASender);
    return false;
  }

  //----------

  bool do_widgetListener_close(SAT_Widget* ASender) {
    if (MListener) return MListener->do_widgetListener_close(ASender);
    return false;
  }

  //----------

  bool do_widgetListener_select(SAT_Widget* ASender) {
    if (MListener) return MListener->do_widgetListener_select(ASender);
    return false;
  }

  //----------

  bool do_widgetListener_setCursor(SAT_Widget* ASender, int32_t ACursor) {
    if (MListener) return MListener->do_widgetListener_setCursor(ASender,ACursor);
    return false;
  }

  //----------

  bool do_widgetListener_setHint(SAT_Widget* ASender, uint32_t AMode, const char* AHint) {
    if (MListener) return MListener->do_widgetListener_setHint(ASender,AMode,AHint);
    return false;
  }

  //----------

  bool do_widgetListener_setModal(SAT_Widget* ASender) {
    if (MListener) return MListener->do_widgetListener_setModal(ASender);
    return false;
  }

};

//----------------------------------------------------------------------
#endif
