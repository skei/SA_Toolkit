#ifndef sat_paint_context_included
#define sat_paint_context_included
//----------------------------------------------------------------------

struct SAT_PaintContext {
  SAT_Painter*  painter       = nullptr;
  SAT_Rect      update_rect   = {};
  uint32_t      window_width  = 0;
  uint32_t      window_height = 0;
};

//----------------------------------------------------------------------
#endif
