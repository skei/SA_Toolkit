

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
    MRootPanel->setInnerBorder(SAT_Rect(5,5,5,5));

    // menu

    //SAT_MenuWidget* menu = new SAT_MenuWidget(SAT_Rect(260,70,150,150));
    SAT_MenuWidget* menu = new SAT_MenuWidget(SAT_Rect(0,0,150,150));
    menu->setRightClickClose(true);

    SAT_MenuItemWidget* i1 = new SAT_MenuItemWidget(SAT_Rect(5,5,140,15),"Item 1");
    menu->appendChildWidget(i1);
    i1->setDrawBorder(false);

    SAT_MenuItemWidget* i2 = new SAT_MenuItemWidget(SAT_Rect(5,20,140,15),"Item 2");
    menu->appendChildWidget(i2);
    i2->setDrawBorder(false);

    SAT_MenuItemWidget* i3 = new SAT_MenuItemWidget(SAT_Rect(5,35,140,15),"Item 3");
    menu->appendChildWidget(i3);
    i3->setDrawBorder(false);

    // header
    
    const char* format = getPluginFormat();
    SAT_PluginHeaderWidget* plugin_header = new SAT_PluginHeaderWidget(SAT_Rect(0,0,EDITOR_WIDTH,40),"toolkit",format);
    MRootPanel->appendChildWidget(plugin_header);
    
    // footer
    
    SAT_PluginFooterWidget* plugin_footer = new SAT_PluginFooterWidget(SAT_Rect(0,0,EDITOR_WIDTH,20));
    MRootPanel->appendChildWidget(plugin_footer);
    
// left column

    SAT_Widget* left_column = new SAT_Widget(SAT_Rect(10,10,200,200));
    MRootPanel->appendChildWidget(left_column);
    left_column->setAlignment(SAT_EDGE_LEFT);
    left_column->setStretching(SAT_EDGE_TOP | SAT_EDGE_BOTTOM);
    
    //

    SAT_ValueWidget* val = new SAT_ValueWidget(SAT_Rect(10,10,180,20),"Param 1", 0.0);
    left_column->appendChildWidget(val);
    val->setTextSize(12);
    val->setRoundedCorners(true);
    val->setCornerSize(5);

    SAT_DragValueWidget* dragval = new SAT_DragValueWidget(SAT_Rect(10,35,180,20),"Param 2", 0.0);
    left_column->appendChildWidget(dragval);
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

    SAT_SliderWidget* slider = new SAT_SliderWidget(SAT_Rect(10,60,180,20),"Param 3", 0.0);
    left_column->appendChildWidget(slider);
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

    SAT_ButtonWidget* button1 = new SAT_ButtonWidget(SAT_Rect(10,85,85,20));
    left_column->appendChildWidget(button1);
    
    SAT_ButtonWidget* button2 = new SAT_ButtonWidget(SAT_Rect(105,85,85,20));
    left_column->appendChildWidget(button2);
    button2->setIsToggle(true);

    //button1->setAlignment(SAT_EDGE_LEFT);
    //button2->setAnchors(SAT_EDGE_BOTTOM);
    
    SAT_TabsWidget* tabs = new SAT_TabsWidget(SAT_Rect(10,110,180,100),3);
    left_column->appendChildWidget(tabs);

      SAT_PanelWidget* page1 = new SAT_PanelWidget(SAT_Rect(0,0,180,80));
      SAT_PanelWidget* page2 = new SAT_PanelWidget(SAT_Rect(0,0,180,80));
      SAT_PanelWidget* page3 = new SAT_PanelWidget(SAT_Rect(0,0,180,80));
      page1->setBackgroundColor(SAT_Color(0.40,0.35,0.35));
      page2->setBackgroundColor(SAT_Color(0.35,0.40,0.35));
      page3->setBackgroundColor(SAT_Color(0.35,0.35,0.40));
      tabs->appendPage("page1",page1);
      tabs->appendPage("page2",page2);
      tabs->appendPage("page3",page3);
      tabs->selectPage(0);
    
    SAT_ButtonRowWidget* buttonrow = new SAT_ButtonRowWidget(SAT_Rect(10,215,180,20),5,buttontext,SAT_BUTTON_ROW_MULTI);
    left_column->appendChildWidget(buttonrow);
    buttonrow->setValueIsBits(true,8);
    buttonrow->setRoundedCorners(true);
    buttonrow->setCornerSizes(8,8,8,8);
    
    SAT_ButtonRowWidget* buttonrow2 = new SAT_ButtonRowWidget(SAT_Rect(10,240,180,20),5,buttontext,SAT_BUTTON_ROW_SINGLE);
    left_column->appendChildWidget(buttonrow2);
    buttonrow2->setValueIsBits(true,8);
    buttonrow2->setRoundedCorners(true);
    buttonrow2->setCornerSizes(4,4,4,4);
    
    SAT_ScrollBarWidget* scrollbar = new SAT_ScrollBarWidget(SAT_Rect(10,265,180,20));
    left_column->appendChildWidget(scrollbar);
    scrollbar->setValue(0.3);
    //scrollbar->setThumbSize(0.3);

    SAT_MenuItemWidget* menuitem = new SAT_MenuItemWidget(SAT_Rect(10,290,180,20),"MenuItem");
    left_column->appendChildWidget(menuitem);

    SAT_SelectorWidget* selector = new SAT_SelectorWidget(SAT_Rect(10,315,180,20),"Select",menu);
    left_column->appendChildWidget(selector);

    SAT_SymbolWidget* symbol1 = new SAT_SymbolWidget(SAT_Rect(10,340,10,10),SAT_SYMBOL_RECT);
    left_column->appendChildWidget(symbol1);

    SAT_SymbolWidget* symbol2 = new SAT_SymbolWidget(SAT_Rect(25,340,10,10),SAT_SYMBOL_TRI_DOWN);
    left_column->appendChildWidget(symbol2);

    SAT_SymbolWidget* symbol3 = new SAT_SymbolWidget(SAT_Rect(40,340,10,10),SAT_SYMBOL_CIRCLE);
    left_column->appendChildWidget(symbol3);

    SAT_SymbolWidget* symbol4 = new SAT_SymbolWidget(SAT_Rect(55,340,10,10),SAT_SYMBOL_FILLED_RECT);
    left_column->appendChildWidget(symbol4);

