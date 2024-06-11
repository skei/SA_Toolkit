#ifndef sat_widget_included
#define sat_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_base_widget.h"

class SAT_Widget;
typedef SAT_Array<SAT_Widget*> SAT_WidgetArray;

//----------------------------------------------------------------------

struct SAT_WidgetLayout {
  uint32_t  flags             = SAT_WIDGET_LAYOUT_DEFAULT;
  //uint32_t  direction         = SAT_DIRECTION_RIGHT;
  SAT_Rect  innerBorder       = {0,0,0,0};
  SAT_Rect  outerBorder       = {0,0,0,0};
  //SAT_Rect  clipOffset        = {0,0,0,0};
  SAT_Point spacing           = {0,0};
};

struct SAT_WidgetOptions {
  bool      opaque            = true;   // fully fills its rect
  bool      autoCapture       = false;  // mouse automatically captured
  bool      autoCursor        = true;   // mouse automatically captured
  bool      autoHint          = false;  // automatically send hint
  bool      realignInvisible  = false;
  bool      autoClip          = false;
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

//SAT_Point         MMovedOffset                        = {0,0};          // manually moved, .. 
//SAT_Point         MScrolledOffset                     = {0,0};          // scrollbox, ..

  void*             MParameters[SAT_WIDGET_NUM_VALUES]  = {0};            // array of values
  uint32_t          MParameterIndex                     = 0;              // index of selected, current value

  double            MValues[SAT_WIDGET_NUM_VALUES]      = {0};            // array of values
  uint32_t          MValueIndex                         = 0;              // index of selected, current value
  uint32_t          MNumValues                          = 1;
  double            MModulation                         = 0.0;

  const char*       MName                               = "SAT_Widget";
  int32_t           MCursor                             = SAT_CURSOR_DEFAULT;
  const char*       MHint                               = "";

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
  virtual int32_t           getCursor()                   { return MCursor; }
  virtual const char*       getHint()                     { return MHint; }
  virtual uint32_t          getIndex()                    { return MIndex; }
  virtual SAT_Rect          getInitialRect()              { return MInitialRect; }
  virtual double            getModulation()               { return MModulation; }
  virtual const char*       getName()                     { return MName; }
  virtual uint32_t          getNumValues()                { return MNumValues; }
  virtual void*             getParameter()                { return MParameters[MParameterIndex]; }
  virtual void*             getParameter(uint32_t AIndex) { return MParameters[AIndex]; }
  virtual SAT_Widget*       getParent()                   { return MParent; }
  virtual SAT_Rect          getRect()                     { return MRect; }
  virtual double            getValue()                    { return MValues[MValueIndex]; }
  virtual double            getValue(uint32_t AIndex)     { return MValues[AIndex]; }
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

//------------------------------

  virtual void setBaseRect(SAT_Rect ARect)                      { MBaseRect = ARect; }

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
  virtual void setRect(SAT_Rect ARect)                          { MRect = ARect; }
  virtual void setValue(double AValue)                          { MValues[MValueIndex] = AValue; }
  virtual void setValue(double AValue, uint32_t AIndex)         { MValues[AIndex] = AValue; }
  virtual void setValueIndex(uint32_t AIndex)                   { MValueIndex = AIndex; }

  virtual void setSize(double AWidth, double AHeight) {
    MRect.w = AWidth;
    MRect.h = AHeight;
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

  // called from
  //   SAT_WidgetWindow.on_window_show()

  virtual void ownerWindowOpened(SAT_WidgetOwner* AOwner) {
    MOwner = AOwner;
    on_widget_open(AOwner);
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* widget = MChildren[i];
      widget->ownerWindowOpened(AOwner);
    }
  }

  //----------

  // called from
  //   SAT_WidgetWindow.on_window_hide()

