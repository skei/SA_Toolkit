#ifndef sat_widget_included
#define sat_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_gui_base.h"
#include "gui/sat_painter.h"

class SAT_MenuWidget;
class SAT_Widget;
typedef SAT_Array<SAT_Widget*> SAT_WidgetArray;

//----------------------------------------------------------------------

struct SAT_WidgetLayout {
  uint32_t  flags             = SAT_WIDGET_LAYOUT_DEFAULT;
  SAT_Rect  innerBorder       = {0,0,0,0};
  SAT_Rect  outerBorder       = {0,0,0,0};
  SAT_Point spacing           = {0,0};
  //uint32_t  direction         = SAT_DIRECTION_RIGHT;
  //SAT_Rect  clipOffset        = {0,0,0,0};
  //double    childScale        = 1.0;
  SAT_Rect  minSize           = {-1,-1,-1,-1};
  SAT_Rect  maxSize           = {-1,-1,-1,-1};
};

struct SAT_WidgetOptions {
  bool      opaque            = false;    // fully fills its rect
  bool      autoCapture       = false;    // mouse automatically captured
  bool      autoCursor        = true;     // mouse automatically captured
  bool      autoHint          = true;     // automatically send hint
  bool      realignInvisible  = false;
  bool      autoClipChildren  = true;
  bool      wantHoverEvents   = false;
};

