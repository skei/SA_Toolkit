#ifndef sat_widget_included
#define sat_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_base_widget.h"
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
: public SAT_BaseWidget
, public SAT_WidgetListener {

//------------------------------
private:
//------------------------------

  SAT_Rect              MInitialRect                        = {}; // from constructor (unscaled, un-abs)
  SAT_Rect              MBasisRect                          = {}; // basis for alignment/layout/scaling
  SAT_Rect              MRect                               = {}; // current screen rect (scaled, abs)
  SAT_Rect              MContentRect                        = {}; // child-widgets
  
  // hierarchy

  SAT_WidgetOwner*      MOwner                              = nullptr;  // aka SAT_Window
  SAT_WidgetListener*   MListener                           = nullptr;  // event listener (usually parent)
  SAT_Widget*           MParentWidget                       = nullptr;  // parent in hierarchy
  SAT_WidgetArray       MChildWidgets                       = {};       // children
  int32_t               MIndex                              = -1;       // index in parent
  int32_t               MLastPainted                        = -1;       // paint-counter of last painting (SAT_PaintContext.counter)
  
  // alignment
  
  uint32_t              MAlignment                          = SAT_WIDGET_ALIGN_DEFAULT; //PARENT;
  uint32_t              MStretching                         = SAT_WIDGET_STRETCH_NONE;  // stretch edges to layout rect
  SAT_Point             MSpacing                            = SAT_Point();              // between widgets
  SAT_Rect              MInnerBorder                        = SAT_Rect();               // border around child widgets
  SAT_Rect              MOuterBorder                        = SAT_Rect();               // additional border (after alignment)
  bool                  MFillLayout                         = true;                     // remove 'occupied' sdpace from layout rect when aligning
  double                MMinWidth                           = -1;                       // resizing limits
  double                MMaxWidth                           = -1;
  double                MMinHeight                          = -1;
  double                MMaxHeight                          = -1;
  
  //
  
  const char*           MName                               = "";
  const char*           MHint                               = "";
  int32_t               MMouseCursor                        = 0;
  uint32_t              MSelectedValue                      = 0;
  double                MValues[SAT_WIDGET_MAX_VALUES]      = {0};
  double                MModulations[SAT_WIDGET_MAX_VALUES] = {0};
  void*                 MParameters[SAT_WIDGET_MAX_VALUES]  = {0};    // connected parameters
  uint32_t              MNumParameters                      = 1;
  
  bool                  MAutoClip                           = true;   // automatically set clipping before drawing
  bool                  MAutoHoverCursor                    = true;   // automatically set cursor when hovering over widget
  bool                  MAutoHoverHint                      = true;   // automatically send hint text when hovering
  
  bool                  MIsActive                           = true;   // is active (receive events)
  bool                  MIsVisible                          = true;   // is visible (will be painted/realigned)
  bool                  MIsDisabled                         = false;  // not interactable
  bool                  MIsInteracting                      = false;  // is currently being interacted with
  
  //
  
  
//------------------------------
public:
//------------------------------

  SAT_Widget(SAT_Rect ARect)
  : SAT_BaseWidget(ARect) {
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
  virtual void*               getParameter(uint32_t AIndex=0)   { return MParameters[AIndex]; }
  virtual SAT_Rect            getContentRect()                  { return MContentRect; }
  virtual double              getHeight()                       { return MRect.h; }
  virtual const char*         getHint()                         { return MHint; }
  virtual int32_t             getIndex()                        { return MIndex; }
  virtual SAT_Rect            getInitialRect()                  { return MInitialRect; }
  virtual SAT_Rect            getInnerBorder()                  { return MInnerBorder; }
  virtual int32_t             getLastPainted()                  { return MLastPainted; }
  virtual SAT_WidgetListener* getListener()                     { return MListener; }
  virtual double              getModulation(uint32_t AIndex=0)  { return MModulations[AIndex]; }
  virtual int32_t             getMouseCursor()                  { return MMouseCursor; }
  virtual const char*         getName()                         { return MName; }
  virtual uint32_t            getNumChildWidgets()              { return MChildWidgets.size(); }
  virtual uint32_t            getNumParameters()                { return MNumParameters; }
  virtual SAT_WidgetOwner*    getOwner()                        { return MOwner; }
  virtual SAT_Widget*         getParentWidget()                 { return MParentWidget; }
  virtual SAT_Rect            getRect()                         { return MRect; }
  virtual double              getSelectedModulation()           { return MModulations[MSelectedValue]; }
  virtual double              getSelectedValue()                { return MValues[MSelectedValue]; }
  virtual uint32_t            getSelectedValueIndex()           { return MSelectedValue; }
  virtual uint32_t            getStretching()                   { return MStretching; }
  virtual double              getValue(uint32_t AIndex=0)       { return MValues[AIndex]; }
  virtual double              getWidth()                        { return MRect.w; }
  
  virtual bool isActive() {
    return MIsActive;
  }
  
  virtual bool isVisible() {
    return MIsVisible;
  }
  
  virtual bool isDisabled() {
    return MIsDisabled;
  }
  
  virtual bool isInteracting() {
    return MIsInteracting;
  }

  virtual bool isRecursivelyActive() {
    if (!MIsActive) return false;
    if (!MParentWidget) return true;
    return MParentWidget->isRecursivelyActive();
  }

  virtual bool isRecursivelyVisible() {
    //SAT_PRINT;
    if (!MIsVisible) return false;
    if (!MParentWidget) return true;
    return MParentWidget->isRecursivelyVisible();
  }

//------------------------------
public:
//------------------------------

  virtual void setAlignment(uint32_t AAlignment)                { MAlignment = AAlignment; }
  virtual void setAutoClip(bool AState=true)                    { MAutoClip = AState; }
  virtual void setAutoHoverCursor(bool AState=true)             { MAutoHoverCursor = AState; }
  virtual void setAutoHoverHint(bool AState=true)               { MAutoHoverHint = AState; }
  virtual void setBasisHeight(double AHeight)                   { MBasisRect.h = AHeight; }  /* scale? */
  virtual void setBasisRect(SAT_Rect ARect)                     { MBasisRect = ARect; }      /* scale? */
  virtual void setBasisWidth(double AWidth)                     { MBasisRect.w = AWidth; }   /* scale? */
  virtual void setCursor(int32_t ACursor)                       { MMouseCursor = ACursor; }
  virtual void setDisabled(bool AState=true)                    { MIsDisabled = AState; }
  virtual void setHeight(double AHeight)                        { MRect.h = AHeight; }
  virtual void setIndex(int32_t AIndex)                         { MIndex = AIndex; }
  virtual void setInnerBorder(SAT_Rect ARect)                   { MInnerBorder = ARect; }
  //virtual void setInnerBorder(double b)                         { MInnerBorder = SAT_Rect(b,b,b,b); }
  //virtual void setInnerBorder(double w, double h)               { MInnerBorder = SAT_Rect(w,h,w,h); }
  virtual void setInteracting(bool AState=true)                 { MIsInteracting = AState; }
  virtual void setLastPainted(int32_t ACount)                   { MLastPainted = ACount; }
  virtual void setListener(SAT_WidgetListener* AListener)       { MListener = AListener; }
  virtual void setModulation(double AValue, uint32_t AIndex=0)  { MModulations[AIndex] = AValue; }
  virtual void setName(const char* AName)                       { MName = AName; }
  virtual void setOuterBorder(SAT_Rect ARect)                   { MOuterBorder = ARect; }
  virtual void setParentWidget(SAT_Widget* AParent)             { MParentWidget = AParent; }
  virtual void setRect(SAT_Rect ARect)                          { MRect = ARect; }
  virtual void setSize(double AWidth, double AHeight)           { MRect.w = AWidth; MRect.h = AHeight; }
  virtual void setSpacing(SAT_Point ASpacing)                   { MSpacing = ASpacing; }
  virtual void setStretching(uint32_t AStretching)              { MStretching = AStretching; }
  virtual void setValue(double AValue, uint32_t AIndex=0)       { MValues[AIndex] = AValue; }
  virtual void setWidth(double AWidth)                          { MRect.w = AWidth; }
  
  virtual void setMinWidth(double AValue)   { MMinWidth = AValue; }
  virtual void setMinHeight(double AValue)  { MMinHeight = AValue; }
  virtual void setMaxWidth(double AValue)   { MMaxWidth = AValue; }
  virtual void setMaxHeight(double AValue)  { MMaxHeight = AValue; }
  
  virtual void setSizeLimits(double AMinW, double AMinH, double AMaxW, double AMaxH) {
    MMinWidth  = AMinW;
    MMinHeight = AMinH;
    MMaxWidth  = AMaxW;
    MMaxHeight = AMaxH;
  }
  
  virtual void setWidthLimits(double AMinW, double AMaxW) {
    MMinWidth  = AMinW;
    MMaxWidth  = AMaxW;
  }
  
  virtual void setHeightLimits(double AMinH, double AMaxH) {
    MMinHeight = AMinH;
    MMaxHeight = AMaxH;
  }
  
  virtual void setSelectedModulation(double AValue) {
    MModulations[MSelectedValue] = AValue;
  }

  virtual void setSelectedValue(double AValue) {
    MValues[MSelectedValue] = AValue;
  }

  virtual void selectValue(uint32_t AIndex) {
    MSelectedValue = AIndex;
  }

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
    MParameters[AIndex] = AParameter;
    if ((AIndex + 1) > MNumParameters) {
      MNumParameters = AIndex + 1;
    }
  }

  //----------
  
  /*
    called from:
      SAT_Window.on_window_open (before realign)
  */
  
  virtual void prepare(SAT_WidgetOwner* AOwner) {
    MOwner = AOwner;
    on_widget_prepare();
    //MOwner = AOwner;
    for (uint32_t i=0; i<MChildWidgets.size(); i++) {
      MChildWidgets[i]->setParentWidget(this);
      MChildWidgets[i]->prepare(MOwner);
    }
  }

  //----------
  
  /*
    called from:
      SAT_Window destructor (-> on_window_close?)
  */
  
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

  //----------
  
  virtual void setVisible(bool AState=true, bool ARecursive=true) {
    MIsVisible = AState;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildWidgets.size(); i++) {
        MChildWidgets[i]->setVisible(AState,ARecursive);
      }
    }
  }
  
  //----------
  
  /*
    called from:
      SAT_PopupWidget.open() to reposition the menu
      SAT_MovableWidget when dragging
  */

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
      if (child->isVisible()) {
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
  
  //private:
  //
  //  void stretch(SAT_Widget* child, SAT_Rect layout_rect) {
  //    uint32_t child_stretching = child->getStretching();
  //    if (child_stretching & SAT_WIDGET_STRETCH_LEFT)      child->MRect.setX1( layout_rect.x );
  //    if (child_stretching & SAT_WIDGET_STRETCH_RIGHT)     child->MRect.setX2( layout_rect.x2() );
  //    if (child_stretching & SAT_WIDGET_STRETCH_TOP)       child->MRect.setY1( layout_rect.y );
  //    if (child_stretching & SAT_WIDGET_STRETCH_BOTTOM)    child->MRect.setY2( layout_rect.y2() );
  //  }
  //  
  //public:
  
  virtual void paintChildWidgets(SAT_PaintContext* AContext) {
    SAT_Rect mrect = getRect();
    SAT_Painter* painter= AContext->painter;
    uint32_t num = MChildWidgets.size();
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
    
//    SAT_Print("MOwner: %p\n",getOwner());
    
    double S = getWindowScale();

    SAT_Rect parent_rect = getRect();
    SAT_Rect layout_rect = parent_rect;
    MContentRect = parent_rect;
    
    //    SAT_Rect outerborder = MOuterBorder;
    //    outerborder.scale(S);
    //    parent_rect.shrink(outerborder);
    //    layout_rect.shrink(outerborder);
    
    // inner border
    
    SAT_Rect innerborder = MInnerBorder;
    innerborder.scale(S);
    layout_rect.shrink(innerborder);
    //parent_rect.shrink(innerborder);
    
    //

    SAT_Point spacing = MSpacing;
    spacing.scale(S);

    for (uint32_t i=0; i<MChildWidgets.size(); i++) {
      SAT_Widget* child = MChildWidgets[i];
      
      //if (child->isVisible()) {

        SAT_Rect child_basisrect = child->getBasisRect();
        child_basisrect.scale(S);

        // assume aligned relative to layout
        
        child->MRect.x = child_basisrect.x + layout_rect.x;
        child->MRect.y = child_basisrect.y + layout_rect.y;
        
        child->MRect.w = child_basisrect.w;
        child->MRect.h = child_basisrect.h;
        
        // negative = percent
        
        SAT_Rect child_initialrect = child->getInitialRect();
        if (child_initialrect.x < 0) child->MRect.x = (fabs(child_initialrect.x) * 0.01) * parent_rect.w;
        if (child_initialrect.y < 0) child->MRect.y = (fabs(child_initialrect.y) * 0.01) * parent_rect.h;
        if (child_initialrect.w < 0) child->MRect.w = (fabs(child_initialrect.w) * 0.01) * parent_rect.w;
        if (child_initialrect.h < 0) child->MRect.h = (fabs(child_initialrect.h) * 0.01) * parent_rect.h;

        // alignment
        
        // remember, for scaling
        SAT_Rect pre_layout_rect = layout_rect;
        
        uint32_t child_alignment = child->getAlignment();
        switch(child_alignment) {
          
          //case SAT_WIDGET_ALIGN_DEFAULT:
          //case SAT_WIDGET_ALIGN_CLIENT:

          case SAT_WIDGET_ALIGN_NONE: {
            // undo parent aligning
            child->MRect.x -= layout_rect.x;
            child->MRect.y -= layout_rect.y;
            break;
          }
          
          case SAT_WIDGET_ALIGN_PARENT: {
            // undo layout aligning (layout -> parent)
            child->MRect.x -= layout_rect.x;
            child->MRect.y -= layout_rect.y;
            child->MRect.x += parent_rect.x;
            child->MRect.y += parent_rect.y;
            break;
          }

          //--------------------
          // center

          case SAT_WIDGET_ALIGN_CENTER: {
            //SAT_Print("unhandled alignment: SAT_WIDGET_ALIGN_CENTER\n");
            //child->MRect.x = (layout_rect.x + child->MRect.w) * 0.5;
            //child->MRect.y = (layout_rect.y + child->MRect.h) * 0.5;
            child->MRect.x = layout_rect.x + (layout_rect.w * 0.5) - (child->MRect.w * 0.5);
            child->MRect.y = layout_rect.y + (layout_rect.h * 0.5) - (child->MRect.h * 0.5);
            break;
          }
          
          case SAT_WIDGET_ALIGN_HCENTER: {
            //SAT_Print("unhandled alignment: SAT_WIDGET_ALIGN_HCENTER\n");
            //child->MRect.x = (layout_rect.x + child->MRect.w) * 0.5;
            child->MRect.x = layout_rect.x + (layout_rect.w * 0.5) - (child->MRect.w * 0.5);
            break;
          }
          
          case SAT_WIDGET_ALIGN_VCENTER: {
            //SAT_Print("unhandled alignment: SAT_WIDGET_ALIGN_VCENTER\n");
            //child->MRect.y = (layout_rect.y + child->MRect.h) * 0.5;
            child->MRect.x = layout_rect.x + (layout_rect.w * 0.5) - (child->MRect.w * 0.5);
            if (MFillLayout) {
            }
            break;
          }
          
          //--------------------
          // fill

          case SAT_WIDGET_ALIGN_FILL: {
            child->MRect = layout_rect;
            if (MFillLayout) {
            }
            break;
          }
          
          case SAT_WIDGET_ALIGN_HFILL: {
            child->MRect.x = layout_rect.x;
            child->MRect.w = layout_rect.w;
            if (MFillLayout) {
            }
            break;
          }
          
          case SAT_WIDGET_ALIGN_VFILL: {
            child->MRect.y = layout_rect.y;
            child->MRect.h = layout_rect.h;
            break;
          }
          
          //--------------------
          // left
          
          case SAT_WIDGET_ALIGN_LEFT: {
            child->MRect.x = layout_rect.x;
            if (MFillLayout) {
              layout_rect.x += (child->MRect.w + spacing.x);
              layout_rect.w -= (child->MRect.w + spacing.x);
            }
            break;
          }
          
          //case SAT_WIDGET_ALIGN_LEFT_FILL: {
          //}

          case SAT_WIDGET_ALIGN_LEFT_TOP: {
            child->MRect.x = layout_rect.x;
            child->MRect.y = layout_rect.y;
            if (MFillLayout) {
              layout_rect.x += (child->MRect.w + spacing.x);
              layout_rect.w -= (child->MRect.w + spacing.x);
            }
            break;
          }
          
          case SAT_WIDGET_ALIGN_LEFT_CENTER: {
            child->MRect.x = layout_rect.x;
            //child->MRect.y = (layout_rect.y + child->MRect.h) * 0.5;
            child->MRect.y = layout_rect.y + (layout_rect.h * 0.5) - (child->MRect.h * 0.5);
            if (MFillLayout) {
              layout_rect.x += (child->MRect.w + spacing.x);
              layout_rect.w -= (child->MRect.w + spacing.x);
            }
            break;
          }
          
          case SAT_WIDGET_ALIGN_LEFT_BOTTOM: {
            child->MRect.x = layout_rect.x;
            child->MRect.y = layout_rect.y2() - child->MRect.h;
            if (MFillLayout) {
              layout_rect.x += (child->MRect.w + spacing.x);
              layout_rect.w -= (child->MRect.w + spacing.x);
            }
            break;
          }
          
          //--------------------
          // right
          
          case SAT_WIDGET_ALIGN_RIGHT: {
            child->MRect.x = layout_rect.x2() - child->MRect.w;
            if (MFillLayout) {
              layout_rect.w -= (child->MRect.w + spacing.x);
            }
            break;
          }
          
          //case SAT_WIDGET_ALIGN_RIGHT_FILL: {
          //}

          case SAT_WIDGET_ALIGN_RIGHT_TOP: {
            child->MRect.x = layout_rect.x2() - child->MRect.w;
            child->MRect.y = layout_rect.y;
            if (MFillLayout) {
              layout_rect.w -= (child->MRect.w + spacing.x);
            }
            break;
          }
          
          case SAT_WIDGET_ALIGN_RIGHT_CENTER: {
            child->MRect.x = layout_rect.x2() - child->MRect.w;
            child->MRect.y = layout_rect.y + (layout_rect.h * 0.5) - (child->MRect.h * 0.5);
            if (MFillLayout) {
              layout_rect.w -= (child->MRect.w + spacing.x);
            }
            break;
          }
          
          case SAT_WIDGET_ALIGN_RIGHT_BOTTOM: {
            child->MRect.x = layout_rect.x2() - child->MRect.w;
            child->MRect.y = layout_rect.y2() - child->MRect.h;
            if (MFillLayout) {
              layout_rect.w -= (child->MRect.w + spacing.x);
            }
            break;
          }
          
          //--------------------
          //top

          case SAT_WIDGET_ALIGN_TOP: {
            child->MRect.y = layout_rect.y;
            if (MFillLayout) {
              layout_rect.y += (child->MRect.h + spacing.y);
              layout_rect.h -= (child->MRect.h + spacing.y);
            }
            break;
          }
          
          //case SAT_WIDGET_ALIGN_TOP_FILL: {
          //}

          case SAT_WIDGET_ALIGN_TOP_LEFT: {
            child->MRect.x = layout_rect.x;
            child->MRect.y = layout_rect.y;
            if (MFillLayout) {
              layout_rect.y += (child->MRect.h + spacing.y);
              layout_rect.h -= (child->MRect.h + spacing.y);
            }
            break;
          }

          case SAT_WIDGET_ALIGN_TOP_CENTER: {
            child->MRect.x = layout_rect.x + (layout_rect.w * 0.5) - (child->MRect.w * 0.5);
            child->MRect.y = layout_rect.y;
            if (MFillLayout) {
              layout_rect.y += (child->MRect.h + spacing.y);
              layout_rect.h -= (child->MRect.h + spacing.y);
            }
            break;
          }

          case SAT_WIDGET_ALIGN_TOP_RIGHT: {
            child->MRect.x = layout_rect.x2() - child->MRect.w;
            child->MRect.y = layout_rect.y;
            if (MFillLayout) {
              layout_rect.y += (child->MRect.h + spacing.y);
              layout_rect.h -= (child->MRect.h + spacing.y);
            }
            break;
          }

          //--------------------
          // bottom

          case SAT_WIDGET_ALIGN_BOTTOM: {
            child->MRect.y = layout_rect.y2() - child->MRect.h;
            if (MFillLayout) {
              layout_rect.h -= (child->MRect.h + spacing.y);
            }
            break;
          }
          
          //case SAT_WIDGET_ALIGN_BOTTOM_FILL: {
          //}

          case SAT_WIDGET_ALIGN_BOTTOM_LEFT: {
            child->MRect.x = layout_rect.x;
            child->MRect.y = layout_rect.y2() - child->MRect.h;
            if (MFillLayout) {
              layout_rect.h -= (child->MRect.h + spacing.y);
            }
            break;
          }

          case SAT_WIDGET_ALIGN_BOTTOM_CENTER: {
            child->MRect.x = layout_rect.x + (layout_rect.w * 0.5) - (child->MRect.w * 0.5);
            child->MRect.y = layout_rect.y2() - child->MRect.h;
            if (MFillLayout) {
              layout_rect.h -= (child->MRect.h + spacing.y);
            }
            break;
          }

          case SAT_WIDGET_ALIGN_BOTTOM_RIGHT: {
            child->MRect.x = layout_rect.x2() - child->MRect.w;
            child->MRect.y = layout_rect.y2() - child->MRect.h;
            if (MFillLayout) {
              layout_rect.h -= (child->MRect.h + spacing.y);
            }
            break;
          }

        } // switch alignment
        
        // stretching
        
        /*
        oops.. layout_rect has probably already been changed if MFillLayout is true..
        */
        
        uint32_t child_stretching = child->getStretching();
        if (child_stretching & SAT_WIDGET_STRETCH_LEFT)      child->MRect.setX1( pre_layout_rect.x );
        if (child_stretching & SAT_WIDGET_STRETCH_RIGHT)     child->MRect.setX2( pre_layout_rect.x2() );
        if (child_stretching & SAT_WIDGET_STRETCH_TOP)       child->MRect.setY1( pre_layout_rect.y );
        if (child_stretching & SAT_WIDGET_STRETCH_BOTTOM)    child->MRect.setY2( pre_layout_rect.y2() );
        
        // outer border (post)
        
        SAT_Rect outer_border = child->MOuterBorder;
        outer_border.scale(S);
        child->MRect.shrink(outer_border);
        
        // min/max
        
        if ((child->MMinWidth  >= 0) && (child->MRect.w < (child->MMinWidth  * S))) child->MRect.w = (child->MMinWidth  * S);
        if ((child->MMinHeight >= 0) && (child->MRect.h < (child->MMinHeight * S))) child->MRect.h = (child->MMinHeight * S);
        if ((child->MMaxWidth  >= 0) && (child->MRect.w > (child->MMaxWidth  * S))) child->MRect.w = (child->MMaxWidth  * S);
        if ((child->MMaxHeight >= 0) && (child->MRect.h > (child->MMaxHeight * S))) child->MRect.h = (child->MMaxHeight * S);

        // recursive

        MContentRect.combine(child->MRect);
        
        child->realignChildWidgets();
      
      //} // visible
      
    } // for
    
    // add w if not auto-aligning?
    MContentRect.w += innerborder.w;
    MContentRect.h += innerborder.h;
    
  }

//------------------------------
public: // (shortcuts)
//------------------------------

  /*
    usually called by other widgets, to make _this_ widget
    call its listener
  */
  
  //----------

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
public: // base widget
//------------------------------

  void on_widget_prepare() override {
  }
  
  void on_widget_cleanup() override {
  }
  
  void on_widget_move(double AXpos, double AYpos) override {
  }
  
  void on_widget_resize(double AWidth, double AHeight) override {
  }
  
  void on_widget_prepaint(SAT_PaintContext* AContext) override {
  }
  
  void on_widget_paint(SAT_PaintContext* AContext) override {
    if (isVisible()) paintChildWidgets(AContext);
  }
  
  void on_widget_postpaint(SAT_PaintContext* AContext) override {
  }
  
  void on_widget_timer(uint32_t AId, double ADelta) override {
  }

  void on_widget_key_press(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATimestamp) override {
  }

  void on_widget_key_release(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATimestamp) override {
  }

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) override {
  }

  void on_widget_mouse_dblclick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) override {
  }

  void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimestamp) override {
  }

  void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATimestamp) override {
  }

  void on_widget_mouse_enter(SAT_BaseWidget* AFrom, double AXpos, double AYpos, uint32_t ATimestamp) override {
    if (isActive() && isVisible()) {
      if (autoHoverCursor()) do_widgetListener_set_cursor(this,MMouseCursor);
      if (autoHoverHint()) do_widgetListener_set_hint(this,0,MHint);
    }
  }

  void on_widget_mouse_leave(SAT_BaseWidget* ATo, double AXpos, double AYpos, uint32_t ATimestamp) override {
    if (isActive() && isVisible()) {
      if (autoHoverCursor()) do_widgetListener_set_cursor(this,SAT_CURSOR_DEFAULT);
      if (autoHoverHint()) do_widgetListener_set_hint(this,0,"");
    }
  }

  void on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ACount, double* AValue) override {
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

  void do_widgetListener_select(SAT_Widget* ASender, int32_t AIndex, int32_t ASubIndex=-1) override {
    if (MListener) MListener->do_widgetListener_select(ASender,AIndex,ASubIndex);
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
  
  // text edit, popup menu
  
  void do_widgetListener_set_modal(SAT_Widget* ASender) override {
    if (MListener) MListener->do_widgetListener_set_modal(ASender);
  }
  
  //----------
  
  // called by SAT_SizerWidget (mose move)

  void do_widgetListener_resized(SAT_Widget* ASender, double ADeltaX, double ADeltaY) override {
    SAT_Rect mrect = getRect();
    double S = getWindowScale();
    mrect.w += ADeltaX;
    mrect.h += ADeltaY;
    
    if ((MMinWidth  >= 0) && (mrect.w < (MMinWidth  * S))) return;//mrect.w = (MMinWidth  * S);
    if ((MMinHeight >= 0) && (mrect.h < (MMinHeight * S))) return;//mrect.h = (MMinHeight * S);
    if ((MMaxWidth  >= 0) && (mrect.w > (MMaxWidth  * S))) return;//mrect.w = (MMaxWidth  * S);
    if ((MMaxHeight >= 0) && (mrect.h > (MMaxHeight * S))) return;//mrect.h = (MMaxHeight * S);
    
    // identical to setRectndBasis ?
    
//    setRect(mrect);
//    mrect.scale(1.0/S);
//    setBasisRect(mrect);

//  virtual void setRectAndBasis(SAT_Rect ARect) {
//    double S = getWindowScale();
//    setRect(ARect);
//    ARect.scale(1.0 / S);
//    setBasisRect(ARect);
//  }
    
    setRectAndBasis(mrect);
    
    //realignChildWidgets(true);
    //parentRedraw();
  }
  
  //----------
  
  // text edit
  
  void do_widgetListener_want_keys(SAT_Widget* ASender) override {
    if (MListener) MListener->do_widgetListener_want_keys(ASender);
  }
  

};

//----------------------------------------------------------------------
#endif


/*

  rect vs basisrect vs initialrect
  
  * initialrect, because we want to check if we had negative coords when aligning
  * basisrect is what we align, scale from to calculate the screen rect
    (we start with basisrect, not current rect)
    coords in (abs) initial-rect-space (not negative values [percentages], which is kept in initialrect)
  * rect is current screen rect
    in screen/mouse-space
*/