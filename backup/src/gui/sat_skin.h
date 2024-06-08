#ifndef sat_skin_incuded
#define sat_skin_incuded
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_paint_context.h"

struct SAT_Skin {

  SAT_Color backgroundColor;
  SAT_Color foregroundColor;
  SAT_Color borderColor;
  SAT_Color textColor;
  SAT_Color valueColor;
  SAT_Color highlightColor;

  //------------------------------

  SAT_Skin() {
    backgroundColor = SAT_DarkGrey;
    foregroundColor = SAT_LightGrey;
    borderColor     = SAT_Black;
    textColor       = SAT_White;
    valueColor      = SAT_Black;
    highlightColor  = SAT_Red;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct SAT_DefaultSkin
: SAT_Skin {

  SAT_DefaultSkin()
  : SAT_Skin() {
    // backgroundColor = SAT_DarkGrey;
    // foregroundColor = SAT_LightGrey;
    // borderColor     = SAT_Black;
    // textColor       = SAT_White;
    // valueColor      = SAT_Black;
    // highlightColor  = SAT_Red;
  }

};

//----------------------------------------------------------------------
#endif
