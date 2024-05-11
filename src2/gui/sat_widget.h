#ifndef sat_widget_included
#define sat_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_base_widget.h"
//#include "gui/widget/sat_widget_owner.h"

class SAT_Widget;
typedef SAT_Array<SAT_Widget*> SAT_WidgetArray;

//----------------------------------------------------------------------

struct SAT_WidgetLayout {
  uint32_t  alignment   = 0;
  uint32_t  anchor      = 0;
  uint32_t  stretch     = 0;
  uint32_t  crop        = 0;
  SAT_Rect  innerBorder = {0,0,0,0};
  SAT_Rect  outerBorder = {0,0,0,0};
  uint32_t  direction   = SAT_DIRECTION_RIGHT;
  SAT_Rect  clipOffset  = {0,0,0,0};
};

struct SAT_WidgetOptions {
  bool opaque       = true;   // fully fills its rect
  bool autoCapture  = false;  // mouse automatically captured
  bool autoCursor   = false;  // mouse automatically captured
  bool autoHint     = false;  // automatically send hint
};

struct SAT_WidgetState {
  bool active       = true;   // receive events
  bool visible      = true;   // alignment, drawing
  bool disabled     = false;  // drawn grayed out
  bool interactive  = false;  // drawn highlighted
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

  SAT_WidgetState   MState                              = {};
  SAT_WidgetLayout  MLayout                             = {};
  SAT_WidgetOptions MOptions                            = {};
  SAT_Rect          MRect                               = {0,0,0,0};      // current on-screen rect

//SAT_Rect          MInitialRect                        = {0,0,0,0};      // rect at creation time (unmodified)
//SAT_Rect          MBaseRect                           = {0,0,0,0};      // basis for re-alignment
//SAT_Point         MMovedOffset                        = {0,0};          // manually moved, .. 
//SAT_Point         MScrolledOffset                     = {0,0};          // scrollbox, ..

//  void*             MParameter                          = nullptr;        // connected parameter

  void*             MParameters[SAT_WIDGET_NUM_VALUES]  = {0};            // array of values
  uint32_t          MParameterIndex                     = 0;              // index of selected, current value

  double            MValues[SAT_WIDGET_NUM_VALUES]      = {0};            // array of values
  uint32_t          MValueIndex                         = 0;              // index of selected, current value
  double            MModulation                         = 0.0;

  const char*       MName                               = "SAT_Widget";
  int32_t           MCursor                             = SAT_CURSOR_DEFAULT;
  const char*       MHint                               = "";

//------------------------------
public:
//------------------------------

  SAT_Widget(SAT_Rect ARect)
  : SAT_BaseWidget() {
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

  virtual uint32_t          getIndex()                    { return MIndex; }
  virtual SAT_WidgetLayout* getLayout()                   { return &MLayout; }
  virtual uint32_t          getLayoutAlignment()          { return MLayout.alignment; }
  virtual uint32_t          getLayoutAnchor()             { return MLayout.anchor; }
  virtual uint32_t          getLayoutCrop()               { return MLayout.crop; }
  virtual SAT_Rect          getLayoutInnerBorder()        { return MLayout.innerBorder; }
  virtual SAT_Rect          getLayoutOuterBorder()        { return MLayout.outerBorder; }
  virtual uint32_t          getLayoutStretch()            { return MLayout.stretch; }
  virtual double            getModulation()               { return MModulation; }
  virtual const char*       getName()                     { return MName; }
  virtual void*             getParameter()                { return MParameters[MParameterIndex]; }
  virtual void*             getParameter(uint32_t AIndex) { return MParameters[AIndex]; }
  virtual SAT_Rect          getRect()                     { return MRect; }
  virtual double            getValue()                    { return MValues[MValueIndex]; }
  virtual double            getValue(uint32_t AIndex)     { return MValues[AIndex]; }
  virtual uint32_t          getValueIndex()               { return MValueIndex; }

  virtual bool              isActive()                    { return MState.active; }
  virtual bool              isDisabled()                  { return MState.disabled; }
  virtual bool              isInteractive()               { return MState.interactive; }
  virtual bool              isVisible()                   { return MState.visible; }

//------------------------------

  virtual void setIndex(uint32_t AIndex)                        { MIndex = AIndex; }
  virtual void setLayout(SAT_WidgetLayout* ALayout)             { memcpy(&MLayout,ALayout,sizeof(SAT_WidgetLayout)); }
  virtual void setLayoutAlignment(uint32_t AAlignment)          { MLayout.alignment = AAlignment; }
  virtual void setLayoutAnchor(uint32_t AAnchor)                { MLayout.anchor = AAnchor; }
  virtual void setLayoutCrop(uint32_t ACrop)                    { MLayout.crop = ACrop; }
  virtual void setLayoutStretch(uint32_t AStretch)              { MLayout.stretch = AStretch; }
  virtual void setLayoutInnerBorder(SAT_Rect ABorder)           { MLayout.innerBorder = ABorder; }
  virtual void setLayoutOuterBorder(SAT_Rect ABorder)           { MLayout.outerBorder = ABorder; }
  virtual void setModulation(double AValue)                     { MModulation = AValue; }
  virtual void setName(const char* AName)                       { MName = AName; }
  virtual void setOwner(SAT_WidgetOwner* AOwner)                { MOwner = AOwner; }
  virtual void setParameter(void* AParameter)                   { MParameters[MParameterIndex] = AParameter; }
  virtual void setParameter(void* AParameter, uint32_t AIndex)  { MParameters[AIndex] = AParameter; }
  virtual void setParameterIndex(uint32_t AIndex)               { MParameterIndex = AIndex; }
  virtual void setParent(SAT_Widget* AParent)                   { MParent = AParent; }
  virtual void setRect(SAT_Rect ARect)                          { MRect = ARect; }
  virtual void setValue(double AValue)                          { MValues[MValueIndex] = AValue; }
  virtual void setValue(double AValue, uint32_t AIndex)         { MValues[AIndex] = AValue; }
  virtual void setValueIndex(uint32_t AIndex)                   { MValueIndex = AIndex; }

  virtual void setActive(bool AState=true)                      { MState.active = AState; }
  virtual void setDisabled(bool AState=true)                    { MState.disabled = AState; }
  virtual void setInteractive(bool AState=true)                 { MState.interactive = AState; }
  virtual void setVisible(bool AState=true)                     { MState.visible = AState; }

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
      SAT_Widget* widget = MChildren[i];
      //widget->
      delete widget;
    }
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

