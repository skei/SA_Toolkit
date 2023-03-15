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
  bool                MIsDisabled                         = false;

  SAT_Rect            MRect                               = {};
  SAT_Rect            MInitialRect                        = {};
  SAT_Rect            MBasisRect                          = {};
  uint32_t            MAlignment                          = SAT_WIDGET_ALIGN_PARENT;
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
    MBasisRect = ARect;
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

  virtual void        setListener(SAT_WidgetListener* AListener)      { MListener = AListener; }
  virtual void        setIndex(uint32_t AIndex)                       { MIndex = AIndex; }
  virtual void        setRect(SAT_Rect ARect)                         { MRect = ARect; }
  virtual void        setAlignment(uint32_t AAlignment)               { MAlignment = AAlignment; }
  virtual void        setValue(double AValue, uint32_t AIndex=0)      { MValues[AIndex] = AValue; }
  virtual void        addValue(double AValue, uint32_t AIndex=0)      { MValues[AIndex] += AValue; }
  virtual void        setModulation(double AValue, uint32_t AIndex=0) { MModulations[AIndex] = AValue; }
  virtual void        setCursor(uint32_t ACursor)                     { MCursor = ACursor; }
  virtual void        setHint(const char* AHint)                      { strcpy(MHint,AHint); }
  virtual void        setAutoCursor(bool AState=true)                 { MAutoCursor = AState; }
  virtual void        setAutoHint(bool AState=true)                   { MAutoHint = AState; }

  virtual void setActive(bool AState=true, bool ARecursive=true) {
    MIsActive = AState;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->setActive(AState,ARecursive);
      }
    }
  }

  virtual void setVisible(bool AState=true, bool ARecursive=true) {
    MIsVisible = AState;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->setVisible(AState,ARecursive);
      }
    }
  }

  virtual void        setDisabled(bool AState=true)                   { MIsDisabled = AState; }
  virtual void        setLastPainted(int32_t ACount)                  { MLastPainted = ACount; }

  //virtual void setInitialRect(SAT_Rect ARect) { MInitialRect = ARect; }
  virtual void setBasisRect(SAT_Rect ARect) { MBasisRect = ARect; }

public:

  virtual uint32_t    getIndex()                                      { return MIndex; }
  virtual SAT_Rect    getRect()                                       { return MRect; }
  virtual double      getWidth()                                      { return MRect.w; }
  virtual double      getHeight()                                     { return MRect.h; }
  virtual double      getValue(uint32_t AIndex=0)                     { return MValues[AIndex]; }
  virtual double      getModulation(uint32_t AIndex=0)                { return MModulations[AIndex]; }
  virtual void*       getConnection(uint32_t AIndex=0)                { return MConnections[AIndex]; }
  virtual bool        isActive()                                      { return MIsActive; }
  virtual bool        isVisible()                                     { return MIsVisible; }
  virtual bool        isDisabled()                                    { return MIsDisabled; }
  virtual double      getWindowScale()                                { return MWindowScale; }
  virtual int32_t     getLastPainted()                                { return MLastPainted; }
  //virtual SAT_Rect    getInitialRect()                                { return MInitialRect; }
  virtual SAT_Rect    getBasisRect()                                  { return MBasisRect; }
  virtual uint32_t    getAlignment()                                  { return MAlignment; }
  virtual uint32_t    getNumChildWidgets()                            { return MChildren.size(); }
  virtual SAT_Widget* getChildWidget(uint32_t AIndex)                 { return MChildren[AIndex]; }
  
  virtual uint32_t    getNumPopupMenuItems()                          { return 0; }
  virtual const char* getPopupMenuItem(uint32_t AIndex)               { return ""; }
  virtual void        activatePopupMenuItem(uint32_t AIndex)          { }

