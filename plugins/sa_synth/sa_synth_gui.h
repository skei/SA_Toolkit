
// PS! included directly inside sa_synth.h
// don't #include this manually..

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------
  
//  const char* buttontext[5] = { "1", "2", "3", "IV", "five" };

const char* sa_synth_osc_type_text[5] = {
  "Sin",
  "Saw",
  "Squ",
  "Tri",
  "Noise"
};

const char* sa_synth_flt_type_text[5] = {
  "off",
  "LP",
  "HP",
  "BP",
  "N"
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------
  
  //SAT_Editor* createEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight) final {
  //  //SAT_PRINT;
  //  SAT_Editor* editor = new SAT_Editor(AListener,AWidth,AHeight);
  //  return editor;
  //}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------
  
  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    
    SAT_KnobWidget* knob;
    
    // root

    SAT_PanelWidget* root = new SAT_PanelWidget( SAT_Rect(0,0,EDITOR_WIDTH,EDITOR_HEIGHT) );
    AWindow->appendRootWidget(root);
    root->setFillBackground(true);
    root->setDrawBorder(false);
    //root->setInnerBorder(SAT_Rect(5,5,5,5));
    
    // osc section
    
    SAT_TextWidget* osc_section_text = new SAT_TextWidget(SAT_Rect(5,5,105,12),"osc 1");
    root->appendChildWidget(osc_section_text);
    osc_section_text->setDrawBorder(false);
    osc_section_text->setFillBackground(true);
    osc_section_text->setBackgroundColor(0.2);
    osc_section_text->setTextColor(0.7);
    osc_section_text->setTextSize(8);
    
    SAT_PanelWidget* osc_section = new SAT_PanelWidget(SAT_Rect(5,20,105,85));
    root->appendChildWidget(osc_section);
    //osc_section->setAlignment(SAT_WIDGET_ALIGN_LEFT_TOP);
    
    // osc
    
    knob = new SAT_KnobWidget(SAT_Rect(5,5,50,50),"shp",0);
    osc_section->appendChildWidget(knob);
    AEditor->connect(knob,getParameter(0));
    knob->setDrawBorder(false);
    knob->setTextSize(10);
    knob->setTextColor(0.6);
    knob->setValueSize(8);
    
    // tuning
    
    knob = new SAT_KnobWidget(SAT_Rect(70,25,30,30),"tun",0);
    osc_section->appendChildWidget(knob);
    AEditor->connect(knob,getParameter(1));
    knob->setArcThickness(4);
    knob->setDrawBorder(false);
    knob->setTextSize(6);
    knob->setTextColor(0.6);
    knob->setValueSize(7);
    
    knob->setSnap(true);
    knob->setSnapPos(0.5);
    //knob->setSnapDist(0.1);
    //knob->setSnapSpeed(1.5);
    
    // adsr

    knob = new SAT_KnobWidget(SAT_Rect(5,60,20,20),"a",0);
    osc_section->appendChildWidget(knob);
    AEditor->connect(knob,getParameter(2));
    knob->setArcThickness(2);
    knob->setDrawBorder(false);
    knob->setTextSize(6);
    knob->setTextColor(0.6);
    knob->setValueSize(5);

    knob = new SAT_KnobWidget(SAT_Rect(30,60,20,20),"d",0);
    osc_section->appendChildWidget(knob);
    AEditor->connect(knob,getParameter(3));
    knob->setArcThickness(2);
    knob->setDrawBorder(false);
    knob->setTextSize(6);
    knob->setTextColor(0.6);
    knob->setValueSize(5);

    knob = new SAT_KnobWidget(SAT_Rect(55,60,20,20),"s",0);
    osc_section->appendChildWidget(knob);
    AEditor->connect(knob,getParameter(4));
    knob->setArcThickness(2);
    knob->setDrawBorder(false);
    knob->setTextSize(6);
    knob->setTextColor(0.6);
    knob->setValueSize(5);

    knob = new SAT_KnobWidget(SAT_Rect(80,60,20,20),"r",0);
    osc_section->appendChildWidget(knob);
    AEditor->connect(knob,getParameter(5));
    knob->setArcThickness(2);
    knob->setDrawBorder(false);
    knob->setTextSize(6);
    knob->setTextColor(0.6);
    knob->setValueSize(5);

    // flt section
    
    SAT_TextWidget* res_section_text = new SAT_TextWidget(SAT_Rect(115,5,105,12),"flt 1");
    root->appendChildWidget(res_section_text);
    res_section_text->setDrawBorder(false);
    res_section_text->setFillBackground(true);
    res_section_text->setBackgroundColor(0.2);
    res_section_text->setTextColor(0.7);
    res_section_text->setTextSize(8);
    
    SAT_PanelWidget* res_section = new SAT_PanelWidget(SAT_Rect(115,20,105,85));
    root->appendChildWidget(res_section);
    //res_section->setAlignment(SAT_WIDGET_ALIGN_LEFT_TOP);
    
    // flt
    
    SAT_MenuWidget* flt_menu = new SAT_MenuWidget(SAT_Rect(75,50));
    SAT_MenuItemWidget* item1 = (SAT_MenuItemWidget*)flt_menu->appendChildWidget( new SAT_MenuItemWidget(10,"off"));
    SAT_MenuItemWidget* item2 = (SAT_MenuItemWidget*)flt_menu->appendChildWidget( new SAT_MenuItemWidget(10,"lowpass"));
    SAT_MenuItemWidget* item3 = (SAT_MenuItemWidget*)flt_menu->appendChildWidget( new SAT_MenuItemWidget(10,"highpass"));
    SAT_MenuItemWidget* item4 = (SAT_MenuItemWidget*)flt_menu->appendChildWidget( new SAT_MenuItemWidget(10,"bandpass"));
    SAT_MenuItemWidget* item5 = (SAT_MenuItemWidget*)flt_menu->appendChildWidget( new SAT_MenuItemWidget(10,"notch"));
    item1->setTextSize(7);
    item2->setTextSize(7);
    item3->setTextSize(7);
    item4->setTextSize(7);
    item5->setTextSize(7);
    
    SAT_SelectorWidget* flt_sel = new SAT_SelectorWidget(SAT_Rect(5,5,95,12),"<select>",flt_menu);
    res_section->appendChildWidget(flt_sel);
    AEditor->connect(flt_sel,getParameter(6));

    // freq
    
    knob = new SAT_KnobWidget(SAT_Rect(5,25,30,30),"frq",1);
    res_section->appendChildWidget(knob);
    AEditor->connect(knob,getParameter(7));
    knob->setArcThickness(4);
    knob->setDrawBorder(false);
    knob->setTextSize(10);
    knob->setTextColor(0.6);
    knob->setValueSize(8);
    
    // bw
    
    knob = new SAT_KnobWidget(SAT_Rect(40,25,30,30),"bw",1);
    res_section->appendChildWidget(knob);
    AEditor->connect(knob,getParameter(8));
    knob->setArcThickness(4);
    knob->setDrawBorder(false);
    knob->setTextSize(6);
    knob->setTextColor(0.6);
    knob->setValueSize(7);
    
    // flt env amt
    
    knob = new SAT_KnobWidget(SAT_Rect(80,35,20,20),"a",0);
    res_section->appendChildWidget(knob);
    AEditor->connect(knob,getParameter(13)); // p_fltamt
    knob->setArcThickness(2);
    knob->setDrawBorder(false);
    knob->setTextSize(6);
    knob->setTextColor(0.6);
    knob->setValueSize(6);
    
    knob->setSnap(true);
    knob->setSnapPos(0.5);
    //knob->setSnapDist(0.1);
    //knob->setSnapSpeed(1.5);
    
    // adsr

    knob = new SAT_KnobWidget(SAT_Rect(5,60,20,20),"a",0);
    res_section->appendChildWidget(knob);
    AEditor->connect(knob,getParameter(9));
    knob->setArcThickness(2);
    knob->setDrawBorder(false);
    knob->setTextSize(6);
    knob->setTextColor(0.6);
    knob->setValueSize(5);

    knob = new SAT_KnobWidget(SAT_Rect(30,60,20,20),"d",0);
    res_section->appendChildWidget(knob);
    AEditor->connect(knob,getParameter(10));
    knob->setArcThickness(2);
    knob->setDrawBorder(false);
    knob->setTextSize(6);
    knob->setTextColor(0.6);
    knob->setValueSize(5);

    knob = new SAT_KnobWidget(SAT_Rect(55,60,20,20),"s",0);
    res_section->appendChildWidget(knob);
    AEditor->connect(knob,getParameter(11));
    knob->setArcThickness(2);
    knob->setDrawBorder(false);
    knob->setTextSize(6);
    knob->setTextColor(0.6);
    knob->setValueSize(5);

    knob = new SAT_KnobWidget(SAT_Rect(80,60,20,20),"r",0);
    res_section->appendChildWidget(knob);
    AEditor->connect(knob,getParameter(12));
    knob->setArcThickness(2);
    knob->setDrawBorder(false);
    knob->setTextSize(6);
    knob->setTextColor(0.6);
    knob->setValueSize(5);
    
    // gain
    
    knob = new SAT_KnobWidget(SAT_Rect(5,110,30,30),"G",1);
    root->appendChildWidget(knob);
    AEditor->connect(knob,getParameter(14));
    knob->setArcThickness(4);
    knob->setDrawBorder(false);
    knob->setTextSize(6);
    knob->setTextColor(0.6);
    knob->setValueSize(7);
    
    // voices
    
    MVoicesWidget = new SAT_VoicesWidget(SAT_Rect(115,110,105,5), NUM_VOICES);
    root->appendChildWidget(MVoicesWidget);
    
    //---
    
    root->appendChildWidget(flt_menu);

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
    for (uint32_t voice=0; voice<NUM_VOICES; voice++) {
      uint32_t state = AVoiceManager->getVoiceState(voice);
      MVoicesWidget->setVoiceState(voice,state);
    }
    MVoicesWidget->parentRedraw();
  }
    
