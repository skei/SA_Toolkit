#ifndef sat_widget_included
#define sat_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_widget.h"
#include "gui/base/sat_widget_listener.h"
//#include "gui/base/sat_widget_owner.h"

//----------

class SAT_Widget;
typedef SAT_Array<SAT_BaseWidget*>  SAT_WidgetArray;

//----------------------------------------------------------------------

class SAT_Widget
: public SAT_BaseWidget
/*, public SAT_WidgetListener*/ {

//------------------------------
private:
//------------------------------

  SAT_WidgetOwner*    MOwner      = nullptr;
  SAT_Widget*         MParent     = nullptr;
  SAT_WidgetArray     MChildren   = {};
  SAT_WidgetListener* MListener   = {};
  uint32_t            MChildIndex = 0;

  const char*         MName       = "";
  double              MValue      = 0.0;

  SAT_Rect            MRect       = {};
  SAT_Rect            MCreateRect = {};
  SAT_Rect            MLayoutRect = {};

//------------------------------
public:
//------------------------------

  SAT_Widget()
  : SAT_BaseWidget() {
  }

  //----------

  virtual ~SAT_Widget() {
  }

//------------------------------
public:
//------------------------------

  double      getHeight()                 override { return MRect.h; }
  const char* getName()                   override { return MName; }
  SAT_Point   getPos()                    override { return SAT_Point(MRect.x,MRect.y); }
  SAT_Rect    getRect()                   override { return MRect; }
  SAT_Point   getSize()                   override { return SAT_Point(MRect.w,MRect.h); }
  double      getValue(uint32_t AIndex=0) override { return 0.0; }
  double      getWidth()                  override { return MRect.w; }

  //----------

  void setName(const char* AName)                 override { MName = AName; }
  void setPos(SAT_Point APos)                     override { MRect.setPos(APos); }
  void setSize(SAT_Point ASize)                   override { MRect.setSize(ASize); }
  void setValue(double AValue, uint32_t AIndex=0) override { MValue = AValue; }

//------------------------------
public:
//------------------------------

  void setWidgetOwner(SAT_WidgetOwner* AOwner) override {
    MOwner = AOwner;
    // tell all sub-widgets..
  }

  void setParentWidget(SAT_BaseWidget* AWidget) override {
  }

  void setWidgetIndex(uint32_t AIndex) override {
  }

  void appendChildWidget(SAT_BaseWidget* AWidget) override {
    uint32_t index = MChildren.size();
    AWidget->setParentWidget(this);
    AWidget->setWidgetIndex(index);
    MChildren.push_back(AWidget);
  }

  void deleteChildWidgets() override {
  }

  SAT_BaseWidget* findChildWidget(uint32_t AXpos, uint32_t AYpos) override {
    return nullptr;
  }

  SAT_BaseWidget* findChildWidget(const char* AName) override { return nullptr;
    return nullptr;
  }

  void realignChildWidgets() override {
  }

  void paintChildWidgets() override {
  }

  void markWidgetDirty() override {
  }


};

//----------------------------------------------------------------------
#endif
