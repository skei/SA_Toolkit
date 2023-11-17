#ifndef sat_widget_included
#define sat_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/system/sat_timer.h"
#include "gui/base/sat_paint_context.h"
#include "gui/base/sat_widget_owner.h"

//----------

class SAT_Widget;
typedef SAT_Array<SAT_Widget*> SAT_WidgetArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct SAT_WidgetLayout {
  uint32_t  flags           = SAT_WIDGET_LAYOUT_DEFAULT;
  SAT_Rect  inner_border    = {};
  SAT_Rect  outer_border    = {};
  SAT_Point spacing         = {};
};

//----------

struct SAT_WidgetState {
  bool active       = true;
  bool visible      = true;
  bool interacting  = false;
};

//----------

struct SAT_WidgetOptions {
  bool autoClip         = true;
  bool autoHoverCursor  = true;
  bool autoHoverHint    = true;
};

//----------------------------------------------------------------------
//
// widget
//
//----------------------------------------------------------------------

class SAT_Widget {

//------------------------------
private:
//------------------------------

  SAT_Rect            MInitialRect                    = {};
  SAT_Rect            MBaseRect                       = {};
  SAT_Point           MBaseOffset                     = {};
  SAT_Rect            MContentRect                    = {};

  SAT_Point           MMouseClickedPos                = {0,0};
  uint32_t            MMouseClickedButton             = SAT_BUTTON_NONE;
  bool                MMouseWaitingForMovement        = false;

//------------------------------
private:
//------------------------------

  SAT_WidgetLayout    MLayout                         = {};
  SAT_WidgetState     MState                          = {};
  SAT_WidgetOptions   MOptions                        = {};

  const char*         MName                           = "MWidget";
  SAT_WidgetOwner*    MOwner                          = nullptr;
  SAT_Widget*         MParent                         = nullptr;
  uint32_t            MParentIndex                    = 0;
  SAT_WidgetArray     MChildWidgets                   = {};
  double              MValues[SAT_WIDGET_MAX_VALUES]  = {};
  double              MModulation                     = 0;
  SAT_Rect            MRect                           = {};
  void*               MParameter                      = nullptr;
  uint32_t            MLastPainted                    = 0;

  int32_t             MMouseCursor                    = SAT_CURSOR_DEFAULT;
  const char*         MHint                           = "";

  int32_t             MSelectedIndex                  = 0;

  //SAT_Rect MLayoutDiff = SAT_Rect(0,0,0,0);

//------------------------------
public:
//------------------------------

  // we need to keep the initial rect, in case the base rect changes
  // (movable,popup), and we use percentage sizes (for realigning)
 
