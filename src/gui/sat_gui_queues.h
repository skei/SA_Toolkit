#ifndef sat_gui_queues_included
#define sat_gui_queues_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_painter.h"
#include "gui/sat_widget.h"
#include "gui/widget/sat_root_widget.h"

#include "extern/cameron314/concurrentqueue.h"
#include "extern/cameron314/readerwriterqueue.h"

//typedef SAT_AtomicQueue<SAT_Widget*,SAT_WINDOW_DIRTY_QUEUE_SIZE> SAT_WidgetQueue;
//typedef SAT_Queue<SAT_Widget*,SAT_WINDOW_DIRTY_QUEUE_SIZE> SAT_WidgetQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_GuiQueues {

  typedef moodycamel::ConcurrentQueue<SAT_Widget*>        SAT_RedrawQueue;
  typedef moodycamel::ConcurrentQueue<SAT_Widget*>        SAT_RealignQueue;
  typedef moodycamel::ReaderWriterQueue<SAT_Widget*>      SAT_PaintQueue;
  //typedef moodycamel::ReaderWriterQueue<uint32_t>         SAT_ResizeQueue;
  //typedef moodycamel::ReaderWriterQueue<SAT_TweenChain*>  SAT_TweenQueue;

//------------------------------
private:
//------------------------------

  SAT_RedrawQueue   MRedrawQueue;
  SAT_RealignQueue  MRealignQueue;

  SAT_PaintQueue    MPaintQueue;

  //SAT_PaintQueue    MReducedRedrawQueue;
  //SAT_PaintQueue    MReducedRealignQueue;

  //SAT_ResizeQueue   MResizeQueue;
  //SAT_TweenQueue    MTweenQueue;

//------------------------------
private:
//------------------------------

  SAT_Widget* temp_realign[1024*16] = {}; // 128k
  SAT_Widget* temp_redraw[1024*16]  = {}; // 128k

  
  //----------




//------------------------------
public:
//------------------------------

  bool queueRedraw(SAT_Widget* AWidget) {
    return MRedrawQueue.enqueue(AWidget);
  }

  //----------

  bool queueRealign(SAT_Widget* AWidget) {
    return MRealignQueue.enqueue(AWidget);
  }

//----------

  SAT_Rect flushRedrawToPaint() {
    uint32_t num_queued = 0;
    uint32_t num_painted = 0;
    SAT_Rect rect = {0,0,0,0};
    uint32_t count = 0;
    SAT_Widget* widget;
    while (MRedrawQueue.try_dequeue(widget)) {
      num_queued += 1;
    //  temp_redraw[num_queued] = widget;
    //}
    //for (uint32_t i=(num_queued-1); i>=0; i--) {
    //  widget = temp_redraw[i];
    //  if (widget->MLastPaintedFrame != AFrame) {
    //    num_painted += 1;
        MPaintQueue.enqueue(widget);
        rect.combine(widget->getRect());
    //    widget->MLastPaintedFrame = AFrame;
    //  }
    }
    return rect;
  }

//----------

  uint32_t realignWidgets(uint32_t AFrame) {
    uint32_t num_queued = 0;
    uint32_t num_realigned = 0;
    SAT_Widget* widget;
    while (MRealignQueue.try_dequeue(widget)) {
      num_queued += 1;
      //SAT_Assert(num_queued < (1024*16) )
      temp_realign[num_queued] = widget;
    }
    if (num_queued > 0) {
      for (uint32_t i=(num_queued-1); i>0; i--) {
        //SAT_PRINT("num_queued %i %i\n",num_queued,i);
        widget = temp_realign[i];

        SAT_PRINT("realigned %i frame %i\n",widget->MLastRealignedFrame,AFrame);

        if (widget->MLastRealignedFrame != AFrame) {
          num_realigned += 1;
          widget->on_widget_realign();
          widget->MLastRealignedFrame = AFrame;
        }
      }
    }
    //SAT_PRINT("num_queued %i num_painted %i\n",num_queued,num_painted);
    return num_realigned;
  }

  //----------

  uint32_t paintWidgets(SAT_PaintContext* AContext, SAT_RootWidget* ARoot=nullptr) {
    uint32_t num_queued = 0;
    uint32_t num_painted = 0;
    if (ARoot) {

      /*
        if we are drawing from the root, we ignore the paint queue
        (just flushes it), and just tells ARoot to paint itself
      */

      SAT_Widget* widget;
      while (MPaintQueue.try_dequeue(widget)) {
        num_queued += 1;
      }
      ARoot->on_widget_paint(AContext);
      return num_queued; // ???

    }

    else {

      SAT_Widget* widget;

      while (MPaintQueue.try_dequeue(widget)) {
        //SAT_Assert(widget);
        if (widget) {
          num_queued += 1;
          //ARoot->on_widget_paint(AContext);
          temp_redraw[num_queued] = widget;
        }
      }

      if (num_queued > 0) {
        for (uint32_t i=(num_queued-1); i>0; i--) {
          //SAT_PRINT("%i. num_queued %i i %i\n",i,num_queued,i);
          widget = temp_redraw[i];
          //SAT_Assert(widget);
          //SAT_PRINT("last painted %i frame %i\n",widget->MLastPaintedFrame,AContext->counter);
          if (widget) {
            if (widget->MLastPaintedFrame != AContext->counter) {
              num_painted += 1;
              widget->on_widget_paint(AContext);
              widget->MLastPaintedFrame = AContext->counter;
            }
          }
        }
      }

    }
    return num_painted;
  }

  
};

