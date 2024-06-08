
/*
  included directly into kode_bitmap.h
*/

//----------------------------------------------------------------------
//
// noise
//
//----------------------------------------------------------------------

//------------------------------
public:
//------------------------------

  void genNoiseLayer(uint32_t ALayer) {
    for (uint32_t y=0; y<MHeight; y++) {
      uint8_t* ptr = (uint8_t*)getLinePtr(y);
      ptr += ALayer;
      for (uint32_t x=0; x<MWidth; x++) {
        uint8_t n = KODE_RandomRangeInt(0,255);
        //KODE_DPrint("%i, ",n);
        *ptr = n;
        ptr += 4;
      }
      //KODE_DPrint("\n");
    }
  }

  //----------

  void genNoise() {
    genNoiseLayer(0);
    genNoiseLayer(1);
    genNoiseLayer(2);
    //gen_noiseLayer(3);
    fillLayer(3,255);
  }

  //----------

  void genNoiseBW() {
    genNoiseLayer(0);
    copyLayer(1,0);
    copyLayer(2,0);
    //copyLayer(0,3);
    fillLayer(3,255);
  }

//----------------------------------------------------------------------
//
// voronoi
//
//----------------------------------------------------------------------

//------------------------------
private:
//------------------------------

  #define MAX_VORONOI_POINTS 1024

  struct gen_voronoi_t {
    int32_t   x;
    int32_t   y;
    uint32_t  c;
  };

  gen_voronoi_t gen_voronoi_areas[MAX_VORONOI_POINTS];

//------------------------------
public:
//------------------------------


  void genVoronoiLayer(uint32_t ALayer, uint32_t ANumPoints) {
    if (ANumPoints >= MAX_VORONOI_POINTS) ANumPoints = MAX_VORONOI_POINTS;
    uint32_t width = MWidth;//ABitmap->getWidth();
    uint32_t height = MHeight;//ABitmap->getHeight();
    //for (uint32 i=0; i<max_points; i++) {
    for (uint32_t i=0; i<ANumPoints; i++) {
      gen_voronoi_areas[i].x = KODE_RandomRangeInt(0,width-1);
      gen_voronoi_areas[i].y = KODE_RandomRangeInt(0,height-1);
      //voronoi_areas[i].c = bgra (rnd(255), rnd(255),rnd(255));
      gen_voronoi_areas[i].c = KODE_RandomRangeInt(0,255);
    }
    for (uint32_t y=0; y<height; y++) {
      uint8_t* ptr = (uint8_t*)getLinePtr(y);
      ptr += ALayer;
      for (uint32_t x=0; x<width; x++) {
        int32_t dist = width * height;
        //for (uint32 i=0; i<max_points; i++) {
        for (uint32_t i=0; i<ANumPoints; i++) {
          int32_t dx = gen_voronoi_areas[i].x - x;
          int32_t dy = gen_voronoi_areas[i].y - y;
          int32_t d = (dx * dx) + (dy * dy);
          if (d < dist) {
            dist = d;
            *ptr = gen_voronoi_areas[i].c;
          }
        }
        ptr += 4;
      }
    }
  }

  //----------

  void genVoronoi(uint32_t ANumPoints) {
    if (ANumPoints >= MAX_VORONOI_POINTS) ANumPoints = MAX_VORONOI_POINTS;
    uint32_t width = MWidth;
    uint32_t height = MHeight;
    for (uint32_t i=0; i<ANumPoints; i++) {
      gen_voronoi_areas[i].x = KODE_RandomRangeInt(0,width-1);
      gen_voronoi_areas[i].y = KODE_RandomRangeInt(0,height-1);
      uint8_t r = KODE_RandomRangeInt(0,255);
      uint8_t g = KODE_RandomRangeInt(0,255);
      uint8_t b = KODE_RandomRangeInt(0,255);
      gen_voronoi_areas[i].c = (r << 16) + (g << 8) + b;
    }
    for (uint32_t y=0; y<height; y++) {
      uint32_t* ptr = (uint32_t*)getLinePtr(y);
      for (uint32_t x=0; x<width; x++) {
        int32_t dist = width * height;
        //for (uint32 i=0; i<max_points; i++) {
        for (uint32_t i=0; i<ANumPoints; i++) {
          int32_t dx = gen_voronoi_areas[i].x - x;
          int32_t dy = gen_voronoi_areas[i].y - y;
          int32_t d = (dx * dx) + (dy * dy);
          if (d < dist) {
            dist = d;
            *ptr = gen_voronoi_areas[i].c;
          }
        }
        ptr += 1;
      }
    }
    fillLayer(3,255);
  }

  //----------

  void genVoronoiBW(uint32_t ANumPoints) {
    genVoronoiLayer(0,ANumPoints);
    copyLayer(1,0);
    copyLayer(2,0);
    //copyLayer(0,3);
    fillLayer(3,255);
  }

  //----------

  #undef MAX_VORONOI_POINTS

