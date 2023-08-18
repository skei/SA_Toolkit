#ifndef sa_botage_prob_page_included
#define sa_botage_prob_page_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widgets.h"
  
//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_botage_prob_page_widget
: public SAT_PanelWidget
, public SAT_MenuListener {

//------------------------------
private:
//------------------------------

  //SAT_MenuWidget* fx_type_menu = nullptr;

//------------------------------
public:
//------------------------------

  sa_botage_prob_page_widget(SAT_Rect ARect, SAT_MenuWidget* fx_menu)
  : SAT_PanelWidget(ARect) {
    
    setFillBackground(true);
    setBackgroundColor(0.18);
    
    setDrawBorder(false);
    setBorderColor(0.2);
    
    // fx type
    
//    fx_type_menu = new SAT_MenuWidget(SAT_Rect(100,100));
//    fx_type_menu->appendChildWidget( new SAT_MenuItemWidget(10,"LowPass") );
//    fx_type_menu->appendChildWidget( new SAT_MenuItemWidget(10,"BandPass") );
//    fx_type_menu->appendChildWidget( new SAT_MenuItemWidget(10,"HighPass") );
    
    // range
    
    sa_botage_knob2_widget*       range_length_prob     = new sa_botage_knob2_widget(       SAT_Rect( 10,     20+10,      40, 40 ), "%",0.0);
    sa_botage_text2_widget*       range_length_text     = new sa_botage_text2_widget(       SAT_Rect( 55,     20+10,      95, 18 ), "Length");
    sa_botage_dualslider_widget*  range_length_minmax   = new sa_botage_dualslider_widget(  SAT_Rect( 55,     20+32,      95, 18 ),"minmax",0.25,0.75);
    
    sa_botage_knob2_widget*       range_speed_prob      = new sa_botage_knob2_widget(       SAT_Rect( 10,     20+50+10,   40, 40 ),"%",0.0);
    sa_botage_text2_widget*       range_speed_text      = new sa_botage_text2_widget(       SAT_Rect( 55,     20+50+10,   95, 18 ), "Speed");
    sa_botage_dualslider_widget*  range_speed_minmax    = new sa_botage_dualslider_widget(  SAT_Rect( 55,     20+50+32,   95, 18 ),"minmax",0.25,0.75);
    
    sa_botage_knob2_widget*       range_offset_prob     = new sa_botage_knob2_widget(       SAT_Rect( 10,     20+100+10,  40, 40 ),"%",0.0);
    sa_botage_text2_widget*       range_offset_text     = new sa_botage_text2_widget(       SAT_Rect( 55,     20+100+10,  95, 18 ),"Offset");
    sa_botage_dualslider_widget*  range_offset_minmax   = new sa_botage_dualslider_widget(  SAT_Rect( 55,     20+100+32,  95, 18 ),"minmax",0.25,0.75);
    
    sa_botage_knob2_widget*       range_reverse_prob    = new sa_botage_knob2_widget(       SAT_Rect( 10,     20+150+10,  40, 40 ),"%",0.0);
    sa_botage_text2_widget*       range_reverse_text    = new sa_botage_text2_widget(       SAT_Rect( 55,     20+150+10,  95, 18 ),"Reverse");
    
    sa_botage_knob2_widget*       range_fx_prob         = new sa_botage_knob2_widget(       SAT_Rect( 10,     20+200+10,  40, 40 ),"%",0.0);
    sa_botage_text2_widget*       range_fx_text         = new sa_botage_text2_widget(       SAT_Rect( 55,     20+200+10,  95, 18 ),"FX");
    sa_botage_dualslider_widget*  range_fx_minmax       = new sa_botage_dualslider_widget(  SAT_Rect( 55,     20+200+32,  95, 18 ),"minmax",0.25,0.75);
    
    // loop
    
    sa_botage_knob2_widget*       loop_length_prob      = new sa_botage_knob2_widget(       SAT_Rect( 150+10, 20+10,      40, 40 ),"%",0.0);
    sa_botage_text2_widget*       loop_length_text      = new sa_botage_text2_widget(       SAT_Rect( 150+55, 20+10,      95, 18 ),"Length");
    sa_botage_dualslider_widget*  loop_length_minmax    = new sa_botage_dualslider_widget(  SAT_Rect( 150+55, 20+32,      95, 18 ),"minmax",0.25,0.75);
    
    sa_botage_knob2_widget*       loop_speed_prob       = new sa_botage_knob2_widget(       SAT_Rect( 150+10, 20+50+10,   40, 40 ),"%",0.0);
    sa_botage_text2_widget*       loop_speed_text       = new sa_botage_text2_widget(       SAT_Rect( 150+55, 20+50+10,   95, 18 ),"Speed");
    sa_botage_dualslider_widget*  loop_speed_minmax     = new sa_botage_dualslider_widget(  SAT_Rect( 150+55, 20+50+32,   95, 18 ),"minmax",0.25,0.75);
    
    sa_botage_knob2_widget*       loop_offset_prob      = new sa_botage_knob2_widget(       SAT_Rect( 150+10, 20+100+10,  40, 40 ),"%",0.0);
    sa_botage_text2_widget*       loop_offset_text      = new sa_botage_text2_widget(       SAT_Rect( 150+55, 20+100+10,  95, 18 ),"Offset");
    sa_botage_dualslider_widget*  loop_offset_minmax    = new sa_botage_dualslider_widget(  SAT_Rect( 150+55, 20+100+32,  95, 18 ),"minmax",0.25,0.75);
    
    sa_botage_knob2_widget*       loop_reverse_prob     = new sa_botage_knob2_widget(       SAT_Rect( 150+10, 20+150+10,  40, 40 ),"%",0.0);
    sa_botage_text2_widget*       loop_reverse_text     = new sa_botage_text2_widget(       SAT_Rect( 150+55, 20+150+10,  95, 18 ),"Reverse");
    
    sa_botage_knob2_widget*       loop_fx_prob          = new sa_botage_knob2_widget(       SAT_Rect( 150+10, 20+200+10,  40, 40 ),"%",0.0);
    sa_botage_text2_widget*       loop_fx_text          = new sa_botage_text2_widget(       SAT_Rect( 150+55, 20+200+10,  95, 18 ),"FX");
    sa_botage_dualslider_widget*  loop_fx_minmax        = new sa_botage_dualslider_widget(  SAT_Rect( 150+55, 20+200+32,  95, 18 ),"minmax",0.25,0.75);
    
    // fx
    
    sa_botage_knob2_widget*       fx1_prob              = new sa_botage_knob2_widget(       SAT_Rect( 300+10, 20+10,      40, 40 ),"%",0.0);
    sa_botage_selector_widget*    fx1_type              = new sa_botage_selector_widget(    SAT_Rect( 300+55, 20+10,      95, 18 ),"FX1",fx_menu,this);
    sa_botage_knob1_widget*       fx1_arg1              = new sa_botage_knob1_widget(       SAT_Rect( 300+55, 20+32,      18, 18 ),"fx1 arg1");
    sa_botage_knob1_widget*       fx1_arg2              = new sa_botage_knob1_widget(       SAT_Rect( 320+55, 20+32,      18, 18 ),"fx1 arg2");
    sa_botage_knob1_widget*       fx1_arg3              = new sa_botage_knob1_widget(       SAT_Rect( 340+55, 20+32,      18, 18 ),"fx1 arg3");

    sa_botage_knob2_widget*       fx2_prob              = new sa_botage_knob2_widget(       SAT_Rect( 300+10, 20+50+10,   40, 40 ),"%",0.0);
    sa_botage_selector_widget*    fx2_type              = new sa_botage_selector_widget(    SAT_Rect( 300+55, 20+50+10,   95, 18 ),"FX2",fx_menu,this);
    sa_botage_knob1_widget*       fx2_arg1              = new sa_botage_knob1_widget(       SAT_Rect( 300+55, 20+50+32,   18, 18 ),"fx2 arg1");
    sa_botage_knob1_widget*       fx2_arg2              = new sa_botage_knob1_widget(       SAT_Rect( 320+55, 20+50+32,   18, 18 ),"fx2 arg2");
    sa_botage_knob1_widget*       fx2_arg3              = new sa_botage_knob1_widget(       SAT_Rect( 340+55, 20+50+32,   18, 18 ),"fx2 arg3");

    sa_botage_knob2_widget*       fx3_prob              = new sa_botage_knob2_widget(       SAT_Rect( 300+10, 20+100+10,  40, 40 ),"%",0.0);
    sa_botage_selector_widget*    fx3_type              = new sa_botage_selector_widget(    SAT_Rect( 300+55, 20+100+10,  95, 18 ),"FX3",fx_menu,this);
    sa_botage_knob1_widget*       fx3_arg1              = new sa_botage_knob1_widget(       SAT_Rect( 300+55, 20+100+32,  18, 18 ),"fx3 arg1");
    sa_botage_knob1_widget*       fx3_arg2              = new sa_botage_knob1_widget(       SAT_Rect( 320+55, 20+100+32,  18, 18 ),"fx3 arg2");
    sa_botage_knob1_widget*       fx3_arg3              = new sa_botage_knob1_widget(       SAT_Rect( 340+55, 20+100+32,  18, 18 ),"fx3 arg3");

    sa_botage_knob2_widget*       fx4_prob              = new sa_botage_knob2_widget(       SAT_Rect( 300+10, 20+150+10,  40, 40 ),"%",0.0);
    sa_botage_selector_widget*    fx4_type              = new sa_botage_selector_widget(    SAT_Rect( 300+55, 20+150+10,  95, 18 ),"FX4",fx_menu,this);
    sa_botage_knob1_widget*       fx4_arg1              = new sa_botage_knob1_widget(       SAT_Rect( 300+55, 20+150+32,  18, 18 ),"fx4 arg1");
    sa_botage_knob1_widget*       fx4_arg2              = new sa_botage_knob1_widget(       SAT_Rect( 320+55, 20+150+32,  18, 18 ),"fx4 arg2");
    sa_botage_knob1_widget*       fx4_arg3              = new sa_botage_knob1_widget(       SAT_Rect( 340+55, 20+150+32,  18, 18 ),"fx4 arg3");

    sa_botage_knob2_widget*       fx5_prob              = new sa_botage_knob2_widget(       SAT_Rect( 300+10, 20+200+10,  40, 40 ),"%",0.0);
    sa_botage_selector_widget*    fx5_type              = new sa_botage_selector_widget(    SAT_Rect( 300+55, 20+200+10,  95, 18 ),"FX5",fx_menu,this);
    sa_botage_knob1_widget*       fx5_arg1              = new sa_botage_knob1_widget(       SAT_Rect( 300+55, 20+200+32,  18, 18 ),"fx5 arg1");
    sa_botage_knob1_widget*       fx5_arg2              = new sa_botage_knob1_widget(       SAT_Rect( 320+55, 20+200+32,  18, 18 ),"fx5 arg2");
    sa_botage_knob1_widget*       fx5_arg3              = new sa_botage_knob1_widget(       SAT_Rect( 340+55, 20+200+32,  18, 18 ),"fx5 arg3");

    // range
    
    appendChildWidget( new sa_botage_text3_widget(SAT_Rect(10,10,140,15),"RANGE"));
    appendChildWidget( new sa_botage_text3_widget(SAT_Rect(160,10,140,15),"LOOP"));
    appendChildWidget( new sa_botage_text3_widget(SAT_Rect(310,10,140,15),"FX"));


    appendChildWidget(range_length_prob);
    appendChildWidget(range_length_text);
    appendChildWidget(range_length_minmax);
    
    appendChildWidget(range_speed_prob);
    appendChildWidget(range_speed_text);
    appendChildWidget(range_speed_minmax);
    
    appendChildWidget(range_offset_prob);
    appendChildWidget(range_offset_text);
    appendChildWidget(range_offset_minmax);
    
    appendChildWidget(range_reverse_prob);
    appendChildWidget(range_reverse_text);
    //appendChildWidget(range_reverse_minmax);
    
    appendChildWidget(range_fx_prob);
    appendChildWidget(range_fx_text);
    appendChildWidget(range_fx_minmax);
    
    // loop
    
    appendChildWidget(loop_length_prob);
    appendChildWidget(loop_length_text);
    appendChildWidget(loop_length_minmax);
    
    appendChildWidget(loop_speed_prob);
    appendChildWidget(loop_speed_text);
    appendChildWidget(loop_speed_minmax);
    
    appendChildWidget(loop_offset_prob);
    appendChildWidget(loop_offset_text);
    appendChildWidget(loop_offset_minmax);
    
    appendChildWidget(loop_reverse_prob);
    appendChildWidget(loop_reverse_text);
    //appendChildWidget(loop_reverse_minmax);
    
    appendChildWidget(loop_fx_prob);
    appendChildWidget(loop_fx_text);
    appendChildWidget(loop_fx_minmax);
    
    // fx
    
    appendChildWidget(fx1_prob);
    appendChildWidget(fx1_type);
    appendChildWidget(fx1_arg1);
    appendChildWidget(fx1_arg2);
    appendChildWidget(fx1_arg3);
    
    appendChildWidget(fx2_prob);
    appendChildWidget(fx2_type);
    appendChildWidget(fx2_arg1);
    appendChildWidget(fx2_arg2);
    appendChildWidget(fx2_arg3);
    
    appendChildWidget(fx3_prob);
    appendChildWidget(fx3_type);
    appendChildWidget(fx3_arg1);
    appendChildWidget(fx3_arg2);
    appendChildWidget(fx3_arg3);
    
    appendChildWidget(fx4_prob);
    appendChildWidget(fx4_type);
    appendChildWidget(fx4_arg1);
    appendChildWidget(fx4_arg2);
    appendChildWidget(fx4_arg3);
    
    appendChildWidget(fx5_prob);
    appendChildWidget(fx5_type);
    appendChildWidget(fx5_arg1);
    appendChildWidget(fx5_arg2);
    appendChildWidget(fx5_arg3);
    
    
    //----------
    
//    appendChildWidget(fx_type_menu);
    
  }
  
};
  
 
//----------------------------------------------------------------------
#endif