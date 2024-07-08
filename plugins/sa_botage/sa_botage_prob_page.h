#ifndef sa_botage_prob_page_included
#define sa_botage_prob_page_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_widgets.h"
  
//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_botage_prob_page_widget
: public SAT_VisualWidget
, public SAT_MenuListener {

//------------------------------
public:
//------------------------------

  //SAT_MenuWidget* fx_type_menu = nullptr;
  
    sa_botage_knob2_widget*       range_length_prob     = nullptr;
    sa_botage_text2_widget*       range_length_text     = nullptr;
    sa_botage_dualslider_widget*  range_length_minmax   = nullptr;
    
    sa_botage_knob2_widget*       range_speed_prob      = nullptr;
    sa_botage_text2_widget*       range_speed_text      = nullptr;
    sa_botage_dualslider_widget*  range_speed_minmax    = nullptr;
    
    sa_botage_knob2_widget*       range_offset_prob     = nullptr;
    sa_botage_text2_widget*       range_offset_text     = nullptr;
    sa_botage_dualslider_widget*  range_offset_minmax   = nullptr;
    
    sa_botage_knob2_widget*       range_reverse_prob    = nullptr;
    sa_botage_text2_widget*       range_reverse_text    = nullptr;
    
    sa_botage_knob2_widget*       range_fx_prob         = nullptr;
    sa_botage_text2_widget*       range_fx_text         = nullptr;
    sa_botage_dualslider_widget*  range_fx_minmax       = nullptr;
    
    // loop
    
    sa_botage_knob2_widget*       loop_length_prob      = nullptr;
    sa_botage_text2_widget*       loop_length_text      = nullptr;
    sa_botage_dualslider_widget*  loop_length_minmax    = nullptr;
    
    sa_botage_knob2_widget*       loop_speed_prob       = nullptr;
    sa_botage_text2_widget*       loop_speed_text       = nullptr;
    sa_botage_dualslider_widget*  loop_speed_minmax     = nullptr;
    
    sa_botage_knob2_widget*       loop_offset_prob      = nullptr;
    sa_botage_text2_widget*       loop_offset_text      = nullptr;
    sa_botage_dualslider_widget*  loop_offset_minmax    = nullptr;
    
    sa_botage_knob2_widget*       loop_reverse_prob     = nullptr;
    sa_botage_text2_widget*       loop_reverse_text     = nullptr;
    
    sa_botage_knob2_widget*       loop_fx_prob          = nullptr;
    sa_botage_text2_widget*       loop_fx_text          = nullptr;
    sa_botage_dualslider_widget*  loop_fx_minmax        = nullptr;
    
    // fx
    
    sa_botage_knob2_widget*       fx1_prob              = nullptr;
    sa_botage_selector_widget*    fx1_type              = nullptr;
    sa_botage_knob1_widget*       fx1_arg1              = nullptr;
    sa_botage_knob1_widget*       fx1_arg2              = nullptr;
    sa_botage_knob1_widget*       fx1_arg3              = nullptr;

    sa_botage_knob2_widget*       fx2_prob              = nullptr;
    sa_botage_selector_widget*    fx2_type              = nullptr;
    sa_botage_knob1_widget*       fx2_arg1              = nullptr;
    sa_botage_knob1_widget*       fx2_arg2              = nullptr;
    sa_botage_knob1_widget*       fx2_arg3              = nullptr;

    sa_botage_knob2_widget*       fx3_prob              = nullptr;
    sa_botage_selector_widget*    fx3_type              = nullptr;
    sa_botage_knob1_widget*       fx3_arg1              = nullptr;
    sa_botage_knob1_widget*       fx3_arg2              = nullptr;
    sa_botage_knob1_widget*       fx3_arg3              = nullptr;

    sa_botage_knob2_widget*       fx4_prob              = nullptr;
    sa_botage_selector_widget*    fx4_type              = nullptr;
    sa_botage_knob1_widget*       fx4_arg1              = nullptr;
    sa_botage_knob1_widget*       fx4_arg2              = nullptr;
    sa_botage_knob1_widget*       fx4_arg3              = nullptr;

    sa_botage_knob2_widget*       fx5_prob              = nullptr;
    sa_botage_selector_widget*    fx5_type              = nullptr;
    sa_botage_knob1_widget*       fx5_arg1              = nullptr;
    sa_botage_knob1_widget*       fx5_arg2              = nullptr;
    sa_botage_knob1_widget*       fx5_arg3              = nullptr;

//------------------------------
public:
//------------------------------

  sa_botage_prob_page_widget(SAT_Rect ARect, SAT_MenuWidget* fx_menu)
  : SAT_VisualWidget(ARect) {
    
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
    
    range_length_prob     = new sa_botage_knob2_widget(       SAT_Rect( 10,     20+10,      40, 40 ), "%", 0.0 );
    range_length_text     = new sa_botage_text2_widget(       SAT_Rect( 55,     20+10,      95, 18 ), "Length" );
    range_length_minmax   = new sa_botage_dualslider_widget(  SAT_Rect( 55,     20+32,      95, 18 ), "%", 0.25, 0.75 );
    
    range_speed_prob      = new sa_botage_knob2_widget(       SAT_Rect( 10,     20+50+10,   40, 40 ), "%", 0.0 );
    range_speed_text      = new sa_botage_text2_widget(       SAT_Rect( 55,     20+50+10,   95, 18 ), "Speed" );
    range_speed_minmax    = new sa_botage_dualslider_widget(  SAT_Rect( 55,     20+50+32,   95, 18 ), "%", 0.25, 0.75 );
    
    range_offset_prob     = new sa_botage_knob2_widget(       SAT_Rect( 10,     20+100+10,  40, 40 ), "%", 0.0 );
    range_offset_text     = new sa_botage_text2_widget(       SAT_Rect( 55,     20+100+10,  95, 18 ), "Offset" );
    range_offset_minmax   = new sa_botage_dualslider_widget(  SAT_Rect( 55,     20+100+32,  95, 18 ), "%", 0.25, 0.75 );
    
    range_reverse_prob    = new sa_botage_knob2_widget(       SAT_Rect( 10,     20+150+10,  40, 40 ), "%", 0.0 );
    range_reverse_text    = new sa_botage_text2_widget(       SAT_Rect( 55,     20+150+10,  95, 18 ), "Reverse" );
    
    range_fx_prob         = new sa_botage_knob2_widget(       SAT_Rect( 10,     20+200+10,  40, 40 ), "%", 0.0 );
    range_fx_text         = new sa_botage_text2_widget(       SAT_Rect( 55,     20+200+10,  95, 18 ), "FX" );
    range_fx_minmax       = new sa_botage_dualslider_widget(  SAT_Rect( 55,     20+200+32,  95, 18 ), "%", 0.25, 0.75 );

    range_length_minmax->setDragSnap(true);
    range_length_minmax->setDragSnapPos(0.5);
    range_speed_minmax->setDragSnap(true);
    range_speed_minmax->setDragSnapPos(0.5);
    range_offset_minmax->setDragQuantize(true);
    range_offset_minmax->setDragQuantizeSteps(17);
    range_fx_minmax->setDragSnap(true);
    range_fx_minmax->setDragSnapPos(0.5);
    
    // loop
    
    loop_length_prob      = new sa_botage_knob2_widget(       SAT_Rect( 150+10, 20+10,      40, 40 ), "%", 0.0 );
    loop_length_text      = new sa_botage_text2_widget(       SAT_Rect( 150+55, 20+10,      95, 18 ), "Length" );
    loop_length_minmax    = new sa_botage_dualslider_widget(  SAT_Rect( 150+55, 20+32,      95, 18 ), "%", 0.25, 0.75 );
    
    loop_speed_prob       = new sa_botage_knob2_widget(       SAT_Rect( 150+10, 20+50+10,   40, 40 ), "%", 0.0 );
    loop_speed_text       = new sa_botage_text2_widget(       SAT_Rect( 150+55, 20+50+10,   95, 18 ), "Speed" );
    loop_speed_minmax     = new sa_botage_dualslider_widget(  SAT_Rect( 150+55, 20+50+32,   95, 18 ), "%", 0.25, 0.75 );
    
    loop_offset_prob      = new sa_botage_knob2_widget(       SAT_Rect( 150+10, 20+100+10,  40, 40 ), "%", 0.0 );
    loop_offset_text      = new sa_botage_text2_widget(       SAT_Rect( 150+55, 20+100+10,  95, 18 ), "Offset" );
    loop_offset_minmax    = new sa_botage_dualslider_widget(  SAT_Rect( 150+55, 20+100+32,  95, 18 ), "%", 0.25, 0.75 );
    
    loop_reverse_prob     = new sa_botage_knob2_widget(       SAT_Rect( 150+10, 20+150+10,  40, 40 ), "%", 0.0 );
    loop_reverse_text     = new sa_botage_text2_widget(       SAT_Rect( 150+55, 20+150+10,  95, 18 ), "Reverse" );
    
    loop_fx_prob          = new sa_botage_knob2_widget(       SAT_Rect( 150+10, 20+200+10,  40, 40 ), "%", 0.0 );
    loop_fx_text          = new sa_botage_text2_widget(       SAT_Rect( 150+55, 20+200+10,  95, 18 ), "FX" );
    loop_fx_minmax        = new sa_botage_dualslider_widget(  SAT_Rect( 150+55, 20+200+32,  95, 18 ), "%", 0.25, 0.75 );
    
    loop_length_minmax->setDragSnap(true);
    loop_length_minmax->setDragSnapPos(0.5);
    loop_speed_minmax->setDragSnap(true);
    loop_speed_minmax->setDragSnapPos(0.5);
    loop_offset_minmax->setDragQuantize(true);
    loop_offset_minmax->setDragQuantizeSteps(17);
    loop_fx_minmax->setDragSnap(true);
    loop_fx_minmax->setDragSnapPos(0.5);
    
    // fx
    
    fx1_prob              = new sa_botage_knob2_widget(       SAT_Rect( 300+10, 20+10,      40, 40 ), "%", 0.0 );
    fx1_type              = new sa_botage_selector_widget(    SAT_Rect( 300+55, 20+10,      95, 18 ), "FX1", fx_menu);
    fx1_arg1              = new sa_botage_knob1_widget(       SAT_Rect( 300+55, 20+32,      18, 18 ), "fx1 arg1", 0.5 );
    fx1_arg2              = new sa_botage_knob1_widget(       SAT_Rect( 320+55, 20+32,      18, 18 ), "fx1 arg2", 0.5 );
    fx1_arg3              = new sa_botage_knob1_widget(       SAT_Rect( 340+55, 20+32,      18, 18 ), "fx1 arg3", 0.5 );

    fx2_prob              = new sa_botage_knob2_widget(       SAT_Rect( 300+10, 20+50+10,   40, 40 ), "%", 0.0 );
    fx2_type              = new sa_botage_selector_widget(    SAT_Rect( 300+55, 20+50+10,   95, 18 ), "FX2", fx_menu);
    fx2_arg1              = new sa_botage_knob1_widget(       SAT_Rect( 300+55, 20+50+32,   18, 18 ), "fx2 arg1", 0.5 );
    fx2_arg2              = new sa_botage_knob1_widget(       SAT_Rect( 320+55, 20+50+32,   18, 18 ), "fx2 arg2", 0.5  );
    fx2_arg3              = new sa_botage_knob1_widget(       SAT_Rect( 340+55, 20+50+32,   18, 18 ), "fx2 arg3", 0.5 );

    fx3_prob              = new sa_botage_knob2_widget(       SAT_Rect( 300+10, 20+100+10,  40, 40 ), "%", 0.0 );
    fx3_type              = new sa_botage_selector_widget(    SAT_Rect( 300+55, 20+100+10,  95, 18 ), "FX3", fx_menu);
    fx3_arg1              = new sa_botage_knob1_widget(       SAT_Rect( 300+55, 20+100+32,  18, 18 ), "fx3 arg1", 0.5 );
    fx3_arg2              = new sa_botage_knob1_widget(       SAT_Rect( 320+55, 20+100+32,  18, 18 ), "fx3 arg2", 0.5 );
    fx3_arg3              = new sa_botage_knob1_widget(       SAT_Rect( 340+55, 20+100+32,  18, 18 ), "fx3 arg3", 0.5 );

    fx4_prob              = new sa_botage_knob2_widget(       SAT_Rect( 300+10, 20+150+10,  40, 40 ), "%", 0.0 );
    fx4_type              = new sa_botage_selector_widget(    SAT_Rect( 300+55, 20+150+10,  95, 18 ), "FX4", fx_menu);
    fx4_arg1              = new sa_botage_knob1_widget(       SAT_Rect( 300+55, 20+150+32,  18, 18 ), "fx4 arg1", 0.5 );
    fx4_arg2              = new sa_botage_knob1_widget(       SAT_Rect( 320+55, 20+150+32,  18, 18 ), "fx4 arg2", 0.5 );
    fx4_arg3              = new sa_botage_knob1_widget(       SAT_Rect( 340+55, 20+150+32,  18, 18 ), "fx4 arg3", 0.5 );

    fx5_prob              = new sa_botage_knob2_widget(       SAT_Rect( 300+10, 20+200+10,  40, 40 ), "%", 0.0 );
    fx5_type              = new sa_botage_selector_widget(    SAT_Rect( 300+55, 20+200+10,  95, 18 ), "FX5", fx_menu);
    fx5_arg1              = new sa_botage_knob1_widget(       SAT_Rect( 300+55, 20+200+32,  18, 18 ), "fx5 arg1", 0.5 );
    fx5_arg2              = new sa_botage_knob1_widget(       SAT_Rect( 320+55, 20+200+32,  18, 18 ), "fx5 arg2", 0.5 );
    fx5_arg3              = new sa_botage_knob1_widget(       SAT_Rect( 340+55, 20+200+32,  18, 18 ), "fx5 arg3", 0.5 );

    // range
    
    appendChild( new sa_botage_text3_widget(SAT_Rect(10,10,140,15),"RANGE"));
    appendChild( new sa_botage_text3_widget(SAT_Rect(160,10,140,15),"LOOP"));
    appendChild( new sa_botage_text3_widget(SAT_Rect(310,10,140,15),"FX"));


    appendChild(range_length_prob);
    appendChild(range_length_text);
    appendChild(range_length_minmax);
    
    appendChild(range_speed_prob);
    appendChild(range_speed_text);
    appendChild(range_speed_minmax);
    
    appendChild(range_offset_prob);
    appendChild(range_offset_text);
    appendChild(range_offset_minmax);
    
    appendChild(range_reverse_prob);
    appendChild(range_reverse_text);
    //appendChild(range_reverse_minmax);
    
    appendChild(range_fx_prob);
    appendChild(range_fx_text);
    appendChild(range_fx_minmax);
    
    // loop
    
    appendChild(loop_length_prob);
    appendChild(loop_length_text);
    appendChild(loop_length_minmax);
    
    appendChild(loop_speed_prob);
    appendChild(loop_speed_text);
    appendChild(loop_speed_minmax);
    
    appendChild(loop_offset_prob);
    appendChild(loop_offset_text);
    appendChild(loop_offset_minmax);
    
    appendChild(loop_reverse_prob);
    appendChild(loop_reverse_text);
    //appendChild(loop_reverse_minmax);
    
    appendChild(loop_fx_prob);
    appendChild(loop_fx_text);
    appendChild(loop_fx_minmax);
    
    // fx
    
    appendChild(fx1_prob);
    appendChild(fx1_type);
    appendChild(fx1_arg1);
    appendChild(fx1_arg2);
    appendChild(fx1_arg3);
    
    appendChild(fx2_prob);
    appendChild(fx2_type);
    appendChild(fx2_arg1);
    appendChild(fx2_arg2);
    appendChild(fx2_arg3);
    
    appendChild(fx3_prob);
    appendChild(fx3_type);
    appendChild(fx3_arg1);
    appendChild(fx3_arg2);
    appendChild(fx3_arg3);
    
    appendChild(fx4_prob);
    appendChild(fx4_type);
    appendChild(fx4_arg1);
    appendChild(fx4_arg2);
    appendChild(fx4_arg3);
    
    appendChild(fx5_prob);
    appendChild(fx5_type);
    appendChild(fx5_arg1);
    appendChild(fx5_arg2);
    appendChild(fx5_arg3);
    
    
    //----------
    
//    appendChild(fx_type_menu);
    
  }
  
};
  
 
//----------------------------------------------------------------------
#endif