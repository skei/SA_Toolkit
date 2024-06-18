#ifndef sat_timeline_widget_included
#define sat_timeline_widget_included
//----------------------------------------------------------------------

#include "base/type/sat_list.h"
#include "gui/sat_widget.h"
#include "gui/widget/sat_visual_widget.h"

//----------

#define TRACK_HEIGHT      20
#define TRACK_WIDTH       75
#define PIXELS_PER_SECOND 7.0
#define SECONDS_PER_PIXEL (1.0 / PIXELS_PER_SECOND)
#define DRAG_HANDLE_SIZE  4

//----------------------------------------------------------------------
//
// segment
//
//----------------------------------------------------------------------

class SAT_TimelineSegment
: public SAT_ListNode {

//------------------------------
public:
//------------------------------

  const char* MName       = "segment";
  float       MStartPos   = 0.0f;
  float       MEndPos     = 1.0f;
  bool        MSelected   = false;

//------------------------------
  public:
//------------------------------

  void setName(const char* AName)   { MName = AName; }
  void setStart(float APos)         { MStartPos = APos; }
  void setEnd(float APos)           { MEndPos = APos; }
  void select(float ASelected=true) { MSelected = ASelected; }

  const char* getName(void)     { return MName; }
  float       getStart(void)    { return MStartPos; }
  float       getEnd(void)      { return MEndPos; }
  bool        isSelected(void)  { return MSelected; }

//------------------------------
public:
//------------------------------

  SAT_TimelineSegment(const char* AName, float AStart, float AEnd)
  : SAT_ListNode() {
    MName     = AName;
    MStartPos = AStart;
    MEndPos   = AEnd;
    MSelected = false;
  }

  //----------

  virtual ~SAT_TimelineSegment() {
  }

  //----------

  bool isInside(float ATime) {
    if ((ATime >= MStartPos) && (ATime <= MEndPos)) return true;
    else return false;
  }

};

//typedef list<SAT_TimelineSegment*> SAT_TimelineSegments;
//typedef SAT_Array<SAT_TimelineSegment*> SAT_TimelineSegments;

//----------------------------------------------------------------------
//
// track
//
//----------------------------------------------------------------------

class SAT_TimelineTrack
: public SAT_ListNode {

//------------------------------
private:
//------------------------------

  const char* MName     = "track";
  bool        MSelected = false;
  SAT_List   MSegments;
  //SAT_TimelineSegments MSegments;

//------------------------------
public:
//------------------------------

  const char*     getName(void) { return MName; }
  void            setName(char* AName) { MName = AName; }
  void            select(float ASelected=true) { MSelected = ASelected; }
  bool            isSelected(void) { return MSelected; }
  SAT_ListNode*  getSegments(void) { return MSegments.head(); }
  //SAT_TimelineSegments*  getSegments(void) { return &MSegments; }

//------------------------------
public:
//------------------------------

  SAT_TimelineTrack(const char* AName)
  : SAT_ListNode() {
    MName = AName;
    MSelected = false;
  }

  //----------

  virtual ~SAT_TimelineTrack() {
    #ifndef SAT_NO_AUTODELETE
    deleteSegments();
    #endif
  }

  //----------

  void addSegment(SAT_TimelineSegment* ASegment) {
    MSegments.insertTail(ASegment);
    //MSegments.push_back(ASegment);
  }

  //----------

  void deleteSegments(void) {
    //KListNode* node = MSegments.head();
    //while (node) {
    //  KTimeline_Segment* segment = (KTimeline_Segment*)node;
    //  node = node->next();
    //  delete segment;
    //}
    MSegments.clear();
  }

};

//typedef SAT_Array<SAT_TimelineTrack*> SAT_TimelineTracks;

//----------------------------------------------------------------------
//
// track
//
//----------------------------------------------------------------------

