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
  uint32_t  anchor        = SAT_EDGE_NONE;
  bool      crop_layout   = false;
  SAT_Rect  inner_border  = {};
  SAT_Rect  outer_border  = {};
};

//----------

struct SAT_WidgetState {
  bool active       = true;
  bool visible      = true;
  bool interacting  = false;
};

//----------

struct SAT_WidgetOptions {
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

  SAT_Rect            MInitialRect                = {};
  SAT_Rect            MBaseRect                   = {};
  SAT_Rect            MContentRect                = {};

  SAT_Point           MMouseClickedPos            = {0,0};
  uint32_t            MMouseClickedButton         = SAT_BUTTON_NONE;
  bool                MMouseWaitingForMovement    = false;

//------------------------------
private:
//------------------------------

  SAT_WidgetLayout    MLayout                     = {};
  SAT_WidgetState     MState                      = {};
  SAT_WidgetOptions   MOptions                    = {};

  const char*         MName                       = "MWidget";
  SAT_WidgetOwner*    MOwner                      = nullptr;
  SAT_Widget*         MParent                     = nullptr;
  uint32_t            MParentIndex                = 0;
  SAT_WidgetArray     MChildren                   = {};
//double              MValue                      = 0.0;
  double              MValues[SAT_WIDGET_MAX_VALUES]  = {};
  SAT_Rect            MRect                       = {};
  void*               MParameter                  = nullptr;
  uint32_t            MLastPainted                = 0;

//int32_t             MId                         = -1;
//SAT_WidgetOptions   MOptions                    = {};

  int32_t     MMouseCursor    = SAT_CURSOR_DEFAULT;
  const char* MHint           = "";

  int32_t    MSelectedIndex  = 0;

//------------------------------
public:
//------------------------------

