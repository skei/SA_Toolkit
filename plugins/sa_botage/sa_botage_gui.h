// be careful!
// directly included into the middle of sa_botage.h
//----------------------------------------------------------------------

  const char* txt_eight[8] = { "1", "2", "3", "4", "5", "6", "7", "8" };

  bool initEditorWindow(SAT_Editor* AEditor) final {
    SAT_Window* window = AEditor->getWindow();
    
    // root panel
    
    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
    window->appendRootWidget(panel);
    panel->setBackgroundColor(SAT_Black);
    
    // menu
    
    SAT_MenuWidget* fx_type_menu = new SAT_MenuWidget(SAT_Rect(100,100));
    
    fx_type_menu->appendChildWidget( new SAT_MenuItemWidget(10,"LowPass") );
    fx_type_menu->appendChildWidget( new SAT_MenuItemWidget(10,"BandPass") );
    fx_type_menu->appendChildWidget( new SAT_MenuItemWidget(10,"HighPass") );
    
    // main
    
    sa_botage_header_widget*      header        = new sa_botage_header_widget(      SAT_Rect( 0, 0,                 EDITOR_WIDTH, 60                  ) );
    sa_botage_footer_widget*      footer        = new sa_botage_footer_widget(      SAT_Rect( 0, EDITOR_HEIGHT-20,  EDITOR_WIDTH, 20                  ) );
    sa_botage_main_panel_widget*  main_panel    = new sa_botage_main_panel_widget(  SAT_Rect( 0, 60,                EDITOR_WIDTH, EDITOR_HEIGHT-60-20 ) );
    
    panel->appendChildWidget(header);
    panel->appendChildWidget(footer);
    panel->appendChildWidget(main_panel);
    
    // main_panel
    
    sa_botage_group_widget* buffer_panel  = new sa_botage_group_widget( SAT_Rect(  10,  10, 350, 410 ), "Buffer" );
    sa_botage_group_widget* trigger_panel = new sa_botage_group_widget( SAT_Rect( 370,  10, 200, 100 ), "Trigger" );
    sa_botage_group_widget* env_panel     = new sa_botage_group_widget( SAT_Rect( 580,  10, 100, 100 ), "Envelopes" );
    sa_botage_tabs_widget*  tabs          = new sa_botage_tabs_widget(  SAT_Rect( 370, 120, 460, 300 ),  3, 15);
    
    main_panel->appendChildWidget(buffer_panel);
    main_panel->appendChildWidget(trigger_panel);
    main_panel->appendChildWidget(env_panel);
    main_panel->appendChildWidget(tabs);
    
      // buffer
    
      buffer_panel->appendChildWidget( new sa_botage_text1_widget(     SAT_Rect(  10, 10, 160, 10  ), "Beats" ));
      buffer_panel->appendChildWidget( new sa_botage_text1_widget(     SAT_Rect( 180, 10, 160, 10  ), "Slices" ));
      buffer_panel->appendChildWidget( new sa_botage_buttonrow_widget( SAT_Rect(  10, 25, 160, 10  ), 8, txt_eight,SAT_BUTTON_ROW_SINGLE,false ));
      buffer_panel->appendChildWidget( new sa_botage_buttonrow_widget( SAT_Rect( 180, 25, 160, 10  ), 8, txt_eight,SAT_BUTTON_ROW_SINGLE,false ));
      buffer_panel->appendChildWidget( new sa_botage_waveform_widget(  SAT_Rect(  10, 50, 330, 330 )  ));
    
      // trigger
    
      sa_botage_knob3_widget* trigger_prob = new sa_botage_knob3_widget(SAT_Rect(10,10,60,60), "%", 0.25);
      trigger_panel->appendChildWidget(trigger_prob);
      trigger_panel->appendChildWidget( new sa_botage_text1_widget(     SAT_Rect(80, 10, 85, 10 ),"Slices") );
      trigger_panel->appendChildWidget( new sa_botage_buttonrow_widget( SAT_Rect(80, 25, 85, 10 ), 8, txt_eight,SAT_BUTTON_ROW_MULTI,false) );
      trigger_panel->appendChildWidget( new sa_botage_text1_widget(     SAT_Rect(80, 40, 85, 10 ),"Loop") );
      trigger_panel->appendChildWidget( new sa_botage_buttonrow_widget( SAT_Rect(80, 55, 85, 10 ), 8, txt_eight,SAT_BUTTON_ROW_MULTI,false) );

      // env
    
      //env_panel->appendChildWidget( new SAT_TextWidget(      SAT_Rect(80, 10, 85, 10 ),"Beats") );
      sa_botage_slider_widget* slider1 = new sa_botage_slider_widget( SAT_Rect(10,10,80,10),"Loop Att",0);
      sa_botage_slider_widget* slider2 = new sa_botage_slider_widget( SAT_Rect(10,25,80,10),"Loop Dec",0);
      sa_botage_slider_widget* slider3 = new sa_botage_slider_widget( SAT_Rect(10,40,80,10),"Slice Att",0);
      sa_botage_slider_widget* slider4 = new sa_botage_slider_widget( SAT_Rect(10,55,80,10),"Slice Dec",0);
      env_panel->appendChildWidget( slider1);
      env_panel->appendChildWidget( slider2);
      env_panel->appendChildWidget( slider3);
      env_panel->appendChildWidget( slider4);

      // tabs

      sa_botage_prob_page_widget* prob_page = new sa_botage_prob_page_widget(0,fx_type_menu);
      sa_botage_seq_page_widget*  seq_page  = new sa_botage_seq_page_widget(0);
      sa_botage_perf_page_widget* perf_page = new sa_botage_perf_page_widget(0);

      tabs->appendPage("Prob",prob_page);
      tabs->appendPage("Seq", seq_page);
      tabs->appendPage("Perf",perf_page);
      tabs->selectPage(0);

    // menu
    
    panel->appendChildWidget(fx_type_menu);
    return true;
  }

//----------------------------------------------------------------------


