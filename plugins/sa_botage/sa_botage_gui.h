// be careful!
// directly included into the middle of sa_botage.h

//------------------------------
private:
//------------------------------

  SAT_RootWidget*               root_panel              = nullptr;
  SAT_MenuWidget*               fx_type_menu            = nullptr;
  sa_botage_header_widget*      header_widget           = nullptr;
  sa_botage_footer_widget*      footer_widget           = nullptr;
  sa_botage_main_panel_widget*  main_panel              = nullptr;
  sa_botage_group_widget*       buffer_panel            = nullptr;
  sa_botage_group_widget*       trigger_panel           = nullptr;
  sa_botage_group_widget*       env_panel               = nullptr;
  sa_botage_tabs_widget*        tabs_widget             = nullptr;
  
  //sa_botage_buttonrow_widget*   buffer_beats_slider     = nullptr;
  //sa_botage_buttonrow_widget*   buffer_slices_slider    = nullptr; 
  
  SAT_SliderWidget*             buffer_beats_slider     = nullptr;
  SAT_SliderWidget*             buffer_slices_slider    = nullptr; 
  
  sa_botage_knob3_widget*       trigger_prob_knob       = nullptr;
  sa_botage_buttongrid_widget*  trigger_slices_buttons  = nullptr;
  sa_botage_buttongrid_widget*  trigger_loops_buttons   = nullptr;

  sa_botage_slider_widget*      env_loop_att_slider     = nullptr;
  sa_botage_slider_widget*      env_loop_dec_slider     = nullptr;
  sa_botage_slider_widget*      slice_loop_att_slider   = nullptr;
  sa_botage_slider_widget*      slice_loop_dec_slider   = nullptr;

  sa_botage_prob_page_widget*   prob_page_widget        = nullptr;
  sa_botage_seq_page_widget*    seq_page_widget         = nullptr;
  sa_botage_perf_page_widget*   perf_page_widget        = nullptr;

  //

  sa_botage_waveform_widget*    waveform = nullptr;

  SAT_Color                   MReadPosColor   = SAT_Color( 0.0, 1.0, 0.0, 0.8 );
  SAT_Color                   MWritePosColor  = SAT_Color( 1.0, 0.0, 0.0, 1.0 );
  double                      MReadPosWidth   = 0.75;
  double                      MWritePosWidth  = 0.75;
  SAT_Color                   MRangeColor     = SAT_Color( 0.0, 0.4, 0.0, 0.5 );
  SAT_Color                   MLoopColor      = SAT_Color( 0.2, 0.6, 0.2, 0.7 );
  SAT_Color                   MSliceColor     = SAT_Color( 0.4, 0.4, 0.4, 0.5 );


