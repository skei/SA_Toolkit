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

  uint32_t flushRealignWidgets(uint32_t AFrame) {
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
        //SAT_PRINT("realigned %i frame %i\n",widget->MLastRealignedFrame,AFrame);
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
  // and enqueue the root as the only widget needing repainting

  SAT_Rect flushRedrawToPaint(SAT_RootWidget* ARoot=nullptr) {
    uint32_t num_queued = 0;
    uint32_t num_painted = 0;
    SAT_Widget* widget;
    SAT_Rect rect = {0,0,0,0};
    if (ARoot) {
      while (MRedrawQueue.try_dequeue(widget)) {
        num_queued += 1;
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

  // called from
  // SAT_Window.paint()
  // SAT_Window.paintRoot()

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
          num_queued += 1;
          //ARoot->on_widget_paint(AContext);
          temp_redraw[num_queued] = widget;
        }
      }

      if (num_queued > 0) {
        for (uint32_t i=(num_queued-1); i>0; i--) {
          //SAT_PRINT("%i. num_queued %i i %i\n",i,num_queued,i);
          widget = temp_redraw[i];
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

