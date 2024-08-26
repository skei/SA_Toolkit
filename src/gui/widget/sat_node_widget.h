#ifndef sat_node_widget_included
#define sat_node_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_scroll_box_widget.h"
#include "gui/widget/sat_text_widget.h"
#include "gui/widget/sat_visual_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_NodeWidget;

#define SAT_NODE_PIN_ENABLED            0x01
#define SAT_NODE_PIN_INPUT              0x02
#define SAT_NODE_PIN_SIGNAL             0x04
#define SAT_NODE_PIN_HOVERING           0x08
#define SAT_NODE_PIN_CONNECTING         0x10

#define SAT_NODE_NOTIFY_MOUSE_MOVE      0
#define SAT_NODE_NOTIFY_BEGIN_DRAG      1
#define SAT_NODE_NOTIFY_END_DRAG        2
#define SAT_NODE_NOTIFY_CONNECT         3
#define SAT_NODE_NOTIFY_BEGIN_HOVER     4
#define SAT_NODE_NOTIFY_END_HOVER       5
#define SAT_NODE_NOTIFY_BEGIN_CONNECT   6
#define SAT_NODE_NOTIFY_END_CONNECT     7
#define SAT_NODE_NOTIFY_UPDATE_MOUSE_CONNECT 8

class SAT_NodePin {
public:
  SAT_NodeWidget* owner = nullptr;
  uint32_t        index = 0;
  uint32_t        flags = 0;
  const char*     name  = "";
};

typedef SAT_Array<SAT_NodePin*> SAT_NodePinArray;

//----------------------------------------------------------------------

class SAT_NodeWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  double              MTitleHeight        = 15;
  double              MPinWidth           = 7;
  double              MPinHeight          = 7;
  double              MPinHDist           = 3;
  double              MPinVDist           = 3;
  SAT_VisualWidget*   MContentWidget      = nullptr;
  SAT_TextWidget*     MTitleWidget        = nullptr;
  bool                MVertical           = false;
  SAT_NodePinArray    MInputPins          = {};
  SAT_NodePinArray    MOutputPins         = {};

  SAT_Point           MMousePos           = {0,0};

  bool                MIsHovering         = false;
  SAT_NodePin*        MHoverPin           = nullptr;
  SAT_NodePin*        MPrevHoverPin       = nullptr;

  bool                MIsConnecting       = false;

  SAT_Color           MConnectColor       = SAT_BrightRed;

  SAT_Color           MSignalColor        = SAT_LighterGrey;
  SAT_Color           MSignalHoverColor   = SAT_DarkerGrey;
  
  SAT_Color           MDataColor          = SAT_DarkerGrey;
  SAT_Color           MDataHoverColor     = SAT_LighterGrey;

//------------------------------
public:
//------------------------------

  SAT_NodeWidget(SAT_Rect ARect, const char* AName="", bool AVertical=false)
  : SAT_VisualWidget(ARect) {

    MVertical = AVertical;

    setName("SAT_NodeWidget");
    setHint("SAT_NodeWidget");

    setDrawBorder(false);
    setFillBackground(true);
    setBackgroundColor(0.47);

    setMovable(true);
    Options.wantHoverEvents = true;
    Options.autoHint = false;

    double pin_w = (MPinWidth + MPinHDist);
    double pin_h = (MPinHeight + MPinVDist);

    SAT_Rect title_rect;
    SAT_Rect content_rect;

    if (MVertical) {
      title_rect.x = pin_w;
      title_rect.y = 0.0;
      title_rect.w = ARect.w - (2.0 * pin_w);
      title_rect.h = MTitleHeight;
      content_rect.x = title_rect.x;
      content_rect.y = title_rect.y + MTitleHeight;
      content_rect.w = title_rect.w;
      content_rect.h = ARect.h - title_rect.h;
      setMovableOffset(SAT_Rect( pin_w, 0, pin_w, 0 ));
    }
    else {
      title_rect.x = 0.0;
      title_rect.y = pin_h;
      title_rect.w = ARect.w;
      title_rect.h = MTitleHeight;
      content_rect.x = title_rect.x;
      content_rect.y = title_rect.y + MTitleHeight;
      content_rect.w = title_rect.w;
      content_rect.h = ARect.h - title_rect.h - (2.0 * (MPinHeight + MPinVDist));
      setMovableOffset(SAT_Rect( 0, pin_h, 0, pin_h ));
    }

    MTitleWidget = new SAT_TextWidget(title_rect,AName);
    SAT_VisualWidget::appendChild(MTitleWidget);

    MTitleWidget->setFillBackground(true);
    MTitleWidget->setBackgroundColor(SAT_DarkerGrey);
    MTitleWidget->setDrawText(true);
    MTitleWidget->setDrawParamText(false);
    MTitleWidget->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    MTitleWidget->setTextColor(SAT_White);
    MTitleWidget->setTextOffset(SAT_Rect(3,0,0,0));
    MTitleWidget->setTextSize(8);
    MTitleWidget->setActive(false,false);

    MContentWidget = new SAT_VisualWidget(content_rect);
    SAT_VisualWidget::appendChild(MContentWidget);
    MContentWidget->setDrawBorder(true);
    MContentWidget->setFillBackground(true);
    MContentWidget->setBackgroundColor(0.53);

  }

  //----------

  virtual ~SAT_NodeWidget() {
    #ifndef SAT_NO_AUTODELETE
      deletePins();
    #endif
  }

