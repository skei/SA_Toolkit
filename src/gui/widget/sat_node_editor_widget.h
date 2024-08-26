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

class SAT_NodeEditorWidget;

struct SAT_NodeWire {
  SAT_NodeWidget* output_node;
  uint32_t        output_pin;
  SAT_NodeWidget* input_node;
  uint32_t        input_pin;
};

typedef SAT_Array<SAT_NodeWire*> SAT_NodeWireArray;

//----------------------------------------------------------------------

class SAT_NodeEditorWidget
: public SAT_ScrollBoxWidget {

//------------------------------
private:
//------------------------------

  SAT_NodeWireArray MWires              = {};

  double            MMouseXpos          = 0.0;
  double            MMouseYpos          = 0.0;

  SAT_Color         MWireColor          = SAT_DarkerGrey;
  double            MWireThickness      = 2.0;

  bool              MDragWire           = false;
  SAT_NodeWidget*   MDragWireNode       = nullptr;
  SAT_NodePin*      MDragWirePin        = nullptr;
  SAT_Point         MDragWireStart      = {0,0};
  SAT_Color         MDragWireColor      = SAT_Black;
  double            MDragWireThickness  = 1.5;

  SAT_Color         MPinDataColor       = SAT_DarkerGrey;
  SAT_Color         MPinSignalColor     = SAT_LighterGrey;

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
    #ifndef SAT_NO_AUTODELETE
      deleteWires();
    #endif
  }

//------------------------------
public: // pins
//------------------------------

  SAT_NodePin* findPin(int32_t AXpos, int32_t AYpos) {

    SAT_VisualWidget* content = getContentWidget();
    if (content) {
      uint32_t num = content->getNumChildren();
      for (uint32_t i=0; i<num; i++) {
        SAT_NodeWidget* node = (SAT_NodeWidget*)content->getChild(i);
        SAT_NodePin* pin = node->findPin(AXpos,AYpos);
        if (pin) return pin;
      }
    }


    return nullptr;
  }

//------------------------------
public: // wires
//------------------------------

  void appendWire(SAT_NodeWidget* AOutputNode, uint32_t AOutputPin, SAT_NodeWidget* AInputNode, uint32_t AInputPin) {
    SAT_NodeWire* wire = new SAT_NodeWire();
    wire->output_node = AOutputNode;
    wire->output_pin  = AOutputPin;
    wire->input_node  = AInputNode;
    wire->input_pin   = AInputPin;
    MWires.append(wire);
  }

  //----------

  void removeWire(SAT_NodeWidget* AOutputNode, uint32_t AOutputPin, SAT_NodeWidget* AInputNode, uint32_t AInputPin) {
    int32_t index = 0;
    index = findWire(AOutputNode,AOutputPin,AInputNode,AInputPin);
    if (index != -1) MWires.remove(index);
  }

  //----------

  void removeWires(SAT_NodeWidget* ANode, uint32_t APin, bool AInput) {
    //SAT_NodePin* pin = ANode->qwe;
    int32_t index = findWire(ANode,APin,AInput);
    while (index != -1) {
      MWires.remove(index);
      index = findWire(ANode,APin,AInput);
    }
  }

  //----------

  void deleteWires() {
    for (uint32_t i=0; i<MWires.size(); i++)  {
      delete MWires[i];
    }
  }

  //----------

  int32_t findWire(SAT_NodeWidget* AOutputNode, uint32_t AOutputPin, SAT_NodeWidget* AInputNode, uint32_t AInputPin) {
    for (uint32_t i=0; i<MWires.size(); i++) {
      if ((AOutputNode == MWires[i]->output_node) &&
          (AInputNode == MWires[i]->input_node) &&
          (AOutputPin  == MWires[i]->output_pin) &&
          (AInputPin  == MWires[i]->input_pin) ) return i;
    }
    return -1;
  }

  //----------

  int32_t findWire(SAT_NodeWidget* ANode, uint32_t APin, bool AInput) {
    for (uint32_t i=0; i<MWires.size(); i++) {
      if (AInput) {
        if ((ANode == MWires[i]->input_node) && (APin  == MWires[i]->input_pin)) return i;
      }
      else {
        if ((ANode == MWires[i]->output_node) && (APin  == MWires[i]->output_pin)) return i;
      }
    }
    return -1;
  }

  //----------

  virtual void drawWires(SAT_PaintContext* AContext) {
    SAT_Rect rect = getRect();
    double scale = getWindowScale();
    SAT_Painter* painter = AContext->painter;
    SAT_VisualWidget* content = getContentWidget();
    for (uint32_t i=0; i<MWires.size(); i++) {
      SAT_NodeWidget* node1 = MWires[i]->output_node;
      SAT_NodeWidget* node2 = MWires[i]->input_node;
      SAT_Point p1 = node1->getPinCenter(MWires[i]->output_pin,false);
      SAT_Point p2 = node2->getPinCenter(MWires[i]->input_pin,true);
      painter->setLineWidth(MWireThickness * scale);
      painter->setDrawColor(MWireColor);
      painter->drawLine(p1.x,p1.y,p2.x,p2.y);
    }
  }