  SAT_Widget(SAT_Rect ARect) {
    MRect = ARect;
    MBaseRect = ARect;
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

  /*virtual*/ const char*         getName()                                   { return MName; }
  /*virtual*/ int32_t             getCursor()                                 { return MMouseCursor; }
  /*virtual*/ const char*         getHint()                                   { return MHint; }
  /*virtual*/ double              getValue(uint32_t AIndex=0)                 { return MValues[AIndex]; }
  /*virtual*/ SAT_Widget*         getParent()                                 { return MParent; }
  /*virtual*/ void*               getParameter()                              { return MParameter; }

  /*virtual*/ int32_t             getSelectedValueIndex()                     { return MSelectedIndex; }
  /*virtual*/ double              getSelectedValue()                          { return MValues[MSelectedIndex]; }

  /*virtual*/ void                setName(const char* AName)                  { MName = AName; }
  /*virtual*/ void                setCursor(int32_t ACursor)                  { MMouseCursor = ACursor; }
  /*virtual*/ void                setHint(const char* AHint)                  { MHint = AHint; }
  /*virtual*/ void                setValue(double AValue, uint32_t AIndex=0)  { MValues[AIndex] = AValue; }

  /*virtual*/ void                selectValue(int32_t AIndex)                 { MSelectedIndex = AIndex; }

  // rect

  /*virtual*/ SAT_Point           getPos()                                    { return SAT_Point(MRect.x,MRect.y); }
  /*virtual*/ SAT_Point           getSize()                                   { return SAT_Point(MRect.w,MRect.h); }
  /*virtual*/ SAT_Rect            getRect()                                   { return MRect; }
  /*virtual*/ double              getWidth()                                  { return MRect.w; }
  /*virtual*/ double              getHeight()                                 { return MRect.h; }
  /*virtual*/ SAT_Rect            getBaseRect()                               { return MBaseRect; }

  /*virtual*/ void                setPos(SAT_Point APos)                      { MRect.setPos(APos); }
  /*virtual*/ void                setSize(SAT_Point ASize)                    { MRect.setSize(ASize); }
  /*virtual*/ void                setRect(SAT_Rect ARect)                     { MRect = ARect; }

  // layout

  /*virtual*/ SAT_WidgetLayout*   getLayout()                                 { return &MLayout; }
  /*virtual*/ uint32_t            getAnchor()                                 { return MLayout.anchor; }
  /*virtual*/ bool                getCropLayout()                             { return MLayout.crop_layout; }
  /*virtual*/ SAT_Rect            getInnerBorder()                            { return MLayout.inner_border; }
  /*virtual*/ SAT_Rect            getOuterBorder()                            { return MLayout.outer_border; }

  /*virtual*/ void                setAnchor(uint32_t AAnchor)                 { MLayout.anchor = AAnchor; }
  /*virtual*/ void                setCropLayout(bool AFill)                   { MLayout.crop_layout = AFill; }
  /*virtual*/ void                setInnerBorder(SAT_Rect ABorder)            { MLayout.inner_border = ABorder; }
  /*virtual*/ void                setOuterBorder(SAT_Rect ABorder)            { MLayout.outer_border = ABorder; }

  // state

  /*virtual*/ SAT_WidgetState*    getState()                                  { return &MState; }
  /*virtual*/ bool                isActive()                                  { return MState.active; }
  /*virtual*/ bool                isVisible()                                 { return MState.visible; }
  /*virtual*/ bool                isInteracting()                             { return MState.interacting; }

  /*virtual*/ void                setActive(bool AActive=true)                { MState.active = AActive; }
  /*virtual*/ void                setVisible(bool AVisible=true)              { MState.visible = AVisible; }
  /*virtual*/ void                setInteracting(bool AInteracting=true)      { MState.interacting = AInteracting; }

  // options

  /*virtual*/ SAT_WidgetOptions*  getOptions()                                { return &MOptions; }
  /*virtual*/ bool                autoHoverCursor()                           { return MOptions.autoHoverCursor; }
  /*virtual*/ bool                autoHoverHint()                             { return MOptions.autoHoverHint; }

  //

  /*virtual*/ uint32_t            getLastPainted()                            { return MLastPainted; }
  /*virtual*/ void                setLastPainted(uint32_t ALast)              { MLastPainted = ALast; }

//------------------------------
public:
//------------------------------

  // called from:
  // SAT_Window.on_window_open()

  virtual void setOwner(SAT_WidgetOwner* AOwner, bool ARecursive=true) {
    MOwner = AOwner;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->setOwner(AOwner,ARecursive);
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
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->setActive(AState,ARecursive);
      }
    }
  }

  //----------
  
  virtual void setVisible(bool AState=true, bool ARecursive=true) {
    MState.visible = AState;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->setVisible(AState,ARecursive);
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

  virtual bool isRecursivelyVisible() {
    if (!isVisible()) return false;
    if (!MParent) return true;
    return MParent->isRecursivelyVisible();
  }

  //----------

  //virtual void setRectAndBase(SAT_Rect ARect) {
  //  double S = getWindowScale();
  //  setRect(ARect);
  //  ARect.scale(1.0 / S);
  //  setBaseRect(ARect);
  //}

  //----------

  virtual void connect(void* AParameter) {
    MParameter = AParameter;
  }

  //----------

  virtual double getWindowWidth() {
    if (MOwner) return MOwner->on_widgetOwner_getWidth();
    else return 0.0;
  }
  
  //----------

  virtual double getWindowHeight() {
    if (MOwner) return MOwner->on_widgetOwner_getHeight();
    else return 0.0;
  }
  
  //----------

  virtual double getWindowScale() {
    if (MOwner) return MOwner->on_widgetOwner_getScale();
    else return 1.0;
  }

  //----------

  virtual void appendChildWidget(SAT_Widget* AWidget) {
    uint32_t index = MChildren.size();
    AWidget->setParent(this);
    AWidget->setParentIndex(index);
    MChildren.push_back(AWidget);
  }

  //----------

  virtual void deleteChildWidgets() {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      if (MChildren[i]) {
        delete MChildren[i];
        MChildren[i] = nullptr;
      }
    }
    MChildren.clear();
  }

  //----------

  // returns null if no child widgets at x,y
  // todo: ifActive

  virtual SAT_Widget* findChildWidget(uint32_t AXpos, uint32_t AYpos, bool ARecursive=true) {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* child = MChildren[i];
      SAT_Rect child_rect = child->getRect();
      if (child_rect.contains(AXpos,AYpos)) {
        if (ARecursive) {
          SAT_Widget* subchild = child->findChildWidget(AXpos,AYpos,ARecursive);
          if (subchild) return subchild;
        }
        return child;
      }
    }
    return nullptr;
  }

  //----------

  virtual SAT_Widget* findChildWidget(const char* AName, bool ARecursive=true) { return nullptr;
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* child = MChildren[i];
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
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      SAT_Widget* child = MChildren[i];
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
    MRect = MBaseRect;
    MRect.scale(AScale);
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->scaleWidget(AScale);
      }
    }
  }


  //----------

  virtual void realignChildWidgets() {
    SAT_Rect layout_rect = MRect;
    layout_rect.shrink(MLayout.inner_border);
    MContentRect = SAT_Rect(layout_rect.x,layout_rect.y,0,0);

    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* child = MChildren[i];
      SAT_WidgetState* child_state = child->getState();

      if (child_state->visible) {

        child->on_widget_realign();

        SAT_Rect child_rect = child->getBaseRect();
        SAT_WidgetLayout* child_layout = child->getLayout();

        child_rect.x += layout_rect.x;
        child_rect.y += layout_rect.y;

        uint32_t child_anchor = child_layout->anchor;
        uint32_t child_crop_layout = child_layout->crop_layout;

        // TODO: centering, percentages, stacking, ..

        if (child_anchor & SAT_EDGE_LEFT) {
          child_rect.x = layout_rect.x;
          if (child_crop_layout) {
            layout_rect.x += child_rect.w;
            layout_rect.w -= child_rect.w;
          }
        }
        if (child_anchor & SAT_EDGE_RIGHT) {
          child_rect.setX2( layout_rect.w - child_rect.w );
          if (child_crop_layout) {
            layout_rect.w -= child_rect.w;
          }
        }
        if (child_anchor & SAT_EDGE_TOP) {
          child_rect.y = layout_rect.y;
          if (child_crop_layout) {
            layout_rect.y += child_rect.h;
            layout_rect.h -= child_rect.h;
          }
        }
        if (child_anchor & SAT_EDGE_BOTTOM) {
          child_rect.setY2( layout_rect.h - child_rect.h );
          if (child_crop_layout) {
            layout_rect.h -= child_rect.h;
          }
        }
        MContentRect.combine(child_rect);
        child->setRect(child_rect);
        child->realignChildWidgets();
      } // visible
    } // for
  }

  //----------

  /*
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
  */

  virtual void paintChildWidgets(SAT_PaintContext* AContext) {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* child = MChildren[i];
      if (child->isVisible()) {
        SAT_Rect child_rect = child->getRect();
        if (child_rect.intersects(MRect)) {
          child->on_widget_paint(AContext);
          MChildren[i]->paintChildWidgets(AContext);
        } // intersects
      } // visible
    } // for
  }