// middle colume

    SAT_PanelWidget* middle_column = new SAT_PanelWidget(SAT_Rect(10,10,200,200));
    MRootPanel->appendChildWidget(middle_column);
    middle_column->setAlignment(SAT_EDGE_LEFT);
    middle_column->setStretching(SAT_EDGE_TOP | SAT_EDGE_BOTTOM);
    middle_column->setName("middle_column");
    
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
    
// tween test

    SAT_Tweening* tweens = AWindow->getTweens();
    //tweens->appendTween(MWaveformWidget,0,1000,5.0);
    double startpos[4] = { 100, 185,   0,  0 };
    double endpos[4]   = {  10, 285, 180, 60 };
    tweens->appendTween(666,MWaveformWidget,38,4,startpos,endpos,2.0);

// right column

    SAT_Widget* right_column = new SAT_Widget(SAT_Rect(10,10,200,200));
    MRootPanel->appendChildWidget(right_column);
    right_column->setAlignment(SAT_EDGE_LEFT);
    right_column->setStretching(SAT_EDGE_TOP | SAT_EDGE_RIGHT | SAT_EDGE_BOTTOM);
    
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
      
//
    
    // menus etc have to be appended last, because they need to be drawn on
    // top of other widgets (when visible)..

    MRootPanel->appendChildWidget(menu);
    
//
    
//    SAT_MovableWidget* movable = new SAT_MovableWidget(SAT_Rect(420,235,120,50));
//    MRootPanel->appendChildWidget(movable);
//    //movable->setBackgroundColor(SAT_Color(0.40,0.35,0.35));
//    movable->setFillBackground(true);
//    movable->setBackgroundColor(SAT_Green2);
//    //movable->setFillGradient(true);
//    //movable->setGradientColors(SAT_Grey,SAT_DarkerGrey);
//    movable->setDrawBorder(true);
//    movable->setBorderColor(SAT_LightGrey);
//    movable->setBorderWidth(1);
//    movable->setRoundedCorners(true);
//    movable->setCornerSize(6);
//    //movable->setDrawDropShadow(true);
//    //movable->setDropShadowFeather(10);
//    //movable->setDropShadowOffset(1,1);
    
    test1_synth_widget* animated = new test1_synth_widget(SAT_Rect(410,305,300,115));
    //myWidget* animated = new myWidget(SAT_Rect(-50,340,140,80));
    MRootPanel->appendChildWidget(animated);
    AWindow->registerTimerWidget(animated);
    //animated->setAlignment(SAT_EDGE_TOP );
    //animated->setStretching(SAT_EDGE_RIGHT);
    
//
    
    AEditor->connect(val,     getParameter(0));
    AEditor->connect(dragval, getParameter(1));
    AEditor->connect(slider,  getParameter(2));

    return true;
  }