//------------------------------
public:
//------------------------------

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

  virtual void cleanup(SAT_Painter* APainter, bool ARecursive=true) {
    uint32_t num = MChildren.size();
    if (ARecursive) {
      for (uint32_t i=0; i<num; i++) {
        MChildren[i]->cleanup(APainter,ARecursive);
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

  //----------

  virtual void redraw() {
    //SAT_PRINT;
    if (MListener) MListener->do_widget_redraw(this,0,0);
  }

  //----------

  virtual void redrawAll() {
    //SAT_PRINT;
    if (MListener) MListener->do_widget_redraw(nullptr,0,0);
  }

//------------------------------
public:
//------------------------------

  virtual SAT_Widget* appendChildWidget(SAT_Widget* AWidget, SAT_WidgetListener* AListener=nullptr) {
    uint32_t index = MChildren.size();
    if (AListener) AWidget->setListener(AListener);
    else AWidget->setListener(this);
    AWidget->setIndex(index);

    //AWidget->addPos(MRect.x,MRect.y);

    MChildren.append(AWidget);
    return AWidget;
  }

  //----------

  virtual void deleteChildWidgets() {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      if (MChildren[i]) delete MChildren[i];
      MChildren[i] = nullptr;
    }
  }

  //----------

  virtual void scaleWidget(double AScale, bool ARecursive=true) {
    MWindowScale = AScale;
    //MRect.scale(AScale);
    MRect = MBasisRect;
    MRect.scale(AScale);
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->scaleWidget(AScale);
      }
    }
  }

  //----------

  virtual void initScaleWidget(double AScale, bool ARecursive=true) {
    MRect.scale(AScale);
    MInitialRect.scale(AScale);
    MBasisRect.scale(AScale);
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->initScaleWidget(AScale,ARecursive);
      }
    }
  }

  //----------

  virtual void realignChildWidgets(bool ARecursive=true) {
    SAT_Rect parent_rect = getRect();
    double S = getWindowScale();
    //SAT_Print("S %.3f\n",S);
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* child = MChildren[i];
      SAT_Rect child_basisrect = child->getBasisRect();
      child_basisrect.scale(S);
      uint32_t alignment = child->getAlignment();
      switch (alignment) {
        
        case SAT_WIDGET_ALIGN_NONE: {
          break;
        }
        
        case SAT_WIDGET_ALIGN_PARENT: {
          child->MRect.x = parent_rect.x + child_basisrect.x;
          child->MRect.y = parent_rect.y + child_basisrect.y;
          break;
        }
        
        case SAT_WIDGET_ALIGN_FILL_PARENT: {
          child->MRect = parent_rect;
          break;
        }
        
      }
      //SAT_Print("%.3f, %.3f\n",child->MRect.x,child->MRect.y);
      if (ARecursive) {
        child->realignChildWidgets(ARecursive);
      }
    }
  }

  //----------

  /*
    return child widget at x,y
    or self if no sub-widget at that point
  */

  virtual SAT_Widget* findChildWidget(double AXpos, double AYpos, bool ARecursive=true) {
    if (!MRect.contains(AXpos,AYpos)) return nullptr;
    if (!isActive()) return nullptr;
    uint32_t num = MChildren.size();
    if (num > 0) {
      for (int32_t i=num-1; i>=0; i--) {
        SAT_Widget* widget = MChildren[i];
        if (widget->isActive() && widget->isVisible()) {
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

  //----------

  virtual void paintChildWidgets(SAT_PaintContext* AContext, bool ARecursive=true) {
//    SAT_Painter* painter = AContext->painter;
//    SAT_Rect mrect = getRect();
    uint32_t num = MChildren.size();
    if (num > 0) {
//      SAT_Rect cliprect = mrect;
//      painter->setClipRect(cliprect);
      //painter->pushClip(cliprect);
      for (uint32_t i=0; i<num; i++) {
        SAT_Widget* widget = MChildren[i];
        if (widget->isVisible()) {
          SAT_Rect widgetrect = widget->getRect();
          //widgetrect.overlap(cliprect);
          if (widgetrect.isNotEmpty()) {
            //painter->pushOverlapClip(widgetrect);
//            painter->pushClip(widgetrect);
            widget->on_widget_paint(AContext);
//            painter->popClip();
          } // intersect
        } // visible
      } // for
      //painter->popClip();
    } // num > 0
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

  //virtual void scaleChildWidgets(double AScale) {
  //  uint32_t num = MChildren.size();
  //  for (uint32_t i=0; i<num; i++) {
  //  }
  //}

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
  
  virtual void on_widget_mouse_dblclick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {
  }
  
  virtual void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_mouse_enter(SAT_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) {
    if (MAutoCursor) do_widget_set_cursor(this,MCursor);
    if (MAutoHint) do_widget_set_hint(this,MHint);
  }

  virtual void on_widget_mouse_leave(SAT_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) {
    if (MAutoCursor) do_widget_set_cursor(this,SAT_CURSOR_DEFAULT);
    if (MAutoHint) do_widget_set_hint(this,"");
  }

  virtual void on_widget_notify(uint32_t AReason=SAT_WIDGET_NOTIFY_NONE) {
  }

  virtual void on_widget_timer() {
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

  void do_widget_set_cursor(SAT_Widget* ASender, int32_t ACursor) override {
    if (MListener) MListener->do_widget_set_cursor(ASender,ACursor);
  }

  void do_widget_set_hint(SAT_Widget* ASender, const char* AHint) override {
    if (MListener) MListener->do_widget_set_hint(ASender,AHint);
  }

  void do_widget_notify(SAT_Widget* ASender, uint32_t AReason, int32_t AValue) override {
    if (MListener) MListener->do_widget_notify(ASender,AReason,AValue);
  }

};

//----------------------------------------------------------------------
#endif

