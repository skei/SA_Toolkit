#ifndef sat_graph_widget_included
#define sat_graph_widget_included
//----------------------------------------------------------------------

/*
  todo:
  - check all drawRect, etc  -> x1,y1,x2,y2 -> x,m,w,h
  - wire endpoint calculations are wrong..
*/

#include "base/type/sat_list.h"
#include "gui/widget/sat_visual_widget.h"
#include "gui/sat_widget.h"

struct  SAT_GraphPin;
class   SAT_GraphWire;
class   SAT_GraphModule;
struct  SAT_Graph;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define MODULE_WIDTH    75
#define MODULE_HEIGHT   25

#define PIN_WIDTH       7
#define PIN_XDIST       9

#define PIN_HEIGHT      5
#define PIN_YDIST       7

#define MAX_PINS        16
//#define BASE_HEIGHT     (MODULE_HEIGHT - (PIN_YDIST * 2) - 2)
#define BASE_HEIGHT     (MODULE_HEIGHT - (PIN_YDIST * 2))

//----------

enum SAT_PinType {
  SAT_PIN_DATA    = 0,
  SAT_PIN_SIGNAL  = 1,
};

//----------------------------------------------------------------------
//
// wire
//
//----------------------------------------------------------------------

class SAT_GraphWire
: public SAT_ListNode {

//------------------------------
public:
//------------------------------

  SAT_GraphModule*  inModule  = nullptr;
  int32_t           inPin     = 0;
  SAT_GraphModule*  outModule = nullptr;
  int32_t           outPin    = 0;

//------------------------------
public:
//------------------------------

  SAT_GraphWire(SAT_GraphModule* AInModule, int32_t AInPin, SAT_GraphModule* AOutModule, int32_t AOutPin) {
    inModule  = AInModule;
    inPin     = AInPin;
    outModule = AOutModule;
    outPin    = AOutPin;
  }

};

//----------------------------------------------------------------------
//
// module
//
//----------------------------------------------------------------------

class SAT_GraphModule
: public SAT_ListNode {

//------------------------------
public:
//------------------------------

  const char* name              = "module";
  double      xpos              = 0;
  double      ypos              = 0;
  int32_t     numInputs         = 0;
  int32_t     numOutputs        = 0;
  uint32_t    inputs[MAX_PINS]  = {0};
  uint32_t    outputs[MAX_PINS] = {0};
  bool        selected          = false;
  bool        hovering          = false;

//  bool        has_been_scaled   = false;
//  double      orig_xpos         = 0.0;
//  double      orig_ypos         = 0.0;

//  double      S                 = 1.0;

//------------------------------
public:
//------------------------------

//  void setpos(double x, double y) {
//    xpos = x;
//    ypos = y;
//    //orig_xpos = y;
//    //orig_ypos = y;
//  }

  bool inside(double x, double y, double AScale) {
    //double S = getWindowScale();
    if ( (x >= xpos) && (x <  xpos + (MODULE_WIDTH  * AScale))
      && (y >= ypos) && (y <  ypos + (MODULE_HEIGHT * AScale))) return true;
    return false;
  }

  bool insideBase(double x, double y, double AScale) {
    //double S = getWindowScale();
    if (y < (ypos + (PIN_YDIST * AScale) + (BASE_HEIGHT * AScale))) return true;
    return false;
  }

  int32_t insideInput(double x, double y, double AScale) {
    //double S = getWindowScale();
    if (y < (ypos + (PIN_YDIST * AScale))) {
      int32_t pin = (x - xpos) / (PIN_XDIST * AScale);
      if (pin < numInputs) return pin;
    }
    return -1;
  }

  int32_t insideOutput(double x, double y, double AScale) {
    //double S = getWindowScale();
    int32_t pin = (x - xpos) / (PIN_XDIST * AScale);
    if (pin < numOutputs) return pin;
    return -1;
  }

};

//----------------------------------------------------------------------
//
// graph
//
//----------------------------------------------------------------------

