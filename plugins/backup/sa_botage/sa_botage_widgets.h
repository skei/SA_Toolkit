#ifndef sa_botage_gui_included
#define sa_botage_gui_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
// header
//
//----------------------------------------------------------------------

class sa_botage_header_widget
: public SAT_PanelWidget {

public:

  sa_botage_header_widget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setName("sa_botage_header_widget");
    setFillBackground(true);
    setBackgroundColor(SAT_Color(0.1));
    setDrawBorder(false);
    SAT_LogoWidget* logo = new SAT_LogoWidget(SAT_Rect(10,10,40,40));
    appendChildWidget(logo);
    logo->setLogoColor(SAT_White);
  }
  
  void on_widget_paint(SAT_PaintContext* AContext) override {
    SAT_PanelWidget::on_widget_paint(AContext);
    SAT_Painter* painter= AContext->painter;
    SAT_Rect mrect = getRect();
    double S = getWindowScale();
    int default_font = painter->getDefaultFont();
    int header_font = painter->getHeaderFont();
    painter->selectFont(header_font);
    painter->setTextColor(0.5);
    painter->setTextSize( 32 * S );
    SAT_Rect nofs = SAT_Rect(60,8,0,0);
    nofs.scale(S);
    SAT_Rect nrect = mrect;
    nrect.shrink(nofs);
    painter->drawTextBox(nrect,"botage",SAT_TEXT_ALIGN_LEFT);
    painter->selectFont(default_font);
  }

};

//----------------------------------------------------------------------
//
// footer
//
//----------------------------------------------------------------------

class sa_botage_footer_widget
: public SAT_PanelWidget {

public:

  sa_botage_footer_widget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setName("sa_botage_footer_widget");
    setFillBackground(true);
    setBackgroundColor(SAT_Color(0.1));
    setDrawBorder(false);
  }
  
};

//----------------------------------------------------------------------
//
// main_panel
//
//----------------------------------------------------------------------

class sa_botage_main_panel_widget
: public SAT_PanelWidget {

public:

  sa_botage_main_panel_widget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setName("sa_botage_main_panel_widget");
    setFillBackground(true);
    setBackgroundColor(SAT_Color(0.15));
    setDrawBorder(false);
  }
  
};

//----------------------------------------------------------------------
//
// group
//
//----------------------------------------------------------------------

class sa_botage_group_widget
: public SAT_GroupBoxWidget {

public:

  sa_botage_group_widget(SAT_Rect ARect, const char* AText)
  : SAT_GroupBoxWidget(ARect,15,false) {
    setName("sa_botage_group_widget");
    SAT_ButtonWidget* titlebar  = getTitleBar();
    SAT_SymbolWidget* symbol    = getSymbol();
    SAT_PanelWidget*  container = getContainer();
    setClosable(false);
    symbol->setVisible(false);
    titlebar->setFillBackground(true);
    titlebar->setBackgroundColors(0.1,0.1);
    titlebar->setDrawBorder(false);
    titlebar->setCursor(SAT_CURSOR_DEFAULT);
    titlebar->setTexts(AText,AText);
    titlebar->setTextSize(8);
    titlebar->setTextColors(0.5,0.5);
    titlebar->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
    container->setFillBackground(true);
    container->setBackgroundColor(0.18);
    container->setDrawBorder(false);
    container->setBorderColor(0.5);
  }
  
};
  
//----------------------------------------------------------------------
//
// tabs
//
//----------------------------------------------------------------------

class sa_botage_tabs_widget
: public SAT_TabsWidget {

public:

  sa_botage_tabs_widget(SAT_Rect ARect, uint32_t ANum, uint32_t AHeaderHeight)
  : SAT_TabsWidget(ARect,ANum,AHeaderHeight) {
    
    //MHeader->setDrawRoundedBottom(false);
    SAT_ButtonRowWidget* header = getHeader();
    SAT_PagesWidget* pages = getPages();
    
    header->setSize(10,10);
    
    //header->setFillBackground(false);
    header->setDrawBorder(false);
    
    header->setRoundedCorners(false);
    header->setBackgroundColor(0.2);
    //header->setCornerSizes(6,6,0,0);
    
    header->setTextSize(8);
    header->setTextColor(0.6);
    header->setActiveTextColor(0.6);
  
    header->setCellFillBackground(true);
    header->setCellBackgroundColor(0.14);
    header->setCellActiveBackgroundColor(0.1);

    header->setCellDrawBorder(true);
    header->setCellBorderColor(0.1);
    
    pages->setFillBackground(false);
    pages->setDrawBorder(false);
    
  }
  
};
  
