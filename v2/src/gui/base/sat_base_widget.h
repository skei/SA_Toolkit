#ifndef sat_base_widget_included
#define sat_base_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_paint_context.h"
#include "gui/base/sat_widget_owner.h"

//----------------------------------------------------------------------

class SAT_BaseWidget {

//------------------------------
public:
//------------------------------

  SAT_BaseWidget() {
  }

  //----------

  virtual ~SAT_BaseWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void            on_widget_firstShown()                                        {}
  virtual void            on_widget_move()                                              {}
  virtual void            on_widget_resize()                                            {}
  virtual void            on_widget_realign()                                           {}
  virtual void            on_widget_paint(SAT_PaintContext* AContext)                   {}
  virtual void            on_widget_timer()                                             {}
  virtual void            on_widget_mouseClick()                                        {}
  virtual void            on_widget_mouseDblClick()                                     {}
  virtual void            on_widget_mouseRelease()                                      {}
  virtual void            on_widget_mouseMove()                                         {}
  virtual void            on_widget_keyPress()                                          {}
  virtual void            on_widget_keyRelease()                                        {}

//------------------------------
public:
//------------------------------

  virtual const char*     getName()                                                     { return ""; }
  virtual double          getValue(uint32_t AIndex=0)                                   { return 0.0; }
  virtual SAT_Point       getPos()                                                      { return SAT_Point(0,0); }
  virtual SAT_Point       getSize()                                                     { return SAT_Point(0,0); }
  virtual SAT_Rect        getRect()                                                     { return SAT_Rect(0,0,0,0); }
  virtual double          getWidth()                                                    { return 0.0; }
  virtual double          getHeight()                                                   { return 0.0; }

  //----------

  virtual void            setName(const char* AName)                                    {}
  virtual void            setValue(double AValue, uint32_t AIndex=0)                    {}
  virtual void            setPos(SAT_Point APos)                                        {}
  virtual void            setSize(SAT_Point ASize)                                      {}
  virtual void            setRect(SAT_Rect ARect)                                       {}

//------------------------------
public: // hierarchy
//------------------------------

  virtual void            setOwnerWidget(SAT_WidgetOwner* AOwner, bool ARecursive=true) {}
  virtual void            setParentWidget(SAT_BaseWidget* AWidget)                      {}
  virtual void            setParentIndex(uint32_t AIndex)                               {}

  virtual void            appendChildWidget(SAT_BaseWidget* AWidget)                    {}
  virtual void            deleteChildWidgets()                                          {}
  virtual SAT_BaseWidget* findChildWidget(uint32_t AXpos, uint32_t AYpos)               { return nullptr; }
  virtual SAT_BaseWidget* findChildWidget(const char* AName)                            { return nullptr; }
  virtual void            realignChildWidgets()                                         {}
  virtual void            paintChildWidgets(SAT_PaintContext* AContext)                 {}

//------------------------------
public: // painting
//------------------------------
  
  virtual void            markWidgetDirty()                                 {}

};

//----------------------------------------------------------------------
#endif