class SAT_GraphWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  SAT_List      MModules        = {};
  SAT_List      MWires          = {};
  SAT_Painter*  MPainter        = nullptr;
  SAT_Rect      MPaintRect      = SAT_Rect(0,0);
  double        MModuleNameSize = 10.0;
  double        MPaintedScale   = 1.0;

//  bool          has_been_scaled = false;
//  SAT_Rect      orig_rect       = SAT_Rect(0,0);
//  double        MCurrentScale   = 1.0;

//  double MScale = 1.0;

//------------------------------
protected:
//------------------------------

  SAT_Color         MModuleColor              = SAT_Color(0.75);//SAT_COLOR_LIGHT_GRAY;
  SAT_Color         MSelectedModuleColor      = SAT_Color(0.25);//SAT_COLOR_DARK_GRAY;

  SAT_Color         MModuleBorder             = SAT_Color(0.25);//SAT_COLOR_DARK_GRAY;
  SAT_Color         MSelectedModuleBorder     = SAT_Color(0.75);//SAT_COLOR_LIGHT_GRAY;

  SAT_Color         MSignalPinColor           = SAT_Color(0.75);//SAT_COLOR_LIGHT_GRAY;
  SAT_Color         MDataPinColor             = SAT_Color(0.25);//SAT_COLOR_DARK_GRAY;
  SAT_Color         MSignalPinHoverColor      = SAT_Color(0);//SAT_COLOR_BLACK;
  SAT_Color         MDataPinHoverColor        = SAT_Color(1);//SAT_COLOR_WHITE;

  SAT_Color         MModuleNameColor          = SAT_Color(0.25);//SAT_COLOR_DARK_GRAY;
  SAT_Color         MSelectedModuleNameColor  = SAT_Color(0.75);//SAT_COLOR_LIGHT_GRAY;

  SAT_Color         MSignalWireColor          = SAT_Color(0.75);//SAT_COLOR_LIGHT_GRAY;
  SAT_Color         MDataWireColor            = SAT_Color(0.25);//SAT_COLOR_DARK_GRAY;

  bool              MDraggingModules          = false;
  bool              MDraggingWire             = false;
  bool              MDraggingSelect           = false;
  // hovering
  SAT_GraphModule*  MHoverModule              = nullptr;
  int32_t           MHoverInput               = -1;
  int32_t           MHoverOutput              = -1;
  double            MMousePrevX               = 0;
  double            MMousePrevY               = 0;
  // wire
  double            MDragWireX1               = 0;
  double            MDragWireY1               = 0;
  double            MDragWireX2               = 0;
  double            MDragWireY2               = 0;
  SAT_GraphModule*  MDragWireModule           = nullptr;
  int32_t           MDragWireInput            = -1;
  int32_t           MDragWireOutput           = -1;
  // selection
  double            MDragSelectX1             = 0;
  double            MDragSelectY1             = 0;
  double            MDragSelectX2             = 0;
  double            MDragSelectY2             = 0;

//------------------------------
public:
//------------------------------

  SAT_GraphWidget(SAT_Rect ARect=SAT_Rect())
  : SAT_VisualWidget(ARect) {
    setName("SAT_GraphWidget");
    //setWidgetName("SAT_GraphWidget");
    //setHint("graph");
    clearSelection();
    //setFlag(SAT_WIDGET_HOVER);
    ////setFlag(SAT_WIDGET_CLIP);
    setFillBackground(false);
    setBackgroundColor( SAT_Color(0.55f) );
  }

  //----------

  virtual ~SAT_GraphWidget() {
    #ifndef SAT_NO_AUTODELETE
      deleteModules();
      deleteWires();
    #endif
  }

