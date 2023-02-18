#ifndef sat_widget_included
#define sat_widget_included
//----------------------------------------------------------------------

#include "gui/sat_paint_context.h"
#include "gui/sat_widget_listener.h"


class SAT_Widget;
typedef SAT_Array<SAT_Widget*> SAT_WidgetArray;

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

  SAT_WidgetListener* MListener                           = nullptr;
  SAT_WidgetArray     MChildren                           = {};
  uint32_t            MIndex                              = 0;

  double              MValues[SAT_WIDGET_MAX_VALUES]      = {0};
  double              MModulations[SAT_WIDGET_MAX_VALUES] = {0};
  void*               MConnections[SAT_WIDGET_MAX_VALUES] = {0};

  bool                MIsActive                           = true;
  bool                MIsVisible                          = true;

  SAT_Rect            MRect                               = {};
  SAT_Rect            MInitialRect                        = {};
  uint32_t            MCursor                             = SAT_CURSOR_DEFAULT;
  char                MHint[256]                          = "";

  bool                MAutoCursor                         = true;
  bool                MAutoHint                           = true;

  double              MWindowScale                        = 1.0;

  int32_t             MLastPainted                        = -1;

//------------------------------
public:
//------------------------------

  SAT_Widget(SAT_Rect ARect) {
    MRect = ARect;
    MInitialRect = ARect;
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

  void        setListener(SAT_WidgetListener* AListener)      { MListener = AListener; }
  void        setIndex(uint32_t AIndex)                       { MIndex = AIndex; }
  void        setRect(SAT_Rect ARect)                         { MRect = ARect; }
  void        setValue(double AValue, uint32_t AIndex=0)      { MValues[AIndex] = AValue; }
  void        addValue(double AValue, uint32_t AIndex=0)      { MValues[AIndex] += AValue; }
  void        setModulation(double AValue, uint32_t AIndex=0) { MModulations[AIndex] = AValue; }
  void        setCursor(uint32_t ACursor)                     { MCursor = ACursor; }
  void        setHint(const char* AHint)                      { strcpy(MHint,AHint); }
  void        setAutoCursor(bool AState=true)                 { MAutoCursor = AState; }
  void        setAutoHint(bool AState=true)                   { MAutoHint = AState; }
  void        setActive(bool AState=true)                     { MIsActive = AState; }
  void        setVisible(bool AState=true)                    { MIsVisible = AState; }
  void        setLastPainted(int32_t ACount)                  { MLastPainted = ACount; }

public:

  uint32_t    getIndex()                                      { return MIndex; }
  SAT_Rect    getRect()                                       { return MRect; }
  double      getValue(uint32_t AIndex=0)                     { return MValues[AIndex]; }
  double      getModulation(uint32_t AIndex=0)                { return MModulations[AIndex]; }
  void*       getConnection(uint32_t AIndex=0)                { return MConnections[AIndex]; }
  bool        isActive()                                      { return MIsActive; }
  bool        isVisible()                                     { return MIsVisible; }
  double      getWindowScale()                                { return MWindowScale; }
  int32_t     getLastPainted()                                { return MLastPainted; }

//------------------------------
public:
//------------------------------

  // called from SAT_Editor.show()

  virtual void prepare(SAT_WidgetListener* AWindow, bool ARecursive=true) {
    uint32_t num = MChildren.size();
    if (ARecursive) {
      for (uint32_t i=0; i<num; i++) {
        MChildren[i]->prepare(AWindow,ARecursive);
      }
    }
  }

  //----------

  virtual void connect(void* AParameter, uint32_t AIndex=0) {
    MConnections[AIndex] = AParameter;
  }

  //----------

  virtual void update() {
    //SAT_PRINT;
    if (MListener) MListener->do_widget_update(this,0,0);
  }

  virtual void redraw() {
    //SAT_PRINT;
    if (MListener) MListener->do_widget_redraw(this,0,0);
  }

  //----------

  virtual void scale(double AScale, bool ARecursive=true) {
    MWindowScale = AScale;
    //MRect.scale(AScale);
    MRect = MInitialRect;
    MRect.scale(AScale);
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->scale(AScale);
      }
    }
  }

  //----------

  //virtual void setPaintStyle(SAT_PaintStyle* AStyle, bool ARecursive=true) {
  //  uint32_t num = MChildren.size();
  //  if (ARecursive) {
  //    for (uint32_t i=0; i<num; i++) {
  //      MChildren[i]->setPaintStyle(AStyle,ARecursive);
  //    }
  //  }
  //}

  //----------