struct SAT_WidgetState {
  bool      active            = true;   // receive events
  bool      visible           = true;   // alignment, drawing
  bool      disabled          = false;  // drawn grayed out
  bool      interactive       = false;  // drawn highlighted
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Widget
: public SAT_BaseWidget {

  friend class SAT_VisualWidget;
  friend class SAT_GuiQueues;

//------------------------------
private:
//------------------------------

  uint32_t MLastRealignedFrame = 0;
  uint32_t MLastPaintedFrame = 0;

//------------------------------
private:
//------------------------------

  SAT_WidgetOwner*  MOwner                              = nullptr;        // aka window
  SAT_Widget*       MParent                             = nullptr;
  SAT_WidgetArray   MChildren                           = {};             // sub-widgets
  uint32_t          MIndex                              = 0;              // index into parent's children list

  SAT_Rect          MRect                               = {0,0,0,0};      // current on-screen rect
  SAT_Rect          MBaseRect                           = {0,0,0,0};      // basis for re-alignment
  SAT_Rect          MInitialRect                        = {0,0,0,0};      // rect at creation time (unmodified)
  SAT_Rect          MContentRect                        = {0,0,0,0};

  SAT_Rect          MManualTween                        = {0,0,0,0};
  SAT_Rect          MPrevTween                          = {0,0,0,0};
  SAT_Rect          MManuallyMoved                      = {0,0,0,0};
//SAT_Rect          MManualScrolled                     = {0,0,0,0};

  // both parameter index & value index?

  void*             MParameters[SAT_WIDGET_NUM_VALUES]  = {0};            // array of values
  uint32_t          MParameterIndex                     = 0;              // index of selected, current value

  double            MValues[SAT_WIDGET_NUM_VALUES]      = {0};            // array of values
  uint32_t          MValueIndex                         = 0;              // index of selected, current value
  uint32_t          MNumValues                          = 1;
  double            MModulation                         = 0.0;

  const char*       MName                               = "SAT_Widget";
  int32_t           MCursor                             = SAT_CURSOR_DEFAULT;
  const char*       MHint                               = "";

  SAT_MenuWidget*   MContextMenu                        = nullptr;

//------------------------------
public:
//------------------------------

  SAT_WidgetState   State   = {};
  SAT_WidgetLayout  Layout  = {};
  SAT_WidgetOptions Options = {};

//------------------------------
public:
//------------------------------

  SAT_Widget(SAT_Rect ARect)
  : SAT_BaseWidget() {
    setName("SAT_Widget");
    setHint("SAT_Widget");
    MInitialRect = ARect;
    MBaseRect = ARect;
    MRect = ARect;
  }

  //----------

  virtual ~SAT_Widget() {
    #ifndef SAT_NO_AUTODELETE
    deleteChildren();
    #endif
  }

//------------------------------
public:
//------------------------------

  virtual SAT_Rect          getBaseRect()                 { return MBaseRect; }
  virtual SAT_Rect          getContentRect()              { return MContentRect; }
  virtual SAT_MenuWidget*   getContextMenu()              { return MContextMenu; }
  virtual int32_t           getCursor()                   { return MCursor; }
  virtual const char*       getHint()                     { return MHint; }
  virtual uint32_t          getIndex()                    { return MIndex; }
  virtual SAT_Rect          getInitialRect()              { return MInitialRect; }
  virtual double            getModulation()               { return MModulation; }
  virtual const char*       getName()                     { return MName; }
  virtual SAT_WidgetOwner*  getOwner()                    { return MOwner; }
  virtual uint32_t          getNumValues()                { return MNumValues; }
  virtual void*             getParameter()                { return MParameters[MParameterIndex]; }
  virtual void*             getParameter(uint32_t AIndex) { return MParameters[AIndex]; }
  virtual SAT_Widget*       getParent()                   { return MParent; }
  virtual SAT_Rect          getRect()                     { return MRect; }
  virtual double            getValue(uint32_t AIndex)     { return MValues[AIndex]; }
  virtual double            getValue()                    { return getValue(MValueIndex); }
  virtual uint32_t          getValueIndex()               { return MValueIndex; }

  virtual bool isRecursivelyActive() {
    if (!State.active) return false;
    if (!MParent) return true;
    return MParent->isRecursivelyActive();
  }

  virtual bool isRecursivelyVisible() {
    if (!State.visible) return false;
    if (!MParent) return true;
    return MParent->isRecursivelyVisible();
  }

  virtual bool isRecursivelyDisabled() {
    if (!State.disabled) return false;
    if (!MParent) return true;
    return MParent->isRecursivelyDisabled();
  }

  virtual SAT_Widget* findOpaqueParent() {
    //if (Options.opaque == true) return this;
    //else {
      if (MParent) {
        if (MParent->Options.opaque == true) return MParent;
        else return MParent->findOpaqueParent();
      }
      else return nullptr;
    //}
  }

//------------------------------

  virtual void setBaseRect(SAT_Rect ARect)                      { MBaseRect = ARect; }
  virtual void setContextMenu(SAT_MenuWidget* AMenu)            { MContextMenu = AMenu; }
  virtual void setHint(const char* AHint)                       { MHint = AHint; }
  virtual void setIndex(uint32_t AIndex)                        { MIndex = AIndex; }
  virtual void setModulation(double AValue)                     { MModulation = AValue; }
  virtual void setCursor(int32_t ACursor)                       { MCursor = ACursor; }
  virtual void setName(const char* AName)                       { MName = AName; }
  virtual void setNumValues(uint32_t ANum)                      { MNumValues = ANum; }
  virtual void setOwner(SAT_WidgetOwner* AOwner)                { MOwner = AOwner; }
  virtual void setParameter(void* AParameter)                   { MParameters[MParameterIndex] = AParameter; }
  virtual void setParameter(void* AParameter, uint32_t AIndex)  { MParameters[AIndex] = AParameter; }
  virtual void setParameterIndex(uint32_t AIndex)               { MParameterIndex = AIndex; }
  virtual void setParent(SAT_Widget* AParent)                   { MParent = AParent; }
  virtual void setPos(SAT_Point APos)                           { MRect.x = APos.x; MRect.y = APos.y; }
  virtual void setRect(SAT_Rect ARect)                          { MRect = ARect; }
  virtual void setSize(SAT_Point ASize)                         { MRect.w = ASize.x; MRect.h = ASize.y; }
  virtual void setValue(double AValue, uint32_t AIndex)         { MValues[AIndex] = AValue; }
  virtual void setValue(double AValue)                          { setValue(AValue,MValueIndex); }
  virtual void setValueIndex(uint32_t AIndex)                   { MValueIndex = AIndex; }

  virtual void setMinPos(double AXpos, double AYpos) {
    Layout.minSize.x = AXpos;
    Layout.minSize.y = AYpos;
  }

  virtual void setMinSize(double AWidth, double AHeight) {
    Layout.minSize.w = AWidth;
    Layout.minSize.h = AHeight;
  }

  virtual void setMaxPos(double AXpos, double AYpos) {
    Layout.maxSize.x = AXpos;
    Layout.maxSize.y = AYpos;
  }

  virtual void setMaxSize(double AWidth, double AHeight) {
    Layout.maxSize.w = AWidth;
    Layout.maxSize.h = AHeight;
  }

  virtual void setSize(double AWidth, double AHeight) {
    MRect.w = AWidth;
    MRect.h = AHeight;
  }

  virtual void setWidth(double AWidth) {
    MRect.w = AWidth;
  }

  virtual void setHeight(double AHeight) {
    MRect.h = AHeight;
  }

  virtual void setActive(bool AState=true, bool ARecursive=true) {
    State.active = AState;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->setActive(AState,ARecursive);
      }
    }
  }

  virtual void setVisible(bool AState=true, bool ARecursive=true) {
    State.visible = AState;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->setVisible(AState,ARecursive);
      }
    }
  }

  virtual void setDisabled(bool AState=true, bool ARecursive=true) {
    State.disabled = AState;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->setDisabled(AState,ARecursive);
      }
    }
  }

  /*
    MRect is scaled, then moved..
    so, to 'rescale' it back, we should un-move first, then un-scale
  */

  virtual void setRectAndBase(SAT_Rect ARect) {
    SAT_Rect rect = ARect;
    setRect(ARect);
    double scale = getWindowScale();
    rect.scale(1.0 / scale);
    setBaseRect(rect);
  }


