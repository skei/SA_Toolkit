// included directly into sa_mael_plugin.h

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
  SAT_RootWidget* root = new SAT_RootWidget(AWindow);
  AWindow->setRootWidget(root);

  //------------------------------
  // menus
  //------------------------------

  // SAT_MenuWidget* osc1_type_menu = new SAT_MenuWidget(SAT_Rect(0,0,110,15*num_osc_types+2+2));
  //   osc1_type_menu->setLayoutInnerBorder(SAT_Rect(2,2,2,2));
  //   for (uint32_t i=0; i<num_osc_types; i++) {
  //     osc1_type_menu->appendChildWidget( new SAT_MenuItemWidget(15,osc_types_txt[i] ));
  //   };

  // SAT_MenuWidget* osc2_type_menu = new SAT_MenuWidget(SAT_Rect(0,0,110,15*num_osc_types+2+2));
  //   osc2_type_menu->setLayoutInnerBorder(SAT_Rect(2,2,2,2));
  //   for (uint32_t i=0; i<num_osc_types; i++) {
  //     osc2_type_menu->appendChildWidget( new SAT_MenuItemWidget(15,osc_types_txt[i] ));
  //   };

  SAT_MenuWidget* mix_type_menu = new SAT_MenuWidget(SAT_Rect(0,0,110,15*num_mix_types+2+2));
    mix_type_menu->setLayoutInnerBorder(SAT_Rect(2,2,2,2));
    for (uint32_t i=0; i<num_mix_types; i++) {
      mix_type_menu->appendChildWidget( new SAT_MenuItemWidget(15,mix_types_txt[i] ));
    };

  SAT_MenuWidget* flt1_type_menu = new SAT_MenuWidget(SAT_Rect(0,0,110,15*num_flt_types+2+2));
    flt1_type_menu->setLayoutInnerBorder(SAT_Rect(2,2,2,2));
    for (uint32_t i=0; i<num_flt_types; i++) {
      flt1_type_menu->appendChildWidget( new SAT_MenuItemWidget(15,flt_types_txt[i] ));
    };

  //------------------------------
  // osc1
  //------------------------------

  SAT_PanelWidget* osc1_panel = new SAT_PanelWidget(SAT_Rect(10,10,160,150));
  root->appendChildWidget(osc1_panel);

  SAT_PanelWidget* osc1_text = new SAT_PanelWidget(SAT_Rect(0,0,160,20));
  osc1_panel->appendChildWidget(osc1_text);
  osc1_text->setText("OSC1");

  SAT_KnobWidget* osc1_squ = new SAT_KnobWidget(SAT_Rect(10,30,40,40),"Sq",0);
  osc1_panel->appendChildWidget(osc1_squ);
  AEditor->connect(osc1_squ,getParameter(SA_MAEL_PARAM_OSC1_SQU));
  osc1_squ->setValueSize(8);
  
  SAT_KnobWidget* osc1_tri = new SAT_KnobWidget(SAT_Rect(60,30,40,40),"Tr",0);
  osc1_panel->appendChildWidget(osc1_tri);
  AEditor->connect(osc1_tri,getParameter(SA_MAEL_PARAM_OSC1_TRI));

  SAT_KnobWidget* osc1_sin = new SAT_KnobWidget(SAT_Rect(110,30,40,40),"Si",0);
  osc1_panel->appendChildWidget(osc1_sin);
  AEditor->connect(osc1_sin,getParameter(SA_MAEL_PARAM_OSC1_SIN));

  SAT_SliderWidget* osc1_width = new SAT_SliderWidget(SAT_Rect(10,80,140,10),"Width",0.5);
  osc1_panel->appendChildWidget(osc1_width);
  AEditor->connect(osc1_width,getParameter(SA_MAEL_PARAM_OSC1_WIDTH));
  osc1_width->setTextSize(7);
  osc1_width->setTextOffset(SAT_Rect(5,1,0,0));
  osc1_width->setValueSize(7);
  osc1_width->setValueOffset(SAT_Rect(0,1,5,0));
  osc1_width->setBipolar(true);
  osc1_width->setBipolarCenter(0.5);
  osc1_width->setSnap(true);
  osc1_width->setSnapPos(0.5);

  SAT_PanelWidget* osc1_preview = new SAT_PanelWidget(SAT_Rect(10,100, 60,40 ));
  osc1_panel->appendChildWidget(osc1_preview);

  SAT_DragValueWidget* osc1_oct  = new SAT_DragValueWidget(SAT_Rect(80,100, 70,12 ),"Oct", 0);
  osc1_panel->appendChildWidget(osc1_oct);
  AEditor->connect(osc1_oct,getParameter(SA_MAEL_PARAM_OSC1_OCT));
  osc1_oct->setTextSize(7);
  osc1_oct->setValueSize(7);

  SAT_DragValueWidget* osc1_semi = new SAT_DragValueWidget(SAT_Rect(80,114, 70,12 ),"Semi",0);
  osc1_panel->appendChildWidget(osc1_semi);
  AEditor->connect(osc1_semi,getParameter(SA_MAEL_PARAM_OSC1_SEMI));
  osc1_semi->setTextSize(7);
  osc1_semi->setValueSize(7);

  SAT_DragValueWidget* osc1_cent = new SAT_DragValueWidget(SAT_Rect(80,128, 70,12 ),"Cent",0);
  osc1_panel->appendChildWidget(osc1_cent);
  AEditor->connect(osc1_cent,getParameter(SA_MAEL_PARAM_OSC1_CENT));
  osc1_cent->setTextSize(7);
  osc1_cent->setValueSize(7);
  osc1_cent->setSnap(true);
  osc1_cent->setSnapPos(0.5);

  //------------------------------
  // osc2
  //------------------------------

  SAT_PanelWidget* osc2_panel = new SAT_PanelWidget(SAT_Rect(180,10,160,150));
  root->appendChildWidget(osc2_panel);

  SAT_PanelWidget* osc2_text = new SAT_PanelWidget(SAT_Rect(0,0,160,20));
  osc2_panel->appendChildWidget(osc2_text);
  osc2_text->setText("OSC2");

  SAT_KnobWidget* osc2_squ = new SAT_KnobWidget(SAT_Rect(10,30,40,40),"Sq",0);
  osc2_panel->appendChildWidget(osc2_squ);
  AEditor->connect(osc2_squ,getParameter(SA_MAEL_PARAM_OSC2_SQU));
  osc2_squ->setValueSize(8);
  
  SAT_KnobWidget* osc2_tri = new SAT_KnobWidget(SAT_Rect(60,30,40,40),"Tr",0);
  osc2_panel->appendChildWidget(osc2_tri);
  AEditor->connect(osc2_tri,getParameter(SA_MAEL_PARAM_OSC2_TRI));

  SAT_KnobWidget* osc2_sin = new SAT_KnobWidget(SAT_Rect(110,30,40,40),"Si",0);
  osc2_panel->appendChildWidget(osc2_sin);
  AEditor->connect(osc2_sin,getParameter(SA_MAEL_PARAM_OSC2_SIN));

  SAT_SliderWidget* osc2_width = new SAT_SliderWidget(SAT_Rect(10,80,140,10),"Width",0.5);
  osc2_panel->appendChildWidget(osc2_width);
  AEditor->connect(osc2_width,getParameter(SA_MAEL_PARAM_OSC2_WIDTH));
  osc2_width->setTextSize(7);
  osc2_width->setTextOffset(SAT_Rect(5,1,0,0));
  osc2_width->setValueSize(7);
  osc2_width->setValueOffset(SAT_Rect(0,1,5,0));
  osc2_width->setBipolar(true);
  osc2_width->setBipolarCenter(0.5);
  osc2_width->setSnap(true);
  osc2_width->setSnapPos(0.5);

  SAT_PanelWidget* osc2_preview = new SAT_PanelWidget(SAT_Rect(10,100, 60,40 ));
  osc2_panel->appendChildWidget(osc2_preview);

  SAT_DragValueWidget* osc2_oct  = new SAT_DragValueWidget(SAT_Rect(80,100, 70,12 ),"Oct", 0);
  osc2_panel->appendChildWidget(osc2_oct);
  AEditor->connect(osc2_oct,getParameter(SA_MAEL_PARAM_OSC2_OCT));
  osc2_oct->setTextSize(7);
  osc2_oct->setValueSize(7);

  SAT_DragValueWidget* osc2_semi = new SAT_DragValueWidget(SAT_Rect(80,114, 70,12 ),"Semi",0);
  osc2_panel->appendChildWidget(osc2_semi);
  AEditor->connect(osc2_semi,getParameter(SA_MAEL_PARAM_OSC2_SEMI));
  osc2_semi->setTextSize(7);
  osc2_semi->setValueSize(7);

  SAT_DragValueWidget* osc2_cent = new SAT_DragValueWidget(SAT_Rect(80,128, 70,12 ),"Cent",0);
  osc2_panel->appendChildWidget(osc2_cent);
  AEditor->connect(osc2_cent,getParameter(SA_MAEL_PARAM_OSC2_CENT));
  osc2_cent->setTextSize(7);
  osc2_cent->setValueSize(7);
  osc2_cent->setSnap(true);
  osc2_cent->setSnapPos(0.5);

  //------------------------------
  // mix
  //------------------------------

  SAT_PanelWidget* mix_panel = new SAT_PanelWidget(SAT_Rect(10,170,130,115));
  root->appendChildWidget(mix_panel);
  
  SAT_PanelWidget* mix_text = new SAT_PanelWidget(SAT_Rect(0,0,130,20));
  mix_panel->appendChildWidget(mix_text);
  mix_text->setText("MIX");

  SAT_SelectorWidget* mix_type = new SAT_SelectorWidget(SAT_Rect(10,30,110,15),"select..",mix_type_menu);
  mix_panel->appendChildWidget(mix_type);
  AEditor->connect(mix_type,getParameter(SA_MAEL_PARAM_MIX_TYPE));

  SAT_KnobWidget* mix_amount = new SAT_KnobWidget(SAT_Rect(10,55,50,50),"Am",0);
  mix_panel->appendChildWidget(mix_amount);
  AEditor->connect(mix_amount,getParameter(SA_MAEL_PARAM_MIX_AMOUNT));

  //------------------------------
  // flt1
  //------------------------------

  SAT_PanelWidget* flt1_panel = new SAT_PanelWidget(SAT_Rect(150,170,130,115));
  root->appendChildWidget(flt1_panel);
  
  SAT_PanelWidget* flt1_text = new SAT_PanelWidget(SAT_Rect(0,0,130,20));
  flt1_panel->appendChildWidget(flt1_text);
  flt1_text->setText("FLT1");

  SAT_SelectorWidget* flt1_type = new SAT_SelectorWidget(SAT_Rect(10,30,110,15),"select..",flt1_type_menu);
  flt1_panel->appendChildWidget(flt1_type);
  AEditor->connect(flt1_type,getParameter(SA_MAEL_PARAM_FLT1_TYPE));

  SAT_KnobWidget* freq_knob = new SAT_KnobWidget(SAT_Rect(10,55,50,50),"Fr",0);
  flt1_panel->appendChildWidget(freq_knob);
  AEditor->connect(freq_knob,getParameter(SA_MAEL_PARAM_FLT1_FREQ));

  SAT_KnobWidget* q_knob = new SAT_KnobWidget(SAT_Rect(70,55,50,50),"Q",0);
  flt1_panel->appendChildWidget(q_knob);
  AEditor->connect(q_knob,getParameter(SA_MAEL_PARAM_FLT1_Q));

  //------------------------------
  // env1
  //------------------------------

  SAT_PanelWidget* env1_panel = new SAT_PanelWidget(SAT_Rect(290,170,120,115));
  root->appendChildWidget(env1_panel);
  
  SAT_PanelWidget* env1_text = new SAT_PanelWidget(SAT_Rect(0,0,130,20));
  env1_panel->appendChildWidget(env1_text);
  env1_text->setText("ENV1");

  SAT_SliderWidget* slider_a = new SAT_SliderWidget(SAT_Rect(10,55,20,50),"A",0);
  env1_panel->appendChildWidget(slider_a);
  AEditor->connect(slider_a,getParameter(SA_MAEL_PARAM_ENV1_ATT));
  slider_a->setTextOffset(SAT_Rect(0,4,0,0));
  slider_a->setDragDirection(SAT_DIRECTION_UP);
  slider_a->setDrawDirection(SAT_DIRECTION_UP);
  slider_a->setDrawValue(false);
  slider_a->setTextAlignment(SAT_TEXT_ALIGN_TOP);

  SAT_SliderWidget* slider_d = new SAT_SliderWidget(SAT_Rect(35,55,20,50),"D",0);
  env1_panel->appendChildWidget(slider_d);
  AEditor->connect(slider_d,getParameter(SA_MAEL_PARAM_ENV1_DEC));
  slider_d->setTextOffset(SAT_Rect(0,4,0,0));
  slider_d->setDragDirection(SAT_DIRECTION_UP);
  slider_d->setDrawDirection(SAT_DIRECTION_UP);
  slider_d->setDrawValue(false);
  slider_d->setTextAlignment(SAT_TEXT_ALIGN_TOP);

  SAT_SliderWidget* slider_s = new SAT_SliderWidget(SAT_Rect(60,55,20,50),"S",0);
  env1_panel->appendChildWidget(slider_s);
  AEditor->connect(slider_s,getParameter(SA_MAEL_PARAM_ENV1_SUS));
  slider_s->setTextOffset(SAT_Rect(0,4,0,0));
  slider_s->setDragDirection(SAT_DIRECTION_UP);
  slider_s->setDrawDirection(SAT_DIRECTION_UP);
  slider_s->setDrawValue(false);
  slider_s->setTextAlignment(SAT_TEXT_ALIGN_TOP);

  SAT_SliderWidget* slider_r = new SAT_SliderWidget(SAT_Rect(85,55,20,50),"R",0);
  env1_panel->appendChildWidget(slider_r);
  AEditor->connect(slider_r,getParameter(SA_MAEL_PARAM_ENV1_REL));
  slider_r->setTextOffset(SAT_Rect(0,4,0,0));
  slider_r->setDragDirection(SAT_DIRECTION_UP);
  slider_r->setDrawDirection(SAT_DIRECTION_UP);
  slider_r->setDrawValue(false);
  slider_r->setTextAlignment(SAT_TEXT_ALIGN_TOP);

  //------------------------------
  // global
  //------------------------------

  SAT_PanelWidget* global_panel = new SAT_PanelWidget(SAT_Rect(350,10,70,85));
  root->appendChildWidget(global_panel);
  
  SAT_PanelWidget* global_text = new SAT_PanelWidget(SAT_Rect(0,0,70,20));
  global_panel->appendChildWidget(global_text);
  global_text->setText("Global");

  SAT_KnobWidget* gain_knob = new SAT_KnobWidget(SAT_Rect(10,30,50,50),"G",0.1);
  global_panel->appendChildWidget(gain_knob);
  AEditor->connect(gain_knob,getParameter(SA_MAEL_PARAM_GAIN));

  //------------------------------
  // menus 
  //------------------------------

  // root->appendChildWidget(osc1_type_menu);
  // root->appendChildWidget(osc2_type_menu);
  root->appendChildWidget(mix_type_menu);
  root->appendChildWidget(flt1_type_menu);

}