//------------------------------
public:
//------------------------------

  virtual double            getTitleHeight()    { return MTitleHeight; }
  virtual double            getPinWidth()       { return MPinWidth; }
  virtual double            getPinHeight()      { return MPinHeight; }
  virtual double            getPinHDist()       { return MPinHDist; }
  virtual double            getPinVDist()       { return MPinVDist; }
  virtual SAT_VisualWidget* getContentWidget()  { return MContentWidget; }
  virtual SAT_TextWidget*   getTitleWidget()    { return MTitleWidget; }
  virtual bool              isVertical()        { return MVertical; }

//------------------------------
public:
//------------------------------

  SAT_Widget* appendChild(SAT_Widget* AWidget) override {
    return MContentWidget->appendChild(AWidget);
  }

//------------------------------
public: // pins
//------------------------------

  void appendPin(const char* AName, bool ASignal, bool AInput) {
    SAT_NodePin* pin = new SAT_NodePin();
    pin->owner = this;
    pin->name = AName;
    pin->flags = SAT_NODE_PIN_ENABLED;
    if (ASignal) pin->flags |= SAT_NODE_PIN_SIGNAL;
    if (AInput) {
      pin->index = MInputPins.size();
      pin->flags |= SAT_NODE_PIN_INPUT;
      MInputPins.append(pin);
    }
    else {
      pin->index = MOutputPins.size();
      MOutputPins.append(pin);
    }
  }

  //----------

  void deletePins() {
    for (uint32_t i=0; i<MInputPins.size(); i++)  { delete MInputPins[i]; }
    for (uint32_t i=0; i<MOutputPins.size(); i++) { delete MOutputPins[i]; }
  }

  //----------

  SAT_NodePin* getPin(uint32_t AIndex, bool AInput) {
    if (AInput) return MInputPins[AIndex];
    else return MOutputPins[AIndex];
  }

  //----------

  SAT_Rect getPinRect(uint32_t AIndex, bool AInput=true) {
    SAT_Rect rect = getRect();
    SAT_Rect pin_rect = {0,0,0,0};
    double scale = getWindowScale();
    double pw = (MPinWidth  + MPinHDist) * scale;
    double ph = (MPinHeight + MPinVDist) * scale;
    pin_rect.w = (MPinWidth * scale);
    pin_rect.h = (MPinHeight * scale);
    if (MVertical) {
      if (AInput) {
        pin_rect.x = rect.x;
        pin_rect.y = rect.y + (AIndex * ph);
      }
      else {
        pin_rect.x = rect.x2() - (MPinWidth * scale);
        pin_rect.y = rect.y + (AIndex * ph);
      }      
    }
    else { // horizontal
      if (AInput) {
        pin_rect.x = rect.x + (AIndex * pw);
        pin_rect.y = rect.y;
      }
      else {
        pin_rect.x = rect.x + (AIndex * pw);
        pin_rect.y = rect.y2() - (MPinHeight * scale);

      }
    }
    return pin_rect;
  }

  //----------

  SAT_Point getPinCenter(uint32_t AIndex, bool AInput=true) {
    SAT_Rect rect = getPinRect(AIndex,AInput);
    SAT_Point center;
    center.x = rect.x + (rect.w * 0.5);
    center.y = rect.y + (rect.h * 0.5);
    return center;
  }

  //----------

  SAT_NodePin* findPin(int32_t AXpos, int32_t AYpos) {
    // inputs
    for (uint32_t i=0; i<MInputPins.size(); i++) {
      SAT_NodePin* pin = MInputPins[i];
      SAT_Rect pin_rect = getPinRect(i,true);
      if (pin_rect.contains(AXpos,AYpos)) return pin;
    }
    // outputs
    for (uint32_t i=0; i<MOutputPins.size(); i++) {
      SAT_NodePin* pin = MOutputPins[i];
      SAT_Rect pin_rect = getPinRect(i,false);
      if (pin_rect.contains(AXpos,AYpos)) return pin;
    }
    return nullptr;
  }