//------------------------------
public: // children
//------------------------------

  virtual SAT_Widget* appendChild(SAT_Widget* AWidget) {
    uint32_t AIndex = MChildren.size();
    AWidget->setParent(this);
    AWidget->setIndex(AIndex);
    MChildren.append(AWidget);
    return AWidget;
  }

  //----------

  virtual void removeChild(SAT_Widget* AWidget) {
    MChildren.remove(AWidget);
  }

  //----------

  virtual void deleteChildren() {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      if (MChildren[i]) {
        delete MChildren[i];
        MChildren[i] = nullptr;
      }
    }
    MChildren.clear();
  }

  //----------

  virtual uint32_t getNumChildren() {
    return MChildren.size();
  }

  //----------

  virtual SAT_Widget* getChild(uint32_t AIndex) {
    return MChildren[AIndex];
  }

  //----------

  virtual SAT_WidgetArray* getChildren() {
    return &MChildren;
  }

//------------------------------
public: // owner window
//------------------------------

  // called from
  //   SAT_WidgetWindow.on_Window_show()
  // (after show, scale, size, realign)

  virtual void ownerWindowOpened(SAT_WidgetOwner* AOwner) {
    MOwner = AOwner;
    on_Widget_open(AOwner);
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* widget = MChildren[i];
      widget->ownerWindowOpened(AOwner);
    }
  }

  //----------

  // called from
  //   SAT_WidgetWindow.on_Window_hide()
  // (before hiding window)

  virtual void ownerWindowClose(SAT_WidgetOwner* AOwner) {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* widget = MChildren[i];
      widget->ownerWindowClose(AOwner);
    }
    on_Widget_close(AOwner);
  }

  //----------

  // asks owner window about its width

  virtual double getWindowWidth() {
    if (MOwner) return MOwner->on_WidgetOwner_getWidth();
    else return 0.0;
  }
  
  //----------

  // asks owner window about its height

  virtual double getWindowHeight() {
    if (MOwner) return MOwner->on_WidgetOwner_getHeight();
    else return 0.0;
  }
  
  //----------

  // asks owner window about its scale

  /*
    can be called if the window is created, but not open yet (clap)
    which means, we haven't called ownerWindowOpened on the widgets yet,
    so if they call getWindowScale (realign), owner is null -> scale = 1.0
    (i think)
  */

  virtual double getWindowScale() {
    double scale = 1.0;
    if (MOwner) {
      scale = MOwner->on_WidgetOwner_getScale();
    }
    return scale;
  }