//------------------------------
public:
//------------------------------

  /*
    'unscale' current position
    'rescale' to new..
  */

  ///// TEMPORARY !!!!

    double pre_align_width  = 0;
    double pre_align_height = 0;
    double post_align_width  = 0;
    double post_align_height = 0;
    
  /////

  void on_widget_preAlign() override {
    // pre_align_width = getRect().w;
    // pre_align_height = getRect().h;
  }

  //----------

  void on_widget_postAlign() override {
    double scale = getWindowScale();
    //SAT_PRINT("scale %.3f painted %.3f\n",scale,MPaintedScale);
    // SAT_Rect rect = getRect();
    // post_align_width = rect.w;
    // post_align_height = rect.h;
    // double xratio = 1.0;
    // double yratio = 1.0;
    // if (pre_align_width > 0.0) xratio = (post_align_width / pre_align_width);
    // if (pre_align_height > 0.0) yratio = (post_align_height / pre_align_height);
    // //SAT_PRINT("ratio %.3f,%.3f\n",xratio,yratio);
    if (scale != MPaintedScale) {
      double ratio = scale / MPaintedScale;
      SAT_ListNode* node = MModules.head();
      while (node) {
        SAT_GraphModule* module = (SAT_GraphModule*)node;
        module->xpos *= ratio;
        module->ypos *= ratio;
        node = node->next();
      }
    }
  }

//------------------------------
public: // modules
//------------------------------

  //void appendModule(SAT_GraphModule* AModule) {
  //  MModules.append(AModule);
  //}

  //----------

  void deleteModules(void) {
    SAT_ListNode* node = MModules.head();
    while (node) {
      SAT_ListNode* next = node->next();
      delete node;
      node = next;
    }
  }

  //----------

  void addModule(SAT_GraphModule* AModule, double AXpos, double AYpos, const char* AName="") {
    AModule->xpos = AXpos;
    AModule->ypos = AYpos;
    //AModule->setpos(AXpos,AYpos);
    AModule->selected = false;
    AModule->hovering = false;
    AModule->name = AName;
    MModules.insertTail(AModule);
  }

  //----------

  void removeModule(SAT_GraphModule* AModule) {
    MModules.removeNode(AModule);
  }

  //----------

  void removeSelectedModules(void) {
    SAT_ListNode* node = MModules.head();
    while (node) {
      SAT_GraphModule* module = (SAT_GraphModule*)node;
      node = node->next();
      if (module->selected) {
        removeWires(module);
        removeModule(module);
        //delete module; // module.destroy;
      }
    }
  }

