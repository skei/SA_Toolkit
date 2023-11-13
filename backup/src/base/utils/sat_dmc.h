#ifndef sat_dmc_included
#define sat_dmc_included
//----------------------------------------------------------------------
/*
  Dynamic Markov Compression (DMC)

  http://en.wikipedia.org/wiki/Dynamic_Markov_Compression

  (http://plg1.cs.uwaterloo.ca/~ftp/dmc/)

  http://www.jjj.de/crs4/dmc.c
  http://www.jjj.de/crs4/

  "Data Compression Using Dynamic Markov Modelling":
  http://webhome.cs.uvic.ca/~nigelh/Publications/DMC.pdf

  (https://wwwtest.cs.uwaterloo.ca/research/tr/1986/CS-86-18.pdf)
  (http://www.comp.nus.edu.sg/~junping/docs/njp-icita2005.pdf)



  - predictive arithmetic coding
  - good compression ratio
  - not widely implemented
*/

//#include "base/sat_memory.h"

//----------

#ifndef SAT_EOF
  #define SAT_EOF -1
#endif

#define SAT_DMC_THRESHOLD 2 // 8
#define SAT_DMC_BIGTHRESH 2 // 8

// threshold = 32;//2;
// bigthresh = 32;//2;

//----------------------------------------------------------------------

//struct sat_dmc_node {
//  float count[2];
//  sat_dmc_node* next[2];
//};

//----------------------------------------------------------------------

class SAT_Dmc {

  struct sat_dmc_node {
    float         count[2]  = {0};
    sat_dmc_node* next[2]   = {0};
  };

//------------------------------
private:
//------------------------------

  unsigned char*  rbuf = nullptr;
  int             rbufsize = 0;
  int             rbufpos = 0;
  unsigned char*  wbuf = nullptr;
//int             wbufsize = 0;
  int             wbufpos = 0;

//------------------------------
private:
//------------------------------

  int             memsize = 0;
  int             threshold = 0;
  int             bigthresh = 0;
  sat_dmc_node   *P = nullptr;
  sat_dmc_node   *N = nullptr;
  sat_dmc_node   nodes[256][256] = {};
  sat_dmc_node   *nodebuf = nullptr;
  sat_dmc_node   *nodemaxp = nullptr;
  sat_dmc_node   *nodesptr = nullptr;

//------------------------------
public:
//------------------------------

  SAT_Dmc() {
    rbuf = nullptr;//NULL;
    rbufsize = 0;
    rbufpos = 0;
    wbuf = nullptr;
    //wbufsize = 0;
    wbufpos = 0;
    memsize   = 0x1000000;
    threshold = SAT_DMC_THRESHOLD;//2;
    bigthresh = SAT_DMC_BIGTHRESH;//2;
    nodebuf = nullptr;
  }

  //----------

  ~SAT_Dmc() {
  }


//------------------------------
private:
//------------------------------

  int get_char(void) {
    //return getchar();
    unsigned char c;
    if (rbufpos<rbufsize) {
      c = rbuf[rbufpos++];
      return (int)c;
    }
    else return SAT_EOF;//-1;
  }

  //----------

  void put_char(int c) {
    //putchar(c);
    //if (wbufpos<wbufsize)
    //{
      unsigned char ch = (unsigned char)c;
      wbuf[wbufpos++] = ch;
      //return (int)ch;
    //}
    //else return SAT_EOF;//-1
  }

  //----------

  void p_init(int memsize) {
    //fprintf(stderr,"using %d bytes of predictor memory\n",memsize);
    nodebuf = (sat_dmc_node *)malloc(memsize);
    //if (nodebuf == (sat_dmc_node *)NULL)
    //{
    //  fprintf(stderr,"memory alloc failed; try smaller predictor memory\n");
    //  exit(1);
    //}
    nodemaxp = nodebuf + (memsize/sizeof(sat_dmc_node)) - 20;
    p_flush();
  }

  //----------

  void p_cleanup(void) {
    if (nodebuf) free(nodebuf);
    nodebuf = nullptr;
  }

  //----------

  void p_flush(void) {
    int i,j;
    for (j=0;j<256;j++) {
      for (i=0;i<127;i++) {
        nodes[j][i].count[0] = 0.2;
        nodes[j][i].count[1] = 0.2;
        nodes[j][i].next[0] = &nodes[j][2*i + 1];
        nodes[j][i].next[1] = &nodes[j][2*i + 2];
      }
      for (i=127;i<255;i++) {
        nodes[j][i].count[0] = 0.2;
        nodes[j][i].count[1] = 0.2;
        nodes[j][i].next[0] = &nodes[i+1][0];
        nodes[j][i].next[1] = &nodes[i-127][0];
      }
    }
    nodesptr = nodebuf;
    p_reset();
  }

  //----------

  void p_reset(void) {
    P = &nodes[0][0];
  }

  //----------

  //float predict(void) {
  //  return P->count[0] / (P->count[0] + P->count[1]);
  //}

