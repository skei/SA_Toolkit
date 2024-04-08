#ifndef demo2_gui_incuded  
#define demo2_gui_incuded  
//----------------------------------------------------------------------

// careful!
// included directly into demo2.h

//----------------------------------------------------------------------

  SAT_MenuWidget* MMenu1 = nullptr;

  #define DEMO_SELECT_COUNT 4
  const char* demo_select_text[DEMO_SELECT_COUNT] = {
    "home",
    "plugin",
    "parameters",
    "gui"
  };

//----------------------------------------------------------------------

  void setupLeftPanel(SAT_Widget* APanel, SAT_Editor* AEditor, SAT_Window* AWindow) {

    SAT_ButtonRowWidget* demo_select = new SAT_ButtonRowWidget(100,DEMO_SELECT_COUNT,demo_select_text,SAT_BUTTON_ROW_SINGLE,true);;
    APanel->appendChildWidget(demo_select);
    demo_select->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
    demo_select->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);

  }

  //----------

  void setupCenterPanel(SAT_Widget* APanel, SAT_Editor* AEditor, SAT_Window* AWindow) {
  }

  //----------

  void setupRightPanel(SAT_Widget* APanel, SAT_Editor* AEditor, SAT_Window* AWindow) {

    //ALeftPanel->setLayoutSizeLimit(SAT_Rect(100,-1,280,-1));
    //ALeftPanel->setLayoutInnerBorder(SAT_Rect(5,5,5,5));
    //ALeftPanel->setLayoutSpacing(SAT_Point(5,5));

    //

    SAT_ScrollBoxWidget* scrollbox1 = new SAT_ScrollBoxWidget(0,true,false);
    APanel->appendChildWidget(scrollbox1);
    scrollbox1->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
    scrollbox1->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_ALL);
    scrollbox1->getContentWidget()->setLayoutInnerBorder(SAT_Rect(5,5,5,5));
    scrollbox1->getContentWidget()->setLayoutSpacing(SAT_Point(5,5));

    //

    SAT_GroupBoxWidget* group1 = new SAT_GroupBoxWidget(150,20,false);
    scrollbox1->appendChildWidget(group1);
    group1->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
    group1->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
    group1->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
    group1->getContainer()->setFillBackground(true);
    group1->getContainer()->setBackgroundColor(0.35);
    group1->getContainer()->setDrawBorder(true);

    SAT_GroupBoxWidget* group2 = new SAT_GroupBoxWidget(100,20,false);
    scrollbox1->appendChildWidget(group2);
    group2->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
    group2->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
    group2->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
    group2->getContainer()->setFillBackground(true);
    group2->getContainer()->setBackgroundColor(0.35);
    group2->getContainer()->setDrawBorder(true);

    // SAT_MovableWidget* movable1 = new SAT_MovableWidget(SAT_Rect(20,0,100,50));
    // scrollbox1->appendChildWidget(movable1);
    // movable1->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP);
    // movable1->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
    // movable1->setIsMovable(true);
    // //movable1->setMovableOffset(SAT_Rect());
    // movable1->setMovableDirections(SAT_DIRECTION_HORIZ);
    // //movable1->setMovableFlags(1); // invert offset
    // movable1->setIsSizable(true);
    // movable1->setSizableBorder(SAT_Rect(5,0,5,5));
    // movable1->setSizableEdges(SAT_EDGE_HORIZ | SAT_EDGE_BOTTOM);

    SAT_SelectorWidget* selector1 = new SAT_SelectorWidget(20,"Select..",MMenu1);
    scrollbox1->appendChildWidget(selector1);
    selector1->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
    selector1->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
    selector1->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);

    SAT_TabsWidget* tabs1 = new SAT_TabsWidget(100,5,20);
    scrollbox1->appendChildWidget(tabs1);
    tabs1->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
    tabs1->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
    tabs1->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
    // tabs1->appendPage("widgets",   create_widgets_page(AEditor,AWindow) );
    // tabs1->appendPage("parameters",create_params_page(AEditor,AWindow)  );
    // tabs1->appendPage("host",      create_host_page(AEditor,AWindow)  );
    // tabs1->appendPage("voices",    create_voices_page(AEditor,AWindow)  );
    // tabs1->appendPage("events",    create_events_page(AEditor,AWindow)  );
    // tabs1->selectPage(0);

  }

