#ifndef sat_tabs_widget_included
#define sat_tabs_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_button_grid_widget.h"
#include "gui/widget/sat_pages_widget.h"

#define MAX_PAGES 32

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_TabsWidget
: public SAT_Widget {

//------------------------------
protected:
//------------------------------

    SAT_ButtonGridWidget* MHeader;
    SAT_PagesWidget*      MPages;

    //int32_t               MNumPages;
    //const char*           MHeaderTexts[32];

//------------------------------
public:
//------------------------------

  SAT_TabsWidget(SAT_Rect ARect, uint32_t AHeaderHeight/*, uint32_t w=0, uint32_t h=0*/)
  : SAT_Widget(ARect) {
    setName("SAT_TabsWidget");
    setHint("SAT_TabsWidget");

    MHeader = new SAT_ButtonGridWidget(SAT_Rect(AHeaderHeight),0,0); // ,w,h);
    MHeader->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP;
    MHeader->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_LEFT;
    MHeader->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
    MHeader->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_TOP;
    MHeader->setSingle(true,true);
    //MHeader->setButtonText("...",0,0);
    
    MPages = new SAT_PagesWidget(SAT_Rect());
    MPages->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_TOP;
    MPages->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_LEFT;
    MPages->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;

    //MNumPages = 0;
    appendChild(MHeader);
    appendChild(MPages);
  }

  //----------

  virtual ~SAT_TabsWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual SAT_ButtonGridWidget* getHeader(void)    { return MHeader; }
  virtual SAT_PagesWidget*      getPages(void)     { return MPages; }
  virtual int32_t               getNumPages(void)  { return MHeader->getNumColumns(); }

  virtual uint32_t getActivePage() {
    return MPages->getActivePage();
  }

  //----------

  virtual int32_t appendPage(const char* ATitle, SAT_Widget* AWidget) {

    AWidget->Layout.flags  = SAT_WIDGET_LAYOUT_ANCHOR_TOP;
    AWidget->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_LEFT;
    AWidget->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;
    int32_t result = MPages->appendPage(AWidget);

    uint32_t num = MHeader->getNumColumns();
    MHeader->resize(num + 1,1);
    MHeader->setButtonText(ATitle,num,0);

    return result;
  }

  //----------

  virtual void selectPage(int32_t AIndex) {
    //SAT_PRINT("AIndex %i getNumColumns %i\n",AIndex,MHeader->getNumColumns());
    if (AIndex < MHeader->getNumColumns()) {
      MHeader->selectCell(AIndex,0);
      MPages->selectPage(AIndex);
      // MPages->realignChildren();
      // do_Widget_redraw(MPages);
    }
  }

//------------------------------
public: // child to parent
//------------------------------

  void do_Widget_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) override {
    if (AWidget == MHeader) {
      int32_t sel = MHeader->getClickedColumn();
      //SAT_PRINT("sel %i\n",sel);
      MPages->selectPage(sel);
      MPages->realignChildren();
      do_Widget_redraw(MPages);
    }
    SAT_Widget::do_Widget_update(AWidget,AIndex,AMode);
  }

};

//----------------------------------------------------------------------
#endif