//------------------------------
public:
//------------------------------

  void resetConnectingFlags() {
    for (uint32_t i=0; i<MInputPins.size(); i++) {
      MInputPins[i]->flags &= ~SAT_NODE_PIN_CONNECTING;
    }
    for (uint32_t i=0; i<MOutputPins.size(); i++) {
      MOutputPins[i]->flags &= ~SAT_NODE_PIN_CONNECTING;
    }
  }

   //----------

  void resetHoverFlags() {
    for (uint32_t i=0; i<MInputPins.size(); i++) {
      MInputPins[i]->flags &= ~SAT_NODE_PIN_HOVERING;
    }
    for (uint32_t i=0; i<MOutputPins.size(); i++) {
      MOutputPins[i]->flags &= ~SAT_NODE_PIN_HOVERING;
    }
  }

  //----------

  void updateMouseHover(int32_t AXpos, int32_t AYpos) {
    resetHoverFlags();
    SAT_NodePin* pin = findPin(AXpos,AYpos);
    if (pin) {
      pin->flags |= SAT_NODE_PIN_HOVERING;
      if (pin != MPrevHoverPin) {
        if (pin) {
//          do_widget_notify(this,SAT_NODE_NOTIFY_BEGIN_HOVER,pin);
          const char* hint = pin->name;
          do_widget_set_hint(this,hint);
          do_widget_redraw(this);
          //do_widget_set_cursor(SAT_CURSOR_FINGER);
        }
      }
    }
    else {
      if (MPrevHoverPin) {
//        do_widget_notify(this,SAT_NODE_NOTIFY_END_HOVER,MPrevHoverPin);
        do_widget_set_hint(this," "); // "" is ignored?
        do_widget_redraw(this);
        //do_widget_set_cursor(SAT_CURSOR_DEFAULT);
     }
    }
    MPrevHoverPin = pin;
  }

