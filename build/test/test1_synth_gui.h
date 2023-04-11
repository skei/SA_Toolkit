

  //SAT_Editor* createEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight) final {
  //  //SAT_PRINT;
  //  SAT_Editor* editor = new SAT_Editor(AListener,AWidth,AHeight);
  //  return editor;
  //}

  //----------

  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    //SAT_PRINT;
    
    // root

    SAT_PanelWidget* MRootPanel = new SAT_PanelWidget( SAT_Rect(0,0,EDITOR_WIDTH,EDITOR_HEIGHT) );
    AWindow->appendRootWidget(MRootPanel);
    MRootPanel->setFillBackground(true);
    MRootPanel->setDrawBorder(false);
    //MRootPanel->setInnerBorder(SAT_Rect(5,5,5,5));

    // menu

    //SAT_MenuWidget* menu = new SAT_MenuWidget(SAT_Rect(260,70,150,150));
    SAT_MenuWidget* menu = new SAT_MenuWidget(SAT_Rect(0,0,150,150));
    menu->setRightClickClose(true);

    SAT_MenuItemWidget* i1 = new SAT_MenuItemWidget(SAT_Rect(5,5,140,15),"Item 1");
    menu->appendChildWidget(i1);
    i1->setDrawBorder(false);
    //i1->setAlignment(SAT_WIDGET_ALIGN_TOP);
    i1->setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);

    SAT_MenuItemWidget* i2 = new SAT_MenuItemWidget(SAT_Rect(5,20,140,15),"Item 2");
    menu->appendChildWidget(i2);
    i2->setDrawBorder(false);
    //i2->setAlignment(SAT_EDGE_TOP);
    i2->setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);

    SAT_MenuItemWidget* i3 = new SAT_MenuItemWidget(SAT_Rect(5,35,140,15),"Item 3");
    menu->appendChildWidget(i3);
    i3->setDrawBorder(false);
    //i3->setAlignment(SAT_EDGE_TOP);
    i3->setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);

    // header
    
    const char* format = getPluginFormat();
    SAT_PluginHeaderWidget* plugin_header = new SAT_PluginHeaderWidget(SAT_Rect(0,0,EDITOR_WIDTH,40),"toolkit",format);
    MRootPanel->appendChildWidget(plugin_header);
    
    // footer
    
    SAT_PluginFooterWidget* plugin_footer = new SAT_PluginFooterWidget(SAT_Rect(0,0,EDITOR_WIDTH,20));
    MRootPanel->appendChildWidget(plugin_footer);


    
