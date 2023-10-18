#ifndef sat_parameter_widget_included
#define sat_parameter_widget_included
//----------------------------------------------------------------------

#include "sat.h"
//#include "gui/base/sat_base_widget.h"
//#include "gui/base/sat_widget_listener.h"
//#include "gui/base/sat_paint_context.h"

//----------------------------------------------------------------------

class SAT_ParameterWidget
: public SAT_Widget {

//------------------------------
private:
//------------------------------

  SAT_Parameter*      MParameter    = nullptr;

//------------------------------
public:
//------------------------------

  SAT_ParameterWidget()
  : SAT_Widget() {
  }

  //----------

  virtual ~SAT_ParameterWidget() {
  }

//------------------------------
public:
//------------------------------

  SAT_Parameter* getParameter() { return MParameter; }
  void setParameter(SAT_Parameter* AParameter) { MParameter = AParameter; }

};

//----------------------------------------------------------------------
#endif
