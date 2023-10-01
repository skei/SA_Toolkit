
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
    // osc section
    //--------------------
    
    SAT_TextWidget* osc_section_text = new SAT_TextWidget(SAT_Rect(5,5,200,15),"osc 1");
    root->appendChildWidget(osc_section_text);
    osc_section_text->setDrawBorder(false);
    osc_section_text->setFillBackground(true);
    osc_section_text->setBackgroundColor(0.2);
    osc_section_text->setTextColor(0.7);
    osc_section_text->setTextSize(12);
    
    SAT_PanelWidget* osc_section = new SAT_PanelWidget(SAT_Rect(5,25,200,120));
    root->appendChildWidget(osc_section);
    //osc_section->setAlignment(SAT_WIDGET_ALIGN_LEFT_TOP);

      // osc type

      SAT_MenuWidget* osc_type_menu = new SAT_MenuWidget(SAT_Rect(75,60));
      osc_type_menu->setDrawBorder(false);
      osc_type_menu->setFillBackground(false);
      
      SAT_MenuItemWidget* osc_type_items[5];
      osc_type_items[0] = (SAT_MenuItemWidget*)osc_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"morph"));
      osc_type_items[1] = (SAT_MenuItemWidget*)osc_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"dsf"));
      osc_type_items[2] = (SAT_MenuItemWidget*)osc_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"padsynth"));
      osc_type_items[3] = (SAT_MenuItemWidget*)osc_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"wavetable"));
      osc_type_items[4] = (SAT_MenuItemWidget*)osc_type_menu->appendChildWidget( new SAT_MenuItemWidget(12,"input"));
      for (uint32_t i=0; i<5; i++) {
        osc_type_items[i]->setTextColor(0.8);
        osc_type_items[i]->setTextSize(7);
        osc_type_items[i]->setFillBackground(true);
        osc_type_items[i]->setBackgroundColor(0.2);
        osc_type_items[i]->setHoverBackgroundColor(0.3);
      }
      
      SAT_SelectorWidget* osc_type_sel = new SAT_SelectorWidget(SAT_Rect(5,5,70,15),"<select>",osc_type_menu);
      osc_section->appendChildWidget(osc_type_sel);
      //AEditor->connect(flt_sel,getParameter(6));
      osc_type_sel->setDrawBorder(false);
      osc_type_sel->setFillBackground(true);
      osc_type_sel->setBackgroundColor(0.33);
      osc_type_sel->setTextSize(8);
      osc_type_sel->setTextColor(0.8);
      
      // osc edit

      SAT_ButtonWidget* osc_edit_button = new SAT_ButtonWidget(SAT_Rect(80,5,30,15),false);
      osc_section->appendChildWidget(osc_edit_button);
      osc_edit_button->setTexts("edit","edit");
      
      // osc shape

      SAT_KnobWidget* osc_shape_knob = new SAT_KnobWidget(SAT_Rect(5,25,50,50),"Sh",0);
      osc_section->appendChildWidget(osc_shape_knob);
      //AEditor->connect(osc_shape_knob,getParameter(0));
      osc_shape_knob->setDrawBorder(false);
      osc_shape_knob->setTextSize(9);
      osc_shape_knob->setTextColor(0.6);
      osc_shape_knob->setValueSize(8);
      osc_shape_knob->setQuantize(true);
      osc_shape_knob->setQuantizeSteps(4);
      
      // osc width

      SAT_KnobWidget* osc_width_knob = new SAT_KnobWidget(SAT_Rect(60,25,50,50),"Wi",0.5);
      osc_section->appendChildWidget(osc_width_knob);
      //AEditor->connect(osc_width_knob,getParameter(0));
      osc_width_knob->setDrawBorder(false);
      osc_width_knob->setTextSize(9);
      osc_width_knob->setTextColor(0.6);
      osc_width_knob->setValueSize(8);
      osc_width_knob->setBipolar(true);
      osc_width_knob->setBipolarCenter(0.5);
      osc_width_knob->setSnap(true);
      osc_width_knob->setSnapPos(0.5);
      
      // osc env a

      SAT_SliderWidget* osc_a_slider = new SAT_SliderWidget(SAT_Rect(120,25,15,35),"a",0.0);
      osc_section->appendChildWidget(osc_a_slider);
      //AEditor->connect(osc_a_slider,getParameter(2));
      osc_a_slider->setDragDirection(SAT_DIRECTION_UP);
      osc_a_slider->setDrawDirection(SAT_DIRECTION_UP);
      osc_a_slider->setDrawValue(false);
      osc_a_slider->setDrawText(false);
      
      SAT_CurveWidget* osc_a_curve = new SAT_CurveWidget(SAT_Rect(120,5,15,15),false);
      osc_section->appendChildWidget(osc_a_curve);
      osc_a_curve->setDrawBorder(false);
      osc_a_curve->setFillBackground(true);
      osc_a_curve->setBackgroundColor(0.33);
      osc_a_curve->setCurveWidth(1.5);
      osc_a_curve->setCurveColor(0.7);
      osc_a_curve->setInteractColor(1.0);
        
      // osc env d
      
      SAT_SliderWidget* osc_d_slider = new SAT_SliderWidget(SAT_Rect(140,25,15,35),"d",0.0);
      osc_section->appendChildWidget(osc_d_slider);
      //AEditor->connect(osc_d_slider,getParameter(3));
      osc_d_slider->setDragDirection(SAT_DIRECTION_UP);
      osc_d_slider->setDrawDirection(SAT_DIRECTION_UP);
      osc_d_slider->setDrawValue(false);
      osc_d_slider->setDrawText(false);
      
      SAT_CurveWidget* osc_d_curve = new SAT_CurveWidget(SAT_Rect(140,5,15,15),true);
      osc_section->appendChildWidget(osc_d_curve);
      osc_d_curve->setDrawBorder(false);
      osc_d_curve->setFillBackground(true);
      osc_d_curve->setBackgroundColor(0.33);
      osc_d_curve->setCurveWidth(1.5);
      osc_d_curve->setCurveColor(0.7);
      osc_d_curve->setInteractColor(1.0);
        
      // osc env s
      
      SAT_SliderWidget* osc_s_slider = new SAT_SliderWidget(SAT_Rect(160,25,15,35),"s",1.0);
      osc_section->appendChildWidget(osc_s_slider);
      //AEditor->connect(osc_s_slider,getParameter(4));
      osc_s_slider->setDragDirection(SAT_DIRECTION_UP);
      osc_s_slider->setDrawDirection(SAT_DIRECTION_UP);
      osc_s_slider->setDrawValue(false);
      osc_s_slider->setDrawText(false);
      
      // osc env r
      
      SAT_SliderWidget* osc_r_slider = new SAT_SliderWidget(SAT_Rect(180,25,15,35),"r",0.0);
      osc_section->appendChildWidget(osc_r_slider);
      //AEditor->connect(osc_r_slider,getParameter(5));
      osc_r_slider->setDragDirection(SAT_DIRECTION_UP);
      osc_r_slider->setDrawDirection(SAT_DIRECTION_UP);
      osc_r_slider->setDrawValue(false);
      osc_r_slider->setDrawText(false);
      
      SAT_CurveWidget* osc_r_curve = new SAT_CurveWidget(SAT_Rect(180,5,15,15),true);
      osc_section->appendChildWidget(osc_r_curve);
      osc_r_curve->setDrawBorder(false);
      osc_r_curve->setFillBackground(true);
      osc_r_curve->setBackgroundColor(0.33);
      osc_r_curve->setCurveWidth(1.5);
      osc_r_curve->setCurveColor(0.7);
      osc_r_curve->setInteractColor(1.0);
        
      // osc env amt
      
      //SAT_KnobWidget* env_amt_knob = new SAT_KnobWidget(SAT_Rect(200,25,30,30),"EA",0);
      SAT_SliderWidget* osc_env_amt_slider = new SAT_SliderWidget(SAT_Rect(120,65,75,10),"Env Amt",0.5);
      osc_section->appendChildWidget(osc_env_amt_slider);
      osc_env_amt_slider->setFillBackground(true);
      osc_env_amt_slider->setBackgroundColor(0.33);
      osc_env_amt_slider->setDrawBorder(false);
      osc_env_amt_slider->setTextSize(6);
      osc_env_amt_slider->setTextColor(0.6);
      osc_env_amt_slider->setValueSize(7);
      osc_env_amt_slider->setBipolar(true);
      osc_env_amt_slider->setBipolarCenter(0.5);
      osc_env_amt_slider->setSnap(true);
      osc_env_amt_slider->setSnapPos(0.5);
      //osc_env_amt_slider->setSnapDist(0.1);
      //osc_env_amt_slider->setSnapSpeed(1.5);
      
      // osc pd amount

      SAT_SliderWidget* osc_pd_amt_slider = new SAT_SliderWidget(SAT_Rect(5,80,55,15),"PD Amt",0.5);
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
      
      SAT_SelectorWidget* osc_pd_type_sel = new SAT_SelectorWidget(SAT_Rect(65,80,70,15),"<select>",osc_pd_type_menu);
      osc_section->appendChildWidget(osc_pd_type_sel);
      //AEditor->connect(flt_sel,getParameter(6));
      osc_pd_type_sel->setDrawBorder(false);
      osc_pd_type_sel->setFillBackground(true);
      osc_pd_type_sel->setBackgroundColor(0.33);
      osc_pd_type_sel->setTextSize(8);
      osc_pd_type_sel->setTextColor(0.8);
      
      // osc ws amount

      SAT_SliderWidget* osc_ws_amt_slider = new SAT_SliderWidget(SAT_Rect(5,100,55,15),"WS Amt",0.5);
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
      
      SAT_SelectorWidget* osc_ws_type_sel = new SAT_SelectorWidget(SAT_Rect(65,100,70,15),"<select>",osc_ws_type_menu);
      osc_section->appendChildWidget(osc_ws_type_sel);
      //AEditor->connect(flt_sel,getParameter(6));
      osc_ws_type_sel->setDrawBorder(false);
      osc_ws_type_sel->setFillBackground(true);
      osc_ws_type_sel->setBackgroundColor(0.33);
      osc_ws_type_sel->setTextSize(8);
      osc_ws_type_sel->setTextColor(0.8);
      
      // osc tuning
      
