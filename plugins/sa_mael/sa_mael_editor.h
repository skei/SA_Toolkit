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

  SAT_MenuWidget* osc_type_menu = new SAT_MenuWidget(SAT_Rect(0,0,110,15*num_osc_types+2+2));
    osc_type_menu->setLayoutInnerBorder(SAT_Rect(2,2,2,2));
    for (uint32_t i=0; i<num_osc_types; i++) {
      osc_type_menu->appendChildWidget( new SAT_MenuItemWidget(15,osc_types_txt[i] ));
    };

  SAT_MenuWidget* flt_type_menu = new SAT_MenuWidget(SAT_Rect(0,0,110,15*num_flt_types+2+2));
    flt_type_menu->setLayoutInnerBorder(SAT_Rect(2,2,2,2));
    for (uint32_t i=0; i<num_flt_types; i++) {
      flt_type_menu->appendChildWidget( new SAT_MenuItemWidget(15,flt_types_txt[i] ));
    };

  //------------------------------
  // global
  //------------------------------
  
  SAT_PanelWidget* global_panel = new SAT_PanelWidget(SAT_Rect(10,10,130,115));
  root->appendChildWidget(global_panel);
  
  SAT_PanelWidget* global_text = new SAT_PanelWidget(SAT_Rect(0,0,130,20));
  global_panel->appendChildWidget(global_text);
  global_text->setText("Global");

  SAT_KnobWidget* gain_knob = new SAT_KnobWidget(SAT_Rect(10,30,50,50),"G",0.1);
  global_panel->appendChildWidget(gain_knob);
  AEditor->connect(gain_knob,getParameter(SA_MAEL_PARAM_GAIN));

  //------------------------------
  // osc1
  //------------------------------

  SAT_PanelWidget* osc1_panel = new SAT_PanelWidget(SAT_Rect(10,135,130,115));
  root->appendChildWidget(osc1_panel);

  SAT_PanelWidget* osc1_text = new SAT_PanelWidget(SAT_Rect(0,0,130,20));
  osc1_panel->appendChildWidget(osc1_text);
  osc1_text->setText("OSC1");

  SAT_SelectorWidget* osc1_type = new SAT_SelectorWidget(SAT_Rect(10,30,110,15),"select..",osc_type_menu);
  osc1_panel->appendChildWidget(osc1_type);
  AEditor->connect(osc1_type,getParameter(SA_MAEL_PARAM_OSC1_TYPE));

  SAT_KnobWidget* shape_knob = new SAT_KnobWidget(SAT_Rect(10,55,50,50),"Sh",0);
  osc1_panel->appendChildWidget(shape_knob);
  AEditor->connect(shape_knob,getParameter(SA_MAEL_PARAM_OSC1_SHAPE));
  //shape_knob->setQuantize(true);
  //shape_knob->setQuantizeSteps(4);

  SAT_KnobWidget* width_knob = new SAT_KnobWidget(SAT_Rect(70,55,50,50),"Wi",0);
  osc1_panel->appendChildWidget(width_knob);
  AEditor->connect(width_knob,getParameter(SA_MAEL_PARAM_OSC1_WIDTH));
  width_knob->setBipolar(true);
  width_knob->setBipolarCenter(0.5);

  //------------------------------
  // flt1
  //------------------------------

  SAT_PanelWidget* flt1_panel = new SAT_PanelWidget(SAT_Rect(150,135,130,115));
  root->appendChildWidget(flt1_panel);
  
  SAT_PanelWidget* flt1_text = new SAT_PanelWidget(SAT_Rect(0,0,130,20));
  flt1_panel->appendChildWidget(flt1_text);
  flt1_text->setText("FLT1");

  SAT_SelectorWidget* flt1_type = new SAT_SelectorWidget(SAT_Rect(10,30,110,15),"select..",flt_type_menu);
  flt1_panel->appendChildWidget(flt1_type);
  AEditor->connect(flt1_type,getParameter(SA_MAEL_PARAM_FLT1_TYPE));

  SAT_KnobWidget* freq_knob = new SAT_KnobWidget(SAT_Rect(10,55,50,50),"Fr",0);
  flt1_panel->appendChildWidget(freq_knob);
  AEditor->connect(freq_knob,getParameter(SA_MAEL_PARAM_FLT1_FREQ));

  SAT_KnobWidget* bw_knob = new SAT_KnobWidget(SAT_Rect(70,55,50,50),"BW",0);
  flt1_panel->appendChildWidget(bw_knob);
  AEditor->connect(bw_knob,getParameter(SA_MAEL_PARAM_FLT1_BW));

  //------------------------------
  // env1
  //------------------------------

  SAT_PanelWidget* env1_panel = new SAT_PanelWidget(SAT_Rect(290,135,120,115));
  root->appendChildWidget(env1_panel);
  
  SAT_PanelWidget* env1_text = new SAT_PanelWidget(SAT_Rect(0,0,130,20));
  env1_panel->appendChildWidget(env1_text);
  env1_text->setText("ENV1");

  SAT_CurveWidget* curve_a = new SAT_CurveWidget(SAT_Rect(10,30,20,20),false);
  env1_panel->appendChildWidget(curve_a);
  AEditor->connect(curve_a,getParameter(SA_MAEL_PARAM_ENV1_ATT_CURVE));
  //curve_a->setSnap(true);
  //curve_a->setSnapPos(0.5);

  SAT_SliderWidget* slider_a = new SAT_SliderWidget(SAT_Rect(10,55,20,50),"A",0);
  env1_panel->appendChildWidget(slider_a);
  AEditor->connect(slider_a,getParameter(SA_MAEL_PARAM_ENV1_ATT));
  slider_a->setTextOffset(SAT_Rect(0,4,0,0));
  slider_a->setDragDirection(SAT_DIRECTION_UP);
  slider_a->setDrawDirection(SAT_DIRECTION_UP);
  slider_a->setDrawValue(false);
  slider_a->setTextAlignment(SAT_TEXT_ALIGN_TOP);

  SAT_CurveWidget* curve_d = new SAT_CurveWidget(SAT_Rect(35,30,20,20),true);
  env1_panel->appendChildWidget(curve_d);
  AEditor->connect(curve_d,getParameter(SA_MAEL_PARAM_ENV1_DEC_CURVE));
  //curve_d->setSnap(true);
  //curve_d->setSnapPos(0.5);

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

  SAT_CurveWidget* curve_r = new SAT_CurveWidget(SAT_Rect(85,30,20,20),true);
  env1_panel->appendChildWidget(curve_r);
  AEditor->connect(curve_r,getParameter(SA_MAEL_PARAM_ENV1_REL_CURVE));
  //curve_r->setSnap(true);
  //curve_r->setSnapPos(0.5);

  SAT_SliderWidget* slider_r = new SAT_SliderWidget(SAT_Rect(85,55,20,50),"R",0);
  env1_panel->appendChildWidget(slider_r);
  AEditor->connect(slider_r,getParameter(SA_MAEL_PARAM_ENV1_REL));
  slider_r->setTextOffset(SAT_Rect(0,4,0,0));
  slider_r->setDragDirection(SAT_DIRECTION_UP);
  slider_r->setDrawDirection(SAT_DIRECTION_UP);
  slider_r->setDrawValue(false);
  slider_r->setTextAlignment(SAT_TEXT_ALIGN_TOP);

  //------------------------------
  // menus 
  //------------------------------

  root->appendChildWidget(osc_type_menu);
  root->appendChildWidget(flt_type_menu);

}