//----------------------------------------------------------------------
//
// fractal cloud
//
//----------------------------------------------------------------------

//------------------------------
private:
//------------------------------

  //int fraccloud_m    = 8;   // mul
  //int fraccloud_a    = 20;  // add
  //int fraccloud_blur = 3;   // blur

//------------------------------
private:
//------------------------------

  int gen_fraccloud_clamp(int x) {
    return (x < 0 ? 0 : (x > 255 ? 255 : x));
  }

//------------------------------
public:
//------------------------------

  //void setup(int mul, int add, int blur) {
  //  _m = mul;
  //  _a = add;
  // _blur = blur;
  //}

  //----------

  void genFractalCloudLayer(uint32_t ALayer, int32_t AMul=8, int32_t AAdd=20/*, int32_t ABlur=3*/) {

    //fillLayer(ALayer,0);

    int w = MWidth;
    int h = MHeight;
    int p2;
    int start_p = 256;
    start_p = (w > h) ? w : h;

    //setLayerValue(ALayer,0,  0,  KODE_RandomRangeInt(0,255));
    //setLayerValue(ALayer,w-1,0,  KODE_RandomRangeInt(0,255));
    //setLayerValue(ALayer,w-1,h-1,KODE_RandomRangeInt(0,255));
    //setLayerValue(ALayer,0,  h-1,KODE_RandomRangeInt(0,255));

    //for (int32_t y=0; y<h; y++) {
    //  for (int32_t x=0; x<w; x++) {
    //    setLayerValue(ALayer,x,y,KODE_RandomRangeInt(0,255));
    //  }
    //}

    for (int p=start_p; p>1; p=p2) {
      p2 = p >> 1;
      int k = p * AMul + AAdd;
      int k2 = k >> 1;
      for (int y=0; y<h; y+=p) {
        for (int x=0; x<w; x+=p) {
          // ac
          // bd
          int a = getLayerValue(ALayer, x,  y);
          int b = getLayerValue(ALayer, x,  y+p);
          int c = getLayerValue(ALayer, x+p,y);
          int d = getLayerValue(ALayer, x+p,y+p);
          //setLayerValue( ALayer, x,    y,    gen_fraccloud_clamp(((a+d)     >> 1) + ( KODE_RandomRangeInt(0,k) - k2)) );  // a
          setLayerValue( ALayer, x,    y+p2, gen_fraccloud_clamp(((a+b)     >> 1) + ( KODE_RandomRangeInt(0,k) - k2)) );    // b
          setLayerValue( ALayer, x+p2, y,    gen_fraccloud_clamp(((a+c)     >> 1) + ( KODE_RandomRangeInt(0,k) - k2)) );    // c
          setLayerValue( ALayer, x+p2, y+p2, gen_fraccloud_clamp(((a+b+c+d) >> 2) + ( KODE_RandomRangeInt(0,k) - k2)) );    // d

        }
      }
    }

//    // blur
//    for (int k = 0; k < ABlur; k++) {
//      for (int y = 0; y < h; y ++) {
//        for (int x = 0; x < w; x++) {
//          setLayerValue( x,y, ALayer, ( getLayerValue( ALayer, x,   y+1 )
//                                      + getLayerValue( ALayer, x+1, y   )
//                                      + getLayerValue( ALayer, x,   y-1 )
//                                      + getLayerValue( ALayer, x+1, y   ) ) >> 2 );
//        }
//      }
//    } // blur

  }

  //----------

  void genFractalCloud(int32_t AMul=8, int32_t AAdd=20/*, int32_t ABlur=3*/) {
    genFractalCloudLayer(0,AMul,AAdd/*,ABlur*/);
    genFractalCloudLayer(1,AMul,AAdd/*,ABlur*/);
    genFractalCloudLayer(2,AMul,AAdd/*,ABlur*/);
    //genFractalCloudLayer(3,AMul,AAdd/*,ABlur*/);
    fillLayer(3,255);
  }

  //----------

  void genFractalCloudBW(int32_t AMul=8, int32_t AAdd=20/*, int32_t ABlur=3*/) {
    genFractalCloudLayer(0,AMul,AAdd/*,ABlur*/);
    copyLayer(1,0);
    copyLayer(2,0);
    //copyLayer(0,3);
    fillLayer(3,255);
  }

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------
