#ifndef sa_tyr_editor_included
#define sa_tyr_editor_included
//----------------------------------------------------------------------

// we need some visual tool to do all of this..

#include "sat.h"
#include "gui/sat_widgets.h"
#include "plugin/sat_editor.h"

#include "sa_tyr_widgets.h"

//#define _PLUG_NAME  "tyr"
//#define _PLUG_VER   "0.0.9"

//#define HEADER_HEIGHT 80
#define HH 80

//----------------------------------------------------------------------

class sa_tyr_editor
: public SAT_Editor
/*, public SAT_MenuListener*/ {

//------------------------------
public:
//------------------------------

  sa_tyr_VoiceWidget* MVoiceWidget          = nullptr;
  SAT_ValueWidget*    MPlayingVoicesWidget  = nullptr;
  SAT_ValueWidget*    MReleasedVoicesWidget = nullptr;
  SAT_ValueWidget*    MTotalVoicesWidget    = nullptr;
  //SAT_KeyboardWidget* MKeyboards[11]        = {0};

//------------------------------
public:
//------------------------------

  sa_tyr_editor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, SAT_ParameterArray* AParameters, const clap_plugin_descriptor_t* descriptor)
  : SAT_Editor(AListener,AWidth,AHeight) { // ,AParameters) {

    //------------------------------
    // menus
    //------------------------------

    sa_tyr_MenuWidget* osc_type_menu = new sa_tyr_MenuWidget(160,15,sa_tyr_osc_type_text,SA_TYR_OSC_TYPE_COUNT);
    sa_tyr_MenuWidget* res_type_menu = new sa_tyr_MenuWidget(160,15,sa_tyr_res_type_text,SA_TYR_RES_TYPE_COUNT);
    sa_tyr_MenuWidget* flt_type_menu = new sa_tyr_MenuWidget(160,15,sa_tyr_flt_type_text,SA_TYR_FLT_TYPE_COUNT);
    sa_tyr_MenuWidget* phase_shaper_type_menu = new sa_tyr_MenuWidget(160,15,sa_tyr_pm_type_text,SA_TYR_PM_TYPE_COUNT);
    sa_tyr_MenuWidget* wave_shaper_type_menu = new sa_tyr_MenuWidget(160,15,sa_tyr_wm_type_text,SA_TYR_WM_TYPE_COUNT);

    //------------------------------
    //
    //------------------------------

    // panel

    SAT_PanelWidget* MEditorPanel = new SAT_PanelWidget(SAT_Rect(0,0,SA_TYR_EDITOR_WIDTH,SA_TYR_EDITOR_HEIGHT));
    setRootWidget(MEditorPanel);

    //    MEditorPanel->setDrawBorder(false);
    //    MEditorPanel->setFillBackground(false);

    // header

    SAT_SAHeaderWidget* header = new SAT_SAHeaderWidget(SAT_DRect(0,0,SA_TYR_EDITOR_WIDTH,HH),descriptor);
    MEditorPanel->appendChildWidget(header);
    //header->setPluginName(SA_TYR_PLUGIN_NAME);
    //header->setPluginVersion(SA_TYR_PLUGIN_VERSION);

    //#undef HEADER_HEAIGHT

//
//
// left --------------------------------------------------
//
//

// ----- osc1

      sa_tyr_SectionPanelWidget* osc1_panel = new sa_tyr_SectionPanelWidget(SAT_DRect(10,HH+10,260,300), "OSC1", 20 );
      MEditorPanel->appendChildWidget(osc1_panel);

        // type

        sa_tyr_SelectorWidget* osc1_type_selector = new sa_tyr_SelectorWidget( SAT_DRect(10+10,HH+30+10,240,20), "Osc Type", osc_type_menu );
        osc1_panel->appendChildWidget(osc1_type_selector);
        connect(osc1_type_selector, AParameters->getItem(PAR_OSC1_TYPE) );

        // shape

        sa_tyr_BigKnobWidget* osc1_shape_knob = new sa_tyr_BigKnobWidget( SAT_DRect(10+10,HH+60,50,82),"Shape");
        //osc1_shape_knob->getLabelWidget()->setTextSize(-0.5);
        osc1_panel->appendChildWidget(osc1_shape_knob);
        connect(osc1_shape_knob,AParameters->getItem(PAR_OSC1_SHAPE));

        // width

        sa_tyr_BigKnobWidget* osc1_width_knob = new sa_tyr_BigKnobWidget( SAT_DRect(70+10,HH+60,50,82),"Width", 0.0, true);
        osc1_panel->appendChildWidget(osc1_width_knob);
        connect(osc1_width_knob,AParameters->getItem(PAR_OSC1_WIDTH));

        // oct

        //sa_tyr_DragValueWidget* osc1_oct_slider = new sa_tyr_DragValueWidget( SAT_DRect(10+10,HH+150,72,20),"Oct");
        sa_tyr_DragValueWidget* osc1_oct_slider = new sa_tyr_DragValueWidget( SAT_DRect(10+10,HH+150,72,20),"Oct", 0.0, true);
        osc1_panel->appendChildWidget(osc1_oct_slider);
        connect(osc1_oct_slider,AParameters->getItem(PAR_OSC1_OCT));

        // semi

        sa_tyr_DragValueWidget* osc1_semi_slider = new sa_tyr_DragValueWidget( SAT_DRect(94+10,HH+150,72,20),"Semi", 0.0, true);
        osc1_panel->appendChildWidget(osc1_semi_slider);
        connect(osc1_semi_slider,AParameters->getItem(PAR_OSC1_SEMI));

        // cent

        sa_tyr_DragValueWidget* osc1_cent_slider = new sa_tyr_DragValueWidget( SAT_DRect(178+10,HH+150,72,20), "Cent", 0.0, true, 0.0, 0.1 );
        osc1_panel->appendChildWidget(osc1_cent_slider);
        connect(osc1_cent_slider,AParameters->getItem(PAR_OSC1_CENT));

        // inputs

        sa_tyr_SmallKnobWidget* osc1_in_o1 = new sa_tyr_SmallKnobWidget( SAT_DRect(10+10,HH+180,27,45),"O1");
        osc1_panel->appendChildWidget(osc1_in_o1);
        connect(osc1_in_o1,AParameters->getItem(PAR_OSC1_IN_O1));

        sa_tyr_SmallKnobWidget* osc1_in_o2 = new sa_tyr_SmallKnobWidget( SAT_DRect(40+10,HH+180,27,45),"O2");
        osc1_panel->appendChildWidget(osc1_in_o2);
        connect(osc1_in_o2,AParameters->getItem(PAR_OSC1_IN_O2));

        sa_tyr_SmallKnobWidget* osc1_in_r1 = new sa_tyr_SmallKnobWidget( SAT_DRect(70+10,HH+180,27,45),"R1");
        osc1_panel->appendChildWidget(osc1_in_r1);
        connect(osc1_in_r1,AParameters->getItem(PAR_OSC1_IN_R1));

        sa_tyr_SmallKnobWidget* osc1_in_r2 = new sa_tyr_SmallKnobWidget( SAT_DRect(100+10,HH+180,27,45),"R2");
        osc1_panel->appendChildWidget(osc1_in_r2);
        connect(osc1_in_r2,AParameters->getItem(PAR_OSC1_IN_R2));

        sa_tyr_SmallKnobWidget* osc1_in_n = new sa_tyr_SmallKnobWidget( SAT_DRect(130+10,HH+180,27,45),"N");
        osc1_panel->appendChildWidget(osc1_in_n);
        connect(osc1_in_n,AParameters->getItem(PAR_OSC1_IN_N));

        sa_tyr_SmallKnobWidget* osc1_in_a = new sa_tyr_SmallKnobWidget( SAT_DRect(160+10,HH+180,27,45),"A");
        osc1_panel->appendChildWidget(osc1_in_a);
        connect(osc1_in_a,AParameters->getItem(PAR_OSC1_IN_A));

        sa_tyr_SmallKnobWidget* osc1_in_i = new sa_tyr_SmallKnobWidget( SAT_DRect(190+10,HH+180,27,45),"I");
        osc1_panel->appendChildWidget(osc1_in_i);
        //osc1_in_i->getKnobWidget()->setSnap(true);
        //osc1_in_i->getKnobWidget()->setSnapPos(0.5);
        //osc1_in_i->getKnobWidget()->setSnapDist(0.05);
        connect(osc1_in_i,AParameters->getItem(PAR_OSC1_IN_I));

        sa_tyr_SmallKnobWidget* osc1_in_s = new sa_tyr_SmallKnobWidget( SAT_DRect(220+10,HH+180,27,45),"S");
        osc1_panel->appendChildWidget(osc1_in_s);
        connect(osc1_in_s,AParameters->getItem(PAR_OSC1_IN_S));

        // ps

        sa_tyr_SelectorWidget* osc1_phase_shaper_selector = new sa_tyr_SelectorWidget( SAT_DRect(10+10,HH+240,115,20),"Phase Shp",phase_shaper_type_menu );
        osc1_panel->appendChildWidget(osc1_phase_shaper_selector);
        connect(osc1_phase_shaper_selector,AParameters->getItem(PAR_OSC1_PM_TYPE));

        sa_tyr_SliderWidget* osc1_phase_shaper_slider = new sa_tyr_SliderWidget(SAT_DRect(135+10,HH+240,115,20),"Phase",0.0);//true);
        osc1_panel->appendChildWidget(osc1_phase_shaper_slider);
        connect(osc1_phase_shaper_slider,AParameters->getItem(PAR_OSC1_PM_AMOUNT));

        // ws

        sa_tyr_SelectorWidget* osc1_wave_shaper_selector = new sa_tyr_SelectorWidget( SAT_DRect(10+10,HH+270,115,20),"Wave Shp",wave_shaper_type_menu );
        osc1_panel->appendChildWidget(osc1_wave_shaper_selector);
        connect(osc1_wave_shaper_selector,AParameters->getItem(PAR_OSC1_WM_TYPE));

        sa_tyr_SliderWidget* osc1_wave_shaper_slider = new sa_tyr_SliderWidget(SAT_DRect(135+10,HH+270,115,20),"Wave",0.0);//,true);
        osc1_panel->appendChildWidget(osc1_wave_shaper_slider);
        connect(osc1_wave_shaper_slider,AParameters->getItem(PAR_OSC1_WM_AMOUNT));

// ----- res1 -----

      sa_tyr_SectionPanelWidget* res1_panel = new sa_tyr_SectionPanelWidget(SAT_DRect(10,HH+320,260,260), "RES1", 20 );
      MEditorPanel->appendChildWidget(res1_panel);

        // type

        sa_tyr_SelectorWidget* res1_type_selector = new sa_tyr_SelectorWidget( SAT_DRect(10+10,HH+310+30+10,240,20),"Res Type",res_type_menu );
        res1_panel->appendChildWidget(res1_type_selector);
        connect(res1_type_selector,AParameters->getItem(PAR_RES1_TYPE));

        // inputs

        sa_tyr_SmallKnobWidget* res1_in_o1 = new sa_tyr_SmallKnobWidget( SAT_DRect(10+10,HH+310+60+10,27,45),"O1");
        res1_panel->appendChildWidget(res1_in_o1);
        connect(res1_in_o1,AParameters->getItem(PAR_RES1_IN_O1));

        sa_tyr_SmallKnobWidget* res1_in_o2 = new sa_tyr_SmallKnobWidget( SAT_DRect(40+10,HH+310+60+10,27,45),"O2");
        res1_panel->appendChildWidget(res1_in_o2);
        connect(res1_in_o2,AParameters->getItem(PAR_RES1_IN_O2));

        sa_tyr_SmallKnobWidget* res1_in_r1 = new sa_tyr_SmallKnobWidget( SAT_DRect(70+10,HH+310+60+10,27,45),"R1");
        res1_panel->appendChildWidget(res1_in_r1);
        connect(res1_in_r1,AParameters->getItem(PAR_RES1_IN_R1));

        sa_tyr_SmallKnobWidget* res1_in_r2 = new sa_tyr_SmallKnobWidget( SAT_DRect(100+10,HH+310+60+10,27,45),"R2");
        res1_panel->appendChildWidget(res1_in_r2);
        connect(res1_in_r2,AParameters->getItem(PAR_RES1_IN_R2));

        sa_tyr_SmallKnobWidget* res1_in_n = new sa_tyr_SmallKnobWidget( SAT_DRect(130+10,HH+310+60+10,27,45),"N");
        res1_panel->appendChildWidget(res1_in_n);
        connect(res1_in_n,AParameters->getItem(PAR_RES1_IN_N));

        sa_tyr_SmallKnobWidget* res1_in_a = new sa_tyr_SmallKnobWidget( SAT_DRect(160+10,HH+310+60+10,27,45),"A");
        res1_panel->appendChildWidget(res1_in_a);
        connect(res1_in_a,AParameters->getItem(PAR_RES1_IN_A));

        sa_tyr_SmallKnobWidget* res1_in_i = new sa_tyr_SmallKnobWidget( SAT_DRect(190+10,HH+310+60+10,27,45),"I");
        res1_panel->appendChildWidget(res1_in_i);
        //res1_in_i->getKnobWidget()->setSnap(true);
        //res1_in_i->getKnobWidget()->setSnapPos(0.5);
        //res1_in_i->getKnobWidget()->setSnapDist(0.05);
        connect(res1_in_i,AParameters->getItem(PAR_RES1_IN_I));

        sa_tyr_SmallKnobWidget* res1_in_s = new sa_tyr_SmallKnobWidget( SAT_DRect(220+10,HH+310+60+10,27,45),"S");
        res1_panel->appendChildWidget(res1_in_s);
        connect(res1_in_s,AParameters->getItem(PAR_RES1_IN_S));

        // shape

        sa_tyr_BigKnobWidget* res1_nshape_knob = new sa_tyr_BigKnobWidget( SAT_DRect(10+10,HH+310+110+10,50,82),"Shape");
        res1_panel->appendChildWidget(res1_nshape_knob);
        connect(res1_nshape_knob,AParameters->getItem(PAR_RES1_SHAPE));

        // fb

        sa_tyr_BigKnobWidget* res1_fb_knob = new sa_tyr_BigKnobWidget( SAT_DRect(70+10,HH+310+110+10,50,82),"FB");
        res1_panel->appendChildWidget(res1_fb_knob);
        connect(res1_fb_knob,AParameters->getItem(PAR_RES1_FB));

        // damp

        sa_tyr_BigKnobWidget* res1_damp_knob = new sa_tyr_BigKnobWidget( SAT_DRect(130+10,HH+310+110+10,50,82),"Damp");
        res1_panel->appendChildWidget(res1_damp_knob);
        connect(res1_damp_knob,AParameters->getItem(PAR_RES1_DAMP));

        // rough

        sa_tyr_BigKnobWidget* res1_rough_knob = new sa_tyr_BigKnobWidget( SAT_DRect(190+10,HH+310+110+10,50,82),"Rough",0.0,true);
        res1_rough_knob->getKnobWidget()->setBipolar(false);
        res1_panel->appendChildWidget(res1_rough_knob);
        connect(res1_rough_knob,AParameters->getItem(PAR_RES1_ROUGH));

        // speed

        SAT_SliderWidget* res1_speed_slider = new SAT_SliderWidget( SAT_DRect(10+10,HH+310+200+10,240,20),"speed" );
        res1_panel->appendChildWidget(res1_speed_slider);
        connect(res1_speed_slider,AParameters->getItem(PAR_RES1_SPEED));

        // oct

        sa_tyr_DragValueWidget* res1_oct_slider = new sa_tyr_DragValueWidget( SAT_DRect(10+10,HH+310+230+10,72,20),"Oct", 0.0, true);
        res1_panel->appendChildWidget(res1_oct_slider);
        connect(res1_oct_slider,AParameters->getItem(PAR_RES1_OCT));

        // semi

        sa_tyr_DragValueWidget* res1_semi_slider = new sa_tyr_DragValueWidget( SAT_DRect(94+10,HH+310+230+10,72,20),"Semi", 0.0, true);
        res1_panel->appendChildWidget(res1_semi_slider);
        connect(res1_semi_slider,AParameters->getItem(PAR_RES1_SEMI));

        // cent

        sa_tyr_DragValueWidget* res1_cent_slider = new sa_tyr_DragValueWidget( SAT_DRect(178+10,HH+310+230+10,72,20),"Cent",0.0,true,0.0,0.1);
        res1_panel->appendChildWidget(res1_cent_slider);
        connect(res1_cent_slider,AParameters->getItem(PAR_RES1_CENT));

//
//
// mid --------------------------------------------------
//
//

// ----- osc2

      sa_tyr_SectionPanelWidget* osc2_panel = new sa_tyr_SectionPanelWidget(SAT_DRect(280,HH+10,260,300), "OSC2", 20 );
      MEditorPanel->appendChildWidget(osc2_panel);

        // type

        sa_tyr_SelectorWidget* osc2_type_selector = new sa_tyr_SelectorWidget( SAT_DRect(280+10,HH+30+10,240,20),"Osc Type",osc_type_menu );
        osc2_panel->appendChildWidget(osc2_type_selector);
        connect(osc2_type_selector,AParameters->getItem(PAR_OSC2_TYPE));

        // shape

        sa_tyr_BigKnobWidget* osc2_shape_knob = new sa_tyr_BigKnobWidget( SAT_DRect(280+10,HH+60,50,82),"Shape");
        osc2_panel->appendChildWidget(osc2_shape_knob);
        connect(osc2_shape_knob,AParameters->getItem(PAR_OSC2_SHAPE));

        // width

        sa_tyr_BigKnobWidget* osc2_width_knob = new sa_tyr_BigKnobWidget( SAT_DRect(280+70,HH+60,50,82),"Width",0.0,true);
        osc2_panel->appendChildWidget(osc2_width_knob);
        connect(osc2_width_knob,AParameters->getItem(PAR_OSC2_WIDTH));

        // oct

        sa_tyr_DragValueWidget* osc2_oct_slider = new sa_tyr_DragValueWidget( SAT_DRect(280+10,HH+150,72,20),"Oct", 0.0, true);
        osc2_panel->appendChildWidget(osc2_oct_slider);
        connect(osc2_oct_slider,AParameters->getItem(PAR_OSC2_OCT));

        // semi

        sa_tyr_DragValueWidget* osc2_semi_slider = new sa_tyr_DragValueWidget( SAT_DRect(280+94,HH+150,72,20),"Semi", 0.0, true);
        osc2_panel->appendChildWidget(osc2_semi_slider);
        connect(osc2_semi_slider,AParameters->getItem(PAR_OSC2_SEMI));

        // cent

        sa_tyr_DragValueWidget* osc2_cent_slider = new sa_tyr_DragValueWidget( SAT_DRect(280+178,HH+150,72,20),"Cent",0.0,true,0.0,0.1);
        osc2_panel->appendChildWidget(osc2_cent_slider);
        connect(osc2_cent_slider,AParameters->getItem(PAR_OSC2_CENT));

        // inputs

        sa_tyr_SmallKnobWidget* osc2_in_o1 = new sa_tyr_SmallKnobWidget( SAT_DRect(280+10,HH+180,27,45),"O1");
        osc2_panel->appendChildWidget(osc2_in_o1);
        connect(osc2_in_o1,AParameters->getItem(PAR_OSC2_IN_O1));

        sa_tyr_SmallKnobWidget* osc2_in_o2 = new sa_tyr_SmallKnobWidget( SAT_DRect(280+40,HH+180,27,45),"O2");
        osc2_panel->appendChildWidget(osc2_in_o2);
        connect(osc2_in_o2,AParameters->getItem(PAR_OSC2_IN_O2));

        sa_tyr_SmallKnobWidget* osc2_in_r1 = new sa_tyr_SmallKnobWidget( SAT_DRect(280+70,HH+180,27,45),"R1");
        osc2_panel->appendChildWidget(osc2_in_r1);
        connect(osc2_in_r1,AParameters->getItem(PAR_OSC2_IN_R1));

        sa_tyr_SmallKnobWidget* osc2_in_r2 = new sa_tyr_SmallKnobWidget( SAT_DRect(280+100,HH+180,27,45),"R2");
        osc2_panel->appendChildWidget(osc2_in_r2);
        connect(osc2_in_r2,AParameters->getItem(PAR_OSC2_IN_R2));

        sa_tyr_SmallKnobWidget* osc2_in_n = new sa_tyr_SmallKnobWidget( SAT_DRect(280+130,HH+180,27,45),"N");
        osc2_panel->appendChildWidget(osc2_in_n);
        connect(osc2_in_n,AParameters->getItem(PAR_OSC2_IN_N));

        sa_tyr_SmallKnobWidget* osc2_in_a = new sa_tyr_SmallKnobWidget( SAT_DRect(280+160,HH+180,27,45),"A");
        osc2_panel->appendChildWidget(osc2_in_a);
        connect(osc2_in_a,AParameters->getItem(PAR_OSC2_IN_A));

        sa_tyr_SmallKnobWidget* osc2_in_i = new sa_tyr_SmallKnobWidget( SAT_DRect(280+190,HH+180,27,45),"I");
        osc2_panel->appendChildWidget(osc2_in_i);
        //osc2_in_i->getKnobWidget()->setSnap(true);
        //osc2_in_i->getKnobWidget()->setSnapPos(0.5);
        //osc2_in_i->getKnobWidget()->setSnapDist(0.05);
        connect(osc2_in_i,AParameters->getItem(PAR_OSC2_IN_I));

        sa_tyr_SmallKnobWidget* osc2_in_s = new sa_tyr_SmallKnobWidget( SAT_DRect(280+220,HH+180,27,45),"S");
        osc2_panel->appendChildWidget(osc2_in_s);
        connect(osc2_in_s,AParameters->getItem(PAR_OSC2_IN_S));

        // ps

        sa_tyr_SelectorWidget* osc2_phase_shaper_selector = new sa_tyr_SelectorWidget( SAT_DRect(280+10,HH+240,115,20),"Phase Shp",phase_shaper_type_menu );
        osc2_panel->appendChildWidget(osc2_phase_shaper_selector);
        connect(osc2_phase_shaper_selector,AParameters->getItem(PAR_OSC2_PM_TYPE));

        sa_tyr_SliderWidget* osc2_phase_shaper_slider = new sa_tyr_SliderWidget(SAT_DRect(280+135,HH+240,115,20),"Phase",0.0);//,true);
        osc2_panel->appendChildWidget(osc2_phase_shaper_slider);
        connect(osc2_phase_shaper_slider,AParameters->getItem(PAR_OSC2_PM_AMOUNT));

        // ws

        sa_tyr_SelectorWidget* osc2_wave_shaper_selector = new sa_tyr_SelectorWidget( SAT_DRect(280+10,HH+270,115,20),"Wave Shp",wave_shaper_type_menu );
        osc2_panel->appendChildWidget(osc2_wave_shaper_selector);
        connect(osc2_wave_shaper_selector,AParameters->getItem(PAR_OSC2_WM_TYPE));

        sa_tyr_SliderWidget* osc2_wave_shaper_slider = new sa_tyr_SliderWidget(SAT_DRect(280+135,HH+270,115,20),"Wave",0.0);//,true);
        osc2_panel->appendChildWidget(osc2_wave_shaper_slider);
        connect(osc2_wave_shaper_slider,AParameters->getItem(PAR_OSC2_WM_AMOUNT));

// ----- res2 -----

      sa_tyr_SectionPanelWidget* res2_panel = new sa_tyr_SectionPanelWidget(SAT_DRect(280,HH+320,260,260), "RES2", 20 );
      MEditorPanel->appendChildWidget(res2_panel);

        // type

        sa_tyr_SelectorWidget* res2_type_selector = new sa_tyr_SelectorWidget( SAT_DRect(280+10,HH+310+30+10,230,20),"Res Type",res_type_menu );
        res2_panel->appendChildWidget(res2_type_selector);
        connect(res2_type_selector,AParameters->getItem(PAR_RES2_TYPE));

        // inputs

        sa_tyr_SmallKnobWidget* res2_in_o1 = new sa_tyr_SmallKnobWidget( SAT_DRect(280+10,HH+310+60+10,27,45),"O1");
        res2_panel->appendChildWidget(res2_in_o1);
        connect(res2_in_o1,AParameters->getItem(PAR_RES2_IN_O1));

        sa_tyr_SmallKnobWidget* res2_in_o2 = new sa_tyr_SmallKnobWidget( SAT_DRect(280+40,HH+310+60+10,27,45),"O2");
        res2_panel->appendChildWidget(res2_in_o2);
        connect(res2_in_o2,AParameters->getItem(PAR_RES2_IN_O2));

        sa_tyr_SmallKnobWidget* res2_in_r1 = new sa_tyr_SmallKnobWidget( SAT_DRect(280+70,HH+310+60+10,27,45),"R1");
        res2_panel->appendChildWidget(res2_in_r1);
        connect(res2_in_r1,AParameters->getItem(PAR_RES2_IN_R1));

        sa_tyr_SmallKnobWidget* res2_in_r2 = new sa_tyr_SmallKnobWidget( SAT_DRect(280+100,HH+310+60+10,27,45),"R2");
        res2_panel->appendChildWidget(res2_in_r2);
        connect(res2_in_r2,AParameters->getItem(PAR_RES2_IN_R2));

        sa_tyr_SmallKnobWidget* res2_in_n = new sa_tyr_SmallKnobWidget( SAT_DRect(280+130,HH+310+60+10,27,45),"N");
        res2_panel->appendChildWidget(res2_in_n);
        connect(res2_in_n,AParameters->getItem(PAR_RES2_IN_N));

        sa_tyr_SmallKnobWidget* res2_in_a = new sa_tyr_SmallKnobWidget( SAT_DRect(280+160,HH+310+60+10,27,45),"A");
        res2_panel->appendChildWidget(res2_in_a);
        connect(res2_in_a,AParameters->getItem(PAR_RES2_IN_A));

        sa_tyr_SmallKnobWidget* res2_in_i = new sa_tyr_SmallKnobWidget( SAT_DRect(280+190,HH+310+60+10,27,45),"I");
        res2_panel->appendChildWidget(res2_in_i);
        //res2_in_i->getKnobWidget()->setSnap(true);
        //res2_in_i->getKnobWidget()->setSnapPos(0.5);
        //res2_in_i->getKnobWidget()->setSnapDist(0.05);
        connect(res2_in_i,AParameters->getItem(PAR_RES2_IN_I));

        sa_tyr_SmallKnobWidget* res2_in_s = new sa_tyr_SmallKnobWidget( SAT_DRect(280+220,HH+310+60+10,27,45),"S");
        res2_panel->appendChildWidget(res2_in_s);
        connect(res2_in_s,AParameters->getItem(PAR_RES2_IN_S));

        // shape

        sa_tyr_BigKnobWidget* res2_nshape_knob = new sa_tyr_BigKnobWidget( SAT_DRect(280+10,HH+310+110+10,50,82),"Shape");
        res2_panel->appendChildWidget(res2_nshape_knob);
        connect(res2_nshape_knob,AParameters->getItem(PAR_RES2_SHAPE));

        // fb

        sa_tyr_BigKnobWidget* res2_fb_knob = new sa_tyr_BigKnobWidget( SAT_DRect(280+70,HH+310+110+10,50,82),"FB");
        res2_panel->appendChildWidget(res2_fb_knob);
        connect(res2_fb_knob,AParameters->getItem(PAR_RES2_FB));

        // damp

        sa_tyr_BigKnobWidget* res2_damp_knob = new sa_tyr_BigKnobWidget( SAT_DRect(280+130,HH+310+110+10,50,82),"Damp");
        res2_panel->appendChildWidget(res2_damp_knob);
        connect(res2_damp_knob,AParameters->getItem(PAR_RES2_DAMP));

        // rough

        sa_tyr_BigKnobWidget* res2_rough_knob = new sa_tyr_BigKnobWidget( SAT_DRect(280+190,HH+310+110+10,50,82),"Rough",0.0,true);
        res2_rough_knob->getKnobWidget()->setBipolar(false);
        res2_panel->appendChildWidget(res2_rough_knob);
        connect(res2_rough_knob,AParameters->getItem(PAR_RES2_ROUGH));

        // speed

        sa_tyr_SliderWidget* res2_speed_slider = new sa_tyr_SliderWidget( SAT_DRect(280+10,HH+310+200+10,240,20),"speed" );
        res2_panel->appendChildWidget(res2_speed_slider);
        connect(res2_speed_slider,AParameters->getItem(PAR_RES2_SPEED));

        // oct

        sa_tyr_DragValueWidget* res2_oct_slider = new sa_tyr_DragValueWidget( SAT_DRect(280+10,HH+310+230+10,72,20),"Oct", 0.0, true);
        res2_panel->appendChildWidget(res2_oct_slider);
        connect(res2_oct_slider,AParameters->getItem(PAR_RES2_OCT));

        // semi

        sa_tyr_DragValueWidget* res2_semi_slider = new sa_tyr_DragValueWidget( SAT_DRect(280+94,HH+310+230+10,72,20),"Semi", 0.0, true);
        res2_panel->appendChildWidget(res2_semi_slider);
        connect(res2_semi_slider,AParameters->getItem(PAR_RES2_SEMI));

        // cent

        sa_tyr_DragValueWidget* res2_cent_slider = new sa_tyr_DragValueWidget( SAT_DRect(280+178,HH+310+230+10,72,20),"Cent",0.0,true,0.0,0.1);
        res2_panel->appendChildWidget(res2_cent_slider);
        connect(res2_cent_slider,AParameters->getItem(PAR_RES2_CENT));

//
//
// right --------------------------------------------------
//
//

      // voices

      sa_tyr_SectionPanelWidget* voice2_panel = new sa_tyr_SectionPanelWidget(SAT_DRect(550,HH+10,276,75+30), "Voices", 20 );
      MEditorPanel->appendChildWidget(voice2_panel);

        // voices

        MVoiceWidget = new sa_tyr_VoiceWidget( SAT_DRect(550+10,HH+30+10,256,8) );
        voice2_panel->appendChildWidget(MVoiceWidget);

        MPlayingVoicesWidget = new SAT_ValueWidget( SAT_DRect(550+10,HH+45+10,150,15), "Playing:",0 );
        MPlayingVoicesWidget->setNumDigits(0);
        MPlayingVoicesWidget->setDrawBorder(false);
        MPlayingVoicesWidget->setFillBackground(false);
        voice2_panel->appendChildWidget(MPlayingVoicesWidget);

        MReleasedVoicesWidget = new SAT_ValueWidget( SAT_DRect(550+10,HH+60+10,150,15), "Released:",0 );
        MReleasedVoicesWidget->setNumDigits(0);
        MReleasedVoicesWidget->setDrawBorder(false);
        MReleasedVoicesWidget->setFillBackground(false);
        voice2_panel->appendChildWidget(MReleasedVoicesWidget);

        MTotalVoicesWidget = new SAT_ValueWidget( SAT_DRect(550+10,HH+75+10,150,15), "Total:",0 );
        MTotalVoicesWidget->setNumDigits(0);
        MTotalVoicesWidget->setDrawBorder(false);
        MTotalVoicesWidget->setFillBackground(false);
        voice2_panel->appendChildWidget(MTotalVoicesWidget);

      // ----- flt1 -----

      sa_tyr_SectionPanelWidget* flt1_panel = new sa_tyr_SectionPanelWidget(SAT_DRect(550,HH+125,276,145), "Filter", 20 );
      MEditorPanel->appendChildWidget(flt1_panel);

        // type

        sa_tyr_SelectorWidget* flt1_type_selector = new sa_tyr_SelectorWidget( SAT_DRect(550+10,HH+125+30,256,20),"Flt Type",flt_type_menu );
        flt1_panel->appendChildWidget(flt1_type_selector);
        connect(flt1_type_selector,AParameters->getItem(PAR_FLT1_TYPE));

        // freq

        sa_tyr_BigKnobWidget* flt1_freq_knob = new sa_tyr_BigKnobWidget( SAT_DRect(550+10,HH+125+60,50,82),"Freq");
        flt1_panel->appendChildWidget(flt1_freq_knob);
        connect(flt1_freq_knob,AParameters->getItem(PAR_FLT1_FREQ));

        // res

        sa_tyr_BigKnobWidget* flt1_res_knob = new sa_tyr_BigKnobWidget( SAT_DRect(550+70,HH+125+60,50,82),"Res");
        flt1_panel->appendChildWidget(flt1_res_knob);
        connect(flt1_res_knob,AParameters->getItem(PAR_FLT1_RES));

      // ----- env1 -----

      sa_tyr_SectionPanelWidget* env1_panel = new sa_tyr_SectionPanelWidget(SAT_DRect(550,HH+280,276,115), "Env", 20 );
      MEditorPanel->appendChildWidget(env1_panel);

        // att

        sa_tyr_BigKnobWidget* env1_att_knob = new sa_tyr_BigKnobWidget( SAT_DRect(550+10,HH+280+30,50,82),"Att");
        env1_panel->appendChildWidget(env1_att_knob);
        connect(env1_att_knob,AParameters->getItem(PAR_ENV1_ATT));

        // dec

        sa_tyr_BigKnobWidget* env1_dec_knob = new sa_tyr_BigKnobWidget( SAT_DRect(550+70,HH+280+30,50,82),"Dec");
        env1_panel->appendChildWidget(env1_dec_knob);
        connect(env1_dec_knob,AParameters->getItem(PAR_ENV1_DEC));

        // sus

        sa_tyr_BigKnobWidget* env1_sus_knob = new sa_tyr_BigKnobWidget( SAT_DRect(550+130,HH+280+30,50,82),"Sus");
        env1_panel->appendChildWidget(env1_sus_knob);
        connect(env1_sus_knob,AParameters->getItem(PAR_ENV1_SUS));

        // rel

        sa_tyr_BigKnobWidget* env1_rel_knob = new sa_tyr_BigKnobWidget( SAT_DRect(550+190,HH+280+30,50,82),"Rel");
        env1_panel->appendChildWidget(env1_rel_knob);
        connect(env1_rel_knob,AParameters->getItem(PAR_ENV1_REL));

      // ----- global -----

      sa_tyr_SectionPanelWidget* global_panel = new sa_tyr_SectionPanelWidget(SAT_DRect(550,HH+405,276,210), "Mix", 20 );
      MEditorPanel->appendChildWidget(global_panel);

        // osc1 out

        sa_tyr_BigKnobWidget* global_osc1_out_knob = new sa_tyr_BigKnobWidget( SAT_DRect(550+10,HH+405+30,50,82),"O1");
        global_panel->appendChildWidget(global_osc1_out_knob);
        connect(global_osc1_out_knob,AParameters->getItem(PAR_MASTER_OSC1_OUT));

        // osc2 out

        sa_tyr_BigKnobWidget* global_osc2_out_knob = new sa_tyr_BigKnobWidget( SAT_DRect(550+70,HH+405+30,50,82),"O2");
        global_panel->appendChildWidget(global_osc2_out_knob);
        connect(global_osc2_out_knob,AParameters->getItem(PAR_MASTER_OSC2_OUT));

        // res1 out

        sa_tyr_BigKnobWidget* global_res1_out_knob = new sa_tyr_BigKnobWidget( SAT_DRect(550+130,HH+405+30,50,82),"R1");
        global_panel->appendChildWidget(global_res1_out_knob);
        connect(global_res1_out_knob,AParameters->getItem(PAR_MASTER_RES1_OUT));

        // res2 out

        sa_tyr_BigKnobWidget* global_res2_out_knob = new sa_tyr_BigKnobWidget( SAT_DRect(550+190,HH+405+30,50,82),"R2");
        global_panel->appendChildWidget(global_res2_out_knob);
        connect(global_res2_out_knob,AParameters->getItem(PAR_MASTER_RES2_OUT));

        // vol

        sa_tyr_BigKnobWidget* global_vol_knob = new sa_tyr_BigKnobWidget( SAT_DRect(550+10,HH+405+120,50,82),"Vol");
        global_panel->appendChildWidget(global_vol_knob);
        connect(global_vol_knob,AParameters->getItem(PAR_MASTER_VOL));

        // pan

        sa_tyr_BigKnobWidget* global_pan_knob = new sa_tyr_BigKnobWidget( SAT_DRect(550+70,HH+405+120,50,82),"Pan",0.0,true);
        global_panel->appendChildWidget(global_pan_knob);
        connect(global_pan_knob,AParameters->getItem(PAR_MASTER_PAN));

//
//
// keyboard --------------------------------------------------
//
//

        //SAT_ScrollBoxWidget* scrollbox = new SAT_ScrollBoxWidget( SAT_DRect(0,SA_TYR_EDITOR_HEIGHT-60,SA_TYR_EDITOR_WIDTH,40),false,true);
        //MEditorPanel->appendChildWidget(scrollbox);
        //for (uint32_t i=0; i<11; i++) {
        //  MKeyboards[i] = new SAT_KeyboardWidget(SAT_DRect( i*80, 0, 80, 40 ));
        //  MKeyboards[i]->Layout.alignment = SAT_WIDGET_ALIGN_LEFT_TOP;
        //  scrollbox->appendChildWidget( MKeyboards[i] );
        //}

//
//
// --------------------------------------------------
//
//
    // menus

    MEditorPanel->appendChildWidget(osc_type_menu);
    MEditorPanel->appendChildWidget(res_type_menu);
    MEditorPanel->appendChildWidget(flt_type_menu);
    MEditorPanel->appendChildWidget(wave_shaper_type_menu);
    MEditorPanel->appendChildWidget(phase_shaper_type_menu);
    //MEditorPanel->appendChildWidget(osc_mix_type_menu);

    //

    //MEditorPanel->Layout.aspectRatio = EDITOR_WIDTH / EDITOR_HEIGHT;
    //MEditorPanel->setRectMode(SAT_WIDGET_SCALE_MODE_INITIAL_RATIO);

//    window->appendChildWidget(MEditorPanel);

  }

  //----------

  void timer_update(sa_tyr_voice_manager* voices) {
    if (MIsEditorOpen) { // && not resizing

      uint32_t playing = 0;
      uint32_t released = 0;
      for (uint32_t v=0; v<SA_TYR_NUM_VOICES; v++) {
        uint32_t state = voices->getVoiceState(v);
        MVoiceWidget->voice_state[v] = state;
        if (state == SAT_VOICE_PLAYING) playing += 1;
        if (state == SAT_VOICE_RELEASED) released += 1;
      }

      MPlayingVoicesWidget->setValue(playing);
      MReleasedVoicesWidget->setValue(released);
      MTotalVoicesWidget->setValue(playing + released);

      MVoiceWidget->redraw();
      MPlayingVoicesWidget->redraw();
      MReleasedVoicesWidget->redraw();
      MTotalVoicesWidget->redraw();

    }
  }

  //----------

  //void updateWaveformWidget() {
  //}


};

#undef HH


//----------------------------------------------------------------------
#endif
