#ifndef sa_demo_page_widgets_included
#define sa_demo_page_widgets_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widgets.h"

class sa_demo_page_widgets
: public SAT_PanelWidget {
  
//------------------------------
private:
//------------------------------

  const char*     buttonrow_text[5] = { "One", "Two", "III", "4", "5" };
  SAT_MenuWidget* MMenu1            = nullptr;
  
  float buffer[1024 * 2] = {0};
  
//------------------------------
public:
//------------------------------

  sa_demo_page_widgets(SAT_Rect ARect, SAT_Plugin* APlugin)
  : SAT_PanelWidget(ARect) {

    setName("sa_demo_page_widgets");
    setStretching(SAT_WIDGET_STRETCH_ALL);
    setInnerBorder(SAT_Rect(10,10,10,10));
    //setSpacing(SAT_Point(5,5));
    
    //----------
    
    for (uint32_t i=0; i<1024; i++) buffer[i] = SAT_RandomRange(-0.99,0.99);
    
    //----------

    MMenu1 = new SAT_MenuWidget(SAT_Rect(100,100));
    MMenu1->setRightClickClose(true);
    MMenu1->setBorderWidth(2);
    MMenu1->setInnerBorder(SAT_Rect(5,5,5,5));
    MMenu1->appendChildWidget(new SAT_MenuItemWidget(15,"Item1"));
    MMenu1->appendChildWidget(new SAT_MenuItemWidget(15,"Item2"));
    MMenu1->appendChildWidget(new SAT_MenuItemWidget(15,"Item3"));

    //----------
    
    SAT_PanelWidget* left_panel = new SAT_PanelWidget(200);
    appendChildWidget(left_panel);
    left_panel->setLayout(SAT_WIDGET_ALIGN_LEFT_TOP,SAT_WIDGET_STRETCH_VERTICAL);
    left_panel->setWidthLimits(100,400);
    left_panel->setInnerBorder(SAT_Rect(10,10,10,10));
    left_panel->setSpacing(SAT_Point(5,5));
    {
      
      SAT_PanelWidget* panel1 = new SAT_PanelWidget(15);
      left_panel->appendChildWidget(panel1);
      panel1->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
      
      SAT_TextWidget* text1 = new SAT_TextWidget(15,"Text");
      left_panel->appendChildWidget(text1);
      text1->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
      
      SAT_ButtonWidget* button1 = new SAT_ButtonWidget(15);
      left_panel->appendChildWidget(button1);
      button1->setTexts("Button (on)","Button (off)");
      button1->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
      
      SAT_ButtonWidget* button2 = new SAT_ButtonWidget(15);
      left_panel->appendChildWidget(button2);
      button2->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
      button2->setTexts("Button (on)","Button (off)");
      button2->setIsToggle(true);
      
      SAT_ButtonRowWidget* buttonrow1 = new SAT_ButtonRowWidget(15,5,buttonrow_text,SAT_BUTTON_ROW_SINGLE,false);
      left_panel->appendChildWidget(buttonrow1);
      buttonrow1->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
      
      SAT_ButtonRowWidget* buttonrow2 = new SAT_ButtonRowWidget(15,5,buttonrow_text,SAT_BUTTON_ROW_MULTI,false);
      left_panel->appendChildWidget(buttonrow2);
      buttonrow2->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
      
      SAT_ValueWidget* value1 = new SAT_ValueWidget(15,"Value",0.5);
      left_panel->appendChildWidget(value1);
      value1->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
      
      SAT_DragValueWidget* dragvalue1 = new SAT_DragValueWidget(15,"DragValue",0.5);
      left_panel->appendChildWidget(dragvalue1);
      dragvalue1->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
      
      SAT_SliderWidget* slider1 = new SAT_SliderWidget(15,"Slider",0.5);
      left_panel->appendChildWidget(slider1);
      slider1->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
      
      //SAT_DualSliderWidget* dualslider1 = new SAT_DualSliderWidget(15,"DualSlider",0.2,0.8);
      //left_panel->appendChildWidget(dualslider1);
      //dualslider1->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
      
      SAT_ScrollBarWidget* scrollbar1 = new SAT_ScrollBarWidget(15);
      left_panel->appendChildWidget(scrollbar1);
      scrollbar1->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
      
      SAT_RangeBarWidget* rangebar1 = new SAT_RangeBarWidget(15);
      left_panel->appendChildWidget(rangebar1);
      rangebar1->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
      
      SAT_MenuItemWidget* menuitem1 = new SAT_MenuItemWidget(15,"MenuItem");
      left_panel->appendChildWidget(menuitem1);
      menuitem1->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
      
      SAT_SelectorWidget* selector1 = new SAT_SelectorWidget(15,"Selector",MMenu1);
      left_panel->appendChildWidget(selector1);
      selector1->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
    }
    
    SAT_SizerWidget* sizer = new SAT_SizerWidget(SAT_Rect(3),SAT_DIRECTION_LEFT,left_panel);
    appendChildWidget(sizer);
    sizer->setFillBackground(true);
    sizer->setBackgroundColor(0.3);
    sizer->setDrawBorder(false);

    SAT_PanelWidget* right_panel = new SAT_PanelWidget(0);
    appendChildWidget(right_panel);
    right_panel->setLayout(SAT_WIDGET_ALIGN_LEFT_TOP,SAT_WIDGET_STRETCH_ALL);
    right_panel->setInnerBorder(SAT_Rect(5,5,5,5));
    right_panel->setSpacing(SAT_Point(5,5));
    {
      
      SAT_WaveformWidget* waveform1 = new SAT_WaveformWidget(100);
      right_panel->appendChildWidget(waveform1);
      waveform1->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
      waveform1->setDrawWaveform(true);
      waveform1->setWaveformWidth(1.0);
      waveform1->setWaveformColor(SAT_Black);
      waveform1->setNumGrid(4);
      waveform1->setNumSubGrid(2);
      waveform1->setBuffer(buffer,1024);
      //waveform1->setBuffers(nullptr,nullptr,1024);
      waveform1->setMarkerActive(0,false);
      waveform1->setMarkerPos(0,0);
      waveform1->setMarkerColor(0,SAT_Green);
      waveform1->setMarkerWidth(0,2.0);
      waveform1->setAreaActive(0,false);
      waveform1->setAreaStart(0,0);
      waveform1->setAreaLength(0,512);
      waveform1->setAreaColor(0,SAT_Red);
      
      SAT_CircularWaveformWidget* cwaveform1 = new SAT_CircularWaveformWidget(100,512);
      right_panel->appendChildWidget(cwaveform1);
      cwaveform1->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_NONE);
      
      cwaveform1->setAudioBuffer(1024,buffer,false);
      cwaveform1->setRadius(0.9, 0.5);
      cwaveform1->setBipolar(true);
      cwaveform1->setWaveformWidth(1.0);
      cwaveform1->setWaveformColor(SAT_Black);
      cwaveform1->setWaveformBackgroundColor(SAT_LightGrey);
//      cwaveform1->setZeroLineColor(SAT_Color AColor);
//      cwaveform1->setZeroLineWidth(double AWidth);
      cwaveform1->setGrid(4,2);
//      cwaveform1->setGrid(uint32_t AGrid);
//      cwaveform1->setSubGrid(uint32_t ASubGrid);
//      cwaveform1->setGridColor(SAT_Color AGrid, SAT_Color ASubGrid);
//      cwaveform1->setGridWidth(double AGrid=0.002, double ASubGrid=0.001);
//      cwaveform1->setAreaActive(uint32_t AIndex, bool AActive=true);
//      cwaveform1->setAreaPos(uint32_t AIndex, double APos, double ASize);
//      cwaveform1->setAreaColor(uint32_t AIndex, SAT_Color AColor);
//      cwaveform1->setMarkerActive(uint32_t AIndex, bool AActive=true);
//      cwaveform1->setMarkerPos(uint32_t AIndex, double APos, double ASize=0.0025);
//      cwaveform1->setMarkerColor(uint32_t AIndex, SAT_Color AColor);

      SAT_Knob2Widget* knob2 = new SAT_Knob2Widget(SAT_Rect(64,80),"Knob2",0.0);
      right_panel->appendChildWidget(knob2);
      knob2->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_NONE);

    }
    
  //----------
    
//    appendChildWidget(selector_menu);
    
  }
  
  //----------

  virtual ~sa_demo_page_widgets() {
  }
  
//------------------------------
public:
//------------------------------

  SAT_MenuWidget* getMenu1() { return MMenu1; }

  

};

//----------------------------------------------------------------------
#endif