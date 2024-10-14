#ifndef sa_synth_gui_included
#define sa_synth_gui_included
//----------------------------------------------------------------------

// note:
// inserted directly into sa_synth.cpp
// (dirty, but we are too lazy to set up a proper class and all that).. :-/

//------------------------------
private:
//------------------------------

  SAT_MenuWidget* osc1_type_menu = nullptr;
  SAT_MenuWidget* osc2_type_menu = nullptr;
  SAT_MenuWidget* res1_type_menu = nullptr;
  SAT_MenuWidget* res2_type_menu = nullptr;
  SAT_MenuWidget* flt1_type_menu = nullptr;
  char MInfoText[SAT_MAX_STRING_LENGTH] = {0};

//------------------------------
public:
//------------------------------


  bool setupEditor(SAT_Editor* AEditor) final {
    SAT_Window* window = AEditor->getWindow();
    if (window) {
      SAT_RootWidget* root = new SAT_RootWidget(window);
      window->setRootWidget(root);
      root->setFillBackground(false);
      root->setDrawBorder(false);

      //------------------------------
      // main
      //------------------------------

      sprintf(MInfoText,"v%s : %s : %s", sa_synth_descriptor.version, getPluginFormat(), SAT_GLOBAL.getOSName() );

      SAT_PluginHeaderWidget* header = new SAT_PluginHeaderWidget(40,"synth",MInfoText);
      root->appendChild(header);

      SAT_PluginFooterWidget* footer = new SAT_PluginFooterWidget(20,"");
      root->appendChild(footer);

      SAT_VisualWidget* main_panel = new SAT_VisualWidget(0);
      root->appendChild(main_panel);
      main_panel->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
      main_panel->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
      main_panel->setDrawBorder(false);

      //------------------------------
      // menus
      //------------------------------

      osc1_type_menu = new SAT_MenuWidget(SAT_Rect(0,0,110,20*sa_synth_osc_type_count+2+2));
      osc1_type_menu->Layout.innerBorder = SAT_Rect(2,2,2,2);
      for (uint32_t i=0; i<sa_synth_osc_type_count; i++) {
        osc1_type_menu->appendItem( osc_type_text[i] );
      }

      osc2_type_menu = new SAT_MenuWidget(SAT_Rect(0,0,110,20*sa_synth_osc_type_count+2+2));
      osc2_type_menu->Layout.innerBorder = SAT_Rect(2,2,2,2);
      for (uint32_t i=0; i<sa_synth_osc_type_count; i++) {
        osc2_type_menu->appendItem( osc_type_text[i] );
      }

      res1_type_menu = new SAT_MenuWidget(SAT_Rect(0,0,110,20*sa_synth_res_type_count+2+2));
      res1_type_menu->Layout.innerBorder = SAT_Rect(2,2,2,2);
      for (uint32_t i=0; i<sa_synth_res_type_count; i++) {
        res1_type_menu->appendItem( res_type_text[i] );
      }

      res2_type_menu = new SAT_MenuWidget(SAT_Rect(0,0,110,20*sa_synth_res_type_count+2+2));
      res2_type_menu->Layout.innerBorder = SAT_Rect(2,2,2,2);
      for (uint32_t i=0; i<sa_synth_res_type_count; i++) {
        res2_type_menu->appendItem( res_type_text[i] );
      }

      flt1_type_menu = new SAT_MenuWidget(SAT_Rect(0,0,110,20*sa_synth_flt_type_count+2+2));
      flt1_type_menu->Layout.innerBorder = SAT_Rect(2,2,2,2);
      for (uint32_t i=0; i<sa_synth_flt_type_count; i++) {
        flt1_type_menu->appendItem( flt_type_text[i] );
      }

      //------------------------------
      // osc.1
      //------------------------------

      SAT_VisualWidget* osc1_panel = new SAT_VisualWidget(SAT_Rect(10,10,350,85));
      main_panel->appendChild(osc1_panel);
      osc1_panel->setFillBackground(false);
      osc1_panel->setDrawBorder(true);
      osc1_panel->setBorderColor(SAT_DarkGrey);
      osc1_panel->setBorderWidth(1);

        SAT_TextWidget* osc1_text = new SAT_TextWidget(30,"Osc 1");
        osc1_panel->appendChild(osc1_text);
        osc1_text->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
        osc1_text->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
        osc1_text->setDrawBorder(false);
        osc1_text->setFillBackground(true);
        osc1_text->setBackgroundColor(SAT_DarkerGrey);
        osc1_text->setTextColor(SAT_LighterGrey);
        osc1_text->setTextAlignment(SAT_TEXT_ALIGN_TOP | SAT_TEXT_ALIGN_LEFT);
        osc1_text->setTextOffset(SAT_Rect(5,5,0,0));

          SAT_KnobWidget* osc1_in_amt = new SAT_KnobWidget(SAT_Rect(117,3,26,26),"Amt",0.0);
          AEditor->connect(osc1_in_amt,getParameter(SA_SYNTH_PARAM_OSC1_IN_AMOUNT));
          osc1_text->appendChild(osc1_in_amt);
          osc1_in_amt->setKnobArcBackgroundColor(SAT_Grey);
          osc1_in_amt->setKnobArcColor(SAT_White);
          osc1_in_amt->setKnobArcThickness(4);
          osc1_in_amt->setDrawText(true);
          osc1_in_amt->setTextSize(5);
          osc1_in_amt->setTextColor(SAT_LightGrey);
          osc1_in_amt->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          osc1_in_amt->setDrawValueText(false);

          SAT_SelectorWidget* osc1_type = new SAT_SelectorWidget(SAT_Rect(148,9,60,12),"Mix",osc1_type_menu);
          AEditor->connect(osc1_type,getParameter(SA_SYNTH_PARAM_OSC1_TYPE));
          osc1_text->appendChild(osc1_type);
          osc1_type->setFillBackground(true);
          osc1_type->setFillGradient(false);
          osc1_type->setBackgroundColor(0.4);
          osc1_type->setDrawBorder(false);
          osc1_type->setTextSize(8);
          osc1_type->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
          osc1_type->setTextOffset(SAT_Rect(2,0,0,0));
          osc1_type->setDrawParamText(false);
          osc1_type->select(0);
          osc1_type_menu->setListener(osc1_type);

          SAT_KnobWidget* osc1_input1 = new SAT_KnobWidget(SAT_Rect(213,6,20,20),"O1",0.5);
          AEditor->connect(osc1_input1,getParameter(SA_SYNTH_PARAM_OSC1_IN_O1));
          osc1_text->appendChild(osc1_input1);
          osc1_input1->setKnobArcBackgroundColor(SAT_Grey);
          osc1_input1->setKnobArcColor(SAT_White);
          osc1_input1->setKnobArcThickness(3);
          osc1_input1->setDrawText(true);
          osc1_input1->setTextSize(5);
          osc1_input1->setTextColor(SAT_LightGrey);
          osc1_input1->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          osc1_input1->setDrawValueText(false);
          //osc1_input1->setDrawBipolar(true);

          SAT_KnobWidget* osc1_input2 = new SAT_KnobWidget(SAT_Rect(235,6,20,20),"O2",0.5);
          AEditor->connect(osc1_input2,getParameter(SA_SYNTH_PARAM_OSC1_IN_O2));
          osc1_text->appendChild(osc1_input2);
          osc1_input2->setKnobArcBackgroundColor(SAT_Grey);
          osc1_input2->setKnobArcColor(SAT_White);
          osc1_input2->setKnobArcThickness(3);
          osc1_input2->setDrawText(true);
          osc1_input2->setTextSize(5);
          osc1_input2->setTextColor(SAT_LightGrey);
          osc1_input2->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          osc1_input2->setDrawValueText(false);
          //osc1_input2->setDrawBipolar(true);

          SAT_KnobWidget* osc1_input3 = new SAT_KnobWidget(SAT_Rect(257,6,20,20),"R1",0.5);
          AEditor->connect(osc1_input3,getParameter(SA_SYNTH_PARAM_OSC1_IN_R1));
          osc1_text->appendChild(osc1_input3);
          osc1_input3->setKnobArcBackgroundColor(SAT_Grey);
          osc1_input3->setKnobArcColor(SAT_White);
          osc1_input3->setKnobArcThickness(3);
          osc1_input3->setDrawText(true);
          osc1_input3->setTextSize(5);
          osc1_input3->setTextColor(SAT_LightGrey);
          osc1_input3->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          osc1_input3->setDrawValueText(false);
          //osc1_input3->setDrawBipolar(true);

          SAT_KnobWidget* osc1_input4 = new SAT_KnobWidget(SAT_Rect(279,6,20,20),"R2",0.5);
          AEditor->connect(osc1_input4,getParameter(SA_SYNTH_PARAM_OSC1_IN_R2));
          osc1_text->appendChild(osc1_input4);
          osc1_input4->setKnobArcBackgroundColor(SAT_Grey);
          osc1_input4->setKnobArcColor(SAT_White);
          osc1_input4->setKnobArcThickness(3);
          osc1_input4->setDrawText(true);
          osc1_input4->setTextSize(5);
          osc1_input4->setTextColor(SAT_LightGrey);
          osc1_input4->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          osc1_input4->setDrawValueText(false);
          //osc1_input4->setDrawBipolar(true);

          SAT_KnobWidget* osc1_inputa = new SAT_KnobWidget(SAT_Rect(301,6,20,20),"A",0.5);
          AEditor->connect(osc1_inputa,getParameter(SA_SYNTH_PARAM_OSC1_IN_A));
          osc1_text->appendChild(osc1_inputa);
          osc1_inputa->setKnobArcBackgroundColor(SAT_Grey);
          osc1_inputa->setKnobArcColor(SAT_White);
          osc1_inputa->setKnobArcThickness(3);
          osc1_inputa->setDrawText(true);
          osc1_inputa->setTextSize(5);
          osc1_inputa->setTextColor(SAT_LightGrey);
          osc1_inputa->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          osc1_inputa->setDrawValueText(false);
          //osc1_inputa->setDrawBipolar(true);

          SAT_KnobWidget* osc1_inputn = new SAT_KnobWidget(SAT_Rect(323,6,20,20),"N",0.5);
          AEditor->connect(osc1_inputn,getParameter(SA_SYNTH_PARAM_OSC1_IN_N));
          osc1_text->appendChild(osc1_inputn);
          osc1_inputn->setKnobArcBackgroundColor(SAT_Grey);
          osc1_inputn->setKnobArcColor(SAT_White);
          osc1_inputn->setKnobArcThickness(3);
          osc1_inputn->setDrawText(true);
          osc1_inputn->setTextSize(5);
          osc1_inputn->setTextColor(SAT_LightGrey);
          osc1_inputn->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          osc1_inputn->setDrawValueText(false);
          //osc1_inputn->setDrawBipolar(true);

        SAT_KnobWidget* osc1_squ = new SAT_KnobWidget(SAT_Rect(5,35,45,45),"Sq",0);
        AEditor->connect(osc1_squ,getParameter(SA_SYNTH_PARAM_OSC1_SQU));
        osc1_panel->appendChild(osc1_squ);
        osc1_squ->setTextSize(5);

        SAT_KnobWidget* osc1_tri = new SAT_KnobWidget(SAT_Rect(55,35,45,45),"Tr",0);
        AEditor->connect(osc1_tri,getParameter(SA_SYNTH_PARAM_OSC1_TRI));
        osc1_panel->appendChild(osc1_tri);
        osc1_tri->setTextSize(5);

        SAT_KnobWidget* osc1_sin = new SAT_KnobWidget(SAT_Rect(105,35,45,45),"Si",0);
        AEditor->connect(osc1_sin,getParameter(SA_SYNTH_PARAM_OSC1_SIN));
        osc1_panel->appendChild(osc1_sin);
        osc1_sin->setTextSize(5);

        SAT_KnobWidget* osc1_width = new SAT_KnobWidget(SAT_Rect(155,35,45,45),"Wi",0.5);
        AEditor->connect(osc1_width,getParameter(SA_SYNTH_PARAM_OSC1_WIDTH));
        osc1_panel->appendChild(osc1_width);
        osc1_width->setDrawBipolar(true);
        osc1_width->setTextSize(5);

        SAT_DragValueWidget* osc1_oct = new SAT_DragValueWidget(SAT_Rect(210+70,35,60,12),"Oct",0);
        AEditor->connect(osc1_oct,getParameter(SA_SYNTH_PARAM_OSC1_OCT));
        osc1_panel->appendChild(osc1_oct);
        osc1_oct->setTextSize(7);
        osc1_oct->setValueTextSize(8);

        SAT_DragValueWidget* osc1_semi = new SAT_DragValueWidget(SAT_Rect(210+70,50,60,12),"Semi",0);
        AEditor->connect(osc1_semi,getParameter(SA_SYNTH_PARAM_OSC1_SEMI));
        osc1_panel->appendChild(osc1_semi);
        osc1_semi->setTextSize(7);
        osc1_semi->setValueTextSize(8);

        SAT_DragValueWidget* osc1_cent = new SAT_DragValueWidget(SAT_Rect(210+70,65,60,12),"Cent",0);
        AEditor->connect(osc1_cent,getParameter(SA_SYNTH_PARAM_OSC1_CENT));
        osc1_panel->appendChild(osc1_cent);
        osc1_cent->setTextSize(7);
        osc1_cent->setValueTextSize(8);

      //------------------------------
      // osc.2
      //------------------------------

      SAT_VisualWidget* osc2_panel = new SAT_VisualWidget(SAT_Rect(10,105,350,85));
      main_panel->appendChild(osc2_panel);
      osc2_panel->setFillBackground(false);
      osc2_panel->setDrawBorder(true);
      osc2_panel->setBorderColor(SAT_DarkGrey);
      osc2_panel->setBorderWidth(1);

        SAT_TextWidget* osc2_text = new SAT_TextWidget(30,"Osc 2");
        osc2_panel->appendChild(osc2_text);
        osc2_text->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
        osc2_text->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
        //osc2_text->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
        osc2_text->setDrawBorder(false);
        osc2_text->setFillBackground(true);
        osc2_text->setBackgroundColor(SAT_DarkerGrey);
        osc2_text->setTextColor(SAT_LighterGrey);
        osc2_text->setTextAlignment(SAT_TEXT_ALIGN_TOP | SAT_TEXT_ALIGN_LEFT);
        osc2_text->setTextOffset(SAT_Rect(5,5,0,0));

          SAT_KnobWidget* osc2_in_amt = new SAT_KnobWidget(SAT_Rect(117,3,26,26),"Amt",0.0);
          AEditor->connect(osc2_in_amt,getParameter(SA_SYNTH_PARAM_OSC2_IN_AMOUNT));
          osc2_text->appendChild(osc2_in_amt);
          osc2_in_amt->setKnobArcBackgroundColor(SAT_Grey);
          osc2_in_amt->setKnobArcColor(SAT_White);
          osc2_in_amt->setKnobArcThickness(4);
          osc2_in_amt->setDrawText(true);
          osc2_in_amt->setTextSize(5);
          osc2_in_amt->setTextColor(SAT_LightGrey);
          osc2_in_amt->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          osc2_in_amt->setDrawValueText(false);

          SAT_SelectorWidget* osc2_type = new SAT_SelectorWidget(SAT_Rect(148,9,60,12),"Phase Mod",osc2_type_menu);
          AEditor->connect(osc2_type,getParameter(SA_SYNTH_PARAM_OSC2_TYPE));
          osc2_text->appendChild(osc2_type);
          osc2_type->setFillBackground(true);
          osc2_type->setFillGradient(false);
          osc2_type->setBackgroundColor(0.4);
          osc2_type->setDrawBorder(false);
          osc2_type->setTextSize(8);
          osc2_type->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
          osc2_type->setTextOffset(SAT_Rect(2,0,0,0));
          osc2_type->setDrawParamText(false);
          osc2_type->select(0);
          osc2_type_menu->setListener(osc2_type);

          SAT_KnobWidget* osc2_input1 = new SAT_KnobWidget(SAT_Rect(213,6,20,20),"O1",0);
          AEditor->connect(osc2_input1,getParameter(SA_SYNTH_PARAM_OSC2_IN_O1));
          osc2_text->appendChild(osc2_input1);
          osc2_input1->setKnobArcBackgroundColor(SAT_Grey);
          osc2_input1->setKnobArcColor(SAT_White);
          osc2_input1->setKnobArcThickness(3);
          osc2_input1->setDrawText(true);
          osc2_input1->setTextSize(5);
          osc2_input1->setTextColor(SAT_LightGrey);
          osc2_input1->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          osc2_input1->setDrawValueText(false);
          //osc2_input1->setDrawBipolar(true);

          SAT_KnobWidget* osc2_input2 = new SAT_KnobWidget(SAT_Rect(235,6,20,20),"O2",0);
          AEditor->connect(osc2_input2,getParameter(SA_SYNTH_PARAM_OSC2_IN_O2));
          osc2_text->appendChild(osc2_input2);
          osc2_input2->setKnobArcBackgroundColor(SAT_Grey);
          osc2_input2->setKnobArcColor(SAT_White);
          osc2_input2->setKnobArcThickness(3);
          osc2_input2->setDrawText(true);
          osc2_input2->setTextSize(5);
          osc2_input2->setTextColor(SAT_LightGrey);
          osc2_input2->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          osc2_input2->setDrawValueText(false);
          //osc2_input2->setDrawBipolar(true);

          SAT_KnobWidget* osc2_input3 = new SAT_KnobWidget(SAT_Rect(257,6,20,20),"R1",0);
          AEditor->connect(osc2_input3,getParameter(SA_SYNTH_PARAM_OSC2_IN_R1));
          osc2_text->appendChild(osc2_input3);
          osc2_input3->setKnobArcBackgroundColor(SAT_Grey);
          osc2_input3->setKnobArcColor(SAT_White);
          osc2_input3->setKnobArcThickness(3);
          osc2_input3->setDrawText(true);
          osc2_input3->setTextSize(5);
          osc2_input3->setTextColor(SAT_LightGrey);
          osc2_input3->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          osc2_input3->setDrawValueText(false);
          //osc2_input3->setDrawBipolar(true);

          SAT_KnobWidget* osc2_input4 = new SAT_KnobWidget(SAT_Rect(279,6,20,20),"R2",0);
          AEditor->connect(osc2_input4,getParameter(SA_SYNTH_PARAM_OSC2_IN_R2));
          osc2_text->appendChild(osc2_input4);
          osc2_input4->setKnobArcBackgroundColor(SAT_Grey);
          osc2_input4->setKnobArcColor(SAT_White);
          osc2_input4->setKnobArcThickness(3);
          osc2_input4->setDrawText(true);
          osc2_input4->setTextSize(5);
          osc2_input4->setTextColor(SAT_LightGrey);
          osc2_input4->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          osc2_input4->setDrawValueText(false);
          //osc2_input4->setDrawBipolar(true);

          SAT_KnobWidget* osc2_inputa = new SAT_KnobWidget(SAT_Rect(301,6,20,20),"A",0.5);
          AEditor->connect(osc2_inputa,getParameter(SA_SYNTH_PARAM_OSC2_IN_A));
          osc2_text->appendChild(osc2_inputa);
          osc2_inputa->setKnobArcBackgroundColor(SAT_Grey);
          osc2_inputa->setKnobArcColor(SAT_White);
          osc2_inputa->setKnobArcThickness(3);
          osc2_inputa->setDrawText(true);
          osc2_inputa->setTextSize(5);
          osc2_inputa->setTextColor(SAT_LightGrey);
          osc2_inputa->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          osc2_inputa->setDrawValueText(false);
          //osc2_inputa->setDrawBipolar(true);

          SAT_KnobWidget* osc2_inputn = new SAT_KnobWidget(SAT_Rect(323,6,20,20),"N",0.5);
          AEditor->connect(osc2_inputn,getParameter(SA_SYNTH_PARAM_OSC2_IN_N));
          osc2_text->appendChild(osc2_inputn);
          osc2_inputn->setKnobArcBackgroundColor(SAT_Grey);
          osc2_inputn->setKnobArcColor(SAT_White);
          osc2_inputn->setKnobArcThickness(3);
          osc2_inputn->setDrawText(true);
          osc2_inputn->setTextSize(5);
          osc2_inputn->setTextColor(SAT_LightGrey);
          osc2_inputn->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          osc2_inputn->setDrawValueText(false);
          //osc2_inputn->setDrawBipolar(true);

        SAT_KnobWidget* osc2_squ = new SAT_KnobWidget(SAT_Rect(5,35,45,45),"Sq",0);
        AEditor->connect(osc2_squ,getParameter(SA_SYNTH_PARAM_OSC2_SQU));
        osc2_panel->appendChild(osc2_squ);
        osc2_squ->setTextSize(5);

        SAT_KnobWidget* osc2_tri = new SAT_KnobWidget(SAT_Rect(55,35,45,45),"Tr",0);
        AEditor->connect(osc2_tri,getParameter(SA_SYNTH_PARAM_OSC2_TRI));
        osc2_panel->appendChild(osc2_tri);
        osc2_tri->setTextSize(5);

        SAT_KnobWidget* osc2_sin = new SAT_KnobWidget(SAT_Rect(105,35,45,45),"Si",0);
        AEditor->connect(osc2_sin,getParameter(SA_SYNTH_PARAM_OSC2_SIN));
        osc2_panel->appendChild(osc2_sin);
        osc2_sin->setTextSize(5);

        SAT_KnobWidget* osc2_width = new SAT_KnobWidget(SAT_Rect(155,35,45,45),"Wi",0.5);
        AEditor->connect(osc2_width,getParameter(SA_SYNTH_PARAM_OSC2_WIDTH));
        osc2_panel->appendChild(osc2_width);
        osc2_width->setDrawBipolar(true);
        osc2_width->setTextSize(5);

        SAT_DragValueWidget* osc2_oct = new SAT_DragValueWidget(SAT_Rect(210,35,60,12),"Oct",0);
        AEditor->connect(osc2_oct,getParameter(SA_SYNTH_PARAM_OSC2_OCT));
        osc2_panel->appendChild(osc2_oct);
        osc2_oct->setTextSize(7);
        osc2_oct->setValueTextSize(8);

        SAT_DragValueWidget* osc2_semi = new SAT_DragValueWidget(SAT_Rect(210,50,60,12),"Semi",0);
        AEditor->connect(osc2_semi,getParameter(SA_SYNTH_PARAM_OSC2_SEMI));
        osc2_panel->appendChild(osc2_semi);
        osc2_semi->setTextSize(7);
        osc2_semi->setValueTextSize(8);

        SAT_DragValueWidget* osc2_cent = new SAT_DragValueWidget(SAT_Rect(210,65,60,12),"Cent",0);
        AEditor->connect(osc2_cent,getParameter(SA_SYNTH_PARAM_OSC2_CENT));
        osc2_panel->appendChild(osc2_cent);
        osc2_cent->setTextSize(7);
        osc2_cent->setValueTextSize(8);

      //------------------------------
      // res.1
      //------------------------------

      SAT_VisualWidget* res1_panel = new SAT_VisualWidget(SAT_Rect(370,10,350,85));
      main_panel->appendChild(res1_panel);
      res1_panel->setFillBackground(false);
      res1_panel->setDrawBorder(true);
      res1_panel->setBorderColor(SAT_DarkGrey);
      res1_panel->setBorderWidth(1);

        SAT_TextWidget* res1_text = new SAT_TextWidget(30,"Res 1");
        res1_panel->appendChild(res1_text);
        res1_text->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
        res1_text->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
        res1_text->setDrawBorder(false);
        res1_text->setFillBackground(true);
        res1_text->setBackgroundColor(SAT_DarkerGrey);
        res1_text->setTextColor(SAT_LighterGrey);
        res1_text->setTextAlignment(SAT_TEXT_ALIGN_TOP | SAT_TEXT_ALIGN_LEFT);
        res1_text->setTextOffset(SAT_Rect(5,5,0,0));

          SAT_KnobWidget* res1_in_amt = new SAT_KnobWidget(SAT_Rect(117,3,26,26),"Amt",0.0);
          AEditor->connect(res1_in_amt,getParameter(SA_SYNTH_PARAM_RES1_IN_AMOUNT));
          res1_text->appendChild(res1_in_amt);
          res1_in_amt->setKnobArcBackgroundColor(SAT_Grey);
          res1_in_amt->setKnobArcColor(SAT_White);
          res1_in_amt->setKnobArcThickness(4);
          res1_in_amt->setDrawText(true);
          res1_in_amt->setTextSize(5);
          res1_in_amt->setTextColor(SAT_LightGrey);
          res1_in_amt->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          res1_in_amt->setDrawValueText(false);

          SAT_SelectorWidget* res1_type = new SAT_SelectorWidget(SAT_Rect(148,9,60,12),"---",res1_type_menu);
          AEditor->connect(res1_type,getParameter(SA_SYNTH_PARAM_RES1_TYPE));
          res1_text->appendChild(res1_type);
          res1_type->setFillBackground(true);
          res1_type->setFillGradient(false);
          res1_type->setBackgroundColor(0.4);
          res1_type->setDrawBorder(false);
          res1_type->setTextSize(8);
          res1_type->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
          res1_type->setTextOffset(SAT_Rect(2,0,0,0));
          res1_type->setDrawParamText(false);
          res1_type->select(0);
          res1_type_menu->setListener(res1_type);

          SAT_KnobWidget* res1_input1 = new SAT_KnobWidget(SAT_Rect(213,6,20,20),"O1",0);
          AEditor->connect(res1_input1,getParameter(SA_SYNTH_PARAM_RES1_IN_O1));
          res1_text->appendChild(res1_input1);
          res1_input1->setKnobArcBackgroundColor(SAT_Grey);
          res1_input1->setKnobArcColor(SAT_White);
          res1_input1->setKnobArcThickness(3);
          res1_input1->setDrawText(true);
          res1_input1->setTextSize(5);
          res1_input1->setTextColor(SAT_LightGrey);
          res1_input1->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          res1_input1->setDrawValueText(false);
          //res1_input1->setDrawBipolar(true);

          SAT_KnobWidget* res1_input2 = new SAT_KnobWidget(SAT_Rect(236,6,20,20),"O2",0);
          AEditor->connect(res1_input2,getParameter(SA_SYNTH_PARAM_RES1_IN_O2));
          res1_text->appendChild(res1_input2);
          res1_input2->setKnobArcBackgroundColor(SAT_Grey);
          res1_input2->setKnobArcColor(SAT_White);
          res1_input2->setKnobArcThickness(3);
          res1_input2->setDrawText(true);
          res1_input2->setTextSize(5);
          res1_input2->setTextColor(SAT_LightGrey);
          res1_input2->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          res1_input2->setDrawValueText(false);
          //res1_input2->setDrawBipolar(true);

          SAT_KnobWidget* res1_input3 = new SAT_KnobWidget(SAT_Rect(257,6,20,20),"R1",0);
          AEditor->connect(res1_input3,getParameter(SA_SYNTH_PARAM_RES1_IN_R1));
          res1_text->appendChild(res1_input3);
          res1_input3->setKnobArcBackgroundColor(SAT_Grey);
          res1_input3->setKnobArcColor(SAT_White);
          res1_input3->setKnobArcThickness(3);
          res1_input3->setDrawText(true);
          res1_input3->setTextSize(5);
          res1_input3->setTextColor(SAT_LightGrey);
          res1_input3->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          res1_input3->setDrawValueText(false);
          //res1_input3->setDrawBipolar(true);

          SAT_KnobWidget* res1_input4 = new SAT_KnobWidget(SAT_Rect(279,6,20,20),"R2",0);
          AEditor->connect(res1_input4,getParameter(SA_SYNTH_PARAM_RES1_IN_R2));
          res1_text->appendChild(res1_input4);
          res1_input4->setKnobArcBackgroundColor(SAT_Grey);
          res1_input4->setKnobArcColor(SAT_White);
          res1_input4->setKnobArcThickness(3);
          res1_input4->setDrawText(true);
          res1_input4->setTextSize(5);
          res1_input4->setTextColor(SAT_LightGrey);
          res1_input4->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          res1_input4->setDrawValueText(false);
          //res1_input4->setDrawBipolar(true);

          SAT_KnobWidget* res1_inputa = new SAT_KnobWidget(SAT_Rect(301,6,20,20),"A",0.5);
          AEditor->connect(res1_inputa,getParameter(SA_SYNTH_PARAM_RES1_IN_A));
          res1_text->appendChild(res1_inputa);
          res1_inputa->setKnobArcBackgroundColor(SAT_Grey);
          res1_inputa->setKnobArcColor(SAT_White);
          res1_inputa->setKnobArcThickness(3);
          res1_inputa->setDrawText(true);
          res1_inputa->setTextSize(5);
          res1_inputa->setTextColor(SAT_LightGrey);
          res1_inputa->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          res1_inputa->setDrawValueText(false);
          //res1_inputa->setDrawBipolar(true);

          SAT_KnobWidget* res1_inputn = new SAT_KnobWidget(SAT_Rect(323,6,20,20),"N",0.5);
          AEditor->connect(res1_inputn,getParameter(SA_SYNTH_PARAM_RES1_IN_N));
          res1_text->appendChild(res1_inputn);
          res1_inputn->setKnobArcBackgroundColor(SAT_Grey);
          res1_inputn->setKnobArcColor(SAT_White);
          res1_inputn->setKnobArcThickness(3);
          res1_inputn->setDrawText(true);
          res1_inputn->setTextSize(5);
          res1_inputn->setTextColor(SAT_LightGrey);
          res1_inputn->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          res1_inputn->setDrawValueText(false);
          //res1_inputn->setDrawBipolar(true);

        SAT_KnobWidget* res1_impulse_freq = new SAT_KnobWidget(SAT_Rect(5,35,45,45),"Im",0);
        AEditor->connect(res1_impulse_freq,getParameter(SA_SYNTH_PARAM_RES1_IMPULSE_FREQ));
        res1_panel->appendChild(res1_impulse_freq);
        res1_impulse_freq->setTextSize(5);

        SAT_KnobWidget* res1_fb = new SAT_KnobWidget(SAT_Rect(115,35,45,45),"Fb",0);
        AEditor->connect(res1_fb,getParameter(SA_SYNTH_PARAM_RES1_FEEDBACK));
        res1_panel->appendChild(res1_fb);
        res1_fb->setTextSize(5);

        SAT_KnobWidget* res1_damp = new SAT_KnobWidget(SAT_Rect(165,35,45,45),"Da",0);
        AEditor->connect(res1_damp,getParameter(SA_SYNTH_PARAM_RES1_DAMPING_FREQ));
        res1_panel->appendChild(res1_damp);
        res1_damp->setTextSize(5);

        SAT_KnobWidget* res1_gain = new SAT_KnobWidget(SAT_Rect(215,35,45,45),"G",0);
        AEditor->connect(res1_gain,getParameter(SA_SYNTH_PARAM_RES1_GAIN));
        res1_panel->appendChild(res1_gain);
        res1_gain->setTextSize(5);

        SAT_DragValueWidget* res1_oct = new SAT_DragValueWidget(SAT_Rect(280,35,60,12),"Oct",0);
        AEditor->connect(res1_oct,getParameter(SA_SYNTH_PARAM_RES1_OCT));
        res1_panel->appendChild(res1_oct);
        res1_oct->setTextSize(7);
        res1_oct->setValueTextSize(8);

        SAT_DragValueWidget* res1_semi = new SAT_DragValueWidget(SAT_Rect(280,50,60,12),"Semi",0);
        AEditor->connect(res1_semi,getParameter(SA_SYNTH_PARAM_RES1_SEMI));
        res1_panel->appendChild(res1_semi);
        res1_semi->setTextSize(7);
        res1_semi->setValueTextSize(8);

        SAT_DragValueWidget* res1_cent = new SAT_DragValueWidget(SAT_Rect(280,65,60,12),"Cent",0);
        AEditor->connect(res1_cent,getParameter(SA_SYNTH_PARAM_RES1_CENT));
        res1_panel->appendChild(res1_cent);
        res1_cent->setTextSize(7);
        res1_cent->setValueTextSize(8);

      //------------------------------
      // res.2
      //------------------------------

      SAT_VisualWidget* res2_panel = new SAT_VisualWidget(SAT_Rect(370,106,350,85));
      main_panel->appendChild(res2_panel);
      res2_panel->setFillBackground(false);
      res2_panel->setDrawBorder(true);
      res2_panel->setBorderColor(SAT_DarkGrey);
      res2_panel->setBorderWidth(1);

        SAT_TextWidget* res2_text = new SAT_TextWidget(30,"Res 2");
        res2_panel->appendChild(res2_text);
        res2_text->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
        res2_text->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
        res2_text->setDrawBorder(false);
        res2_text->setFillBackground(true);
        res2_text->setBackgroundColor(SAT_DarkerGrey);
        res2_text->setTextColor(SAT_LighterGrey);
        res2_text->setTextAlignment(SAT_TEXT_ALIGN_TOP | SAT_TEXT_ALIGN_LEFT);
        res2_text->setTextOffset(SAT_Rect(5,5,0,0));

          SAT_KnobWidget* res2_in_amt = new SAT_KnobWidget(SAT_Rect(117,3,26,26),"Amt",0.0);
          AEditor->connect(res2_in_amt,getParameter(SA_SYNTH_PARAM_RES2_IN_AMOUNT));
          res2_text->appendChild(res2_in_amt);
          res2_in_amt->setKnobArcBackgroundColor(SAT_Grey);
          res2_in_amt->setKnobArcColor(SAT_White);
          res2_in_amt->setKnobArcThickness(4);
          res2_in_amt->setDrawText(true);
          res2_in_amt->setTextSize(5);
          res2_in_amt->setTextColor(SAT_LightGrey);
          res2_in_amt->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          res2_in_amt->setDrawValueText(false);

          SAT_SelectorWidget* res2_type = new SAT_SelectorWidget(SAT_Rect(148,9,60,12),"---",res2_type_menu);
          AEditor->connect(res2_type,getParameter(SA_SYNTH_PARAM_RES2_TYPE));
          res2_text->appendChild(res2_type);
          res2_type->setFillBackground(true);
          res2_type->setFillGradient(false);
          res2_type->setBackgroundColor(0.4);
          res2_type->setDrawBorder(false);
          res2_type->setTextSize(8);
          res2_type->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
          res2_type->setTextOffset(SAT_Rect(2,0,0,0));
          res2_type->setDrawParamText(false);
          res2_type->select(0);
          res2_type_menu->setListener(res2_type);

          SAT_KnobWidget* res2_input1 = new SAT_KnobWidget(SAT_Rect(213,6,20,20),"O1",0);
          AEditor->connect(res2_input1,getParameter(SA_SYNTH_PARAM_RES2_IN_O1));
          res2_text->appendChild(res2_input1);
          res2_input1->setKnobArcBackgroundColor(SAT_Grey);
          res2_input1->setKnobArcColor(SAT_White);
          res2_input1->setKnobArcThickness(3);
          res2_input1->setDrawText(true);
          res2_input1->setTextSize(5);
          res2_input1->setTextColor(SAT_LightGrey);
          res2_input1->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          res2_input1->setDrawValueText(false);
          //res2_input1->setDrawBipolar(true);

          SAT_KnobWidget* res2_input2 = new SAT_KnobWidget(SAT_Rect(235,6,20,20),"O2",0);
          AEditor->connect(res2_input2,getParameter(SA_SYNTH_PARAM_RES2_IN_O2));
          res2_text->appendChild(res2_input2);
          res2_input2->setKnobArcBackgroundColor(SAT_Grey);
          res2_input2->setKnobArcColor(SAT_White);
          res2_input2->setKnobArcThickness(3);
          res2_input2->setDrawText(true);
          res2_input2->setTextSize(5);
          res2_input2->setTextColor(SAT_LightGrey);
          res2_input2->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          res2_input2->setDrawValueText(false);
          //res2_input2->setDrawBipolar(true);

          SAT_KnobWidget* res2_input3 = new SAT_KnobWidget(SAT_Rect(257,6,20,20),"R1",0);
          AEditor->connect(res2_input3,getParameter(SA_SYNTH_PARAM_RES2_IN_R1));
          res2_text->appendChild(res2_input3);
          res2_input3->setKnobArcBackgroundColor(SAT_Grey);
          res2_input3->setKnobArcColor(SAT_White);
          res2_input3->setKnobArcThickness(3);
          res2_input3->setDrawText(true);
          res2_input3->setTextSize(5);
          res2_input3->setTextColor(SAT_LightGrey);
          res2_input3->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          res2_input3->setDrawValueText(false);
          //res2_input3->setDrawBipolar(true);

          SAT_KnobWidget* res2_input4 = new SAT_KnobWidget(SAT_Rect(279,6,20,20),"R2",0);
          AEditor->connect(res2_input4,getParameter(SA_SYNTH_PARAM_RES2_IN_R2));
          res2_text->appendChild(res2_input4);
          res2_input4->setKnobArcBackgroundColor(SAT_Grey);
          res2_input4->setKnobArcColor(SAT_White);
          res2_input4->setKnobArcThickness(3);
          res2_input4->setDrawText(true);
          res2_input4->setTextSize(5);
          res2_input4->setTextColor(SAT_LightGrey);
          res2_input4->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          res2_input4->setDrawValueText(false);
          //res2_input4->setDrawBipolar(true);

          SAT_KnobWidget* res2_inputa = new SAT_KnobWidget(SAT_Rect(301,6,20,20),"A",0.5);
          AEditor->connect(res2_inputa,getParameter(SA_SYNTH_PARAM_RES2_IN_A));
          res2_text->appendChild(res2_inputa);
          res2_inputa->setKnobArcBackgroundColor(SAT_Grey);
          res2_inputa->setKnobArcColor(SAT_White);
          res2_inputa->setKnobArcThickness(3);
          res2_inputa->setDrawText(true);
          res2_inputa->setTextSize(5);
          res2_inputa->setTextColor(SAT_LightGrey);
          res2_inputa->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          res2_inputa->setDrawValueText(false);
          //res2_inputa->setDrawBipolar(true);

          SAT_KnobWidget* res2_inputn = new SAT_KnobWidget(SAT_Rect(323,6,20,20),"N",0.5);
          AEditor->connect(res2_inputn,getParameter(SA_SYNTH_PARAM_RES2_IN_N));
          res2_text->appendChild(res2_inputn);
          res2_inputn->setKnobArcBackgroundColor(SAT_Grey);
          res2_inputn->setKnobArcColor(SAT_White);
          res2_inputn->setKnobArcThickness(3);
          res2_inputn->setDrawText(true);
          res2_inputn->setTextSize(5);
          res2_inputn->setTextColor(SAT_LightGrey);
          res2_inputn->setTextAlignment(SAT_TEXT_ALIGN_CENTER);
          res2_inputn->setDrawValueText(false);
          //res2_inputn->setDrawBipolar(true);

        SAT_KnobWidget* res2_impulse_freq = new SAT_KnobWidget(SAT_Rect(5,35,45,45),"Im",0);
        AEditor->connect(res2_impulse_freq,getParameter(SA_SYNTH_PARAM_RES2_IMPULSE_FREQ));
        res2_panel->appendChild(res2_impulse_freq);
        res2_impulse_freq->setTextSize(5);

        SAT_KnobWidget* res2_fb = new SAT_KnobWidget(SAT_Rect(115,35,45,45),"Fb",0);
        AEditor->connect(res2_fb,getParameter(SA_SYNTH_PARAM_RES2_FEEDBACK));
        res2_panel->appendChild(res2_fb);
        res2_fb->setTextSize(5);

        SAT_KnobWidget* res2_damp = new SAT_KnobWidget(SAT_Rect(165,35,45,45),"Da",0);
        AEditor->connect(res2_damp,getParameter(SA_SYNTH_PARAM_RES2_DAMPING_FREQ));
        res2_panel->appendChild(res2_damp);
        res2_damp->setTextSize(5);

        SAT_KnobWidget* res2_gain = new SAT_KnobWidget(SAT_Rect(215,35,45,45),"G",0);
        AEditor->connect(res2_gain,getParameter(SA_SYNTH_PARAM_RES2_GAIN));
        res2_panel->appendChild(res2_gain);
        res2_gain->setTextSize(5);

        SAT_DragValueWidget* res2_oct = new SAT_DragValueWidget(SAT_Rect(280,35,60,12),"Oct",0);
        AEditor->connect(res2_oct,getParameter(SA_SYNTH_PARAM_RES2_OCT));
        res2_panel->appendChild(res2_oct);
        res2_oct->setTextSize(7);
        res2_oct->setValueTextSize(8);

        SAT_DragValueWidget* res2_semi = new SAT_DragValueWidget(SAT_Rect(280,50,60,12),"Semi",0);
        AEditor->connect(res2_semi,getParameter(SA_SYNTH_PARAM_RES2_SEMI));
        res2_panel->appendChild(res2_semi);
        res2_semi->setTextSize(7);
        res2_semi->setValueTextSize(8);

        SAT_DragValueWidget* res2_cent = new SAT_DragValueWidget(SAT_Rect(280,65,60,12),"Cent",0);
        AEditor->connect(res2_cent,getParameter(SA_SYNTH_PARAM_RES2_CENT));
        res2_panel->appendChild(res2_cent);
        res2_cent->setTextSize(7);
        res2_cent->setValueTextSize(8);

      //------------------------------
      // mix
      //------------------------------

      SAT_VisualWidget* mix_panel = new SAT_VisualWidget(SAT_Rect(10,180+20,205,75));
      main_panel->appendChild(mix_panel);
      mix_panel->setFillBackground(false);
      mix_panel->setDrawBorder(true);
      mix_panel->setBorderColor(SAT_DarkGrey);
      mix_panel->setBorderWidth(1);

        SAT_TextWidget* mix_text = new SAT_TextWidget(20,"Mix");
        mix_panel->appendChild(mix_text);
        mix_text->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
        mix_text->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
        mix_text->setDrawBorder(false);
        mix_text->setFillBackground(true);
        mix_text->setBackgroundColor(SAT_DarkerGrey);
        mix_text->setTextColor(SAT_LighterGrey);
        mix_text->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
        mix_text->setTextOffset(SAT_Rect(5,0,0,0));

        SAT_KnobWidget* mix_o1 = new SAT_KnobWidget(SAT_Rect(5,25,45,45),"O1",1);
        AEditor->connect(mix_o1,getParameter(SA_SYNTH_PARAM_MIX_O1));
        mix_panel->appendChild(mix_o1);
        mix_o1->setTextSize(5);

        SAT_KnobWidget* mix_o2 = new SAT_KnobWidget(SAT_Rect(55,25,45,45),"O2",0);
        AEditor->connect(mix_o2,getParameter(SA_SYNTH_PARAM_MIX_O2));
        mix_panel->appendChild(mix_o2);
        mix_o2->setTextSize(5);

        SAT_KnobWidget* mix_r1 = new SAT_KnobWidget(SAT_Rect(105,25,45,45),"R1",0);
        AEditor->connect(mix_r1,getParameter(SA_SYNTH_PARAM_MIX_R1));
        mix_panel->appendChild(mix_r1);
        mix_r1->setTextSize(5);

        SAT_KnobWidget* mix_r2 = new SAT_KnobWidget(SAT_Rect(155,25,45,45),"R2",0);
        AEditor->connect(mix_r2,getParameter(SA_SYNTH_PARAM_MIX_R2));
        mix_panel->appendChild(mix_r2);
        mix_r2->setTextSize(5);

      //------------------------------
      // flt
      //------------------------------

      SAT_VisualWidget* flt1_panel = new SAT_VisualWidget(SAT_Rect(225,180+20,155,75));
      main_panel->appendChild(flt1_panel);
      flt1_panel->setFillBackground(false);
      flt1_panel->setDrawBorder(true);
      flt1_panel->setBorderColor(SAT_DarkGrey);
      flt1_panel->setBorderWidth(1);

        SAT_TextWidget* flt1_text = new SAT_TextWidget(20,"Flt");
        flt1_panel->appendChild(flt1_text);
        flt1_text->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
        flt1_text->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
        flt1_text->setDrawBorder(false);
        flt1_text->setFillBackground(true);
        flt1_text->setBackgroundColor(SAT_DarkerGrey);
        flt1_text->setTextColor(SAT_LighterGrey);
        flt1_text->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
        flt1_text->setTextOffset(SAT_Rect(5,0,0,0));

          SAT_SelectorWidget* flt1_type = new SAT_SelectorWidget(SAT_Rect(90,4,60,12),"---",flt1_type_menu);
          AEditor->connect(flt1_type,getParameter(SA_SYNTH_PARAM_FLT1_TYPE));
          flt1_text->appendChild(flt1_type);
          flt1_type->setFillBackground(true);
          flt1_type->setFillGradient(false);
          flt1_type->setBackgroundColor(0.4);
          flt1_type->setDrawBorder(false);
          flt1_type->setTextSize(8);
          flt1_type->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
          flt1_type->setTextOffset(SAT_Rect(2,0,0,0));
          flt1_type->setDrawParamText(false);
          flt1_type->select(0);
          flt1_type_menu->setListener(flt1_type);

        SAT_KnobWidget* flt1_freq = new SAT_KnobWidget(SAT_Rect(5,25,45,45),"Fr",0.75);
        AEditor->connect(flt1_freq,getParameter(SA_SYNTH_PARAM_FLT1_FREQ));
        flt1_panel->appendChild(flt1_freq);
        flt1_freq->setTextSize(5);

        SAT_KnobWidget* flt1_bw = new SAT_KnobWidget(SAT_Rect(55,25,45,45),"Q",0.25);
        AEditor->connect(flt1_bw,getParameter(SA_SYNTH_PARAM_FLT1_Q));
        flt1_panel->appendChild(flt1_bw);
        flt1_bw->setTextSize(5);

        SAT_KnobWidget* flt1_gain = new SAT_KnobWidget(SAT_Rect(105,25,45,45),"G",0.5);
        AEditor->connect(flt1_gain,getParameter(SA_SYNTH_PARAM_FLT1_GAIN));
        flt1_panel->appendChild(flt1_gain);
        flt1_gain->setDrawBipolar(true);
        flt1_gain->setTextSize(5);

      //------------------------------
      // env
      //------------------------------

      SAT_VisualWidget* env1_panel = new SAT_VisualWidget(SAT_Rect(390,180+20,205,75));
      main_panel->appendChild(env1_panel);
      env1_panel->setFillBackground(false);
      env1_panel->setDrawBorder(true);
      env1_panel->setBorderColor(SAT_DarkGrey);
      env1_panel->setBorderWidth(1);

        SAT_TextWidget* env1_text = new SAT_TextWidget(20,"Env");
        env1_panel->appendChild(env1_text);
        env1_text->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
        env1_text->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
        env1_text->setDrawBorder(false);
        env1_text->setFillBackground(true);
        env1_text->setBackgroundColor(SAT_DarkerGrey);
        env1_text->setTextColor(SAT_LighterGrey);
        env1_text->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
        env1_text->setTextOffset(SAT_Rect(5,0,0,0));

        SAT_KnobWidget* env1_a = new SAT_KnobWidget(SAT_Rect(5,25,45,45),"A",0.01);
        AEditor->connect(env1_a,getParameter(SA_SYNTH_PARAM_ENV1_ATT));
        env1_panel->appendChild(env1_a);
        env1_a->setTextSize(5);

        SAT_KnobWidget* env1_d = new SAT_KnobWidget(SAT_Rect(55,25,45,45),"D",0);
        AEditor->connect(env1_d,getParameter(SA_SYNTH_PARAM_ENV1_DEC));
        env1_panel->appendChild(env1_d);
        env1_d->setTextSize(5);

        SAT_KnobWidget* env1_s = new SAT_KnobWidget(SAT_Rect(105,25,45,45),"S",1);
        AEditor->connect(env1_s,getParameter(SA_SYNTH_PARAM_ENV1_SUS));
        env1_panel->appendChild(env1_s);
        env1_s->setTextSize(5);

        SAT_KnobWidget* env1_r = new SAT_KnobWidget(SAT_Rect(155,25,45,45),"R",0.3);
        AEditor->connect(env1_r,getParameter(SA_SYNTH_PARAM_ENV1_REL));
        env1_panel->appendChild(env1_r);
        env1_r->setTextSize(5);

      //------------------------------
      // effects
      //------------------------------

      SAT_VisualWidget* fx1_panel = new SAT_VisualWidget(SAT_Rect(10,285,205,75));
      main_panel->appendChild(fx1_panel);
      fx1_panel->setFillBackground(false);
      fx1_panel->setDrawBorder(true);
      fx1_panel->setBorderColor(SAT_DarkGrey);
      fx1_panel->setBorderWidth(1);

        SAT_TextWidget* fx1_text = new SAT_TextWidget(20,"FX");
        fx1_panel->appendChild(fx1_text);
        fx1_text->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
        fx1_text->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
        fx1_text->setDrawBorder(false);
        fx1_text->setFillBackground(true);
        fx1_text->setBackgroundColor(SAT_DarkerGrey);
        fx1_text->setTextColor(SAT_LighterGrey);
        fx1_text->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
        fx1_text->setTextOffset(SAT_Rect(5,0,0,0));

        SAT_KnobWidget* fx1_reverb = new SAT_KnobWidget(SAT_Rect(5,25,45,45),"Reverb");
        fx1_panel->appendChild(fx1_reverb);
        fx1_reverb->setTextSize(5);

        SAT_KnobWidget* fx1_delay = new SAT_KnobWidget(SAT_Rect(55,25,45,45),"Delay");
        fx1_panel->appendChild(fx1_delay);
        fx1_delay->setTextSize(5);

        SAT_KnobWidget* fx1_chorus = new SAT_KnobWidget(SAT_Rect(105,25,45,45),"Chorus");
        fx1_panel->appendChild(fx1_chorus);
        fx1_chorus->setTextSize(5);

        SAT_KnobWidget* fx1_etc = new SAT_KnobWidget(SAT_Rect(155,25,45,45),"etc..");
        fx1_panel->appendChild(fx1_etc);
        fx1_etc->setTextSize(5);

      //------------------------------
      // global
      //------------------------------

      SAT_VisualWidget* global_panel = new SAT_VisualWidget(SAT_Rect(225,285,55,75));
      main_panel->appendChild(global_panel);
      global_panel->setFillBackground(false);
      global_panel->setDrawBorder(true);
      global_panel->setBorderColor(SAT_DarkGrey);
      global_panel->setBorderWidth(1);

        SAT_TextWidget* global_text = new SAT_TextWidget(20,"Global");
        global_panel->appendChild(global_text);
        global_text->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP_LEFT;
        global_text->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
        global_text->setDrawBorder(false);
        global_text->setFillBackground(true);
        global_text->setBackgroundColor(SAT_DarkerGrey);
        global_text->setTextColor(SAT_LighterGrey);
        global_text->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
        global_text->setTextOffset(SAT_Rect(5,0,0,0));

        SAT_KnobWidget* global_gain = new SAT_KnobWidget(SAT_Rect(5,25,45,45),"",0.5);
        AEditor->connect(global_gain,getParameter(SA_SYNTH_PARAM_GLOBAL_GAIN));
        global_panel->appendChild(global_gain);
        global_gain->setTextSize(5);

      //------------------------------
      // voices
      //------------------------------

      MVoicesWidget = new SAT_VoicesWidget(SAT_Rect(290,285,200,20),SA_SYNTH_MAX_VOICES);
      main_panel->appendChild(MVoicesWidget);



    }
    return true;

  }

//------------------------------
public:
//------------------------------

  bool setupOverlay(SAT_Editor* AEditor) final {
    SAT_Window* window = AEditor->getWindow();
    if (window) {
      SAT_Widget* overlay = window->getOverlayWidget();
      if (overlay) {
        overlay->appendChild(osc1_type_menu);
        overlay->appendChild(osc2_type_menu);
        overlay->appendChild(res1_type_menu);
        overlay->appendChild(res2_type_menu);
        overlay->appendChild(flt1_type_menu);
        return true;
      }
    }
    return false;
  }

//----------------------------------------------------------------------
#endif