#ifndef sa_demo_page_layout_included
#define sa_demo_page_layout_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widgets.h"

class sa_demo_page_layout
: public SAT_PanelWidget {
  
//------------------------------
public:
//------------------------------

  sa_demo_page_layout(SAT_Rect ARect, SAT_Plugin* APlugin)
  : SAT_PanelWidget(ARect) {
    setName("sa_demo_page_layout");
    setStretching(SAT_WIDGET_STRETCH_ALL);
    setInnerBorder(SAT_Rect(10,10,10,10));
    //setSpacing(SAT_Point(5,5));
    
    // left panel
    
    SAT_PanelWidget* left_panel = new SAT_PanelWidget(200);
    appendChildWidget(left_panel);
    left_panel->setLayout(SAT_WIDGET_ALIGN_LEFT_TOP,SAT_WIDGET_STRETCH_VERTICAL);
    left_panel->setWidthLimits(100,400);
    left_panel->setFillBackground(false);
    left_panel->setDrawBorder(false);
    {
    }
    
    SAT_SizerWidget* sizer = new SAT_SizerWidget(SAT_Rect(3),SAT_DIRECTION_LEFT,left_panel);
    appendChildWidget(sizer);
    sizer->setFillBackground(true);
    sizer->setBackgroundColor(0.3);
    sizer->setDrawBorder(false);

    // right panel
    
    SAT_PanelWidget* right_panel = new SAT_PanelWidget(0);
    appendChildWidget(right_panel);
    right_panel->setLayout(SAT_WIDGET_ALIGN_FILL,SAT_WIDGET_STRETCH_ALL);
    right_panel->setFillBackground(false);
    right_panel->setDrawBorder(false);
    
    {
      
      // top panel
    
      SAT_PanelWidget* top_panel = new SAT_PanelWidget(200);
      right_panel->appendChildWidget(top_panel);
      top_panel->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,SAT_WIDGET_STRETCH_HORIZONTAL);
      top_panel->setHeightLimits(50,400);
      top_panel->setInnerBorder(SAT_Rect(5,5,5,5));
      top_panel->setSpacing(SAT_Point(5,5));
      top_panel->setDrawBorder(false);
      {
        
        SAT_PanelWidget* top_box1 = new SAT_PanelWidget(200);
        top_panel->appendChildWidget(top_box1);
        top_box1->setLayout(SAT_WIDGET_ALIGN_LEFT_TOP,SAT_WIDGET_STRETCH_VERTICAL);
        top_box1->setInnerBorder(SAT_Rect(5,5,5,5));
        top_box1->setSpacing(SAT_Point(5,5));
        top_box1->setFillBackground(true);
        top_box1->setBackgroundColor(0.33);
        {
          top_box1->appendChildWidget(new SAT_TextWidget(SAT_Rect(75,15),"left")   )->setLayout(SAT_WIDGET_ALIGN_TOP_LEFT,    SAT_WIDGET_STRETCH_NONE);
          top_box1->appendChildWidget(new SAT_TextWidget(SAT_Rect(75,15),"right")  )->setLayout(SAT_WIDGET_ALIGN_TOP_RIGHT,   SAT_WIDGET_STRETCH_NONE);
          top_box1->appendChildWidget(new SAT_TextWidget(SAT_Rect(75,15),"center") )->setLayout(SAT_WIDGET_ALIGN_TOP_CENTER,  SAT_WIDGET_STRETCH_NONE);
          top_box1->appendChildWidget(new SAT_TextWidget(SAT_Rect(75,15),"center, stretch left") )->setLayout(SAT_WIDGET_ALIGN_TOP_CENTER,  SAT_WIDGET_STRETCH_LEFT);
          top_box1->appendChildWidget(new SAT_TextWidget(SAT_Rect(75,15),"center, stretch right"))->setLayout(SAT_WIDGET_ALIGN_TOP_CENTER,  SAT_WIDGET_STRETCH_RIGHT);
          top_box1->appendChildWidget(new SAT_TextWidget(SAT_Rect(75,15),"bottom right"))->setLayout(SAT_WIDGET_ALIGN_BOTTOM_RIGHT,SAT_WIDGET_STRETCH_NONE);
        }

        SAT_SizerWidget* top_sizer = new SAT_SizerWidget(SAT_Rect(3),SAT_DIRECTION_LEFT,top_box1);
        top_panel->appendChildWidget(top_sizer);
        top_sizer->setFillBackground(true);
        top_sizer->setBackgroundColor(0.3);
        top_sizer->setDrawBorder(false);

        SAT_PanelWidget* top_box2 = new SAT_PanelWidget(200);
        top_panel->appendChildWidget(top_box2);
        top_box2->setLayout(SAT_WIDGET_ALIGN_LEFT,SAT_WIDGET_STRETCH_VERTICAL);
        top_box2->setFillBackground(true);
        top_box2->setBackgroundColor(0.33);
        {
          top_box2->appendChildWidget(new SAT_TextWidget(SAT_Rect(40,60),"top")     )->setLayout(SAT_WIDGET_ALIGN_LEFT_TOP,   SAT_WIDGET_STRETCH_NONE);
          top_box2->appendChildWidget(new SAT_TextWidget(SAT_Rect(40,60),"bottom")  )->setLayout(SAT_WIDGET_ALIGN_LEFT_BOTTOM,SAT_WIDGET_STRETCH_NONE);
          top_box2->appendChildWidget(new SAT_TextWidget(SAT_Rect(40,60),"center")  )->setLayout(SAT_WIDGET_ALIGN_LEFT_CENTER,SAT_WIDGET_STRETCH_NONE);
          top_box2->appendChildWidget(new SAT_TextWidget(SAT_Rect(40,60),"s_top")   )->setLayout(SAT_WIDGET_ALIGN_LEFT_CENTER,SAT_WIDGET_STRETCH_TOP);
          top_box2->appendChildWidget(new SAT_TextWidget(SAT_Rect(40,60),"s_bottom"))->setLayout(SAT_WIDGET_ALIGN_LEFT_CENTER,SAT_WIDGET_STRETCH_BOTTOM);
        }

      }
    
      SAT_SizerWidget* sizer = new SAT_SizerWidget(SAT_Rect(3),SAT_DIRECTION_TOP,top_panel);
      right_panel->appendChildWidget(sizer);
      sizer->setFillBackground(true);
      sizer->setBackgroundColor(0.3);
      sizer->setDrawBorder(false);
      
      // bottom panel
      
      SAT_PanelWidget* bottom_panel = new SAT_PanelWidget(0);
      right_panel->appendChildWidget(bottom_panel);
      bottom_panel->setLayout(SAT_WIDGET_ALIGN_FILL,SAT_WIDGET_STRETCH_ALL);
      bottom_panel->setInnerBorder(SAT_Rect(5,5,5,5));
      bottom_panel->setSpacing(SAT_Point(5,5));
      bottom_panel->setDrawBorder(false);
      {
        
        SAT_PanelWidget* bottom_box1 = new SAT_PanelWidget(100);
        bottom_panel->appendChildWidget(bottom_box1);
        bottom_box1->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);

        SAT_PanelWidget* bottom_box2 = new SAT_PanelWidget(200);
        bottom_panel->appendChildWidget(bottom_box2);
        bottom_box2->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
        
      }
      
    }

  }
  
  //----------

  virtual ~sa_demo_page_layout() {
  }

};

//----------------------------------------------------------------------
#endif