// left column ------------------------------



    //SAT_PanelWidget* left_column = new SAT_PanelWidget(SAT_Rect(10,10,200,200));
    SAT_ScrollBoxWidget* left_column = new SAT_ScrollBoxWidget(SAT_Rect(10,10,200,200),true,false);
    
    SAT_Widget* content = left_column->getContentWidget();
    content->setInnerBorder(SAT_Rect(6,6,6,6));
    content->setSpacing(SAT_Point(3,3));
    
    
    MRootPanel->appendChildWidget(left_column);
    left_column->setLayout(SAT_WIDGET_ALIGN_LEFT,SAT_WIDGET_STRETCH_VERTICAL);
    left_column->setDrawBorder(false);
    left_column->setFillBackground(true);
    left_column->setBackgroundColor(0.4);
    //left_column->setInnerBorder(SAT_Rect(10,10,10,10));
    //left_column->setSpacing(SAT_Point(5,5));
    
    SAT_SizerWidget* sizer1 = new SAT_SizerWidget(SAT_Rect(5),SAT_DIRECTION_LEFT,left_column);
    MRootPanel->appendChildWidget(sizer1);
    sizer1->setLayout(SAT_WIDGET_ALIGN_LEFT,SAT_WIDGET_STRETCH_VERTICAL);


    
    //



    SAT_ValueWidget* val = new SAT_ValueWidget(20,"Param 1", 0.0);
    left_column->appendChildWidget(val);
    AEditor->connect(val,getParameter(0));
    val->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
    val->setTextSize(12);
    val->setRoundedCorners(true);
    val->setCornerSize(5);
    
    
    SAT_GroupBoxWidget* groupbox1 = new SAT_GroupBoxWidget(100,true);
    left_column->appendChildWidget(groupbox1);
    groupbox1->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
    
      SAT_PanelWidget* grpage1 = new SAT_PanelWidget(0);
      groupbox1->appendChildWidget(grpage1);
      grpage1->setBackgroundColor(SAT_Color(0.40,0.35,0.35));
      grpage1->setLayout(SAT_WIDGET_ALIGN_NONE,SAT_WIDGET_STRETCH_ALL);
      
    SAT_GroupBoxWidget* groupbox2 = new SAT_GroupBoxWidget(100,true);
    left_column->appendChildWidget(groupbox2);
    groupbox2->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
    
      SAT_PanelWidget* grpage2 = new SAT_PanelWidget(0);
      groupbox2->appendChildWidget(grpage2);
      grpage2->setBackgroundColor(SAT_Color(0.35,0.40,0.35));
      grpage2->setLayout(SAT_WIDGET_ALIGN_NONE,SAT_WIDGET_STRETCH_ALL);

        for (uint32_t i=0; i<3; i++) {
          SAT_KnobWidget* k = new SAT_KnobWidget(SAT_Rect(10+(i*40),10,35,35),"",0);
          grpage2->appendChildWidget(k);
          k->setTextSize(8);
          k->setValueSize(8);
          k->setArcThickness(4);
        }

    SAT_GroupBoxWidget* groupbox3 = new SAT_GroupBoxWidget(100,true);
    left_column->appendChildWidget(groupbox3);
    groupbox3->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
    
      SAT_PanelWidget* grpage3 = new SAT_PanelWidget(0);
      groupbox3->appendChildWidget(grpage3);
      grpage3->setBackgroundColor(SAT_Color(0.35,0.35,0.40));
      grpage3->setLayout(SAT_WIDGET_ALIGN_NONE,SAT_WIDGET_STRETCH_ALL);

        for (uint32_t i=0; i<3; i++) {
          SAT_SliderWidget* s = new SAT_SliderWidget(SAT_Rect(10,10+(i*20),100,15),"",0);
          grpage3->appendChildWidget(s);
          s->setTextSize(8);
          s->setValueSize(8);
        }

    SAT_DragValueWidget* dragval = new SAT_DragValueWidget(20,"Param 2", 0.0);
    left_column->appendChildWidget(dragval);
    AEditor->connect(dragval,getParameter(1));
    dragval->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
    dragval->setTextSize(7);
    dragval->setValueSize(7);
    dragval->setSnap(true);
    dragval->setSnapPos(0.5);
    dragval->setAutoHideCursor(false);
    dragval->setAutoLockCursor(true);
    dragval->setRoundedCorners(true);
    dragval->setCornerSize(2);
    dragval->setDrawBorder(true);
    dragval->setValueSize(9);
    dragval->setValueColor(SAT_Black);
    dragval->setValueOffset(SAT_Rect(0,0,8,0));
    dragval->setBorderWidth(0.5);
    dragval->setBorderColor(SAT_LightGrey);
    dragval->setDrawDropShadow(true);
    dragval->setDropShadowFeather(6);
    dragval->setDropShadowOffset(1,1);

    SAT_SliderWidget* slider = new SAT_SliderWidget(20,"Param 3", 0.0);
    left_column->appendChildWidget(slider);
    AEditor->connect(slider,getParameter(2));
    slider->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
    slider->setTextSize(12);
    slider->setBipolar(true);
    slider->setBipolarCenter(0.5);
    slider->setSnap(true);
    slider->setSnapPos(0.5);
    slider->setAutoHideCursor(true);
    slider->setAutoLockCursor(true);
    //slider->setModulation(0.25);
    slider->setDrawModulation(true);
    slider->setModulationColor( SAT_Color(1,1,1,0.25) );
    slider->setDragDirection(SAT_DIRECTION_RIGHT);

    SAT_ButtonWidget* button1 = new SAT_ButtonWidget(20);
    left_column->appendChildWidget(button1);
    button1->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
    
    
    SAT_ButtonWidget* button2 = new SAT_ButtonWidget(20);
    left_column->appendChildWidget(button2);
    button2->setIsToggle(true);
    button2->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);

    //button1->setAlignment(SAT_EDGE_LEFT);
    //button2->setAnchors(SAT_EDGE_BOTTOM);
    
    SAT_TabsWidget* tabs = new SAT_TabsWidget(100,3);
    left_column->appendChildWidget(tabs);
    tabs->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);

      SAT_PanelWidget* page1 = new SAT_PanelWidget(0);
      tabs->appendPage("page1",page1);
      page1->setBackgroundColor(SAT_Color(0.40,0.35,0.35));
      
      SAT_PanelWidget* page2 = new SAT_PanelWidget(0);
      tabs->appendPage("page2",page2);
      page2->setBackgroundColor(SAT_Color(0.35,0.40,0.35));

        for (uint32_t i=0; i<3; i++) {
          SAT_KnobWidget* k = new SAT_KnobWidget(SAT_Rect(10+(i*40),10,35,35),"",0);
          page2->appendChildWidget(k);
          k->setTextSize(8);
          k->setValueSize(8);
          k->setArcThickness(4);
        }

      SAT_PanelWidget* page3 = new SAT_PanelWidget(0);
      tabs->appendPage("page3",page3);
      page3->setBackgroundColor(SAT_Color(0.35,0.35,0.40));

        for (uint32_t i=0; i<3; i++) {
          SAT_SliderWidget* s = new SAT_SliderWidget(SAT_Rect(10,10+(i*20),100,15),"",0);
          page3->appendChildWidget(s);
          s->setTextSize(8);
          s->setValueSize(8);
          if (i==0) AEditor->connect(s,getParameter(3));
        }

      tabs->selectPage(0);
    
    SAT_ButtonRowWidget* buttonrow = new SAT_ButtonRowWidget(20,5,buttontext,SAT_BUTTON_ROW_MULTI);
    left_column->appendChildWidget(buttonrow);
    buttonrow->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
    buttonrow->setValueIsBits(true,8);
    buttonrow->setRoundedCorners(true);
    buttonrow->setCornerSizes(8,8,8,8);
    
    SAT_ButtonRowWidget* buttonrow2 = new SAT_ButtonRowWidget(20,5,buttontext,SAT_BUTTON_ROW_SINGLE);
    left_column->appendChildWidget(buttonrow2);
    buttonrow2->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
    buttonrow2->setValueIsBits(true,8);
    buttonrow2->setRoundedCorners(true);
    buttonrow2->setCornerSizes(4,4,4,4);
    
    SAT_ScrollBarWidget* scrollbar = new SAT_ScrollBarWidget(20);
    left_column->appendChildWidget(scrollbar);
    scrollbar->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
    scrollbar->setValue(0.3);
    //scrollbar->setThumbSize(0.3);

    SAT_MenuItemWidget* menuitem = new SAT_MenuItemWidget(20,"MenuItem");
    left_column->appendChildWidget(menuitem);
    menuitem->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);

    SAT_SelectorWidget* selector = new SAT_SelectorWidget(20,"Select",menu);
    left_column->appendChildWidget(selector);
    selector->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);

    SAT_ScrollBarWidget0* scrollbar0 = new SAT_ScrollBarWidget0(20);
    left_column->appendChildWidget(scrollbar0);
    scrollbar0->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
    
    SAT_KeyboardWidget* keyboard = new SAT_KeyboardWidget(30);
    left_column->appendChildWidget(keyboard);
    keyboard->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);

    SAT_SliderBankWidget* sliderbank = new SAT_SliderBankWidget(30);
    left_column->appendChildWidget(sliderbank);
    sliderbank->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);

    SAT_ValueGraphWidget* valuegraph = new SAT_ValueGraphWidget(30,16);
    left_column->appendChildWidget(valuegraph);
    valuegraph->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);

    SAT_SymbolWidget* symbol1 = new SAT_SymbolWidget(10,SAT_SYMBOL_RECT);
    left_column->appendChildWidget(symbol1);
    symbol1->setLayout(SAT_WIDGET_ALIGN_LEFT,SAT_WIDGET_STRETCH_NONE);
    symbol1->setCursor(SAT_CURSOR_PENCIL);

    SAT_SymbolWidget* symbol2 = new SAT_SymbolWidget(SAT_Rect(10),SAT_SYMBOL_TRI_DOWN);
    left_column->appendChildWidget(symbol2);
    symbol2->setLayout(SAT_WIDGET_ALIGN_LEFT,SAT_WIDGET_STRETCH_NONE);
    symbol2->setCursor(SAT_CURSOR_MOVE);

    SAT_SymbolWidget* symbol3 = new SAT_SymbolWidget(SAT_Rect(10),SAT_SYMBOL_CIRCLE);
    left_column->appendChildWidget(symbol3);
    symbol3->setLayout(SAT_WIDGET_ALIGN_LEFT,SAT_WIDGET_STRETCH_NONE);
    symbol3->setCursor(SAT_CURSOR_INVALID);

    SAT_SymbolWidget* symbol4 = new SAT_SymbolWidget(SAT_Rect(10),SAT_SYMBOL_FILLED_RECT);
    left_column->appendChildWidget(symbol4);
    symbol4->setLayout(SAT_WIDGET_ALIGN_LEFT,SAT_WIDGET_STRETCH_NONE);
    symbol4->setCursor(SAT_CURSOR_WAIT);



