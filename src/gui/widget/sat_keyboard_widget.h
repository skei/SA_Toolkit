#ifndef sat_keyboard_widget_included
#define sat_keyboard_widget_included
//----------------------------------------------------------------------

#include "gui/widget/sat_visual_widget.h"

/*

  |  |3| | |  |  | | | | | |  |
  |  |_| |_|  |  |_| |_| |_|  |
  |0  |1  |2  |   |   |   |   |
  |___|___|___|___|___|___|___|

  note_data:

  ofs: 'steps' (note-width/3) to skip before draw/calc, to get to the upper
       left of the next note (from the previous position)
  type: 0..3 = c,d,e,black

*/

//static
const int32_t SAT_KEYBOARD_INTERNAL_NOTE_DATA[12*2] = {
  0, 0, // c
  2, 3, // c#
  1, 1, // d
  2, 3, // d#
  1, 2, // e
  3, 0, // f
  2, 3, // f#
  1, 1, // g
  2, 3, // g#
  1, 1, // a
  2, 3, // a#
  1, 2  // b
};

//----------------------------------------------------------------------

class SAT_KeyboardWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  //bool        MActiveMap[12]    = {1,1,1,1,1,1,1,1,1,1,1,1};
  bool        MActiveMap[12]    = {0,0,0,0,0,0,0,0,0,0,0,0};
  int32_t     MNoteRects1[12*4] = {0};
  int32_t     MNoteRects2[12*4] = {0};

//------------------------------
protected:
//------------------------------

  double    MDist             = 0.5;
  float     MRatio            = 0.6;
  SAT_Color MBlackColor        = SAT_Color(0.3,0.3,0.3);
  SAT_Color MWhiteColor        = SAT_Color(0.6,0.6,0.6);
  SAT_Color MBlackActiveColor  = SAT_Color(0.1,0.1,0.1);
  SAT_Color MWhiteActiveColor  = SAT_Color(0.8,0.8,0.8);
  bool      MVertical         = false;

//------------------------------
public:
//------------------------------

  SAT_KeyboardWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_KeyboardWidget");
    //setWidgetHint("keyboard");
    setCursor(SAT_CURSOR_FINGER);
    //recalc();
  }

  virtual ~SAT_KeyboardWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setColors(SAT_Color AWhite, SAT_Color ABlack, SAT_Color AWhiteActive, SAT_Color ABlackActive) {
    MWhiteColor       = AWhite;
    MWhiteActiveColor = AWhiteActive;
    MBlackColor       = ABlack;
    MBlackActiveColor = ABlackActive;
  }

  //----------

  virtual void setDist(double ADist) {
    MDist = ADist + 1.0;
  }

  //----------

  virtual void setRatio(float ARatio) {
    MRatio = ARatio;
  }

  virtual void setVertical(bool AVertical=true) {
    MVertical = AVertical;
  }

  //----------

  virtual void activate(int32_t num) {
    MActiveMap[num] = true;
  }

  //----------

  virtual void deactivate(int32_t num) {
    MActiveMap[num] = false;
  }