//------------------------------
public: // mouse
//------------------------------

  // void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
  //   SAT_ScrollBoxWidget::on_widget_mouse_move(AXpos,AYpos,AState,ATime);
  // }

//------------------------------
public: // paint
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    paintChildren(AContext);
    drawWires(AContext);

    SAT_Painter* painter= AContext->painter;
    double scale = getWindowScale();

    if (MDragWire) {
      double x1 = MDragWireStart.x;
      double y1 = MDragWireStart.y;
      double x2 = MMouseXpos;
      double y2 = MMouseYpos;
      painter->setDrawColor(MDragWireColor);
      painter->setLineWidth(MDragWireThickness * scale);
      painter->drawLine(x1,y1,x2,y2);
    }

    drawIndicators(AContext);
    drawBorder(AContext);
  };

//------------------------------
public: // notify
//------------------------------

  void do_widget_notify(SAT_Widget* AWidget, int32_t AValue, void* APtr/*=nullptr*/) override {
    switch (AValue) {

      case SAT_NODE_NOTIFY_MOUSE_MOVE: {
        //SAT_PRINT("SAT_NODE_NOTIFY_MOUSE_MOVE\n");
        SAT_Point* point = (SAT_Point*)APtr;
        MMouseXpos = point->x;
        MMouseYpos = point->y;
        break;
      }

      case SAT_NODE_NOTIFY_BEGIN_DRAG: {
        //SAT_PRINT("SAT_NODE_NOTIFY_BEGIN_DRAG\n");
        SAT_NodePin* pin = (SAT_NodePin*)APtr;
        SAT_NodeWidget* node = pin->owner;
        uint32_t index = pin->index;
        MDragWire = true;
        MDragWireNode = node;
        MDragWirePin = pin;
        MDragWireStart = node->getPinCenter(index,(pin->flags & SAT_NODE_PIN_INPUT));
        break;
      }

      case SAT_NODE_NOTIFY_END_DRAG: {
        //SAT_PRINT("SAT_NODE_NOTIFY_END_DRAG\n");
        MDragWire = false;
        MDragWireStart = {0,0};
        break;
      }

      case SAT_NODE_NOTIFY_CONNECT: {
        //SAT_PRINT("SAT_NODE_NOTIFY_CONNECT\n");
        SAT_NodePin* pin = MConnectPin;
        SAT_NodeWidget* src_node = MDragWireNode;
        uint32_t src_index = MDragWirePin->index;
        if (pin) {
          pin->flags &= ~SAT_NODE_PIN_CONNECTING;
          SAT_NodeWidget* dst_node = pin->owner;
          uint32_t dst_index  = pin->index;
          if ((src_node != dst_node)) {
            if (pin->flags & SAT_NODE_PIN_INPUT) {
              if ((MDragWirePin->flags & SAT_NODE_PIN_INPUT) != 0) return;
              connectNodes(src_node,src_index,dst_node,dst_index);
            }
            else {
              if ((MDragWirePin->flags & SAT_NODE_PIN_INPUT) == 0) return;
              connectNodes(dst_node,dst_index,src_node,src_index);
            }
          }
        }
        else {
          disconnectAllNodes(src_node,src_index,(MDragWirePin->flags & SAT_NODE_PIN_INPUT));
        } // src = dst
        break;
      }

      case SAT_NODE_NOTIFY_BEGIN_HOVER: {
        break;
      }

      case SAT_NODE_NOTIFY_END_HOVER: {
        break;
      }

      case SAT_NODE_NOTIFY_BEGIN_CONNECT: {
        break;
      }


      case SAT_NODE_NOTIFY_END_CONNECT: {
        break;
      }

      case SAT_NODE_NOTIFY_UPDATE_MOUSE_CONNECT: {
        SAT_Point* point = (SAT_Point*)APtr;
        updateMouseConnect(point->x,point->y);
        break;
      }

    }
  }

