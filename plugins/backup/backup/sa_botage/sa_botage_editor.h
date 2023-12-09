#ifndef sa_botage_editor_included
#define sa_botage_editor_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widgets.h"
//#include "plugin/sat_editor.h"

#define BACKGROUNDCOLOR 0.2

#define WAVEFORM_LINES  (360 * 3)
#define WAVEFORM_COLOR  SAT_Color( 1,1,1 )
#define WAVEFORM_WIDTH  1.0

const char* prob_slices_text[] = {
  "1", "2", "3", "4", "5", "6", "7", "8"
};

//----------------------------------------------------------------------

class sa_botage_MenuWidget
: public SAT_MenuWidget {
public:
  sa_botage_MenuWidget(uint32_t AWidth, uint32_t AHeight, const char** AText, uint32_t ACount)
  : SAT_MenuWidget(SAT_Rect(0,0,AWidth,ACount*AHeight)) {
    setFillBackground(true);
    for (uint32_t i=0; i<ACount; i++) {
      SAT_MenuItemWidget* menuitem = new SAT_MenuItemWidget( SAT_Rect(0, i*AHeight, AWidth, AHeight ), AText[i] );
      appendChildWidget(menuitem);
      menuitem->setDrawBorder(false);
      menuitem->setFillBackground(true);
      menuitem->setTextSize(8);
      menuitem->setTextSize(9);
      menuitem->setTextColor(0);
      menuitem->setBackgroundColor(0.5);
    }
  }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_botage_editor
: public SAT_Editor {

  friend class sa_botage_plugin;

//------------------------------
private:
//------------------------------

  SAT_ParameterArray*         MParameters       = nullptr;

  SAT_PanelWidget*            MRootWidget       = nullptr;
  SAT_CircularWaveformWidget* MWaveformWidget   = nullptr;
  SAT_DragValueWidget*        MNumBeatsWidget   = nullptr;
  SAT_DragValueWidget*        MNumSlicesWidget  = nullptr;
  SAT_TextWidget*             MHintWidget       = nullptr;

  SAT_PagesWidget*            MPages            = nullptr;

  SAT_ButtonWidget*           prob_page_button  = nullptr;
  SAT_ButtonWidget*           seq_page_button   = nullptr;
  SAT_ButtonWidget*           perf_page_button  = nullptr;

  uint32_t                    MCurrentPage = 0;

  //

  SAT_Color                   MReadPosColor   = SAT_Color( 0.0, 1.0, 0.0, 0.8 );
  SAT_Color                   MWritePosColor  = SAT_Color( 1.0, 0.0, 0.0, 1.0 );

  double                      MReadPosWidth   = 2.0;
  double                      MWritePosWidth  = 2.5;

  SAT_Color                   MRangeColor     = SAT_Color( 0.0, 0.4, 0.0, 0.5 );
  SAT_Color                   MLoopColor      = SAT_Color( 0.2, 0.6, 0.2, 0.7 );
  SAT_Color                   MSliceColor     = SAT_Color( 0.4, 0.4, 0.4, 0.5 );

  //

  SAT_KnobWidget*             MMainProbWidget           = nullptr;

  SAT_ButtonRowWidget*        MMainSlicesWidget         = nullptr;
  SAT_ButtonRowWidget*        MMainLoopsWidget          = nullptr;

  SAT_KnobWidget*             MRangeSizeWidget          = nullptr;
  SAT_KnobWidget*             MRangeSpeedWidget         = nullptr;
  SAT_KnobWidget*             MRangeOffsetWidget        = nullptr;
  SAT_KnobWidget*             MRangeReverseWidget       = nullptr;
  SAT_KnobWidget*             MRangeFXWidget            = nullptr;

  SAT_KnobWidget*             MLoopSizeWidget           = nullptr;
  SAT_KnobWidget*             MLoopSpeedWidget          = nullptr;
  SAT_KnobWidget*             MLoopOffsetWidget         = nullptr;
  SAT_KnobWidget*             MLoopReverseWidget        = nullptr;
  SAT_KnobWidget*             MLoopFXWidget             = nullptr;

  SAT_DualSliderWidget*       MRangeSizeValueWidget     = nullptr;
  SAT_DualSliderWidget*       MRangeSpeedValueWidget    = nullptr;
  SAT_DualSliderWidget*       MRangeOffsetValueWidget   = nullptr;
  SAT_DualSliderWidget*       MRangeFXValueWidget       = nullptr;

  SAT_DualSliderWidget*       MLoopSizeValueWidget      = nullptr;
  SAT_DualSliderWidget*       MLoopSpeedValueWidget     = nullptr;
  SAT_DualSliderWidget*       MLoopOffsetValueWidget    = nullptr;
  SAT_DualSliderWidget*       MLoopFXValueWidget        = nullptr;

  SAT_SelectorWidget*         MFXTypeSelector[5]        = {0};
  SAT_KnobWidget*             MFXProbKnob[5]            = {0};
  SAT_KnobWidget*             MFXArgKnob[5*3]           = {0};

//------------------------------
public:
//------------------------------

  //sa_botage_editor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, SAT_ParameterArray* AParameters, const clap_plugin_descriptor_t* descriptor)
  sa_botage_editor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, SAT_ParameterArray* AParameters)
  : SAT_Editor(AListener,AWidth,AHeight) { // ,AParameters) {
    MParameters = AParameters;
  }

  //----------

  //virtual ~sa_botage_editor() {
  //  if (MRootWidget) delete MRootWidget;
  //}

//------------------------------
public:
//------------------------------

  bool initEditorWindow(SAT_Window* AWindow/*, SAT_Plugin* APlugin*/) {
    SAT_Window* window = AWindow; // !!

    // root

    MRootWidget = new SAT_PanelWidget(SAT_Rect(0,0,SA_BOTAGE_EDITOR_WIDTH,SA_BOTAGE_EDITOR_HEIGHT));
    window->appendRootWidget(MRootWidget);
    MRootWidget->setFillBackground(true);
    MRootWidget->setBackgroundColor(BACKGROUNDCOLOR);
    MRootWidget->setDrawBorder(false);
    MRootWidget->setBorderColor(0.5);

    // header
    
    //const char* format = APlugin->getPluginFormat();
    SAT_PluginHeaderWidget* plugin_header = new SAT_PluginHeaderWidget(SAT_Rect(0,0,SA_BOTAGE_EDITOR_WIDTH,40),"botage",""/*format*/);
    MRootWidget->appendChildWidget(plugin_header);
    
    // footer
    
    SAT_PluginFooterWidget* plugin_footer = new SAT_PluginFooterWidget(SAT_Rect(0,0,SA_BOTAGE_EDITOR_WIDTH,20));
    MRootWidget->appendChildWidget(plugin_footer);    
    
    // hint

    //    MHintWidget = new SAT_TextWidget(SAT_Rect(210,25,245,10),"Hello world!");
    //    MRootWidget->appendChildWidget(MHintWidget);
    //    MHintWidget->setFillBackground(true);
    //    MHintWidget->setBackgroundColor(0.23);
    //    MHintWidget->setDrawBorder(false);
    //    MHintWidget->setBorderColor(SAT_Grey);
    //    MHintWidget->setTextSize(9);
    //    MHintWidget->setTextColor(0.8);
    //    MHintWidget->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    //    MHintWidget->setTextOffset(SAT_Rect(5,0,0,0));

    // c

    SAT_TextWidget* c_icon = new SAT_TextWidget(SAT_Rect(475,65,10,10),"c");
    MRootWidget->appendChildWidget(c_icon);
    c_icon->setCursor(SAT_CURSOR_FINGER);
    c_icon->setTextSize(9);
    c_icon->setTextColor(0.8);
    c_icon->setDrawBorder(false);
    c_icon->setBorderColor(0.5);
    c_icon->setFillBackground(true);
    c_icon->setBackgroundColor(0.2);
//    c_icon->setHint("Config");

    // i

    SAT_TextWidget* i_icon = new SAT_TextWidget(SAT_Rect(460,65,10,10),"i");
    MRootWidget->appendChildWidget(i_icon);
    i_icon->setCursor(SAT_CURSOR_FINGER);
    i_icon->setTextSize(9);
    i_icon->setTextColor(0.8);
    i_icon->setDrawBorder(false);
    i_icon->setBorderColor(0.5);
    i_icon->setFillBackground(true);
    i_icon->setBackgroundColor(0.2);
//    i_icon->setHint("Info");

    // pages

    prob_page_button = new SAT_ButtonWidget(SAT_Rect(510,10,30,10));
    MRootWidget->appendChildWidget(prob_page_button);
    prob_page_button->setText("PROB","Prob");
    //prob_page_button->setHint("Probabilities");
    prob_page_button->setFillBackground(true);
    prob_page_button->setBackgroundColor(0.3);
    prob_page_button->setDrawBorder(false);
    prob_page_button->setTextSize(8);
    prob_page_button->setTextColor(0.6);

    seq_page_button = new SAT_ButtonWidget(SAT_Rect(545,10,30,10));
    MRootWidget->appendChildWidget(seq_page_button);
    seq_page_button->setText("SEQ","Seq");
    //seq_page_button->setHint("Sequence");
    seq_page_button->setFillBackground(true);
    seq_page_button->setBackgroundColor(0.25);
    seq_page_button->setDrawBorder(false);
    seq_page_button->setTextSize(8);
    seq_page_button->setTextColor(0.6);

    perf_page_button = new SAT_ButtonWidget(SAT_Rect(580,10,30,10));
    MRootWidget->appendChildWidget(perf_page_button);
    perf_page_button->setText("PERF","Perf");
    //perf_page_button->setHint("Perform");
    perf_page_button->setFillBackground(true);
    perf_page_button->setBackgroundColor(0.25);
    perf_page_button->setDrawBorder(false);
    perf_page_button->setTextSize(8);
    perf_page_button->setTextColor(0.6);

    MPages = new SAT_PagesWidget( SAT_Rect(255,30,357,292));
    MRootWidget->appendChildWidget(MPages);

//    #include "sa_botage_editor_page_prob.h"
//    #include "sa_botage_editor_page_seq.h"
//    #include "sa_botage_editor_page_perf.h"
//
//    MPages->appendPage(page_prob);
//    MPages->appendPage(page_seq);
//    MPages->appendPage(page_perf);
//
//    MPages->setPage(0);

    //------------------------------
    // waveform / buffer
    //------------------------------

    MWaveformWidget = new SAT_CircularWaveformWidget( SAT_Rect(10,40,230,230), WAVEFORM_LINES );
    MRootWidget->appendChildWidget(MWaveformWidget);
    MWaveformWidget->setGrid(4,2);
    MWaveformWidget->setFillBackground(false);;
    MWaveformWidget->setWaveformWidth(WAVEFORM_WIDTH);
    MWaveformWidget->setGridColor(0.55, 0.4);
    MWaveformWidget->setZeroLineColor(0.6);
    MWaveformWidget->setWaveformColor( WAVEFORM_COLOR );

    // beats

    MNumBeatsWidget = new SAT_DragValueWidget( SAT_Rect(10,280,110,16),"Beats",0);
    MRootWidget->appendChildWidget(MNumBeatsWidget);
    MNumBeatsWidget->setFillBackground(true);
    MNumBeatsWidget->setBackgroundColor( 0.5 );
    MNumBeatsWidget->setTextColor(SAT_Black);
    MNumBeatsWidget->setTextSize(11);
    MNumBeatsWidget->setValueColor(SAT_Black);
    MNumBeatsWidget->setValueSize(11);
    //MNumBeatsWidget->setHint("Number of beats in buffer");
    //MNumBeatsWidget->setInteractiveValueColor(SAT_White);

    //connect( MNumBeatsWidget, AParameters->getItem(PAR_NUM_BEATS) );
//    connect( MNumBeatsWidget, MParameters[PAR_NUM_BEATS] );

    // slices

    MNumSlicesWidget = new SAT_DragValueWidget( SAT_Rect(130,280,110,16),"Split Beats",0);
    MRootWidget->appendChildWidget(MNumSlicesWidget);
    MNumSlicesWidget->setFillBackground(true);
    MNumSlicesWidget->setBackgroundColor( 0.5 );
    MNumSlicesWidget->setTextColor(SAT_Black);
    MNumSlicesWidget->setTextSize(11);
    MNumSlicesWidget->setValueColor(SAT_Black);
    MNumSlicesWidget->setValueSize(11);
    //MNumSlicesWidget->setHint("Number of slices per beat");
    //MNumSlicesWidget->setInteractiveValueColor(SAT_COLOR_WHITE);

//    connect( MNumSlicesWidget, MParameters[PAR_NUM_SLICES] );

    // loop env

    //    SAT_EnvSliderWidget* env1_slider = new SAT_EnvSliderWidget( SAT_Rect(10,356,110,16), "Loop", 0.1, 0.9 );
    //    MRootWidget->appendChildWidget(env1_slider);
    //    env1_slider->setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
    //    env1_slider->setHint("Loop Envelope");
    //    env1_slider->setBackgroundColor(0.4);
    //    env1_slider->setSliderColor(0.6);
    //    env1_slider->setTextColor(SAT_Color(0,0,0,0.5));
    //    env1_slider->setTextSize(9);
    //    env1_slider->setValueColor(0.2);
    //    env1_slider->setBackgroundColor( SAT_Color(0.0, 0.5, 0.5) );
    //    env1_slider->setSliderColor( SAT_Color(0.0, 0.7, 0.7) );
    //    env1_slider->setInteractiveSliderColor( SAT_Color(0.0, 0.9, 0.9) );
    //
    //    connect( env1_slider, 0, AParameters->getItem(PAR_LOOP_ENV_ATTACK) );
    //    connect( env1_slider, 1, AParameters->getItem(PAR_LOOP_ENV_DECAY) );

    SAT_SliderWidget* env1a_slider = new SAT_SliderWidget( SAT_Rect(10,306,50,16), "Loop Start", 0.1 );
    MRootWidget->appendChildWidget(env1a_slider);
    env1a_slider->setTextColor(SAT_Color(0,0,0,0.5));
    env1a_slider->setTextSize(9);
    env1a_slider->setDrawValue(false);
    env1a_slider->setValueColor(0.2);
    env1a_slider->setBackgroundColor( SAT_Color(0.0, 0.5, 0.5) );
    env1a_slider->setSliderBarColor( SAT_Color(0.0, 0.7, 0.7) );
    //env1a_slider->setInteractiveSliderColor( SAT_Color(0.0, 0.9, 0.9) );

//    connect( env1a_slider, APlugin->getParameter(PAR_LOOP_ENV_ATTACK) );

    SAT_SliderWidget* env1d_slider = new SAT_SliderWidget( SAT_Rect(10+60,306,50,16), "Loop End", 0.9 );
    MRootWidget->appendChildWidget(env1d_slider);
    env1d_slider->setTextColor(SAT_Color(0,0,0,0.5));
    env1d_slider->setTextSize(9);
    env1d_slider->setDrawValue(false);
    env1d_slider->setValueColor(0.2);
    env1d_slider->setBackgroundColor( SAT_Color(0.0, 0.5, 0.5) );
    env1d_slider->setSliderBarColor( SAT_Color(0.0, 0.7, 0.7) );
    //env1d_slider->setInteractiveSliderColor( SAT_Color(0.0, 0.9, 0.9) );

//    connect( env1d_slider, APlugin->getParameter(PAR_LOOP_ENV_DECAY) );

    // slice env

    //    SAT_EnvSliderWidget* env2_slider = new SAT_EnvSliderWidget( SAT_Rect(130,356,110,16), "Slice", 0.1, 0.9 );
    //    MRootWidget->appendChildWidget(env2_slider);
    //    env2_slider->setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
    //    env2_slider->setHint("Slice envelope");
    //    env2_slider->setBackgroundColor(0.4);
    //    env2_slider->setSliderColor(0.6);
    //    env2_slider->setTextColor(SAT_Color(0,0,0,0.5));
    //    env2_slider->setTextSize(9);
    //    env2_slider->setValueColor(0.2);
    //    env2_slider->setBackgroundColor( SAT_Color(0.0, 0.5, 0.5) );
    //    env2_slider->setSliderColor( SAT_Color(0.0, 0.7, 0.7) );
    //    env2_slider->setInteractiveSliderColor( SAT_Color(0.0, 0.9, 0.9) );
    //
    //    connect( env2_slider, 0, AParameters->getItem(PAR_SLICE_ENV_ATTACK) );
    //    connect( env2_slider, 1, AParameters->getItem(PAR_SLICE_ENV_DECAY) );

    SAT_SliderWidget* env2a_slider = new SAT_SliderWidget( SAT_Rect(130,306,50,16), "Slice Start", 0.1 );
    MRootWidget->appendChildWidget(env2a_slider);
    env2a_slider->setTextColor(SAT_Color(0,0,0,0.5));
    env2a_slider->setTextSize(9);
    env2a_slider->setDrawValue(false);
    env2a_slider->setValueColor(0.2);
    env2a_slider->setBackgroundColor( SAT_Color(0.0, 0.5, 0.5) );
    env2a_slider->setSliderBarColor( SAT_Color(0.0, 0.7, 0.7) );
    //env2a_slider->setInteractiveSliderColor( SAT_Color(0.0, 0.9, 0.9) );

//    connect( env2a_slider, APlugin->getParameter(PAR_SLICE_ENV_ATTACK) );

    SAT_SliderWidget* env2d_slider = new SAT_SliderWidget( SAT_Rect(130+60,306,50,16), "Slice End", 0.9 );
    MRootWidget->appendChildWidget(env2d_slider);
    env2d_slider->setTextColor(SAT_Color(0,0,0,0.5));
    env2d_slider->setTextSize(9);
    env2d_slider->setDrawValue(false);
    env2d_slider->setValueColor(0.2);
    env2d_slider->setBackgroundColor( SAT_Color(0.0, 0.5, 0.5) );
    env2d_slider->setSliderBarColor( SAT_Color(0.0, 0.7, 0.7) );
    //env2d_slider->setInteractiveSliderColor( SAT_Color(0.0, 0.9, 0.9) );

//    connect( env2d_slider, APlugin->getParameter(PAR_SLICE_ENV_DECAY) );

    // footer

    //double height = ARootWidget->getHeight();
    //sa_botage_footer* footer  = new sa_botage_footer(SAT_Rect(0,(height - SA_BOTAGE_FOOTER_HEIGHT),width,SA_BOTAGE_FOOTER_HEIGHT));
    //ARootWidget->appendChildWidget(footer);

    // menu

//    MRootWidget->appendChildWidget(fx_type_menu);

    return true;
  }


    

#if 0
#endif // 0


//------------------------------
public:
//------------------------------

  bool show() override {
//    uint32_t beats = MParameters->getItem(0)->getValue();
//    uint32_t slices = MParameters->getItem(1)->getValue();
//    MWaveformWidget->setGrid(beats,slices);
//    //double scale = 1.0;
//    //double aspect = (double)MEditorWidth / (double)MEditorHeight;
//    //if (aspect >= MAspectRatio) scale = (double)MEditorHeight / (double)MInitialHeight;
//    //else scale = (double)MEditorWidth / (double)MInitialWidth;
//    //MWindow->setWindowScale(scale);
    return SAT_Editor::show();
  }

  //----------

  //void do_widgetListener_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex) override {
  void do_windowListener_update_widget(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex) override {

    SAT_Editor::do_windowListener_update_widget(ASender,AMode,AIndex);

//    if (AWidget == MNumBeatsWidget) {
//      double value = AWidget->getValue();
//      MWaveformWidget->setGrid(value);
//      MWaveformWidget->redraw();
//      return;
//    }
//
//    if (AWidget == MNumSlicesWidget) {
//      double value = AWidget->getValue();
//      MWaveformWidget->setSubGrid(value);
//      MWaveformWidget->redraw();
//      return;
//    }
//
//    if (AWidget == prob_page_button) {
//      MCurrentPage = 0;
//      MPages->setPage(0);
//      MPages->redraw();
//      prob_page_button->setBackgroundColor(0.3);
//      seq_page_button->setBackgroundColor(0.25);
//      perf_page_button->setBackgroundColor(0.25);
//      prob_page_button->redraw();
//      seq_page_button->redraw();
//      perf_page_button->redraw();
//      return;
//    }
//
//    if (AWidget == seq_page_button)  {
//      MCurrentPage = 1;
//      MPages->setPage(2); MPages->redraw();
//      prob_page_button->setBackgroundColor(0.25);
//      seq_page_button->setBackgroundColor(0.3);
//      perf_page_button->setBackgroundColor(0.25);
//      prob_page_button->redraw();
//      seq_page_button->redraw();
//      perf_page_button->redraw();
//      return;
//    }
//
//    if (AWidget == perf_page_button) {
//      MCurrentPage = 2;
//      MPages->setPage(3); MPages->redraw();
//      prob_page_button->setBackgroundColor(0.25);
//      seq_page_button->setBackgroundColor(0.25);
//      perf_page_button->setBackgroundColor(0.3);
//      prob_page_button->redraw();
//      seq_page_button->redraw();
//      perf_page_button->redraw();
//      return;
//    }
//
//    for (uint32_t i=0; i<5; i++) {
//      if (AWidget == MFXTypeSelector[i]) {
//        uint32_t it = MFXTypeSelector[i]->getValue();
//        for (uint32_t j=0; j<5; j++) {
//          if (i != j) {
//            // i=clicked, j=other
//            uint32_t jt = MFXTypeSelector[j]->getValue();
//            if (it == jt) {
//              //SAT_Print("swap effect %i <-> %i\n",i,j);
//
//              if (it != 0) {
//
//                MFXTypeSelector[j]->setSelected(0);
//                MFXTypeSelector[j]->update();
//                MFXTypeSelector[j]->redraw();
//
//                //double iv1 = 0;//MFXArgKnob[(i*3)  ]->getValue();
//                //double iv2 = 0;//MFXArgKnob[(i*3)+1]->getValue();
//                //double iv3 = 0;//MFXArgKnob[(i*3)+2]->getValue();
//                double jv1 = MFXArgKnob[(j*3)  ]->getValue();
//                double jv2 = MFXArgKnob[(j*3)+1]->getValue();
//                double jv3 = MFXArgKnob[(j*3)+2]->getValue();
//
//                MFXArgKnob[(i*3)  ]->setValue(jv1);
//                MFXArgKnob[(i*3)+1]->setValue(jv2);
//                MFXArgKnob[(i*3)+2]->setValue(jv3);
//                MFXArgKnob[(i*3)  ]->update();
//                MFXArgKnob[(i*3)+1]->update();
//                MFXArgKnob[(i*3)+2]->update();
//                MFXArgKnob[(i*3)  ]->redraw();
//                MFXArgKnob[(i*3)+1]->redraw();
//                MFXArgKnob[(i*3)+2]->redraw();
//
//                //MFXArgKnob[(j*3)+0]->setValue(iv1);
//                //MFXArgKnob[(j*3)+1]->setValue(iv2);
//                //MFXArgKnob[(j*3)+2]->setValue(iv3);
//                //MFXArgKnob[(j*3)  ]->update();
//                //MFXArgKnob[(j*3)+1]->update();
//                //MFXArgKnob[(j*3)+2]->update();
//                //MFXArgKnob[(j*3)  ]->redraw();
//                //MFXArgKnob[(j*3)+1]->redraw();
//                //MFXArgKnob[(j*3)+2]->redraw();
//
//              } // it!=0
//
//
//            } // it==jt
//          } // i"=j
//        } // for j
//      } // AWidget
//    } // for i

  }

  //----------

//  void do_windowListener_set_hint(SAT_Widget* AWidget, const char* AHint) override {
//    if (strcmp(AHint,MHintWidget->getText()) != 0) {
//      MHintWidget->setText(AHint);
//      MHintWidget->redraw();
//    }
//    SAT_Editor::do_windowListener_set_hint(AWidget,AHint);
//  }

//------------------------------
public:
//------------------------------

  void updateWaveformWidget(sa_botage_processor* processor) {
    
#if 0

    uint32_t buflen = processor->MBufferLength;
    if (buflen > 0) {

      MWaveformWidget->setAudioBuffer(buflen, processor->MLeftBuffer, false);
      MWaveformWidget->setGrid(processor->par_num_beats, processor->par_num_slices);

      // marker 0 write pos

      MWaveformWidget->setMarkerActive(0, true);
      MWaveformWidget->setMarkerPos(   0, (double)processor->MWritePos/buflen, 0.0025);
      MWaveformWidget->setMarkerColor( 0, MWritePosColor);
      //MWaveformWidget->setMarkerWidth( 0, MWritePosWidth);

      // marker 1 read pos

      MWaveformWidget->setMarkerActive(1, true);
      MWaveformWidget->setMarkerPos(   1, processor->MReadPos/buflen, 0.0025);
      MWaveformWidget->setMarkerColor( 1, MReadPosColor);
      //MWaveformWidget->setMarkerWidth( 1,MReadPosWidth);

      if (processor->MRange) {

        // area 1 range

        MWaveformWidget->setAreaActive(0, false);
        MWaveformWidget->setAreaActive(1, true);
        MWaveformWidget->setAreaPos(   1, processor->MRangeStart/buflen, processor->MRangeLength/buflen);
        MWaveformWidget->setAreaColor( 1, MRangeColor);

        if (processor->MLoop) {

          // area 2 loop

          MWaveformWidget->setAreaActive(2, true);
          MWaveformWidget->setAreaPos(   2, processor->MLoopStart/buflen, processor->MLoopLength/buflen);
          MWaveformWidget->setAreaColor( 2, MLoopColor);

        }
        else {
          MWaveformWidget->setAreaActive(2, false);
        }
      }

      else { // no range
        MWaveformWidget->setAreaActive(1, false);
        MWaveformWidget->setAreaActive(2, false);

        // area 0 current slice

        if (processor->MIsPlaying) {
          MWaveformWidget->setAreaActive(0, true);
          MWaveformWidget->setAreaPos(   0, processor->MSliceStart/buflen, processor->MSliceLength/buflen);
          MWaveformWidget->setAreaColor( 0, MSliceColor);
        }
        else {
          MWaveformWidget->setAreaActive(0, false);
        }

      } // range

      MWaveformWidget->redraw();

    } // buflen

#endif // 0

  }

  //----------

  void updateProbIndicators(sa_botage_processor* processor) {
    
#if 0

    if (MCurrentPage == 0) {

      MMainProbWidget->setIndicatorValue(processor->rnd_main_prob);
      if (processor->MRange) {

        //MMainProbWidget->setIndicatorValue(processor->rnd_main_prob);
        MMainSlicesWidget->setIndicatorValue(processor->rnd_main_slices);
        MMainLoopsWidget->setIndicatorValue(processor->rnd_main_subdiv);

        MRangeSizeWidget->setIndicatorValue(processor->rnd_range_size);
        MRangeSpeedWidget->setIndicatorValue(processor->rnd_range_speed);
        MRangeOffsetWidget->setIndicatorValue(processor->rnd_range_offset);
        MRangeReverseWidget->setIndicatorValue(processor->rnd_range_reverse);
        MRangeFXWidget->setIndicatorValue(processor->rnd_range_fx);

        if (processor->rnd_range_size_on) MRangeSizeValueWidget->setIndicatorValue(processor->rnd_range_size_value);
        else MRangeSizeValueWidget->setIndicatorValue(-1);
        if (processor->rnd_range_speed_on) MRangeSpeedValueWidget->setIndicatorValue(processor->rnd_range_speed_value);
        else MRangeSpeedValueWidget->setIndicatorValue(-1);
        if (processor->rnd_range_offset_on) MRangeOffsetValueWidget->setIndicatorValue(processor->rnd_range_offset_value);
        else MRangeOffsetValueWidget->setIndicatorValue(-1);
        if (processor->rnd_range_fx_on) MRangeFXValueWidget->setIndicatorValue(processor->rnd_range_fx_value);
        else MRangeFXValueWidget->setIndicatorValue(-1);

//------------------------------

        if (processor->rnd_range_fx_on) {
          MFXProbKnob[0]->setIndicatorValue(processor->rnd_fx1);
          MFXProbKnob[1]->setIndicatorValue(processor->rnd_fx2);
          MFXProbKnob[2]->setIndicatorValue(processor->rnd_fx3);
          MFXProbKnob[3]->setIndicatorValue(processor->rnd_fx4);
          MFXProbKnob[4]->setIndicatorValue(processor->rnd_fx5);
        }
        else {
          MFXProbKnob[0]->setIndicatorValue(-1);
          MFXProbKnob[1]->setIndicatorValue(-1);
          MFXProbKnob[2]->setIndicatorValue(-1);
          MFXProbKnob[3]->setIndicatorValue(-1);
          MFXProbKnob[4]->setIndicatorValue(-1);
        }

//        if (processor->rnd_fx1_on) MFXProbKnob[0]->setIndicatorValue(processor->rnd_fx1);
//        else MFXProbKnob[0]->setIndicatorValue(-1);
//        if (processor->rnd_fx2_on) MFXProbKnob[1]->setIndicatorValue(processor->rnd_fx2);
//        else MFXProbKnob[1]->setIndicatorValue(-1);
//        if (processor->rnd_fx3_on) MFXProbKnob[2]->setIndicatorValue(processor->rnd_fx3);
//        else MFXProbKnob[2]->setIndicatorValue(-1);
//        if (processor->rnd_fx4_on) MFXProbKnob[3]->setIndicatorValue(processor->rnd_fx4);
//        else MFXProbKnob[3]->setIndicatorValue(-1);
//        if (processor->rnd_fx5_on) MFXProbKnob[4]->setIndicatorValue(processor->rnd_fx5);
//        else MFXProbKnob[4]->setIndicatorValue(-1);

//------------------------------

        if (processor->MLoopWrapped) {
          MLoopSizeWidget->setIndicatorValue(processor->rnd_loop_size);
          MLoopSpeedWidget->setIndicatorValue(processor->rnd_loop_speed);
          MLoopOffsetWidget->setIndicatorValue(processor->rnd_loop_offset);
          MLoopReverseWidget->setIndicatorValue(processor->rnd_loop_reverse);
          MLoopFXWidget->setIndicatorValue(processor->rnd_loop_fx);

          if (processor->rnd_loop_size_on) MLoopSizeValueWidget->setIndicatorValue(processor->rnd_loop_size_value);
          else MLoopSizeValueWidget->setIndicatorValue(-1);
          if (processor->rnd_loop_speed_on) MLoopSpeedValueWidget->setIndicatorValue(processor->rnd_loop_speed_value);
          else MLoopSpeedValueWidget->setIndicatorValue(-1);
          if (processor->rnd_loop_offset_on) MLoopOffsetValueWidget->setIndicatorValue(processor->rnd_loop_offset_value);
          else MLoopOffsetValueWidget->setIndicatorValue(-1);
          if (processor->rnd_loop_fx_on) MLoopFXValueWidget->setIndicatorValue(processor->rnd_loop_fx_value);
          else MLoopFXValueWidget->setIndicatorValue(-1);
        }
        else {
          MLoopSizeWidget->setIndicatorValue(-1);
          MLoopSpeedWidget->setIndicatorValue(-1);
          MLoopOffsetWidget->setIndicatorValue(-1);
          MLoopReverseWidget->setIndicatorValue(-1);
          MLoopFXWidget->setIndicatorValue(-1);

          MLoopSizeValueWidget->setIndicatorValue(-1);
          MLoopSpeedValueWidget->setIndicatorValue(-1);
          MLoopOffsetValueWidget->setIndicatorValue(-1);
          MLoopFXValueWidget->setIndicatorValue(-1);
        }

      }
      else { // ! range

        //MMainProbWidget->setIndicatorValue(-1);
        MMainSlicesWidget->setIndicatorValue(-1);
        MMainLoopsWidget->setIndicatorValue(-1);

        MRangeSizeWidget->setIndicatorValue(-1);
        MRangeSpeedWidget->setIndicatorValue(-1);
        MRangeOffsetWidget->setIndicatorValue(-1);
        MRangeReverseWidget->setIndicatorValue(-1);
        MRangeFXWidget->setIndicatorValue(-1);

        MRangeSizeValueWidget->setIndicatorValue(-1);
        MRangeSpeedValueWidget->setIndicatorValue(-1);
        MRangeOffsetValueWidget->setIndicatorValue(-1);
        MRangeFXValueWidget->setIndicatorValue(-1);

        MLoopSizeWidget->setIndicatorValue(-1);
        MLoopSpeedWidget->setIndicatorValue(-1);
        MLoopOffsetWidget->setIndicatorValue(-1);
        MLoopReverseWidget->setIndicatorValue(-1);
        MLoopFXWidget->setIndicatorValue(-1);

        MLoopSizeValueWidget->setIndicatorValue(-1);
        MLoopSpeedValueWidget->setIndicatorValue(-1);
        MLoopOffsetValueWidget->setIndicatorValue(-1);
        MLoopFXValueWidget->setIndicatorValue(-1);

        MFXProbKnob[0]->setIndicatorValue(-1);
        MFXProbKnob[1]->setIndicatorValue(-1);
        MFXProbKnob[2]->setIndicatorValue(-1);
        MFXProbKnob[3]->setIndicatorValue(-1);
        MFXProbKnob[4]->setIndicatorValue(-1);

      }

    } // page == 0
    
#endif // 0    
    
  }

};

//----------------------------------------------------------------------
#endif