  SAT_Widget(SAT_Rect ARect) {
    MRect = ARect;
    MBaseRect = ARect;
    MInitialRect = ARect;
    MBaseOffset = {0,0};
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

  virtual const char*         getName()                                   { return MName; }
  virtual int32_t             getCursor()                                 { return MMouseCursor; }
  virtual const char*         getHint()                                   { return MHint; }
  virtual SAT_Widget*         getParent()                                 { return MParent; }
  virtual int32_t             getParentIndex()                            { return MParentIndex; }
  virtual void*               getParameter(uint32_t AIndex=0)             { return MParameter; }
  virtual SAT_WidgetOwner*    getOwner()                                  { return MOwner; }
  
  //----------


  virtual uint32_t            getNumChildWidgets()                        { return MChildWidgets.size(); }
  virtual SAT_Widget*         getChildWidget(uint32_t AIndex)             { return MChildWidgets[AIndex]; }

  virtual void                setName(const char* AName)                  { MName = AName; }
  virtual void                setCursor(int32_t ACursor)                  { MMouseCursor = ACursor; }
  virtual void                setHint(const char* AHint)                  { MHint = AHint; }
  virtual void                setParameter(void* AParameter, uint32_t AIndex=0) { MParameter = AParameter; }

  // value

  virtual double              getSelectedValue()                          { return MValues[MSelectedIndex]; }
  virtual int32_t             getSelectedValueIndex()                     { return MSelectedIndex; }
  virtual double              getValue(uint32_t AIndex=0)                 { return MValues[AIndex]; }
  virtual double              getModulation()                             { return MModulation; }

  virtual void                selectValue(int32_t AIndex)                 { MSelectedIndex = AIndex; }
  virtual void                setValue(double AValue, uint32_t AIndex=0)  { MValues[AIndex] = AValue; }
  virtual void                setModulation(double AValue)                { MModulation = AValue; }

  // rect

  virtual SAT_Point           getPos()                                    { return SAT_Point(MRect.x,MRect.y); }
  virtual SAT_Point           getSize()                                   { return SAT_Point(MRect.w,MRect.h); }
  virtual SAT_Rect            getRect()                                   { return MRect; }
  virtual double              getWidth()                                  { return MRect.w; }
  virtual double              getHeight()                                 { return MRect.h; }
  virtual SAT_Rect            getBaseRect()                               { return MBaseRect; }
  virtual SAT_Point           getBaseOffset()                             { return MBaseOffset; }
  virtual SAT_Rect            getContentRect()                            { return MContentRect; }
  virtual SAT_Rect            getInitialRect()                            { return MInitialRect; }

  virtual void                setPos(SAT_Point APos)                      { MRect.setPos(APos); }
  virtual void                setSize(SAT_Point ASize)                    { MRect.setSize(ASize); }
  virtual void                setRect(SAT_Rect ARect)                     { MRect = ARect; }
  virtual void                setBaseRect(SAT_Rect ARect)                 { MBaseRect = ARect; }
  virtual void                setBaseOffset(SAT_Point AOffset)            { MBaseOffset = AOffset; }

  virtual void                setWidth(double AWidth)                     { MRect.w = AWidth; }
  virtual void                setHeight(double AHeight)                   { MRect.h = AHeight; }
  virtual void                setBaseWidth(double AWidth)                 { MBaseRect.w = AWidth; }
  virtual void                setBaseHeight(double AHeight)               { MBaseRect.h = AHeight; }

  // layout

  virtual SAT_WidgetLayout*   getLayout()                                 { return &MLayout; }
  virtual uint32_t            getLayoutFlags()                            { return MLayout.flags; }
  virtual SAT_Rect            getLayoutInnerBorder()                      { return MLayout.inner_border; }
  virtual SAT_Rect            getLayoutOuterBorder()                      { return MLayout.outer_border; }
  virtual SAT_Point           getLayoutSpacing()                          { return MLayout.spacing; }

  virtual void                setLayoutFlags(uint32_t AFlags)             { MLayout.flags = AFlags; }
  virtual void                setLayoutInnerBorder(SAT_Rect ABorder)      { MLayout.inner_border = ABorder; }
  virtual void                setLayoutOuterBorder(SAT_Rect ABorder)      { MLayout.outer_border = ABorder; }
  virtual void                setLayoutSpacing(SAT_Point ASpacing)        { MLayout.spacing = ASpacing; }

  virtual bool                hasLayoutFlag(uint32_t AFlag)               { return (MLayout.flags & AFlag); }
  virtual void                addLayoutFlag(uint32_t AFlag)               { MLayout.flags |= AFlag; }
  virtual void                clearLayoutFlag(uint32_t AFlag)             { MLayout.flags &= ~AFlag; }

  // state

  virtual SAT_WidgetState*    getState()                                  { return &MState; }
  virtual bool                isActive()                                  { return MState.active; }
  virtual bool                isVisible()                                 { return MState.visible; }
  virtual bool                isInteracting()                             { return MState.interacting; }

//virtual void                setActive(bool AActive=true)                { MState.active = AActive; }    // see later down
//virtual void                setVisible(bool AVisible=true)              { MState.visible = AVisible; }  // see later down
  virtual void                setInteracting(bool AInteracting=true)      { MState.interacting = AInteracting; }

  // options

  virtual SAT_WidgetOptions*  getOptions()                                { return &MOptions; }
  virtual bool                autoClip()                                  { return MOptions.autoClip; }
  virtual bool                autoHoverCursor()                           { return MOptions.autoHoverCursor; }
  virtual bool                autoHoverHint()                             { return MOptions.autoHoverHint; }

  // ---

  virtual uint32_t            getLastPainted()                            { return MLastPainted; }
  virtual void                setLastPainted(uint32_t ALast)              { MLastPainted = ALast; }

//------------------------------
public:
//------------------------------

  virtual void redrawParent() {
    if (MParent) MParent->do_widget_redraw(MParent);
  }

  // called from:
  // SAT_Window.on_window_open()

  virtual void ownerWindowOpened(SAT_WidgetOwner* AOwner, bool ARecursive=true) {
    MOwner = AOwner;
    on_widget_open(AOwner);
    if (ARecursive) {
      for (uint32_t i=0; i<MChildWidgets.size(); i++) {
        MChildWidgets[i]->ownerWindowOpened(AOwner,ARecursive);
      }
    }
  }

  //----------

  // called from:
  // appendChildWidget()

  virtual void setParent(SAT_Widget* AWidget) {
    MParent = AWidget;
  }

  //----------

  // called from:
  // appendChildWidget()

  virtual void setParentIndex(uint32_t AIndex) {
    MParentIndex = AIndex;
  }

  //----------

  virtual void setActive(bool AState=true, bool ARecursive=true) {
    MState.active = AState;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildWidgets.size(); i++) {
        MChildWidgets[i]->setActive(AState,ARecursive);
      }
    }
  }

  //----------

  virtual bool isRecursivelyActive() {
    if (!isActive()) return false;
    if (!MParent) return true;
    return MParent->isRecursivelyActive();
  }

  //----------
  
  virtual void setVisible(bool AState=true, bool ARecursive=true) {
    MState.visible = AState;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildWidgets.size(); i++) {
        MChildWidgets[i]->setVisible(AState,ARecursive);
      }
    }
  }
  
  //----------

  virtual bool isRecursivelyVisible() {
    if (!isVisible()) return false;
    if (!MParent) return true;
    return MParent->isRecursivelyVisible();
  }

  //----------

  /*
    widgers could have an base_moved, which is set here..
    instead of modifying baserect directly
    the difference between the designed base rect, and the runtime modified
    when realigning we could take this account (calling getBaseRect,
    which returns base rect + offset)

    getBaseRect called from:
      realignChildWidgets, SAT_MovableWidget.on_widget_mouseMove
  */

  virtual void setRectAndBase(SAT_Rect ARect/*, double AScale=1.0*/) {
    double scale = getWindowScale();
    setRect(ARect);
    ARect.scale(1.0 / scale);//AScale);
    setBaseRect(ARect);
  }

  //----------

  virtual void connect(void* AParameter) {
    MParameter = AParameter;
  }

  //----------

  // asks /owner window about its width

  virtual double getWindowWidth() {
    if (MOwner) return MOwner->on_widgetOwner_getWidth();
    else return 0.0;
  }
  
  //----------

  // asks owner window about its height

  virtual double getWindowHeight() {
    if (MOwner) return MOwner->on_widgetOwner_getHeight();
    else return 0.0;
  }
  
  //----------

  // asks owner window about its scale

  virtual double getWindowScale() {
    double scale = 1.0;
    if (MOwner) scale = MOwner->on_widgetOwner_getScale();
    //SAT_Print("scale %f\n",scale);
    return scale;
  }

  //----------

  virtual SAT_Widget* appendChildWidget(SAT_Widget* AWidget) {
    uint32_t index = MChildWidgets.size();
    AWidget->setParent(this);
    AWidget->setParentIndex(index);
    MChildWidgets.push_back(AWidget);
    return AWidget;
  }

  //----------

  virtual void deleteChildWidgets() {
    for (uint32_t i=0; i<MChildWidgets.size(); i++) {
      if (MChildWidgets[i]) {
        delete MChildWidgets[i];
        MChildWidgets[i] = nullptr;
      }
    }
    MChildWidgets.clear();
  }

  //----------

  // returns null if no child widgets at x,y
  // todo: ifActive

  virtual SAT_Widget* findChildWidget(uint32_t AXpos, uint32_t AYpos, bool ARecursive=true) {
    uint32_t num = MChildWidgets.size();
    for (int32_t i=(num-1); i>=0; i--) {
    //for (uint32_t i=0; i<num; i++) {
      SAT_Widget* child = MChildWidgets[i];
      if (child->isActive()) {
        SAT_Rect child_rect = child->getRect();
        if (child_rect.contains(AXpos,AYpos)) {
          if (ARecursive) {
            SAT_Widget* subchild = child->findChildWidget(AXpos,AYpos,ARecursive);
            if (subchild) return subchild;
          }
          return child;
        }
      }
    }
    return nullptr;
  }

  //----------

  virtual SAT_Widget* findChildWidget(const char* AName, bool ARecursive=true) { return nullptr;
    for (uint32_t i=0; i<MChildWidgets.size(); i++) {
      SAT_Widget* child = MChildWidgets[i];
      const char* name = child->getName();
      if (strcmp(AName,name) == 0) {
        return child;
      }
      else {
        if (ARecursive) {
          SAT_Widget* subchild = child->findChildWidget(AName,ARecursive);
          if (subchild) return subchild;
        }
      }

    }
    return nullptr;
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

  // called from: SAT_Window.on_window_resize (on root window)
  // also called (override) by SAT_GraphWidget
  // (doesn't end up in anything)
  // scales from base rect

  
  // virtual void scaleWidget(double AScale, bool ARecursive=true) {
  //   MRect = getBaseRect();
  //   MRect.scale(AScale);
  //   if (ARecursive) {
  //     for (uint32_t i=0; i<MChildWidgets.size(); i++) {
  //       MChildWidgets[i]->scaleWidget(AScale);
  //     }
  //   }
  // }


  //----------

  // realigns from base rect
  // (sets rect, and content)

  virtual void realignChildWidgets(bool ARecursive=true) {

    double scale = getWindowScale();
    //SAT_Print("scale %f\n",scale);

    SAT_Rect inner_border = MLayout.inner_border;
    inner_border.scale(scale);

    SAT_Rect outer_border = MLayout.outer_border;
    outer_border.scale(scale);

    SAT_Point spacing = MLayout.spacing;
    spacing.scale(scale);

    SAT_Rect parent_rect = getRect();
    parent_rect.shrink(MLayout.inner_border);
    double parent_xcenter = parent_rect.x + (parent_rect.w * 0.5);
    double parent_ycenter = parent_rect.y + (parent_rect.h * 0.5);

    SAT_Rect layout_rect = getRect();
    layout_rect.shrink(MLayout.inner_border);
    double layout_xcenter = layout_rect.x + (layout_rect.w * 0.5);
    double layout_ycenter = layout_rect.y + (layout_rect.h * 0.5);

    MContentRect = SAT_Rect(layout_rect.x,layout_rect.y,0,0);

    for (uint32_t i=0; i<MChildWidgets.size(); i++) {

      SAT_Widget* child = MChildWidgets[i];

      SAT_Rect child_rect;

      SAT_WidgetState*  child_state   = child->getState();
      uint32_t          child_layout  = child->getLayout()->flags;
      SAT_Point         child_offset;

      if (child_state->visible) {
        child->on_widget_prealign();

        //-----

        if (child_layout & SAT_WIDGET_LAYOUT_PERCENT) {
          child_rect = SAT_Rect(parent_rect.w,parent_rect.h,parent_rect.w,parent_rect.h);
          child_rect.scale(child->getBaseRect());
          child_rect.scale(0.01);
        }
        else {

          // initial rect, so we don't risk using rect thas has been moved
          // (popup, movable, etc)..

          child_rect = child->getInitialRect();
          child_rect.scale(scale);
        }

        //child->MLayoutDiff = child_rect;

        //SAT_Print("%.f,%.f,%.f,%.f\n",child_rect.x,child_rect.y,child_rect.w,child_rect.h);

        //-----

        // if (child_layout & SAT_WIDGET_LAYOUT_WINDOW) {
        //   //child_rect.x -= layout_rect.x;
        //   //child_rect.y -= layout_rect.y;
        // }
        // else if (child_layout & SAT_WIDGET_LAYOUT_PARENT) {
        //   //child_rect.x += parent_rect.x /*- layout_rect.x*/;
        //   //child_rect.y += parent_rect.y /*- layout_rect.y*/;
        // }
        // else {
        //   //child_rect.x += layout_rect.x;
        //   //child_rect.y += layout_rect.y;
        // }

        //-----

        // child_rect contains border/spacing
        // with += and it will apply again here.. :-/
        // if we force it to 0,0, we lose the ability to offset..
        // let's try, and see how it goes..

        bool xanchored = false;
        bool yanchored = false;
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_LEFT)         { xanchored = true; child_rect.x += layout_rect.x; }
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_TOP)          { yanchored = true; child_rect.y += layout_rect.y; }
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_RIGHT)        { xanchored = true; child_rect.x += (layout_rect.x2()  - child_rect.w); }
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM)       { yanchored = true; child_rect.y += (layout_rect.y2()  - child_rect.h); }
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_CENTER_HORIZ) { xanchored = true; child_rect.x += (layout_xcenter - (child_rect.w * 0.5)); }
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_CENTER_VERT)  { yanchored = true; child_rect.y += (layout_ycenter - (child_rect.h * 0.5)); }

        if (!xanchored) child_rect.x += parent_rect.x;
        if (!yanchored) child_rect.y += parent_rect.y;

        // previous offsets
        //if (xanchored) child_rect.x -= child_offset.x;
        //if (yanchored) child_rect.y += child_offset.y;
        // negative, so we can add realigned pos to find diff
        //child_offset.x = - child_rect.x;
        //child_offset.y = - child_rect.y;

        // if (!xanchored) child_rect.x += parent_rect.x;          
        // if (!yanchored) child_rect.y += parent_rect.y;          

        //SAT_Print("%.f,%.f,%.f,%.f\n",child_rect.x,child_rect.y,child_rect.w,child_rect.h);

        //-----

        if (child_layout & SAT_WIDGET_LAYOUT_STRETCH_LEFT)        { child_rect.setX1( layout_rect.x ); }
        if (child_layout & SAT_WIDGET_LAYOUT_STRETCH_TOP)         { child_rect.setY1( layout_rect.y ); }
        if (child_layout & SAT_WIDGET_LAYOUT_STRETCH_RIGHT)       { child_rect.setX2( layout_rect.x2() /*- child_rect.w*/ ); }
        if (child_layout & SAT_WIDGET_LAYOUT_STRETCH_BOTTOM)      { child_rect.setY2( layout_rect.y2() /*- child_rect.h*/ ); }

        //-----

        if (child_layout & SAT_WIDGET_LAYOUT_CROP_LEFT)           { layout_rect.setX1( child_rect.x2() ); layout_rect.x += spacing.x; layout_rect.w -= spacing.x; }
        if (child_layout & SAT_WIDGET_LAYOUT_CROP_TOP)            { layout_rect.setY1( child_rect.y2() ); layout_rect.y += spacing.y; layout_rect.h -= spacing.y; }
        if (child_layout & SAT_WIDGET_LAYOUT_CROP_RIGHT)          { layout_rect.setX2( child_rect.x    ); layout_rect.w -= spacing.x; }
        if (child_layout & SAT_WIDGET_LAYOUT_CROP_BOTTOM)         { layout_rect.setY2( child_rect.y    ); layout_rect.h -= spacing.y; }

        //-----

        child_rect.shrink(outer_border);

        MContentRect.combine(child_rect);
        child_rect.shrink(MLayout.outer_border);

        //child_offset.x += child_rect.x;
        //child_offset.y += child_rect.y;
        //SAT_Print("offset %.f,%.f\n",child_offset.x,child_offset.y);
        //child->MLayoutOffset = child_offset;

        //child->MLayoutDiff -= child_rect;

        child->setRect(child_rect);

        child->on_widget_postalign();
        if (ARecursive) child->realignChildWidgets(ARecursive);

      } // visible
    } // for

    MContentRect.w += inner_border.w;
    MContentRect.h += inner_border.h;

  }

  //----------

  // for (uint32_t i=0; i<MChildWidgets.size(); i++) {
  //   SAT_Widget* child = MChildWidgets[i];
  //   if (child->isVisible()) {
  //     SAT_Rect child_rect = child->getRect();
  //     if (child_rect.intersects(MRect)) {
  //       child->on_widget_paint(AContext);
  //       MChildWidgets[i]->paintChildWidgets(AContext);
  //     } // intersects
  //   } // visible
  // } // for

  // doesn't paint itself

  virtual void paintChildWidgets(SAT_PaintContext* AContext) {
    SAT_Rect mrect = getRect();
    SAT_Painter* painter= AContext->painter;
    uint32_t num = MChildWidgets.size();
    if (num > 0) {
      if (autoClip()) painter->pushOverlappingClip(mrect);

      for (uint32_t i=0; i<num; i++) {
      //for (int32_t i=(num-1); i>=0; i--) {
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
      if (autoClip()) painter->popClip();
    } // num > 0
  }

//------------------------------
public: // on_ (downwards)
//------------------------------

  // called from
  // SAT_Window.on_window_open() -> SAT_Widget.ownerWindowOpened

  virtual void on_widget_open(SAT_WidgetOwner* AOwner) {
  }

  // never called

  virtual void on_widget_move(double AXpos, double AYpos) {
  }

  // never called

  virtual void on_widget_resize(double AWidth, double AHeight) {
  }

  // called from SAT_Widget.realignChildWidgets
  // just before alignment

  virtual void on_widget_prealign() {
  }

  // called from SAT_Widget.realignChildWidgets
  // after alignment

  virtual void on_widget_postalign() {
  }

  virtual void on_widget_paint(SAT_PaintContext* AContext) {
    if (isVisible()) paintChildWidgets(AContext);
  }

  // called from 
  // SAT_Window.on_timerListener_callback -> SAT_Window.on_window_timer (MTimerWidgets)

  virtual void on_widget_timer(SAT_Timer* ATimer, double AElapsed) {
  }

  // called from
  // SAT_TweenManager

  virtual void on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ANum, double* AData) {
    //todo: handleTweening...
  }

  //

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
  
  virtual void on_widget_enter(SAT_Widget* AFrom, double AXpos, double AYpos) {
    if (isActive() && isVisible()) {
      if (autoHoverCursor()) do_widget_setCursor(this,MMouseCursor);
      if (autoHoverHint()) do_widget_setHint(this,MHint);
    }
  }

  virtual void on_widget_leave(SAT_Widget* ATo, double AXpos, double AYpos) {
    if (isActive() && isVisible()) {
      if (autoHoverCursor()) do_widget_setCursor(this,SAT_CURSOR_DEFAULT);
      if (autoHoverHint()) do_widget_setHint(this,"");
    }
  }

