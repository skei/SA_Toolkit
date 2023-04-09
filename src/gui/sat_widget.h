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

  SAT_Rect              MRect                               = {};
  SAT_Rect              MBasisRect                          = {};
  SAT_Rect              MInitialRect                        = {};
  SAT_Rect              MContentRect                        = {};

  SAT_WidgetOwner*      MOwner                              = nullptr;
  SAT_WidgetListener*   MListener                           = nullptr;
  SAT_Widget*           MParentWidget                       = nullptr;
  SAT_WidgetArray       MChildWidgets                       = {};
  int32_t               MIndex                              = -1;
  int32_t               MLastPainted                        = -1;
  
  
  uint32_t              MAlignment                          = SAT_WIDGET_ALIGN_PARENT;
  uint32_t              MStretching                         = SAT_WIDGET_STRETCH_NONE;
  SAT_Point             MSpacing                            = SAT_Point();
  SAT_Rect              MInnerBorder                        = SAT_Rect();
  bool                  MOccupyClient                       = true;
  
  const char*           MName                               = "";
  const char*           MHint                               = "";
  int32_t               MMouseCursor                        = 0;
  double                MValues[SAT_WIDGET_MAX_VALUES]      = {0};
  double                MModulations[SAT_WIDGET_MAX_VALUES] = {0};
  void*                 MConnections[SAT_WIDGET_MAX_VALUES] = {0};
  
  bool                  MAutoClip                           = true;
  bool                  MAutoHoverCursor                    = true;
  bool                  MAutoHoverHint                      = true;
  
  bool                  MIsActive                           = true;
  bool                  MIsVisible                          = true;
  bool                  MIsDisabled                         = false;
  bool                  MIsInteracting                      = false;
  
  //
  
  
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

  virtual bool                autoClip()                        { return MAutoClip; }
  virtual bool                autoHoverCursor()                 { return MAutoHoverCursor; }
  virtual bool                autoHoverHint()                   { return MAutoHoverHint; }

  virtual uint32_t            getAlignment()                    { return MAlignment; }
  virtual SAT_Rect            getBasisRect()                    { return MBasisRect; }
  virtual SAT_Widget*         getChildWidget(uint32_t AIndex)   { return MChildWidgets[AIndex]; }
  virtual SAT_WidgetArray*    getChildWidgets()                 { return &MChildWidgets; }
  virtual void*               getConnection(uint32_t AIndex=0)  { return MConnections[AIndex]; }
  virtual SAT_Rect            getContentRect()                  { return MContentRect; }
  virtual double              getHeight()                       { return MRect.h; }
  virtual const char*         getHint()                         { return MHint; }
  virtual int32_t             getIndex()                        { return MIndex; }
  virtual SAT_Rect            getInitialRect()                  { return MInitialRect; }
  virtual int32_t             getLastPainted()                  { return MLastPainted; }
  virtual SAT_WidgetListener* getListener()                     { return MListener; }
  virtual double              getModulation(uint32_t AIndex=0)  { return MModulations[AIndex]; }
  virtual int32_t             getMouseCursor()                  { return MMouseCursor; }
  virtual const char*         getName()                         { return MName; }
  virtual uint32_t            getNumChildWidgets()              { return MChildWidgets.size(); }
  virtual SAT_WidgetOwner*    getOwner()                        { return MOwner; }
  virtual SAT_Widget*         getParentWidget()                 { return MParentWidget; }
  virtual SAT_Rect            getRect()                         { return MRect; }
  virtual uint32_t            getStretching()                   { return MStretching; }
  virtual double              getValue(uint32_t AIndex=0)       { return MValues[AIndex]; }
  virtual double              getWidth()                        { return MRect.w; }
  
  virtual bool                isActive()                        { return MIsActive; }
  virtual bool                isVisible()                       { return MIsVisible; }
  virtual bool                isDisabled()                      { return MIsDisabled; }
  virtual bool                isInteracting()                   { return MIsInteracting; }