//----------------------------------------
public: // wires
//----------------------------------------

  void appendWire(SAT_GraphWire* AWire) {
    MWires.insertTail(AWire);
  }

  //----------

  void deleteWires(void) {
    SAT_ListNode* node = MWires.head();
    while (node) {
      SAT_ListNode* next = node->next();
      delete node;
      node = next;
    }

  }

  //----------

  void addWire(SAT_GraphWire* AWire) {
    MWires.insertTail(AWire);
  }

  //----------

  void removeWire(SAT_GraphWire* AWire) {
    MWires.removeNode(AWire);
  }

  //----------

  // remove all input wires connected to a specific pin on a module

  void removeInputWires(SAT_GraphModule* AModule, int32_t APin) {
    SAT_ListNode* node = MWires.head();
    while (node) {
      SAT_GraphWire* wire = (SAT_GraphWire*)node;
      node = node->next();
      if ((AModule == wire->inModule) && (APin == wire->inPin))
      /*or ( (AModule = wire.outModule) and (APin = wire.outPin) )*/ {
        removeWire(wire);
        delete wire; // wire.Destroy;
      }
      //node = node.next;
    }
  }

  //----------

  // remove all input wires connected to a specific pin on a module

  void removeOutputWires(SAT_GraphModule* AModule, int32_t APin) {
    SAT_ListNode* node = MWires.head();
    while (node) {
      SAT_GraphWire* wire = (SAT_GraphWire*)node;
      node = node->next();
      if ((AModule == wire->outModule) && (APin == wire->outPin)  )
      /*or ( (AModule = wire.outModule) and (APin = wire.outPin) )*/ {
        removeWire(wire);
        delete wire;//wire.Destroy;
      }
      //node = node.next;
    }
  }

  //----------

  // remove all wires connected to a module

  void removeWires(SAT_GraphModule* AModule) {
    //node : KListNode;
    //ire : SAT_GraphWire;
    SAT_ListNode* node = MWires.head();
    while (node) {
      SAT_GraphWire* wire = (SAT_GraphWire*)node;
      node = node->next();
      if ((AModule == wire->inModule) || (AModule == wire->outModule)) {
        removeWire(wire);
        delete wire;//wire.Destroy;
      }
      //node = node.next;
    }
  }

  //----------

  // find wire, input or output

  SAT_GraphWire* findWire(SAT_GraphModule* AModule, int32_t APin) {
    SAT_ListNode* node = MWires.head();
    while (node) {
      SAT_GraphWire* wire = (SAT_GraphWire*)node;
      if ( ((AModule == wire->inModule ) && (APin == wire->inPin ))
        || ((AModule == wire->outModule) && (APin == wire->outPin)) ) return wire;//System.exit(wire);
      node = node->next();
    }
    return nullptr;
  }

  //----------

  SAT_GraphWire* findWire(SAT_GraphModule* AInModule, int32_t AInPin, SAT_GraphModule* AOutModule, int32_t AOutPin) {
    SAT_ListNode* node = MWires.head();
    while (node) {
      SAT_GraphWire* wire = (SAT_GraphWire*)node;
      if ( (AInModule  == wire->inModule)
        && (AInPin     == wire->inPin)
        && (AOutModule == wire->outModule)
        && (AOutPin    == wire->outPin) ) return wire;
      node = node->next();
    }
    return nullptr;
  }

//----------------------------------------
public: // selection
//----------------------------------------

  bool hasSelection(void) {
    SAT_ListNode* node = MModules.head();
    while (node) {
      SAT_GraphModule* module = (SAT_GraphModule*)node;
      if (module->selected) return true;
      node = node->next();
    }
    return false;
  }

  //----------

  void clearSelection(void) {
    SAT_ListNode* node = MModules.head();
    while (node) {
      SAT_GraphModule* module = (SAT_GraphModule*)node;
      module->selected = false;
      node = node->next();
    }
  }

  //----------

  void selectModule(SAT_GraphModule* AModule) {
    AModule->selected = true;
  }

  //----------

  void deselectModule(SAT_GraphModule* AModule) {
    AModule->selected = false;
  }

  //----------

  void toggleModule(SAT_GraphModule* AModule) {
    AModule->selected = AModule->selected ? false : true;
  }

  //----------

  bool isSelected(SAT_GraphModule* AModule) {
    return AModule->selected;
  }

  //----------

  void selectModules(double x1, double y1, double x2, double y2) {
    double S = getWindowScale();
    x1 -= getRect().x;
    y1 -= getRect().y;
    x2 -= getRect().x;
    y2 -= getRect().y;
    double temp;
    if (x1 > x2) {
      temp = x1;
      x1 = x2;
      x2 = temp;
    }
    if (y1 > y2) {
      temp = y1;
      y1 = y2;
      y2 = temp;
    }
    SAT_ListNode* node = MModules.head();
    while (node) {
      SAT_GraphModule* module = (SAT_GraphModule*)node;
      if ( ( module->xpos                <= x2)
        && ((module->xpos+(MODULE_WIDTH*S))  >= x1)
        && ( module->ypos                <= y2)
        && ((module->ypos+(MODULE_HEIGHT*S)) >= y1)) {
        module->selected = true;
      }
      node = node->next();
    } // while
  }


  //----------

  void moveSelectedModules(double AXdelta, double AYdelta) {
    SAT_ListNode* node = MModules.head();
    while (node) {
      SAT_GraphModule* module = (SAT_GraphModule*)node;
      if (module->selected) {
        module->xpos = module->xpos + AXdelta;
        module->ypos = module->ypos + AYdelta;
//        module->orig_xpos = module->xpos;
//        module->orig_ypos = module->ypos;
      }
      node = node->next();
    }
  }