//----------------------------------------------------------------------
//
// text
//
//----------------------------------------------------------------------

// small

class sa_botage_text1_widget
: public SAT_TextWidget {

public:

  sa_botage_text1_widget(SAT_Rect ARect, const char* AText)
  : SAT_TextWidget(ARect,AText) {
    setFillBackground(false);
    //setBackgroundColor(0.18);
    setDrawBorder(true);
    setBorderColor(0.3);
    setBorderEdges(SAT_EDGE_BOTTOM);
    setTextColor(0.5);
    setTextSize(9);
    setTextAlignment(SAT_TEXT_ALIGN_LEFT);
  }
  
};

// medium

class sa_botage_text2_widget
: public SAT_TextWidget {

public:

  sa_botage_text2_widget(SAT_Rect ARect, const char* AText)
  : SAT_TextWidget(ARect,AText) {
    setFillBackground(false);
    //setBackgroundColor(0.18);
    setDrawBorder(true);
    setBorderColor(0.25);
    setBorderEdges(SAT_EDGE_BOTTOM);
    setTextColor(0.5);
    setTextSize(9);
    setTextAlignment(SAT_TEXT_ALIGN_LEFT);
  }
  
};

// large

class sa_botage_text3_widget
: public SAT_TextWidget {

public:

  sa_botage_text3_widget(SAT_Rect ARect, const char* AText)
  : SAT_TextWidget(ARect,AText) {
    setFillBackground(false);
    //setBackgroundColor(0.18);
    setDrawBorder(true);
    setBorderColor(0.3);
    setBorderEdges(SAT_EDGE_BOTTOM);
    setTextColor(0.5);
    setTextSize(10);
    setTextAlignment(SAT_TEXT_ALIGN_CENTER);
  }
  
};

//----------------------------------------------------------------------
//
// circular waveform
//
//----------------------------------------------------------------------

class sa_botage_waveform_widget
: public SAT_CircularWaveformWidget {

private:
  
  float test_buffer[4096] = {0};
  
public:

  sa_botage_waveform_widget(SAT_Rect ARect, uint32_t ANumLines=360)
  : SAT_CircularWaveformWidget(ARect,ANumLines) {
    
    for (uint32_t i=0; i<4096; i++) test_buffer[i] = SAT_RandomSigned();
  
    setFillBackground(false);
    //setBackgroundColor(0.18);
    setDrawBorder(false);
    //setBorderColor(0.2);
    setAudioBuffer(4096,test_buffer,false);
    setRadius(1.0, 0.5);
    setBipolar(true);
    setWaveformColor(SAT_Blue1);
    setWaveformWidth(1.0);
    setWaveformBackgroundColor(SAT_DarkBlue1);
    setZeroLineColor(0.2);
    setZeroLineWidth(0.5);
    setGrid(4,2);
    setGridColor(0.3,0.25);
    setGridWidth(0.002,0.001);
    //setAreaActive(uint32_t AIndex, bool AActive=true);
    //setAreaPos(uint32_t AIndex, double APos, double ASize);
    //setAreaColor(uint32_t AIndex, SAT_Color AColor);
    //setMarkerActive(uint32_t AIndex, bool AActive=true);
    //setMarkerPos(uint32_t AIndex, double APos, double ASize=0.0025);
    //setMarkerColor(uint32_t AIndex, SAT_Color AColor);
    
  }
  
};

//----------------------------------------------------------------------
//
// knobs
//
//----------------------------------------------------------------------

// knob1 - small (fx arg)

class sa_botage_knob1_widget
: public SAT_KnobWidget {

public:

  sa_botage_knob1_widget(SAT_Rect ARect, const char* AText="%", double AValue=0.0)
  : SAT_KnobWidget(ARect,AText,AValue) {
    setFillBackground(true);
    setBackgroundColor(0.18);
//    setDrawBorder(false);
//    setBorderColor(0.2);
    setDrawValue(false);
    setDrawText(false);
    setArcThickness(4);
    setArcValueColor(0.5);
    setBipolar(true);
    setSnap(true);
    setSnapPos(0.5);
  }
  
};

//----------
  
// knob2 - medium (prob)