//------------------------------
public:
//------------------------------

  //void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
  bool setupEditor(SAT_Editor* AEditor) final {
    SAT_Window* window = AEditor->getWindow();
    SAT_RootWidget* root_panel = new SAT_RootWidget(window);
    window->setRootWidget(root_panel);

    root_panel->setBackgroundColor(SAT_Black);
    
    // fx menu
    
    fx_type_menu = new SAT_MenuWidget(SAT_Rect(100,50));
    for (uint32_t i=0; i<NUM_FX_TYPES; i++) {
      fx_type_menu->appendChild( new SAT_MenuItemWidget(10,fx_type_text[i]) );
    }
    
    // main
    
    header_widget = new sa_botage_header_widget(     SAT_Rect( 0, 0,                EDITOR_WIDTH, 60                  ) );
    footer_widget = new sa_botage_footer_widget(     SAT_Rect( 0, EDITOR_HEIGHT-20, EDITOR_WIDTH, 20                  ) );
    main_panel    = new sa_botage_main_panel_widget( SAT_Rect( 0, 60,               EDITOR_WIDTH, EDITOR_HEIGHT-60-20 ) );
    
    root_panel->appendChild(header_widget);
    root_panel->appendChild(footer_widget);
    root_panel->appendChild(main_panel);
    
    // main_panel
    
    buffer_panel  = new sa_botage_group_widget( SAT_Rect(  10,  10, 350, 410 ), "Buffer" );
    trigger_panel = new sa_botage_group_widget( SAT_Rect( 370,  10, 200, 100 ), "Trigger" );
    env_panel     = new sa_botage_group_widget( SAT_Rect( 580,  10, 100, 100 ), "Envelopes" );
    tabs_widget   = new sa_botage_tabs_widget(  SAT_Rect( 370, 120, 460, 300 ),  15);
    
    main_panel->appendChild(buffer_panel);
    main_panel->appendChild(trigger_panel);
    main_panel->appendChild(env_panel);
    main_panel->appendChild(tabs_widget);
    
      // buffer
      
      // buffer_panel->appendChild(  new sa_botage_text1_widget(     SAT_Rect(  10, 10, 160, 10  ), "Beats" ));
      // buffer_panel->appendChild(  new sa_botage_text1_widget(     SAT_Rect( 180, 10, 160, 10  ), "Slices" ));
      // buffer_beats_slider  = new sa_botage_buttonrow_widget( SAT_Rect(  10, 25, 160, 10 ), 8, txt_eight,SAT_BUTTON_ROW_SINGLE,false );
      // buffer_slices_slider = new sa_botage_buttonrow_widget( SAT_Rect( 180, 25, 160, 10 ), 8, txt_eight,SAT_BUTTON_ROW_SINGLE,false );

      buffer_beats_slider  = new SAT_SliderWidget( SAT_Rect(  10, 10/*25*/, 160, 15 ), "Beats" );
      buffer_slices_slider = new SAT_SliderWidget( SAT_Rect( 180, 10/*25*/, 160, 15 ), "Slices" );

      buffer_beats_slider->setBackgroundColor(0.3);
      buffer_slices_slider->setBackgroundColor(0.3);

      buffer_panel->appendChild(  buffer_beats_slider );
      buffer_panel->appendChild(  buffer_slices_slider );

      waveform = new sa_botage_waveform_widget( SAT_Rect( 10, 50, 330, 330 ));
      buffer_panel->appendChild(waveform);
    
      // trigger
    
      trigger_prob_knob       = new sa_botage_knob3_widget(      SAT_Rect(10, 10, 60, 60 ), "%", 0.25);
      trigger_slices_buttons  = new sa_botage_buttongrid_widget( SAT_Rect(80, 25, 85, 10 ), 8, 1, txt_eight);
      trigger_loops_buttons   = new sa_botage_buttongrid_widget( SAT_Rect(80, 55, 85, 10 ), 8, 1, txt_eight);

      // trigger_slices_buttons->setValueIsBits(true,8);
      // trigger_slices_buttons->setAllowZeroBits(false);
      // trigger_loops_buttons->setValueIsBits(true,8);
      // trigger_loops_buttons->setAllowZeroBits(false);
      
      trigger_panel->appendChild( trigger_prob_knob );
      trigger_panel->appendChild( new sa_botage_text1_widget(     SAT_Rect(80, 10, 85, 10 ),"Slices") );
      trigger_panel->appendChild( trigger_slices_buttons );
      trigger_panel->appendChild( new sa_botage_text1_widget(     SAT_Rect(80, 40, 85, 10 ),"Loop") );
      trigger_panel->appendChild( trigger_loops_buttons );

      // env
    
      //env_panel->appendChild( new SAT_TextWidget(      SAT_Rect(80, 10, 85, 10 ),"Beats") );
      env_loop_att_slider   = new sa_botage_slider_widget( SAT_Rect(10,10,80,10),"Loop Att",0);
      env_loop_dec_slider   = new sa_botage_slider_widget( SAT_Rect(10,25,80,10),"Loop Dec",0);
      slice_loop_att_slider = new sa_botage_slider_widget( SAT_Rect(10,45,80,10),"Slice Att",0);
      slice_loop_dec_slider = new sa_botage_slider_widget( SAT_Rect(10,60,80,10),"Slice Dec",0);

      env_panel->appendChild( env_loop_att_slider );
      env_panel->appendChild( env_loop_dec_slider );
      env_panel->appendChild( slice_loop_att_slider );
      env_panel->appendChild( slice_loop_dec_slider );

      // tabs

      prob_page_widget = new sa_botage_prob_page_widget(0,fx_type_menu);
      seq_page_widget  = new sa_botage_seq_page_widget(0);
      perf_page_widget = new sa_botage_perf_page_widget(0);

      tabs_widget->appendPage("Probability",prob_page_widget);
      tabs_widget->appendPage("Sequence", seq_page_widget);
      tabs_widget->appendPage("Perform",perf_page_widget);
      tabs_widget->selectPage(0);

    // menu
    
    root_panel->appendChild(fx_type_menu);

    //----------

  //AEditor->connect( gain_knob,                              getParameter(SA_BOTAGE_PARAM_GAIN) );

    AEditor->connect( env_loop_att_slider,                    getParameter(SA_BOTAGE_PARAM_ENV_LOOP_ATTACK) );
    AEditor->connect( env_loop_dec_slider,                    getParameter(SA_BOTAGE_PARAM_ENV_LOOP_DECAY) );
    AEditor->connect( slice_loop_att_slider,                  getParameter(SA_BOTAGE_PARAM_ENV_SLICE_ATTACK) );
    AEditor->connect( slice_loop_dec_slider,                  getParameter(SA_BOTAGE_PARAM_ENV_SLICE_DECAY) );

    AEditor->connect( buffer_beats_slider,                    getParameter(SA_BOTAGE_PARAM_BUFFER_BEATS) );
    AEditor->connect( buffer_slices_slider,                   getParameter(SA_BOTAGE_PARAM_BUFFER_SLICES) );

    AEditor->connect( trigger_prob_knob,                      getParameter(SA_BOTAGE_PARAM_PROB_TRIGGER) );
    AEditor->connect( trigger_slices_buttons,                 getParameter(SA_BOTAGE_PARAM_PROB_TRIGGER_SLICES) );
    AEditor->connect( trigger_loops_buttons,                  getParameter(SA_BOTAGE_PARAM_PROB_TRIGGER_LOOP) );

    AEditor->connect( prob_page_widget->range_length_prob,    getParameter(SA_BOTAGE_PARAM_PROB_RANGE_LENGTH) );
    AEditor->connect( prob_page_widget->range_length_minmax,  getParameter(SA_BOTAGE_PARAM_PROB_RANGE_LENGTH_MIN), 0);
    AEditor->connect( prob_page_widget->range_length_minmax,  getParameter(SA_BOTAGE_PARAM_PROB_RANGE_LENGTH_MAX), 1);
    AEditor->connect( prob_page_widget->range_speed_prob,     getParameter(SA_BOTAGE_PARAM_PROB_RANGE_SPEED) );
    AEditor->connect( prob_page_widget->range_speed_minmax,   getParameter(SA_BOTAGE_PARAM_PROB_RANGE_SPEED_MIN), 0 );
    AEditor->connect( prob_page_widget->range_speed_minmax,   getParameter(SA_BOTAGE_PARAM_PROB_RANGE_SPEED_MAX), 1 );
    AEditor->connect( prob_page_widget->range_offset_prob,    getParameter(SA_BOTAGE_PARAM_PROB_RANGE_OFFSET) );
    AEditor->connect( prob_page_widget->range_offset_minmax,  getParameter(SA_BOTAGE_PARAM_PROB_RANGE_OFFSET_MIN), 0 );
    AEditor->connect( prob_page_widget->range_offset_minmax,  getParameter(SA_BOTAGE_PARAM_PROB_RANGE_OFFSET_MAX), 1 );
    AEditor->connect( prob_page_widget->range_reverse_prob,   getParameter(SA_BOTAGE_PARAM_PROB_RANGE_REVERSE) );
    AEditor->connect( prob_page_widget->range_fx_prob,        getParameter(SA_BOTAGE_PARAM_PROB_RANGE_FX) );
    AEditor->connect( prob_page_widget->range_fx_minmax,      getParameter(SA_BOTAGE_PARAM_PROB_RANGE_FX_MIN), 0 );
    AEditor->connect( prob_page_widget->range_fx_minmax,      getParameter(SA_BOTAGE_PARAM_PROB_RANGE_FX_MAX), 1 );

    AEditor->connect( prob_page_widget->loop_length_prob,    getParameter(SA_BOTAGE_PARAM_PROB_LOOP_LENGTH) );
    AEditor->connect( prob_page_widget->loop_length_minmax,  getParameter(SA_BOTAGE_PARAM_PROB_LOOP_LENGTH_MIN), 0);
    AEditor->connect( prob_page_widget->loop_length_minmax,  getParameter(SA_BOTAGE_PARAM_PROB_LOOP_LENGTH_MAX), 1);
    AEditor->connect( prob_page_widget->loop_speed_prob,     getParameter(SA_BOTAGE_PARAM_PROB_LOOP_SPEED) );
    AEditor->connect( prob_page_widget->loop_speed_minmax,   getParameter(SA_BOTAGE_PARAM_PROB_LOOP_SPEED_MIN), 0 );
    AEditor->connect( prob_page_widget->loop_speed_minmax,   getParameter(SA_BOTAGE_PARAM_PROB_LOOP_SPEED_MAX), 1 );
    AEditor->connect( prob_page_widget->loop_offset_prob,    getParameter(SA_BOTAGE_PARAM_PROB_LOOP_OFFSET) );
    AEditor->connect( prob_page_widget->loop_offset_minmax,  getParameter(SA_BOTAGE_PARAM_PROB_LOOP_OFFSET_MIN), 0 );
    AEditor->connect( prob_page_widget->loop_offset_minmax,  getParameter(SA_BOTAGE_PARAM_PROB_LOOP_OFFSET_MAX), 1 );
    AEditor->connect( prob_page_widget->loop_reverse_prob,   getParameter(SA_BOTAGE_PARAM_PROB_LOOP_REVERSE) );
    AEditor->connect( prob_page_widget->loop_fx_prob,        getParameter(SA_BOTAGE_PARAM_PROB_LOOP_FX) );
    AEditor->connect( prob_page_widget->loop_fx_minmax,      getParameter(SA_BOTAGE_PARAM_PROB_LOOP_FX_MIN), 0 );
    AEditor->connect( prob_page_widget->loop_fx_minmax,      getParameter(SA_BOTAGE_PARAM_PROB_LOOP_FX_MAX), 1 );
/*
    AEditor->connect( prob_page_widget->fx1_prob,            getParameter(SA_BOTAGE_PARAM_PROB_FX1 ));
    AEditor->connect( prob_page_widget->fx1_type,            getParameter(SA_BOTAGE_PARAM_PROB_FX1_TYPE ));
    AEditor->connect( prob_page_widget->fx1_arg1,            getParameter(SA_BOTAGE_PARAM_PROB_FX1_ARG1 ));
    AEditor->connect( prob_page_widget->fx1_arg2,            getParameter(SA_BOTAGE_PARAM_PROB_FX1_ARG2 ));
    AEditor->connect( prob_page_widget->fx1_arg3,            getParameter(SA_BOTAGE_PARAM_PROB_FX1_ARG3 ));
    AEditor->connect( prob_page_widget->fx2_prob,            getParameter(SA_BOTAGE_PARAM_PROB_FX2 ));
    AEditor->connect( prob_page_widget->fx2_type,            getParameter(SA_BOTAGE_PARAM_PROB_FX2_TYPE ));
    AEditor->connect( prob_page_widget->fx2_arg1,            getParameter(SA_BOTAGE_PARAM_PROB_FX2_ARG1 ));
    AEditor->connect( prob_page_widget->fx2_arg2,            getParameter(SA_BOTAGE_PARAM_PROB_FX2_ARG2 ));
    AEditor->connect( prob_page_widget->fx2_arg3,            getParameter(SA_BOTAGE_PARAM_PROB_FX2_ARG3 ));
    AEditor->connect( prob_page_widget->fx3_prob,            getParameter(SA_BOTAGE_PARAM_PROB_FX3 ));
    AEditor->connect( prob_page_widget->fx3_type,            getParameter(SA_BOTAGE_PARAM_PROB_FX3_TYPE ));
    AEditor->connect( prob_page_widget->fx3_arg1,            getParameter(SA_BOTAGE_PARAM_PROB_FX3_ARG1 ));
    AEditor->connect( prob_page_widget->fx3_arg2,            getParameter(SA_BOTAGE_PARAM_PROB_FX3_ARG2 ));
    AEditor->connect( prob_page_widget->fx3_arg3,            getParameter(SA_BOTAGE_PARAM_PROB_FX3_ARG3 ));
    AEditor->connect( prob_page_widget->fx4_prob,            getParameter(SA_BOTAGE_PARAM_PROB_FX4 ));
    AEditor->connect( prob_page_widget->fx4_type,            getParameter(SA_BOTAGE_PARAM_PROB_FX4_TYPE ));
    AEditor->connect( prob_page_widget->fx4_arg1,            getParameter(SA_BOTAGE_PARAM_PROB_FX4_ARG1 ));
    AEditor->connect( prob_page_widget->fx4_arg2,            getParameter(SA_BOTAGE_PARAM_PROB_FX4_ARG2 ));
    AEditor->connect( prob_page_widget->fx4_arg3,            getParameter(SA_BOTAGE_PARAM_PROB_FX4_ARG3 ));
    AEditor->connect( prob_page_widget->fx5_prob,            getParameter(SA_BOTAGE_PARAM_PROB_FX5 ));
    AEditor->connect( prob_page_widget->fx5_type,            getParameter(SA_BOTAGE_PARAM_PROB_FX5_TYPE ));
    AEditor->connect( prob_page_widget->fx5_arg1,            getParameter(SA_BOTAGE_PARAM_PROB_FX5_ARG1 ));
    AEditor->connect( prob_page_widget->fx5_arg2,            getParameter(SA_BOTAGE_PARAM_PROB_FX5_ARG2 ));
    AEditor->connect( prob_page_widget->fx5_arg3,            getParameter(SA_BOTAGE_PARAM_PROB_FX5_ARG3 ));
*/
    return true;
  }

  //----------

  bool setupOverlay(SAT_Editor* AEditor) final {
    return true;
  }

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

  void updateWaveformWidget(sa_botage_processor* processor) {

    uint32_t buflen = processor->MBufferLength;
    if (buflen > 0) {

      waveform->setAudioBuffer(buflen, processor->MLeftBuffer, false);
      waveform->setGrid(processor->par_num_beats, processor->par_num_slices);

      // marker 0 write pos

      waveform->setMarkerActive(0, true);
      waveform->setMarkerPos(   0, (double)processor->MWritePos/buflen, 0.0010);//0.0025);
      waveform->setMarkerColor( 0, MWritePosColor);
      //waveform->setMarkerWidth( 0, MWritePosWidth);

      // marker 1 read pos

      waveform->setMarkerActive(1, true);
      waveform->setMarkerPos(   1, processor->MReadPos/buflen, 0.0010);//0.0025);
      waveform->setMarkerColor( 1, MReadPosColor);
      //waveform->setMarkerWidth( 1,MReadPosWidth);

      if (processor->MRange) {

        // area 1 range

        waveform->setAreaActive(0, false);
        waveform->setAreaActive(1, true);
        waveform->setAreaPos(   1, processor->MRangeStart/buflen, processor->MRangeLength/buflen);
        waveform->setAreaColor( 1, MRangeColor);

        if (processor->MLoop) {

          // area 2 loop

          waveform->setAreaActive(2, true);
          waveform->setAreaPos(   2, processor->MLoopStart/buflen, processor->MLoopLength/buflen);
          waveform->setAreaColor( 2, MLoopColor);

        }
        else {
          waveform->setAreaActive(2, false);
        }
      }

      else { // no range
        waveform->setAreaActive(1, false);
        waveform->setAreaActive(2, false);

        // area 0 current slice

        if (processor->MIsPlaying) {
          waveform->setAreaActive(0, true);
          waveform->setAreaPos(   0, processor->MSliceStart/buflen, processor->MSliceLength/buflen);
          waveform->setAreaColor( 0, MSliceColor);
        }
        else {
          waveform->setAreaActive(0, false);
        }

      } // range

      waveform->do_Widget_redraw(waveform,0,0);

    } // buflen

  }

  //----------

  void updateProbIndicators(sa_botage_processor* processor) {

#if 0

    trigger_prob_knob->setMarkerValue(processor->rnd_main_prob);
    //if (!trigger_prob_knob->drawMarker()) {
      trigger_prob_knob->setDrawMarker(true);
      trigger_prob_knob->do_Widget_redraw(trigger_prob_knob,0,0);
    //}

    uint32_t current_page = tabs_widget->getActivePage();
    if (current_page == 0) {

      if (processor->MRange) {
        //SAT_Print("%.1f, %.1f\n",processor->rnd_main_slices,processor->rnd_main_subdiv);

        // MMainSlicesWidget->setIndicatorValue(processor->rnd_main_slices);
        trigger_slices_buttons->setDrawIndicator(true);
        trigger_slices_buttons->setIndicatorValue(processor->rnd_main_slices - 1);
        trigger_slices_buttons->do_Widget_redraw(trigger_slices_buttons,0,0);

        // MMainLoopsWidget->setIndicatorValue(processor->rnd_main_subdiv);
        trigger_loops_buttons->setDrawIndicator(true);
        trigger_loops_buttons->setIndicatorValue(processor->rnd_main_subdiv - 1);
        trigger_loops_buttons->do_Widget_redraw(trigger_loops_buttons,0,0);

        prob_page_widget->range_length_prob->setMarkerValue(processor->rnd_range_size);
        prob_page_widget->range_length_prob->setDrawMarker(true);
        prob_page_widget->range_length_prob->do_Widget_redraw(prob_page_widget->range_length_prob,0,0);

        prob_page_widget->range_speed_prob->setMarkerValue(processor->rnd_range_speed);
        prob_page_widget->range_speed_prob->setDrawMarker(true);
        prob_page_widget->range_speed_prob->do_Widget_redraw(prob_page_widget->range_speed_prob,0,0);

        prob_page_widget->range_offset_prob->setMarkerValue(processor->rnd_range_offset);
        prob_page_widget->range_offset_prob->setDrawMarker(true);
        prob_page_widget->range_offset_prob->do_Widget_redraw(prob_page_widget->range_offset_prob,0,0);

        prob_page_widget->range_reverse_prob->setMarkerValue(processor->rnd_range_reverse);
        prob_page_widget->range_reverse_prob->setDrawMarker(true);
        prob_page_widget->range_reverse_prob->do_Widget_redraw(prob_page_widget->range_reverse_prob,0,0);

        prob_page_widget->range_fx_prob->setMarkerValue(processor->rnd_range_fx);
        prob_page_widget->range_fx_prob->setDrawMarker(true);
        prob_page_widget->range_fx_prob->do_Widget_redraw(prob_page_widget->range_fx_prob,0,0);

        if (processor->rnd_range_size_on) {
          prob_page_widget->range_length_minmax->setDrawIndicator(true);
          prob_page_widget->range_length_minmax->setIndicatorValue(processor->rnd_range_size_value);
        }
        else prob_page_widget->range_length_minmax->setDrawIndicator(false);
        prob_page_widget->range_length_minmax->do_Widget_redraw(prob_page_widget->range_length_minmax,0,0);

        if (processor->rnd_range_speed_on) {
          prob_page_widget->range_speed_minmax->setDrawIndicator(true);
          prob_page_widget->range_speed_minmax->setIndicatorValue(processor->rnd_range_speed_value);
        }
        else prob_page_widget->range_speed_minmax->setDrawIndicator(false);
        prob_page_widget->range_speed_minmax->do_Widget_redraw(prob_page_widget->range_speed_minmax,0,0);

        if (processor->rnd_range_offset_on) {
          prob_page_widget->range_offset_minmax->setDrawIndicator(true);
          prob_page_widget->range_offset_minmax->setIndicatorValue(processor->rnd_range_offset_value);
        }
        else prob_page_widget->range_offset_minmax->setDrawIndicator(false);
        prob_page_widget->range_offset_minmax->do_Widget_redraw(prob_page_widget->range_offset_minmax,0,0);

        if (processor->rnd_range_fx_on) {
          prob_page_widget->range_fx_minmax->setDrawIndicator(true);
          prob_page_widget->range_fx_minmax->setIndicatorValue(processor->rnd_range_fx_value);
        }
        else prob_page_widget->range_fx_minmax->setDrawIndicator(false);
        prob_page_widget->range_fx_minmax->do_Widget_redraw(prob_page_widget->range_fx_minmax,0,0);

        // //------------------------------

        if (processor->rnd_range_fx_on) {
          // MFXProbKnob[0]->setIndicatorValue(processor->rnd_fx1);
          // MFXProbKnob[1]->setIndicatorValue(processor->rnd_fx2);
          // MFXProbKnob[2]->setIndicatorValue(processor->rnd_fx3);
          // MFXProbKnob[3]->setIndicatorValue(processor->rnd_fx4);
          // MFXProbKnob[4]->setIndicatorValue(processor->rnd_fx5);
        }
        else {
          // MFXProbKnob[0]->setIndicatorValue(-1);
          // MFXProbKnob[1]->setIndicatorValue(-1);
          // MFXProbKnob[2]->setIndicatorValue(-1);
          // MFXProbKnob[3]->setIndicatorValue(-1);
          // MFXProbKnob[4]->setIndicatorValue(-1);
        }

        // if (processor->rnd_fx1_on) MFXProbKnob[0]->setIndicatorValue(processor->rnd_fx1);
        // else MFXProbKnob[0]->setIndicatorValue(-1);
        // if (processor->rnd_fx2_on) MFXProbKnob[1]->setIndicatorValue(processor->rnd_fx2);
        // else MFXProbKnob[1]->setIndicatorValue(-1);
        // if (processor->rnd_fx3_on) MFXProbKnob[2]->setIndicatorValue(processor->rnd_fx3);
        // else MFXProbKnob[2]->setIndicatorValue(-1);
        // if (processor->rnd_fx4_on) MFXProbKnob[3]->setIndicatorValue(processor->rnd_fx4);
        // else MFXProbKnob[3]->setIndicatorValue(-1);
        // if (processor->rnd_fx5_on) MFXProbKnob[4]->setIndicatorValue(processor->rnd_fx5);
        // else MFXProbKnob[4]->setIndicatorValue(-1);

        // //------------------------------

        if (processor->MLoopWrapped) {

          prob_page_widget->loop_length_prob->setMarkerValue(processor->rnd_loop_size);
          prob_page_widget->loop_length_prob->setDrawMarker(true);
          prob_page_widget->loop_length_prob->do_Widget_redraw(prob_page_widget->loop_length_prob,0,0);

          prob_page_widget->loop_speed_prob->setMarkerValue(processor->rnd_loop_speed);
          prob_page_widget->loop_speed_prob->setDrawMarker(true);
          prob_page_widget->loop_speed_prob->do_Widget_redraw(prob_page_widget->loop_speed_prob,0,0);

          prob_page_widget->loop_offset_prob->setMarkerValue(processor->rnd_loop_offset);
          prob_page_widget->loop_offset_prob->setDrawMarker(true);
          prob_page_widget->loop_offset_prob->do_Widget_redraw(prob_page_widget->loop_offset_prob,0,0);

          prob_page_widget->loop_reverse_prob->setMarkerValue(processor->rnd_loop_reverse);
          prob_page_widget->loop_reverse_prob->setDrawMarker(true);
          prob_page_widget->loop_reverse_prob->do_Widget_redraw(prob_page_widget->loop_reverse_prob,0,0);

          prob_page_widget->loop_fx_prob->setMarkerValue(processor->rnd_loop_fx);
          prob_page_widget->loop_fx_prob->setDrawMarker(true);
          prob_page_widget->loop_fx_prob->do_Widget_redraw(prob_page_widget->loop_fx_prob,0,0);

          if (processor->rnd_loop_size_on) {
            prob_page_widget->loop_length_minmax->setDrawIndicator(true);
            prob_page_widget->loop_length_minmax->setIndicatorValue(processor->rnd_loop_size_value);
          }
          else prob_page_widget->loop_length_minmax->setDrawIndicator(false);
          prob_page_widget->loop_length_minmax->do_Widget_redraw(prob_page_widget->loop_length_minmax,0,0);

          if (processor->rnd_loop_speed_on) {
            prob_page_widget->loop_speed_minmax->setDrawIndicator(true);
            prob_page_widget->loop_speed_minmax->setIndicatorValue(processor->rnd_loop_speed_value);
          }
          else prob_page_widget->loop_speed_minmax->setDrawIndicator(false);
          prob_page_widget->loop_speed_minmax->do_Widget_redraw(prob_page_widget->loop_speed_minmax,0,0);

          if (processor->rnd_loop_offset_on) {
            prob_page_widget->loop_offset_minmax->setDrawIndicator(true);
            prob_page_widget->loop_offset_minmax->setIndicatorValue(processor->rnd_loop_offset_value);
          }
          else prob_page_widget->loop_offset_minmax->setDrawIndicator(false);
          prob_page_widget->loop_offset_minmax->do_Widget_redraw(prob_page_widget->loop_offset_minmax,0,0);

          if (processor->rnd_loop_fx_on) {
            prob_page_widget->loop_fx_minmax->setDrawIndicator(true);
            prob_page_widget->loop_fx_minmax->setIndicatorValue(processor->rnd_loop_fx_value);
          }
          else prob_page_widget->loop_fx_minmax->setDrawIndicator(false);
          prob_page_widget->loop_fx_minmax->do_Widget_redraw(prob_page_widget->loop_fx_minmax,0,0);

        }
        else {

          prob_page_widget->loop_length_prob->setDrawMarker(false);
          prob_page_widget->loop_length_prob->do_Widget_redraw(prob_page_widget->loop_length_prob,0,0);

          prob_page_widget->loop_speed_prob->setDrawMarker(false);
          prob_page_widget->loop_speed_prob->do_Widget_redraw(prob_page_widget->loop_speed_prob,0,0);

          prob_page_widget->loop_offset_prob->setDrawMarker(false);
          prob_page_widget->loop_offset_prob->do_Widget_redraw(prob_page_widget->loop_offset_prob,0,0);

          prob_page_widget->loop_reverse_prob->setDrawMarker(false);
          prob_page_widget->loop_reverse_prob->do_Widget_redraw(prob_page_widget->loop_reverse_prob,0,0);

          prob_page_widget->loop_fx_prob->setDrawMarker(false);
          prob_page_widget->loop_fx_prob->do_Widget_redraw(prob_page_widget->loop_fx_prob,0,0);

          prob_page_widget->loop_length_minmax->setDrawIndicator(false);
          prob_page_widget->loop_length_minmax->do_Widget_redraw(prob_page_widget->loop_length_minmax,0,0);

          prob_page_widget->loop_speed_minmax->setDrawIndicator(false);
          prob_page_widget->loop_speed_minmax->do_Widget_redraw(prob_page_widget->loop_speed_minmax,0,0);

          prob_page_widget->loop_offset_minmax->setDrawIndicator(false);
          prob_page_widget->loop_offset_minmax->do_Widget_redraw(prob_page_widget->loop_offset_minmax,0,0);

          prob_page_widget->loop_fx_minmax->setDrawIndicator(false);
          prob_page_widget->loop_fx_minmax->do_Widget_redraw(prob_page_widget->loop_fx_minmax,0,0);

        }

      } // MRange
      else { // ! range

        trigger_slices_buttons->setDrawIndicator(false);
        trigger_slices_buttons->do_Widget_redraw(trigger_slices_buttons,0,0);

        trigger_loops_buttons->setDrawIndicator(false);
        trigger_loops_buttons->do_Widget_redraw(trigger_loops_buttons,0,0);

        prob_page_widget->range_length_prob->setDrawMarker(false);
        prob_page_widget->range_length_prob->do_Widget_redraw(prob_page_widget->range_length_prob,0,0);

        prob_page_widget->range_speed_prob->setDrawMarker(false);
        prob_page_widget->range_speed_prob->do_Widget_redraw(prob_page_widget->range_speed_prob,0,0);

        prob_page_widget->range_offset_prob->setDrawMarker(false);
        prob_page_widget->range_offset_prob->do_Widget_redraw(prob_page_widget->range_offset_prob,0,0);

        prob_page_widget->range_reverse_prob->setDrawMarker(false);
        prob_page_widget->range_reverse_prob->do_Widget_redraw(prob_page_widget->range_reverse_prob,0,0);

        prob_page_widget->range_fx_prob->setDrawMarker(false);
        prob_page_widget->range_fx_prob->do_Widget_redraw(prob_page_widget->range_fx_prob,0,0);

        prob_page_widget->range_length_minmax->setDrawIndicator(false);
        prob_page_widget->range_length_minmax->do_Widget_redraw(prob_page_widget->range_length_minmax,0,0);

        prob_page_widget->range_speed_minmax->setDrawIndicator(false);
        prob_page_widget->range_speed_minmax->do_Widget_redraw(prob_page_widget->range_speed_minmax,0,0);

        prob_page_widget->range_offset_minmax->setDrawIndicator(false);
        prob_page_widget->range_offset_minmax->do_Widget_redraw(prob_page_widget->range_offset_minmax,0,0);

        prob_page_widget->range_fx_minmax->setDrawIndicator(false);
        prob_page_widget->range_fx_minmax->do_Widget_redraw(prob_page_widget->range_fx_minmax,0,0);

        prob_page_widget->loop_length_prob->setDrawMarker(false);
        prob_page_widget->loop_length_prob->do_Widget_redraw(prob_page_widget->loop_length_prob,0,0);

        prob_page_widget->loop_speed_prob->setDrawMarker(false);
        prob_page_widget->loop_speed_prob->do_Widget_redraw(prob_page_widget->loop_speed_prob,0,0);

        prob_page_widget->loop_offset_prob->setDrawMarker(false);
        prob_page_widget->loop_offset_prob->do_Widget_redraw(prob_page_widget->loop_offset_prob,0,0);

        prob_page_widget->loop_reverse_prob->setDrawMarker(false);
        prob_page_widget->loop_reverse_prob->do_Widget_redraw(prob_page_widget->loop_reverse_prob,0,0);

        prob_page_widget->loop_fx_prob->setDrawMarker(false);
        prob_page_widget->loop_fx_prob->do_Widget_redraw(prob_page_widget->loop_fx_prob,0,0);

        prob_page_widget->loop_length_minmax->setDrawIndicator(false);
        prob_page_widget->loop_length_minmax->do_Widget_redraw(prob_page_widget->loop_length_minmax,0,0);

        prob_page_widget->loop_speed_minmax->setDrawIndicator(false);
        prob_page_widget->loop_speed_minmax->do_Widget_redraw(prob_page_widget->loop_speed_minmax,0,0);

        prob_page_widget->loop_offset_minmax->setDrawIndicator(false);
        prob_page_widget->loop_offset_minmax->do_Widget_redraw(prob_page_widget->loop_offset_minmax,0,0);

        prob_page_widget->loop_fx_minmax->setDrawIndicator(false);
        prob_page_widget->loop_fx_minmax->do_Widget_redraw(prob_page_widget->loop_fx_minmax,0,0);

        // MFXProbKnob[0]->setIndicatorValue(-1);
        // MFXProbKnob[1]->setIndicatorValue(-1);
        // MFXProbKnob[2]->setIndicatorValue(-1);
        // MFXProbKnob[3]->setIndicatorValue(-1);
        // MFXProbKnob[4]->setIndicatorValue(-1);

      } // range

    } // page == 0

#endif // 0

  }