class SAT_TimelineWidget
: public SAT_VisualWidget {

//------------------------------
private:
//------------------------------

  float                 MZoom             = 4.0f;
  float                 MStartPos         = 0.0f;
  int32_t               MPrevMouseX       = 0;
  int32_t               MPrevMouseY       = 0;

//------------------------------
private:
//------------------------------

  SAT_TimelineTrack*   MHoverTrack       = nullptr;
  SAT_TimelineSegment* MHoverSegment     = nullptr;
  float                 MHoverTime        = -1.0f;
  bool                  MHoverLeft        = false;
  bool                  MHoverRight       = false;

//------------------------------
private:
//------------------------------

  SAT_TimelineSegment* MDraggingSegment  = nullptr;
  bool                  MDraggingLeft     = false;
  bool                  MDraggingRight    = false;

//------------------------------
protected:
//------------------------------

  SAT_Color MBackColor        = SAT_Grey;
  SAT_Color MTrackBackColor   = SAT_DarkerGrey;
  SAT_Color MTrackTextColor   = SAT_White;
  SAT_Color MSegmentBackColor = SAT_LightGrey;
  SAT_Color MSegmentTextColor = SAT_Black;
  SAT_Color MBorderColor      = SAT_Black;
  SAT_Color MTextColor        = SAT_Red;
  
  double    MTrackTextSize    = 10.0;
  double    MSegmentTextSize  = 10.0;

//------------------------------
private:
//------------------------------

  //SAT_TimelineTracks   MTracks;

  SAT_List MTracks;
  int32_t   MPlayPos;

//------------------------------
public:
//------------------------------

  //SAT_TimelineTracks*  getTracks(void)          { return &MTracks; }
  SAT_ListNode*  getTracks(void)                { return MTracks.head(); }
  int32_t         getPlayPos(void)              { return MPlayPos; }
  float           getZoom(void)                 { return MZoom; }
  float           getStartpos(void)             { return MStartPos; }

  void            setPlayPos(int32_t APlayPos)  { MPlayPos=APlayPos; }
  void            setZoom(float AZoom)          { MZoom = AZoom; }
  void            setStartpos(float APos)       { MStartPos = APos; }

//------------------------------
public:
//------------------------------

  SAT_TimelineWidget(SAT_Rect ARect)
  : SAT_VisualWidget(ARect) {
    setName("SAT_TimelineWidget");
    setHint("SAT_TimelineWidget");
    //setFlag(SAT_WIDGET_HOVER);
    //setFlag(SAT_WIDGET_CLIP);
    Options.wantHoverEvents = true;
  }

  //----------

  virtual ~SAT_TimelineWidget() {
    #ifndef SAT_NO_AUTODELETE
    deleteTracks();
    #endif
  }

//------------------------------
public:
//------------------------------

  void addTrack(SAT_TimelineTrack* ATrack) {
    //MTracks.append(ATrack);
    //MTracks.push_back(ATrack);
    MTracks.insertTail(ATrack);
  }

  //----------

  void deleteTracks(void) {
    //KListNode* node = MTracks.head();
    //while (node) {
    //  KTimeline_Track* track = (KTimeline_Track*)node;
    //  node = node->next();
    //  //track->deleteSegments();
    //  delete track;
    //}
    MTracks.clear();
  }

  //----------

  void drawTimeline(SAT_PaintContext* AContext) {
    SAT_Rect mrect = getRect();
    SAT_Painter* painter = AContext->painter;
    double S = getWindowScale();

    painter->setLineWidth(S);

    //painter->setFillColor(MBackColor);
    //painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
    
    painter->setDrawColor(MBorderColor);
    painter->drawLine(mrect.x,mrect.y,mrect.x2(),mrect.y);

    float x1 = mrect.x;
    float y1 = mrect.y;
    float x2 = x1 + (TRACK_WIDTH * S);
    float y2 = y1 + (TRACK_HEIGHT * S);

    SAT_TimelineTrack* track = (SAT_TimelineTrack*)MTracks.head();
    while (track) {

      SAT_Rect rect;

      // { track background }

      if (track == MHoverTrack) painter->setFillColor(0.3);
      else painter->setFillColor(MTrackBackColor);
      rect = SAT_Rect(x1,y1,(TRACK_WIDTH*S),(TRACK_HEIGHT*S));
      painter->fillRect(rect.x,rect.y,rect.w,rect.h);

      // { track name }

      painter->setTextSize(MTrackTextSize*S);
      painter->setTextColor(MTrackTextColor);
      rect.x += (5*S);
      rect.w -= (5*S);
      painter->drawTextBox(rect,track->getName(), SAT_TEXT_ALIGN_LEFT);//CENTER);

      // { track border }

      painter->setDrawColor(MBorderColor);      // below
      painter->drawLine(x1,y2,mrect.x2(),y2);   // below
      painter->drawLine(x2,y1,x2,y2);           // right

      // { track segments }

      SAT_TimelineSegment* segment = (SAT_TimelineSegment*)track->getSegments();
      while (segment) {

        float ss = segment->getStart() * MZoom * (PIXELS_PER_SECOND *S);
        float se = segment->getEnd() * MZoom * (PIXELS_PER_SECOND *S);
        float sw = se - ss;

        rect = SAT_Rect(x2+ss,y1,sw,TRACK_HEIGHT*S);

        // { back }

        if (segment == MHoverSegment) painter->setFillColor(0.7);
        else painter->setFillColor(MSegmentBackColor);
        painter->fillRect(rect.x,rect.y,rect.w,rect.h);

        // { name }

        painter->setTextColor(MSegmentTextColor);
        SAT_Rect rect2 = rect;
        rect2.x += (5*S);
        rect2.w -= (5*S);
        painter->drawTextBox(rect2,segment->getName(),SAT_TEXT_ALIGN_LEFT);//CENTER);

        // { border }

        //APainter->setColor(MBorderColor);
        //APainter->vertLine(x2+ss,y1,y2);
        //APainter->strokePath();
        //APainter->vertLine(x2+se,y1,y2);
        //APainter->strokePath();

        painter->setDrawColor(MBorderColor);
        painter->drawRect(rect.x,rect.y,rect.w,rect.h);

        // { resize indicators }

        if (segment == MHoverSegment) {
          painter->setFillColor( SAT_BrightBlue1 );
          if (MHoverLeft) {
            rect = SAT_Rect(x2+ss,y1,(DRAG_HANDLE_SIZE*S),(TRACK_HEIGHT*S));
            painter->fillRect(rect.x,rect.y,rect.w,rect.h);
          }
          if (MHoverRight) {
            rect = SAT_Rect(x2+1+SAT_Trunc(se)-(DRAG_HANDLE_SIZE*S),y1,(DRAG_HANDLE_SIZE*S),(TRACK_HEIGHT*S));
            painter->fillRect(rect.x,rect.y,rect.w,rect.h);
          }
        }

        segment = (SAT_TimelineSegment*)segment->next();
      } // while segment
      y1 += (TRACK_HEIGHT*S);
      track = (SAT_TimelineTrack*)track->next();
    } // while tnode
    float cur = MPlayPos * MZoom * (PIXELS_PER_SECOND*S);
    painter->setDrawColor( SAT_LightRed );
    painter->drawLine( x2 + cur, mrect.y, x2 + cur, mrect.y2() );
  }
  
//------------------------------
private:
//------------------------------

  float calcLength(int32_t APixels) {
    double S = getWindowScale();
    float time = (float)( APixels * (SECONDS_PER_PIXEL/S) );
    return time / MZoom;
  }

  //----------

  int32_t calcPixels(int32_t ATime) {
    double S = getWindowScale();
    float pixel = ATime * (PIXELS_PER_SECOND*S);
    return SAT_Trunc( pixel * MZoom );
  }

  //----------

  float calcTime(int32_t AXpos) {
    double S = getWindowScale();
    //KTrace("%i\n",AXpos);
    if (MZoom <= 0) return -1;
    float time = (float)( AXpos - (TRACK_WIDTH*S) );
    //KTrace("%.3f\n",time);
    if (time < 0) return -1;
    time *= (SECONDS_PER_PIXEL/S);
    //KTrace("%.3f\n",time);
    float result = (MStartPos + time) / MZoom;
    return result;
  }

  //----------

  float calcTimeDiff(int32_t AXpos, int32_t APrev) {
    double S = getWindowScale();
    if (MZoom <= 0) return 0;
    float diff = (float)( AXpos - APrev );
    //if time < 0 then exit(0);
    diff *= (SECONDS_PER_PIXEL/S);
    return diff / MZoom;
  }

  //----------

  int32_t calcXpos(float ATime) {
    double S = getWindowScale();
    if (ATime >= 0) {
      float pixel = ((ATime*MZoom) * (PIXELS_PER_SECOND*S));
      return  (TRACK_WIDTH*S) + SAT_Trunc(pixel);
    }
    return -1;
  }

  //----------

  /*
    TODO:
    - each track has individual height

  */

  SAT_TimelineTrack* findTrack(int32_t AYpos) {
    double S = getWindowScale();
    SAT_Rect mrect = getRect();
    int32_t y = AYpos - mrect.y;
    if (y < 0) return nullptr;
    int32_t t = y / (TRACK_HEIGHT*S);

    SAT_ListNode* node = (SAT_ListNode*)MTracks.head();
    while (node) {
      if (t == 0) return (SAT_TimelineTrack*)node;
      t -= 1;
      node = node->next();
    }
    return nullptr;

    //SAT_TimelineTracks::iterator iter = MTracks.begin();
    //while (iter != MTracks.end()) {
    //  SAT_TimelineTrack* track = *iter;
    //  if (t == 0) return track;
    //  t -= 1;
    //}

  }

  //----------

  SAT_TimelineSegment* findSegment(SAT_TimelineTrack* ATrack, float ATime) {

    //SAT_ListNode* node = (SAT_ListNode*)ATrack->getSegments()->head();
    SAT_ListNode* node = ATrack->getSegments();
    while (node) {
      SAT_TimelineSegment* segment = (SAT_TimelineSegment*)node;
      if ( (ATime >= segment->getStart()) && (ATime <= segment->getEnd()) ) return segment;
      node = node->next();
    }
    return nullptr;

    //SAT_TimelineSegments* segments = ATrack->getSegments();
    //SAT_TimelineSegments::iterator iter = segments->begin();
    //while (iter != segments->end()) {
    //  SAT_TimelineSegment* segment = *iter;
    //  if ( (ATime >= segment->getStart()) && (ATime <= segment->getEnd()) ) return segment;
    //}
    //return nullptr;
  }

//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    bool changed = false;
    if (AButton == SAT_BUTTON_LEFT) {
      MPrevMouseX = AXpos;
      MPrevMouseY = AYpos;
      if (!MHoverSegment) return;
      MDraggingSegment = MHoverSegment;
      MDraggingLeft = MHoverLeft;
      MDraggingRight = MHoverRight;
    }
    if (changed) do_widget_redraw(this);
    //KWidget::on_widgetMouseDown(AWidget,AXpos,AYpos,AButton,AState);
  }

  //----------

  void on_widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    bool changed = false;
    if (AButton == SAT_BUTTON_LEFT) {
      //if (!MDraggingSegment) return;
      MDraggingSegment = nullptr;
      MDraggingLeft = false;
      MDraggingRight = false;
    }
    if (changed) do_widget_redraw(this);
    //KWidget::on_widgetMouseUp(AWidget,AXpos,AYpos,AButton,AState);
  }

  //----------

  void on_widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATimeStamp) override {
    double S = getWindowScale();
    SAT_Rect mrect = getRect();
    bool changed = false;
    SAT_TimelineTrack* track = findTrack(AYpos);
    SAT_TimelineSegment* segment = nullptr;
    MHoverTime = calcTime(AXpos - mrect.x);
    if (MDraggingSegment) {
      float mintime = calcLength(10*S); // minimum 10 pixels
      float diff = calcTimeDiff(AXpos,MPrevMouseX);
      float st = MDraggingSegment->getStart();
      float en = MDraggingSegment->getEnd();
      if (MDraggingLeft) {
        st += diff;
        st = SAT_Max(st,0.0f);
        if (st > (en-mintime)) st = (en-mintime);
      }
      else if (MDraggingRight) {
        en += diff;
        if (en <  (st+mintime)) en = (st+mintime);
      }
      else {
        // dragging segment
        float le = en - st;
        st += diff;
        st = SAT_Max(st,0.0f);
        en = st + le;
      }
      MDraggingSegment->setStart(st);
      MDraggingSegment->setEnd(en);
      changed = true;
    } // dragging
    else { // not dragging
      bool hoverleft = false;
      bool hoverright = false;
      if (track) {
        segment = findSegment(track,MHoverTime);
        if (segment) {
          int32_t leftx  = calcXpos(segment->getStart());
          int32_t rightx = calcXpos(segment->getEnd());
          if ((AXpos-mrect.x) < (leftx+(DRAG_HANDLE_SIZE*S))) hoverleft = true;
          if ((AXpos-mrect.x) >= (rightx-(DRAG_HANDLE_SIZE*S))) hoverright = true;
        }
      }
      if (track != MHoverTrack) {
        MHoverTrack = track;
        changed = true;
      }
      if (segment != MHoverSegment) {
        MHoverSegment = segment;
        changed = true;
      }
      if (hoverleft != MHoverLeft) {
        MHoverLeft = hoverleft;
        changed = true;
      }
      if (hoverright != MHoverRight) {
        MHoverRight = hoverright;
        changed = true;
      }
    } // not dragging
    if (changed) do_widget_redraw(this);
    MPrevMouseX = AXpos;
    MPrevMouseY = AYpos;
  }

  //----------

  //void on_widget_paint(SAT_Painter* APainter, SAT_Rect ARect, uint32_t AMode=0) override {
  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawTimeline(AContext);
    paintChildren(AContext);
    drawBorder(AContext);
  }

  //----------

  void on_widget_leave(SAT_Widget* ATo, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    bool changed = false;
    if (MHoverTrack) {
      MHoverTrack = nullptr;
      changed = true;
    }
    if (MHoverSegment) {
      MHoverSegment = nullptr;
      changed = true;
    }
    if (changed) do_widget_redraw(this);
    //KWidget::on_widgetLeave(AWidget, int32_t AXpos, int32_t AYpos);
  }

};

//----------------------------------------------------------------------

#undef TRACK_HEIGHT
#undef TRACK_WIDTH
#undef PIXELS_PER_SECOND
#undef SECONDS_PER_PIXEL
#undef DRAG_HANDLE_SIZE

//----------------------------------------------------------------------
#endif