//----------------------------------------
public: // dragging
//----------------------------------------

  void startDragInput(SAT_GraphModule* AModule, int32_t APin) {
    double S = getWindowScale();
    MDraggingWire   = true;
    MDragWireInput  = APin;
    MDragWireOutput = -1;
    MDragWireModule = AModule;
    MDragWireX1     = getRect().x + AModule->xpos + (APin * (PIN_XDIST * S)) + ((PIN_WIDTH * S) * 0.5);
    MDragWireY1     = getRect().y + AModule->ypos + ((PIN_HEIGHT * S) * 0.5);
    MDragWireX2     = MDragWireX1;
    MDragWireY2     = MDragWireY1;
  }

  //----------

  void startDragOutput(SAT_GraphModule* AModule, int32_t APin) {
    double S = getWindowScale();
    MDraggingWire   = true;
    MDragWireInput  = -1;
    MDragWireOutput = APin;
    MDragWireModule = AModule;
    MDragWireX1     = getRect().x + AModule->xpos + (APin * (PIN_XDIST * S)) + ((PIN_WIDTH * S) * 0.5);
    MDragWireY1     = getRect().y + AModule->ypos + ((MODULE_HEIGHT * S) - 1) - ((PIN_HEIGHT * S) * 0.5);
    MDragWireX2     = MDragWireX1;
    MDragWireY2     = MDragWireY1;
  }

  //----------

  void startDragSelect(double AXpos, double AYpos) {
    MDraggingSelect = true;
    MDragSelectX1   = AXpos;
    MDragSelectY1   = AYpos;
    MDragSelectX2   = AXpos;
    MDragSelectY2   = AYpos;
  }

