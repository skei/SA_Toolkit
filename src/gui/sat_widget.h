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

  const char*         MName                               = "";

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
  SAT_Rect            MContentRect                        = {};
  
//  uint32_t            MAlignment                          = SAT_WIDGET_ALIGN_PARENT;
  uint32_t            MCursor                             = SAT_CURSOR_DEFAULT;
  char                MHint[256]                          = "";

  bool                MAutoCursor                         = true;
  bool                MAutoHint                           = true;

  double              MWindowScale                        = 1.0;

  int32_t             MLastPainted                        = -1;
  
  uint32_t            MAlignment                          = SAT_WIDGET_ALIGN_NONE;
  uint32_t            MStretching                         = SAT_WIDGET_STRETCH_NONE;
  SAT_Rect            MInnerBorder                        = SAT_Rect(0,0,0,0);

//------------------------------
public:
//------------------------------

  SAT_Widget(SAT_Rect ARect) {
    setName("SAT_Widget");
    MInitialRect = ARect;

    MRect = ARect;
    MRect.x = abs(MRect.x);
    MRect.y = abs(MRect.y);
    MRect.w = abs(MRect.w);
    MRect.h = abs(MRect.h);
    
    MBasisRect = MRect;
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

  virtual void setName(const char* AName) { MName = AName; }

  virtual void        setListener(SAT_WidgetListener* AListener)      { MListener = AListener; }
  virtual void        setIndex(uint32_t AIndex)                       { MIndex = AIndex; }
  virtual void        setRect(SAT_Rect ARect)                         { MRect = ARect; }
  virtual void        setValue(double AValue, uint32_t AIndex=0)      { MValues[AIndex] = AValue; }
  virtual void        addValue(double AValue, uint32_t AIndex=0)      { MValues[AIndex] += AValue; }
  virtual void        setModulation(double AValue, uint32_t AIndex=0) { MModulations[AIndex] = AValue; }
  virtual void        setCursor(uint32_t ACursor)                     { MCursor = ACursor; }
  virtual void        setHint(const char* AHint)                      { strcpy(MHint,AHint); }
  virtual void        setAutoCursor(bool AState=true)                 { MAutoCursor = AState; }
  virtual void        setAutoHint(bool AState=true)                   { MAutoHint = AState; }
  
  virtual void        setAlignment(uint32_t AAlignment)               { MAlignment = AAlignment; }
  virtual void        setStretching(uint32_t AStretch)                { MStretching = AStretch; }
  virtual void        setInnerBorder(SAT_Rect ABorder)                { MInnerBorder = ABorder; }

//virtual void        setInitialRect(SAT_Rect ARect)                  { MInitialRect = ARect; }
  virtual void        setBasisRect(SAT_Rect ARect)                    { MBasisRect = ARect; }

  virtual void        setDisabled(bool AState=true)                   { MIsDisabled = AState; }
  virtual void        setLastPainted(int32_t ACount)                  { MLastPainted = ACount; }

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

//------------------------------
public:
//------------------------------

  virtual const char*       getName() { return MName; }

  virtual uint32_t          getIndex()                                      { return MIndex; }
  virtual double            getWidth()                                      { return MRect.w; }
  virtual double            getHeight()                                     { return MRect.h; }
  virtual double            getValue(uint32_t AIndex=0)                     { return MValues[AIndex]; }
  virtual double            getModulation(uint32_t AIndex=0)                { return MModulations[AIndex]; }
  virtual void*             getConnection(uint32_t AIndex=0)                { return MConnections[AIndex]; }
  virtual bool              isActive()                                      { return MIsActive; }
  virtual bool              isVisible()                                     { return MIsVisible; }
  virtual bool              isDisabled()                                    { return MIsDisabled; }
  virtual double            getWindowScale()                                { return MWindowScale; }
  virtual int32_t           getLastPainted()                                { return MLastPainted; }
  
  virtual SAT_Rect          getRect()                                       { return MRect; }
  virtual SAT_Rect          getInitialRect()                                { return MInitialRect; }
  virtual SAT_Rect          getBasisRect()                                  { return MBasisRect; }
  virtual SAT_Rect          getContentRect()                                { return MContentRect; }
  
  virtual uint32_t          getNumChildWidgets()                            { return MChildren.size(); }
  virtual SAT_Widget*       getChildWidget(uint32_t AIndex)                 { return MChildren[AIndex]; }
  virtual SAT_WidgetArray*  getChildWidgets()                               { return &MChildren; }
  
  virtual uint32_t          getNumPopupMenuItems()                          { return 0; }
  virtual const char*       getPopupMenuItem(uint32_t AIndex)               { return ""; }
  virtual void              activatePopupMenuItem(uint32_t AIndex)          { }

  virtual uint32_t          getAlignment()                                  { return MAlignment; }
  virtual uint32_t          getStretching()                                 { return MStretching; }
  virtual SAT_Rect          getInnerBorder()                                { return MInnerBorder; }
  
  

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

  virtual void parentUpdate() {
    //SAT_PRINT;
    if (MListener) MListener->do_widget_update(this,0,0);
  }

  //----------

  virtual void parentRedraw() {
    //SAT_Print("%s\n",getName());
    if (MListener) MListener->do_widget_redraw(this,0,0);
  }

  //----------

  virtual void parentRedrawAll() {
    //SAT_PRINT;
    if (MListener) MListener->do_widget_redraw(nullptr,0,0);
  }

  //----------

  virtual void parentNotify(uint32_t AReason, int32_t AValue) {
    //SAT_PRINT;
    //if (MListener) MListener->do_widget_notify(this,AReason,AValue);
    if (MListener) MListener->do_widget_notify(this,AReason,AValue);
  }

  //----------

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
  
  // called from:
  // - SAT_Window.on_window_resize()
  //
  // SAT_GraphWidget overrides this (and call back)

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
  
  virtual void scrollChildren(float AOffsetX, float AOffsetY) {
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      SAT_Widget* child = MChildren[i];
      if (child->MIsVisible) {
        //child->setChildrenOffset(AOffsetX,AOffsetY);
        child->MRect.x += AOffsetX;
        child->MRect.y += AOffsetY;
        child->scrollChildren(AOffsetX,AOffsetY);
      }
    }
  }  
  
  //----------
  
  // called from:
  //   SAT_Window.on_window_resize()
  //   SAT_PopupWidget.open()

  virtual void realignChildWidgets(bool ARecursive=true) {
    double S = getWindowScale();
    SAT_Rect parent_rect = getRect();

    //    SAT_Rect ib = MInnerBorder;
    //    ib.scale(S);
    //    parent_rect.shrink(ib);

    SAT_Rect client_rect = parent_rect;
    //SAT_Rect content_rect = SAT_Rect(parent_rect.x,parent_rect.y,0,0);
    MContentRect = SAT_Rect(parent_rect.x,parent_rect.y,0,0);
    
    for (uint32_t i=0; i<MChildren.size(); i++) {

      SAT_Widget* child = MChildren[i];
      SAT_Rect child_basisrect = child->getBasisRect();
      child_basisrect.scale(S);

      // assume aligned relative to parent
      // (client or parent?)
      
      child->MRect.x = parent_rect.x + child_basisrect.x;
      child->MRect.y = parent_rect.y + child_basisrect.y;
      
      // negative = percent
      
      SAT_Rect child_initialrect = child->getInitialRect();
      if (child_initialrect.x < 0) child->MRect.x = parent_rect.w * (fabs(child_initialrect.x) * 0.01);
      if (child_initialrect.y < 0) child->MRect.y = parent_rect.h * (fabs(child_initialrect.y) * 0.01);
      if (child_initialrect.w < 0) child->MRect.w = parent_rect.w * (fabs(child_initialrect.w) * 0.01);
      if (child_initialrect.h < 0) child->MRect.h = parent_rect.h * (fabs(child_initialrect.h) * 0.01);

      // alignment

      uint32_t child_alignment = child->getAlignment();
      switch(child_alignment) {
        case SAT_WIDGET_ALIGN_LEFT: {
          //child->MRect.x = parent_rect.x;
          child->MRect.x = client_rect.x;
          client_rect.x += child->MRect.w;
          client_rect.w -= child->MRect.w;
          break;
        }
        case SAT_WIDGET_ALIGN_RIGHT: {
          //child->MRect.x = parent_rect.x2() - child->MRect.w;
          child->MRect.x = client_rect.x2() - child->MRect.w;
          //client_rect.x += child->MRect.w;
          client_rect.w -= child->MRect.w;
          break;
        }
        case SAT_WIDGET_ALIGN_TOP: {
          //child->MRect.y = parent_rect.y;
          child->MRect.y = client_rect.y;
          client_rect.y += child->MRect.h;
          client_rect.h -= child->MRect.h;
          break;
        }
        case SAT_WIDGET_ALIGN_BOTTOM: {
          //child->MRect.y = parent_rect.y2() - child->MRect.h;
          child->MRect.y = client_rect.y2() - child->MRect.h;
          //client_rect.y += child->MRect.h;
          client_rect.h -= child->MRect.h;
          break;
        }
      }
      
      // stretching

      uint32_t child_stretching   = child->getStretching();
      if (child_stretching & SAT_WIDGET_STRETCH_LEFT)      child->MRect.setX1( client_rect.x );
      if (child_stretching & SAT_WIDGET_STRETCH_RIGHT)     child->MRect.setX2( client_rect.x2() );
      if (child_stretching & SAT_WIDGET_STRETCH_TOP)       child->MRect.setY1( client_rect.y );
      if (child_stretching & SAT_WIDGET_STRETCH_BOTTOM)    child->MRect.setY2( client_rect.y2() );
      
      // recursive
      
      //content_rect.combine(child->MRect);
      MContentRect.combine(child->MRect);
      
      if (ARecursive) {
        child->realignChildWidgets(ARecursive);
      }
      
    } // for
    
    //MContntRect = content_rect;
    
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
        if (widget->isActive() /*&& widget->isVisible()*/) {
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
    SAT_Painter* painter= AContext->painter;
    uint32_t num = MChildren.size();
    if (num > 0) {
      for (uint32_t i=0; i<num; i++) {
        SAT_Widget* widget = MChildren[i];
        if (widget->isVisible()) {
          SAT_Rect widgetrect = widget->getRect();
          if (widgetrect.isNotEmpty()) {
            
            painter->pushClip(getRect());
            widget->on_widget_paint(AContext);
            painter->popClip();
            
          } // intersect
        } // visible
      } // for
    } // num > 0
  }

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

  virtual void on_widget_timer(double ADelta) {
  }

  virtual void on_widget_tween(uint32_t AId, uint32_t ACount, double* AData) {
    switch (AId) {
      case 666: {
      //SAT_Print("id %i count %i data[0] %.2f data[1] %.2f data[2] %.2f data[3] %.2f\n",AId,ACount,AData[0],AData[1],AData[2],AData[3]);
        double S = getWindowScale();
        SAT_Rect R = { AData[0], AData[1], AData[2], AData[3] };
        setBasisRect(R);
        SAT_Rect R2 = R;
        R2.scale(S);
        setRect(R2);
        //R.scale(1.0 / S);
        
        //do_widget_notify(this,SAT_WIDGET_NOTIFY_MOVED,0);
        parentNotify(SAT_WIDGET_NOTIFY_REALIGN,0);
        break;
      }
    }
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

  //----------

  void do_widget_notify(SAT_Widget* ASender, uint32_t AReason, int32_t AValue) override {
    switch(AReason) {
      case SAT_WIDGET_NOTIFY_CLOSE:
        realignChildWidgets(true);
        parentRedraw();
        break;
      case SAT_WIDGET_NOTIFY_REALIGN:
        //SAT_Print("%s\n",getName());
        realignChildWidgets(true);
        parentRedraw();
        break;
      default:
        //SAT_PRINT;
        //SAT_Widget::do_widget_notify(ASender,AReason,AValue);
        if (MListener) MListener->do_widget_notify(ASender,AReason,AValue);
        break;
    }
  }
  
  //----------

  void do_widget_resized(SAT_Widget* ASender, double ADeltaX, double ADeltaY) override {
    SAT_Rect mrect = getRect();
    double S = getWindowScale();
    mrect.w += ADeltaX;
    mrect.h += ADeltaY;
    setRect(mrect);
    mrect.scale(1.0/S);
    setBasisRect(mrect);
    //realignChildWidgets(true);
    //parentRedraw();
  }

};

//----------------------------------------------------------------------
#endif

