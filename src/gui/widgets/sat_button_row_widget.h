#ifndef sat_button_row_widget_included
#define sat_button_row_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/sat_grid_widget.h"

#define SAT_MAX_STATES        32
#define SAT_BUTTON_ROW_SINGLE 0
#define SAT_BUTTON_ROW_MULTI  1
#define SAT_BUTTON_ROW_HORIZ 0
#define SAT_BUTTON_ROW_VERT  1


class SAT_ButtonRowWidget
: public SAT_GridWidget {

//------------------------------
protected:
//------------------------------

  int32_t     MMode                       = SAT_BUTTON_ROW_SINGLE;
  bool        MVertical                   = false;
  int32_t     MSelected                   = 0;
  bool        MStates[SAT_MAX_STATES]     = {0};
  const char* MLabels[SAT_MAX_STATES]     = {0};

  uint32_t    MNumBits                    = 8;//8;
  bool        MValueIsBits                = false;//true;
  bool        MAllowZeroBits              = true;

  //double      MTextSize                   = 9.0;
  //SAT_Color   MTextColor                  = SAT_Color(0.0);
  SAT_Color   MActiveTextColor            = SAT_Color(0.0);

  bool        MCellFillBackground         = true;
  SAT_Color   MCellBackgroundColor        = SAT_Color(0.3);
  SAT_Color   MCellActiveBackgroundColor  = SAT_Color(0.5);

  bool        MCellDrawBorder             = true;
  SAT_Color   MCellBorderColor            = SAT_Color(0.3);
  bool        MDrawRoundedBottom          = true;
  float       MRounded                    = 5;
  

//------------------------------
public:
//------------------------------

  SAT_ButtonRowWidget(SAT_Rect ARect, int32_t ANum, const char** ATxt=nullptr, int32_t AMode=SAT_BUTTON_ROW_SINGLE, bool AVertical=false)
  : SAT_GridWidget(ARect, AVertical?1:ANum, AVertical?ANum:1 ) {
    setName("SAT_ButtonRowWidget");
   
    MVertical = AVertical;

//    setNumParameters(1);

    //MName = "SAT_ButtonRowWidget";
    //setHint("buttonrow");
    setCursor(SAT_CURSOR_FINGER);
    setRoundedCorners(true);
    
    MMode                 = AMode;
    MSelected             = 0;
    MDrawSelectedCells    = false;
    MDrawHorizontalLines  = false;
    MDrawVerticalLines    = false;

    if (ATxt) {
      for (int32_t i=0; i<ANum; i++) {
        MStates[i] = false;
        MLabels[i] = (char*)ATxt[i];
      }
    }

    setFillBackground(false);
    setBackgroundColor(0.6);
    setDrawBorder(false);
  }

  //----------

  virtual ~SAT_ButtonRowWidget() {
  }

//------------------------------
public:
//------------------------------

  //virtual void setTextColor(SAT_Color AColor)                 { MTextColor = AColor; }
  virtual void setActiveTextColor(SAT_Color AColor)           { MActiveTextColor = AColor; }
  
  virtual void setCellFillBackground(bool AFill)              { MCellFillBackground = AFill; }
  virtual void setCellBackgroundColor(SAT_Color AColor)       { MCellBackgroundColor = AColor; }
  virtual void setCellActiveBackgroundColor(SAT_Color AColor) { MCellActiveBackgroundColor = AColor; }

  virtual void setCellDrawBorder(bool ADraw)                  { MCellDrawBorder = ADraw; }
  virtual void setCellBorderColor(SAT_Color AColor)           { MCellBorderColor = AColor; }

//------------------------------
public:
//------------------------------

  int32_t getSelected(void) {
    return MSelected;
  }

  void setValueIsBits(bool AIsBits=true, uint32_t ANumBits=8) {
    MValueIsBits = AIsBits;
    MNumBits = ANumBits;
  }

  void setDrawRoundedBottom(bool AState) {
    MDrawRoundedBottom = AState;
  }

  //void setTextSize(double ASize) {
  //  MTextSize = ASize;
  //}

  void setAllowZeroBits(bool AAllow=true) {
    MAllowZeroBits = AAllow;
  }

//------------------------------
public:
//------------------------------

  void setNumButtons(uint32_t ANum) {
    setNumColumns(ANum);
  }

  //----------

  bool getButtonState(int32_t i) {
    return MStates[i];
  }

  void setButtonState(int32_t i, bool s) {
    MStates[i] = s;
  }


  //----------

  uint32_t getButtonBits() {
    uint32_t bits = 0;
    for (uint32_t i=0; i<MNumBits; i++) {
      if (MStates[i] == true) bits |= (1 << i);
    }
    return bits;
  }

  //----------

  uint32_t getNumActiveBits() {
    uint32_t bits = 0;
    for (uint32_t i=0; i<MNumBits; i++) {
      if (MStates[i] == true) bits += 1;
    }
    return bits;
  }

  //----------

  void setButtonBits(uint32_t ABits) {
    for (uint32_t i=0; i<MNumBits; i++) {
      bool b = ( ABits & (1 << i) );
      MStates[i] = b;
    }
  }

  //----------

  const char* getButtonName(int32_t i) {
    return MLabels[i];
  }

  void setButtonName(int32_t i, const char* AName) {
    MLabels[i] = (char*)AName;
  }

  //----------

  double getValue(uint32_t AIndex=0) override {
    if (MValueIsBits) {
      sat_param_t value = SAT_GridWidget::getValue();
      uint32_t bits = getButtonBits();
      SAT_Print("%.3f -> %i\n",value,bits);
      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (param) return param->normalize(bits);
      return bits;
    }
    else {
      sat_param_t value = SAT_GridWidget::getValue();
      //SAT_Parameter* param = (SAT_Parameter*)getParameter();
      //if (param) value = param->denormalize(value);
      SAT_Print("%.3f\n",value);
      return value;
    }
  }

  //----------


  void setValue(double AValue, uint32_t AIndex=0) override {
    SAT_Print("value %.3f\n",AValue);
    if (MValueIsBits) {
      SAT_Parameter* param = (SAT_Parameter*)getParameter();
      if (param) AValue = param->denormalize(AValue);
      int i = (int)AValue;
      SAT_Print("%.3f -> %i\n",AValue,i);
      setButtonBits(i);
      SAT_GridWidget::setValue(AValue);
    }
    else {
      SAT_Print("AValue %.3f\n",AValue);
      SAT_GridWidget::setValue(AValue);
      //SAT_Parameter* param = (SAT_Parameter*)getParameter();
      //if (param) AValue = param->normalize(AValue);
//      selectValue(AValue);
    }
  }

  //----------

  //void setValue(uint32_t AIndex,double AValue) override {
  //  setValue(AValue);
  //}

  //----------

  void selectButton(int32_t index) {
    MSelected = index;
    if (MMode == SAT_BUTTON_ROW_SINGLE) {
      int32_t num;
      if (MVertical) num = MNumRows;
      else num = MNumColumns;
      for (int32_t i=0; i<num; i++) {
        if (i==MSelected) {
          MStates[i] = true;
          //MValues[0] = i;
        }
        else {
          MStates[i] = false;
        }
      }
      //MValues[0] = MSelected;
      //setValue(MSelected);
      SAT_GridWidget::setValue(MSelected);
      //float v = (float)MSelected / ((float)MNumColumns - 1.0f);
      //SAT_Widget::setValue(v);
      do_widget_select(this,MSelected);
      
    }
    else { // MULTI
      MStates[MSelected] = MStates[MSelected] ? false : true;
      if ( !MAllowZeroBits && (getNumActiveBits() == 0) ) {
        MStates[MSelected] = true;
      }
      //calcBitValue();
      //MValues[0] = getButtonBits();
      uint32_t bits = getButtonBits();
      //setValue( bits );
      SAT_GridWidget::setValue( bits );
      do_widget_select(this,bits);
    }
  }

  //----------

  // value = 0..1

  void selectValue(float AValue) {
    //SAT_Print("value %.3f\n",AValue);
    // float num = AValue * MNumColumns;
    // num = SAT_Min(num,float(MNumColumns-1));
    // selectButton( (int)num );
    selectButton( (int)AValue );
  }

//------------------------------
public:
//------------------------------

  void on_clickCell(int32_t AX, int32_t AY, int32_t AB) override {
    if (AB == SAT_BUTTON_LEFT) {
      
      //SAT_Print("MVertical %i AX %i AY %i\n",MVertical,AX,AY);
      
      if (MVertical) selectButton(AY);
      else selectButton(AX);
      //setValue();
      do_widget_update(this,0,0);
      do_widget_redraw(this,0,0);
    }
  }

  //----------

  void on_paintCell(SAT_PaintContext* AContext, SAT_Rect ARect, int32_t AX, int32_t AY) override {
    char buf[256];
    SAT_Painter* painter= AContext->painter;
    double S = getWindowScale();
    
    int32_t AA;
    if (MVertical) AA = AY;
    else AA = AX;
    
    SAT_Color c1,c2;
    if (MStates[AA]) {
      c1 = MCellActiveBackgroundColor;
      c2 = MCellBackgroundColor;
    }
    else {
      c1 = MCellBackgroundColor;
      c2 = MCellActiveBackgroundColor;
    }

    // background
    
    if (MRoundedCorners) {
      double ul = 0;
      double ur = 0;
      double ll = 0;
      double lr = 0;
      if (MVertical) {
        if (AY == 0) {
          ul = MTLCorner * S;
          ur = MTRCorner * S;
        }
        if (AY == (MNumRows-1))  {
          ll = MBLCorner * S;
          lr = MBRCorner * S;
        }
      }
      else {
        if (AX == 0) {
          ul = MTLCorner * S;
          ll = MBLCorner * S;
        }
        if (AX == (MNumColumns-1))  {
          ur = MTRCorner * S;
          lr = MBRCorner * S;
        }
      }

      if (MCellFillBackground) {
        if (MFillGradient) {
          SAT_Color g1 = MGradientColor1;
          SAT_Color g2 = MGradientColor2;
          painter->setFillLinearGradient(ARect.x,ARect.y,ARect.x,ARect.y2(),g1,g2);
        }
        else {
          painter->setFillColor(c1);
        }
        painter->fillRoundedRect(ARect.x,ARect.y,ARect.w,ARect.h,ul,ur,lr,ll);
      }
      if (MCellDrawBorder) {
        painter->setDrawColor( MCellBorderColor );
        painter->setLineWidth( 1.0 * S );
        painter->drawRoundedRect(ARect.x,ARect.y,ARect.w,ARect.h,ul,ur,lr,ll);
      }
      
    }
    else {
      if (MCellFillBackground) {
        if (MFillGradient) {
          SAT_Color g1 = MGradientColor1;
          SAT_Color g2 = MGradientColor2;
          painter->setFillLinearGradient(ARect.x,ARect.y,ARect.x,ARect.y2(),g1,g2);
        }
        else {
          painter->setFillColor(c1);
        }
        painter->fillRect(ARect.x,ARect.y,ARect.w,ARect.h);
      }
      if (MCellDrawBorder) {
        painter->setDrawColor( MCellBorderColor );
        painter->setLineWidth( 1.0 * S );
        painter->drawRect(ARect.x,ARect.y,ARect.w,ARect.h);
      }
    }

    // text

    const char* txt = MLabels[AA];
    SAT_Color color = MTextColor;
    if (MStates[AA]) color = MActiveTextColor;
    else color = MTextColor;
    if (txt) {
      painter->setTextColor(color);
      painter->setTextSize(MTextSize * S);
      painter->drawTextBox(ARect,txt,SAT_TEXT_ALIGN_CENTER);
    }
    else {
      sprintf(buf,"%i",AA);
      painter->setTextColor(color);
      painter->setTextSize(MTextSize * S);
      painter->drawTextBox(ARect,buf,SAT_TEXT_ALIGN_CENTER);
    }

  }

};

#undef SAT_MAX_STATES

//----------------------------------------------------------------------
#endif