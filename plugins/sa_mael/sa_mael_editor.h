// included directly into sa_mael_plugin.h

//------------------------------
private:
//------------------------------

  SAT_MenuWidget*   mix_type_menu   = nullptr;
  SAT_MenuWidget*   flt1_type_menu  = nullptr;
  SAT_VoicesWidget* voices          = nullptr;

//------------------------------
public:
//------------------------------

  bool setupEditor(SAT_Editor* AEditor) final {
    SAT_Window* window = AEditor->getWindow();

    SAT_RootWidget* root = new SAT_RootWidget( window, SAT_Rect() );
    window->setRootWidget(root);
    root->setDrawBorder(true);
    root->setBorderColor(0.4);

    SAT_PluginHeaderWidget* header = new SAT_PluginHeaderWidget(40,"mael");
    root->appendChild(header);

    SAT_PluginFooterWidget* footer = new SAT_PluginFooterWidget(25,"(hint)");
    root->appendChild(footer);
    window->setHintWidget(footer);

    SAT_VisualWidget* middle = new SAT_VisualWidget(0);
    root->appendChild(middle);
    middle->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM_LEFT; // not TOP?
    middle->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
    //middle->Layout.innerBorder  = SAT_Rect(5,5,5,5);
    //middle->Layout.spacing      = SAT_Point(0,0);

    //------------------------------
    // menus
    //------------------------------

    // SAT_MenuWidget* osc1_type_menu = new SAT_MenuWidget(SAT_Rect(0,0,110,15*num_osc_types+2+2));
    //   osc1_type_menu->setLayoutInnerBorder(SAT_Rect(2,2,2,2));
    //   for (uint32_t i=0; i<num_osc_types; i++) {
    //     osc1_type_menu->appendChild( new SAT_MenuItemWidget(15,osc_types_txt[i] ));
    //   };

    // SAT_MenuWidget* osc2_type_menu = new SAT_MenuWidget(SAT_Rect(0,0,110,15*num_osc_types+2+2));
    //   osc2_type_menu->setLayoutInnerBorder(SAT_Rect(2,2,2,2));
    //   for (uint32_t i=0; i<num_osc_types; i++) {
    //     osc2_type_menu->appendChild( new SAT_MenuItemWidget(15,osc_types_txt[i] ));
    //   };

    mix_type_menu = new SAT_MenuWidget(SAT_Rect(0,0,110,20*num_mix_types+2+2));
      mix_type_menu->Layout.innerBorder = SAT_Rect(2,2,2,2);
      for (uint32_t i=0; i<num_mix_types; i++) {
        mix_type_menu->appendItem( mix_types_txt[i] );
      }


    flt1_type_menu = new SAT_MenuWidget(SAT_Rect(0,0,110,20*num_flt_types+2+2));
      flt1_type_menu->Layout.innerBorder = SAT_Rect(2,2,2,2);
      for (uint32_t i=0; i<num_flt_types; i++) {
        flt1_type_menu->appendItem( flt_types_txt[i] );
      }

    //------------------------------
    // osc1
    //------------------------------

    SAT_VisualWidget* osc1_panel = new SAT_VisualWidget(SAT_Rect(10,10,160,150));
    middle->appendChild(osc1_panel);

    SAT_TextWidget* osc1_text = new SAT_TextWidget(SAT_Rect(0,0,160,20));
    osc1_panel->appendChild(osc1_text);
    osc1_text->setText("OSC1");

    SAT_KnobWidget* osc1_squ = new SAT_KnobWidget(SAT_Rect(10,30,40,40),"Sq",0);
    osc1_panel->appendChild(osc1_squ);
    AEditor->connect(osc1_squ,getParameter(SA_MAEL_PARAM_OSC1_SQU));
    osc1_squ->setValueTextSize(8);
    
    SAT_KnobWidget* osc1_tri = new SAT_KnobWidget(SAT_Rect(60,30,40,40),"Tr",0);
    osc1_panel->appendChild(osc1_tri);
    AEditor->connect(osc1_tri,getParameter(SA_MAEL_PARAM_OSC1_TRI));

    SAT_KnobWidget* osc1_sin = new SAT_KnobWidget(SAT_Rect(110,30,40,40),"Si",0);
    osc1_panel->appendChild(osc1_sin);
    AEditor->connect(osc1_sin,getParameter(SA_MAEL_PARAM_OSC1_SIN));

    SAT_SliderWidget* osc1_width = new SAT_SliderWidget(SAT_Rect(10,80,140,10),"Width",0.5);
    osc1_panel->appendChild(osc1_width);
    AEditor->connect(osc1_width,getParameter(SA_MAEL_PARAM_OSC1_WIDTH));
    osc1_width->setTextSize(7);
    osc1_width->setTextOffset(SAT_Rect(5,1,0,0));
    osc1_width->setValueTextSize(7);
    osc1_width->setValueTextOffset(SAT_Rect(0,1,5,0));
    osc1_width->setDrawBipolar(true);
    osc1_width->setBipolarCenter(0.5);
    osc1_width->setDragSnap(true);
    osc1_width->setDragSnapPos(0.5);

    SAT_VisualWidget* osc1_preview = new SAT_VisualWidget(SAT_Rect(10,100, 60,40 ));
    osc1_panel->appendChild(osc1_preview);

    SAT_DragValueWidget* osc1_oct  = new SAT_DragValueWidget(SAT_Rect(110,100, 40,12 ),"Oct", 0);
    osc1_panel->appendChild(osc1_oct);
    AEditor->connect(osc1_oct,getParameter(SA_MAEL_PARAM_OSC1_OCT));
    osc1_oct->setTextSize(7);
    osc1_oct->setValueTextSize(7);
    
    SAT_DragValueWidget* osc1_semi = new SAT_DragValueWidget(SAT_Rect(110,114, 40,12 ),"Semi",0);
    osc1_panel->appendChild(osc1_semi);
    AEditor->connect(osc1_semi,getParameter(SA_MAEL_PARAM_OSC1_SEMI));
    osc1_semi->setTextSize(7);
    osc1_semi->setValueTextSize(7);

    SAT_DragValueWidget* osc1_cent = new SAT_DragValueWidget(SAT_Rect(110,128, 40,12 ),"Cent",0);
    osc1_panel->appendChild(osc1_cent);
    AEditor->connect(osc1_cent,getParameter(SA_MAEL_PARAM_OSC1_CENT));
    osc1_cent->setTextSize(7);
    osc1_cent->setValueTextSize(7);
    osc1_cent->setDragSnap(true);
    osc1_cent->setDragSnapPos(0.5);

    //------------------------------
    // osc2
    //------------------------------

    SAT_VisualWidget* osc2_panel = new SAT_VisualWidget(SAT_Rect(180,10,160,150));
    middle->appendChild(osc2_panel);

    SAT_TextWidget* osc2_text = new SAT_TextWidget(SAT_Rect(0,0,160,20));
    osc2_panel->appendChild(osc2_text);
    osc2_text->setText("OSC2");

    SAT_KnobWidget* osc2_squ = new SAT_KnobWidget(SAT_Rect(10,30,40,40),"Sq",0);
    osc2_panel->appendChild(osc2_squ);
    AEditor->connect(osc2_squ,getParameter(SA_MAEL_PARAM_OSC2_SQU));
    osc2_squ->setValueTextSize(8);
    
    SAT_KnobWidget* osc2_tri = new SAT_KnobWidget(SAT_Rect(60,30,40,40),"Tr",0);
    osc2_panel->appendChild(osc2_tri);
    AEditor->connect(osc2_tri,getParameter(SA_MAEL_PARAM_OSC2_TRI));

    SAT_KnobWidget* osc2_sin = new SAT_KnobWidget(SAT_Rect(110,30,40,40),"Si",0);
    osc2_panel->appendChild(osc2_sin);
    AEditor->connect(osc2_sin,getParameter(SA_MAEL_PARAM_OSC2_SIN));

    SAT_SliderWidget* osc2_width = new SAT_SliderWidget(SAT_Rect(10,80,140,10),"Width",0.5);
    osc2_panel->appendChild(osc2_width);
    AEditor->connect(osc2_width,getParameter(SA_MAEL_PARAM_OSC2_WIDTH));
    osc2_width->setTextSize(7);
    osc2_width->setTextOffset(SAT_Rect(5,1,0,0));
    osc2_width->setValueTextSize(7);
    osc2_width->setValueTextOffset(SAT_Rect(0,1,5,0));
    osc2_width->setDrawBipolar(true);
    osc2_width->setBipolarCenter(0.5);
    osc2_width->setDragSnap(true);
    osc2_width->setDragSnapPos(0.5);

    SAT_VisualWidget* osc2_preview = new SAT_VisualWidget(SAT_Rect(10,100, 60,40 ));
    osc2_panel->appendChild(osc2_preview);

    SAT_DragValueWidget* osc2_oct  = new SAT_DragValueWidget(SAT_Rect(110,100, 40,12 ),"Oct", 0);
    osc2_panel->appendChild(osc2_oct);
    AEditor->connect(osc2_oct,getParameter(SA_MAEL_PARAM_OSC2_OCT));
    osc2_oct->setTextSize(7);
    osc2_oct->setValueTextSize(7);

    SAT_DragValueWidget* osc2_semi = new SAT_DragValueWidget(SAT_Rect(110,114, 40,12 ),"Semi",0);
    osc2_panel->appendChild(osc2_semi);
    AEditor->connect(osc2_semi,getParameter(SA_MAEL_PARAM_OSC2_SEMI));
    osc2_semi->setTextSize(7);
    osc2_semi->setValueTextSize(7);

    SAT_DragValueWidget* osc2_cent = new SAT_DragValueWidget(SAT_Rect(110,128, 40,12 ),"Cent",0);
    osc2_panel->appendChild(osc2_cent);
    AEditor->connect(osc2_cent,getParameter(SA_MAEL_PARAM_OSC2_CENT));
    osc2_cent->setTextSize(7);
    osc2_cent->setValueTextSize(7);
    osc2_cent->setDragSnap(true);
    osc2_cent->setDragSnapPos(0.5);

    //------------------------------
    // mix
    //------------------------------

    SAT_VisualWidget* mix_panel = new SAT_VisualWidget(SAT_Rect(10,170,130,115));
    middle->appendChild(mix_panel);
    
    SAT_TextWidget* mix_text = new SAT_TextWidget(SAT_Rect(0,0,130,20));
    mix_panel->appendChild(mix_text);
    mix_text->setText("MIX");

    SAT_SelectorWidget* mix_type = new SAT_SelectorWidget(SAT_Rect(10,30,110,15),"select..",mix_type_menu);
    mix_panel->appendChild(mix_type);
    AEditor->connect(mix_type,getParameter(SA_MAEL_PARAM_MIX_TYPE));
    mix_type_menu->setListener(mix_type);
    mix_type->setDrawParamText(false);
    mix_type->select(0);

    SAT_KnobWidget* mix_amount = new SAT_KnobWidget(SAT_Rect(10,55,50,50),"Am",0);
    mix_panel->appendChild(mix_amount);
    AEditor->connect(mix_amount,getParameter(SA_MAEL_PARAM_MIX_AMOUNT));

    //------------------------------
    // flt1
    //------------------------------

    SAT_VisualWidget* flt1_panel = new SAT_VisualWidget(SAT_Rect(150,170,130,115));
    middle->appendChild(flt1_panel);
    
    SAT_TextWidget* flt1_text = new SAT_TextWidget(SAT_Rect(0,0,130,20));
    flt1_panel->appendChild(flt1_text);
    flt1_text->setText("FLT1");

    SAT_SelectorWidget* flt1_type = new SAT_SelectorWidget(SAT_Rect(10,30,110,15),"select..",flt1_type_menu);
    flt1_panel->appendChild(flt1_type);
    AEditor->connect(flt1_type,getParameter(SA_MAEL_PARAM_FLT1_TYPE));
    flt1_type_menu->setListener(flt1_type);
    flt1_type->setDrawParamText(false);
    flt1_type->select(0);

    SAT_KnobWidget* freq_knob = new SAT_KnobWidget(SAT_Rect(10,55,50,50),"Fr",0);
    flt1_panel->appendChild(freq_knob);
    AEditor->connect(freq_knob,getParameter(SA_MAEL_PARAM_FLT1_FREQ));

    SAT_KnobWidget* q_knob = new SAT_KnobWidget(SAT_Rect(70,55,50,50),"Q",0);
    flt1_panel->appendChild(q_knob);
    AEditor->connect(q_knob,getParameter(SA_MAEL_PARAM_FLT1_Q));

    //------------------------------
    // env1
    //------------------------------

    SAT_VisualWidget* env1_panel = new SAT_VisualWidget(SAT_Rect(290,170,120,115));
    middle->appendChild(env1_panel);
    
    SAT_TextWidget* env1_text = new SAT_TextWidget(SAT_Rect(0,0,130,20));
    env1_panel->appendChild(env1_text);
    env1_text->setText("ENV1");

    SAT_SliderWidget* slider_a = new SAT_SliderWidget(SAT_Rect(10,55,20,50),"A",0);
    env1_panel->appendChild(slider_a);
    AEditor->connect(slider_a,getParameter(SA_MAEL_PARAM_ENV1_ATT));
    slider_a->setTextOffset(SAT_Rect(0,4,0,0));
    slider_a->setDragDirection(SAT_DIRECTION_UP);
    slider_a->setDrawDirection(SAT_DIRECTION_UP);
    slider_a->setDrawValueText(false);
    slider_a->setTextAlignment(SAT_TEXT_ALIGN_TOP);

    SAT_SliderWidget* slider_d = new SAT_SliderWidget(SAT_Rect(35,55,20,50),"D",0);
    env1_panel->appendChild(slider_d);
    AEditor->connect(slider_d,getParameter(SA_MAEL_PARAM_ENV1_DEC));
    slider_d->setTextOffset(SAT_Rect(0,4,0,0));
    slider_d->setDragDirection(SAT_DIRECTION_UP);
    slider_d->setDrawDirection(SAT_DIRECTION_UP);
    slider_d->setDrawValueText(false);
    slider_d->setTextAlignment(SAT_TEXT_ALIGN_TOP);

    SAT_SliderWidget* slider_s = new SAT_SliderWidget(SAT_Rect(60,55,20,50),"S",0);
    env1_panel->appendChild(slider_s);
    AEditor->connect(slider_s,getParameter(SA_MAEL_PARAM_ENV1_SUS));
    slider_s->setTextOffset(SAT_Rect(0,4,0,0));
    slider_s->setDragDirection(SAT_DIRECTION_UP);
    slider_s->setDrawDirection(SAT_DIRECTION_UP);
    slider_s->setDrawValueText(false);
    slider_s->setTextAlignment(SAT_TEXT_ALIGN_TOP);

    SAT_SliderWidget* slider_r = new SAT_SliderWidget(SAT_Rect(85,55,20,50),"R",0);
    env1_panel->appendChild(slider_r);
    AEditor->connect(slider_r,getParameter(SA_MAEL_PARAM_ENV1_REL));
    slider_r->setTextOffset(SAT_Rect(0,4,0,0));
    slider_r->setDragDirection(SAT_DIRECTION_UP);
    slider_r->setDrawDirection(SAT_DIRECTION_UP);
    slider_r->setDrawValueText(false);
    slider_r->setTextAlignment(SAT_TEXT_ALIGN_TOP);

    //------------------------------
    // global
    //------------------------------

    SAT_VisualWidget* global_panel = new SAT_VisualWidget(SAT_Rect(350,10,70,85));
    middle->appendChild(global_panel);
    
    SAT_TextWidget* global_text = new SAT_TextWidget(SAT_Rect(0,0,70,20));
    global_panel->appendChild(global_text);
    global_text->setText("Global");

    SAT_KnobWidget* gain_knob = new SAT_KnobWidget(SAT_Rect(10,30,50,50),"G",0.1);
    global_panel->appendChild(gain_knob);
    AEditor->connect(gain_knob,getParameter(SA_MAEL_PARAM_GAIN));

    //

    voices = new SAT_VoicesWidget(SAT_Rect(350,105,70,10),MAX_VOICES);
    middle->appendChild(voices);

    return true;
  }

//------------------------------
public:
//------------------------------

  bool setupOverlay(SAT_Editor* AEditor) final {
    SAT_Window* window = AEditor->getWindow();
    SAT_Widget* overlay = window->getOverlayWidget();
    // root->appendChild(osc1_type_menu);
    // root->appendChild(osc2_type_menu);
    overlay->appendChild(mix_type_menu);
    overlay->appendChild(flt1_type_menu);
    return true;
  }


