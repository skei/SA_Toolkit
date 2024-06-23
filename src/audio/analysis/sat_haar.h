#ifndef sat_haar_included
#define sat_haar_included
//----------------------------------------------------------------------


template <typename T>
class SAT_Haar {

//------------------------------
private:
//------------------------------

  //T buffer1[64 * 1024] = {};
  T avg[64 * 1024] = {};
  T diff[64 * 1024] = {};
  T tmpbuf[64 * 1024] = {};

  //T buffer2[64 * 1024] = {};
  //T avg2[64 * 1024] = {};
  //T diff2[64 * 1024] = {};

//------------------------------
public:
//------------------------------

  SAT_Haar() {
  }

  //----------

  ~SAT_Haar() {
  }

//------------------------------
public:
//------------------------------

  void process(T* buffer, uint32_t size) {
    uint32_t count = size;
    while (count > 1) {
      uint32_t i = 0;
      while (i < (count / 2)) {
        avg[i] = (buffer[2 * i] + buffer[(2 * i) + 1]) / 2;
        diff[i] = buffer[2 * i] - avg[i];
        i += 1;
      }
      i = 0;
      while (i < (count / 2)) {
        buffer[i] = avg[i];
        buffer[i + (count / 2)] = diff[i];
        i += 1;
      }
      count = count / 2;
      count > 1;
    }
  }

  //----------

  void reverse(T* buffer, uint32_t size) {
    uint32_t count = 2;
    while (count <= size) {
      uint32_t i = 0;
      while (i < (count/2)) {
        tmpbuf[ 2 * i]      = buffer[i] + buffer[i + (count / 2)];
        tmpbuf[(2 * i) + 1] = buffer[i] - buffer[i + (count / 2)];
        i += 1;
      }
      i = 0;
      while (i < count) {
        buffer[i] = tmpbuf[i];
        i += 1;
      }
      count *= 2;
    }
  }

//------------------------------
public:
//------------------------------

};


//----------------------------------------------------------------------
#endif









#if 0

desc:fx_haarmorph :: ccernn.2009 :: v0.0.1
// haar.wavelet.analysis/resynthesis
slider1:10<2,15,1>analysis bits
slider2:1<0,100,0.1>wavelets view.size;
slider3:0<0,1,0.01>morph;

//--------------------------------------------------
@init

  bits = 8;
  bufbits          = 8;
  bufsize          = 2^bufbits;
  bufpos           = 0;
  pdc_top_ch       = 2;
  pdc_bot_ch       = 0;
  pdc_delay        = bufsize;
  buffer1          = (64 * 1024) * 0;
  buffer2          = (64 * 1024) * 1;
  avg1             = (64 * 1024) * 2;
  diff1            = (64 * 1024) * 3;
  avg2             = (64 * 1024) * 4;
  diff2            = (64 * 1024) * 5;
  tmpbuf           = (64 * 1024) * 6;

//--------------------------------------------------
@slider

  bits = slider1;
  scale = slider2;
  morph = slider3;
  bits != bufbits ?
  (
    bufbits          = bits;
    bufsize          = 2^bufbits;
    bufpos           = 0;
    pdc_top_ch = 2;
    pdc_bot_ch = 0;
    pdc_delay  = bufsize;
  );

//--------------------------------------------------
@sample

  buffer1[bufpos] = (spl0 + spl1) * 0.5;
  buffer2[bufpos] = (spl2 + spl3) * 0.5;

  bufpos += 1;
  bufpos >= (bufsize) ?
  (
    bufpos = 0;

    //--- haar

    count = bufsize;
    while
    (
      i = 0;
      while
      (
        avg1[i]  = (buffer1[2*i] + buffer1[2*i + 1]) / 2;
        diff1[i] =  buffer1[2*i] - avg1[i];
        avg2[i]  = (buffer2[2*i] + buffer2[2*i + 1]) / 2;
        diff2[i] =  buffer2[2*i] - avg2[i];
        i += 1;
        i < (count/2);
      );
      i = 0;
      while
      (
        buffer1[i] = avg1[i];
        buffer1[i + count/2] = diff1[i];
        buffer2[i] = avg2[i];
        buffer2[i + count/2] = diff2[i];
        i += 1;
        (i < count/2);
      );
      count = count / 2;
      count > 1;
    );

    //--- morph

    i = 0;
    while
    (
      buffer1[i] = (buffer1[i] * (1-morph))  +  (buffer2[i] * (morph));
      i += 1;
      (i < bufsize);
    );

    //--- undo haar

    count = 2;
    while
    (
      i = 0;
      while
      (
        tmpbuf[2*i]   = buffer1[i] + buffer1[i+count/2];
        tmpbuf[2*i+1] = buffer1[i] - buffer1[i+count/2];
        i += 1;
        i < (count/2);
      );
      i = 0;
      while
      (
        buffer1[i] = tmpbuf[i];
        i += 1;
        (i < count);
      );
      count *= 2;
      count <= bufsize;
    );

  );

  spl0 = buffer1[bufpos];
  spl1 = spl0;


#endif // 0



#if 0

  // wavelet
  (drawmode==3) || (drawmode==4) ? (
    p = 1;   // pos/size
    l = 1;   // level
    yy  = (gfx_h/bits);
    y1  = 0; //gfx_h - yy;
    loop( bits,
      n = p;
      x1 = 0;
      xx = (gfx_w/p);
      loop( p,
        drawmode==3 ? an = ((viewbuf[n]*viewzoom) + 1) * 0.5;
        drawmode==4 ? an = abs(viewbuf[n])*viewzoom;
        gfx_r=an; gfx_g=an; gfx_b=an; gfx_a=1;
        gfx_x = x1;
        gfx_y = y1;
        gfx_rectto(x1+xx+1,y1+yy+1);
        n += 1;
        x1 += xx;
      );
      gfx_x = 0; gfx_y = y1;
      gfx_r=1; gfx_g=0; gfx_b=0; gfx_a=0.5;
      gfx_drawnumber((p*srate/size),2);
      p *= 2;
      l += 1;
      y1 += yy; //-= yy;
    );
  )

#endif // 0
