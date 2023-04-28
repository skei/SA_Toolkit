
//----------------------------------------------------------------------
//
// inline
//
//----------------------------------------------------------------------

    // fx type

    sa_botage_MenuWidget* fx_type_menu = new sa_botage_MenuWidget(70,13,fx_type_text,SA_BOTAGE_FX_TYPE_COUNT);
    fx_type_menu->setDrawBorder(true);
    fx_type_menu->setBorderColor(0);
    fx_type_menu->setFillBackground(true);
    fx_type_menu->setBackgroundColor(0.5);

//----------------------------------------------------------------------

    SAT_Widget* page_prob = new SAT_Widget( SAT_Rect(255,90,357,292));

    //------------------------------
    //
    //------------------------------

    // main prob

    MMainProbWidget = new SAT_KnobWidget(SAT_Rect(255,90,80,80), "%", 0.0);
    page_prob->appendChildWidget(MMainProbWidget);
//    MMainProbWidget->setHint("Main probability");
    MMainProbWidget->setDrawText(true);
    MMainProbWidget->setDrawValue(true);
    MMainProbWidget->setArcThickness(15);
    MMainProbWidget->setTextSize(20);
    MMainProbWidget->setValueSize(25);
    MMainProbWidget->setTextColor(0.6);
    MMainProbWidget->setValueColor(0.7);
    MMainProbWidget->setArcBackColor(             SAT_Color(0.0, 0.3, 0.6) );
    MMainProbWidget->setArcValueColor(            SAT_Color(0.0, 0.5, 1.0) );
    MMainProbWidget->setInteractiveArcValueColor( SAT_Color(0.5, 0.75, 1.0) );

    connect( MMainProbWidget, AParameters->getItem(PAR_RANGE_PROB) );

    // slices

    SAT_TextWidget* prob_slices_label = new SAT_TextWidget( SAT_Rect(345,95,135,12), "Range (# slices)" );
    page_prob->appendChildWidget(prob_slices_label);
    prob_slices_label->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    prob_slices_label->setTextColor(0.8);
    prob_slices_label->setTextSize(12);
    prob_slices_label->setDrawBorder(false);
    prob_slices_label->setFillBackground(false);

    MMainSlicesWidget = new SAT_ButtonRowWidget( SAT_Rect(345,109,135,16), 8, prob_slices_text, SAT_BUTTON_ROW_MULTI );
    page_prob->appendChildWidget(MMainSlicesWidget);
    MMainSlicesWidget->setHint("Number of slices in range");
    MMainSlicesWidget->selectButton(0);
    MMainSlicesWidget->setAllowZeroBits(false);
    MMainSlicesWidget->setValueIsBits(true,8);
    MMainSlicesWidget->setDrawBorder(false);
    MMainSlicesWidget->setBackgroundCellColor(SAT_Color(0.0, 0.3, 0.6) );
    MMainSlicesWidget->setActiveCellColor(    SAT_Color(0.0, 0.5, 1.0) );

    connect( MMainSlicesWidget,  AParameters->getItem(PAR_RANGE_SLICE_COUNT) );

    // loops

    SAT_TextWidget* prob_loops_label = new SAT_TextWidget( SAT_Rect(345,134,135,12), "Split Range" );
    page_prob->appendChildWidget(prob_loops_label);
    prob_loops_label->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    prob_loops_label->setTextColor(0.8);
    prob_loops_label->setTextSize(12);
    prob_loops_label->setDrawBorder(false);
    prob_loops_label->setFillBackground(false);

    MMainLoopsWidget = new SAT_ButtonRowWidget( SAT_Rect(345,148,135,16), 8, prob_slices_text, SAT_BUTTON_ROW_MULTI );
    page_prob->appendChildWidget(MMainLoopsWidget);
    MMainLoopsWidget->selectButton(0);
    MMainLoopsWidget->setAllowZeroBits(false);
    MMainLoopsWidget->setValueIsBits(true,8);
    MMainLoopsWidget->setHint("Range subdivision");
    MMainLoopsWidget->setDrawBorder(false);
    MMainLoopsWidget->setBackgroundCellColor( SAT_Color(0.0, 0.3, 0.6) );
    MMainLoopsWidget->setActiveCellColor(     SAT_Color(0.0, 0.5, 1.0) );

    connect( MMainLoopsWidget,  AParameters->getItem(PAR_RANGE_LOOP_COUNT) );

    //------------------------------
    //
    //------------------------------

    // txt range

    SAT_TextWidget* c1_header = new SAT_TextWidget(SAT_Rect(255,180,103,14),"Start");
    page_prob->appendChildWidget(c1_header);
    c1_header->setDrawBorder(true);
    c1_header->setBorderColor(0.6);
    c1_header->setBorderEdges(SAT_EDGE_BOTTOM);
    c1_header->setFillBackground(false);
    c1_header->setBackgroundColor(0.35);
    c1_header->setTextColor(0.8);
    c1_header->setTextSize(11);

    // txt loop

    SAT_TextWidget* c2_header = new SAT_TextWidget(SAT_Rect(377,180,103,14),"Wrap");
    page_prob->appendChildWidget(c2_header);
    c2_header->setDrawBorder(true);
    c2_header->setBorderColor(0.6);
    c2_header->setBorderEdges(SAT_EDGE_BOTTOM);
    c2_header->setFillBackground(false);
    c2_header->setBackgroundColor(0.35);
    c2_header->setTextColor(0.8);
    c2_header->setTextSize(11);

    // txt effects

    SAT_TextWidget* c3_header = new SAT_TextWidget(SAT_Rect(499,180,103,14),"Effects");
    page_prob->appendChildWidget(c3_header);
    c3_header->setDrawBorder(true);
    c3_header->setBorderColor(0.6);
    c3_header->setBorderEdges(SAT_EDGE_BOTTOM);
    c3_header->setFillBackground(false);
    c3_header->setBackgroundColor(0.35);
    c3_header->setTextColor(0.8);
    c3_header->setTextSize(11);

    //------------------------------
    //
    //------------------------------

    // range length

    SAT_TextWidget* range_length_text = new SAT_TextWidget(SAT_Rect(292,205,66,9),"Length");
    page_prob->appendChildWidget(range_length_text);
    range_length_text->setDrawBorder(false);
    range_length_text->setFillBackground(false);
    range_length_text->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    range_length_text->setTextColor(0.6);
    range_length_text->setTextSize(10);

    MRangeSizeWidget = new SAT_KnobWidget(SAT_Rect(255,205,33,33),"%", 0.0);
    page_prob->appendChildWidget(MRangeSizeWidget);
    MRangeSizeWidget->setArcThickness(7);
    MRangeSizeWidget->setHint("Loop length");
    MRangeSizeWidget->setTextColor(0.6);
    MRangeSizeWidget->setValueColor(0.8);
    MRangeSizeWidget->setArcBackColor(            SAT_Color(0.5, 0.0, 0.0) );
    MRangeSizeWidget->setArcValueColor(           SAT_Color(0.7, 0.0, 0.0) );
    MRangeSizeWidget->setInteractiveArcValueColor(SAT_Color(0.9, 0.0, 0.0) );

    connect( MRangeSizeWidget, AParameters->getItem(PAR_PROB_SIZE_PROB_RANGE) );

    MRangeSizeValueWidget = new SAT_DualSliderWidget( SAT_Rect(292,216,66,16), "%");
    page_prob->appendChildWidget(MRangeSizeValueWidget);
    MRangeSizeValueWidget->setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
    MRangeSizeValueWidget->setHint("Loop min/max");
    MRangeSizeValueWidget->setBackgroundColor( SAT_Color(0.5, 0.0, 0.0) );
    MRangeSizeValueWidget->setTextColor(              SAT_Color(0,0,0,0.5));
    MRangeSizeValueWidget->setSliderColor(            SAT_Color(0.7, 0.0, 0.0) );
    MRangeSizeValueWidget->setInteractiveSliderColor( SAT_Color(0.9, 0.0, 0.0) );

    connect( MRangeSizeValueWidget, AParameters->getItem(PAR_PROB_SIZE_MIN_RANGE), 0 );
    connect( MRangeSizeValueWidget, AParameters->getItem(PAR_PROB_SIZE_MAX_RANGE), 1 );

    // loop length

    SAT_TextWidget* loop_length_text = new SAT_TextWidget(SAT_Rect(414,205,66,9),"Length");
    page_prob->appendChildWidget(loop_length_text);
    loop_length_text->setDrawBorder(false);
    loop_length_text->setFillBackground(false);
    loop_length_text->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    loop_length_text->setTextColor(0.6);
    loop_length_text->setTextSize(10);

    MLoopSizeWidget = new SAT_KnobWidget(SAT_Rect(377,205,33,33),"%", 0.0);
    page_prob->appendChildWidget(MLoopSizeWidget);
    MLoopSizeWidget->setArcThickness(7);
    MLoopSizeWidget->setValueColor(0.8);
    MLoopSizeWidget->setTextColor(0.6);
    MLoopSizeWidget->setArcBackColor(             SAT_Color(0.5, 0.0, 0.0) );
    MLoopSizeWidget->setArcValueColor(            SAT_Color(0.7, 0.0, 0.0) );
    MLoopSizeWidget->setInteractiveArcValueColor( SAT_Color(0.9, 0.0, 0.0) );

    connect( MLoopSizeWidget, AParameters->getItem(PAR_PROB_SIZE_PROB_LOOP) );

    MLoopSizeValueWidget = new SAT_DualSliderWidget( SAT_Rect(414,216,66,16), "%", 0.5, 0.5 );
    page_prob->appendChildWidget(MLoopSizeValueWidget);
    MLoopSizeValueWidget->setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
    MLoopSizeValueWidget->setBackgroundColor(       SAT_Color(0.5, 0.0, 0.0) );
    MLoopSizeValueWidget->setSliderColor(           SAT_Color(0.7, 0.0, 0.0) );
    MLoopSizeValueWidget->setInteractiveSliderColor(SAT_Color(0.9, 0.0, 0.0) );
    MLoopSizeValueWidget->setTextColor(             SAT_Color(0, 0, 0, 0.5));

    connect( MLoopSizeValueWidget, AParameters->getItem(PAR_PROB_SIZE_MIN_LOOP), 0 );
    connect( MLoopSizeValueWidget, AParameters->getItem(PAR_PROB_SIZE_MAX_LOOP), 1 );

    //------------------------------

    // range speed

    SAT_TextWidget* range_speed_text = new SAT_TextWidget(SAT_Rect(292,240,66,9),"Speed");
    page_prob->appendChildWidget(range_speed_text);
    range_speed_text->setDrawBorder(false);
    range_speed_text->setFillBackground(false);
    range_speed_text->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    range_speed_text->setTextColor(0.6);
    range_speed_text->setTextSize(10);

    MRangeSpeedWidget = new SAT_KnobWidget(SAT_Rect(255,240,33,33),"%", 0.0);
    page_prob->appendChildWidget(MRangeSpeedWidget);
    MRangeSpeedWidget->setArcThickness(7);
    MRangeSpeedWidget->setValueColor(0.8);
    MRangeSpeedWidget->setTextColor(0.6);
    MRangeSpeedWidget->setArcBackColor(             SAT_Color(0.5, 0.25, 0.0) );
    MRangeSpeedWidget->setArcValueColor(            SAT_Color(0.7, 0.35, 0.0) );
    MRangeSpeedWidget->setInteractiveArcValueColor( SAT_Color(0.9, 0.45, 0.0) );

    connect( MRangeSpeedWidget, AParameters->getItem(PAR_PROB_SPEED_PROB_RANGE) );

    MRangeSpeedValueWidget= new SAT_DualSliderWidget( SAT_Rect(292,251,66,16), "%", 0.5, 0.5 );
    page_prob->appendChildWidget(MRangeSpeedValueWidget);
    MRangeSpeedValueWidget->setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
    MRangeSpeedValueWidget->setBackgroundColor(       SAT_Color(0.5, 0.25, 0.0) );
    MRangeSpeedValueWidget->setSliderColor(           SAT_Color(0.7, 0.35, 0.0) );
    MRangeSpeedValueWidget->setInteractiveSliderColor(SAT_Color(0.9, 0.45, 0.0) );
    MRangeSpeedValueWidget->setTextColor(             SAT_Color(0, 0, 0, 0.5));

    connect( MRangeSpeedValueWidget, AParameters->getItem(PAR_PROB_SPEED_MIN_RANGE), 0 );
    connect( MRangeSpeedValueWidget, AParameters->getItem(PAR_PROB_SPEED_MAX_RANGE), 1 );

    // loop speed

    SAT_TextWidget* loop_speed_text = new SAT_TextWidget(SAT_Rect(414,240,66,9),"Speed");
    page_prob->appendChildWidget(loop_speed_text);
    loop_speed_text->setDrawBorder(false);
    loop_speed_text->setFillBackground(false);
    loop_speed_text->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    loop_speed_text->setTextColor(0.6);
    loop_speed_text->setTextSize(10);

    MLoopSpeedWidget = new SAT_KnobWidget(SAT_Rect(377,240,33,33),"%", 0.0);
    page_prob->appendChildWidget(MLoopSpeedWidget);
    MLoopSpeedWidget->setArcThickness(7);
    MLoopSpeedWidget->setValueColor(0.8);
    MLoopSpeedWidget->setTextColor(0.6);
    MLoopSpeedWidget->setArcBackColor(            SAT_Color(0.5, 0.25, 0.0) );
    MLoopSpeedWidget->setArcValueColor(           SAT_Color(0.7, 0.35, 0.0) );
    MLoopSpeedWidget->setInteractiveArcValueColor(SAT_Color(0.9, 0.45, 0.0) );

    connect( MLoopSpeedWidget, AParameters->getItem(PAR_PROB_SPEED_PROB_LOOP) );

    MLoopSpeedValueWidget = new SAT_DualSliderWidget( SAT_Rect(414,251,66,16), "%", 0.5, 0.5 );
    page_prob->appendChildWidget(MLoopSpeedValueWidget);
    MLoopSpeedValueWidget->setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
    MLoopSpeedValueWidget->setBackgroundColor(        SAT_Color(0.5, 0.25, 0.0) );
    MLoopSpeedValueWidget->setSliderColor(            SAT_Color(0.7, 0.35, 0.0) );
    MLoopSpeedValueWidget->setInteractiveSliderColor( SAT_Color(0.9, 0.45, 0.0) );
    MLoopSpeedValueWidget->setTextColor(              SAT_Color(0, 0, 0, 0.5));

    connect( MLoopSpeedValueWidget, AParameters->getItem(PAR_PROB_SPEED_MIN_LOOP), 0 );
    connect( MLoopSpeedValueWidget, AParameters->getItem(PAR_PROB_SPEED_MAX_LOOP), 1 );

    //------------------------------

    // range offset

    SAT_TextWidget* range_offset_text = new SAT_TextWidget(SAT_Rect(292,275,66,9),"Offset");
    page_prob->appendChildWidget(range_offset_text);
    range_offset_text->setFillBackground(false);
    range_offset_text->setDrawBorder(false);
    range_offset_text->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    range_offset_text->setTextColor(0.6);
    range_offset_text->setTextSize(10);

    MRangeOffsetWidget = new SAT_KnobWidget(SAT_Rect(255,275,33,33),"%", 0.0);
    page_prob->appendChildWidget(MRangeOffsetWidget);
    MRangeOffsetWidget->setArcThickness(7);
    MRangeOffsetWidget->setValueColor(0.8);
    MRangeOffsetWidget->setTextColor(0.6);
    MRangeOffsetWidget->setArcBackColor(            SAT_Color(0.5, 0.5, 0.0) );
    MRangeOffsetWidget->setArcValueColor(           SAT_Color(0.7, 0.7, 0.0) );
    MRangeOffsetWidget->setInteractiveArcValueColor(SAT_Color(0.9, 0.9, 0.0) );

    connect( MRangeOffsetWidget, AParameters->getItem(PAR_PROB_OFFSET_PROB_RANGE) );

    MRangeOffsetValueWidget = new SAT_DualSliderWidget( SAT_Rect(292,286,66,16), "sl", 0.5, 0.5 );
    page_prob->appendChildWidget(MRangeOffsetValueWidget);
    MRangeOffsetValueWidget->setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
    MRangeOffsetValueWidget->setBackgroundColor(        SAT_Color(0.5, 0.5, 0.0) );
    MRangeOffsetValueWidget->setSliderColor(            SAT_Color(0.7, 0.7, 0.0) );
    MRangeOffsetValueWidget->setInteractiveSliderColor( SAT_Color(0.9, 0.9, 0.0) );
    MRangeOffsetValueWidget->setTextColor(              SAT_Color(0, 0, 0, 0.5));

    connect( MRangeOffsetValueWidget, AParameters->getItem(PAR_PROB_OFFSET_MIN_RANGE), 0 );
    connect( MRangeOffsetValueWidget, AParameters->getItem(PAR_PROB_OFFSET_MAX_RANGE), 1 );

    // loop offset

    SAT_TextWidget* loop_offset_text = new SAT_TextWidget(SAT_Rect(414,275,66,9),"Offset");
    page_prob->appendChildWidget(loop_offset_text);
    loop_offset_text->setDrawBorder(false);
    loop_offset_text->setFillBackground(false);
    loop_offset_text->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    loop_offset_text->setTextColor(0.6);
    loop_offset_text->setTextSize(10);

    MLoopOffsetWidget = new SAT_KnobWidget(SAT_Rect(377,275,33,33),"%", 0.0);
    page_prob->appendChildWidget(MLoopOffsetWidget);
    MLoopOffsetWidget->setArcThickness(7);
    MLoopOffsetWidget->setValueColor(0.8);
    MLoopOffsetWidget->setTextColor(0.6);
    MLoopOffsetWidget->setArcBackColor(             SAT_Color(0.5, 0.5, 0.0) );
    MLoopOffsetWidget->setArcValueColor(            SAT_Color(0.7, 0.7, 0.0) );
    MLoopOffsetWidget->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.0) );

    connect( MLoopOffsetWidget, AParameters->getItem(PAR_PROB_OFFSET_PROB_LOOP) );

    MLoopOffsetValueWidget = new SAT_DualSliderWidget( SAT_Rect(414,286,66,16), "sl", 0.5, 0.5 );
    page_prob->appendChildWidget(MLoopOffsetValueWidget);
    MLoopOffsetValueWidget->setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
    MLoopOffsetValueWidget->setBackgroundColor(       SAT_Color(0.5, 0.5, 0.0) );
    MLoopOffsetValueWidget->setSliderColor(           SAT_Color(0.7, 0.7, 0.0) );
    MLoopOffsetValueWidget->setInteractiveSliderColor(SAT_Color(0.9, 0.9, 0.0) );
    MLoopOffsetValueWidget->setTextColor(             SAT_Color(0, 0, 0, 0.5));

    connect( MLoopOffsetValueWidget, AParameters->getItem(PAR_PROB_OFFSET_MIN_LOOP), 0 );
    connect( MLoopOffsetValueWidget, AParameters->getItem(PAR_PROB_OFFSET_MAX_LOOP), 1 );

    //------------------------------

    // range reverse

    SAT_TextWidget* range_reverse_text = new SAT_TextWidget(SAT_Rect(292,310,66,9),"Reverse");
    page_prob->appendChildWidget(range_reverse_text);
    range_reverse_text->setFillBackground(false);
    range_reverse_text->setDrawBorder(false);
    range_reverse_text->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    range_reverse_text->setTextColor(0.6);
    range_reverse_text->setTextSize(10);

    MRangeReverseWidget = new SAT_KnobWidget(SAT_Rect(255,310,33,33),"%", 0.0);
    page_prob->appendChildWidget(MRangeReverseWidget);
    MRangeReverseWidget->setArcThickness(7);
    MRangeReverseWidget->setValueColor(0.8);
    MRangeReverseWidget->setTextColor(0.6);
    MRangeReverseWidget->setArcBackColor(             SAT_Color(0.25, 0.5, 0.0) );
    MRangeReverseWidget->setArcValueColor(            SAT_Color(0.35, 0.7, 0.0) );
    MRangeReverseWidget->setInteractiveArcValueColor( SAT_Color(0.45, 0.9, 0.0) );

    connect( MRangeReverseWidget, AParameters->getItem(PAR_PROB_REVERSE_PROB_RANGE) );

    // loop reverse

    SAT_TextWidget* loop_reverse_text = new SAT_TextWidget(SAT_Rect(414,310,66,9),"Reverse");
    page_prob->appendChildWidget(loop_reverse_text);
    loop_reverse_text->setDrawBorder(false);
    loop_reverse_text->setFillBackground(false);
    loop_reverse_text->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    loop_reverse_text->setTextColor(0.6);
    loop_reverse_text->setTextSize(10);

    MLoopReverseWidget = new SAT_KnobWidget(SAT_Rect(377,310,33,33),"%", 0.0);
    page_prob->appendChildWidget(MLoopReverseWidget);
    MLoopReverseWidget->setArcThickness(7);
    MLoopReverseWidget->setValueColor(0.8);
    MLoopReverseWidget->setTextColor(0.6);
    MLoopReverseWidget->setArcBackColor(            SAT_Color(0.25, 0.5, 0.0) );
    MLoopReverseWidget->setArcValueColor(           SAT_Color(0.35, 0.7, 0.0) );
    MLoopReverseWidget->setInteractiveArcValueColor(SAT_Color(0.45, 0.9, 0.0) );

    connect( MLoopReverseWidget, AParameters->getItem(PAR_PROB_REVERSE_PROB_LOOP) );

    //------------------------------

    // range fx

    SAT_TextWidget* range_fx_text = new SAT_TextWidget(SAT_Rect(292,345,66,9),"FX");
    page_prob->appendChildWidget(range_fx_text);
    range_fx_text->setFillBackground(false);
    range_fx_text->setDrawBorder(false);
    range_fx_text->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    range_fx_text->setTextColor(0.6);
    range_fx_text->setTextSize(10);

    MRangeFXWidget = new SAT_KnobWidget(SAT_Rect(255,345,33,33),"%", 0.0);
    page_prob->appendChildWidget(MRangeFXWidget);
    MRangeFXWidget->setArcThickness(7);
    MRangeFXWidget->setValueColor(0.8);
    MRangeFXWidget->setTextColor(0.6);
    MRangeFXWidget->setArcBackColor(             SAT_Color(0.0, 0.5, 0.0) );
    MRangeFXWidget->setArcValueColor(            SAT_Color(0.0, 0.7, 0.0) );
    MRangeFXWidget->setInteractiveArcValueColor( SAT_Color(0.0, 0.9, 0.0) );

    connect( MRangeFXWidget, AParameters->getItem(PAR_PROB_FX_PROB_RANGE) );

    MRangeFXValueWidget = new SAT_DualSliderWidget( SAT_Rect(292,356,66,16), "%", 0.5, 0.5 );
    page_prob->appendChildWidget(MRangeFXValueWidget);
    MRangeFXValueWidget->setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
    MRangeFXValueWidget->setBackgroundColor(        SAT_Color(0.0, 0.5, 0.0) );
    MRangeFXValueWidget->setSliderColor(            SAT_Color(0.0, 0.7, 0.0) );
    MRangeFXValueWidget->setInteractiveSliderColor( SAT_Color(0.0, 0.9, 0.0) );
    MRangeFXValueWidget->setTextColor(              SAT_Color(0, 0, 0, 0.5));

    //MRangeFXValueWidget->setSnap(true);

    connect( MRangeFXValueWidget, AParameters->getItem(PAR_PROB_FX_MIN_RANGE), 0 );
    connect( MRangeFXValueWidget, AParameters->getItem(PAR_PROB_FX_MAX_RANGE), 1 );

    // loop fx

    SAT_TextWidget* loop_fx_text = new SAT_TextWidget(SAT_Rect(414,345,66,9),"FX");
    page_prob->appendChildWidget(loop_fx_text);
    loop_fx_text->setDrawBorder(false);
    loop_fx_text->setFillBackground(false);
    loop_fx_text->setTextAlignment(SAT_TEXT_ALIGN_LEFT);
    loop_fx_text->setTextColor(0.6);
    loop_fx_text->setTextSize(10);

    MLoopFXWidget = new SAT_KnobWidget(SAT_Rect(377,345,33,33),"%", 0.0);
    page_prob->appendChildWidget(MLoopFXWidget);
    MLoopFXWidget->setArcThickness(7);
    MLoopFXWidget->setValueColor(0.8);
    MLoopFXWidget->setTextColor(0.6);
    MLoopFXWidget->setArcBackColor(            SAT_Color(0.0, 0.5, 0.0) );
    MLoopFXWidget->setArcValueColor(           SAT_Color(0.0, 0.7, 0.0) );
    MLoopFXWidget->setInteractiveArcValueColor(SAT_Color(0.0, 0.9, 0.0) );

    connect( MLoopFXWidget, AParameters->getItem(PAR_PROB_FX_PROB_LOOP) );

    MLoopFXValueWidget = new SAT_DualSliderWidget( SAT_Rect(414,356,66,16), "%", 0.5, 0.5 );
    page_prob->appendChildWidget(MLoopFXValueWidget);
    MLoopFXValueWidget->setCursor(SAT_CURSOR_ARROW_LEFT_RIGHT);
    MLoopFXValueWidget->setBackgroundColor(       SAT_Color(0.0, 0.5, 0.0) );
    MLoopFXValueWidget->setSliderColor(           SAT_Color(0.0, 0.7, 0.0) );
    MLoopFXValueWidget->setInteractiveSliderColor(SAT_Color(0.0, 0.9, 0.0) );
    MLoopFXValueWidget->setTextColor(             SAT_Color(0, 0, 0, 0.5));

    //MLoopFXValueWidget->setSnap(true);

    connect( MLoopFXValueWidget, AParameters->getItem(PAR_PROB_FX_MIN_LOOP), 0 );
    connect( MLoopFXValueWidget, AParameters->getItem(PAR_PROB_FX_MAX_LOOP), 1 );

    //------------------------------
    // Effects
    //------------------------------

    SAT_ButtonRowWidget* fx_mode_buttons = new SAT_ButtonRowWidget( SAT_Rect(499,154,103,10), FX_MODE_COUNT, fx_mode_text, SAT_BUTTON_ROW_SINGLE );
    page_prob->appendChildWidget(fx_mode_buttons);
    //fx_mode_buttons->setValue(0);
    //fx_mode_buttons->selectButton(0);
    fx_mode_buttons->setTextSize(8);
    fx_mode_buttons->setValueIsBits(false);//(true,8);
    //fx_mode_buttons->setAllowZeroBits(false);

    connect( fx_mode_buttons, AParameters->getItem(PAR_FX_MODE) );

    //------------------------------

    // FX 1

    MFXProbKnob[0] = new SAT_KnobWidget(SAT_Rect(499,205,33,33),"%", 0.0);
    page_prob->appendChildWidget(MFXProbKnob[0]);
    MFXProbKnob[0]->setArcThickness(7);
    MFXProbKnob[0]->setValueColor(0.8);
    MFXProbKnob[0]->setTextColor(0.6);
    MFXProbKnob[0]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXProbKnob[0]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXProbKnob[0]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );

    connect( MFXProbKnob[0], AParameters->getItem(PAR_FX1_PROB) );

    MFXTypeSelector[0] = new SAT_SelectorWidget(SAT_Rect(537,205,66,11), "Off", fx_type_menu);
    page_prob->appendChildWidget(MFXTypeSelector[0]);
    MFXTypeSelector[0]->setDrawBorder(true);
    MFXTypeSelector[0]->setBorderColor(0.4);
    MFXTypeSelector[0]->setBorderEdges(SAT_EDGE_BOTTOM);
    MFXTypeSelector[0]->setFillBackground(false);
    MFXTypeSelector[0]->setBackgroundColor(0.5);
    MFXTypeSelector[0]->setTextColor(0.8);
    MFXTypeSelector[0]->setTextSize(9);
    MFXTypeSelector[0]->setTextOffset(SAT_Rect(2,0,0,0));
    MFXTypeSelector[0]->setDrawArrow(true);
    MFXTypeSelector[0]->setArrowColor(0.5);

    connect( MFXTypeSelector[0], AParameters->getItem(PAR_FX1_TYPE) );

    // fx1 par 1

    MFXArgKnob[(0*3)+0] = new SAT_KnobWidget(SAT_Rect(537,220,18,18),"", 0.0);
    page_prob->appendChildWidget(MFXArgKnob[(0*3)+0]);
    MFXArgKnob[(0*3)+0]->setDrawText(false);
    MFXArgKnob[(0*3)+0]->setDrawValue(false);
    MFXArgKnob[(0*3)+0]->setArcThickness(4);
    MFXArgKnob[(0*3)+0]->setValueColor(0.8);
    MFXArgKnob[(0*3)+0]->setTextColor(0.6);
    MFXArgKnob[(0*3)+0]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXArgKnob[(0*3)+0]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXArgKnob[(0*3)+0]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );

    connect( MFXArgKnob[(0*3)+0], AParameters->getItem(PAR_FX1_PAR1) );

    // fx1 par 2

    MFXArgKnob[(0*3)+1] = new SAT_KnobWidget(SAT_Rect(557,220,18,18),"", 0.0);
    page_prob->appendChildWidget(MFXArgKnob[(0*3)+1]);
    MFXArgKnob[(0*3)+1]->setDrawText(false);
    MFXArgKnob[(0*3)+1]->setDrawValue(false);
    MFXArgKnob[(0*3)+1]->setArcThickness(4);
    MFXArgKnob[(0*3)+1]->setValueColor(0.8);
    MFXArgKnob[(0*3)+1]->setTextColor(0.6);
    MFXArgKnob[(0*3)+1]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXArgKnob[(0*3)+1]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXArgKnob[(0*3)+1]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );

    connect( MFXArgKnob[(0*3)+1], AParameters->getItem(PAR_FX1_PAR2) );

    // fx1 par 3

    MFXArgKnob[(0*3)+2] = new SAT_KnobWidget(SAT_Rect(577,220,18,18),"", 0.0);
    page_prob->appendChildWidget(MFXArgKnob[(0*3)+2]);
    MFXArgKnob[(0*3)+2]->setDrawText(false);
    MFXArgKnob[(0*3)+2]->setDrawValue(false);
    MFXArgKnob[(0*3)+2]->setArcThickness(4);
    MFXArgKnob[(0*3)+2]->setValueColor(0.8);
    MFXArgKnob[(0*3)+2]->setTextColor(0.6);
    MFXArgKnob[(0*3)+2]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXArgKnob[(0*3)+2]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXArgKnob[(0*3)+2]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );
    //MFXArgKnob[(0*3)+2]->setBipolar(true);
    //MFXArgKnob[(0*3)+2]->setSnap(true);
    //MFXArgKnob[(0*3)+2]->setSnapPos(0.5);
    //MFXArgKnob[(0*3)+2]->setSnapDist(0.07);

    connect( MFXArgKnob[(0*3)+2], AParameters->getItem(PAR_FX1_PAR3) );

    // FX 2

    MFXProbKnob[1] = new SAT_KnobWidget(SAT_Rect(499,205+35,33,33),"%", 0.0);
    page_prob->appendChildWidget(MFXProbKnob[1]);
    MFXProbKnob[1]->setArcThickness(7);
    MFXProbKnob[1]->setValueColor(0.8);
    MFXProbKnob[1]->setTextColor(0.6);
    MFXProbKnob[1]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXProbKnob[1]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXProbKnob[1]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );

    connect( MFXProbKnob[1], AParameters->getItem(PAR_FX2_PROB) );

    MFXTypeSelector[1] = new SAT_SelectorWidget(SAT_Rect(537,205+35,66,11), "Off", fx_type_menu);
    page_prob->appendChildWidget(MFXTypeSelector[1]);
    MFXTypeSelector[1]->setDrawBorder(true);
    MFXTypeSelector[1]->setBorderColor(0.4);
    MFXTypeSelector[1]->setBorderEdges(SAT_EDGE_BOTTOM);
    MFXTypeSelector[1]->setFillBackground(false);
    MFXTypeSelector[1]->setBackgroundColor(0.5);
    MFXTypeSelector[1]->setTextColor(0.8);
    MFXTypeSelector[1]->setTextSize(9);
    MFXTypeSelector[1]->setTextOffset(SAT_Rect(2,0,0,0));
    MFXTypeSelector[1]->setDrawArrow(true);
    MFXTypeSelector[1]->setArrowColor(0.5);

    connect( MFXTypeSelector[1], AParameters->getItem(PAR_FX2_TYPE) );

    // fx2 par 1

    MFXArgKnob[(1*3)+0] = new SAT_KnobWidget(SAT_Rect(537,220+35,18,18),"", 0.0);
    page_prob->appendChildWidget(MFXArgKnob[(1*3)+0]);
    MFXArgKnob[(1*3)+0]->setDrawText(false);
    MFXArgKnob[(1*3)+0]->setDrawValue(false);
    MFXArgKnob[(1*3)+0]->setArcThickness(4);
    MFXArgKnob[(1*3)+0]->setValueColor(0.8);
    MFXArgKnob[(1*3)+0]->setTextColor(0.6);
    MFXArgKnob[(1*3)+0]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXArgKnob[(1*3)+0]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXArgKnob[(1*3)+0]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );

    connect( MFXArgKnob[(1*3)+0], AParameters->getItem(PAR_FX2_PAR1) );

    // fx2 par 2

    MFXArgKnob[(1*3)+1] = new SAT_KnobWidget(SAT_Rect(557,220+35,18,18),"", 0.0);
    page_prob->appendChildWidget(MFXArgKnob[(1*3)+1]);
    MFXArgKnob[(1*3)+1]->setDrawText(false);
    MFXArgKnob[(1*3)+1]->setDrawValue(false);
    MFXArgKnob[(1*3)+1]->setArcThickness(4);
    MFXArgKnob[(1*3)+1]->setValueColor(0.8);
    MFXArgKnob[(1*3)+1]->setTextColor(0.6);
    MFXArgKnob[(1*3)+1]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXArgKnob[(1*3)+1]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXArgKnob[(1*3)+1]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );

    connect( MFXArgKnob[(1*3)+1], AParameters->getItem(PAR_FX2_PAR2) );

    // fx2 par 3

    MFXArgKnob[(1*3)+2] = new SAT_KnobWidget(SAT_Rect(577,220+35,18,18),"", 0.0);
    page_prob->appendChildWidget(MFXArgKnob[(1*3)+2]);
    MFXArgKnob[(1*3)+2]->setDrawText(false);
    MFXArgKnob[(1*3)+2]->setDrawValue(false);
    MFXArgKnob[(1*3)+2]->setArcThickness(4);
    MFXArgKnob[(1*3)+2]->setValueColor(0.8);
    MFXArgKnob[(1*3)+2]->setTextColor(0.6);
    MFXArgKnob[(1*3)+2]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXArgKnob[(1*3)+2]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXArgKnob[(1*3)+2]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );
    //MFXArgKnob[(1*3)+2]->setBipolar(true);
    //MFXArgKnob[(1*3)+2]->setSnap(true);
    //MFXArgKnob[(1*3)+2]->setSnapPos(0.5);
    //MFXArgKnob[(1*3)+2]->setSnapDist(0.07);

    connect( MFXArgKnob[(1*3)+2], AParameters->getItem(PAR_FX2_PAR3) );

    // FX 3

    MFXProbKnob[2] = new SAT_KnobWidget(SAT_Rect(499,205+35+35,33,33),"%", 0.0);
    page_prob->appendChildWidget(MFXProbKnob[2]);
    MFXProbKnob[2]->setArcThickness(7);
    MFXProbKnob[2]->setValueColor(0.8);
    MFXProbKnob[2]->setTextColor(0.6);
    MFXProbKnob[2]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXProbKnob[2]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXProbKnob[2]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );

    connect( MFXProbKnob[2], AParameters->getItem(PAR_FX3_PROB) );

    MFXTypeSelector[2] = new SAT_SelectorWidget(SAT_Rect(537,205+35+35,66,11), "Off", fx_type_menu);
    page_prob->appendChildWidget(MFXTypeSelector[2]);
    MFXTypeSelector[2]->setDrawBorder(true);
    MFXTypeSelector[2]->setBorderColor(0.4);
    MFXTypeSelector[2]->setBorderEdges(SAT_EDGE_BOTTOM);
    MFXTypeSelector[2]->setFillBackground(false);
    MFXTypeSelector[2]->setBackgroundColor(0.5);
    MFXTypeSelector[2]->setTextColor(0.8);
    MFXTypeSelector[2]->setTextSize(9);
    MFXTypeSelector[2]->setTextOffset(SAT_Rect(2,0,0,0));
    MFXTypeSelector[2]->setDrawArrow(true);
    MFXTypeSelector[2]->setArrowColor(0.5);

    connect( MFXTypeSelector[2], AParameters->getItem(PAR_FX3_TYPE) );

    // fx3 par 1

    MFXArgKnob[(2*3)+0] = new SAT_KnobWidget(SAT_Rect(537,220+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(MFXArgKnob[(2*3)+0]);
    MFXArgKnob[(2*3)+0]->setDrawText(false);
    MFXArgKnob[(2*3)+0]->setDrawValue(false);
    MFXArgKnob[(2*3)+0]->setArcThickness(4);
    MFXArgKnob[(2*3)+0]->setValueColor(0.8);
    MFXArgKnob[(2*3)+0]->setTextColor(0.6);
    MFXArgKnob[(2*3)+0]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXArgKnob[(2*3)+0]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXArgKnob[(2*3)+0]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );

    connect( MFXArgKnob[(2*3)+0], AParameters->getItem(PAR_FX3_PAR1) );

    // fx3 par 2

    MFXArgKnob[(2*3)+1] = new SAT_KnobWidget(SAT_Rect(557,220+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(MFXArgKnob[(2*3)+1]);
    MFXArgKnob[(2*3)+1]->setDrawText(false);
    MFXArgKnob[(2*3)+1]->setDrawValue(false);
    MFXArgKnob[(2*3)+1]->setArcThickness(4);
    MFXArgKnob[(2*3)+1]->setValueColor(0.8);
    MFXArgKnob[(2*3)+1]->setTextColor(0.6);
    MFXArgKnob[(2*3)+1]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXArgKnob[(2*3)+1]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXArgKnob[(2*3)+1]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );

    connect( MFXArgKnob[(2*3)+1], AParameters->getItem(PAR_FX3_PAR2) );

    // fx3 par 3

    MFXArgKnob[(2*3)+2] = new SAT_KnobWidget(SAT_Rect(577,220+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(MFXArgKnob[(2*3)+2]);
    MFXArgKnob[(2*3)+2]->setDrawText(false);
    MFXArgKnob[(2*3)+2]->setDrawValue(false);
    MFXArgKnob[(2*3)+2]->setArcThickness(4);
    MFXArgKnob[(2*3)+2]->setValueColor(0.8);
    MFXArgKnob[(2*3)+2]->setTextColor(0.6);
    MFXArgKnob[(2*3)+2]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXArgKnob[(2*3)+2]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXArgKnob[(2*3)+2]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );
    //MFXArgKnob[(2*3)+2]->setBipolar(true);
    //MFXArgKnob[(2*3)+2]->setSnap(true);
    //MFXArgKnob[(2*3)+2]->setSnapPos(0.5);
    //MFXArgKnob[(2*3)+2]->setSnapDist(0.07);

    connect( MFXArgKnob[(2*3)+2], AParameters->getItem(PAR_FX3_PAR3) );

    // FX 4

    MFXProbKnob[3] = new SAT_KnobWidget(SAT_Rect(499,205+35+35+35,33,33),"%", 0.0);
    page_prob->appendChildWidget(MFXProbKnob[3]);
    MFXProbKnob[3]->setArcThickness(7);
    MFXProbKnob[3]->setValueColor(0.8);
    MFXProbKnob[3]->setTextColor(0.6);
    MFXProbKnob[3]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXProbKnob[3]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXProbKnob[3]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );

    connect( MFXProbKnob[3], AParameters->getItem(PAR_FX4_PROB) );

    MFXTypeSelector[3] = new SAT_SelectorWidget(SAT_Rect(537,205+35+35+35,66,11), "Off", fx_type_menu);
    page_prob->appendChildWidget(MFXTypeSelector[3]);
    MFXTypeSelector[3]->setDrawBorder(true);
    MFXTypeSelector[3]->setBorderColor(0.4);
    MFXTypeSelector[3]->setBorderEdges(SAT_EDGE_BOTTOM);
    MFXTypeSelector[3]->setFillBackground(false);
    MFXTypeSelector[3]->setBackgroundColor(0.5);
    MFXTypeSelector[3]->setTextColor(0.8);
    MFXTypeSelector[3]->setTextSize(9);
    MFXTypeSelector[3]->setTextOffset(SAT_Rect(2,0,0,0));
    MFXTypeSelector[3]->setDrawArrow(true);
    MFXTypeSelector[3]->setArrowColor(0.5);

    connect( MFXTypeSelector[3], AParameters->getItem(PAR_FX4_TYPE) );

    // fx4 par 1

    MFXArgKnob[(3*3)+0] = new SAT_KnobWidget(SAT_Rect(537,220+35+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(MFXArgKnob[(3*3)+0]);
    MFXArgKnob[(3*3)+0]->setDrawText(false);
    MFXArgKnob[(3*3)+0]->setDrawValue(false);
    MFXArgKnob[(3*3)+0]->setArcThickness(4);
    MFXArgKnob[(3*3)+0]->setValueColor(0.8);
    MFXArgKnob[(3*3)+0]->setTextColor(0.6);
    MFXArgKnob[(3*3)+0]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXArgKnob[(3*3)+0]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXArgKnob[(3*3)+0]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );

    connect( MFXArgKnob[(3*3)+0], AParameters->getItem(PAR_FX4_PAR1) );

    // fx4 par 2

    MFXArgKnob[(3*3)+1] = new SAT_KnobWidget(SAT_Rect(557,220+35+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(MFXArgKnob[(3*3)+1]);
    MFXArgKnob[(3*3)+1]->setDrawText(false);
    MFXArgKnob[(3*3)+1]->setDrawValue(false);
    MFXArgKnob[(3*3)+1]->setArcThickness(4);
    MFXArgKnob[(3*3)+1]->setValueColor(0.8);
    MFXArgKnob[(3*3)+1]->setTextColor(0.6);
    MFXArgKnob[(3*3)+1]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXArgKnob[(3*3)+1]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXArgKnob[(3*3)+1]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );

    connect( MFXArgKnob[(3*3)+1], AParameters->getItem(PAR_FX4_PAR2) );

    // fx4 par 3

    MFXArgKnob[(3*3)+2] = new SAT_KnobWidget(SAT_Rect(577,220+35+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(MFXArgKnob[(3*3)+2]);
    MFXArgKnob[(3*3)+2]->setDrawText(false);
    MFXArgKnob[(3*3)+2]->setDrawValue(false);
    MFXArgKnob[(3*3)+2]->setArcThickness(4);
    MFXArgKnob[(3*3)+2]->setValueColor(0.8);
    MFXArgKnob[(3*3)+2]->setTextColor(0.6);
    MFXArgKnob[(3*3)+2]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXArgKnob[(3*3)+2]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXArgKnob[(3*3)+2]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );
    //MFXArgKnob[(3*3)+2]->setBipolar(true);
    //MFXArgKnob[(3*3)+2]->setSnap(true);
    //MFXArgKnob[(3*3)+2]->setSnapPos(0.5);
    //MFXArgKnob[(3*3)+2]->setSnapDist(0.07);

    connect( MFXArgKnob[(3*3)+2], AParameters->getItem(PAR_FX4_PAR3) );

    // FX 5

    MFXProbKnob[4] = new SAT_KnobWidget(SAT_Rect(499,205+35+35+35+35,33,33),"%", 0.0);
    page_prob->appendChildWidget(MFXProbKnob[4]);
    MFXProbKnob[4]->setArcThickness(7);
    MFXProbKnob[4]->setValueColor(0.8);
    MFXProbKnob[4]->setTextColor(0.6);
    MFXProbKnob[4]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXProbKnob[4]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXProbKnob[4]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );

    connect( MFXProbKnob[4], AParameters->getItem(PAR_FX5_PROB) );

    MFXTypeSelector[4] = new SAT_SelectorWidget(SAT_Rect(537,205+35+35+35+35,66,11), "Off", fx_type_menu);
    page_prob->appendChildWidget(MFXTypeSelector[4]);
    MFXTypeSelector[4]->setDrawBorder(true);
    MFXTypeSelector[4]->setBorderColor(0.4);
    MFXTypeSelector[4]->setBorderEdges(SAT_EDGE_BOTTOM);
    MFXTypeSelector[4]->setFillBackground(false);
    MFXTypeSelector[4]->setBackgroundColor(0.5);
    MFXTypeSelector[4]->setTextColor(0.8);
    MFXTypeSelector[4]->setTextSize(9);
    MFXTypeSelector[4]->setTextOffset(SAT_Rect(2,0,0,0));
    MFXTypeSelector[4]->setDrawArrow(true);
    MFXTypeSelector[4]->setArrowColor(0.5);

    connect( MFXTypeSelector[4], AParameters->getItem(PAR_FX5_TYPE) );

    // fx4 par 1

    MFXArgKnob[(4*3)+0] = new SAT_KnobWidget(SAT_Rect(537,220+35+35+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(MFXArgKnob[(4*3)+0]);
    MFXArgKnob[(4*3)+0]->setDrawText(false);
    MFXArgKnob[(4*3)+0]->setDrawValue(false);
    MFXArgKnob[(4*3)+0]->setArcThickness(4);
    MFXArgKnob[(4*3)+0]->setValueColor(0.8);
    MFXArgKnob[(4*3)+0]->setTextColor(0.6);
    MFXArgKnob[(4*3)+0]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXArgKnob[(4*3)+0]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXArgKnob[(4*3)+0]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );

    connect( MFXArgKnob[(4*3)+0], AParameters->getItem(PAR_FX5_PAR1) );

    // fx4 par 2

    MFXArgKnob[(4*3)+1] = new SAT_KnobWidget(SAT_Rect(557,220+35+35+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(MFXArgKnob[(4*3)+1]);
    MFXArgKnob[(4*3)+1]->setDrawText(false);
    MFXArgKnob[(4*3)+1]->setDrawValue(false);
    MFXArgKnob[(4*3)+1]->setArcThickness(4);
    MFXArgKnob[(4*3)+1]->setValueColor(0.8);
    MFXArgKnob[(4*3)+1]->setTextColor(0.6);
    MFXArgKnob[(4*3)+1]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXArgKnob[(4*3)+1]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXArgKnob[(4*3)+1]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );

    connect( MFXArgKnob[(4*3)+1], AParameters->getItem(PAR_FX5_PAR2) );

    // fx4 par 3

    MFXArgKnob[(4*3)+2] = new SAT_KnobWidget(SAT_Rect(577,220+35+35+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(MFXArgKnob[(4*3)+2]);
    MFXArgKnob[(4*3)+2]->setDrawText(false);
    MFXArgKnob[(4*3)+2]->setDrawValue(false);
    MFXArgKnob[(4*3)+2]->setArcThickness(4);
    MFXArgKnob[(4*3)+2]->setValueColor(0.8);
    MFXArgKnob[(4*3)+2]->setTextColor(0.6);
    MFXArgKnob[(4*3)+2]->setArcBackColor(             SAT_Color(0.5, 0.5, 0.5) );
    MFXArgKnob[(4*3)+2]->setArcValueColor(            SAT_Color(0.7, 0.7, 0.7) );
    MFXArgKnob[(4*3)+2]->setInteractiveArcValueColor( SAT_Color(0.9, 0.9, 0.9) );
    //MFXArgKnob[(4*3)+2]->setBipolar(true);
    //MFXArgKnob[(4*3)+2]->setSnap(true);
    //MFXArgKnob[(4*3)+2]->setSnapPos(0.5);
    //MFXArgKnob[(4*3)+2]->setSnapDist(0.07);

    connect( MFXArgKnob[(4*3)+2], AParameters->getItem(PAR_FX5_PAR3) );

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------