//------------------------------
public:
//------------------------------

  virtual void drawPins(SAT_PaintContext* AContext) {
    SAT_Painter* painter = AContext->painter;

    double scale = getWindowScale();
    painter->setLineWidth(1.0 * scale);

    // inputs
    for (uint32_t i=0; i<MInputPins.size(); i++) {
      SAT_NodePin* pin = MInputPins[i];
      SAT_Rect r = getPinRect(i,true);
      if (pin->flags & SAT_NODE_PIN_SIGNAL) {
        painter->setFillColor(MSignalColor);
        painter->fillRect(r.x,r.y,r.w,r.h);
        if (pin->flags & SAT_NODE_PIN_CONNECTING) {
          painter->setDrawColor(MConnectColor);
          painter->drawRect(r.x,r.y,r.w,r.h);
        }
        else if (pin->flags & SAT_NODE_PIN_HOVERING) {
          painter->setDrawColor(SAT_DarkerGrey);
          painter->drawRect(r.x,r.y,r.w,r.h);
        }
      }
      else {
        painter->setFillColor(SAT_DarkerGrey);
        painter->fillRect(r.x,r.y,r.w,r.h);
        if (pin->flags & SAT_NODE_PIN_CONNECTING) {
          painter->setDrawColor(MConnectColor);
          painter->drawRect(r.x,r.y,r.w,r.h);
        }
        else if (pin->flags & SAT_NODE_PIN_HOVERING) {
          painter->setDrawColor(SAT_LighterGrey);
          painter->drawRect(r.x,r.y,r.w,r.h);
        }
      }
    }

    // outputs
    for (uint32_t i=0; i<MOutputPins.size(); i++) {
      SAT_NodePin* pin = MOutputPins[i];
      SAT_Rect r = getPinRect(i,false);
      if (pin->flags & SAT_NODE_PIN_SIGNAL) {
        painter->setFillColor(MSignalColor);
        painter->fillRect(r.x,r.y,r.w,r.h);
        if (pin->flags & SAT_NODE_PIN_CONNECTING) {
          painter->setDrawColor(MConnectColor);
          painter->drawRect(r.x,r.y,r.w,r.h);
        }
        else if (pin->flags & SAT_NODE_PIN_HOVERING) {
          painter->setDrawColor(MSignalHoverColor);
          painter->drawRect(r.x,r.y,r.w,r.h);
        }
      }
      else {
        painter->setFillColor(MDataColor);
        painter->fillRect(r.x,r.y,r.w,r.h);
        if (pin->flags & SAT_NODE_PIN_CONNECTING) {
          painter->setDrawColor(MConnectColor);
          painter->drawRect(r.x,r.y,r.w,r.h);
        }
        else if (pin->flags & SAT_NODE_PIN_HOVERING) {
          painter->setDrawColor(MDataHoverColor);
          painter->drawRect(r.x,r.y,r.w,r.h);
        }
      }
    }

  }

//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    SAT_VisualWidget::on_widget_mouse_click(AXpos,AYpos,AButton,AState,ATime);
    if ((AButton == SAT_BUTTON_LEFT) && (MIsConnecting == false)) {
      SAT_NodePin* pin = findPin(AXpos,AYpos);
      if (pin) {
        MIsConnecting = true;
        do_widget_notify(this,SAT_NODE_NOTIFY_BEGIN_DRAG,pin);
      }
    }
  }

  //----------

  void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    SAT_VisualWidget::on_widget_mouse_release(AXpos,AYpos,AButton,AState,ATime);
    if ((AButton == SAT_BUTTON_LEFT) && (MIsConnecting == true)) {
      MIsConnecting = false;
      do_widget_notify(this,SAT_NODE_NOTIFY_END_DRAG,nullptr);
      do_widget_notify(this,SAT_NODE_NOTIFY_CONNECT,nullptr);

    }
  }

  //----------

  void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    SAT_VisualWidget::on_widget_mouse_move(AXpos,AYpos,AState,ATime);
    MMousePos.x = AXpos;
    MMousePos.y = AYpos;
    do_widget_notify(this,SAT_NODE_NOTIFY_MOUSE_MOVE,&MMousePos);
    if (MIsConnecting) {
      // updateMouseConnect(AXpos,AYpos);
      do_widget_notify(this,SAT_NODE_NOTIFY_UPDATE_MOUSE_CONNECT,&MMousePos);
    }
    else {
      if (!isMoving() && !isResizing()) {
        updateMouseHover(AXpos,AYpos);
      }
    }
  }

  //----------

  void on_widget_leave(SAT_Widget* ATo, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    SAT_VisualWidget::on_widget_leave(ATo,AXpos,AYpos,ATime);
    if (MPrevHoverPin) {
      //SAT_PRINT("no pin (leave)\n");
//      do_widget_notify(this,SAT_NODE_NOTIFY_END_HOVER,MPrevHoverPin);
      MPrevHoverPin = nullptr;
      resetHoverFlags();
      do_widget_redraw(this);
      //do_widget_set_hint(this," ");
    }
  }

  //----------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    paintChildren(AContext);
    drawPins(AContext);
    drawIndicators(AContext);
    drawBorder(AContext);
  };

};

//----------------------------------------------------------------------
#endif