//----------------------------------------
public: // draw
//----------------------------------------

  void drawModule(SAT_GraphModule* AModule) {

    double scale = getWindowScale();
    double xdist  = PIN_XDIST * scale;
    double ydist  = PIN_YDIST * scale;
    double pinw   = PIN_WIDTH * scale;
    double pinh   = PIN_HEIGHT * scale;
    SAT_Rect rect = getRect();
    
    SAT_Rect module_full_rect = SAT_Rect(
      rect.x + (AModule->xpos /** scale*/),
      rect.y + (AModule->ypos /** scale*/),
      (MODULE_WIDTH * scale),
      (MODULE_HEIGHT * scale)
    );

    SAT_Rect module_body_rect = module_full_rect;
    module_body_rect.shrink(0,ydist,0,ydist);

    SAT_Color color;

    SAT_Rect input_rect = module_body_rect;
    input_rect.y -= (ydist);
    input_rect.setSize(pinw,pinh);

    SAT_Rect output_rect = module_body_rect;
    output_rect.y += (output_rect.h + ydist - pinh);
    output_rect.setSize(pinw,pinh);

    // body

    if (AModule->selected) color = MSelectedModuleColor; else color = MModuleColor;
    MPainter->setFillColor(color);
    MPainter->fillRect(module_body_rect.x,module_body_rect.y,module_body_rect.w,module_body_rect.h);

    // inputs

    for (int32_t i=0; i<AModule->numInputs; i++) {
      // fill
      if (AModule->inputs[i] == SAT_PIN_SIGNAL) color = MSignalPinColor; else color = MDataPinColor;
      MPainter->setFillColor(color);
      MPainter->fillRect(input_rect.x,input_rect.y,input_rect.w,input_rect.h);

      // border

      if ((AModule == MHoverModule) && (i == MHoverInput) && (!MDraggingModules)) {
        if (AModule->inputs[i] == SAT_PIN_SIGNAL) color = MSignalPinHoverColor; else color = MDataPinHoverColor;
        MPainter->setLineWidth(1.0 * scale);
        MPainter->setDrawColor(color);
        MPainter->drawRect(input_rect.x,input_rect.y,input_rect.w,input_rect.h);
      }
      input_rect.x += xdist;
    }

    // outputs

    for (int32_t i=0; i<AModule->numOutputs; i++) {
      if (AModule->outputs[i] == SAT_PIN_SIGNAL) color = MSignalPinColor; else color = MDataPinColor;
      MPainter->setFillColor(color);
      MPainter->fillRect(output_rect.x,output_rect.y,output_rect.w,output_rect.h);

      // border

      if ((AModule == MHoverModule) && (i == MHoverOutput) && (!MDraggingModules)) {
        if (AModule->outputs[i] == SAT_PIN_SIGNAL) color = MSignalPinHoverColor; else color = MDataPinHoverColor;
        MPainter->setDrawColor(color);
        MPainter->setLineWidth(1.0 * scale);
        MPainter->drawRect(output_rect.x,output_rect.y,output_rect.w,output_rect.h);
      }
      output_rect.x += xdist;
    }

    // name

    if (AModule->selected) color = MSelectedModuleNameColor; else color = MModuleNameColor;
    MPainter->setTextSize(MModuleNameSize * scale);
    MPainter->setTextColor(color);
    MPainter->drawTextBox(module_body_rect, AModule->name, SAT_TEXT_ALIGN_CENTER);

    // border

    color = MModuleBorder;
    MPainter->setDrawColor(color);
    MPainter->setLineWidth(1.0 * scale);
    MPainter->drawRect(module_body_rect.x,module_body_rect.y,module_body_rect.w,module_body_rect.h);

  }

  //----------

  void drawWire(SAT_GraphWire* AWire) {

    double scale = getWindowScale();
    double xdist  = PIN_XDIST  * scale;
    double ydist  = PIN_YDIST  * scale;
    double pinw   = PIN_WIDTH  * scale;
    double pinh   = PIN_HEIGHT * scale;

    double x1 = getRect().x + AWire->outModule->xpos + (AWire->outPin * xdist);// + 2;
    double y1 = getRect().y + AWire->outModule->ypos;// + 31 - 2;

    double x2 = getRect().x + AWire->inModule->xpos + (AWire->inPin * xdist);// + 2;
    double y2 = getRect().y + AWire->inModule->ypos;// + 2;

    x1 += (pinw * 0.5);
    y1 += (pinh * 0.5) + (MODULE_HEIGHT * scale) - ydist;
    x2 += (pinw * 0.5);
    y2 += (pinh * 0.5);

    //if (AWire->outModule->outputs[AWire->outPin] == kpt_signal)
    //  MPainter->drawLine(x1,y1,x2,y2,MSignalWireColor,1);
    //else
    //  MPainter->drawLine(x1,y1,x2,y2,MDataWireColor,1);

    SAT_Color color;
    if (AWire->outModule->outputs[AWire->outPin] == SAT_PIN_SIGNAL) color = MSignalWireColor;
    else color = MDataWireColor;

    MPainter->setDrawColor(color);
    MPainter->setLineWidth(2 * scale);
    MPainter->drawLine(x1,y1,x2,y2);

  }

  //----------

  void drawModules(void) {
    SAT_ListNode* node = MModules.head();
    while (node) {
      SAT_GraphModule* module = (SAT_GraphModule*)node;
      drawModule(module);
      node = node->next();
    }
  }

  //----------

  void drawWires(void) {
    SAT_ListNode* node = MWires.head();
    while (node) {
      SAT_GraphWire* wire = (SAT_GraphWire*)node;
      drawWire(wire);
      node = node->next();
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    SAT_Painter* painter = AContext->painter;
    //MScale = getWindowScale();
    double scale = getWindowScale();
    MPaintedScale = scale;
    fillBackground(AContext);
    MPainter = AContext->painter;
    //MPaintRect = AContext->update_rect;
    
    drawModules();
    drawWires();
    
    if (MDraggingWire) {
      painter->setDrawColor( SAT_Black );
      painter->setLineWidth(0.5 * scale);
      painter->drawLine(MDragWireX1,MDragWireY1,MDragWireX2,MDragWireY2);
    }
    
    if (MDraggingSelect) {
      float w = MDragSelectX2 - MDragSelectX1;
      float h = MDragSelectY2 - MDragSelectY1;
      painter->setDrawColor( SAT_White );
      painter->setLineWidth(0.5 * scale);
      painter->drawRect(MDragSelectX1,MDragSelectY1,w,h);
    }
    
    paintChildren(AContext);
    drawBorder(AContext);

  }

  //----------

  // AXpos, AYpos = 'world' coordinates
  // x,y = local coords (graph)

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
      SAT_ListNode* node;
      SAT_GraphModule* module;
      int32_t pin;
      bool changed;
      double x,y;

      x = AXpos - getRect().x;
      y = AYpos - getRect().y;
      double scale = getWindowScale();

      SAT_Assert(scale > 0.0);
      //x /= scale;
      //y /= scale;

      changed = false;
      if (AButton == SAT_BUTTON_LEFT) {
        MMousePrevX = AXpos;
        MMousePrevY = AYpos;
        module = nullptr;
        node = MModules.tail();
        while (node) {
          module = (SAT_GraphModule*)node;
          if (module->inside(x,y,scale)) {

            // inputs

            pin = module->insideInput(x,y,scale);
            if ((pin >= 0) && (!MDraggingModules)) {
              startDragInput(module,pin);
              break;
            } else

            // base

            if (module->insideBase(x,y,scale)) {
              if (AState & SAT_STATE_CTRL) {
                toggleModule(module);
                changed = true;
              }
              else {
                if (!module->selected) clearSelection();
                selectModule(module);
                MDraggingModules = true;
                changed = true;
              }
              break;
            } else

            // outputs

            pin = module->insideOutput(x,y,scale);
            if ((pin >= 0) && (!MDraggingModules)) {
              startDragOutput(module,pin);
              break;
            }

          } // inside module
          node = node->prev();
        } // while

        if (!node) {

          if (hasSelection()) {
            if (!(AState & SAT_STATE_CTRL)) {
              clearSelection();
              changed = true;
            }
          }

          if (!MDraggingSelect) {
            if (!(AState & SAT_STATE_CTRL))
              clearSelection();
            startDragSelect(AXpos,AYpos);
          }

        } // node
      }
      if (changed) do_widget_redraw(this,0);
      SAT_VisualWidget::on_widget_mouse_click(AButton,AState,AXpos,AYpos,ATime);
  }

  //----------

  void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {

      SAT_GraphWire* wire;
      bool changed;

      //AXpos -= FRect.x;
      //AYpos -= FRect.y;

      changed = false;
      if (AButton == SAT_BUTTON_LEFT) {

        if (MDraggingModules) {
          MDraggingModules = false;
          changed = true;
        }

        if (MDraggingWire) {
          MDraggingWire = false;
          changed = true;

          if (MDragWireModule) {

            if (MDragWireInput >= 0) {
              if (MHoverOutput >= 0) {
                wire = findWire(MDragWireModule,MDragWireInput,MHoverModule,MHoverOutput);
                if (wire) {
                  removeWire(wire);
                  delete wire;
                }
                else {
                  addWire( new SAT_GraphWire(MDragWireModule,MDragWireInput, MHoverModule, MHoverOutput) ); // in -> out
                }
              }
              else {
                if (MDragWireInput != MHoverInput)
                removeInputWires(MDragWireModule,MDragWireInput);
              }

            } // input

            if (MDragWireOutput >= 0) {
              if (MHoverInput >= 0) {
                wire = findWire(MHoverModule,MHoverInput,MDragWireModule,MDragWireOutput);
                if (wire) {
                  removeWire(wire);
                  delete wire;
                }
                else {
                  addWire( new SAT_GraphWire(MHoverModule,MHoverInput,MDragWireModule,MDragWireOutput) ); // in -> out
                }
              } // output
              else {
                if (MDragWireOutput != MHoverOutput)
                  removeOutputWires(MDragWireModule,MDragWireOutput);
              }
            }

          } // dragWireModule
        } // dragging wire

        if (MDraggingSelect) {
          MDraggingSelect = false;
          changed = true;
        }

      }

      if (changed) do_widget_redraw(this,0);
      //inherited;
      SAT_VisualWidget::on_widget_mouse_release(AButton,AState,AXpos,AYpos,ATime);
  }

  //----------

  void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
      double mx = AXpos - getRect().x;
      double my = AYpos - getRect().y;
      double scale = getWindowScale();

      SAT_Assert(scale > 0.0);
      double x = mx;// /= scale;
      double y = my;// /= scale;

      bool changed = false;
      SAT_ListNode* node = (SAT_GraphModule*)MModules.tail();
      while (node) {
        SAT_GraphModule* module = (SAT_GraphModule*)node;
        if (module->inside(x,y,scale)) {

          // inputs

          int32_t pin = module->insideInput(x,y,scale);
          if (pin >= 0) {
            MHoverModule = module; // used when drawing modules
            if ((MHoverInput != pin) && (!MDraggingSelect)) {
              MHoverInput = pin;
              MHoverOutput = -1;
              changed = true;
            }
            break;
          } else

          // base

          if (module->insideBase(x,y,scale)) {
            MHoverModule = module;
            if (MHoverInput != -1) {
              MHoverInput = -1;
              changed = true;
            }
            if (MHoverOutput != -1) {
              MHoverOutput = -1;
              changed = true;
            }
            break;
          } else

          // outputs

          pin = module->insideOutput(x,y,scale);
          if (pin >= 0) {
            MHoverModule = module;
            if ((MHoverOutput != pin) && !MDraggingSelect) {
              MHoverInput = -1;
              MHoverOutput = pin;
              changed = true;
            }
            break;
          }

        }
        node = node->prev();
      } // while

      if (!node) {
        MHoverModule = nullptr;
        if (MHoverInput != -1) {
          MHoverInput = -1;
          changed = true;
        }
        if (MHoverOutput != -1) {
          MHoverOutput = -1;
          changed = true;
        }
      }

      if (MDraggingWire) {
        MDragWireX2 = AXpos;
        MDragWireY2 = AYpos;
        changed = true;
      } else

      if (MDraggingModules) {
        moveSelectedModules(AXpos-MMousePrevX,AYpos-MMousePrevY);
        changed = true;
      } else

      if (MDraggingSelect) {
        MDragSelectX2 = AXpos;
        MDragSelectY2 = AYpos;
        clearSelection();
        selectModules(MDragSelectX1,MDragSelectY1,MDragSelectX2,MDragSelectY2);
        changed = true;
      }

      if (changed) do_widget_redraw(this,0);
      MMousePrevX = AXpos;
      MMousePrevY = AYpos;

      SAT_VisualWidget::on_widget_mouse_move(AState,AXpos,AYpos,ATime);
  }

  //----------

//  void on_widget_keyPress(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

//  void on_widget_keyRelease(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

  //------------------------------
  public:
  //------------------------------

//  void do_widgetListener_realign(SAT_Widget* ASender, bool ARecursive=true) override {
//    //SAT_PRINT;
//    //alignChildren(ARecursive);
//    findContentRect();
//  }

};

//----------------------------------------------------------------------

#undef PIN_WIDTH
#undef PIN_HEIGHT
#undef MODULE_WIDTH
#undef MODULE_HEIGHT
#undef PIN_XDIST
#undef PIN_YDIST
#undef MAX_PINS
#undef BASE_HEIGHT

//----------------------------------------------------------------------
#endif
