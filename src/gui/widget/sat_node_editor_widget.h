#ifndef sat_node_editor_widget_included
#define sat_node_editor_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_scroll_box_widget.h"
#include "gui/widget/sat_node_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_NodeEditorWidget
//: public SAT_VisualWidget {
: public SAT_ScrollBoxWidget {

//------------------------------
private:
//------------------------------

  bool            MConnectSrcActive = false;
  SAT_NodeWidget* MConnectSrcNode   = nullptr;
  SAT_NodePin*    MConnectSrcPin    = nullptr;
  SAT_Rect        MConnectSrcRect   = {0,0,0,0};

  bool            MConnectDstActive = false;
  SAT_NodeWidget* MConnectDstNode   = nullptr;
  SAT_NodePin*    MConnectDstPin    = nullptr;
  SAT_Rect        MConnectDstRect   = {0,0,0,0};

  double MMouseXpos = 0.0;
  double MMouseYpos = 0.0;

//------------------------------
public:
//------------------------------

  SAT_NodeEditorWidget(SAT_Rect ARect)
  //: SAT_VisualWidget(ARect) {
  : SAT_ScrollBoxWidget(ARect,true,true) {
    setName("SAT_NodeEditorWidget");
    setHint("SAT_NodeEditorWidget");
    //Options.autoClip = true;
    Options.wantHoverEvents = true;
  }

  //----------

  virtual ~SAT_NodeEditorWidget() {
  }

//------------------------------
public:
//------------------------------

  // virtual uint32_t getNumChildren() {
  //   return MChildren.size();
  // }

  //----------

  // virtual SAT_Widget* getChild(uint32_t AIndex) {
  //   return MChildren[AIndex];
  // }


//------------------------------
public:
//------------------------------

  virtual void drawWires(SAT_PaintContext* AContext) {
    SAT_Rect rect = getRect();
    double scale = getWindowScale();
    SAT_Painter* painter = AContext->painter;
    SAT_VisualWidget* content = getContentWidget();

    SAT_NodeWidget* node1 = (SAT_NodeWidget*)content->getChild(0);
    SAT_NodeWidget* node2 = (SAT_NodeWidget*)content->getChild(1);
    SAT_Point p1 = node1->getPinCenter(0,false);
    SAT_Point p2 = node2->getPinCenter(0,true);
    painter->setLineWidth(2.0 * scale);
    painter->setDrawColor(SAT_DarkerGrey);
    painter->drawLine(p1.x,p1.y,p2.x,p2.y);

  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    paintChildren(AContext);
    drawWires(AContext);

    SAT_Painter* painter= AContext->painter;
    double scale = getWindowScale();

    if (MConnectSrcActive) {
      painter->setDrawColor(SAT_BrightRed);
      painter->setLineWidth(1.0 * scale);
      painter->drawRect(MConnectSrcRect.x,MConnectSrcRect.y,MConnectSrcRect.w,MConnectSrcRect.h);
      // dragging wire
      double x1 = MConnectSrcRect.x + (MConnectSrcRect.w * 0.5);
      double y1 = MConnectSrcRect.y + (MConnectSrcRect.h * 0.5);
      double x2 = MMouseXpos;
      double y2 = MMouseYpos;
      painter->setLineWidth(2 * scale);
      painter->setDrawColor(SAT_Black);
      painter->drawLine(x1,y1,x2,y2);
    }

    if (MConnectDstActive) {
      painter->setDrawColor(SAT_BrightGreen);
      painter->setLineWidth(1.0 * scale);
      painter->drawRect(MConnectDstRect.x,MConnectDstRect.y,MConnectDstRect.w,MConnectDstRect.h);
    }

    drawIndicators(AContext);
    drawBorder(AContext);
  };

//------------------------------
public:
//------------------------------

  void do_widget_notify(SAT_Widget* AWidget, int32_t AValue, void* APtr=nullptr) override {
    switch (AValue) {

      case SAT_NODE_PIN_MOUSE_MOVE: {
        SAT_Point* point = (SAT_Point*)APtr;
        MMouseXpos = point->x;
        MMouseYpos = point->y;
        break;
      }

      case SAT_NODE_PIN_SRC_ACTIVE: {
        MConnectSrcPin = (SAT_NodePin*)APtr;
        if (!MConnectSrcPin) return;
        MConnectSrcActive = true;
        uint32_t index = MConnectSrcPin->index;
        bool input = false;
        if (MConnectSrcPin->flags & SAT_NODE_PIN_INPUT) input = true;
        MConnectSrcNode = MConnectSrcPin->owner;
        if (MConnectSrcNode) {
          MConnectSrcRect = MConnectSrcNode->getPinRect(index,input);
          MConnectSrcNode->do_widget_redraw(MConnectSrcNode);
        }
        break;
      }

      case SAT_NODE_PIN_SRC_INACTIVE: {
        MConnectSrcActive = false;
        if (MConnectSrcNode) {
          MConnectSrcNode->do_widget_redraw(MConnectSrcNode);
        }
        break;
      }

      case SAT_NODE_PIN_DST_ACTIVE: {
        MConnectDstPin = (SAT_NodePin*)APtr;
        if (!MConnectDstPin) return;
        MConnectDstActive = true;
        uint32_t index = MConnectDstPin->index;
        bool input = false;
        if (MConnectDstPin->flags & SAT_NODE_PIN_INPUT) input = true;
        MConnectDstNode = MConnectDstPin->owner;
        if (MConnectDstNode) {
          MConnectDstRect = MConnectDstNode->getPinRect(index,input);
          MConnectDstNode->do_widget_redraw(MConnectDstNode);
        }
        break;
      }

      case SAT_NODE_PIN_DST_INACTIVE: {
        MConnectDstActive = false;
        if (MConnectDstNode) {
          MConnectDstNode->do_widget_redraw(MConnectDstNode);
        }
        break;
      }
      
    }
  }

  //----------

  // a module has been moved (or resized)..
  
  // void do_widget_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_SELF) override {
  //   SAT_ScrollBoxWidget::do_widget_realign(AWidget,AMode);
  //   //SAT_PRINT("%s\n",AWidget->getName());
  // }

  //----------

  // a module (or selection retangle) wants to be redrawn..
  // because of the wires (on top), we want to redraw the entire node editor..
  
  void do_widget_redraw(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_SELF) override {
    SAT_ScrollBoxWidget::do_widget_redraw(this,AIndex,SAT_WIDGET_REDRAW_SELF);
  }

};

//----------------------------------------------------------------------
#endif