//------------------------------
public: // redraw
//------------------------------

  // a module has been moved (or resized)..
  // because of the wires (on top), we want to redraw the entire node editor..
  
  void do_widget_redraw(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_SELF) override {
    SAT_ScrollBoxWidget::do_widget_redraw(this,0,SAT_WIDGET_REDRAW_SELF);
  }

//------------------------------
public: // connections
//------------------------------

  virtual void connectNodes(SAT_NodeWidget* AOutputNode, uint32_t AOutputPin, SAT_NodeWidget* AInputNode, uint32_t AInputPin) {
    if (findWire(AOutputNode,AOutputPin,AInputNode,AInputPin) == -1) appendWire(AOutputNode,AOutputPin,AInputNode,AInputPin);
    else disconnectNodes(AOutputNode,AOutputPin,AInputNode,AInputPin);
  }

  //----------

  virtual void disconnectNodes(SAT_NodeWidget* AOutputNode, uint32_t AOutputPin, SAT_NodeWidget* AInputNode, uint32_t AInputPin) {
    removeWire(AOutputNode,AOutputPin,AInputNode,AInputPin);
  }

  //----------

  virtual void disconnectAllNodes(SAT_NodeWidget* ANode, uint32_t APin, bool AInput) {
    removeWires(ANode,APin,AInput);
  }

//------------------------------
private: // update mouse connect
//------------------------------

  SAT_NodePin*    MPrevConnectPin = nullptr;
  SAT_NodeWidget* MConnectNode    = nullptr;
  SAT_NodePin*    MConnectPin     = nullptr;

  void updateMouseConnect(double AXpos, double AYpos) {
    //SAT_TRACE;
    SAT_NodePin* pin = findPin(AXpos,AYpos);
    if (pin) {
      if (pin != MPrevConnectPin) {
        pin->flags |= SAT_NODE_PIN_CONNECTING;
        SAT_NodeWidget* owner = pin->owner;
        owner->do_widget_redraw(owner);
        MConnectNode = owner;
        MPrevConnectPin = pin;
        MConnectPin = pin;
      }
    }
    else {
      if (MPrevConnectPin) {
        MPrevConnectPin->flags &= ~SAT_NODE_PIN_CONNECTING;
        MConnectNode = nullptr;
        MConnectPin = nullptr;
        //SAT_NodeWidget* owner = MPrevConnectPin->owner;
        //owner->do_widget_notify(owner,SAT_NODE_NOTIFY_END_CONNECT,MPrevConnectPin);
        //owner->do_widget_redraw(owner);
        MPrevConnectPin = nullptr;
      } // prev
    }
  }
};

//----------------------------------------------------------------------
#endif