  virtual SAT_Widget* findWidget(int32_t AXpos, int32_t AYpos/*, bool ARecursive=true*/) {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* widget = MChildren[i];
      SAT_Rect widget_rect = widget->getRect();
      if (widget_rect.contains(AXpos,AYpos)) {
        /*if (ARecursive)*/ return widget->findWidget(AXpos,AYpos);
        //return widget;
      }
    }
    return this;
  }

  //----------

  virtual void paintChildren(SAT_PaintContext* AContext/*, bool ARecursive=true*/) {
    //SAT_PRINT("\n");
    uint32_t numchildren = MChildren.size();
    if (numchildren > 0) {
      // SAT_Painter* painter = AContext->painter;
      // SAT_Rect rect = getRect();
      // painter->pushClip(rect);
      for (uint32_t i=0; i<numchildren; i++) {
        SAT_Widget* widget = MChildren[i];
        // if (widget->isVisible()) {
          // SAT_Rect widgetrect = widget->getRect();
          // if (widgetrect.intersects(rect)) {
            widget->on_widget_paint(AContext);
          // }
        // }
      }
      // painter->popClip();
    }
  }

  //----------

  virtual void realignChildren(/*bool ARecursive=true*/) {
    //SAT_PRINT("\n");
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* widget = MChildren[i];
      widget->on_widget_realign();
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
  }
  
  //----------

  // called from:
  //   SAT_WidgetWindow.on_window_resize

  void on_widget_resize(uint32_t AWidth, uint32_t AHeight) override {
    //do_widget_realign(MParent,SAT_WIDGET_REALIGN_SIZE);
  }
  
  //----------

  // called from:
  //   SAT_Widget.paintChildren
  //   SAT_WidgetWindow.on_window_paint  

  void on_widget_paint(SAT_PaintContext* AContext) override {
    paintChildren(AContext);
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
    if (MOptions.autoCursor) do_widget_set_cursor(this,MCursor);
    if (MOptions.autoHint) do_widget_set_hint(this,MHint);
  }
  
  //----------

  void on_widget_leave(SAT_Widget* ATo, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    if (MOptions.autoCursor) do_widget_set_cursor(this,SAT_CURSOR_DEFAULT);
    if (MOptions.autoHint) do_widget_set_hint(this,"");
  }

  //----------

  void on_widget_tween(uint32_t AId, uint32_t AType, uint32_t ACount, double* AData) override {
  }

  //----------

  void on_widget_timer(uint32_t AId, double ADelta) override {
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
  
  void do_widget_mouse_capture(SAT_Widget* AWidget) override {
    if (MParent) MParent->do_widget_mouse_capture(AWidget);
  }
  
  void do_widget_key_capture(SAT_Widget* AWidget) override {
    if (MParent) MParent->do_widget_key_capture(AWidget);
  }
  
  void do_widget_modal(SAT_Widget* AWidget) override {
    if (MParent) MParent->do_widget_modal(AWidget);
  }

  void do_widget_set_cursor(SAT_Widget* AWidget, int32_t ACursor) override {
    if (MParent) MParent->do_widget_set_cursor(AWidget,ACursor);
  }
  
  void do_widget_set_hint(SAT_Widget* AWidget, const char* AHint) override {
    if (MParent) MParent->do_widget_set_hint(AWidget,AHint);
  }
  
};

//----------------------------------------------------------------------
#endif
