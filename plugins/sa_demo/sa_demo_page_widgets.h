#ifndef sa_demo_page_widgets_included
#define sa_demo_page_widgets_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"

#include "../../data/img/SA.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_demo_page_widgets
: public SAT_VisualWidget {

//------------------------------
public:
//------------------------------

  SAT_MenuWidget* MMenu1 = nullptr;

  const char* MSelectorTexts[5] = {
    "Off", "Lowpass", "Highpass", "Bandpass", "Notch"
  };

  const char* MButtonTexts[10*6] = {
    "0",   "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59"
  };

//------------------------------
public:
//------------------------------

  sa_demo_page_widgets()
  : SAT_VisualWidget(0) {
    setName("sa_demo_page_widgets");
    setHint("sa_demo_page_widgets");
    //Options.autoClip = true;
    //Options.realignInvisible = true;
    Layout.flags = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
    //setFillBackground(true);
    //setBackgroundColor(SAT_Yellow);

    // menu

    MMenu1 = new SAT_MenuWidget(SAT_Rect(100,20*5));
    for (uint32_t i=0; i<5; i++) {
      MMenu1->appendItem( MSelectorTexts[i] );
    }

    //

    // right_panel

    //SAT_VisualWidget* right_panel = new SAT_VisualWidget(100);
    SAT_ScrollBoxWidget* right_panel = new SAT_ScrollBoxWidget(150,true,false);
    appendChild(right_panel);
    right_panel->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_RIGHT;
    right_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_VERT;
    right_panel->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_RIGHT;
    right_panel->getContentWidget()->Layout.innerBorder = { 5,5,5,5 };
    right_panel->getContentWidget()->Layout.spacing = { 5,5 };
    right_panel->setDrawBorder(false);
    right_panel->setFillBackground(false);
    right_panel->getVerticalScrollBar()->setRoundedThumb(5.0);
    right_panel->getVerticalScrollBar()->setDrawRoundedCorners(true);
    right_panel->getVerticalScrollBar()->setRoundedCorners(SAT_CORNER_ALL);
    right_panel->getVerticalScrollBar()->setRoundedCornerSize(5.0);

    // right_sizer

    SAT_SizerWidget* right_sizer = new SAT_SizerWidget(5,SAT_DIRECTION_RIGHT,right_panel);
    appendChild(right_sizer);
    right_sizer->setBackgroundColor(0.4);

    // bottom_panel

    //SAT_VisualWidget* bottom_panel = new SAT_VisualWidget(100);
    SAT_ScrollBoxWidget* bottom_panel = new SAT_ScrollBoxWidget(150,true,true);
    appendChild(bottom_panel);
    bottom_panel->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_LEFT;
    bottom_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    bottom_panel->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_BOTTOM;
    bottom_panel->getContentWidget()->Layout.innerBorder = { 5,5,5,5 };
    bottom_panel->getContentWidget()->Layout.spacing = { 5,5 };
    bottom_panel->setDrawBorder(false);
    bottom_panel->setFillBackground(false);
    bottom_panel->getHorizontalScrollBar()->setRoundedThumb(5.0);
    bottom_panel->getHorizontalScrollBar()->setDrawRoundedCorners(true);
    bottom_panel->getHorizontalScrollBar()->setRoundedCorners(SAT_CORNER_ALL);
    bottom_panel->getHorizontalScrollBar()->setRoundedCornerSize(5.0);
    bottom_panel->getVerticalScrollBar()->setRoundedThumb(5.0);
    bottom_panel->getVerticalScrollBar()->setDrawRoundedCorners(true);
    bottom_panel->getVerticalScrollBar()->setRoundedCorners(SAT_CORNER_ALL);
    bottom_panel->getVerticalScrollBar()->setRoundedCornerSize(5.0);

    // bottom_sizer

    SAT_SizerWidget* bottom_sizer = new SAT_SizerWidget(5,SAT_DIRECTION_DOWN,bottom_panel);
    appendChild(bottom_sizer);
    bottom_sizer->setBackgroundColor(0.4);

    // left_panel

    //SAT_VisualWidget* left_panel = new SAT_VisualWidget(150);
    SAT_ScrollBoxWidget* left_panel = new SAT_ScrollBoxWidget(200,true,false);
    appendChild(left_panel);
    left_panel->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    left_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_VERT;
    left_panel->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
    left_panel->getContentWidget()->Layout.innerBorder = { 5,5,5,5 };
    left_panel->getContentWidget()->Layout.spacing = { 5,5 };
    left_panel->setDrawBorder(false);
    left_panel->setFillBackground(false);

    left_panel->getVerticalScrollBar()->setRoundedThumb(5.0);
    left_panel->getVerticalScrollBar()->setDrawRoundedCorners(true);
    left_panel->getVerticalScrollBar()->setRoundedCorners(SAT_CORNER_ALL);
    left_panel->getVerticalScrollBar()->setRoundedCornerSize(5.0);

    // left_sizer

    SAT_SizerWidget* left_sizer = new SAT_SizerWidget(5,SAT_DIRECTION_LEFT,left_panel);
    appendChild(left_sizer);
    left_sizer->setBackgroundColor(0.4);

    // center_panel

    SAT_VisualWidget* center_panel = new SAT_VisualWidget(0);
    appendChild(center_panel);
    center_panel->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    center_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
    //center_panel->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_BOTTOM;
    center_panel->setDrawBorder(false);
    center_panel->setFillBackground(false);

    //------------------------------
    // left_panel
    //------------------------------

    SAT_VisualWidget* visual1 = new SAT_VisualWidget(20);
    left_panel->appendChild(visual1);
    visual1->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    visual1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    visual1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

    SAT_TextWidget* text1 = new SAT_TextWidget(20,"Text");
    left_panel->appendChild(text1);
    text1->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    text1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    text1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

    SAT_ValueWidget* value1 = new SAT_ValueWidget(20,"Value",0.5);
    left_panel->appendChild(value1);
    value1->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    value1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    value1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

    SAT_ButtonWidget* button1 = new SAT_ButtonWidget(20,true);
    left_panel->appendChild(button1);
    button1->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    button1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    button1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

    SAT_ButtonWidget* button2 = new SAT_ButtonWidget(20,false);
    left_panel->appendChild(button2);
    button2->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    button2->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    button2->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

    SAT_DragValueWidget* dragvalue1 = new SAT_DragValueWidget(20,"DragValue",0.5);
    left_panel->appendChild(dragvalue1);
    dragvalue1->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    dragvalue1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    dragvalue1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

    SAT_SliderWidget* slider1 = new SAT_SliderWidget(20);
    left_panel->appendChild(slider1);
    slider1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    slider1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    slider1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    slider1->setText("Slider");
    //AEditor->connect(slider1,getParameter(5));

    SAT_DualSliderWidget* dualslider1 = new SAT_DualSliderWidget(20);
    left_panel->appendChild(dualslider1);
    dualslider1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    dualslider1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    dualslider1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    dualslider1->setValue(0.2,0);
    dualslider1->setValue(0.8,1);

    SAT_ScrollBarWidget* scrollbar1 = new SAT_ScrollBarWidget(20);
    left_panel->appendChild(scrollbar1);
    scrollbar1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    scrollbar1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    scrollbar1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

    SAT_TextEditWidget* textedit1 = new SAT_TextEditWidget(20,"TextEdit");
    left_panel->appendChild(textedit1);
    textedit1->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    textedit1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    textedit1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

    // SAT_Widget* endstack1 = new SAT_Widget(0);
    // left_panel->appendChild(endstack1);
    // endstack1->Layout.flags = SAT_WIDGET_LAYOUT_STACK_HORIZ;

    //------------------------------
    // right_panel
    //------------------------------

    // SAT_SelectorWidget* selector1 = new SAT_SelectorWidget(20,"Select..",menu1);
    SAT_SelectorWidget* selector1 = new SAT_SelectorWidget(20,"Select..",MMenu1);
    right_panel->appendChild(selector1);
    MMenu1->setListener(selector1);
    selector1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    selector1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    selector1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    selector1->setDrawParamText(false);
    selector1->select(0);

    SAT_GroupBoxWidget* groupbox1 = new SAT_GroupBoxWidget(80,20,true);
    right_panel->appendChild(groupbox1);
    groupbox1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    groupbox1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    groupbox1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    groupbox1->appendChild( new SAT_KnobWidget(SAT_Rect( 5,5,40,40)));
    groupbox1->appendChild( new SAT_KnobWidget(SAT_Rect(50,5,40,40)));
    groupbox1->appendChild( new SAT_KnobWidget(SAT_Rect(95,5,40,40)));

    SAT_TabsWidget* tabs1 = new SAT_TabsWidget(80,20);
    right_panel->appendChild(tabs1);
    tabs1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    tabs1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    tabs1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    tabs1->getHeader()->setFillCellsGradient(true);
    SAT_VisualWidget* p1 = new SAT_VisualWidget(15);
    SAT_VisualWidget* p2 = new SAT_VisualWidget(15);
    SAT_VisualWidget* p3 = new SAT_VisualWidget(15);
    //p1->setBackgroundColor(SAT_Yellow);
    p1->appendChild( new SAT_KnobWidget(SAT_Rect( 5,5,40,40)));
    p1->appendChild( new SAT_KnobWidget(SAT_Rect(50,5,40,40)));
    p1->appendChild( new SAT_KnobWidget(SAT_Rect(95,5,40,40)));
    //p2->setBackgroundColor(SAT_Orange);
    p2->appendChild( new SAT_SliderWidget(SAT_Rect(5, 5,150,20)));
    p2->appendChild( new SAT_SliderWidget(SAT_Rect(5,30,150,20)));
    //p3->setBackgroundColor(SAT_Green2);
    p3->appendChild( new SAT_ButtonWidget(SAT_Rect( 5, 5,40,20)));
    p3->appendChild( new SAT_ButtonWidget(SAT_Rect(50, 5,40,20)));
    p3->appendChild( new SAT_ButtonWidget(SAT_Rect( 5,30,40,20)));
    p3->appendChild( new SAT_ButtonWidget(SAT_Rect(50,30,40,20)));
    tabs1->appendPage( "Page1", p1);
    tabs1->appendPage( "Page2", p2);
    tabs1->appendPage( "Page3", p3);
    tabs1->selectPage(0);

    SAT_GridWidget* grid1 = new SAT_GridWidget(60,4,4);
    right_panel->appendChild(grid1);
    grid1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    grid1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    grid1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    grid1->setCanSelectCell(true);
    grid1->setCanSelectMultipleCells(true);
    grid1->setDrawSelectedCells(true);

    SAT_ButtonGridWidget* buttongrid1 = new SAT_ButtonGridWidget(60,4,4,MButtonTexts);
    right_panel->appendChild(buttongrid1);
    buttongrid1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    buttongrid1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    buttongrid1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    buttongrid1->setSingle(false,true);

    SAT_KeyboardWidget* keyboard1 = new SAT_KeyboardWidget(60);
    right_panel->appendChild(keyboard1);
    keyboard1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    keyboard1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    keyboard1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

    SAT_SliderBankWidget* sliderbank1 = new SAT_SliderBankWidget(60,16);
    right_panel->appendChild(sliderbank1);
    sliderbank1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    sliderbank1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    sliderbank1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

    SAT_ValueGraphWidget* valuegraph1 = new SAT_ValueGraphWidget(60,16);
    right_panel->appendChild(valuegraph1);
    valuegraph1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    valuegraph1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    valuegraph1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;

    SAT_VisualWidget* symbol_container = new SAT_VisualWidget(20);
    right_panel->appendChild(symbol_container);
    symbol_container->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    symbol_container->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    symbol_container->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    symbol_container->setDrawBorder(false);
    for (uint32_t i=0; i<17; i++) {
      SAT_SymbolWidget* symbol = new SAT_SymbolWidget(SAT_Rect(i*20,0,18,18),i);
      symbol->setColor(SAT_DarkerGrey);
      symbol->setColor(SAT_DarkerGrey);
      symbol->setPenWidth(2);
      symbol_container->appendChild(symbol);
    }

    for (uint32_t i=0; i<10; i++) {
      SAT_KnobWidget* knob = new SAT_KnobWidget(25);
      right_panel->appendChild(knob);
      knob->Layout.flags  = SAT_WIDGET_LAYOUT_STACK_HORIZ;
      knob->setDrawValueText(false);
      knob->setDrawText(false);
      if (i==9) {
        knob->Layout.flags |= SAT_WIDGET_LAYOUT_STACK_END;
      }
    }

    //------------------------------
    // bottom_panel
    //------------------------------

      SAT_KnobWidget* knob1 = new SAT_KnobWidget(100);
      bottom_panel->appendChild(knob1);
      //AEditor->connect(knob1,getParameter(4));
      knob1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      knob1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      knob1->setDragSnap(true);
      knob1->setDrawBipolar(true);
      knob1->setDrawKnobNeedle(true);
      knob1->setKnobNeedleStart(0.45);
      knob1->setKnobNeedleLength(0.85);
      knob1->setValueTextSize(15);

      SAT_KnobWidget* knob2 = new SAT_KnobWidget(100);
      bottom_panel->appendChild(knob2);
      //AEditor->connect(knob2,getParameter(5));
      knob2->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      knob2->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      knob2->setKnobArcThickness(20);
      knob2->setValueTextSize(20);

      SAT_CurveWidget* curve1 = new SAT_CurveWidget(30,false);
      bottom_panel->appendChild(curve1);
      curve1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      curve1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      curve1->setDrawParamText(false);
      //AEditor->connect(curve1,getParameter(1));

      SAT_CurveWidget* curve2 = new SAT_CurveWidget(30,true);
      bottom_panel->appendChild(curve2);
      curve2->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      curve2->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      curve2->setDrawParamText(false);
      //AEditor->connect(curve2,getParameter(2));

      SAT_SliderWidget* slider2 = new SAT_SliderWidget(SAT_Rect(25,100));
      bottom_panel->appendChild(slider2);
      slider2->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      slider2->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      slider2->setDrawDirection(SAT_DIRECTION_UP);
      slider2->setDrawText(false);
      slider2->setValueTextAlignment(SAT_TEXT_ALIGN_CENTER);
      slider2->setDrawParamText(false);
      //AEditor->connect(slider2,getParameter(6));

      SAT_ScrollBarWidget* scrollbar2 = new SAT_ScrollBarWidget(SAT_Rect(25,100));
      bottom_panel->appendChild(scrollbar2);
      scrollbar2->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      scrollbar2->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      scrollbar2->setDirection(SAT_DIRECTION_VERT);

      SAT_XYPadWidget* xypad1 = new SAT_XYPadWidget(100);
      bottom_panel->appendChild(xypad1);
      xypad1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      xypad1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      //AEditor->connect(xypad1,getParameter(6),0);
      //AEditor->connect(xypad1,getParameter(7),1);

      SAT_ImageWidget* image1 = new SAT_ImageWidget(100,(void*)SA_png,SA_png_size);
      bottom_panel->appendChild(image1);
      image1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      image1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      image1->setDrawBorder(false);

      SAT_LogoWidget* logo1 = new SAT_LogoWidget(SAT_Rect(5,0,100,100));
      bottom_panel->appendChild(logo1);
      logo1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      logo1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      logo1->setLogoColor(0.25);
      logo1->setLogoWidth(3);
      logo1->setFillBackground(true);
      logo1->setBackgroundColor(0.85);
      logo1->setDrawBorder(true);
      logo1->setBorderColor(0);
      logo1->setLogoOffset(SAT_Rect(12,12,12,12));

      logo1->setSizable(true);
      logo1->Options.wantHoverEvents = true;

      SAT_TextBoxWidget* textbox1 = new SAT_TextBoxWidget(SAT_Rect(150,100));
      bottom_panel->appendChild(textbox1);
      textbox1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      textbox1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
        textbox1->appendLine("Hello world!");
        textbox1->appendLine("another line");
        textbox1->appendLine("let's try a much, much longer line, and see if the scrollbars are updated");

      SAT_WaveformWidget* waveform1 = new SAT_WaveformWidget(SAT_Rect(150,100));
      bottom_panel->appendChild(waveform1);
      waveform1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      waveform1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      //waveform1->setAudioBuffer(size,buffer,stereo);

      SAT_CircularWaveformWidget* waveform2 = new SAT_CircularWaveformWidget(100,360);
      bottom_panel->appendChild(waveform2);
      waveform2->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      waveform2->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
      //waveform2->setAudioBuffer(size,buffer,stereo);

    //------------------------------
    // center_panel
    //------------------------------

      SAT_TabsWidget* center_tabs1 = new SAT_TabsWidget(0,15);//,3,0);
      center_panel->appendChild(center_tabs1);
      center_tabs1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      center_tabs1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
      center_tabs1->getPages()->setDrawBorder(false);
      center_tabs1->getHeader()->setFillCellsGradient(true);

        // tab 1 - modular

        SAT_VisualWidget* modular_tab1 = new SAT_VisualWidget(15);
        center_tabs1->appendPage( "Modular", modular_tab1);
        //modular_tab1->Layout.innerBorder = SAT_Rect(5,5,5,5);
        modular_tab1->Layout.spacing = SAT_Point(0,5);
        modular_tab1->setDrawBorder(false);

          SAT_TimelineWidget* timeline1 = new SAT_TimelineWidget(100);
          modular_tab1->appendChild(timeline1);
          timeline1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_LEFT;
          timeline1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
          timeline1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_BOTTOM;
          SAT_TimelineTrack* track1 = new SAT_TimelineTrack("Track1");
          timeline1->addTrack(track1);
          SAT_TimelineSegment* segment1 = new SAT_TimelineSegment("1",1,10);
          SAT_TimelineSegment* segment2 = new SAT_TimelineSegment("2",3,5);
          SAT_TimelineSegment* segment3 = new SAT_TimelineSegment("3",6,10);
          SAT_TimelineSegment* segment4 = new SAT_TimelineSegment("4",2,7);
          track1->addSegment(segment1);
          SAT_TimelineTrack* track2 = new SAT_TimelineTrack("T2");
          timeline1->addTrack(track2);
          track2->addSegment(segment2);
          track2->addSegment(segment3);
          SAT_TimelineTrack* track3 = new SAT_TimelineTrack("Automation");
          timeline1->addTrack(track3);
          track3->addSegment(segment4);

          SAT_GraphWidget* graph1 = new SAT_GraphWidget(0);
          modular_tab1->appendChild(graph1);
          graph1->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
          graph1->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
          //graph1->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
          graph1->setFillBackground(true);
          //graph->setAlignment(SAT_EDGE_BOTTOM);
          //graph->setStretching(SAT_EDGE_RIGHT | SAT_EDGE_BOTTOM);
          for (uint32_t i=0; i<6; i++) {
            SAT_GraphModule* module = new SAT_GraphModule();
            module->numInputs = 2;
            module->inputs[0] = SAT_PIN_SIGNAL;
            module->outputs[0] = SAT_PIN_SIGNAL;
            module->numOutputs = 2;
            graph1->addModule(module,i*15,i*15,"module");
          }

        // tab 2 - painting

        SAT_VisualWidget* painting_tab1 = new SAT_VisualWidget(15);
        center_tabs1->appendPage( "Painting", painting_tab1);
        //t2_tab1->Layout.innerBorder = SAT_Rect(5,5,5,5);
        painting_tab1->Layout.spacing = SAT_Point(0,5);
        painting_tab1->setDrawBorder(false);

          SAT_VisualWidget* ds1 = new SAT_VisualWidget(SAT_Rect(20,20,50,50));
          painting_tab1->appendChild(ds1);
          ds1->setDrawDropShadow(true);
          //ds1->setDropShadowCorner(5);
          ds1->setFillBackground(false);
          ds1->setDrawBorder(false);
          SAT_VisualWidget* ds2 = new SAT_VisualWidget(SAT_Rect(25,25,40,40));
          painting_tab1->appendChild(ds2);

          SAT_VisualWidget* ds3 = new SAT_VisualWidget(SAT_Rect(80,20,50,50));
          painting_tab1->appendChild(ds3);
          ds3->setDrawDropShadow(true);
          ds3->setDropShadowInner(true);
          ds3->setDropShadowCorner(5);
          ds3->setFillBackground(false);
          ds3->setDrawBorder(true);

          SAT_VisualWidget* ds5 = new SAT_VisualWidget(SAT_Rect(20,80,50,50));
          painting_tab1->appendChild(ds5);
          ds5->setFillBackground(true);
          ds5->setFillGradient(true);
          //ds5->setDrawBorder(true);

          SAT_VisualWidget* ds6 = new SAT_VisualWidget(SAT_Rect(80,80,50,50));
          painting_tab1->appendChild(ds6);
          ds6->setFillBackground(true);
          ds6->setBackgroundColor(SAT_Yellow);
          ds6->setDrawRoundedCorners(true);
          ds6->setRoundedCornerSize(10);
          ds6->setRoundedCorners(SAT_CORNER_UPPER_LEFT | SAT_CORNER_LOWER_RIGHT);

        // tab 3 -parameters

        //center_tabs1->appendPage( "t3", new SAT_VisualWidget(0));
        SAT_VisualWidget* parameter_tab1 = new SAT_VisualWidget(15);
        center_tabs1->appendPage( "Parameters", parameter_tab1);
        //parameter_tab1->Layout.innerBorder = SAT_Rect(5,5,5,5);
        parameter_tab1->Layout.spacing = SAT_Point(0,5);
        parameter_tab1->setDrawBorder(false);

          SAT_SliderWidget* t3s1 = new SAT_SliderWidget(SAT_Rect(10,10,200,20));
          parameter_tab1->appendChild(t3s1);
          //AEditor->connect(t3s1,getParameter(1));
          t3s1->setDragSnap(true);
          t3s1->setDrawValueBar(true);

          SAT_SliderWidget* t3s2 = new SAT_SliderWidget(SAT_Rect(10,40,200,20));
          parameter_tab1->appendChild(t3s2);
          //AEditor->connect(t3s2,getParameter(2));
          t3s2->setDragQuantize(true);
          t3s2->setDragQuantizeSteps(11);
          t3s2->setDrawValueBar(true);
          //t3s2->setDrawQuantized(true);

          SAT_SliderWidget* t3s3 = new SAT_SliderWidget(SAT_Rect(10,70,200,20));
          parameter_tab1->appendChild(t3s3);
          //AEditor->connect(t3s3,getParameter(3));
          t3s3->setCursor(SAT_CURSOR_ARROW_LEFT);
          t3s3->setDragDirection(SAT_DIRECTION_LEFT);
          t3s3->setDrawDirection(SAT_DIRECTION_LEFT);

        // center_tabs1->getPages()->setPage(0);
        center_tabs1->selectPage(0);
        //center_tabs1->realignChildren();


  }

  //----------

  virtual ~sa_demo_page_widgets() {
  }

};

//----------------------------------------------------------------------
#endif