//------------------------------
public:
//------------------------------

  // returns null if no child widgets at x,y

  virtual SAT_Widget* findWidget(int32_t AXpos, int32_t AYpos, bool ARecursive=true) {
    if (MChildren.size() > 0) {
      for (int32_t i=MChildren.size()-1; i>=0; i--) {
        SAT_Widget* widget = MChildren[i];
        SAT_Rect widget_rect = widget->getRect();
        if (widget->State.active) {
          if (widget_rect.contains(AXpos,AYpos)) {
            if (ARecursive) return widget->findWidget(AXpos,AYpos);
          }
        }
      }
    }
    return this;
  }

  //----------

  virtual SAT_Widget* findWidget(const char* AName, bool ARecursive=true) { return nullptr;
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* child = MChildren[i];
      const char* name = child->getName();
      if (strcmp(AName,name) == 0) {
        return child;
      }
      else {
        if (ARecursive) {
          SAT_Widget* subchild = child->findWidget(AName,ARecursive);
          if (subchild) return subchild;
        }
      }
    }
    return nullptr;
  }

  //----------

  virtual void scrollChildren(double AOffsetX, double AOffsetY) {
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      SAT_Widget* child = MChildren[i];
      if (child->State.visible) {
        child->MRect.x += AOffsetX;
        child->MRect.y += AOffsetY;
        child->scrollChildren(AOffsetX,AOffsetY);
      }
    }
  }
  //----------

  // note: doesn't paint itself..

  virtual void paintChildren(SAT_PaintContext* AContext) {
    SAT_Rect mrect = getRect();
    SAT_Painter* painter= AContext->painter;
    uint32_t numchildren = MChildren.size();
    if (numchildren > 0) {
      SAT_Rect clip_rect = mrect;
      if (Options.autoClipChildren) painter->pushOverlappingClip(clip_rect);
      for (uint32_t i=0; i<numchildren; i++) {
        SAT_Widget* widget = MChildren[i];
        if (widget->isRecursivelyVisible()) {
          SAT_Rect widgetrect = widget->getRect();
          widgetrect.overlap(mrect);
          if (widgetrect.isNotEmpty()) {
            widget->on_Widget_paint(AContext);
          }
        }
      }
      if (Options.autoClipChildren) painter->popClip();
    }
  }

  //----------

  virtual void realignChildren(bool ARecursive=true) {

    double scale = getWindowScale();

    SAT_Rect inner_border = Layout.innerBorder;
    inner_border.scale(scale);
    SAT_Point spacing = Layout.spacing;
    spacing.scale(scale);

    SAT_Rect mrect = getRect();
    SAT_Rect parent_rect = mrect;
    parent_rect.shrink(inner_border);
    SAT_Rect layout_rect = mrect;
    layout_rect.shrink(inner_border);

    double layout_xcenter = layout_rect.x + (layout_rect.w * 0.5);
    double layout_ycenter = layout_rect.y + (layout_rect.h * 0.5);
    MContentRect = SAT_Rect( mrect.x,mrect.y, 0,0 );

    double stackx          = 0;
    double stacky          = 0;
    double stack_highest   = 0;
    double stack_widest    = 0;
    
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* child = MChildren[i];
      uint32_t child_layout = child->Layout.flags;
      SAT_Rect child_rect;

      bool need_realign = (child->State.visible || child->Options.realignInvisible);
      if (need_realign) {

        if (child_layout & SAT_WIDGET_LAYOUT_PERCENT) {
          child_rect = SAT_Rect(mrect.w,mrect.h,mrect.w,mrect.h);
          child_rect.scale(child->getInitialRect());
          child_rect.scale(0.01);
        }
        else if (child_layout & SAT_WIDGET_LAYOUT_PERCENT_PARENT) {
          child_rect = SAT_Rect(parent_rect.w,parent_rect.h,parent_rect.w,parent_rect.h);
          child_rect.scale(child->getInitialRect());
          child_rect.scale(0.01);
        }
        else if (child_layout & SAT_WIDGET_LAYOUT_PERCENT_LAYOUT) {
          child_rect = SAT_Rect(layout_rect.w,layout_rect.h,layout_rect.w,layout_rect.h);
          child_rect.scale(child->getInitialRect());
          child_rect.scale(0.01);
        }
        else {
          child_rect = child->getBaseRect();
          child_rect.scale(scale);
        }

        SAT_Rect manual = child->MManualTween;
        manual.scale(scale);
        child_rect.add(manual);

        //child_rect.add(child->MManuallyMoved);
        child_rect = child->on_Widget_preAlign(child_rect);
        child_rect.add(child->MManuallyMoved);

        // anchor
        bool xanchored = false;
        bool yanchored = false;
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_LEFT)         { xanchored = true; child_rect.x += layout_rect.x; }
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_TOP)          { yanchored = true; child_rect.y += layout_rect.y; }
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_RIGHT)        { xanchored = true; child_rect.x += (layout_rect.x2()  - child_rect.w); }
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM)       { yanchored = true; child_rect.y += (layout_rect.y2()  - child_rect.h); }
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_CENTER_HORIZ) { xanchored = true; child_rect.x += (layout_xcenter - (child_rect.w * 0.5)); }
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_CENTER_VERT)  { yanchored = true; child_rect.y += (layout_ycenter - (child_rect.h * 0.5)); }

        if (child_layout & SAT_WIDGET_LAYOUT_STACK_HORIZ) {
          if ((stackx + child_rect.w /*+ border.w - spacing.x*/) >= layout_rect.w) {
            if (stackx != 0) {  // first widget..
              stackx = 0;
              stacky += stack_highest + spacing.y;
              stack_highest = 0;
            }
          }
          xanchored = true;
          yanchored = true;
          child_rect.x += (layout_rect.x + stackx);
          child_rect.y += (layout_rect.y + stacky);
          stackx += child_rect.w + spacing.x;
          if (child_rect.h > stack_highest) stack_highest = child_rect.h;
        }

        if (child_layout & SAT_WIDGET_LAYOUT_STACK_VERT) {
          // xanchored = true;
          // yanchored = true;
          if ((stacky + child_rect.h /*+ border.h - spacing.y*/) >= layout_rect.h) {
            if (stacky != 0) {  // first widget..
              stacky = 0;
              stackx += stack_widest + spacing.x;
              stack_widest = 0;
            }
          }
          xanchored = true;
          yanchored = true;
          child_rect.x += (layout_rect.x + stackx);
          child_rect.y += (layout_rect.y + stacky);
          stacky += child_rect.h + spacing.y;
          if (child_rect.w > stack_widest) stack_widest = child_rect.w;
        }

        if (!xanchored) child_rect.x += mrect.x;
        if (!yanchored) child_rect.y += mrect.y;

        // stretch
        if (child_layout & SAT_WIDGET_LAYOUT_STRETCH_LEFT)        { child_rect.setX1( layout_rect.x   ); }
        if (child_layout & SAT_WIDGET_LAYOUT_STRETCH_TOP)         { child_rect.setY1( layout_rect.y   ); }
        if (child_layout & SAT_WIDGET_LAYOUT_STRETCH_RIGHT)       { child_rect.setX2( layout_rect.x2()); }
        if (child_layout & SAT_WIDGET_LAYOUT_STRETCH_BOTTOM)      { child_rect.setY2( layout_rect.y2()); }
        // crop
        if (child_layout & SAT_WIDGET_LAYOUT_FILL_LEFT)           { layout_rect.setX1( child_rect.x2()); layout_rect.x += spacing.x; layout_rect.w -= spacing.x; }
        if (child_layout & SAT_WIDGET_LAYOUT_FILL_TOP)            { layout_rect.setY1( child_rect.y2()); layout_rect.y += spacing.y; layout_rect.h -= spacing.y; }
        if (child_layout & SAT_WIDGET_LAYOUT_FILL_RIGHT)          { layout_rect.setX2( child_rect.x   ); layout_rect.w -= spacing.x; }
        if (child_layout & SAT_WIDGET_LAYOUT_FILL_BOTTOM)         { layout_rect.setY2( child_rect.y   ); layout_rect.h -= spacing.y; }

        if (child_layout & SAT_WIDGET_LAYOUT_STACK_END) {
          if (child_layout & SAT_WIDGET_LAYOUT_STACK_VERT) {
            float w = (stackx + stack_widest + spacing.x);
            layout_rect.x += w;
            layout_rect.w -= w;
          }
          if (child_layout & SAT_WIDGET_LAYOUT_STACK_HORIZ) {
            float h = (stacky + stack_highest + spacing.y);
            layout_rect.y += h;
            layout_rect.h -= h;
          }
        }

        double min_width  = child->Layout.minSize.w * scale;
        double min_height = child->Layout.minSize.h * scale;
        double max_width  = child->Layout.maxSize.w * scale;
        double max_height = child->Layout.maxSize.h * scale;
        if ((min_width  >= 0) && (child_rect.w < min_width )) child_rect.w = min_width;
        if ((min_height >= 0) && (child_rect.h < min_height)) child_rect.h = min_height;
        if ((max_width  >= 0) && (child_rect.w > max_width )) child_rect.w = max_width;
        if ((max_height >= 0) && (child_rect.h > max_height)) child_rect.h = max_height;

        MContentRect.combine(child_rect);

        // outer border
        SAT_Rect outer_border = child->Layout.outerBorder;
        outer_border.scale(scale);
        child_rect.shrink(outer_border);

        child_rect = child->on_Widget_postAlign(child_rect);
        
        child->setRect(child_rect);

        if (ARecursive) child->realignChildren(ARecursive);

          // child->realignChildren(ARecursive);
          // if (child_layout & SAT_WIDGET_LAYOUT_CONTENT_SIZE) {
          //   SAT_Rect child_content = child->getContentRect();
          //   child->setRect(child_content);
          // }

      } // visible
    } // for

    MContentRect.w += inner_border.w;
    MContentRect.h += inner_border.h;

  }

  //----------

  virtual void clearPrevTween() {
    MPrevTween = {0,0,0,0};
  }

  //----------

  virtual void handleTweening(uint32_t AId, uint32_t AType, uint32_t ACount, double* AData) {
    switch(AType) {
      case SAT_TWEEN_FINISHED:
        break;
      case SAT_TWEEN_VALUE:
        break;
      case SAT_TWEEN_RECT:
        SAT_Rect tween = SAT_Rect(AData[0],AData[1],AData[2],AData[3]);
        SAT_Rect prev = MPrevTween;
        MPrevTween = tween;
        tween.sub(prev);
        MManualTween.add(tween);
        //if (MParent) {
        //  MParent->do_Widget_realign(MParent,SAT_WIDGET_REALIGN_SELF);
        //  MParent->do_Widget_redraw(MParent,0,SAT_WIDGET_REDRAW_SELF);
        //}
        do_Widget_realign(this,SAT_WIDGET_REALIGN_PARENT);
        do_Widget_redraw(this,0,SAT_WIDGET_REDRAW_PARENT);
        break;
    }
  }