// middle colume ------------------------------



    SAT_PanelWidget* middle_column = new SAT_PanelWidget(SAT_Rect(200,490));
    MRootPanel->appendChildWidget(middle_column);
    middle_column->setLayout(SAT_WIDGET_ALIGN_LEFT,SAT_WIDGET_STRETCH_NONE);//VERTICAL);
    middle_column->setName("middle_column");
    middle_column->setDrawBorder(true);
    middle_column->setBorderColor(0.5);
    middle_column->setFillBackground(true);
    middle_column->setBackgroundColor(0.375);
    middle_column->setRoundedCorners(true);
    middle_column->setCornerSize(5);
    
    //

    SAT_GridWidget* grid = new SAT_GridWidget(SAT_Rect(10,10,180,80),16,8);
    middle_column->appendChildWidget(grid);
    
    SAT_PanelWidget* pp2 = new SAT_PanelWidget(SAT_Rect(10,100,180,40));
    middle_column->appendChildWidget(pp2);
    pp2->setDrawBorder(true);
    pp2->setBorderColor(SAT_DarkestGrey);
    pp2->setBorderWidth(0.5);
    pp2->setRoundedCorners(true);
    pp2->setCornerSize(5);
    pp2->setFillBackground(false);
    pp2->setDrawDropShadow(true);
    pp2->setDropShadowFeather(5);
    pp2->setDropShadowOffset(3,3);
    pp2->setDropShadowColors( SAT_Grey, SAT_DarkerGrey );
    //pp2->setDropShadowOffset(1,1);
    pp2->setDropShadowInner(true);

    SAT_PanelWidget* ppp = new SAT_PanelWidget(SAT_Rect(10,145,180,40));
    middle_column->appendChildWidget(ppp);
    ppp->setFillBackground(true);
    ppp->setFillGradient(true);
    ppp->setGradientColors(SAT_Grey,SAT_DarkerGrey);
    ppp->setDrawBorder(true);
    ppp->setBorderColor(SAT_LightGrey);
    ppp->setBorderWidth(0.5);
    ppp->setRoundedCorners(true);
    ppp->setCornerSize(4);
    ppp->setDrawDropShadow(true);
    ppp->setDropShadowFeather(10);
    //ppp->setDropShadowColors( SAT_Color(0,0,0,0.75), SAT_Color(0,0,0,0) );
    ppp->setDropShadowColors( SAT_DarkestGrey, SAT_DarkGrey );
    ppp->setDropShadowOffset(1,1);
    //ppp->setAlignment(SAT_EDGE_RIGHT);
    //ppp->setStretching(SAT_EDGE_BOTTOM);
    
    MVoicesWidget = new SAT_VoicesWidget(SAT_Rect(10,190,180,10),NUM_VOICES);
    middle_column->appendChildWidget(MVoicesWidget);
    //MVoicesWidget->setAlignment(SAT_EDGE_BOTTOM);
    //MVoicesWidget->setStretching(SAT_EDGE_RIGHT);

    SAT_KnobWidget* knob = new SAT_KnobWidget(SAT_Rect(10,205,50,50),"%",0.0);
    middle_column->appendChildWidget(knob);
    knob->setBipolar(true);
    knob->setBipolarCenter(0.5);
    knob->setSnap(true);
    knob->setSnapPos(0.5);
    knob->setValue(0.25);
    