//------------------------------
public:
//------------------------------

  virtual void recalc_note_rects(void) {
    double S = getWindowScale();
    double mdist_s = (MDist * S);
    SAT_Rect mrect = getRect();
    if (MVertical) {
      int32_t size = mrect.h;
      float   s1 = (float)size / 21.0; // 7 notes, 3 'splits' per note?
      float   s2 = s1 * 2;
      float   s3 = s1 * 3;
      int32_t y1    = 0;
      int32_t x0    = 0;
      int32_t x1    = floorf((float)mrect.w * (1.0 - MRatio));
      int32_t x2    = mrect.w - 1;
      for (int32_t note=0; note<12; note++) {
        int32_t n4    = note * 4;
        int32_t iofs  = SAT_KEYBOARD_INTERNAL_NOTE_DATA[note*2];
        float   ofs   = (float)iofs * s1;
        y1 += ofs;
        int32_t typ   = SAT_KEYBOARD_INTERNAL_NOTE_DATA[(note*2)+1];
        switch(typ) {
          case 0: // C
            MNoteRects1[n4+0] = x0;         MNoteRects1[n4+1] = floorf(y1);
            MNoteRects1[n4+2] = x1;         MNoteRects1[n4+3] = floorf(y1+s3) - mdist_s;//MDist;
            MNoteRects2[n4+0] = x1;         MNoteRects2[n4+1] = floorf(y1);
            MNoteRects2[n4+2] = x2;         MNoteRects2[n4+3] = floorf(y1+s2) - mdist_s;//MDist;
            break;
          case 1: // D
            MNoteRects1[n4+0] = x0;         MNoteRects1[n4+1] = floorf(y1);
            MNoteRects1[n4+2] = x1;         MNoteRects1[n4+3] = floorf(y1+s3) - mdist_s;//MDist;
            MNoteRects2[n4+0] = x1;         MNoteRects2[n4+1] = floorf(y1+s1);
            MNoteRects2[n4+2] = x2;         MNoteRects2[n4+3] = floorf(y1+s2) - mdist_s;//MDist;
            break;
          case 2: // E
            MNoteRects1[n4+0] = x0;         MNoteRects1[n4+1] = floorf(y1);
            MNoteRects1[n4+2] = x1;         MNoteRects1[n4+3] = floorf(y1+s3) - mdist_s;//MDist;
            MNoteRects2[n4+0] = x1;         MNoteRects2[n4+1] = floorf(y1+s1);
            MNoteRects2[n4+2] = x2;         MNoteRects2[n4+3] = floorf(y1+s3) - mdist_s;//MDist;
            break;
          case 3: // black
            MNoteRects1[n4+0] = x1 + /*MDist*/mdist_s; MNoteRects1[n4+1] = floorf(y1);
            MNoteRects1[n4+2] = x2;         MNoteRects1[n4+3] = floorf(y1+s2) - mdist_s - S;//MDist - 1;
            break;
        } // switch
      } // for note
    }
    else {
      int32_t size = mrect.w;
      float   s1 = (float)size / 21.0; // 7 notes, 3 'splits' per note?
      float   s2 = s1 * 2;
      float   s3 = s1 * 3;
      float   x1    = 0;
      int32_t y1    = floorf((float)mrect.h * MRatio);
      int32_t y2    = mrect.h - 1;
      for (int32_t note=0; note<12; note++) {
        int32_t n4    = note * 4;
        int32_t iofs  = SAT_KEYBOARD_INTERNAL_NOTE_DATA[note*2];
        float   ofs   = (float)iofs * s1;
        x1 += ofs;
        int32_t typ   = SAT_KEYBOARD_INTERNAL_NOTE_DATA[(note*2)+1];
        switch(typ) {
          case 0: // C
            MNoteRects1[n4+0] = floorf(x1);                         MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1+s2) - mdist_s;/*MDist;*/  MNoteRects1[n4+3] = y1;
            MNoteRects2[n4+0] = floorf(x1);                         MNoteRects2[n4+1] = y1;
            MNoteRects2[n4+2] = floorf(x1+s3) - mdist_s;/*MDist;*/  MNoteRects2[n4+3] = y2;
            break;
          case 1: // D
            MNoteRects1[n4+0] = floorf(x1+s1);                      MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1+s2) - mdist_s;/*MDist;*/  MNoteRects1[n4+3] = y1;
            MNoteRects2[n4+0] = floorf(x1);                         MNoteRects2[n4+1] = y1;
            MNoteRects2[n4+2] = floorf(x1+s3) - mdist_s;/*MDist;*/  MNoteRects2[n4+3] = y2;
            break;
          case 2: // E
            MNoteRects1[n4+0] = floorf(x1+s1);                      MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1+s3) - mdist_s;/*MDist;*/  MNoteRects1[n4+3] = y1;
            MNoteRects2[n4+0] = floorf(x1);                         MNoteRects2[n4+1] = y1;
            MNoteRects2[n4+2] = floorf(x1+s3) - mdist_s;/*MDist;*/  MNoteRects2[n4+3] = y2;
            break;
          case 3: // black
            MNoteRects1[n4+0] = floorf(x1);                         MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1+s2) - mdist_s;/*MDist;*/  MNoteRects1[n4+3] = y1 - mdist_s;//MDist;
            break;
        } // switch
      } // for note
    } // !vertical
  }

  //----------

  // which = 0..1, top/bottom

  virtual void fill_note_rects(SAT_Painter* APainter, int32_t note, uint32_t which, SAT_Color color) {
    SAT_Rect r;
    float x = getRect().x;
    float y = getRect().y;
    int32_t n = note * 4;
    switch (which) {
      case 1:
        r.x =     x + MNoteRects1[n  ];
        r.y =     y + MNoteRects1[n+1];
        r.setX2(  x + MNoteRects1[n+2] );
        r.setY2(  y + MNoteRects1[n+3] );
        break;
      case 2:
        r.x =     x + MNoteRects2[n  ];
        r.y =     y + MNoteRects2[n+1];
        r.setX2(  x + MNoteRects2[n+2] );
        r.setY2(  y + MNoteRects2[n+3] );
        break;
    }
    //APainter->fillRectangle( r, color );
    APainter->setFillColor(color);
    APainter->fillRect(r.x,r.y,r.w,r.h);

  }

  //----------

  virtual void draw_note(SAT_Painter* APainter, int32_t ANote, bool AActive=false) {
    int32_t typ = SAT_KEYBOARD_INTERNAL_NOTE_DATA[(ANote*2)+1];
    if (typ == 3) {
      if (AActive) {
        fill_note_rects(APainter,ANote,1,MBlackActiveColor);
      }
      else {
        fill_note_rects(APainter,ANote,1,MBlackColor);
      }
    }
    else {
      if (AActive) {
        fill_note_rects(APainter,ANote,1,MWhiteActiveColor);
        fill_note_rects(APainter,ANote,2,MWhiteActiveColor);
      }
      else {
        fill_note_rects(APainter,ANote,1,MWhiteColor);
        fill_note_rects(APainter,ANote,2,MWhiteColor);
      }
    } //type=0,1,2
  }

  //----------

  virtual int32_t find_note(float AXpos, float AYpos) {
    SAT_Rect mrect = getRect();
    //int32_t hit = -1;
    for (int32_t note=0; note<12; note++) {
      int32_t n4 = note*4;
      SAT_Rect r;
      r.x =  mrect.x + MNoteRects1[n4  ];
      r.y =  mrect.y + MNoteRects1[n4+1];
      r.w = (mrect.x + MNoteRects1[n4+2]) - r.x;
      r.h = (mrect.y + MNoteRects1[n4+3]) - r.y;
      if (r.contains(AXpos,AYpos)) return note;
      int32_t typ = SAT_KEYBOARD_INTERNAL_NOTE_DATA[/*n2*/(note*2)+1];
      if (typ < 3) { //0,1,2 = white keys
        r.x =  mrect.x + MNoteRects2[n4  ];
        r.y =  mrect.y + MNoteRects2[n4+1];
        r.w = (mrect.x + MNoteRects2[n4+2]) - r.x;
        r.h = (mrect.y + MNoteRects2[n4+3]) - r.y;
        if (r.contains(AXpos,AYpos)) return note;
      }
    }
    return -1;
  }


//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    recalc_note_rects();
    for (int32_t note=0; note<12; note++) {
      bool active = MActiveMap[note];
      draw_note(AContext->painter,note,active);
    }
    paintChildren(AContext);
    drawBorder(AContext);
    
  }

  //----------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    //SAT_DRect mrect = getRect();
    if (AButton == SAT_BUTTON_LEFT) {
      int32_t note = find_note(AXpos,AYpos);
      if (note >= 0) {
        if (MActiveMap[note]) deactivate(note);
        else activate(note);
        do_widget_update(this,0);
        do_widget_redraw(this,0);
      }
    }
  }

};

//----------------------------------------------------------------------
#endif