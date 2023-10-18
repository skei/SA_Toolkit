#ifndef sat_widget_included
#define sat_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_widget.h"
#include "gui/base/sat_widget_listener.h"
#include "gui/base/sat_paint_context.h"

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

  SAT_WidgetOwner*    MOwner        = nullptr;
  SAT_WidgetListener* MListener     = {};

  SAT_BaseWidget*     MParent       = nullptr;
  uint32_t            MParentIndex  = 0;
  SAT_WidgetArray     MChildren     = {};

  const char*         MName         = "";
  double              MValue        = 0.0;

  SAT_Rect            MRect         = {};

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

  const char* getName() override {
    return MName;
  }

  double getValue(uint32_t AIndex=0) override {
    return 0.0;
  }

  SAT_Point getPos() override {
    return SAT_Point(MRect.x,MRect.y);
  }

  SAT_Point getSize() override {
    return SAT_Point(MRect.w,MRect.h);
  }

  SAT_Rect getRect() override {
    return MRect;
  }

  double getWidth() override {
    return MRect.w;
  }

  double getHeight() override {
    return MRect.h;
  }

//------------------------------
public:
//------------------------------

  void setName(const char* AName) override {
    MName = AName;
  }

  void setValue(double AValue, uint32_t AIndex=0) override {
    MValue = AValue;
  }

  void setPos(SAT_Point APos) override
  {
    MRect.setPos(APos);
  }
  
  void setSize(SAT_Point ASize) override
  {
    MRect.setSize(ASize);
  }
  
  void setRect(SAT_Rect ARect) override {
    MRect = ARect;
  }

//------------------------------
public:
//------------------------------

  void setOwnerWidget(SAT_WidgetOwner* AOwner, bool ARecursive=true) override {
    MOwner = AOwner;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->setOwnerWidget(AOwner,ARecursive);
      }
    }
  }

  //----------

  void setParentWidget(SAT_BaseWidget* AWidget) override {
    MParent = AWidget;
  }

  //----------

  void setParentIndex(uint32_t AIndex) override {
    MParentIndex = AIndex;
  }

  //----------

  void appendChildWidget(SAT_BaseWidget* AWidget) override {
    uint32_t index = MChildren.size();
    AWidget->setParentWidget(this);
    AWidget->setParentIndex(index);
    MChildren.push_back(AWidget);
  }

  //----------

  void deleteChildWidgets() override {
  }

  //----------

  SAT_BaseWidget* findChildWidget(uint32_t AXpos, uint32_t AYpos) override {
    return nullptr;
  }

  //----------

  SAT_BaseWidget* findChildWidget(const char* AName) override { return nullptr;
    return nullptr;
  }

  //----------

  void realignChildWidgets() override {
  }

  //----------

  void paintChildWidgets(SAT_PaintContext* AContext) override {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      MChildren[i]->on_widget_paint(AContext);
    }
  }

  //----------

  void markWidgetDirty() override {
  }

};

//----------------------------------------------------------------------
#endif
