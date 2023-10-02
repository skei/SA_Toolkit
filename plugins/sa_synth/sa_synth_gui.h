
// PS! included directly inside sa_synth.h
// don't #include this manually..

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------
  
  bool initEditorWindow(SAT_Editor* AEditor/*, SAT_Window* AWindow*/) final {
    SAT_Window* window = AEditor->getWindow();
    
    //SAT_KnobWidget* knob;
    
    // root

    SAT_PanelWidget* root = new SAT_PanelWidget( SAT_Rect(0,0,EDITOR_WIDTH,EDITOR_HEIGHT) );
    window->appendRootWidget(root);
    root->setFillBackground(true);
    root->setDrawBorder(false);
    //root->setInnerBorder(SAT_Rect(5,5,5,5));
    
    //--------------------
    // main
    //--------------------
    
    SAT_PanelWidget* main_panel = new SAT_PanelWidget( SAT_Rect(0,0,EDITOR_WIDTH,EDITOR_HEIGHT) );
    root->appendChildWidget(main_panel);
    main_panel->setFillBackground(true);
    main_panel->setDrawBorder(false);
    //main_panel->setInnerBorder(SAT_Rect(5,5,5,5));

    //--------------------
    // osc section
    //--------------------
    
    SAT_PanelWidget* osc_section = new SAT_PanelWidget(SAT_Rect(5,5,275,70));
    main_panel->appendChildWidget(osc_section);
    //osc_section->setAlignment(SAT_WIDGET_ALIGN_LEFT_TOP);

      SAT_TextWidget* osc_section_text = new SAT_TextWidget(SAT_Rect(0,0,300,10),"osc 1");
      osc_section->appendChildWidget(osc_section_text);
      osc_section_text->setDrawBorder(false);
      osc_section_text->setFillBackground(true);
      osc_section_text->setBackgroundColor(0.2);
      osc_section_text->setTextColor(0.7);
      osc_section_text->setTextSize(8);
    
      //-----
      
      // osc oct
      
      SAT_SliderWidget* osc_oct_slider = new SAT_SliderWidget(SAT_Rect(5,15,60,10),"Oct",0.5);
      osc_section->appendChildWidget(osc_oct_slider);
      //AEditor->connect(osc_oct_slider,getParameter(1));
      osc_oct_slider->setFillBackground(true);
      osc_oct_slider->setBackgroundColor(0.33);
      osc_oct_slider->setDrawBorder(false);
      osc_oct_slider->setTextSize(7);
      osc_oct_slider->setValueSize(7);
      osc_oct_slider->setBipolar(true);
      osc_oct_slider->setBipolarCenter(0.5);
      osc_oct_slider->setSnap(true);
      osc_oct_slider->setSnapPos(0.5);
      
      // osc semi

      SAT_SliderWidget* osc_semi_slider = new SAT_SliderWidget(SAT_Rect(5,30,60,10),"Semi",0.5);
      osc_section->appendChildWidget(osc_semi_slider);
      //AEditor->connect(osc_semi_slider,getParameter(1));
      osc_semi_slider->setFillBackground(true);
      osc_semi_slider->setBackgroundColor(0.33);
      osc_semi_slider->setDrawBorder(false);
      osc_semi_slider->setTextSize(7);
      osc_semi_slider->setValueSize(7);
      osc_semi_slider->setBipolar(true);
      osc_semi_slider->setBipolarCenter(0.5);
      osc_semi_slider->setSnap(true);
      osc_semi_slider->setSnapPos(0.5);
      
      // osc cent

      SAT_SliderWidget* osc_cent_slider = new SAT_SliderWidget(SAT_Rect(5,45,60,10),"Cent",0.5);
      osc_section->appendChildWidget(osc_cent_slider);
      //AEditor->connect(osc_cent_slider,getParameter(1));
      osc_cent_slider->setFillBackground(true);
      osc_cent_slider->setBackgroundColor(0.33);
      osc_cent_slider->setDrawBorder(false);
      osc_cent_slider->setTextSize(7);
      osc_cent_slider->setValueSize(7);
      osc_cent_slider->setBipolar(true);
      osc_cent_slider->setBipolarCenter(0.5);
      osc_cent_slider->setSnap(true);
      osc_cent_slider->setSnapPos(0.5);
      
      //-----
      
      // osc pd type

      SAT_MenuWidget* osc_pd_type_menu = new SAT_MenuWidget(SAT_Rect(75,60));
      osc_pd_type_menu->setDrawBorder(false);
      osc_pd_type_menu->setFillBackground(false);
      
      SAT_MenuItemWidget* osc_pd_type_items[5];
      osc_pd_type_items[0] = (SAT_MenuItemWidget*)osc_pd_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"none"));
      osc_pd_type_items[1] = (SAT_MenuItemWidget*)osc_pd_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"PD"));
      osc_pd_type_items[2] = (SAT_MenuItemWidget*)osc_pd_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"Sync"));
      osc_pd_type_items[3] = (SAT_MenuItemWidget*)osc_pd_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"CZ"));
      osc_pd_type_items[4] = (SAT_MenuItemWidget*)osc_pd_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"something.."));
      for (uint32_t i=0; i<5; i++) {
        osc_pd_type_items[i]->setTextColor(0.8);
        osc_pd_type_items[i]->setTextSize(7);
        osc_pd_type_items[i]->setFillBackground(true);
        osc_pd_type_items[i]->setBackgroundColor(0.2);
        osc_pd_type_items[i]->setHoverBackgroundColor(0.3);
      }
      
      SAT_SelectorWidget* osc_pd_type_sel = new SAT_SelectorWidget(SAT_Rect(70,15,60,10),"<phase>",osc_pd_type_menu);
      osc_section->appendChildWidget(osc_pd_type_sel);
      //AEditor->connect(flt_sel,getParameter(6));
      osc_pd_type_sel->setDrawBorder(false);
      osc_pd_type_sel->setFillBackground(true);
      osc_pd_type_sel->setBackgroundColor(0.33);
      osc_pd_type_sel->setTextSize(7);
      osc_pd_type_sel->setTextColor(0.8);

      // osc pd inputs
      
      SAT_KnobWidget* osc_pd_in1_knob = new SAT_KnobWidget(SAT_Rect(70,30,14,14),"o1",0.0);
      osc_section->appendChildWidget(osc_pd_in1_knob);
      osc_pd_in1_knob->setArcThickness(3.0);
      osc_pd_in1_knob->setDrawText(false);
      osc_pd_in1_knob->setDrawValue(false);
      
      SAT_KnobWidget* osc_pd_in2_knob = new SAT_KnobWidget(SAT_Rect(85,30,14,14),"o2",0.0);
      osc_section->appendChildWidget(osc_pd_in2_knob);
      osc_pd_in2_knob->setArcThickness(3.0);
      osc_pd_in2_knob->setDrawText(false);
      osc_pd_in2_knob->setDrawValue(false);
      
      SAT_KnobWidget* osc_pd_in3_knob = new SAT_KnobWidget(SAT_Rect(100,30,14,14),"r1",0.0);
      osc_section->appendChildWidget(osc_pd_in3_knob);
      osc_pd_in3_knob->setArcThickness(3.0);
      osc_pd_in3_knob->setDrawText(false);
      osc_pd_in3_knob->setDrawValue(false);
      
      SAT_KnobWidget* osc_pd_in4_knob = new SAT_KnobWidget(SAT_Rect(115,30,14,14),"r2",0.0);
      osc_section->appendChildWidget(osc_pd_in4_knob);
      osc_pd_in4_knob->setArcThickness(3.0);
      osc_pd_in4_knob->setDrawText(false);
      osc_pd_in4_knob->setDrawValue(false);
      
      // osc pd amount

      SAT_SliderWidget* osc_pd_amt_slider = new SAT_SliderWidget(SAT_Rect(70,50,60,10),"PD Amt",0.5);
      osc_section->appendChildWidget(osc_pd_amt_slider);
      osc_pd_amt_slider->setFillBackground(true);
      osc_pd_amt_slider->setBackgroundColor(0.33);
      osc_pd_amt_slider->setDrawBorder(false);
      osc_pd_amt_slider->setTextSize(6);
      osc_pd_amt_slider->setTextColor(0.6);
      osc_pd_amt_slider->setValueSize(7);
      osc_pd_amt_slider->setBipolar(true);
      osc_pd_amt_slider->setBipolarCenter(0.5);
      osc_pd_amt_slider->setSnap(true);
      osc_pd_amt_slider->setSnapPos(0.5);
      
      //-----
      
      // osc waveform

      SAT_MenuWidget* osc_waveform_menu = new SAT_MenuWidget(SAT_Rect(75,60));
      osc_waveform_menu->setDrawBorder(false);
      osc_waveform_menu->setFillBackground(false);
      
      SAT_MenuItemWidget* osc_type_items[5];
      osc_type_items[0] = (SAT_MenuItemWidget*)osc_waveform_menu->appendChildWidget( new SAT_MenuItemWidget(12,"morph"));
      osc_type_items[1] = (SAT_MenuItemWidget*)osc_waveform_menu->appendChildWidget( new SAT_MenuItemWidget(12,"dsf"));
      osc_type_items[2] = (SAT_MenuItemWidget*)osc_waveform_menu->appendChildWidget( new SAT_MenuItemWidget(12,"padsynth"));
      osc_type_items[3] = (SAT_MenuItemWidget*)osc_waveform_menu->appendChildWidget( new SAT_MenuItemWidget(12,"wavetable"));
      osc_type_items[4] = (SAT_MenuItemWidget*)osc_waveform_menu->appendChildWidget( new SAT_MenuItemWidget(12,"input"));
      for (uint32_t i=0; i<5; i++) {
        osc_type_items[i]->setTextColor(0.8);
        osc_type_items[i]->setTextSize(7);
        osc_type_items[i]->setFillBackground(true);
        osc_type_items[i]->setBackgroundColor(0.2);
        osc_type_items[i]->setHoverBackgroundColor(0.3);
      }
      
      SAT_SelectorWidget* osc_type_sel = new SAT_SelectorWidget(SAT_Rect(135,15,45,10),"waveform",osc_waveform_menu);
      osc_section->appendChildWidget(osc_type_sel);
      //AEditor->connect(flt_sel,getParameter(6));
      osc_type_sel->setDrawBorder(false);
      osc_type_sel->setFillBackground(true);
      osc_type_sel->setBackgroundColor(0.33);
      osc_type_sel->setTextSize(8);
      osc_type_sel->setTextColor(0.8);
      
      // osc edit btn

      SAT_ButtonWidget* osc_edit_button = new SAT_ButtonWidget(SAT_Rect(185,15,10,10),false);
      osc_section->appendChildWidget(osc_edit_button);
      osc_edit_button->setTexts("e","e");
      
      // osc shape

      SAT_KnobWidget* osc_shape_knob = new SAT_KnobWidget(SAT_Rect(135,30,25,25),"S",0);
      osc_section->appendChildWidget(osc_shape_knob);
      //AEditor->connect(osc_shape_knob,getParameter(0));
      osc_shape_knob->setArcThickness(3.0);
      osc_shape_knob->setDrawBorder(false);
      osc_shape_knob->setTextSize(6);
      osc_shape_knob->setTextColor(0.6);
      osc_shape_knob->setValueSize(6);
      //osc_shape_knob->setQuantize(true);
      //osc_shape_knob->setQuantizeSteps(4);
      
      // osc width

      SAT_KnobWidget* osc_width_knob = new SAT_KnobWidget(SAT_Rect(165,30,25,25),"W",0.5);
      osc_section->appendChildWidget(osc_width_knob);
      //AEditor->connect(osc_width_knob,getParameter(0));
      osc_width_knob->setArcThickness(3.0);
      osc_width_knob->setDrawBorder(false);
      osc_width_knob->setTextSize(6);
      osc_width_knob->setTextColor(0.6);
      osc_width_knob->setValueSize(6);
      osc_width_knob->setBipolar(true);
      osc_width_knob->setBipolarCenter(0.5);
      osc_width_knob->setSnap(true);
      osc_width_knob->setSnapPos(0.5);
      
      //-----
      
      // osc ws type

      SAT_MenuWidget* osc_ws_type_menu = new SAT_MenuWidget(SAT_Rect(75,60));
      osc_ws_type_menu->setDrawBorder(false);
      osc_ws_type_menu->setFillBackground(false);
      
      SAT_MenuItemWidget* osc_ws_type_items[5];
      osc_ws_type_items[0] = (SAT_MenuItemWidget*)osc_ws_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"none"));
      osc_ws_type_items[1] = (SAT_MenuItemWidget*)osc_ws_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"saturate"));
      osc_ws_type_items[2] = (SAT_MenuItemWidget*)osc_ws_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"clip"));
      osc_ws_type_items[3] = (SAT_MenuItemWidget*)osc_ws_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"distort"));
      osc_ws_type_items[4] = (SAT_MenuItemWidget*)osc_ws_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"filter"));
      for (uint32_t i=0; i<5; i++) {
        osc_ws_type_items[i]->setTextColor(0.8);
        osc_ws_type_items[i]->setTextSize(7);
        osc_ws_type_items[i]->setFillBackground(true);
        osc_ws_type_items[i]->setBackgroundColor(0.2);
        osc_ws_type_items[i]->setHoverBackgroundColor(0.3);
      }
      
      SAT_SelectorWidget* osc_ws_type_sel = new SAT_SelectorWidget(SAT_Rect(200,15,60,10),"<wave>",osc_ws_type_menu);
      osc_section->appendChildWidget(osc_ws_type_sel);
      //AEditor->connect(flt_sel,getParameter(6));
      osc_ws_type_sel->setDrawBorder(false);
      osc_ws_type_sel->setFillBackground(true);
      osc_ws_type_sel->setBackgroundColor(0.33);
      osc_ws_type_sel->setTextSize(8);
      osc_ws_type_sel->setTextColor(0.8);
      
      // osc ws inputs

      // osc pd inputs
      
      SAT_KnobWidget* osc_ws_in1_knob = new SAT_KnobWidget(SAT_Rect(200,30,14,14),"o1",0.0);
      osc_section->appendChildWidget(osc_ws_in1_knob);
      osc_ws_in1_knob->setArcThickness(3.0);
      osc_ws_in1_knob->setDrawText(false);
      osc_ws_in1_knob->setDrawValue(false);
      
      SAT_KnobWidget* osc_ws_in2_knob = new SAT_KnobWidget(SAT_Rect(215,30,14,14),"o2",0.0);
      osc_section->appendChildWidget(osc_ws_in2_knob);
      osc_ws_in2_knob->setArcThickness(3.0);
      osc_ws_in2_knob->setDrawText(false);
      osc_ws_in2_knob->setDrawValue(false);
      
      SAT_KnobWidget* osc_ws_in3_knob = new SAT_KnobWidget(SAT_Rect(230,30,14,14),"r1",0.0);
      osc_section->appendChildWidget(osc_ws_in3_knob);
      osc_ws_in3_knob->setArcThickness(3.0);
      osc_ws_in3_knob->setDrawText(false);
      osc_ws_in3_knob->setDrawValue(false);
      
      SAT_KnobWidget* osc_ws_in4_knob = new SAT_KnobWidget(SAT_Rect(245,30,14,14),"r2",0.0);
      osc_section->appendChildWidget(osc_ws_in4_knob);
      osc_ws_in4_knob->setArcThickness(3.0);
      osc_ws_in4_knob->setDrawText(false);
      osc_ws_in4_knob->setDrawValue(false);

      // osc ws amount

      SAT_SliderWidget* osc_ws_amt_slider = new SAT_SliderWidget(SAT_Rect(200,50,60,10),"WS Amt",0.5);
      osc_section->appendChildWidget(osc_ws_amt_slider);
      osc_ws_amt_slider->setFillBackground(true);
      osc_ws_amt_slider->setBackgroundColor(0.33);
      osc_ws_amt_slider->setDrawBorder(false);
      osc_ws_amt_slider->setTextSize(6);
      osc_ws_amt_slider->setTextColor(0.6);
      osc_ws_amt_slider->setValueSize(7);
      osc_ws_amt_slider->setBipolar(true);
      osc_ws_amt_slider->setBipolarCenter(0.5);
      osc_ws_amt_slider->setSnap(true);
      osc_ws_amt_slider->setSnapPos(0.5);

    //--------------------
    // flt section
    //--------------------

    SAT_PanelWidget* flt_section = new SAT_PanelWidget(SAT_Rect(5,145+100,105,70));
    main_panel->appendChildWidget(flt_section);
    //res_section->setAlignment(SAT_WIDGET_ALIGN_LEFT_TOP);
    
      SAT_TextWidget* flt_section_text = new SAT_TextWidget(SAT_Rect(0,0,105,10),"flt 1");
      flt_section->appendChildWidget(flt_section_text);
      flt_section_text->setDrawBorder(false);
      flt_section_text->setFillBackground(true);
      flt_section_text->setBackgroundColor(0.2);
      flt_section_text->setTextColor(0.7);
      flt_section_text->setTextSize(8);
    
      // flt type
      
      SAT_MenuWidget* flt_type_menu = new SAT_MenuWidget(SAT_Rect(75,60));
      flt_type_menu->setDrawBorder(false);
      flt_type_menu->setFillBackground(false);

      SAT_MenuItemWidget* flt_type_items[5];
      flt_type_items[0] = (SAT_MenuItemWidget*)flt_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"off"));
      flt_type_items[1] = (SAT_MenuItemWidget*)flt_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"lowpass"));
      flt_type_items[2] = (SAT_MenuItemWidget*)flt_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"highpass"));
      flt_type_items[3] = (SAT_MenuItemWidget*)flt_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"bandpass"));
      flt_type_items[4] = (SAT_MenuItemWidget*)flt_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"notch"));
      for (uint32_t i=0; i<5; i++) {
        flt_type_items[i]->setTextColor(0.8);
        flt_type_items[i]->setTextSize(7);
        flt_type_items[i]->setFillBackground(true);
        flt_type_items[i]->setBackgroundColor(0.2);
        flt_type_items[i]->setHoverBackgroundColor(0.3);
      }
      
      SAT_SelectorWidget* flt_type_sel = new SAT_SelectorWidget(SAT_Rect(5,15,95,12),"<filter>",flt_type_menu);
      flt_section->appendChildWidget(flt_type_sel);
      //AEditor->connect(flt_type_sel,getParameter(6));

      // flt freq
      
      SAT_KnobWidget* flt_freq_knob = new SAT_KnobWidget(SAT_Rect(5,35,30,30),"frq",1);
      flt_section->appendChildWidget(flt_freq_knob);
      //AEditor->connect(flt_freq_knob,getParameter(7));
      flt_freq_knob->setArcThickness(4);
      flt_freq_knob->setDrawBorder(false);
      flt_freq_knob->setTextSize(6);
      flt_freq_knob->setTextColor(0.6);
      flt_freq_knob->setValueSize(7);
      
      // flt bw
      
      SAT_KnobWidget* flt_bw_knob = new SAT_KnobWidget(SAT_Rect(40,35,30,30),"bw",1);
      flt_section->appendChildWidget(flt_bw_knob);
      // AEditor->connect(flt_bw_knob,getParameter(8));
      flt_bw_knob->setArcThickness(4);
      flt_bw_knob->setDrawBorder(false);
      flt_bw_knob->setTextSize(6);
      flt_bw_knob->setTextColor(0.6);
      flt_bw_knob->setValueSize(7);
   
    //--------------------
    // voice section
    //--------------------
    
    SAT_PanelWidget* voice_section = new SAT_PanelWidget(SAT_Rect(196+100,5,180,75));
    main_panel->appendChildWidget(voice_section);
    //voice_section->setAlignment(SAT_WIDGET_ALIGN_LEFT_TOP);

      SAT_TextWidget* voice_section_text = new SAT_TextWidget(SAT_Rect(0,0,180,10),"voice");
      voice_section->appendChildWidget(voice_section_text);
      voice_section_text->setDrawBorder(false);
      voice_section_text->setFillBackground(true);
      voice_section_text->setBackgroundColor(0.2);
      voice_section_text->setTextColor(0.7);
      voice_section_text->setTextSize(8);
    
      // env a
      
      SAT_CurveWidget* env_a_curve = new SAT_CurveWidget(SAT_Rect(5,15,15,15),false);
      voice_section->appendChildWidget(env_a_curve);
      env_a_curve->setDrawBorder(false);
      env_a_curve->setFillBackground(true);
      env_a_curve->setBackgroundColor(0.33);
      env_a_curve->setCurveWidth(1.5);
      env_a_curve->setCurveColor(0.7);
      env_a_curve->setInteractColor(1.0);
        
      SAT_SliderWidget* env_a_slider = new SAT_SliderWidget(SAT_Rect(5,35,15,35),"a",0.0);
      voice_section->appendChildWidget(env_a_slider);
      //AEditor->connect(env_a_slider,getParameter(2));
      env_a_slider->setDragDirection(SAT_DIRECTION_UP);
      env_a_slider->setDrawDirection(SAT_DIRECTION_UP);
      env_a_slider->setDrawValue(false);
      env_a_slider->setDrawText(false);
      
      // env d
      
      SAT_CurveWidget* env_d_curve = new SAT_CurveWidget(SAT_Rect(25,15,15,15),true);
      voice_section->appendChildWidget(env_d_curve);
      env_d_curve->setDrawBorder(false);
      env_d_curve->setFillBackground(true);
      env_d_curve->setBackgroundColor(0.33);
      env_d_curve->setCurveWidth(1.5);
      env_d_curve->setCurveColor(0.7);
      env_d_curve->setInteractColor(1.0);
        
      SAT_SliderWidget* env_d_slider = new SAT_SliderWidget(SAT_Rect(25,35,15,35),"d",0.0);
      voice_section->appendChildWidget(env_d_slider);
      //AEditor->connect(env_d_slider,getParameter(3));
      env_d_slider->setDragDirection(SAT_DIRECTION_UP);
      env_d_slider->setDrawDirection(SAT_DIRECTION_UP);
      env_d_slider->setDrawValue(false);
      env_d_slider->setDrawText(false);
      
      // env s
      
      SAT_SliderWidget* env_s_slider = new SAT_SliderWidget(SAT_Rect(45,35,15,35),"s",1.0);
      voice_section->appendChildWidget(env_s_slider);
      //AEditor->connect(env_s_slider,getParameter(4));
      env_s_slider->setDragDirection(SAT_DIRECTION_UP);
      env_s_slider->setDrawDirection(SAT_DIRECTION_UP);
      env_s_slider->setDrawValue(false);
      env_s_slider->setDrawText(false);
      
      // env r
      
      SAT_CurveWidget* env_r_curve = new SAT_CurveWidget(SAT_Rect(65,15,15,15),true);
      voice_section->appendChildWidget(env_r_curve);
      env_r_curve->setDrawBorder(false);
      env_r_curve->setFillBackground(true);
      env_r_curve->setBackgroundColor(0.33);
      env_r_curve->setCurveWidth(1.5);
      env_r_curve->setCurveColor(0.7);
      env_r_curve->setInteractColor(1.0);
        
      SAT_SliderWidget* env_r_slider = new SAT_SliderWidget(SAT_Rect(65,35,15,35),"r",0.0);
      voice_section->appendChildWidget(env_r_slider);
      //AEditor->connect(env_r_slider,getParameter(5));
      env_r_slider->setDragDirection(SAT_DIRECTION_UP);
      env_r_slider->setDrawDirection(SAT_DIRECTION_UP);
      env_r_slider->setDrawValue(false);
      env_r_slider->setDrawText(false);
      
      // voices
      
      MVoicesWidget = new SAT_VoicesWidget(SAT_Rect(100,15,75,5), NUM_VOICES);
      voice_section->appendChildWidget(MVoicesWidget);
      
      // gain
      
      SAT_KnobWidget* gain_knob = new SAT_KnobWidget(SAT_Rect(100,25,25,25),"G",0.5);
      voice_section->appendChildWidget(gain_knob);
      //AEditor->connect(gain_knob,getParameter(14));
      gain_knob->setArcThickness(4);
      gain_knob->setDrawBorder(false);
      gain_knob->setTextSize(6);
      gain_knob->setTextColor(0.6);
      gain_knob->setValueSize(6);
    
    // menus

    main_panel->appendChildWidget(osc_waveform_menu);
    main_panel->appendChildWidget(flt_type_menu);
    main_panel->appendChildWidget(osc_pd_type_menu);
    main_panel->appendChildWidget(osc_ws_type_menu);

    return true;
  }
  
