#ifndef sat_text_widget_included
#define sat_text_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widgets/sat_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_TextWidget
: public SAT_PanelWidget {

public:

  SAT_TextWidget(SAT_Rect ARect, const char* AText="")
  : SAT_PanelWidget(ARect) {
    setText(AText);
    setTextColor(SAT_Black);
    setDrawText(true);
    setDrawBorder(false);
    setFillBackground(false);
    setTextAlignment(SAT_TEXT_ALIGN_LEFT);
  }

  virtual ~SAT_TextWidget() {
  }

};

//----------------------------------------------------------------------
#endif