  virtual void closeOwnerWindow(SAT_WidgetOwner* AOwner) {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* widget = MChildren[i];
      widget->closeOwnerWindow(AOwner);
    }
    on_widget_close(AOwner);
  }

  //----------

  // asks owner window about its width

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

  /*
    can be called if the window is created, but not open yet (clap)
    which means, we haven't called ownerWindowOpened on the widgets yet,
    so if they call getWindowScale (realign), owner is null -> scale = 1.0
    (i think)
  */

  virtual double getWindowScale() {
    double scale = 1.0;
    if (MOwner) {
      scale = MOwner->on_widgetOwner_getScale();
    }
    else {
      //SAT_PRINT("no owner\n");
      //SAT_GLOBAL.DEBUG.CALL_STACK.print();
    }
    return scale;
  }


  //----------

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

  // note: doesn't paint itself..

  virtual void paintChildren(SAT_PaintContext* AContext) {
    //SAT_PRINT("\n");
    SAT_Rect mrect = getRect();
    SAT_Painter* painter= AContext->painter;
    uint32_t numchildren = MChildren.size();
    if (numchildren > 0) {

      SAT_Rect clip_rect = mrect;
      if (Options.autoClip) painter->pushOverlappingClip(clip_rect);

      //for (int32_t i=(numchildren-1); i>=0; i--) {
      for (uint32_t i=0; i<numchildren; i++) {
        SAT_Widget* widget = MChildren[i];
        if (widget->State.visible) {
        //if (widget->isRecursivelyVisible()) {
          SAT_Rect widgetrect = widget->getRect();
          widgetrect.overlap(mrect);
          if (widgetrect.isNotEmpty()) {
            widget->on_widget_paint(AContext);
          }
        }
      }

      if (Options.autoClip) painter->popClip();

    }
  }

  //----------

  virtual void realignChildren(bool ARecursive=true) {
    //SAT_PRINT("%s\n",getName());

    double scale = getWindowScale();

    SAT_Rect inner_border = Layout.innerBorder;
    inner_border.scale(scale);
    SAT_Point spacing = Layout.spacing;
    spacing.scale(scale);

    SAT_Rect mrect = getRect();
    SAT_Rect parent_rect = mrect;
    parent_rect.shrink(Layout.innerBorder);
    SAT_Rect layout_rect = mrect;
    layout_rect.shrink(Layout.innerBorder);

    double layout_xcenter = layout_rect.x + (layout_rect.w * 0.5);
    double layout_ycenter = layout_rect.y + (layout_rect.h * 0.5);
    MContentRect = SAT_Rect( mrect.x,mrect.y, 0,0 );
    
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* child = MChildren[i];
      uint32_t child_layout = child->Layout.flags;
      SAT_Rect child_rect;
  //  SAT_Rect orig_rect;
      bool need_realign = child->State.visible || child->Options.realignInvisible;
      if (need_realign) {
        child->on_widget_preAlign();
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
  //    orig_rect = child_rect; // before scaling...
        // anchor
        bool xanchored = false;
        bool yanchored = false;
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_LEFT)         { xanchored = true; child_rect.x += layout_rect.x; }
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_TOP)          { yanchored = true; child_rect.y += layout_rect.y; }
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_RIGHT)        { xanchored = true; child_rect.x += (layout_rect.x2()  - child_rect.w); }
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM)       { yanchored = true; child_rect.y += (layout_rect.y2()  - child_rect.h); }
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_CENTER_HORIZ) { xanchored = true; child_rect.x += (layout_xcenter - (child_rect.w * 0.5)); }
        if (child_layout & SAT_WIDGET_LAYOUT_ANCHOR_CENTER_VERT)  { yanchored = true; child_rect.y += (layout_ycenter - (child_rect.h * 0.5)); }
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
        MContentRect.combine(child_rect);
        // outer border
        SAT_Rect outer_border = child->Layout.outerBorder;
        outer_border.scale(scale);
        child_rect.shrink(outer_border);
  //    child->MLayoutOffset.x = (child_rect.x - orig_rect.x); //(orig_rect.x * scale));
  //    child->MLayoutOffset.y = (child_rect.y - orig_rect.y); //(orig_rect.y * scale));
        child->setRect(child_rect);
        child->on_widget_postAlign();
        if (ARecursive) child->realignChildren(ARecursive);
      } // visible
    } // for
    MContentRect.w += inner_border.w;
    MContentRect.h += inner_border.h;

  }

  //----------

  virtual void handleTweening(uint32_t AId, uint32_t AType, uint32_t ACount, double* AData) {
    switch(AType) {
      case SAT_TWEEN_VALUE:
        SAT_PRINT("AId %i AType %i ACount %i : AData[0] %.3f \n",AId,AType,ACount,AData[0]);
        break;
      case SAT_TWEEN_RECT:
        setRect(SAT_Rect(AData[0],AData[1],AData[2],AData[3]));
        if (MParent) {
          MParent->do_widget_realign(MParent,SAT_WIDGET_REALIGN_POS);
        }
        break;
    }
  }