//      SAT_KnobWidget* tuning_knob = new SAT_KnobWidget(SAT_Rect(5,80,30,30),"Tu",0);
//      osc_section->appendChildWidget(tuning_knob);
//      AEditor->connect(tuning_knob,getParameter(1));
//      tuning_knob->setArcThickness(4);
//      tuning_knob->setDrawBorder(false);
//      tuning_knob->setTextSize(6);
//      tuning_knob->setTextColor(0.6);
//      tuning_knob->setValueSize(7);
//      
//      tuning_knob->setBipolar(true);
//      tuning_knob->setBipolarCenter(0.5);
//      tuning_knob->setSnap(true);
//      tuning_knob->setSnapPos(0.5);
//      //tuning_knob->setSnapDist(0.1);
//      //tuning_knob->setSnapSpeed(1.5);

      SAT_SliderWidget* tuning_oct_slider = new SAT_SliderWidget(SAT_Rect(140,80,55,10),"Oct",0.5);
      osc_section->appendChildWidget(tuning_oct_slider);
      //AEditor->connect(tuning_oct_slider,getParameter(1));
      tuning_oct_slider->setFillBackground(true);
      tuning_oct_slider->setBackgroundColor(0.33);
      tuning_oct_slider->setDrawBorder(false);
      tuning_oct_slider->setTextSize(7);
      tuning_oct_slider->setValueSize(7);
      tuning_oct_slider->setBipolar(true);
      tuning_oct_slider->setBipolarCenter(0.5);
      tuning_oct_slider->setSnap(true);
      tuning_oct_slider->setSnapPos(0.5);
      
      SAT_SliderWidget* tuning_semi_slider = new SAT_SliderWidget(SAT_Rect(140,92,55,10),"Semi",0.5);
      osc_section->appendChildWidget(tuning_semi_slider);
      //AEditor->connect(tuning_semi_slider,getParameter(1));
      tuning_semi_slider->setFillBackground(true);
      tuning_semi_slider->setBackgroundColor(0.33);
      tuning_semi_slider->setDrawBorder(false);
      tuning_semi_slider->setTextSize(7);
      tuning_semi_slider->setValueSize(7);
      tuning_semi_slider->setBipolar(true);
      tuning_semi_slider->setBipolarCenter(0.5);
      tuning_semi_slider->setSnap(true);
      tuning_semi_slider->setSnapPos(0.5);
      
      SAT_SliderWidget* tuning_cent_slider = new SAT_SliderWidget(SAT_Rect(140,104,55,10),"Cent",0.5);
      osc_section->appendChildWidget(tuning_cent_slider);
      //AEditor->connect(tuning_cent_slider,getParameter(1));
      tuning_cent_slider->setFillBackground(true);
      tuning_cent_slider->setBackgroundColor(0.33);
      tuning_cent_slider->setDrawBorder(false);
      tuning_cent_slider->setTextSize(7);
      tuning_cent_slider->setValueSize(7);
      tuning_cent_slider->setBipolar(true);
      tuning_cent_slider->setBipolarCenter(0.5);
      tuning_cent_slider->setSnap(true);
      tuning_cent_slider->setSnapPos(0.5);

    //--------------------
    // flt section
    //--------------------

    SAT_TextWidget* flt_section_text = new SAT_TextWidget(SAT_Rect(210,5,105,12),"flt 1");
    root->appendChildWidget(flt_section_text);
    flt_section_text->setDrawBorder(false);
    flt_section_text->setFillBackground(true);
    flt_section_text->setBackgroundColor(0.2);
    flt_section_text->setTextColor(0.7);
    flt_section_text->setTextSize(8);
    
    SAT_PanelWidget* flt_section = new SAT_PanelWidget(SAT_Rect(210,20,105,85));
    root->appendChildWidget(flt_section);
    //res_section->setAlignment(SAT_WIDGET_ALIGN_LEFT_TOP);
    
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
      
      SAT_SelectorWidget* flt_type_sel = new SAT_SelectorWidget(SAT_Rect(5,5,95,12),"<select>",flt_type_menu);
      flt_section->appendChildWidget(flt_type_sel);
      //AEditor->connect(flt_type_sel,getParameter(6));

      // flt freq
      
      SAT_KnobWidget* flt_freq_knob = new SAT_KnobWidget(SAT_Rect(5,25,30,30),"frq",1);
      flt_section->appendChildWidget(flt_freq_knob);
      //AEditor->connect(flt_freq_knob,getParameter(7));
      flt_freq_knob->setArcThickness(4);
      flt_freq_knob->setDrawBorder(false);
      flt_freq_knob->setTextSize(10);
      flt_freq_knob->setTextColor(0.6);
      flt_freq_knob->setValueSize(8);
      
      // flt bw
      
      SAT_KnobWidget* flt_bw_knob = new SAT_KnobWidget(SAT_Rect(40,25,30,30),"bw",1);
      flt_section->appendChildWidget(flt_bw_knob);
      // AEditor->connect(flt_bw_knob,getParameter(8));
      flt_bw_knob->setArcThickness(4);
      flt_bw_knob->setDrawBorder(false);
      flt_bw_knob->setTextSize(6);
      flt_bw_knob->setTextColor(0.6);
      flt_bw_knob->setValueSize(7);
      
      // flt env amt
      
      SAT_KnobWidget* flt_env_amt_knob = new SAT_KnobWidget(SAT_Rect(80,35,20,20),"a",0);
      flt_section->appendChildWidget(flt_env_amt_knob);
      //AEditor->connect(flt_env_amt_knob,getParameter(13)); // p_fltamt
      flt_env_amt_knob->setArcThickness(2);
      flt_env_amt_knob->setDrawBorder(false);
      flt_env_amt_knob->setTextSize(6);
      flt_env_amt_knob->setTextColor(0.6);
      flt_env_amt_knob->setValueSize(6);
      
      flt_env_amt_knob->setBipolar(true);
      flt_env_amt_knob->setBipolarCenter(0.5);
      flt_env_amt_knob->setSnap(true);
      flt_env_amt_knob->setSnapPos(0.5);
      //flt_env_amt_knob->setSnapDist(0.1);
      //flt_env_amt_knob->setSnapSpeed(1.5);
      
      // flt adsr

      SAT_KnobWidget* flt_a_knob = new SAT_KnobWidget(SAT_Rect(5,60,20,20),"a",0);
      flt_section->appendChildWidget(flt_a_knob);
      //AEditor->connect(flt_a_knob,getParameter(9));
      flt_a_knob->setArcThickness(2);
      flt_a_knob->setDrawBorder(false);
      flt_a_knob->setTextSize(6);
      flt_a_knob->setTextColor(0.6);
      flt_a_knob->setValueSize(5);

      SAT_KnobWidget* flt_d_knob = new SAT_KnobWidget(SAT_Rect(30,60,20,20),"d",0);
      flt_section->appendChildWidget(flt_d_knob);
      //AEditor->connect(flt_d_knob,getParameter(10));
      flt_d_knob->setArcThickness(2);
      flt_d_knob->setDrawBorder(false);
      flt_d_knob->setTextSize(6);
      flt_d_knob->setTextColor(0.6);
      flt_d_knob->setValueSize(5);

      SAT_KnobWidget* flt_s_knob = new SAT_KnobWidget(SAT_Rect(55,60,20,20),"s",0);
      flt_section->appendChildWidget(flt_s_knob);
      //AEditor->connect(flt_s_knob,getParameter(11));
      flt_s_knob->setArcThickness(2);
      flt_s_knob->setDrawBorder(false);
      flt_s_knob->setTextSize(6);
      flt_s_knob->setTextColor(0.6);
      flt_s_knob->setValueSize(5);

      SAT_KnobWidget* flt_r_knob = new SAT_KnobWidget(SAT_Rect(80,60,20,20),"r",0);
      flt_section->appendChildWidget(flt_r_knob);
      //AEditor->connect(flt_r_knob,getParameter(12));
      flt_r_knob->setArcThickness(2);
      flt_r_knob->setDrawBorder(false);
      flt_r_knob->setTextSize(6);
      flt_r_knob->setTextColor(0.6);
      flt_r_knob->setValueSize(5);
    
    //--------------------
    // global
    //--------------------

    // voices
    
    MVoicesWidget = new SAT_VoicesWidget(SAT_Rect(210,110,105,5), NUM_VOICES);
    root->appendChildWidget(MVoicesWidget);
    
    // gain
    
    SAT_KnobWidget* gain_knob = new SAT_KnobWidget(SAT_Rect(210,120,25,25),"G",0.5);
    root->appendChildWidget(gain_knob);
    //AEditor->connect(gain_knob,getParameter(14));
    gain_knob->setArcThickness(4);
    gain_knob->setDrawBorder(false);
    gain_knob->setTextSize(6);
    gain_knob->setTextColor(0.6);
    gain_knob->setValueSize(6);
    
    //--------------------

    root->appendChildWidget(osc_type_menu);
    root->appendChildWidget(flt_type_menu);
    root->appendChildWidget(osc_pd_type_menu);
    root->appendChildWidget(osc_ws_type_menu);

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
    
