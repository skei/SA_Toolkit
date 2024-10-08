#ifndef sat_grid_widget_included
#define sat_grid_widget_included
//----------------------------------------------------------------------

/*
  todo:
  - selected x1,y1,x2,y2
    invert if necesary
    SAT_List<cells>
  - scroll?
  - cut, copy, paste
  - move/drag
*/

//----------------------------------------------------------------------

#include "sat.h"
#include "base/util/sat_math.h"
#include "gui/widget/sat_visual_widget.h"

//#include "gui/sat_widget.h"
//#include "base/utils/sat_math.h"
//#include "gui/sat_paint_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_GridWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  bool      MIsDragging           = false;
  bool      MIsMoving             = false;
  int32_t   MClickedX             = -1;
  int32_t   MClickedY             = -1;
  uint32_t  MClickedB             = 0;
  int32_t   MSelectedX            = -1;
  int32_t   MSelectedY            = -1;
  int32_t   MSelectedXcount       = 0;
  int32_t   MSelectedYcount       = 0;

//------------------------------
protected:
//------------------------------

  int32_t   MNumColumns               = 0;
  int32_t   MNumRows                  = 0;

  bool      MSelectCell               = false;//true;
  bool      MSelectMultipleCells      = false;//true;
  bool      MDragCell                 = false;//true;

  bool      MDrawCells                = true;
  bool      MDrawHorizontalLines      = true;
  bool      MDrawVerticalLines        = true;
  bool      MDrawSelectedCells        = true;
  SAT_Color MSelectedCellBorderColor  = SAT_White;

  SAT_Color MGridColor                = SAT_Color(0.25);//SAT_COLOR_DARK_GRAY;
  double    MGridLineWidth            = 1.0;

  //bool      MFillBackground           = false;
  //SAT_Color MBackgroundColor          = SAT_COLOR_GRAY;

//------------------------------
public:
//------------------------------

  SAT_GridWidget(SAT_Rect ARect, uint32_t AColumns, uint32_t ARows)
  : SAT_VisualWidget(ARect) {
    setName("SAT_GridWidget");
    setHint("SAT_GridWidget");
    setCursor(SAT_CURSOR_FINGER);
    setNumCells(AColumns,ARows);
    //setDrawBorder(true);
  }

  //----------

  virtual ~SAT_GridWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual int32_t getNumColumns()                           { return MNumColumns; }
  virtual int32_t getnumRows()                              { return MNumRows; }

  virtual void    setNumColumns(int32_t AWidth)             { MNumColumns = AWidth; }
  virtual void    setNumRows(int32_t AHeight)               { MNumRows = AHeight; }
  virtual void    setNumCells(int32_t AX, int32_t AY)       { MNumColumns = AX; MNumRows = AY; }

  virtual void    setCanSelectCell(bool ASelect=true)          { MSelectCell = ASelect; }
  virtual void    setCanSelectMultipleCells(bool ASelect=true) { MSelectMultipleCells = ASelect; }
  virtual void    setCanDragCell(bool ADrag=true)              { MDragCell = ADrag; }

  virtual void    setDrawCells(bool ADraw=true)             { MDrawCells = ADraw; }
  virtual void    setDrawLines(bool H=true, bool V=true)    { MDrawHorizontalLines = H; MDrawVerticalLines = V; }
  virtual void    setDrawHorizontalLines(bool ADraw=true)   { MDrawHorizontalLines = ADraw; }
  virtual void    setDrawVerticalLines(bool ADraw=true)     { MDrawVerticalLines = ADraw; }
  virtual void    setDrawSelectedCells(bool ADraw=true)     { MDrawSelectedCells = ADraw; }
//virtual void    setBackgroundColor(SAT_Color AColor)      { MBackgroundColor = AColor; }
  virtual void    setGridColor(SAT_Color AColor)            { MGridColor = AColor; }

//------------------------------
public:
//------------------------------

  virtual void selectCell(int32_t AX, int32_t AY, bool AState=true) {
    if (AState) {
      MSelectedX = AX;
      MSelectedY = AY;
      MSelectedXcount = 1;
      MSelectedYcount = 1;
    }
    else {
      MSelectedX = -1;
      MSelectedY = -1;
      MSelectedXcount = 0;
      MSelectedYcount = 0;
    }
  }

  //MSelectCell = AState;
  //MSelectMultipleCells = false;