//------------------------------
public:
//------------------------------

  // called from:
  //   SAT_Widget.ownerWindowOpened

  void on_widget_open(SAT_WidgetOwner* AOwner) override {
  }

  //----------

  // called from:
  //   SAT_Widget.closeOwnerWindow

  void on_widget_close(SAT_WidgetOwner* AOwner) override {
  }

  //----------

  // called from:
  // nowhere yet.. todo: during realigning? runtime movement?

  void on_widget_move(int32_t AXpos, int32_t AYpos) override {
    //do_widget_realign(MParent,SAT_WIDGET_REALIGN_POS);
    //realignChildren();
  }
  
  //----------

  // called from:
  //   SAT_WidgetWindow.on_window_resize

  void on_widget_resize(uint32_t AWidth, uint32_t AHeight) override {
    //do_widget_realign(MParent,SAT_WIDGET_REALIGN_SIZE);
    //realignChildren();
  }
  
  //----------

  // called from:
  //   SAT_Widget.paintChildren
  //   SAT_WidgetWindow.on_window_paint  

  void on_widget_paint(SAT_PaintContext* AContext) override {
    if (State.visible) {
      paintChildren(AContext);
    }
  }
  
  //----------

  // called from:
  //   SAT_Widget.realignChildren
  //   todo: before showing window

  void on_widget_realign() override {
    realignChildren();
  }
  
  //----------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  }
  
  //----------

  void on_widget_mouse_dblclick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  }
  
  //----------

  void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  }
  
  //----------

  void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
  }
  
  //----------

  void on_widget_key_press(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) override {
  }
  
  //----------

  void on_widget_key_release(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) override {
  }
  
  //----------

  void on_widget_enter(SAT_Widget* AFrom, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //if (isActive() && isVisible()) {
      if (Options.autoCursor) do_widget_set_cursor(this,MCursor);
      if (Options.autoHint) do_widget_set_hint(this,MHint);
    //}
  }
  
  //----------

  void on_widget_leave(SAT_Widget* ATo, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //if (isActive() && isVisible()) {
      if (Options.autoCursor) do_widget_set_cursor(this,SAT_CURSOR_DEFAULT);
      if (Options.autoHint) do_widget_set_hint(this,"");
    //}
  }

  //----------

  void on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ACount, double* AData) override {
    handleTweening(AId,AType,ACount,AData);
  }

  //----------

  void on_widget_timer(double ADelta) override {
  }

//------------------------------
public:
//------------------------------

  void do_widget_update(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) override {
    //SAT_PRINT("%s\n",AWidget->getName());
    if (MParent) MParent->do_widget_update(AWidget,AMode);
  }
  
  void do_widget_redraw(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REDRAW_ALL) override {
    if (MParent) MParent->do_widget_redraw(AWidget,AMode);
  }
  
  void do_widget_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_POS) override {
    if (MParent) MParent->do_widget_realign(AWidget,AMode);
  }
  
  void do_widget_notify(SAT_Widget* AWidget, int32_t AValue) override {
    if (MParent) MParent->do_widget_notify(AWidget,AValue);
  }
  
  void do_widget_mouse_capture(SAT_Widget* AWidget, bool ACapture=true) override {
    if (MParent) MParent->do_widget_mouse_capture(AWidget);
  }
  
  void do_widget_key_capture(SAT_Widget* AWidget, bool ACapture=true) override {
    if (MParent) MParent->do_widget_key_capture(AWidget);
  }
  
  void do_widget_modal(SAT_Widget* AWidget, bool AModal=true) override {
    if (MParent) MParent->do_widget_modal(AWidget);
  }

  void do_widget_set_cursor(SAT_Widget* AWidget, int32_t ACursor) override {
    if (MParent) MParent->do_widget_set_cursor(AWidget,ACursor);
  }
  
  void do_widget_set_hint(SAT_Widget* AWidget, const char* AHint) override {
    if (MParent) MParent->do_widget_set_hint(AWidget,AHint);
  }

  void do_widget_want_timer(SAT_Widget* AWidget, bool AWantTimer=true) override {
    if (MParent) MParent->do_widget_want_timer(AWidget,AWantTimer);
  }

  void do_widget_start_tween(SAT_Widget* AWidget, SAT_TweenChain* ATween) override {
    if (MParent) MParent->do_widget_start_tween(AWidget,ATween);
  }

  
};

//----------------------------------------------------------------------
#endif