//*********************************************    

    SAT_FreqRespWidget<SAT_FR_Noop>* freqres = new SAT_FreqRespWidget<SAT_FR_Noop>(SAT_Rect(70,205,50,50));
    middle_column->appendChildWidget(freqres);

//*********************************************    
    
    
    SAT_CurveWidget* curve1 = new SAT_CurveWidget(SAT_Rect(10,260,20,20),false);
    middle_column->appendChildWidget(curve1);
    
    SAT_CurveWidget* curve2 = new SAT_CurveWidget(SAT_Rect(35,260,20,20),true);
    middle_column->appendChildWidget(curve2);
    
    MWaveformWidget = new SAT_WaveformWidget(SAT_Rect(10,285,180,60));
    middle_column->appendChildWidget(MWaveformWidget);
    MWaveformWidget->setNumGrid(4);
    MWaveformWidget->setNumSubGrid(2);
    //MWaveformWidget->setAlignment(SAT_EDGE_BOTTOM);
    //MWaveformWidget->setStretching(SAT_EDGE_RIGHT);
    
    SAT_TextBoxWidget* textbox = new SAT_TextBoxWidget(SAT_Rect(10,400,100,70));
    middle_column->appendChildWidget(textbox);
    //textbox->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
      for (uint32_t i=0; i<25; i++) { textbox->appendLine("Hello world! This is a test to see if it works.. 123abcxyzæøå"); }
    
    SAT_ButtonRowWidget* buttonrow3 = new SAT_ButtonRowWidget(SAT_Rect(120,400,70,70),5,buttontext,SAT_BUTTON_ROW_SINGLE,true);
    middle_column->appendChildWidget(buttonrow3);
    buttonrow3->setRoundedCorners(true);
    buttonrow3->setCornerSize(3);