//------------------------------
public:
//------------------------------

  virtual void on_initCell(SAT_Rect ARect, int32_t AX, int32_t AY) {}
  virtual void on_clickCell(int32_t AX, int32_t AY, int32_t AB, int32_t AS) {}
  virtual void on_releaseCell(int32_t AX, int32_t AY, int32_t AB, int32_t AS) {}
  virtual void on_dragCell(int32_t AX, int32_t AY, int32_t AB, int32_t AS) {}
  virtual void on_paintCell(SAT_PaintContext* AContext, SAT_Rect ARect, int32_t AX, int32_t AY) {}
  virtual void on_selectCells(int32_t AX, int32_t AY, int32_t AW, int32_t AH) {}

//------------------------------
public:
//------------------------------

  virtual void initCells(void) {
    SAT_Rect mrect = getRect();
    if ((MNumColumns > 0) && (MNumRows > 0)) {
      float xcell = ( mrect.w / (float)MNumColumns );
      float ycell = ( mrect.h / (float)MNumRows );
      for (int32_t yy=0; yy<MNumRows; yy++) {
        for (int32_t xx=0; xx<MNumColumns; xx++) {
          SAT_Rect R = SAT_Rect(
            mrect.x + /*floorf*/(xx * xcell),
            mrect.y + /*floorf*/(yy * ycell),
                      /*floorf*/(     xcell),
                      /*floorf*/(     ycell)
          );
          on_initCell(R,xx,yy);
        }
      }
    }
  }

  //----------

  /*
  virtual void redrawCell(int32_t x, int32_t y) {
    float xcell = ( (float)MRect.w / (float)MNumColumns );
    float ycell = ( (float)MRect.h / (float)MNumRows );
    MSelectedX = x;
    MSelectedY = y;
    //SAT_Trace("%.3f %.3f %.3f %.3f\n",x*xcell,y*ycell,xcell,ycell);
    do_redraw(this,SAT_Rect(x*xcell,y*ycell,xcell,ycell));
  }
  */

