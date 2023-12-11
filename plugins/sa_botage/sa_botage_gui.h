// be careful!
// directly included into the middle of sa_botage.h
//----------------------------------------------------------------------

  const char* txt_eight[8] = { "1", "2", "3", "4", "5", "6", "7", "8" };
  
  SAT_RootWidget*               root_panel            = nullptr;
  SAT_MenuWidget*               fx_type_menu          = nullptr;
  sa_botage_header_widget*      header_widget         = nullptr;
  sa_botage_footer_widget*      footer_widget         = nullptr;
  sa_botage_main_panel_widget*  main_panel            = nullptr;
  sa_botage_group_widget*       buffer_panel          = nullptr;
  sa_botage_group_widget*       trigger_panel         = nullptr;
  sa_botage_group_widget*       env_panel             = nullptr;
  sa_botage_tabs_widget*        tabs_widget           = nullptr;
  
  sa_botage_buttonrow_widget*   buffer_beats_slider   = nullptr;
  sa_botage_buttonrow_widget*   buffer_slices_slider  = nullptr; 
  
  sa_botage_knob3_widget*       trigger_prob_knob     = nullptr;
  sa_botage_buttonrow_widget*   trigger_slices_slider = nullptr;
  sa_botage_buttonrow_widget*   trigger_loops_slider  = nullptr;

  sa_botage_slider_widget*      env_loop_att_slider   = nullptr;
  sa_botage_slider_widget*      env_loop_dec_slider   = nullptr;
  sa_botage_slider_widget*      slice_loop_att_slider = nullptr;
  sa_botage_slider_widget*      slice_loop_dec_slider = nullptr;

  sa_botage_prob_page_widget*   prob_page_widget      = nullptr;
  sa_botage_seq_page_widget*    seq_page_widget       = nullptr;
  sa_botage_perf_page_widget*   perf_page_widget      = nullptr;

  //----------

  void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    
    // root panel
    root_panel = new SAT_RootWidget(AWindow);
    AWindow->setRootWidget(root_panel);
    root_panel->setBackgroundColor(SAT_Black);
    
    // menu
    
    fx_type_menu                      = new SAT_MenuWidget(SAT_Rect(100,100));
    fx_type_menu->appendChildWidget(    new SAT_MenuItemWidget(10,"LowPass") );
    fx_type_menu->appendChildWidget(    new SAT_MenuItemWidget(10,"BandPass") );
    fx_type_menu->appendChildWidget(    new SAT_MenuItemWidget(10,"HighPass") );
    
    // main
    
    header_widget                     = new sa_botage_header_widget(      SAT_Rect( 0, 0,                 EDITOR_WIDTH, 60                  ) );
    footer_widget                     = new sa_botage_footer_widget(      SAT_Rect( 0, EDITOR_HEIGHT-20,  EDITOR_WIDTH, 20                  ) );
    main_panel                        = new sa_botage_main_panel_widget(  SAT_Rect( 0, 60,                EDITOR_WIDTH, EDITOR_HEIGHT-60-20 ) );
    
    root_panel->appendChildWidget(header_widget);
    root_panel->appendChildWidget(footer_widget);
    root_panel->appendChildWidget(main_panel);
    
    // main_panel
    
    buffer_panel                      = new sa_botage_group_widget( SAT_Rect(  10,  10, 350, 410 ), "Buffer" );
    trigger_panel                     = new sa_botage_group_widget( SAT_Rect( 370,  10, 200, 100 ), "Trigger" );
    env_panel                         = new sa_botage_group_widget( SAT_Rect( 580,  10, 100, 100 ), "Envelopes" );
    tabs_widget                       = new sa_botage_tabs_widget(  SAT_Rect( 370, 120, 460, 300 ),  3, 15);
    
    main_panel->appendChildWidget(buffer_panel);
    main_panel->appendChildWidget(trigger_panel);
    main_panel->appendChildWidget(env_panel);
    main_panel->appendChildWidget(tabs_widget);
    
      // buffer
      
      buffer_beats_slider             = new sa_botage_buttonrow_widget( SAT_Rect(  10, 25, 160, 10  ), 8, txt_eight,SAT_BUTTON_ROW_SINGLE,false );
      buffer_slices_slider            = new sa_botage_buttonrow_widget( SAT_Rect( 180, 25, 160, 10  ), 8, txt_eight,SAT_BUTTON_ROW_SINGLE,false );
    
      buffer_panel->appendChildWidget(  new sa_botage_text1_widget(     SAT_Rect(  10, 10, 160, 10  ), "Beats" ));
      buffer_panel->appendChildWidget(  new sa_botage_text1_widget(     SAT_Rect( 180, 10, 160, 10  ), "Slices" ));
      buffer_panel->appendChildWidget(  buffer_beats_slider );
      buffer_panel->appendChildWidget(  buffer_slices_slider );
      buffer_panel->appendChildWidget(  new sa_botage_waveform_widget(  SAT_Rect(  10, 50, 330, 330 )  ));
    
      // trigger
    
      trigger_prob_knob               = new sa_botage_knob3_widget(SAT_Rect(10,10,60,60), "%", 0.25);
      trigger_slices_slider = new sa_botage_buttonrow_widget( SAT_Rect(80, 25, 85, 10 ), 8, txt_eight,SAT_BUTTON_ROW_MULTI,false);
      trigger_loops_slider  = new sa_botage_buttonrow_widget( SAT_Rect(80, 55, 85, 10 ), 8, txt_eight,SAT_BUTTON_ROW_MULTI,false);
      
      trigger_panel->appendChildWidget( trigger_prob_knob );
      trigger_panel->appendChildWidget( new sa_botage_text1_widget(     SAT_Rect(80, 10, 85, 10 ),"Slices") );
      trigger_panel->appendChildWidget( trigger_slices_slider );
      trigger_panel->appendChildWidget( new sa_botage_text1_widget(     SAT_Rect(80, 40, 85, 10 ),"Loop") );
      trigger_panel->appendChildWidget( trigger_loops_slider );

      // env
    
      //env_panel->appendChildWidget( new SAT_TextWidget(      SAT_Rect(80, 10, 85, 10 ),"Beats") );
      env_loop_att_slider   = new sa_botage_slider_widget( SAT_Rect(10,10,80,10),"Loop Att",0);
      env_loop_dec_slider   = new sa_botage_slider_widget( SAT_Rect(10,25,80,10),"Loop Dec",0);
      slice_loop_att_slider = new sa_botage_slider_widget( SAT_Rect(10,40,80,10),"Slice Att",0);
      slice_loop_dec_slider = new sa_botage_slider_widget( SAT_Rect(10,55,80,10),"Slice Dec",0);
      env_panel->appendChildWidget( env_loop_att_slider );
      env_panel->appendChildWidget( env_loop_dec_slider );
      env_panel->appendChildWidget( slice_loop_att_slider );
      env_panel->appendChildWidget( slice_loop_dec_slider );

      // tabs

      prob_page_widget = new sa_botage_prob_page_widget(0,fx_type_menu);
      seq_page_widget  = new sa_botage_seq_page_widget(0);
      perf_page_widget = new sa_botage_perf_page_widget(0);

      tabs_widget->appendPage("Prob",prob_page_widget);
      tabs_widget->appendPage("Seq", seq_page_widget);
      tabs_widget->appendPage("Perf",perf_page_widget);
      tabs_widget->selectPage(0);

    // menu
    
    root_panel->appendChildWidget(fx_type_menu);
  }

//----------------------------------------------------------------------


