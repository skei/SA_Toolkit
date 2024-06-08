#ifndef sat_visual_widget_included
#define sat_visual_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_widget.h"
//#include "plugin/sat_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_VisualWidget
: public SAT_Widget {

//------------------------------
protected:
//------------------------------

  //SAT_Rect    MCreateRect;
  //SAT_Rect    MLayoutRect;
  //SAT_Rect    MRect;

  SAT_Rect    MActiveOffset = { 0,0,0,0 };  // active hot-spot/rect

  bool        MIsActive  = true;
  bool        MIsEnabled = true;
  bool        MIsVisible = true;

//------------------------------
public:
//------------------------------

  SAT_VisualWidget(SAT_Rect ARect)
  : SAT_Widget(ARect) {
    setName("SAT_VisualWidget");
  }

  virtual ~SAT_VisualWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setActive(bool AActive=true)   { MIsActive = AActive; }
  virtual void setEnabled(bool AEnabled=true) { MIsEnabled = AEnabled; }
  virtual void setVisible(bool AVisible=true) { MIsVisible = AVisible; }

  virtual bool isActive() { return MIsActive; }
  virtual bool isEnabled() { return MIsEnabled; }
  virtual bool isVisible() { return MIsVisible; }

};

//----------------------------------------------------------------------
#endif
