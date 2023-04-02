#ifndef sat_tabs_widget_included
#define sat_tabs_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widget.h"
#include "gui/widgets/sat_button_row_widget.h"
#include "gui/widgets/sat_pages_widget.h"
#include "gui/sat_paint_context.h"

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

    SAT_ButtonRowWidget*  MHeader;
    SAT_PagesWidget*      MPages;
    int32_t               MNumPages;

//------------------------------
public:
//------------------------------

  SAT_TabsWidget(SAT_Rect ARect, uint32_t ANum=MAX_PAGES)
  : SAT_Widget(ARect) {
    setName("SAT_TabsWidget");
    //Layout.spacing = 5;
    
    MHeader = new SAT_ButtonRowWidget(SAT_Rect(20),ANum);
    //Layout.alignment = SAT_WIDGET_ALIGN_FILL_TOP;
    MHeader->setAlignment(SAT_WIDGET_ALIGN_TOP);
    MHeader->setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);
    
    //MHeader->setDrawRoundedBottom(false);
    MHeader->setRoundedCorners(true);
    MHeader->setCornerSizes(6,6,0,0);
    
    MPages = new SAT_PagesWidget(SAT_Rect());
    //MPages->Layout.alignment = SAT_WIDGET_ALIGN_FILL_CLIENT;
    MPages->setStretching(SAT_WIDGET_STRETCH_ALL);
    MNumPages = 0;
    //MHeader->setWidgetWidth(0);
    appendChildWidget(MHeader);
    appendChildWidget(MPages);
  }

  //----------

  virtual ~SAT_TabsWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual SAT_Widget* getHeader(void)    { return MHeader; }
  virtual SAT_Widget* getPages(void)     { return MPages; }
  virtual int32_t     getNumPages(void)  { return MNumPages; }

  //----------

  virtual int32_t appendPage(const char* ATitle, SAT_Widget* AWidget) {
    //AWidget->Layout.alignment = SAT_WIDGET_ALIGN_FILL_CLIENT;
    //AWidget->setStretching(SAT_WIDGET_STRETCH_ALL);
    AWidget->setLayout(SAT_WIDGET_ALIGN_NONE,SAT_WIDGET_STRETCH_ALL);
    int32_t result = MPages->appendPage(AWidget);
    MHeader->setButtonName(MNumPages,ATitle);
    MNumPages += 1;
    //MHeader->setWidth(MNumPages);
    MHeader->setNumButtons(MNumPages);
    return result;
  }

  //----------

  virtual void selectPage(int32_t AIndex/*, bool ARedraw=false*/) {
    //SAT_Print("index %i num_pages %i\n",AIndex,MNumPages);
    if (AIndex < MNumPages) {
      MHeader->selectButton(AIndex/*,ARedraw*/);
      MPages->setPage(AIndex/*,ARedraw*/);
    }
  }

//------------------------------
public: // child to parent
//------------------------------

  void do_widget_update(SAT_Widget* ASender, uint32_t AMode, uint32_t AIndex=0) override {
    if (ASender == MHeader) {
      
      int32_t sel = MHeader->getSelected();
      MPages->setPage(sel/*,true*/);
      //MPages->on_realign();

      MPages->realignChildWidgets(true);
      do_widget_redraw(MPages,0);
      
    }
    SAT_Widget::do_widget_update(ASender,AMode,AIndex);
    SAT_Widget::do_widget_redraw(MPages,0);
  }

};

//----------------------------------------------------------------------
#endif


