// included directly into sa_mael_plugin.h

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
  SAT_RootWidget* root = new SAT_RootWidget(0,AWindow);
  AWindow->setRootWidget(root);

  SAT_KnobWidget* knob;

  // global
  
  SAT_PanelWidget* global_panel = new SAT_PanelWidget(SAT_Rect(10,10,200,200));
  root->appendChildWidget(global_panel);
  
  SAT_PanelWidget* global_text = new SAT_PanelWidget(SAT_Rect(0,0,200,20));
  global_panel->appendChildWidget(global_text);
  global_text->setText("Global");

  knob = new SAT_KnobWidget(SAT_Rect(10,30,50,50),"Gain",0.1);
  global_panel->appendChildWidget(knob);
  AEditor->connect(knob,getParameter(SA_MAEL_PARAM_GAIN));

  // osc1

  SAT_PanelWidget* osc1_panel = new SAT_PanelWidget(SAT_Rect(10,220,200,200));
  root->appendChildWidget(osc1_panel);

  SAT_PanelWidget* osc1_text = new SAT_PanelWidget(SAT_Rect(0,0,200,20));
  osc1_panel->appendChildWidget(osc1_text);
  osc1_text->setText("OSC1");

  knob = new SAT_KnobWidget(SAT_Rect(10,30,50,50),"Type",0);
  osc1_panel->appendChildWidget(knob);
  AEditor->connect(knob,getParameter(SA_MAEL_PARAM_OSC1_TYPE));

  knob = new SAT_KnobWidget(SAT_Rect(70,30,50,50),"Shape",0);
  osc1_panel->appendChildWidget(knob);
  AEditor->connect(knob,getParameter(SA_MAEL_PARAM_OSC1_SHAPE));

  knob = new SAT_KnobWidget(SAT_Rect(130,30,50,50),"Width",0);
  osc1_panel->appendChildWidget(knob);
  AEditor->connect(knob,getParameter(SA_MAEL_PARAM_OSC1_WIDTH));

  // flt1

  SAT_PanelWidget* flt1_panel = new SAT_PanelWidget(SAT_Rect(220,220,200,200));
  root->appendChildWidget(flt1_panel);
  
  SAT_PanelWidget* flt1_text = new SAT_PanelWidget(SAT_Rect(0,0,200,20));
  flt1_panel->appendChildWidget(flt1_text);
  flt1_text->setText("FLT1");

  knob = new SAT_KnobWidget(SAT_Rect(10,30,50,50),"Type",0);
  flt1_panel->appendChildWidget(knob);
  AEditor->connect(knob,getParameter(SA_MAEL_PARAM_FLT1_TYPE));

  knob = new SAT_KnobWidget(SAT_Rect(70,30,50,50),"Freq",0);
  flt1_panel->appendChildWidget(knob);
  AEditor->connect(knob,getParameter(SA_MAEL_PARAM_FLT1_FREQ));

  knob = new SAT_KnobWidget(SAT_Rect(130,30,50,50),"BW",0);
  flt1_panel->appendChildWidget(knob);
  AEditor->connect(knob,getParameter(SA_MAEL_PARAM_FLT1_BW));

  // env1

  SAT_PanelWidget* env1_panel = new SAT_PanelWidget(SAT_Rect(430,220,200,200));
  root->appendChildWidget(env1_panel);
  
  SAT_PanelWidget* env1_text = new SAT_PanelWidget(SAT_Rect(0,0,200,20));
  env1_panel->appendChildWidget(env1_text);
  env1_text->setText("ENV1");

  knob = new SAT_KnobWidget(SAT_Rect(10,30,35,35),"A",0);
  env1_panel->appendChildWidget(knob);
  knob->setArcThickness(5);
  knob->setValueSize(8);

  knob = new SAT_KnobWidget(SAT_Rect(55,30,35,35),"D",0);
  env1_panel->appendChildWidget(knob);
  knob->setArcThickness(5);
  knob->setValueSize(8);

  knob = new SAT_KnobWidget(SAT_Rect(100,30,35,35),"S",0);
  env1_panel->appendChildWidget(knob);
  knob->setArcThickness(5);
  knob->setValueSize(8);

  knob = new SAT_KnobWidget(SAT_Rect(145,30,35,35),"R",0);
  env1_panel->appendChildWidget(knob);
  knob->setArcThickness(5);
  knob->setValueSize(8);


}