//------------------------------
public:
//------------------------------

  // called from:
  //   SAT_Widget.ownerWindowOpened

  void on_Widget_open(SAT_WidgetOwner* AOwner) override {
  }

  //----------

  // called from:
  //   SAT_Widget.ownerWindowClose

  void on_Widget_close(SAT_WidgetOwner* AOwner) override {
  }

  //----------

  // called from:
  // nowhere yet.. todo: during realigning? runtime movement?

  void on_Widget_move(int32_t AXpos, int32_t AYpos) override {
  }
  
  //----------

  // called from:
  //   SAT_WidgetWindow.on_Window_resize

  void on_Widget_resize(uint32_t AWidth, uint32_t AHeight) override {
  }
  
  //----------

  // called from:
  //   SAT_Widget.paintChildren
  //   SAT_WidgetWindow.on_Window_paint  

  void on_Widget_paint(SAT_PaintContext* AContext) override {
    if (State.visible) {
      paintChildren(AContext);
    }
  }
  
  //----------

  // called from:
  //   SAT_Widget.realignChildren
  //   todo: before showing window

  void on_Widget_realign() override {
    realignChildren();
  }
  
  //----------

  void on_Widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  }
  
  //----------

  void on_Widget_mouse_dblclick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //SAT_TRACE;
  }
  
  //----------

  void on_Widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  }
  
  //----------

  void on_Widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
  }
  
  //----------

  void on_Widget_key_press(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) override {
  }
  
  //----------

  void on_Widget_key_release(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) override {
  }
  
  //----------

  void on_Widget_enter(SAT_Widget* AFrom, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //if (isActive() && isVisible()) {
      if (Options.autoCursor) do_Widget_set_cursor(this,MCursor);
      if (Options.autoHint) do_Widget_set_hint(this,MHint);
    //}
  }
  
  //----------

  void on_Widget_leave(SAT_Widget* ATo, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //if (isActive() && isVisible()) {
      if (Options.autoCursor) do_Widget_set_cursor(this,SAT_CURSOR_DEFAULT);
      if (Options.autoHint) do_Widget_set_hint(this,"");
    //}
  }

  //----------

  void on_Widget_tween(uint32_t AId, uint32_t AType, uint32_t ACount, double* AData) override {
    handleTweening(AId,AType,ACount,AData);
  }

  //----------

  void on_Widget_timer(double ADelta) override {
  }

