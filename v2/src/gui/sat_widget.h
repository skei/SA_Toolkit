#ifndef sat_widget_included
#define sat_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_paint_context.h"
#include "gui/base/sat_widget_listener.h"
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
  bool      fill_layout   = true;
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

  SAT_Rect            MInitialRect              = {};
  SAT_Rect            MBaseRect                 = {};
  SAT_Rect            MContentRect              = {};

  SAT_Point           MMouseClickedPos          = {0,0};
  uint32_t            MMouseClickedButton       = SAT_BUTTON_NONE;
  bool                MMouseWaitingForMovement  = false;

//------------------------------
private:
//------------------------------

  const char*         MName                     = "";
  int32_t             MId                       = -1;

  SAT_WidgetOwner*    MOwner                    = nullptr;
  SAT_Widget*         MParent                   = nullptr;
  uint32_t            MParentIndex              = 0;
  SAT_WidgetArray     MChildren                 = {};
  double              MValue                    = 0.0;
  SAT_Rect            MRect                     = {};

  SAT_WidgetLayout    MLayout                   = {};
  SAT_WidgetState     MState                    = {};
  SAT_WidgetOptions   MOptions                  = {};

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

  virtual const char* getName()                     { return MName; }
  virtual int32_t     getId()                       { return MId; }
  virtual double      getValue(uint32_t AIndex=0)   { return MValue; }
  virtual SAT_Point   getPos()                      { return SAT_Point(MRect.x,MRect.y); }
  virtual SAT_Point   getSize()                     { return SAT_Point(MRect.w,MRect.h); }
  virtual SAT_Rect    getRect()                     { return MRect; }
  virtual double      getWidth()                    { return MRect.w; }
  virtual double      getHeight()                   { return MRect.h; }

  //----------

  uint32_t            getAnchor()                   { return MLayout.anchor; }
  bool                getFillLayout()               { return MLayout.fill_layout; }
  SAT_Rect            getInnerBorder()              { return MLayout.inner_border; }
  SAT_Rect            getOuterBorder()              { return MLayout.outer_border; }

  bool                isActive()                    { return MState.active; }
  bool                isVisible()                   { return MState.visible; }
  bool                isInteracting()               { return MState.interacting; }

  //

  SAT_Rect            getBaseRect()                 { return MBaseRect; }
  SAT_WidgetLayout*   getLayout()                   { return &MLayout; }
  SAT_WidgetState*    getState()                    { return &MState; }

//------------------------------
public:
//------------------------------

  virtual void  setName(const char* AName)                  { MName = AName; }
  virtual void  setId(int32_t AId)                          { MId = AId; }
  virtual void  setValue(double AValue, uint32_t AIndex=0)  { MValue = AValue; }
  virtual void  setPos(SAT_Point APos)                      { MRect.setPos(APos); }
  virtual void  setSize(SAT_Point ASize)                    { MRect.setSize(ASize); }
  virtual void  setRect(SAT_Rect ARect)                     { MRect = ARect; }

  //----------

  void          setAnchor(uint32_t AAnchor)                 { MLayout.anchor = AAnchor; }
  void          setFillLayout(bool AFill)                   { MLayout.fill_layout = AFill; }
  void          setInnerBorder(SAT_Rect ABorder)            { MLayout.inner_border = ABorder; }
  void          setOuterBorder(SAT_Rect ABorder)            { MLayout.outer_border = ABorder; }

  void          setActive(bool AActive=true)                { MState.active = AActive; }
  void          setVisible(bool AVisible=true)              { MState.visible = AVisible; }
  void          setInteracting(bool AInteracting=true)      { MState.interacting = AInteracting; }

//------------------------------
public:
//------------------------------

  virtual void setOwner(SAT_WidgetOwner* AOwner, bool ARecursive=true) {
    MOwner = AOwner;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->setOwner(AOwner,ARecursive);
      }
    }
  }

  //----------

  virtual void setParent(SAT_Widget* AWidget) {
    MParent = AWidget;
  }

  //----------

  virtual void setParentIndex(uint32_t AIndex) {
    MParentIndex = AIndex;
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

  virtual SAT_Widget* findChildWidget(int32_t AId, bool ARecursive=true) { return nullptr;
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* child = MChildren[i];
      int32_t id = child->getId();
      if (AId == id) {
        return child;
      }
      else {
        if (ARecursive) {
          SAT_Widget* subchild = child->findChildWidget(AId,ARecursive);
          if (subchild) return subchild;
        }
      }

    }
    return nullptr;
  }

  //----------

  virtual void realignChildWidgets() {

    //SAT_Rect parent_rect = MRect;

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
        uint32_t child_fill_layout = child_layout->fill_layout;

        // TODO: centering, percentages

        if (child_anchor & SAT_EDGE_LEFT) {
          child_rect.x = layout_rect.x;
          if (child_fill_layout) {
            layout_rect.x += child_rect.w;
            layout_rect.w -= child_rect.w;
          }
        }
        if (child_anchor & SAT_EDGE_RIGHT) {
          child_rect.setX2( layout_rect.w - child_rect.w );
          if (child_fill_layout) {
            layout_rect.w -= child_rect.w;
          }
        }
        if (child_anchor & SAT_EDGE_TOP) {
          child_rect.y = layout_rect.y;
          if (child_fill_layout) {
            layout_rect.y += child_rect.h;
            layout_rect.h -= child_rect.h;
          }
        }
        if (child_anchor & SAT_EDGE_BOTTOM) {
          child_rect.setY2( layout_rect.h - child_rect.h );
          if (child_fill_layout) {
            layout_rect.h -= child_rect.h;
          }
        }
        MContentRect.combine(child_rect);
        child->realignChildWidgets();
      } // visible
    } // for
  }

  //----------

  virtual void paintChildWidgets(SAT_PaintContext* AContext) {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      SAT_Widget* child = MChildren[i];
      if (child->getState()->visible) {
        SAT_Rect child_rect = child->getRect();
        if (child_rect.intersects(MRect)) {
          child->on_widget_paint(AContext);
          MChildren[i]->paintChildWidgets(AContext);
        } // intersects
      } // visible
    } // for
  }

//------------------------------
public:
//------------------------------

//virtual void on_widget_open(SAT_WidgetOwner* AOwner){}
//virtual void on_widget_firstShown() {}

  virtual void on_widget_move() {}
  virtual void on_widget_resize() {}
  virtual void on_widget_realign() {}                           // alignChildWidgets (pre-align)
  virtual void on_widget_paint(SAT_PaintContext* AContext) {}   // paintChildWidgets
  virtual void on_widget_timer() {}
  virtual void on_widget_mouseClick() {}
  virtual void on_widget_mouseDblClick() {}
  virtual void on_widget_mouseRelease() {}
  virtual void on_widget_mouseMove() {}
  virtual void on_widget_keyPress() {}
  virtual void on_widget_keyRelease() {}

  //----------

  virtual void do_widget_update_value(SAT_Widget* AWidget) {
    if (MParent) MParent->do_widget_update_value(AWidget);
  }

  //----------

  virtual void do_widget_redraw(SAT_Widget* AWidget) {
    if (MParent) MParent->do_widget_redraw(AWidget);
  }

  //----------

  virtual void do_widget_realign(SAT_Widget* AWidget) {
    if (MParent) MParent->do_widget_realign(AWidget);
  }

};

//----------------------------------------------------------------------
#endif