//------------------------------
public:
//------------------------------

  /*
    TODO
    - only draw visible cells..
    - cells intersecting with update-rect
  */

  void on_Widget_paint(SAT_PaintContext* AContext) override {
    SAT_Rect mrect = getRect();
    SAT_Painter* painter = AContext->painter;
    double S = getWindowScale();

    //drawDropShadow(AContext); // grid will fill entire rect
    fillBackground(AContext);

    if ((MNumColumns > 0) && (MNumRows > 0)) {
      float xcell = ( mrect.w / (float)MNumColumns );
      float ycell = ( mrect.h / (float)MNumRows );

      // cells
      if (MDrawCells) {
        for (int32_t xx=0; xx<MNumColumns; xx++) {
          for (int32_t yy=0; yy<MNumRows; yy++) {
            SAT_Rect R = SAT_Rect(
              mrect.x + /*floorf*/(xx * xcell),
              mrect.y + /*floorf*/(yy * ycell),
              /*floorf*/(xcell),
              /*floorf*/(ycell)
            );
            on_paintCell(AContext,R,xx,yy);
          }
        }
      }

      // grid-lines
      float x = mrect.x + xcell - 1;
      float y = mrect.y + ycell - 1;

      if (MDrawVerticalLines) {
        if (MNumColumns > 1) {
          painter->setLineWidth(MGridLineWidth * S);
          painter->setDrawColor(MGridColor);
          for (int32_t i=0; i<MNumColumns-1; i++) {
            painter->drawLine( floorf(x), mrect.y, floorf(x), mrect.y2() );
            x += xcell;
          }
        }
      }

      if (MDrawHorizontalLines) {
        if (MNumRows > 1) {
          painter->setLineWidth(MGridLineWidth * S);
          painter->setDrawColor(MGridColor);
          for (int32_t i=0; i<MNumRows-1; i++) {
            painter->drawLine( mrect.x, floorf(y), mrect.x2(), floorf(y) );
            y += ycell;
          }
        }
      }

      // selected cell(s)
      if (MDrawSelectedCells) {
        if ((MSelectedX >= 0) && (MSelectedY >= 0)) {
          x = xcell * MSelectedX;
          y = ycell * MSelectedY;
          float xn = xcell * MSelectedXcount;
          float yn = ycell * MSelectedYcount;
          float ww = xn - S;
          float hh = yn - S;
          x  += mrect.x;
          y  += mrect.y;
          painter->setDrawColor(MSelectedCellBorderColor);
          painter->setLineWidth(MGridLineWidth * S);
          painter->drawRect( x,y,ww,hh );
        }
      }

    }

    paintChildren(AContext);
    drawIndicators(AContext);
    drawBorder(AContext);

  }

  //----------

  /*
    todo:
    - has selection -> move
  */

  void on_Widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    SAT_Rect mrect = getRect();
    float xcell = ( mrect.w / (float)MNumColumns );
    float ycell = ( mrect.h / (float)MNumRows );
    int32_t x = /*floorf*/( (float)(AXpos - mrect.x) / xcell );
    int32_t y = /*floorf*/( (float)(AYpos - mrect.y) / ycell );
    MClickedX = x;
    MClickedY = y;
    MClickedB = AButton;

    MIsDragging = true;

    if (AButton == SAT_BUTTON_LEFT) {

      //MIsDragging = true;

      if (MSelectCell) {
        MSelectedX = x;
        MSelectedY = y;
        MSelectedXcount = 1;
        MSelectedYcount = 1;
//        if (MSelectMultipleCells) {
//          MIsDragging = true;
//        }
      }
      //on_clickCell(x,y,AButton,AState);
      //do_Widget_redraw(this);
    }
    on_clickCell(x,y,AButton,AState);
    do_Widget_redraw(this);
  }

  //----------

  void on_Widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {

    MIsDragging = false;

//    if (AButton == SAT_BUTTON_LEFT) {
//      MIsDragging = false;
//    }

    //on_releaseCell(x,y,AButton,AState);
    //do_Widget_redraw(this);
  }

  //----------

  void on_Widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) override {
    SAT_Rect mrect = getRect();
    float xcell = ( mrect.w / (float)MNumColumns );
    float ycell = ( mrect.h / (float)MNumRows );
    float x = /*floorf*/( (float)(AXpos - mrect.x) / xcell );
    float y = /*floorf*/( (float)(AYpos - mrect.y) / ycell );
    x = SAT_Clamp(x,0,MNumColumns-1);
    y = SAT_Clamp(y,0,MNumRows-1);

    if (MIsDragging) {

      if (MDragCell) {
        on_dragCell(x,y,MClickedB,AState);
        do_Widget_redraw(this);
      }

      if (MSelectMultipleCells && (MClickedB == SAT_BUTTON_LEFT)) {
      
        if (x < MClickedX) {
          MSelectedX = x;
          MSelectedXcount = MClickedX - x + 1;
        }
        else {
          MSelectedX = MClickedX;
          MSelectedXcount = x - MClickedX + 1;
        }
        if (y < MClickedY) {
          MSelectedY = y;
          MSelectedYcount = MClickedY - y + 1;
        }
        else {
          MSelectedY = MClickedY;
          MSelectedYcount = y - MClickedY + 1;
        }
        on_selectCells(MSelectedX,MSelectedY,MSelectedXcount,MSelectedYcount);
        do_Widget_redraw(this);
      }

    }

  }  

};

//----------------------------------------------------------------------
#endif




#if 0

//------------------------------
public: // parent to child
//------------------------------

//  void on_Widget_move(double AXpos, double AYpos)  override {}
//  void on_Widget_resize(double AWidth, double AHeight) override {}
//  void on_Widget_align(bool ARecursive=true) override {}
//  void on_Widget_paint(SAT_PaintContext* AContext) override { paintChildWidgets(AContext); }
//  void on_Widget_key_press(uint8_t AChar, uint32_t AKeySym, uint32_t AState, uint32_t ATime) override {}
//  void on_Widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}
//  void on_Widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_Widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_Widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_Widget_enter(SAT_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_Widget_leave(SAT_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) override {}
//  //void on_Widget_connect(SAT_Parameter* AParameter) override {}

#endif // 0
