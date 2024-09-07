
// included directly into sa_demo.h

//------------------------------
private:
//------------------------------

  SAT_MenuWidget*           file_menu = nullptr;
  SAT_MenuWidget*           edit_menu = nullptr;
  SAT_PagesWidget*          pages     = nullptr;
  SAT_AnimTestWidget*       animated  = nullptr;

  #define button_grid_count 3
  const char* button_grid_texts[button_grid_count] = {
    "Start",
    "Widgets",
    "Parameters"
  };

//------------------------------
private:
//------------------------------

  // temp menu listener (for context menus)

  class sa_demo_menu_listener
  : public SAT_MenuListener {
  public:
    void on_menuListener_select(SAT_MenuWidget* AMenu, int32_t AIndex) override {
      if (AMenu) AMenu->closeMenu();
    }
  };

  //----------

  sa_demo_menu_listener MMenuListener;

//------------------------------
public:
//------------------------------

  //----------

  bool setupEditor(SAT_Editor* AEditor) final {
    SAT_Window* window = AEditor->getWindow();

    // root

    sa_demo_root_widget* root = new sa_demo_root_widget(window);
    window->setRootWidget(root);
    root->setFillBackground(false);
    root->setDrawBorder(false);
    //root->setBorderColor(0.4);

    //------------------------------

    // menus

    file_menu = new SAT_MenuWidget(SAT_Rect(100,100),&MMenuListener);
    edit_menu = new SAT_MenuWidget(SAT_Rect(100,100),&MMenuListener);
    pages     = new SAT_PagesWidget(0);

    file_menu->appendItem( "New" );
    file_menu->appendItem( "Open" );
    file_menu->appendItem( "Close" );
    file_menu->appendItem( "Save" );
    file_menu->appendItem( "..and more.." );

    edit_menu->appendItem( "Cut" );
    edit_menu->appendItem( "Copy" );
    edit_menu->appendItem( "Paste" );
    edit_menu->appendItem( "fourth" );
    edit_menu->appendItem( "and a fifth one.." );


    // void on_menuListener_select(SAT_MenuWidget* AMenu, int32_t AIndex) override {
    //   if (AIndex >= 0) {
    //     //select(AIndex);
    //     do_Widget_update(this);
    //   }
    //   if (MMenu) MMenu->closeMenu();
    // }

    //------------------------------
    //
    //------------------------------

    // header

    SAT_PluginHeaderWidget* header = new SAT_PluginHeaderWidget(40,"demo");
    root->appendChild(header);

    header->setContextMenu(edit_menu);

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

    footer->setContextMenu(file_menu);

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
    left_panel->Layout.spacing = {0,20};

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

    // buttons
    
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

    // 9patch

    // SAT_9PatchImageWidget* patch9 = new SAT_9PatchImageWidget(100,(void*)SA_png,SA_png_size);
    // left_panel->appendChild(patch9);
    // patch9->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
    // patch9->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    // patch9->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    // patch9->setEdgeOffset(SAT_Rect(30,30,30,30));
    // patch9->setfillCenter(true);
    // patch9->setStretchCenter(true);
    // patch9->setStretchEdges(true);

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

    //pages->setDrawBorder(false);

    pages->selectPage(0);
    pages->realignChildren(); // needed? or automatic when changing page?

    
    //------------------------------
    //
    //------------------------------

    animated = new SAT_AnimTestWidget(SAT_Rect(10,370,130,130));
    root->appendChild(animated);

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
    SAT_Assert(window);

    SAT_Widget* overlay = window->getOverlayWidget();
    SAT_Assert(overlay);

    sa_demo_page_widgets* widgets_page = (sa_demo_page_widgets*)pages->getChild(1); // 1 ?????
    SAT_Assert(widgets_page);
    
    overlay->appendChild(widgets_page->MMenu1);

    // animated = new SAT_AnimTestWidget(SAT_Rect(10,370,130,130));
    // overlay->appendChild(animated);

    return true;
  }