//------------------------------
public:
//------------------------------

  void do_Widget_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) override {
    if (MParent) MParent->do_Widget_update(AWidget,AIndex,AMode);
  }
  
  void do_Widget_redraw(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_SELF) override {
    if (MParent) MParent->do_Widget_redraw(AWidget,AIndex,AMode);
  }
  
  void do_Widget_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_SELF) override {
    if (MParent) MParent->do_Widget_realign(AWidget,AMode);
  }
  
  void do_Widget_notify(SAT_Widget* AWidget, int32_t AValue, void* APtr=nullptr) override {
    if (MParent) MParent->do_Widget_notify(AWidget,AValue,APtr);
  }
  
  void do_Widget_mouse_capture(SAT_Widget* AWidget) override {
    if (MParent) MParent->do_Widget_mouse_capture(AWidget);
  }
  
  void do_Widget_key_capture(SAT_Widget* AWidget) override {
    if (MParent) MParent->do_Widget_key_capture(AWidget);
  }
  
  void do_Widget_modal(SAT_Widget* AWidget) override {
    if (MParent) MParent->do_Widget_modal(AWidget);
  }

  void do_Widget_set_cursor(SAT_Widget* AWidget, int32_t ACursor) override {
    if (MParent) MParent->do_Widget_set_cursor(AWidget,ACursor);
  }
  
  void do_Widget_set_hint(SAT_Widget* AWidget, const char* AHint) override {
    if (MParent) MParent->do_Widget_set_hint(AWidget,AHint);
  }

  void do_Widget_want_timer(SAT_Widget* AWidget, bool AWantTimer=true) override {
    if (MParent) MParent->do_Widget_want_timer(AWidget,AWantTimer);
  }

  void do_Widget_start_tween(SAT_Widget* AWidget, SAT_TweenChain* ATween) override {
    if (MParent) MParent->do_Widget_start_tween(AWidget,ATween);
  }

  void do_Widget_resize(SAT_Widget* AWidget, double ADeltaX, double ADeltaY) override {
    SAT_Rect base_rect = getBaseRect();
    double scale = getWindowScale();
    base_rect.w += (ADeltaX / scale);
    base_rect.h += (ADeltaY / scale);

    double min_width  = Layout.minSize.w;// * scale;
    double min_height = Layout.minSize.h;// * scale;
    double max_width  = Layout.maxSize.w;// * scale;
    double max_height = Layout.maxSize.h;// * scale;
    
    if ((min_width  >= 0) && (base_rect.w < min_width )) base_rect.w = min_width;
    if ((min_height >= 0) && (base_rect.h < min_height)) base_rect.h = min_height;
    if ((max_width  >= 0) && (base_rect.w > max_width )) base_rect.w = max_width;
    if ((max_height >= 0) && (base_rect.h > max_height)) base_rect.h = max_height;

    // if ((Layout.minSize.w >= 0) && (base_rect.w < Layout.minSize.w)) base_rect.w = Layout.minSize.w;
    // if ((Layout.minSize.h >= 0) && (base_rect.h < Layout.minSize.h)) base_rect.h = Layout.minSize.h;
    // if ((Layout.maxSize.h >= 0) && (base_rect.w > Layout.maxSize.w)) base_rect.w = Layout.maxSize.w;
    // if ((Layout.maxSize.w >= 0) && (base_rect.h > Layout.maxSize.h)) base_rect.h = Layout.maxSize.h;

    setBaseRect(base_rect);
    SAT_Widget* parent = AWidget->getParent();
    if (parent) {
      parent->on_Widget_realign();
      parent->do_Widget_redraw(parent);
    }
  }

  void do_Widget_set_overlay_color(SAT_Widget* AWidget, SAT_Color AColor) override {
    if (MParent) MParent->do_Widget_set_overlay_color(AWidget,AColor);
  }
  
};

//----------------------------------------------------------------------
#endif