//------------------------------
public:
//------------------------------

//virtual void setActive(bool AState=true)                      { MIsActive = AState; }
  virtual void setAlignment(uint32_t AAlignment)                { MAlignment = AAlignment; }
  virtual void setAutoClip(bool AState=true)                    { MAutoClip = AState; }
  virtual void setAutoHoverCursor(bool AState=true)             { MAutoHoverCursor = AState; }
  virtual void setAutoHoverHint(bool AState=true)               { MAutoHoverHint = AState; }
  virtual void setBasisHeight(double AHeight)                   { MBasisRect.h = AHeight; } /*scale*/
  virtual void setBasisRect(SAT_Rect ARect)                     { MBasisRect = ARect; }     /*scale*/
  virtual void setBasisWidth(double AWidth)                     { MBasisRect.w = AWidth; }  /*scale*/
  virtual void setCursor(int32_t ACursor)                       { MMouseCursor = ACursor; }
  virtual void setDisabled(bool AState=true)                    { MIsDisabled = AState; }
  virtual void setHeight(double AHeight)                        { MRect.h = AHeight; }
  virtual void setIndex(int32_t AIndex)                         { MIndex = AIndex; }
  virtual void setInnerBorder(SAT_Rect ARect)                   { MInnerBorder = ARect; }
  virtual void setInteracting(bool AState=true)                 { MIsInteracting = AState; }
  virtual void setLastPainted(int32_t ACount)                   { MLastPainted = ACount; }
  virtual void setListener(SAT_WidgetListener* AListener)       { MListener = AListener; }
  virtual void setModulation(double AValue, uint32_t AIndex=0)  { MValues[AIndex] = AValue; }
  virtual void setName(const char* AName)                       { MName = AName; }
  virtual void setParentWidget(SAT_Widget* AParent)             { MParentWidget = AParent; }
  virtual void setRect(SAT_Rect ARect)                          { MRect = ARect; }
//virtual void setRectAndBasis(SAT_Rect ARect)                  { MRect = ARect; MBasisRect = ARect; } /*scale*/
  virtual void setSize(double AWidth, double AHeight)           { MRect.w = AWidth; MRect.h = AHeight; }
  virtual void setSpacing(SAT_Point ASpacing)                   { MSpacing = ASpacing; }
  virtual void setStretching(uint32_t AStretching)              { MStretching = AStretching; }
  virtual void setValue(double AValue, uint32_t AIndex=0)       { MValues[AIndex] = AValue; }
//virtual void setVisible(bool AState=true)                     { MIsVisible = AState; }
  virtual void setWidth(double AWidth)                          { MRect.w = AWidth; }
  