class sa_botage_knob2_widget
: public SAT_KnobWidget {

public:

  sa_botage_knob2_widget(SAT_Rect ARect, const char* AText="%", double AValue=0.0)
  : SAT_KnobWidget(ARect,AText,AValue) {
    setFillBackground(true);
    setBackgroundColor(0.18);
    //setDrawBorder(false);
    //setBorderColor(0.2);
    setDrawText(true);
    setTextColor(0.4);
    setTextSize(10);
    setArcValueColor(0.5);
    setDrawValue(true);
    setValueColor(0.5);
    setValueSize(7);
    setArcThickness(6);
    //setSnap(true);
    //setSnapPos(0.5);
  }
  
};

//----------
  
// knob3 - big (activation prob)

class sa_botage_knob3_widget
: public SAT_KnobWidget {

public:

  sa_botage_knob3_widget(SAT_Rect ARect, const char* AText="%", double AValue=0.0)
  : SAT_KnobWidget(ARect,AText,AValue) {
    setFillBackground(true);
    setBackgroundColor(0.18);
    //setDrawBorder(false);
    //setBorderColor(0.2);
    setDrawText(true);
    setTextColor(0.4);
    setTextSize(10);
    setDrawValue(true);
    setValueColor(0.5);
    setValueSize(12);
    setArcThickness(10);
    setArcValueColor(0.5);
    //setSnap(true);
    //setSnapPos(0.5);
  }
  
};

//----------------------------------------------------------------------
//
// sliders
//
//----------------------------------------------------------------------

class sa_botage_slider_widget
: public SAT_SliderWidget {

public:

  sa_botage_slider_widget(SAT_Rect ARect, const char* AText, double AValue)
  : SAT_SliderWidget(ARect,AText,AValue) {
    setFillBackground(true);
    setBackgroundColor(0.25);
    //setDrawBorder(false);
    //setBorderColor(0.2);
    setTextColor(0);
    setTextSize(7);
    setValueColor(0);
    setValueSize(8);
    setSliderBarColor(0.4);
    setSliderEdgeColor(0.5);
  }
  
};

//----------

class sa_botage_dualslider_widget
: public SAT_DualSliderWidget {

public:

  sa_botage_dualslider_widget(SAT_Rect ARect, const char* AText, double AValue1, double AValue2)
  : SAT_DualSliderWidget(ARect,AText,AValue1,AValue2) {
    setFillBackground(true);
    setBackgroundColor(0.25);
    setDrawBorder(false);
    //setBorderColor(0.2);
    setTextColor(0);
    setTextSize(7);
    setValueColors(0,0);
    setValueSizes(8,8);
    setValueOffsets(SAT_Rect(2,0,0,0),SAT_Rect(0,0,2,0));
    setSliderBarColor(0.3);
    setSliderEdgeColor(0.5);
  }
  
};

//----------------------------------------------------------------------
//
// buttonrow
//
//----------------------------------------------------------------------

class sa_botage_buttonrow_widget
: public SAT_ButtonRowWidget {

public:

  sa_botage_buttonrow_widget(SAT_Rect ARect, int32_t ANum, const char** ATxt=nullptr, int32_t AMode=SAT_BUTTON_ROW_SINGLE, bool AVertical=false)
  : SAT_ButtonRowWidget(ARect,ANum,ATxt,AMode,AVertical) {
    //setFillBackground(true);
    //setBackgroundColor(0.18);
    //setDrawBorder(false);
    //setBorderColor(0.2);
    setTextColor(0.5);
    setActiveTextColor(0.0);
    setCellFillBackground(true);
    setCellBackgroundColor(0.2);
    setCellActiveBackgroundColor(0.4);
    setCellDrawBorder(false);
    setCellBorderColor(0.3);
    
  }
  
};

//----------------------------------------------------------------------
//
// selector
//
//----------------------------------------------------------------------

class sa_botage_selector_widget
: public SAT_SelectorWidget {

public:

  sa_botage_selector_widget(SAT_Rect ARect, const char* AText, SAT_MenuWidget* AMenu, SAT_MenuListener* AListener=nullptr)
  : SAT_SelectorWidget(ARect,AText,AMenu,AListener) {
    setFillBackground(false);
    //setBackgroundColor(0.18);
    setDrawBorder(true);
    setBorderColor(0.3);
    setBorderEdges(SAT_EDGE_BOTTOM);
    setTextColor(0.5);
    setTextSize(9);
    setTextAlignment(SAT_TEXT_ALIGN_LEFT);
  }
  
};

//----------------------------------------------------------------------
#endif