#ifndef sat_value_graph_widget_included
#define sat_value_graph_widget_included
//----------------------------------------------------------------------

#include "base/utils/sat_random.h"
#include "gui/sat_widget.h"
#include "gui/widgets/sat_panel_widget.h"

//----------

#define SAT_NUM_WIDGETNODES 1024

struct SAT_ValueGraphNode {
  float     value;
  uint32_t  flags;
};

//----------------------------------------------------------------------

class SAT_ValueGraphWidget
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------

  SAT_Color      MBackColor;
  SAT_Color      MLineColor;
  SAT_Color      MNodeColor;
  int32_t         MNumNodes;
  SAT_ValueGraphNode MNodes[SAT_NUM_WIDGETNODES];
  int32_t         MSelected;

//------------------------------
public:
//------------------------------

  SAT_ValueGraphWidget(SAT_Rect ARect, int32_t ANum)
  : SAT_PanelWidget(ARect) {
    setName("SAT_ValueGraphWidget");
    //setHint("valuegraph");
    MBackColor = SAT_DarkGrey;
    MLineColor = SAT_LightGrey;
    MNodeColor = SAT_BrightYellow;
    MNumNodes = ANum;
    memset(MNodes,0,sizeof(MNodes));
    MSelected = -1;
    //setFlag(SAT_WIDGET_HOVER);
    ////setFlag(kwf_clip);
    for (int32_t i=0; i<SAT_NUM_WIDGETNODES; i++) {
      MNodes[i].value = SAT_RandomSigned();
      MNodes[i].flags = 0;
    }
    //Options.wantHoverEvents = true;
  }

//------------------------------
public:
//------------------------------

  void numNodes(int32_t ANum) {
    MNumNodes = ANum;
  }

  //----------
    
  void setNode(int32_t AIndex, float AValue, uint32_t AFlags) {
    MNodes[AIndex].value = AValue;
    MNodes[AIndex].flags = AFlags;
  }

  //----------
    
  void appendNode(float AValue, uint32_t AFlags) {
    MNodes[MNumNodes].value = AValue;
    MNodes[MNumNodes].flags = AFlags;
    MNumNodes++;
  }

  //----------
    
  int32_t findNode(int32_t AXpos, int32_t AYpos) {
    float w = (float)(AXpos - getRect().x) / (float)getRect().w; // 0..1
    w *= (MNumNodes-1);
    int32_t index = roundf(w);// SMin( MNumNodes-1, roundf(w) );
    //STrace("w %f index %i\n",w,index);
    return index;//-1;
  }
    
  //----------
    
  void drawValueGraph(SAT_PaintContext* AContext) {
    
    SAT_Rect mrect = getRect();
    SAT_Painter* painter = AContext->painter;
    double S = getWindowScale();
    
    painter->pushClip(getRect());
    
    if (MNumNodes>0) {
      //int32 halfw = (mrect.w/2);
      int32_t halfh = (mrect.h/2);
      if (MNumNodes==1) {
        int32_t h = halfh - ( (MNodes[0].value * halfh ) );
        painter->setDrawColor(MLineColor);
        painter->drawLine(mrect.x,mrect.y+h,mrect.x2(),mrect.y+h);
      }
      else if (MNumNodes==2) {
        int32_t h1 = halfh - ( (MNodes[0].value * halfh ) );
        int32_t h2 = halfh - ( (MNodes[1].value * halfh ) );
        painter->setDrawColor(MLineColor);
        painter->drawLine(mrect.x,mrect.y+h1,mrect.x2(),mrect.y+h2);
      }
      else {
        float w = (float)mrect.w / (float)(MNumNodes-1);
        float x = mrect.x;
        for (int32_t i=0; i<MNumNodes; i++) {
          int32_t h1,h2;
          h1 = halfh - ( (MNodes[i].value * halfh ) );
          if (i<MNumNodes-1) {
            h2 = halfh - ( (MNodes[i+1].value * halfh ) );
            painter->setDrawColor(MLineColor);
            painter->drawLine(x,mrect.y+h1,x+w,mrect.y+h2);
          }
          if (i==MSelected) {
            painter->setFillColor(MNodeColor);
            double s3 = S * 3;
            double s6 = S * 6;
            painter->fillRect(x-s3,mrect.y+h1-s3,s6,s6);
          }
          x += w;
        }
      }
    }
    
    painter->popClip();
    
    
    
  }
   

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawValueGraph(AContext);
    drawBorder(AContext);
  }

//  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

//  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

  void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATime=0) override {
      //SWidget::on_mouseMove(AXpos,AYpos,AState);

      //float w = (float)(AXpos - mrect.x) / (float)mrect.w; // 0..1
      //int32 index = (w * (float)MNumNodes);

      int32_t index = findNode(AXpos,AYpos);
      if (index != MSelected) {
        MSelected = index;
        //STrace("MSelected: %i\n",MSelected);
        do_widget_redraw(this,0);
      }
  }

//  void on_widget_enter(float AXpos, float AYpos, SAT_Widget* AFrom, uint32_t ATimeStamp=0) override {
//    //if (flags.autoCursor) do_widget_setMouseCursor(this,MCursor);
//    //if (flags.autoHint) do_widget_setHint(this,MHint);
//  }

  void on_widget_mouse_leave(SAT_Widget* ATo, double AXpos, double AYpos, uint32_t ATime=0) override {
//    //if (flags.autoCursor) do_widget_setMouseCursor(this,SAT_CURSOR_DEFAULT);
//    //if (flags.autoHint) do_widget_setHint(this,"");
      //SWidget::on_leave(AWidget);
      if (MSelected>=0) {
        MSelected = -1;
        do_widget_redraw(this,0);
      }
      SAT_Widget::on_widget_mouse_leave(ATo,AXpos,AYpos,ATime);
  }

};

#undef SAT_NUM_WIDGETNODES

//----------------------------------------------------------------------
#endif