//----------------------------------------------------------------------
#endif










































#if 0

/*
  TODO
  re-evaluate the entire threading and lock-free concept..
  MPMC queues
*/


//#include "extern/cameron314/concurrentqueue.h"
//#include "extern/cameron314/readerwritercircularbuffer.h"

//----------

typedef SAT_AtomicQueue<SAT_Widget*,SAT_WINDOW_DIRTY_QUEUE_SIZE> SAT_WidgetQueue;
//typedef SAT_Queue<SAT_Widget*,SAT_WINDOW_DIRTY_QUEUE_SIZE> SAT_WidgetQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_GuiQueues {

//------------------------------
private:
//------------------------------

  SAT_WidgetQueue     MDirtyGuiWidgets      = {};
  SAT_WidgetQueue     MDirtyTimerWidgets    = {};
  SAT_WidgetQueue     MDirtyAudioWidgets    = {};
  SAT_WidgetQueue     MPaintWidgets         = {};
  
//------------------------------
public:
//------------------------------
  
  // void queueDirtyGuiWidget(SAT_Widget* AWidget)
  // SAT_Rect flushDirtyGuiWidgets()

  // void queueDirtyTimerWidget(SAT_Widget* AWidget)
  // SAT_Rect flushDirtyTimerWidgets()

  // void queueDirtyAudioWidget(SAT_Widget* AWidget)
  // SAT_Rect flushDirtyAudioWidgets()
      
  // void queuePaintWidget(SAT_Widget* AWidget)
  // void flushPaintWidgets(SAT_PaintContext* AContext)
  // void flushPaintRootWidget(SAT_PaintContext* AContext)

  //----------

  /*
    push widget onto dirty widgets queue

    called from  [GUI THREAD]
      on_window_show()            - opening editor  - gui thread (before window is open)
      on_window_resize()          - resizing editor - gui thread (while window is open)
      on_widgetListener_redraw()  - tweaking knob   - timer thread (do_widget_redraw)... ouch!!!
  */

  // [GUI THREAD]

  void queueDirtyGuiWidget(SAT_Widget* AWidget) {
    if (!MDirtyGuiWidgets.write(AWidget)) {
      SAT_PRINT("Error! Couldn't write to dirty gui widget queue\n");
    }
  }

  //----------

  /*
    pops all widgets from dirty que and push them onto paint queue
    invalidates combined rect
    TODO: check widget if already set to be drawn (render frame #)

    called from
      on_TimerListener_update
  */

  // [TIMER THREAD]

  SAT_Rect flushDirtyGuiWidgets() {
    uint32_t count = 0;
    SAT_Widget* widget;
    bool have_rect = false;
    SAT_Rect update_rect = {0,0,0,0};
    while (MDirtyGuiWidgets.read(&widget)) {
      SAT_Assert(widget);
      queuePaintWidget(widget);
      SAT_Rect rect = widget->getRect();
      //update_rect.combine(rect);
      if (have_rect) update_rect.combine(rect);
      else {
        update_rect = rect;
        have_rect = true;
      }
      count += 1;
    }
    //if (count > 0) { SAT_PRINT("flushed %i widgets\n",count); }
    return update_rect;
  }

  //----------

  /*
    called from  [GUI THREAD]
      on_widgetListener_redraw (if SAT_WIDGET_REDRAW_GUI)
  */

  // [TIMER THREAD]

  void queueDirtyTimerWidget(SAT_Widget* AWidget) {
    if (!MDirtyTimerWidgets.write(AWidget)) {
      SAT_PRINT("Error! Couldn't write to dirty timer widget queue\n");
    }
  }

  //----------

  /*
    called from
      on_TimerListener_update
  */

  // [TIMER THREAD]

  SAT_Rect flushDirtyTimerWidgets() {
    uint32_t count = 0;
    SAT_Widget* widget;
    bool have_rect = false;
    SAT_Rect update_rect = {0,0,0,0};
    while (MDirtyTimerWidgets.read(&widget)) {
      SAT_Assert(widget);
      queuePaintWidget(widget);
      SAT_Rect rect = widget->getRect();
      //update_rect.combine(rect);
      if (have_rect) update_rect.combine(rect);
      else {
        update_rect = rect;
        have_rect = true;
      }
      count += 1;
    }
    //if (count > 0) { SAT_PRINT("flushed %i widgets\n",count); }
    return update_rect;
  }
  //----------

  /*
    called from  [AUDIO THREAD]
      on_widgetListener_redraw (if SAT_WIDGET_REDRAW_AUDIO)
  */

  // [AUDIO THREAD]

  void queueDirtyAudioWidget(SAT_Widget* AWidget) {
    if (!MDirtyAudioWidgets.write(AWidget)) {
      SAT_PRINT("Error! Couldn't write to dirty audio widget queue\n");
    }
  }

  //----------

  /*
    called from
      on_TimerListener_update
  */

  // [TIMER THREAD]

  SAT_Rect flushDirtyAudioWidgets() {
    uint32_t count = 0;
    SAT_Widget* widget;
    bool have_rect = false;
    SAT_Rect update_rect = {0,0,0,0};
    while (MDirtyAudioWidgets.read(&widget)) {
      SAT_Assert(widget);
      queuePaintWidget(widget);
      SAT_Rect rect = widget->getRect();
      //update_rect.combine(rect);
      if (have_rect) update_rect.combine(rect);
      else {
        update_rect = rect;
        have_rect = true;
      }
      count += 1;
    }
    //if (count > 0) { SAT_PRINT("flushed %i widgets\n",count); }
    return update_rect;
  }

  //----------

  /*
    called from
      flushDirtyGuiWidgets()
      flushDirtyTimerWidgets()
      flushDirtyAudioWidgets()

  */

  // [TIMER THREAD] 

  void queuePaintWidget(SAT_Widget* AWidget) {
    if (!MPaintWidgets.write(AWidget)) {
      SAT_PRINT("Error! Couldn't write to paint widget queue\n");
    }
  }

  //----------

  /*
    pops all widgets from paint queue
    search upwards (parent) until we find an opaque widget
      set clipping, paint

    called from
      paint()
      
  */

  // [GUI THREAD]

  void flushPaintWidgets(SAT_PaintContext* AContext) {
    uint32_t count = 0;
    SAT_Painter* painter = AContext->painter;
    SAT_Widget* widget = nullptr;
    while (MPaintWidgets.read(&widget)) {
      count += 1;
      //painter->pushOverlappingClip(widget->getRect());
      SAT_Widget* parent = widget->findOpaqueParent();
      if (parent) {
        painter->pushOverlappingClip( parent->getRect() );
        parent->on_widget_paint(AContext);
        painter->popClip();
      }
      else {
        widget->on_widget_paint(AContext);
      }
      //painter->popClip();
    }
    //if (count > 0) { SAT_PRINT("flushed %i widgets\n",count); }
  }

  //----------

  /*
    empties paint queue
    draw root widget (and children)

    called from
      paint() (ifdef SAT_WINDOW_BUFFERED) [GUI THREAD]
      paintRoot()
  */

  // [GUI THREAD]

  void flushPaintRootWidget(SAT_PaintContext* AContext, SAT_RootWidget* ARootWidget=nullptr) {
    uint32_t count = 0;
    SAT_Widget* widget = nullptr;
    while (MPaintWidgets.read(&widget)) {
      count += 1;
    }
    if (ARootWidget) ARootWidget->on_widget_paint(AContext);
    //if (count > 0) { SAT_PRINT("dumped %i widgets\n",count); }
  }

};

#endif // 0