//------------------------------

//    if (MIsEditorOpen) { // && not resizing
//      uint32_t playing = 0;
//      uint32_t released = 0;
//      for (uint32_t v=0; v<SA_TYR_NUM_VOICES; v++) {
//        uint32_t state = voices->getVoiceState(v);
//        MVoiceWidget->voice_state[v] = state;
//        if (state == SAT_VOICE_PLAYING) playing += 1;
//        if (state == SAT_VOICE_RELEASED) released += 1;
//      }
//      MPlayingVoicesWidget->setValue(playing);
//      MReleasedVoicesWidget->setValue(released);
//      MTotalVoicesWidget->setValue(playing + released);
//      MVoiceWidget->redraw();
//      MPlayingVoicesWidget->redraw();
//      MReleasedVoicesWidget->redraw();
//      MTotalVoicesWidget->redraw();
//    }

  void update_voices_widget(sa_synth_voice_manager* AVoiceManager) {
    
    //SAT_DPrint("playing %i released %i max %i\n",AVoiceManager->getNumPlayingVoices(),AVoiceManager->getNumReleasedVoices(),NUM_VOICES);
    //TODO: update gui text widgets..
    
    for (uint32_t voice=0; voice<NUM_VOICES; voice++) {
      uint32_t state = AVoiceManager->getVoiceState(voice);
      MVoicesWidget->setVoiceState(voice,state);
    }
    MVoicesWidget->parentRedraw();
  }
    