//    // tween test
//
//    SAT_Tweening* tweens = AWindow->getTweens();
//    //tweens->appendTween(MWaveformWidget,0,1000,5.0);
//    double startpos[4] = { 100, 185,   0,  0 };
//    double endpos[4]   = {  10, 285, 180, 60 };
//    tweens->appendTween(SAT_WIDGET_TWEEN_RECT/*666*/,MWaveformWidget,38,4,startpos,endpos,2.0);



// right column ------------------------------



    SAT_PanelWidget* right_column = new SAT_PanelWidget(SAT_Rect(10,10,200,200));
    MRootPanel->appendChildWidget(right_column);
    right_column->setStretching(SAT_WIDGET_STRETCH_ALL);
    //right_column->setLayout(SAT_WIDGET_ALIGN_LEFT,SAT_WIDGET_STRETCH_VERTICAL);
    right_column->setDrawBorder(false);
    right_column->setInnerBorder(5);
    right_column->setFillBackground(true);
    right_column->setBackgroundColor(0.4);
    
    
    //

    SAT_GraphWidget* graph = new SAT_GraphWidget(SAT_Rect(10,10,300,245));
    right_column->appendChildWidget(graph);
    graph->setFillBackground(true);
    //graph->setAlignment(SAT_EDGE_BOTTOM);
    //graph->setStretching(SAT_EDGE_RIGHT | SAT_EDGE_BOTTOM);
      for (uint32_t i=0; i<5; i++) {
        SAT_GraphModule* module = new SAT_GraphModule();
        module->numInputs = 2;
        module->inputs[0] = SAT_PIN_SIGNAL;
        module->outputs[0] = SAT_PIN_SIGNAL;
        module->numOutputs = 2;
        graph->addModule(module,i*10,i*10,"module");
      }
      
    //SAT_PanelWidget* sizable = new SAT_PanelWidget(SAT_Rect(165,265,145,115));
    //right_column->appendChildWidget(sizable);
    //sizable->setDrawBorder(true);

    SAT_TimelineWidget* timeline = new SAT_TimelineWidget(SAT_Rect(10,390,300,90));
    right_column->appendChildWidget(timeline);
    timeline->setStretching(SAT_WIDGET_STRETCH_RIGHT | SAT_WIDGET_STRETCH_BOTTOM);
    timeline->setDrawBorder(true);
    timeline->setBorderColor(SAT_LightGrey);
    timeline->setBorderWidth(0.5);
    
      SAT_TimelineTrack* track1 = new SAT_TimelineTrack("Track 1");
      timeline->addTrack(track1);
    
        SAT_TimelineSegment* segment1 = new SAT_TimelineSegment("Clip1",0,10);
        track1->addSegment(segment1);

        SAT_TimelineSegment* segment2 = new SAT_TimelineSegment("C2",11,15);
        track1->addSegment(segment2);

        SAT_TimelineSegment* segment3 = new SAT_TimelineSegment("3",15.2,17);
        track1->addSegment(segment3);

      SAT_TimelineTrack* track2 = new SAT_TimelineTrack("Track 1");
      timeline->addTrack(track2);
    

      
    
    
      
//
    
    // menus etc have to be appended last, because they need to be drawn on
    // top of other widgets (when visible)..

    MRootPanel->appendChildWidget(menu);
    
// animation
    
    test1_synth_widget* movable = new test1_synth_widget(SAT_Rect(415,305,145,115));
    MRootPanel->appendChildWidget(movable);
    AWindow->registerTimerWidget(movable);
    movable->setAlignment(SAT_WIDGET_ALIGN_PARENT );

//
    
    //AEditor->connect(val,     getParameter(0));
    //AEditor->connect(dragval, getParameter(1));
    //AEditor->connect(slider,  getParameter(2));

    return true;
  }