//  virtual void paintChildWidgets(SAT_PaintContext* AContext, bool ARecursive=true) {
//    uint32_t num = MChildren.size();
//    for (uint32_t i=0; i<num; i++) {
//      SAT_Widget* child = MChildren[i];
//      child->on_widget_paint(AContext);
//    }
//  }

  //----------

  virtual void scaleChildWidgets(double AScale) {
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
    }
  }

  //----------

  virtual void paintChildWidgets(SAT_PaintContext* AContext, bool ARecursive=true) {
//    SAT_Rect updaterect = AContext->update_rect;
//    SAT_Painter* painter = AContext->painter;
//    SAT_Rect mrect = getRect();
    uint32_t num = MChildren.size();
    if (num > 0) {
//      SAT_Rect cliprect = mrect;
//      cliprect.overlap(updaterect);
      for (uint32_t i=0; i<num; i++) {
        SAT_Widget* widget = MChildren[i];
        if (widget->isVisible()) {
//          SAT_Rect widgetrect = widget->getRect();
//          widgetrect.overlap(cliprect);
//          if (widgetrect.isNotEmpty()) {
//            painter->pushOverlapClip(widgetrect);
            widget->on_widget_paint(AContext);
//            painter->popClip();
//          } // intersect
        } // visible
      } // for
    } // num > 0
  }

  //----------

  virtual void setPos(double AXpos, double AYpos) {
    MRect.x = AXpos;
    MRect.y = AYpos;
  }

  //----------

  virtual void setSize(double AWidth, double AHeight) {
    MRect.w = AWidth;
    MRect.h = AHeight;
  }

  //----------

  virtual void addPos(double AXpos, double AYpos) {
    MRect.x += AXpos;
    MRect.y += AYpos;
  }

  //----------

//------------------------------
public: // widget
//------------------------------

  virtual void on_widget_move(double AXpos, double AYpos) {
  }

  virtual void on_widget_resize(double AWidth, double AHeight) {
  }

  virtual void on_widget_paint(SAT_PaintContext* AContext) {
    paintChildWidgets(AContext);
  }

  virtual void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_mouse_release(uint32_t AKey, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_mouse_enter(SAT_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) {
    if (MAutoCursor) do_widget_set_cursor(this,MCursor);
    if (MAutoHint) do_widget_set_hint(this,MHint);
  }

  virtual void on_widget_mouse_leave(SAT_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) {
    if (MAutoCursor) do_widget_set_cursor(this,SAT_CURSOR_DEFAULT);
    if (MAutoHint) do_widget_set_hint(this,"");
  }

  virtual void on_widget_notify(uint32_t AReason=0, int32_t AValue=0) {
  }

//------------------------------
public: // widget listener
//------------------------------

  void do_widget_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (MListener) MListener->do_widget_update(ASender,AMode,AIndex);
  }

  void do_widget_redraw(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (MListener) MListener->do_widget_redraw(ASender,AMode,AIndex);
  }

  void do_widget_set_state(SAT_Widget* ASender, uint32_t AState) override {
    if (MListener) MListener->do_widget_set_state(ASender,AState);
  }

  void do_widget_set_cursor(SAT_Widget* ASender, uint32_t ACursor) override {
    if (MListener) MListener->do_widget_set_cursor(ASender,ACursor);
  }

  void do_widget_set_hint(SAT_Widget* ASender, const char* AHint) override {
    if (MListener) MListener->do_widget_set_hint(ASender,AHint);
  }

//------------------------------
public: // children
//------------------------------

  SAT_Widget* appendChildWidget(SAT_Widget* AWidget, SAT_WidgetListener* AListener=nullptr) {
    uint32_t index = MChildren.size();
    if (AListener) AWidget->setListener(AListener);
    else AWidget->setListener(this);
    AWidget->setIndex(index);
    //AWidget->addPos(MRect.x,MRect.y);
    MChildren.append(AWidget);
    return AWidget;
  }

  //----------

  void deleteChildWidgets() {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      if (MChildren[i]) delete MChildren[i];
      MChildren[i] = nullptr;
    }
  }

  //----------

  /*
    return child widget at x,y
    or self if no sub-widget at that point
  */

  SAT_Widget* findChildWidget(double AXpos, double AYpos, bool ARecursive=true) {
    uint32_t num = MChildren.size();
    if (num > 0) {
      for (int32_t i=num-1; i>=0; i--) {
        SAT_Widget* widget = MChildren[i];
        //if (widget->isVisible()) {
        if (widget->isActive()) {
          SAT_Rect rect = widget->getRect();
          if (rect.contains(AXpos,AYpos)) {
            SAT_Widget* child = widget;
            if (ARecursive) {
              child = widget->findChildWidget(AXpos,AYpos,ARecursive);
            }
            return child;
          }
        }
      }
    }
    return this;
  }

};

//----------------------------------------------------------------------
#endif

