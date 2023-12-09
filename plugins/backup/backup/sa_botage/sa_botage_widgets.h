//#ifndef sa_botage_widgets_included
//#define sa_botage_widgets_included
////----------------------------------------------------------------------
//
//#include "base/mip.h"
//#include "gui/widgets/mip_image_widget.h"
//#include "gui/widgets/mip_panel_widget.h"
//
////----------
//
//#include "../img/sa_img.h"
//#include "../img/botage_img.h"
//#include "../img/mip_img.h"
//
//#define SA_BOTAGE_HEADER_HEIGHT 80
//#define SA_BOTAGE_FOOTER_HEIGHT 20
//
////----------------------------------------------------------------------
//
//class sa_botage_header
//: public MIP_PanelWidget {
//
//private:
//
//  int m_sa      = 0;
//  int m_botage  = 0;
//  int m_mip     = 0;
//
//  MIP_ImageWidget*  MSaImage      = nullptr;
//  MIP_ImageWidget*  MBotageImage  = nullptr;
//  MIP_ImageWidget*  MMipImage     = nullptr;
//  MIP_TextWidget*   MVersionText  = nullptr;
//  MIP_TextWidget*   MClapText     = nullptr;
//
//  char clap_string[32] = {0};
//
//public:
//
//  sa_botage_header(MIP_DRect ARect, const char* AVersion)
//  : MIP_PanelWidget(ARect) {
//
//    setFillBackground(true);
//    setBackgroundColor(0.25);
//
//    // sa
//
//    MSaImage = new MIP_ImageWidget(MIP_DRect(0,0,80,80),(void*)sa_img,sa_img_size);
//    appendChildWidget(MSaImage);
//    MSaImage->setHint("skei.audio");
//
//    // botage
//
//    MBotageImage = new MIP_ImageWidget(MIP_DRect(80,0,160,80),(void*)botage_img,botage_img_size);
//    appendChildWidget(MBotageImage);
//    MBotageImage->setHint("sa_botage");
//
//    // sa_botage version
//
//    MVersionText = new MIP_TextWidget(MIP_DRect(84,50,60,20),AVersion);
//    appendChildWidget(MVersionText);
//    MVersionText->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
//    MVersionText->setTextSize(8);
//    MVersionText->setTextColor(0.4);
//    MVersionText->setFillBackground(false);
//    MVersionText->setDrawBorder(false);
//
//    // mip
//
//    MMipImage = new MIP_ImageWidget(MIP_DRect((ARect.w - 120),0,120,80),(void*)mip_img,mip_img_size);
//    appendChildWidget(MMipImage);
//    MMipImage->setAlpha(0.6);
//    MMipImage->setHint("Multum In Parvo");
//
//    // mip version
//
//    MVersionText = new MIP_TextWidget(MIP_DRect((ARect.w - 110),55,60,10),MIP_VERSION);
//    appendChildWidget(MVersionText);
//    MVersionText->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
//    MVersionText->setTextSize(8);
//    MVersionText->setTextColor(0.4);
//    MVersionText->setFillBackground(false);
//    MVersionText->setDrawBorder(false);
//
//    // clap
//
////    sprintf(clap_string,"CLAP %i.%i.%i",CLAP_VERSION_MAJOR,CLAP_VERSION_MINOR,CLAP_VERSION_REVISION);
////    MClapText = new MIP_TextWidget(MIP_DRect((ARect.w - 110),65,60,10),clap_string);
////    appendChildWidget(MClapText);
////    MClapText->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
////    MClapText->setTextSize(9);
////    MClapText->setTextColor(0.4);
////    MClapText->setFillBackground(false);
////    MClapText->setDrawBorder(false);
//
//  }
//
//  virtual ~sa_botage_header() {
//  }
//
////  virtual void open(MIP_BaseWindow* AOwnerWindow, bool ARecursive=true) {
////    MIP_Widget::open(AOwnerWindow,ARecursive);
////    //MIP_Window* window = AEditor->getWindow();
////    MIP_Window* window = (MIP_Window*)AOwnerWindow;
////    MIP_Painter* painter = window->getWindowPainter();
////    int m_sa      = painter->loadImage((void*)sa_img,    sa_img_size);
////    int m_botage  = painter->loadImage((void*)botage_img,botage_img_size);
////    int m_mip     = painter->loadImage((void*)mip_img,   mip_img_size);
////  }
//
//};
//
////----------------------------------------------------------------------
////
////
////
////----------------------------------------------------------------------
//
////class sa_botage_footer
////: public MIP_PanelWidget {
////
////public:
////
////  sa_botage_footer(MIP_DRect ARect)
////  : MIP_PanelWidget(ARect) {
////
////    setFillBackground(true);
////    setBackgroundColor(0.25);
////  }
////
////  virtual ~sa_botage_footer() {
////  }
////
////};
//
////----------------------------------------------------------------------
//#endif
//