//------------------------------
public: // on_ (downwards)
//------------------------------

  //virtual void on_widget_open(SAT_WidgetOwner* AOwner) {
  //}

  //virtual void on_widget_firstShown() {
  //}

  virtual void on_widget_move(double AXpos, double AYpos) {
  }

  virtual void on_widget_resize(double AWidth, double AHeight) {
  }

  virtual void on_widget_realign() {
  }

  virtual void on_widget_paint(SAT_PaintContext* AContext) {
    if (isVisible()) paintChildWidgets(AContext);
  }

  virtual void on_widget_timer(SAT_Timer* ATimer, double AElapsed) {
  }

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

  virtual void do_widget_update(SAT_Widget* AWidget) {
    if (MParent) MParent->do_widget_update(AWidget);
  }

  virtual void do_widget_redraw(SAT_Widget* AWidget) {
    if (MParent) MParent->do_widget_redraw(AWidget);
  }

  virtual void do_widget_realign(SAT_Widget* AWidget) {
    //if (MParent) MParent->do_widget_realign(AWidget);
    if (AWidget) {
      realignChildWidgets();
      do_widget_redraw(this);
    }
    else {
      if (MParent) MParent->do_widget_realign(this);
    }
  }

  virtual void do_widget_setCursor(SAT_Widget* AWidget, int32_t ACursor) {
    if (MParent) MParent->do_widget_setCursor(AWidget,ACursor);
  }

  virtual void do_widget_setHint(SAT_Widget* AWidget, const char* AHint) {
    if (MParent) MParent->do_widget_setHint(AWidget,AHint);
  }

  virtual void do_widget_setModal(SAT_Widget* AWidget) {
    if (MParent) MParent->do_widget_setModal(AWidget);
  }

  virtual void do_widget_captureKeys(SAT_Widget* AWidget) {
    if (MParent) MParent->do_widget_captureKeys(AWidget);
  }

  virtual void do_widget_close(SAT_Widget* AWidget) {
    if (MParent) MParent->do_widget_close(AWidget);
  }

  virtual void do_widget_select(SAT_Widget* AWidget, int32_t AIndex, int32_t ASubIndex=-1) {
    if (MParent) MParent->do_widget_select(AWidget,AIndex,ASubIndex);
  }
  
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