  //----------

  void p_update(int b) {
    float r;
    if (P->count[b] >= threshold && P->next[b]->count[0]+P->next[b]->count[1] >= bigthresh + P->count[b]) {
      N = nodesptr++;
      P->next[b]->count[0] -= N->count[0] = P->next[b]->count[0] * (r = P->count[b]/(P->next[b]->count[1]+P->next[b]->count[0]));
      P->next[b]->count[1] -= N->count[1] = P->next[b]->count[1] * r;
      N->next[0] = P->next[b]->next[0];
      N->next[1] = P->next[b]->next[1];
      P->next[b] = N;
    }
    P->count[b]++;
    P = P->next[b];
    if (nodesptr > nodemaxp) {
      //fprintf(stderr,"flushing ...\n");
      p_flush();
    }
  }

  //----------

  float predict(void) {
    return P->count[0] / (P->count[0] + P->count[1]);
  }

//------------------------------
public:
//------------------------------

  #ifndef SAT_NO_DMC_COMPRESS

  int compress(unsigned char* outbuf, unsigned char* inbuf, int insize) {
    rbuf     = inbuf;
    rbufsize = insize;
    rbufpos  = 0;
    wbuf     = outbuf;
    //wbufsize = 0;
    wbufpos  = 0;
    int max = 0x1000000;
    int min = 0;
    int inbytes  = 0;
    int outbytes = 3;
    int pout     = 3;
    p_init(memsize);
    //TODO: for (0..buffersize)
    for(;;) {
      int c = get_char(); /**/
      if (c == SAT_EOF) { /**/
        min = max-1;
        //fprintf(stderr,"compress done: bytes in %d, bytes out %d, ratio %f\n",inbytes,outbytes,(float)outbytes/inbytes);
        break;
      }
      for (int i=0;i<8;i++) {
        int bit = (c << i) & 0x80;
        int mid = min + (max-min-1) * predict();
        p_update(bit != 0);
        if (mid == min) mid++;
        if (mid == (max-1)) mid--;
        if (bit) { min = mid; }
        else { max = mid; }
        while ((max-min) < 256) {
          if (bit) max--;
          put_char(min >> 16); /**/
          outbytes++;
          min = (min << 8) & 0xffff00;
          max = ((max << 8) & 0xffff00 ) ;
          if (min >= max) max = 0x1000000;
        } //max min
      } //i
      if (!(++inbytes & 0xff)) {
        //if (!(inbytes & 0xffff))
        //{
        //  //fprintf(stderr,"compressing... bytes in %d, bytes out %d, ratio %f\r",inbytes,outbytes,(float)outbytes/inbytes);
        //}
        if (outbytes - pout > 256) { p_flush(); } // compression failing
        pout = outbytes;
      } //inbytes
    } //for
    put_char( min>>16); /**/
    put_char((min>>8) & 0xff); /**/
    put_char( min & 0x00ff); /**/
    p_cleanup();
    return wbufpos;
  }

  #endif // SAT_NO_DMC_COMPRESS

//------------------------------
//
//------------------------------

  int expand(unsigned char* outbuf, unsigned char* inbuf, int insize)
  {
    rbuf     = inbuf;
    rbufsize = insize;
    rbufpos  = 0;
    wbuf     = outbuf;
  //wbufsize = 0;
    wbufpos  = 0;
    int max = 0x1000000;
    int min = 0;
    // mid,
    int val;
    //    i,
    int inbytes=3;
    int pin=3;
    int outbytes=0;
    //    bit,
    //    c;
    p_init(memsize);
    val  = get_char()<<16; /**/
    val += get_char()<<8; /**/
    val += get_char(); /**/
    while (1) {
      int c = 0;
      if (val == (max-1)) {
        //fprintf(stderr,"expand: input %d output %d\n",inbytes,outbytes);
        break;
      }
      for (int i=0;i<8;i++) {
        int mid = min + (max-min-1)*predict();
        if (mid == min) mid++;
        if (mid == (max-1)) mid--;
        int bit;
        if (val >= mid) {
          bit = 1;
          min = mid;
        }
        else {
          bit = 0;
          max = mid;
        }
        p_update(bit != 0);
        c = c + c + bit;
        while ((max-min) < 256) {
          if (bit)max--;
          inbytes++;
          //val =  (val << 8) & 0xffff00  | (get_char()& 0xff); // warning: suggest parentheses around arithmetic in operand
          val = ((val << 8) & 0xffff00) | (get_char()& 0xff); /**/
          min =  (min << 8) & 0xffff00;
          max = ((max << 8) & 0xffff00);
          if (min>=max) max = 0x1000000;
        } //max min
      } //for
      put_char(c); /**/
      if (!(++outbytes & 0xff)) {
        if (inbytes - pin > 256) { p_flush(); } /* compression was failing */
        pin = inbytes;
      }
    } //while
    p_cleanup();
    return wbufpos;
  }

};

//----------------------------------------------------------------------
#endif