//----------------------------------------------------------------------

  void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {

    SAT_RootWidget* root = new SAT_RootWidget(AWindow);
    AWindow->setRootWidget(root);
    root->setFillBackground(false);
    root->setDrawBorder(false);

    // menu(s)

    MMenu1 = new SAT_MenuWidget(SAT_Rect(200,15*5+2+2));
    MMenu1->setLayoutInnerBorder(SAT_Rect(2,2,2,2));
    //MMenu1->setLayoutSpacing(SAT_Point(1,1));
    MMenu1->appendChildWidget( new SAT_MenuItemWidget(15,"Item 1") );
    MMenu1->appendChildWidget( new SAT_MenuItemWidget(15,"Item 2") );
    MMenu1->appendChildWidget( new SAT_MenuItemWidget(15,"Item 3") );
    MMenu1->appendChildWidget( new SAT_MenuItemWidget(15,"Item 4") );
    MMenu1->appendChildWidget( new SAT_MenuItemWidget(15,"Item 5") );

    // header

    SAT_PluginHeaderWidget* header = new SAT_PluginHeaderWidget(60,"demo2");
    root->appendChildWidget(header);
    header->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
    header->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
    header->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
    header->setPluginNameSize(36);
    header->setPluginNameColor(0.8);
    header->setPluginNameOffset(SAT_Rect(60,12,0,0));

    // footer

    SAT_PluginFooterWidget* footer = new SAT_PluginFooterWidget(20,"...");
    root->appendChildWidget(footer);
    footer->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_LEFT);
    footer->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
    footer->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_BOTTOM);

    // main

    SAT_PanelWidget* main_panel = new SAT_PanelWidget(0);
    root->appendChildWidget(main_panel);
    main_panel->setLayoutInnerBorder(SAT_Rect(10,10,10,10));
    //main_panel->setLayoutSpacing(SAT_Point(10,10));
    main_panel->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
    main_panel->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_ALL);
    //main_panel->setFillBackground(false);
    //main_panel->setDrawBorder(false);

    // left

    SAT_PanelWidget* left_panel = new SAT_PanelWidget(200);
    main_panel->appendChildWidget(left_panel);
    left_panel->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
    left_panel->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_VERT);
    left_panel->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_LEFT);
    setupLeftPanel(left_panel,AEditor,AWindow);

    // left sizer

    SAT_SizerWidget* left_sizer = new SAT_SizerWidget(5,SAT_DIRECTION_LEFT,left_panel);
    main_panel->appendChildWidget(left_sizer);
    left_sizer->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
    left_sizer->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_VERT);
    left_sizer->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_LEFT);
    left_sizer->setFillBackground(true);
    //left_sizer->setBackgroundColor(0.2);
    left_sizer->setDrawBorder(false);

    // right

    SAT_PanelWidget* right_panel = new SAT_PanelWidget(200);
    main_panel->appendChildWidget(right_panel);
    right_panel->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_RIGHT);
    right_panel->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_VERT);
    right_panel->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_RIGHT);
    setupRightPanel(right_panel,AEditor,AWindow);

    // right sizer

    SAT_SizerWidget* right_sizer = new SAT_SizerWidget(5,SAT_DIRECTION_RIGHT,right_panel);
    main_panel->appendChildWidget(right_sizer);
    right_sizer->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_RIGHT);
    right_sizer->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_VERT);
    right_sizer->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_RIGHT);
    right_sizer->setFillBackground(true);
    //right_sizer->setBackgroundColor(0.2);
    right_sizer->setDrawBorder(false);

    // center

    SAT_PanelWidget* center_panel = new SAT_PanelWidget(0);
    main_panel->appendChildWidget(center_panel);
    center_panel->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT);
    center_panel->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_ALL);
    setupCenterPanel(center_panel,AEditor,AWindow);

    // menu(s)

    root->appendChildWidget(MMenu1);


  }

//----------------------------------------------------------------------
#endif