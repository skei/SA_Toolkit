#ifndef sat_gui_queues_included
#define sat_gui_queues_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_painter.h"
#include "gui/sat_widget.h"
#include "gui/widget/sat_root_widget.h"

//#include "extern/cameron314/concurrentqueue.h"
//#include "extern/cameron314/readerwriterqueue.h"

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

  // called from
  // SAT_Window.on_WidgetListener_realign()

  bool queueRealign(SAT_Widget* AWidget) {
    return MRealignQueue.enqueue(AWidget);
  }

  //----------

  // called from
  // SAT_Window.paint()
  // SAT_Window.paintRoot()

  // call on_widget_realign for every widget in the realign queue
  // (unless it has already been realigned this frame)

  uint32_t flushRealignWidgets(uint32_t AFrame) {
    uint32_t num_queued = 0;
    uint32_t num_realigned = 0;
    SAT_Widget* widget;
    while (MRealignQueue.try_dequeue(widget)) {
      temp_realign[num_queued] = widget;
      num_queued += 1;
      // todo: if is_root, has_root = true
    }
    if (num_queued > 0) {
      // todo: if has_root, realign only root
      for (uint32_t i=0; i<num_queued; i++) {
        widget = temp_realign[i];
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

  uint32_t emptyRealignQueue() {
    uint32_t num_realigned = 0;
    SAT_Widget* widget;
    while (MRealignQueue.try_dequeue(widget)) {
      num_realigned += 1;
    }
    return num_realigned;
  }




//------------------------------
public:
//------------------------------

  // called from
  // SAT_Window.on_window_show()
  // SAT_Window.on_WidgetListener_redraw()

  bool queueRedraw(SAT_Widget* AWidget) {
    return MRedrawQueue.enqueue(AWidget);
  }

  //----------

  // called from
  // SAT_Window.on_window_timer

  // if ARoot is not null, just flush the redraw queue,
  // and queue the root as the only widget needing repainting
  // else move all widgets over to paint queue

  SAT_Rect flushRedrawToPaint(SAT_RootWidget* ARoot=nullptr) {
    uint32_t num_queued = 0;
    uint32_t num_painted = 0;
    SAT_Widget* widget;
    SAT_Rect rect = {0,0,0,0};
    if (ARoot) {
      while (MRedrawQueue.try_dequeue(widget)) {
        num_queued += 1;
        // we don't care about queued redraws, we will draw the root anyway
      }
      MPaintQueue.enqueue(ARoot);
      rect = ARoot->getRect();
    }
    else {
      while (MRedrawQueue.try_dequeue(widget)) {
        num_queued += 1;
        MPaintQueue.enqueue(widget);
        rect.combine(widget->getRect());
      }
    }

    return rect;

  }

  //----------

  uint32_t emptyRedrawQueue() {
    uint32_t num_redrawn = 0;
    SAT_Widget* widget;
    while (MRedrawQueue.try_dequeue(widget)) {
      num_redrawn += 1;
    }
    return num_redrawn;
  }


  //----------

  // called from
  // SAT_Window.paint()
  // SAT_Window.paintRoot()

  // if root is not null, just draw root, flush/dump paint queue
  // else draw paint queue, one by one

  uint32_t flushPaintWidgets(SAT_PaintContext* AContext, SAT_RootWidget* ARoot=nullptr) {
    uint32_t num_queued = 0;
    uint32_t num_painted = 0;

    if (ARoot) {
      // if we are drawing from the root, we ignore the paint queue
      // (just flushes it), and just tells ARoot to paint itself
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
        if (widget) {
          temp_redraw[num_queued] = widget;
          num_queued += 1;
          //ARoot->on_widget_paint(AContext);
        }
      }

      if (num_queued > 0) {

        //for (uint32_t i=(num_queued-1); i>0; i--) {
        for (uint32_t i=0; i<num_queued; i++) {

          //SAT_PRINT("%i. num_queued %i i %i\n",i,num_queued,i);
          widget = temp_redraw[i];
          //SAT_PRINT("last painted %i frame %i\n",widget->MLastPaintedFrame,AContext->counter);
          if (widget) {
            if (widget->MLastPaintedFrame != AContext->counter) {
              widget->MLastPaintedFrame = AContext->counter;
              num_painted += 1;
              widget->on_widget_paint(AContext);
            }
          }
        }
      }

    }

    return num_painted;
  }

//----------

  uint32_t emptyPaintQueue() {
    uint32_t num_painted = 0;
    SAT_Widget* widget;
    while (MPaintQueue.try_dequeue(widget)) {
      num_painted += 1;
    }
    return num_painted;
  }


};

//----------------------------------------------------------------------
#endif

