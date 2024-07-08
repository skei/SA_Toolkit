
// included directly into sa_demo.h

//------------------------------
private:
//------------------------------

  SAT_MenuWidget*   file_menu = nullptr;
  SAT_MenuWidget*   edit_menu = nullptr;
  SAT_PagesWidget*  pages     = nullptr;

  #define button_grid_count 3
  const char* button_grid_texts[button_grid_count] = {
    "Start",
    "Widgets",
    "Parameters"
  };

//------------------------------
public:
//------------------------------

  bool setupEditor(SAT_Editor* AEditor) final {
    SAT_Window* window = AEditor->getWindow();

    // root

    sa_demo_root_widget* root = new sa_demo_root_widget( window, SAT_Rect() );
    window->setRootWidget(root);
    root->setFillBackground(false);
    root->setDrawBorder(false);
    //root->setBorderColor(0.4);

    //------------------------------

    // menus

    file_menu = new SAT_MenuWidget(SAT_Rect(100,100));
    edit_menu = new SAT_MenuWidget(SAT_Rect(100,100));
    pages     = new SAT_PagesWidget(0);

    //------------------------------
    //
    //------------------------------

    // header

    SAT_PluginHeaderWidget* header = new SAT_PluginHeaderWidget(40,"demo");
    root->appendChild(header);

    // main menu

    SAT_MainMenuWidget* mainmenu = new SAT_MainMenuWidget(24);
    root->appendChild(mainmenu);
    mainmenu->Layout.innerBorder = {2,2,2,2};
      mainmenu->appendMenu("File",            file_menu);
      mainmenu->appendMenu("View",            edit_menu);
      mainmenu->appendMenu("Options",         edit_menu);
      mainmenu->appendMenu("Help",            edit_menu);
      mainmenu->appendMenu("Something else",  edit_menu);

    // footer

    SAT_PluginFooterWidget* footer = new SAT_PluginFooterWidget(25,"  ...");
    root->appendChild(footer);
    window->setHintWidget(footer);

    // center

    SAT_VisualWidget* center = new SAT_VisualWidget(0);
    root->appendChild(center);
    center->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    center->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
    //center->Layout.innerBorder  = SAT_Rect(5,5,5,5);
    //center->Layout.spacing      = SAT_Point(0,0);
    center->setFillBackground(true);
    center->setDrawBorder(false);

    //------------------------------

    // left_panel

    SAT_VisualWidget* left_panel = new SAT_VisualWidget(150);
    center->appendChild(left_panel);
    left_panel->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    left_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_VERT;
    left_panel->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_LEFT;
    left_panel->setFillBackground(false);
    //left_panel->setBackgroundColor(0.5);
    left_panel->setDrawBorder(false);
    left_panel->Layout.innerBorder = {10,10,10,10};

    // sizer

    // SAT_SizerWidget* left_sizer = new SAT_SizerWidget(5,SAT_DIRECTION_LEFT,left_panel);
    // center->appendChild(left_sizer);

    // main_panel

    SAT_VisualWidget* main_panel = new SAT_VisualWidget(0);
    center->appendChild(main_panel);
    main_panel->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    main_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
    main_panel->Layout.innerBorder = {0,10,10,10};
    main_panel->setFillBackground(false);
    main_panel->setDrawBorder(false);

    //------------------------------
    // left_panel
    //------------------------------

    sa_demo_button_grid_widget* button_grid = new sa_demo_button_grid_widget(button_grid_count*20,1,button_grid_count,button_grid_texts,pages);
    left_panel->appendChild(button_grid);
    button_grid->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    button_grid->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    button_grid->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    button_grid->setSingle(true,true);
    //button_grid->setFillCellsGradient(true);
    //button_grid->setDrawRoundedCorners(true);
    //button_grid->setRoundedCorners(SAT_CORNER_ALL);
    //button_grid->setRoundedCornerSize(5);
    button_grid->selectCell(0,0);
    button_grid->setCanDeselectAll(false);

    //------------------------------
    // main_panel
    //------------------------------

    //SAT_PagesWidget* pages = new SAT_PagesWidget(0);
    main_panel->appendChild(pages);
    pages->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    pages->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;

    pages->appendPage( new sa_demo_page_blank(this,AEditor) );
    pages->appendPage( new sa_demo_page_widgets(this,AEditor) );
    pages->appendPage( new sa_demo_page_parameters(this,AEditor) );

    pages->setPage(0);
    pages->realignChildren();

    
    //------------------------------
    //
    //------------------------------

    root->appendChild(file_menu);
    root->appendChild(edit_menu);


    return true;
  }

  //----------

  // void cleanupEditor(SAT_Editor* AEditor) final {
  // }

  //----------

  bool setupOverlay(SAT_Editor* AEditor) final {
    SAT_Window* window = AEditor->getWindow();
    SAT_Widget* overlay = window->getOverlayWidget();

    sa_demo_page_widgets* widgets_page = (sa_demo_page_widgets*)pages->getChild(1);
    overlay->appendChild(widgets_page->MMenu1);

    return true;
  }

