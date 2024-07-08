#ifndef sa_demo_widgets_included
#define sa_demo_widgets_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
// root
//
//----------------------------------------------------------------------

class sa_demo_root_widget
: public SAT_RootWidget {

//------------------------------
public:
//------------------------------

  sa_demo_root_widget(SAT_WidgetListener* AListener, SAT_Rect ARect)
  : SAT_RootWidget(AListener,ARect) {
    setName("sa_demo_root_widget");
    setHint("sa_demo_root_widget");
  }

  //----------

  virtual ~sa_demo_root_widget() {
  }

//------------------------------
public:
//------------------------------

  // void do_widget_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) override {
  //   SAT_TRACE;
  //   SAT_RootWidget::do_widget_update(AWidget,AIndex,AMode);
  // }

};

//----------------------------------------------------------------------
//
// menu (button_grid)
//
//----------------------------------------------------------------------

class sa_demo_button_grid_widget
: public SAT_ButtonGridWidget {

//------------------------------
private:
//------------------------------

  SAT_PagesWidget* MPages = nullptr;

//------------------------------
public:
//------------------------------

  sa_demo_button_grid_widget(SAT_Rect ARect, uint32_t AColumns, uint32_t ARows, const char** ATexts=nullptr, SAT_PagesWidget* APages=nullptr)
  : SAT_ButtonGridWidget(ARect,AColumns,ARows,ATexts) {
    MPages = APages;
  }

  //----------

  virtual ~sa_demo_button_grid_widget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setPages(SAT_PagesWidget* APages) {
    MPages = APages;
  }

  void on_clickCell(int32_t AX, int32_t AY, int32_t AB, int32_t AS) override {
    SAT_ButtonGridWidget::on_clickCell(AX,AY,AB,AS);
    if (MPages) {
      MPages->setPage(AY);
      MPages->realignChildren();
      MPages->do_widget_redraw(MPages);

    }
  }


};

//----------------------------------------------------------------------
#endif