//------------------------------
public: // hierarchy
//------------------------------

  virtual SAT_Widget* appendChildWidget(SAT_Widget* AWidget, SAT_WidgetListener* AListener=nullptr) {
    int32_t index = MChildWidgets.size();
    AWidget->setParentWidget(this);
    if (AListener) AWidget->setListener(AListener);
    else AWidget->setListener(this);
    AWidget->setIndex(index);
    MChildWidgets.append(AWidget);
    return AWidget;
  }

  //----------
  
  virtual void deleteChildWidgets(){
    for (uint32_t i=0; i<MChildWidgets.size(); i++) {
      if (MChildWidgets[i]) delete MChildWidgets[i];
    }
    MChildWidgets.clear();
  }

  //----------
  
  virtual double getWindowWidth() {
    if (MOwner) return MOwner->do_widgetOwner_get_width();
    else return 0.0;
  }
  
  virtual double getWindowHeight() {
    if (MOwner) return MOwner->do_widgetOwner_get_height();
    else return 0.0;
  }
  
  virtual double getWindowScale() {
    if (MOwner) return MOwner->do_widgetOwner_get_scale();
    else return 1.0;
  }
  
  //----------
  
  virtual void setLayout(uint32_t AAlign, uint32_t AStretch) {
    MAlignment = AAlign;
    MStretching = AStretch;
  }
  
  //----------
  
  virtual void connect(void* AParameter, uint32_t AIndex=0) {
    MConnections[AIndex] = AParameter;
  }

  //----------
  
  virtual void prepare(SAT_WidgetOwner* AOwner) {
    on_widget_prepare();
    MOwner = AOwner;
    for (uint32_t i=0; i<MChildWidgets.size(); i++) {
      MChildWidgets[i]->setParentWidget(this);
      MChildWidgets[i]->prepare(MOwner);
    }
  }

  //----------
  
  virtual void cleanup(SAT_Painter* APainter) {
    on_widget_cleanup();
    uint32_t num = MChildWidgets.size();
    for (uint32_t i=0; i<num; i++) {
      MChildWidgets[i]->cleanup(APainter);
    }
  }
  
  //----------
  
  virtual void setActive(bool AState=true, bool ARecursive=true) {
    MIsActive = AState;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildWidgets.size(); i++) {
        MChildWidgets[i]->setActive(AState,ARecursive);
      }
    }
  }

  virtual void setVisible(bool AState=true, bool ARecursive=true) {
    MIsVisible = AState;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildWidgets.size(); i++) {
        MChildWidgets[i]->setVisible(AState,ARecursive);
      }
    }
  }

  virtual void setRectAndBasis(SAT_Rect ARect) {
    double S = getWindowScale();
    setRect(ARect);
    ARect.scale(1.0 / S);
    setBasisRect(ARect);
  }
  
  //----------
  
  virtual void scrollChildWidgets(double AOffsetX, double AOffsetY) {
    uint32_t num = MChildWidgets.size();
    for (uint32_t i=0; i<num; i++) {
      SAT_Widget* child = MChildWidgets[i];
      if (child->MIsVisible) {
        //child->setChildrenOffset(AOffsetX,AOffsetY);
        child->MRect.x += AOffsetX;
        child->MRect.y += AOffsetY;
        child->scrollChildWidgets(AOffsetX,AOffsetY);
      }
    }
  }
  
  //----------
  
  virtual void scaleWidget(double AScale, bool ARecursive=true) {
    //MRect.scale(AScale);
    MRect = MBasisRect;
    MRect.scale(AScale);
    if (ARecursive) {
      for (uint32_t i=0; i<MChildWidgets.size(); i++) {
        MChildWidgets[i]->scaleWidget(AScale);
      }
    }
  }

  //----------
  
  virtual bool isRecursivelyVisible() {
    if (!MIsVisible) return false;
    if (!MParentWidget) return true;
    return MParentWidget->isRecursivelyVisible();
  }
  
  //----------

  virtual SAT_Widget* findChildWidget(double AXpos, double AYpos, bool ARecursive=true) {
    if (!MRect.contains(AXpos,AYpos)) return nullptr;
    if (!isActive()) return nullptr;
    uint32_t num = MChildWidgets.size();
    if (num > 0) {
      for (int32_t i=num-1; i>=0; i--) {
        SAT_Widget* widget = MChildWidgets[i];
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
  
  virtual void paintChildWidgets(SAT_PaintContext* AContext) {
    SAT_Rect mrect = getRect();
    SAT_Painter* painter= AContext->painter;
    uint32_t num = MChildWidgets.size();
    //SAT_Print("num children: %i\n",num);
    if (num > 0) {
      if (MAutoClip) painter->pushOverlappingClip(mrect);
      for (uint32_t i=0; i<num; i++) {
        SAT_Widget* widget = MChildWidgets[i];
        //if (widget->isRecursivelyVisible()) {
        if (widget->isVisible()) {
          SAT_Rect wr = widget->getRect();
          wr.overlap(mrect);
          if (wr.isNotEmpty()) {
            widget->on_widget_paint(AContext);
          } // not empty
        } // visible
      } // for
      if (MAutoClip) painter->popClip();
    } // num > 0
  }
  
  //----------
  
  virtual void realignChildWidgets() {
    double S = getWindowScale();
    SAT_Rect parent_rect = getRect();

    MContentRect = parent_rect;
    //MContentRect.w = 0;
    //MContentRect.h = 0;

    SAT_Rect client_rect = parent_rect;
    
    SAT_Rect innerborder = MInnerBorder;
    innerborder.scale(S);
    client_rect.shrink(innerborder);

    SAT_Point spacing = MSpacing;
    spacing.scale(S);

    for (uint32_t i=0; i<MChildWidgets.size(); i++) {

      SAT_Widget* child = MChildWidgets[i];
      SAT_Rect child_basisrect = child->getBasisRect();
      child_basisrect.scale(S);

      // assume aligned relative to client
      
      child->MRect.x = client_rect.x + child_basisrect.x;
      child->MRect.y = client_rect.y + child_basisrect.y;
      
      // negative = percent
      
      SAT_Rect child_initialrect = child->getInitialRect();
      if (child_initialrect.x < 0) child->MRect.x = client_rect.w * (fabs(child_initialrect.x) * 0.01);
      if (child_initialrect.y < 0) child->MRect.y = client_rect.h * (fabs(child_initialrect.y) * 0.01);
      if (child_initialrect.w < 0) child->MRect.w = client_rect.w * (fabs(child_initialrect.w) * 0.01);
      if (child_initialrect.h < 0) child->MRect.h = client_rect.h * (fabs(child_initialrect.h) * 0.01);

      // alignment

      uint32_t child_alignment = child->getAlignment();
      switch(child_alignment) {
        
        case SAT_WIDGET_ALIGN_NONE: {
          child->MRect.x -= client_rect.x;
          child->MRect.y -= client_rect.y;
          break;
        }
        
        case SAT_WIDGET_ALIGN_PARENT: {
          // undo client aligning (client -> parent)
          child->MRect.x -= client_rect.x;
          child->MRect.y -= client_rect.y;
          child->MRect.x += parent_rect.x;
          child->MRect.y += parent_rect.y;
          break;
        }

        case SAT_WIDGET_ALIGN_CENTER: {
          break;
        }
        
        case SAT_WIDGET_ALIGN_HCENTER: {
          break;
        }
        
        case SAT_WIDGET_ALIGN_VCENTER: {
          break;
        }
        
        case SAT_WIDGET_ALIGN_LEFT: {
          child->MRect.x = client_rect.x;
          if (MOccupyClient) {
            client_rect.x += (child->MRect.w + spacing.x);
            client_rect.w -= (child->MRect.w + spacing.x);
          }
          break;
        }
        
        case SAT_WIDGET_ALIGN_LEFT_TOP: {
          child->MRect.x = client_rect.x;
          child->MRect.y = client_rect.y;
          if (MOccupyClient) {
          }
          break;
        }
        
        case SAT_WIDGET_ALIGN_LEFT_CENTER: {
          child->MRect.x = client_rect.x;
          child->MRect.y = (client_rect.y + child->MRect.h) * 0.5;
          if (MOccupyClient) {
          }
          break;
        }
        
        case SAT_WIDGET_ALIGN_LEFT_BOTTOM: {
          child->MRect.x = client_rect.x;
          child->MRect.y = client_rect.y2() - child->MRect.h;
          if (MOccupyClient) {
          }
          break;
        }
        
        case SAT_WIDGET_ALIGN_RIGHT: {
          child->MRect.x = client_rect.x2() - child->MRect.w;
          client_rect.w -= (child->MRect.w + spacing.x);
          if (MOccupyClient) {
          }
          break;
        }
        
        case SAT_WIDGET_ALIGN_RIGHT_TOP: {
          child->MRect.x = client_rect.x2() - child->MRect.w;
          child->MRect.y = client_rect.y;
          if (MOccupyClient) {
          }
          break;
        }
        
        case SAT_WIDGET_ALIGN_RIGHT_CENTER: {
          child->MRect.x = client_rect.x2() - child->MRect.w;
          child->MRect.y = (client_rect.y + child->MRect.h) * 0.5;
          if (MOccupyClient) {
          }
          break;
        }
        
        case SAT_WIDGET_ALIGN_RIGHT_BOTTOM: {
          child->MRect.x = client_rect.x2() - child->MRect.w;
          child->MRect.y = client_rect.y2() - child->MRect.h;
          if (MOccupyClient) {
          }
          break;
        }
        
        case SAT_WIDGET_ALIGN_TOP: {
          child->MRect.y = client_rect.y;
          if (MOccupyClient) {
            client_rect.y += (child->MRect.h + spacing.y);
            client_rect.h -= (child->MRect.h + spacing.y);
          }
          break;
        }
        
        case SAT_WIDGET_ALIGN_TOP_LEFT: {
          break;
        }

        case SAT_WIDGET_ALIGN_TOP_CENTER: {
          break;
        }

        case SAT_WIDGET_ALIGN_TOP_RIGHT: {
          break;
        }

        case SAT_WIDGET_ALIGN_BOTTOM: {
          child->MRect.y = client_rect.y2() - child->MRect.h;
          client_rect.h -= (child->MRect.h + spacing.y);
          if (MOccupyClient) {
          }
          break;
        }
        
        case SAT_WIDGET_ALIGN_BOTTOM_LEFT: {
          break;
        }

        case SAT_WIDGET_ALIGN_BOTTOM_CENTER: {
          break;
        }

        case SAT_WIDGET_ALIGN_BOTTOM_RIGHT: {
          break;
        }

      }
      
      // stretching

      uint32_t child_stretching = child->getStretching();
      if (child_stretching & SAT_WIDGET_STRETCH_LEFT)      child->MRect.setX1( client_rect.x );
      if (child_stretching & SAT_WIDGET_STRETCH_RIGHT)     child->MRect.setX2( client_rect.x2() );
      if (child_stretching & SAT_WIDGET_STRETCH_TOP)       child->MRect.setY1( client_rect.y );
      if (child_stretching & SAT_WIDGET_STRETCH_BOTTOM)    child->MRect.setY2( client_rect.y2() );
      
      // recursive

      MContentRect.combine(child->MRect);
      
      child->realignChildWidgets();
      
    } // for
    
    // add w if not auto-aligning?
    //MContentRect.w += innerborder.w;
    MContentRect.h += innerborder.h;
    
  }

//------------------------------
public:
//------------------------------

  virtual void parentUpdate() {
    if (MListener) MListener->do_widgetListener_update(this,0,0);
  }

  //----------

  virtual void parentRealign() {
    if (MListener) MListener->do_widgetListener_realign(this);
  }

  //----------

  virtual void parentRealignParent() {
    if (MListener) MListener->do_widgetListener_realign(nullptr);
  }

  //----------

  virtual void parentRedraw() {
    if (MListener) MListener->do_widgetListener_redraw(this,0,0);
  }

  //----------

  virtual void parentRedrawAll() {
    if (MListener) MListener->do_widgetListener_redraw(nullptr,0,0);
  }

//------------------------------
public: // base
//------------------------------

  virtual void on_widget_prepare() {
  }
  
  virtual void on_widget_cleanup() {
  }
  
  virtual void on_widget_move(double AXpos, double AYpos) {
  }
  
  virtual void on_widget_resize(double AWidth, double AHeight) {
  }
  
  virtual void on_widget_paint(SAT_PaintContext* AContext) {
    if (MIsVisible) paintChildWidgets(AContext);
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
    if (MIsActive && MIsVisible) {
      if (autoHoverCursor()) do_widgetListener_set_cursor(this,MMouseCursor);
      if (autoHoverHint()) do_widgetListener_set_hint(this,0,MHint);
    }
  }

  virtual void on_widget_mouse_leave(SAT_Widget* ATo, double AXpos, double AYpos, uint32_t ATimestamp) {
    if (MIsActive && MIsVisible) {
      if (autoHoverCursor()) do_widgetListener_set_cursor(this,SAT_CURSOR_DEFAULT);
      if (autoHoverHint()) do_widgetListener_set_hint(this,0,"");
    }
  }

  virtual void on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ACount, double* AValue) {
    //switch (AType) {
    //  case SAT_WIDGET_TWEEN_RECT/*666*/: {
    //  //SAT_Print("id %i count %i data[0] %.2f data[1] %.2f data[2] %.2f data[3] %.2f\n",AId,ACount,AData[0],AData[1],AData[2],AData[3]);
    //    double S = getWindowScale();
    //    SAT_Rect R = { AData[0], AData[1], AData[2], AData[3] };
    //    setBasisRect(R);
    //    SAT_Rect R2 = R;
    //    R2.scale(S);
    //    setRect(R2);
    //    //R.scale(1.0 / S);
    //    //do_widgetListener_notify(this,SAT_WIDGET_NOTIFY_MOVED,0);
    //    parentNotify(SAT_WIDGET_NOTIFY_REALIGN,0);
    //    break;
    //  }
    //}
  }

//------------------------------
public: // widget listener
//------------------------------

  void do_widgetListener_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (MListener) MListener->do_widgetListener_update(ASender,AMode,AIndex);
  }

  //----------

  void do_widgetListener_redraw(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (MListener) MListener->do_widgetListener_redraw(ASender,AMode,AIndex);
  }

  //----------

  void do_widgetListener_realign(SAT_Widget* ASender) override {
    if (ASender) {
      realignChildWidgets();
      do_widgetListener_redraw(this,0);
    }
    else {
      if (MListener) MListener->do_widgetListener_realign(this);
    }
  }

  //----------

  void do_widgetListener_close(SAT_Widget* ASender) override {
    if (MListener) MListener->do_widgetListener_close(ASender);
  }

  //----------

  void do_widgetListener_select(SAT_Widget* ASender, int32_t AIndex) override {
    if (MListener) MListener->do_widgetListener_select(ASender,AIndex);
  }

  //----------

  void do_widgetListener_set_cursor(SAT_Widget* ASender, int32_t ACursor) override {
    if (MListener) MListener->do_widgetListener_set_cursor(ASender,ACursor);
  }

  //----------

  void do_widgetListener_set_hint(SAT_Widget* ASender, uint32_t AMode, const char* AHint) override {
    if (MListener) MListener->do_widgetListener_set_hint(ASender,AMode,AHint);
  }

  //----------
  
  void do_widgetListener_set_modal(SAT_Widget* ASender) override {
    if (MListener) MListener->do_widgetListener_set_modal(ASender);
  }
  
  //----------

  void do_widgetListener_resized(SAT_Widget* ASender, double ADeltaX, double ADeltaY) override {
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

  //----------

  //  void do_widgetListener_notify(SAT_Widget* ASender, uint32_t AReason, int32_t AValue) override {
  //    if (MListener) MListener->do_widgetListener_notify(ASender,AReason,AValue);
  //  }

  //void do_widgetListener_notify(SAT_Widget* ASender, uint32_t AReason, int32_t AValue) override {
  //  switch(AReason) {
  //    case SAT_WIDGET_NOTIFY_CLOSE:
  //      //SAT_PRINT;
  //      //realignChildWidgets(true);
  //      //parentRedraw();
  //      break;
  //    case SAT_WIDGET_NOTIFY_REALIGN:
  //      //SAT_Print("this: %s ASender: %s\n",getName(),ASender->getName());
  //      realignChildWidgets(true);
  //      parentRedraw();
  //      break;
  //    default:
  //      //SAT_PRINT;
  //      //realignChildWidgets(true);
  //      //parentRedraw();
  //      break;
  //  }
  //  if (MListener) MListener->do_widgetListener_notify(ASender,AReason,AValue);
  //}

};

//----------------------------------------------------------------------
#endif