//------------------------------
public: // do_ (upwards)
//------------------------------

  // widget value have changed
  // (redrawing is handled separately)

  virtual void do_widget_update(SAT_Widget* AWidget) {
    if (MParent) MParent->do_widget_update(AWidget);
  }

  // widget needs to be redrawn for some reason

  virtual void do_widget_redraw(SAT_Widget* AWidget) {
    if (MParent) MParent->do_widget_redraw(AWidget);
  }

  // this is called by sub-widgets that have moved or resized
  // to signal that we might need to realign all our children
  // is widget (sender) is empty/null, we call the parent
  // (if we want the realign message to reach the root, we must set the sender as null)
  // (why/when do we need that?)

  virtual void do_widget_realign(SAT_Widget* AWidget) {
    //if (MParent) MParent->do_widget_realign(AWidget);
    //if (AWidget) {
    //  realignChildWidgets();
    //  do_widget_redraw(this);
    //}
    //else {
      if (MParent) MParent->do_widget_realign(this);
    //}
  }

  // widget wants to change the mouse cursor (or show/hide, lock/unlock)
  // see SAT_CURSOR_... 

  virtual void do_widget_setCursor(SAT_Widget* AWidget, int32_t ACursor) {
    if (MParent) MParent->do_widget_setCursor(AWidget,ACursor);
  }

  // widget sends a hint text

  virtual void do_widget_setHint(SAT_Widget* AWidget, const char* AHint) {
    if (MParent) MParent->do_widget_setHint(AWidget,AHint);
  }

  // widget wants to 'go modal'
  // only this (modal widgets) will receive events until it's turned off

  virtual void do_widget_setModal(SAT_Widget* AWidget) {
    if (MParent) MParent->do_widget_setModal(AWidget);
  }

  // widget wants key pressed

  virtual void do_widget_captureKeys(SAT_Widget* AWidget) {
    if (MParent) MParent->do_widget_captureKeys(AWidget);
  }

  // widget is clsoed
  // menus, popups..

  virtual void do_widget_close(SAT_Widget* AWidget) {
    if (MParent) MParent->do_widget_close(AWidget);
  }

  // item has been selected.. for popups, menus..
  // (todo: combine with select? index = -1, = close)

  virtual void do_widget_select(SAT_Widget* AWidget, int32_t AIndex, int32_t ASubIndex=-1) {
    if (MParent) MParent->do_widget_select(AWidget,AIndex,ASubIndex);
  }

  // widget is told by another widget to resize itself, and then notify parent, so it can realign and redraw..
  // todo: this it's an upward/downward event, but called directly from one widget to another..
  // put all of this into resizeWidget.. handle similar to resiing window.. ???
  
  //void do_widgetListener_resized(SAT_Widget* ASender, double ADeltaX, double ADeltaY) override {
  //  SAT_Rect mrect = getRect();
  //  double S = getWindowScale();
  //  mrect.w += ADeltaX;
  //  mrect.h += ADeltaY;
  //  if ((MMinWidth  >= 0) && (mrect.w < (MMinWidth  * S))) return;//mrect.w = (MMinWidth  * S);
  //  if ((MMinHeight >= 0) && (mrect.h < (MMinHeight * S))) return;//mrect.h = (MMinHeight * S);
  //  if ((MMaxWidth  >= 0) && (mrect.w > (MMaxWidth  * S))) return;//mrect.w = (MMaxWidth  * S);
  //  if ((MMaxHeight >= 0) && (mrect.h > (MMaxHeight * S))) return;//mrect.h = (MMaxHeight * S);
  //  setRectAndBasis(mrect);
  //  //realignChildWidgets(true);
  //  //parentRedraw();
  //}

};

//----------------------------------------------------------------------
#endif
