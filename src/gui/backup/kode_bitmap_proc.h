
/*
  included directly into kode_bitmap.h
*/

//----------------------------------------------------------------------
//
// box blur
//
//----------------------------------------------------------------------

// https://www.openprocessing.org/sketch/59221#

//#define MAX_SIZE    4096
//#define MAX_RADIUS  256
//
//class K64_procBlurBox {
//
//  private:
//    K64_Bitmap  temp_buffer;
//    uint32      vmin[MAX_SIZE]; // max(w,h);
//    uint32      vmax[MAX_SIZE];
//    uint32      MKernel[256*(MAX_RADIUS*2+1)];
//    int32       MKernelRadius;
//
//  public:
//
//    NO_INLINE
//    void setupBuffer(uint32 AWidth, uint32 AHeight) {
//      temp_buffer.setup(AWidth,AHeight);
//    }
//
//    //----------
//
//    /*
//      AWeight not used
//      just to make the 'api' compatible with gaussian blur
//    */
//
//    NO_INLINE
//    void createKernel(int32 radius, float AWeight) {
//      int32 div = radius + radius + 1;
//      for (int32 i=0; i<256*div; i++) {
//        MKernel[i] = (i / div);
//      }
//      MKernelRadius = radius;
//    }
//
//    //----------
//
//    NO_INLINE
//    void processHorizontal(K64_Bitmap* ADst, K64_Bitmap* ASrc) {
//      if (MKernelRadius < 1) return;
//      uint32 W = ASrc->getWidth();
//      uint32 H = ASrc->getHeight();
//      uint32 WM = W - 1;
//      for (uint32 y=0; y<H; y++) {
//        uint32 rsum = 0;
//        uint32 gsum = 0;
//        uint32 bsum = 0;
//        for (int32 i=-MKernelRadius; i<=MKernelRadius; i++) {
//          uint32 pos = min(WM,max(i,0));
//          uint32 p = ASrc->getPixel(pos,y);
//          rsum += (p & 0xff0000) >> 16;
//          gsum += (p & 0x00ff00) >> 8;
//          bsum += (p & 0x0000ff);
//        }
//        for (uint32 x=0; x<W; x++) {
//          //if (rsum >= MMaxKernelIndex) printf("rsum %i MMaxKernelIndex %i\n",rsum,MMaxKernelIndex);
//          //if (gsum >= MMaxKernelIndex) printf("gsum %i MMaxKernelIndex %i\n",gsum,MMaxKernelIndex);
//          //if (bsum >= MMaxKernelIndex) printf("bsum %i MMaxKernelIndex %i\n",bsum,MMaxKernelIndex);
//          //k64_assert( rsum < MMaxKernelIndex );
//          //k64_assert( gsum < MMaxKernelIndex );
//          //k64_assert( bsum < MMaxKernelIndex );
//          ADst->setPixel(x,y,bgra(MKernel[rsum], MKernel[gsum], MKernel[bsum]) );
//          if (y==0) {
//            vmin[x] = min( x + MKernelRadius + 1, WM );
//            vmax[x] = max( x - MKernelRadius,     0  );
//          }
//          uint32 p1 = ASrc->getPixel(vmin[x],y);
//          uint32 p2 = ASrc->getPixel(vmax[x],y);
//          rsum += (p1 & 0xff0000) >> 16;
//          gsum += (p1 & 0x00ff00) >> 8;
//          bsum += (p1 & 0x0000ff);
//          rsum -= (p2 & 0xff0000) >> 16;
//          gsum -= (p2 & 0x00ff00) >> 8;
//          bsum -= (p2 & 0x0000ff);
//        }
//      }
//    }
//
//    //----------
//
//    NO_INLINE
//    void processVertical(K64_Bitmap* ADst, K64_Bitmap* ASrc) {
//      if (MKernelRadius < 1) return;
//      uint32 W = ASrc->getWidth();
//      uint32 H = ASrc->getHeight();
//      uint32 HM = H - 1;
//      for (uint32 x=0; x<W; x++) {
//        uint32 rsum = 0;
//        uint32 gsum = 0;
//        uint32 bsum = 0;
//        for (int32 i=-MKernelRadius; i<=MKernelRadius; i++) {
//          uint32 pos = min(HM,max(i,0));
//          uint32 p = ASrc->getPixel(x,pos);
//          rsum += (p & 0xff0000) >> 16;
//          gsum += (p & 0x00ff00) >> 8;
//          bsum +=  p & 0x0000ff;
//        }
//        for (uint32 y=0;y<H;y++){
//          ADst->setPixel(x ,y, bgra(MKernel[rsum], MKernel[gsum], MKernel[bsum] ));
//          if (x==0) {
//            vmin[y] = min(y + MKernelRadius + 1, HM );
//            vmax[y] = max(y - MKernelRadius,     0  );
//          }
//          uint32 p1 = ASrc->getPixel(x,vmin[y]);
//          uint32 p2 = ASrc->getPixel(x,vmax[y]);
//          rsum += (p1 & 0xff0000) >> 16;
//          gsum += (p1 & 0x00ff00) >> 8;
//          bsum += (p1 & 0x0000ff);
//          rsum -= (p2 & 0xff0000) >> 16;
//          gsum -= (p2 & 0x00ff00) >> 8;
//          bsum -= (p2 & 0x0000ff);
//        }
//      }
//    }
//
//    //----------
//
//    NO_INLINE
//    void process(K64_Bitmap* ADst, K64_Bitmap* ASrc) {
//      processHorizontal(&temp_buffer,ASrc);
//      processVertical(ADst,&temp_buffer);
//    }
//
//};

//----------------------------------------------------------------------
//
// gaussian blur
//
//----------------------------------------------------------------------


//#define MAX_BLUR_KERNEL_SIZE    256
//#define MAX_BLUR_WIDTH          640
//#define MAX_BLUR_HEIGHT         480
//
//class K64_procBlurGaussian {
//
//  private:
//
//    float       MBlurKernel[MAX_BLUR_KERNEL_SIZE+1] = {0};
//    int32       MBlurKernelRadius                   = 0;
//    K64_Bitmap  MTemp;
//
//  public:
//
//    NO_INLINE
//    void setupBuffer(uint32 AWidth, uint32 AHeight) {
//      MTemp.setup(AWidth,AHeight);
//    }
//
//    // 1d gaussian bell shaped kernel
//
//    NO_INLINE
//    void createKernel(int32 ARadius, float AWeight) {
//      int mem_amount = (ARadius*2)+1;
//      float twoRadiusSquaredRecip = 1.0 / (2.0 * ARadius * ARadius);
//      float sqrtTwoPiTimesRadiusRecip = 1.0 / (sqrtf(K64_PI2) * ARadius);
//      float radiusModifier = AWeight;
//      int r = -ARadius;
//      float sum = 0.0f;
//      for (int i = 0; i < mem_amount; i++) {
//        float x = r * radiusModifier;
//        x *= x;
//        float v = sqrtTwoPiTimesRadiusRecip * expf(-x * twoRadiusSquaredRecip);
//        //gaussian_kernel[i] = v;
//        MBlurKernel[i] = v;
//        sum += v;
//        r++;
//      }
//      // Normalize distribution
//      float div = sum;
//      for (int i=0; i<mem_amount; i++) MBlurKernel[i] /= div;
//      MBlurKernelRadius = ARadius;
//    }
//
//    //----------
//
//    NO_INLINE
//    void processHorizontal(K64_Bitmap* ADst, K64_Bitmap* ASrc, uint32 AChannel) {
//      if (MBlurKernelRadius == 0) return;
//      uint32 pixels_in_row = 1 + (MBlurKernelRadius*2);
//      uint32 width = ADst->getWidth();
//      uint32 height = ADst->getHeight();
//      for (uint32 y=0; y<height; y++) {
//        uint8* dst = ADst->getLinePtr(y);
//        dst += AChannel;
//        for (uint32 x=0; x<width; x++) {
//          float blurred_value = 0.0f;
//          for (uint32 xoffset=0; xoffset<pixels_in_row; xoffset++) {
//            int32 sx = (x - MBlurKernelRadius) + xoffset;
//            sx = clampI_minmax(sx,0,(width-1));
//            float v = ASrc->getPixelChannel(sx,y,AChannel);
//            blurred_value += ( v * MBlurKernel[xoffset] );
//          }
//          *dst = (uint8)blurred_value;
//          dst += 4;
//        }
//      }
//    }
//
//    //----------
//
//    NO_INLINE
//    void processHorizontal(K64_Bitmap* ADst, K64_Bitmap* ASrc) {
//      if (MBlurKernelRadius == 0) return;
//      uint32 pixels_in_row = 1 + (MBlurKernelRadius*2);
//      uint32 width = ADst->getWidth();
//      uint32 height = ADst->getHeight();
//      for (uint32 y=0; y<height; y++) {
//        uint8* dst = ADst->getLinePtr(y);
//        for (uint32 x=0; x<width; x++) {
//          float blurred_r = 0.0f;
//          float blurred_g = 0.0f;
//          float blurred_b = 0.0f;
//          for (uint32 xoffset=0; xoffset<pixels_in_row; xoffset++) {
//            int32 sx = (x - MBlurKernelRadius) + xoffset;
//            sx = clampI_minmax(sx,0,(width-1));
//            uint32 pixel = ASrc->getPixel(sx,y);
//            float r = (pixel & 0xff0000) >> 16;
//            float g = (pixel & 0x00ff00) >> 8;
//            float b = (pixel & 0x0000ff);
//            blurred_r += ( r * MBlurKernel[xoffset] );
//            blurred_g += ( g * MBlurKernel[xoffset] );
//            blurred_b += ( b * MBlurKernel[xoffset] );
//          }
//          *dst++ = (uint8)blurred_b;
//          *dst++ = (uint8)blurred_g;
//          *dst++ = (uint8)blurred_r;
//          *dst++ = ASrc->getPixelChannel(x,y,3);
//        }
//      }
//    }
//
//    //----------
//
//    NO_INLINE
//    void processVertical(K64_Bitmap* ADst, K64_Bitmap* ASrc, uint32 AChannel) {
//      if (MBlurKernelRadius == 0) return;
//      uint32 pixels_in_row = 1 + (MBlurKernelRadius*2);
//      uint32 width = ADst->getWidth();
//      uint32 height = ADst->getHeight();
//      for (uint32 y=0; y<height; y++) {
//        uint8* dst = ADst->getLinePtr(y);
//        dst += AChannel;
//        for (uint32 x=0; x<width; x++) {
//
//          float blurred_value = 0.0f;
//          for (uint32 yoffset=0; yoffset<pixels_in_row; yoffset++) {
//            int32 sy = (y - MBlurKernelRadius) + yoffset;
//            sy = clampI_minmax(sy,0,(height-1));
//            float v = ASrc->getPixelChannel(x,sy,AChannel);
//            blurred_value += ( v * MBlurKernel[yoffset] );
//          }
//
//          *dst = (uint8)blurred_value;
//          dst += 4;
//        }
//      }
//    }
//
//    NO_INLINE
//    void processVertical(K64_Bitmap* ADst, K64_Bitmap* ASrc) {
//      if (MBlurKernelRadius == 0) return;
//      uint32 pixels_in_row = 1 + (MBlurKernelRadius*2);
//      uint32 width = ADst->getWidth();
//      uint32 height = ADst->getHeight();
//      for (uint32 y=0; y<height; y++) {
//        uint8* dst = ADst->getLinePtr(y);
//        for (uint32 x=0; x<width; x++) {
//          float blurred_r = 0.0f;
//          float blurred_g = 0.0f;
//          float blurred_b = 0.0f;
//          for (uint32 yoffset=0; yoffset<pixels_in_row; yoffset++) {
//            int32 sy = (y - MBlurKernelRadius) + yoffset;
//            sy = clampI_minmax(sy,0,(height-1));
//            uint32 pixel = ASrc->getPixel(x,sy);
//            float r = (pixel & 0xff0000) >> 16;
//            float g = (pixel & 0x00ff00) >> 8;
//            float b = (pixel & 0x0000ff);
//            blurred_r += ( r * MBlurKernel[yoffset] );
//            blurred_g += ( g * MBlurKernel[yoffset] );
//            blurred_b += ( b * MBlurKernel[yoffset] );
//          }
//          *dst++ = (uint8)blurred_b;
//          *dst++ = (uint8)blurred_g;
//          *dst++ = (uint8)blurred_r;
//          *dst++ = ASrc->getPixelChannel(x,y,3);
//        }
//      }
//    }
//
//    //----------
//
//    void process(K64_Bitmap* ADst, K64_Bitmap* ASrc) {
//      processHorizontal(&MTemp,ASrc);
//      processVertical(ADst,&MTemp);
//    }
//
//    void process(K64_Bitmap* ADst, K64_Bitmap* ASrc, uint32 AChannel) {
//      processHorizontal(&MTemp,ASrc,AChannel);
//      processVertical(ADst,&MTemp,AChannel);
//    }
//
//};

//----------------------------------------------------------------------
//
// radial blur
//
//----------------------------------------------------------------------

//class K64_procBlurRadial {
//
//  private:
//
//    K64_Bitmap      polar_buffer;
//    K64_Bitmap      blur_buffer;
//    K64_procBlurBox proc_blur;
//
//  public:
//
//    NO_INLINE
//    void setupBuffer(uint32 AWidth, uint32 AHeight) {
//      polar_buffer.setup(AWidth,AHeight);
//      blur_buffer.setup(AWidth,AHeight);
//    }
//
//    //----------
//
//    NO_INLINE
//    void createKernel(int32 radius, float weight) {
//      proc_blur.createKernel(radius,weight);
//    }
//
//    //----------
//
//    NO_INLINE
//    void cartesian_to_polar(K64_Bitmap* ABitmap, int mouseX, int mouseY) {
//      uint32 width = ABitmap->getWidth();
//      uint32 height = ABitmap->getHeight();
//      int step = 1;
//      for (uint32 y=0; y<height; y+=step) {
//        for (uint32 x=0; x<width; x+=step) {
//
//          float cx = x - (width/2);
//          float cy = y - (height/2);
//
//          float pr = sqrtf(cx*cx + cy*cy);    // radius
//          float pa = atan2(cy,cx);            // angle
//          if (pa<0) pa = K64_PI2 + pa;        // 0..PI2
//          pa = (pa * K64_INVPI2) * width;
//
//          uint32 color = ABitmap->getPixel(pa,pr);
//          polar_buffer.setPixel(x,y,color);
//
//	      }
//      }
//    }
//
//    //----------
//
//    NO_INLINE
//    void polar_to_cartesian(K64_Bitmap* ABitmap, int mouseX, int mouseY) {
//      uint32 width  = blur_buffer.getWidth();
//      uint32 height = blur_buffer.getHeight();
//      int step = 1;
//      for (uint32 y=0; y<height; y+=step) {
//        for (uint32 x=0; x<width; x+=step) {
//
//
//          float pa = x / width * K64_PI2; // 0..PI2
//          float pr = y;
//
//          //float cx = x - (width/2);
//          //float cy = y - (height/2);
//          //float pr = sqrtf(cx*cx + cy*cy);    // radius
//          //float pa = atan2(cy,cx);            // angle
//          //if (pa<0) pa = K64_PI2 + pa;        // 0..PI2
//
//          float cx = pr * cos(pa);
//          float cy = pr * sin(pa);
//
//          uint32 color = ABitmap->getPixel(cx,cy);
//          polar_buffer.setPixel(x,y,color);
//
//        }
//      }
//    }
//
//    //----------
//
//    NO_INLINE
//    void process(K64_Bitmap* ADst, K64_Bitmap* ASrc, int mouseX, int mouseY) {
//      cartesian_to_polar(ASrc,mouseX,mouseY); // -> temp1
//      proc_blur.processVertical(&blur_buffer,&polar_buffer);
//      polar_to_cartesian(ADst,mouseX,mouseY); // -> temp1
//
//    }
//
//    //----------
//
//};

//------------------------------

//// http://www.hugi.scene.org/online/coding/hugi%2015%20-%20coradblu.htm
//
///*
//
//private:
//
//  void radial_blur_process(int32 x, int32 y, int32 cx, int32 cy, float s, uint8 b) {
//    vec2 v = vec2(cx,cy);
//    v -= vec2(x,y);
//    v.normalize();
//
//    // (a)
//
//    uint32 p = getPixel(x,y);
//    v *= s;
//    uint32 vp  = getPixel(x+v.x,y+v.y);
//    uint32 svp = color_scale_ext(vp,b);
//    uint32 color = color_add_sat(p,svp);
//
//    //uint32 color = color_average(p,svp);
//    //uint32 color = color_blend(p,vp,b);
//    //uint32 color = color_sub_sat(p,svp);
//    //uint32 color = color_mul_sat(p,svp);
//
//    // (b)
//
//    //uint32 color  = getPixel(x,y);
//    //for (uint32 i=1; i<=s; i++) {
//    //  vec2 v2 = v;
//    //  v *= i;
//    //  uint32 vp  = getPixel(x+v2.x,y+v2.y);
//    //  uint32 svp = color_alpha(vp,b);
//    //  color = color_add_sat(color,svp);
//    //}
//
//    //uint32 color = color_add_sat(p,pp);
//    setPixel(x,y,color);
//  }
//
//public:
//
//  void radial_blur(int32 AXcenter, int32 AYcenter, float AScale, uint8 ABlend) {
//    if (AXcenter < 1) AXcenter = 1;
//    if (AYcenter < 1) AYcenter = 1;
//    if (AXcenter > (int32)MWidth-2)  AXcenter = MWidth  - 2;
//    if (AYcenter > (int32)MHeight-2) AYcenter = MHeight - 2;
//    for (uint32 y=AYcenter; y>0; y--) { // upper half
//      for (uint32 x=AXcenter; x>0; x--) { // upper left quadrant
//        radial_blur_process(x,y,AXcenter,AYcenter,AScale,ABlend);
//      }
//      for (uint32 x=AXcenter+1; x<MWidth; x++) { // upper right quadrant
//        radial_blur_process(x,y,AXcenter,AYcenter,AScale,ABlend);
//      }
//    }
//    for (uint32 y=AYcenter+1; y<MHeight; y++) { // lower half
//      for (uint32 x=AXcenter; x>0; x--) { // lower left quadrant
//        radial_blur_process(x,y,AXcenter,AYcenter,AScale,ABlend);
//      }
//      for (uint32 x=AXcenter+1; x<MWidth; x++) { // lower right quadrant
//        radial_blur_process(x,y,AXcenter,AYcenter,AScale,ABlend);
//      }
//    }
//  };
//
//*/

//----------------------------------------------------------------------
//
// convolve
//
//----------------------------------------------------------------------

// http://lodev.org/cgtutor/filtering.html

//#define MAX_KERNEL_WIDTH 256
//#define MAX_KERNEL_HEIGHT 256
//
//struct k64_proc_convolve_setup_t {
//  uint32 width;
//  uint32 height;
//  float* coeffs;
//  float factor;
//  float bias;
//};
//
//class K64_procConvolve {
//
//  private:
//
//    /*
//    #define filterWidth 3
//    #define filterHeight 3
//    double filter[filterHeight][filterWidth] = {
//      0, 0, 0,
//      0, 1, 0,
//      0, 0, 0
//    };
//    */
//
//    uint32  MKernelWidth = 0;
//    uint32  MKernelHeight = 0;
//    float   MKernel[MAX_KERNEL_HEIGHT][MAX_KERNEL_WIDTH] = {{0}};
//    float   MKernelFactor = 1.0f;
//    float   MKernelBias = 0.0f;
//
//  public:
//
//    //void setupKernel(uint32 AWidth, uint32 AHeight, float* ACoeffs, float AFactor, float ABias) {
//    //  uint32 i = 0;
//    //  MKernelWidth = AWidth;
//    //  MKernelHeight = AHeight;
//    //  for(uint32 y=0; y<AHeight; y++) {
//    //    for(uint32 x=0; x<AWidth; x++) {
//    //      MKernel[y][x] = ACoeffs[i++];
//    //    }
//    //  }
//    //  MKernelFactor = AFactor;
//    //  MKernelBias = ABias;
//    //}
//
//    //----------
//
//    NO_INLINE
//    void setupKernel(k64_proc_convolve_setup_t* setup) {
//      uint32 i = 0;
//      MKernelWidth = setup->width;
//      MKernelHeight = setup->height;
//      for(uint32 y=0; y<setup->height; y++) {
//        for(uint32 x=0; x<setup->width; x++) {
//          MKernel[y][x] = setup->coeffs[i++];
//        }
//      }
//      MKernelFactor = setup->factor;
//      MKernelBias = setup->bias;
//    }
//
//    //----------
//
//    NO_INLINE
//    void process(K64_Bitmap* ABitmap, K64_Bitmap* ASource) {
//      uint32 w = ABitmap->getWidth();
//      uint32 h = ABitmap->getHeight();
//      //apply the filter
//      for(uint32 y=0; y<h; y++) {
//        uint32* dst_ptr = (uint32*)ABitmap->getLinePtr(y);
//        for(uint32 x=0; x<w; x++) {
//          float red = 0.0, green = 0.0, blue = 0.0;
//          //multiply every value of the filter with corresponding image pixel
//          for(uint32 filterY=0; filterY<MKernelHeight; filterY++) {
//            for(uint32 filterX=0; filterX<MKernelWidth; filterX++) {
//              uint32 imageX = (x - (MKernelWidth / 2)  + filterX + w) % w;
//              uint32 imageY = (y - (MKernelHeight / 2) + filterY + h) % h;
//              uint32 color = ASource->getPixel(imageX,imageY);
//              red   += ((color & 0xff0000) >> 16) * MKernel[filterY][filterX];
//              green += ((color & 0xff00)   >> 8 ) * MKernel[filterY][filterX];
//              blue  += ((color & 0xff)          ) * MKernel[filterY][filterX];
//            }
//          }
//          //truncate values smaller than zero and larger than 255
//
//          uint8 r = min(max(int(MKernelFactor * red   + MKernelBias), 0), 255);
//          uint8 g = min(max(int(MKernelFactor * green + MKernelBias), 0), 255);
//          uint8 b = min(max(int(MKernelFactor * blue  + MKernelBias), 0), 255);
//
//          /*
//          // If you want to take the absolute value of values smaller than zero
//          // instead of truncating it, use this code instead:
//          //take absolute value and truncate to 255
//          result[y * w + x].r = min(abs(int(factor * red + bias)), 255);
//          result[y * w + x].g = min(abs(int(factor * green + bias)), 255);
//          result[y * w + x].b = min(abs(int(factor * blue + bias)), 255);
//          */
//
//          uint32 color = bgra(r,g,b);
//
//          *dst_ptr++ = color;
//
//        }
//      }
//    }
//
//};
//
//
//#undef MAX_KERNEL_SIZE

//------------------------------

///*
//
//int main(int argc, char *argv[])
//{
//  //load the image into the buffer
//  unsigned long w = 0, h = 0;
//  std::vector<ColorRGB> image;
//  loadImage(image, w, h, "pics/photo3.png");
//  std::vector<ColorRGB> result(image.size());
//
//  //set up the screen
//  screen(w, h, 0, "Filters");
//
//  ColorRGB color; //the color for the pixels
//
//  //apply the filter
//  for(int x = 0; x < w; x++)
//  for(int y = 0; y < h; y++)
//  {
//    double red = 0.0, green = 0.0, blue = 0.0;
//
//    //multiply every value of the filter with corresponding image pixel
//    for(int filterY = 0; filterY < filterHeight; filterY++)
//    for(int filterX = 0; filterX < filterWidth; filterX++)
//    {
//      int imageX = (x - filterWidth / 2 + filterX + w) % w;
//      int imageY = (y - filterHeight / 2 + filterY + h) % h;
//      red += image[imageY * w + imageX].r * filter[filterY][filterX];
//      green += image[imageY * w + imageX].g * filter[filterY][filterX];
//      blue += image[imageY * w + imageX].b * filter[filterY][filterX];
//    }
//
//    //truncate values smaller than zero and larger than 255
//    result[y * w + x].r = min(max(int(factor * red + bias), 0), 255);
//    result[y * w + x].g = min(max(int(factor * green + bias), 0), 255);
//    result[y * w + x].b = min(max(int(factor * blue + bias), 0), 255);
//  }
//
//  //draw the result buffer to the screen
//  for(int y = 0; y < h; y++)
//  for(int x = 0; x < w; x++)
//  {
//    pset(x, y, result[y * w + x]);
//  }
//
//  //redraw & sleep
//  redraw();
//  sleep();
//}
//
//*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//float k64_std_convolve_noop_kernel[]= {
//  0, 0, 0,
//  0, 1, 0,
//  0, 0, 0
//};
//
//k64_proc_convolve_setup_t k64_std_convolve_noop = {
//  3, 3,                         // size
//  k64_std_convolve_noop_kernel, // kernel
//  1.0f,                         // factor
//  0.0f                          // bias
//};
//
////----------
//
//float k64_std_convolve_blur3_kernel[]= {
//  0.0, 0.2, 0.0,
//  0.2, 0.2, 0.2,
//  0.0, 0.2, 0.0
//};
//
//k64_proc_convolve_setup_t k64_std_convolve_blur3 = {
//  3, 3,                           // size
//  k64_std_convolve_blur3_kernel,  // kernel
//  1.0f,                           // factor
//  0.0f                            // bias
//};
//
////----------
//
//
//float k64_std_convolve_blur5_kernel[]= {
//  0, 0, 1, 0, 0,
//  0, 1, 1, 1, 0,
//  1, 1, 1, 1, 1,
//  0, 1, 1, 1, 0,
//  0, 0, 1, 0, 0
//};
//
//k64_proc_convolve_setup_t k64_std_convolve_blur5 = {
//  5, 5,                           // size
//  k64_std_convolve_blur5_kernel,  // kernel
//  1.0f / 13.0f,                   // factor
//  0.0f                            // bias
//};
//
////----------
//
//
//float k64_std_convolve_find_edges_kernel[]= {
//  -1, -1, -1,
//  -1,  8, -1,
//  -1, -1, -1
//};
//
//k64_proc_convolve_setup_t k64_std_convolve_find_edges = {
//  3, 3,                               // size
//  k64_std_convolve_find_edges_kernel, // kernel
//  1.0f,                               // factor
//  0.0f                                // bias
//};
//
////----------
//
//
//float k64_std_convolve_find_horizontal_edges_kernel[]= {
//  0,  0, -1,  0,  0,
//  0,  0, -1,  0,  0,
//  0,  0,  2,  0,  0,
//  0,  0,  0,  0,  0,
//  0,  0,  0,  0,  0
//};
//
//k64_proc_convolve_setup_t k64_std_convolve_find_horizontal_hedges = {
//  5, 5,                           // size
//  k64_std_convolve_find_horizontal_edges_kernel,      // kernel
//  1.0f,                           // factor
//  0.0f                            // bias
//};
//
////----------
//
//
//float k64_std_convolve_find_vertical_edges_kernel[]= {
//  0,  0, -1,  0,  0,
//  0,  0, -1,  0,  0,
//  0,  0,  4,  0,  0,
//  0,  0, -1,  0,  0,
//  0,  0, -1,  0,  0
//};
//
//k64_proc_convolve_setup_t k64_std_convolve_find_vertical_edges = {
//  5, 5,                               // size
//  k64_std_convolve_find_vertical_edges_kernel, // kernel
//  1.0f,                               // factor
//  0.0f                                // bias
//};
//
////----------
//
//
//float k64_std_convolve_find_diagonal_edges_kernel[]= {
//  -1,  0,  0,  0,  0,
//   0, -2,  0,  0,  0,
//   0,  0,  6,  0,  0,
//   0,  0,  0, -2,  0,
//   0,  0,  0,  0, -1
//};
//
//k64_proc_convolve_setup_t k64_std_convolve_find_diagonal_edges = {
//  5, 5,                               // size
//  k64_std_convolve_find_diagonal_edges_kernel, // kernel
//  1.0f,                               // factor
//  0.0f                                // bias
//};
//
////----------
//
//
//float k64_std_convolve_motion_blur_kernel[]= {
//  1, 0, 0, 0, 0, 0, 0, 0, 0,
//  0, 1, 0, 0, 0, 0, 0, 0, 0,
//  0, 0, 1, 0, 0, 0, 0, 0, 0,
//  0, 0, 0, 1, 0, 0, 0, 0, 0,
//  0, 0, 0, 0, 1, 0, 0, 0, 0,
//  0, 0, 0, 0, 0, 1, 0, 0, 0,
//  0, 0, 0, 0, 0, 0, 1, 0, 0,
//  0, 0, 0, 0, 0, 0, 0, 1, 0,
//  0, 0, 0, 0, 0, 0, 0, 0, 1,
//};
//
//k64_proc_convolve_setup_t k64_std_convolve_motion_blur = {
//  9, 9,                               // size
//  k64_std_convolve_motion_blur_kernel, // kernel
//  1.0f / 9.0f,                        // factor
//  0.0f                                // bias
//};
//
////----------
//
//
//float k64_std_convolve_sharpen_kernel[]= {
//  -1, -1, -1,
//  -1,  9, -1,
//  -1, -1, -1
//};
//
//k64_proc_convolve_setup_t k64_std_convolve_sharpen = {
//  3, 3,                           // size
//  k64_std_convolve_sharpen_kernel,      // kernel
//  1.0f,                           // factor
//  0.0f                            // bias
//};
//
////----------
//
//
//float k64_std_convolve_subtle_sharpen_kernel[]= {
//  -1, -1, -1, -1, -1,
//  -1,  2,  2,  2, -1,
//  -1,  2,  8,  2, -1,
//  -1,  2,  2,  2, -1,
//  -1, -1, -1, -1, -1
//};
//
//k64_proc_convolve_setup_t k64_std_convolve_subtle_sharpen = {
//  5, 5,                                   // size
//  k64_std_convolve_subtle_sharpen_kernel,  // kernel
//  1.0f,                                   // factor
//  0.0f                                    // bias
//};
//
////----------
//
//
//float k64_std_convolve_excessive_edges_kernel[]= {
//  1,  1,  1,
//  1, -7,  1,
//  1,  1,  1
//};
//
//k64_proc_convolve_setup_t k64_std_convolve_excessive_edges = {
//  3, 3,                                   // size
//  k64_std_convolve_excessive_edges_kernel, // kernel
//  1.0f,                                   // factor
//  0.0f                                    // bias
//};
//
////----------
//
//
//float k64_std_convolve_emboss_kernel[]= {
//  -1, -1,  0,
//  -1,  0,  1,
//   0,  1,  1
//};
//
//k64_proc_convolve_setup_t k64_std_convolve_emboss = {
//  3, 3,                           // size
//  k64_std_convolve_emboss_kernel, // kernel
//  1.0f,                           // factor
//  128.0f                          // bias
//};
//
////----------
//
//
//float k64_std_convolve_exaggerated_emboss_kernel[]= {
//  -1, -1, -1, -1,  0,
//  -1, -1, -1,  0,  1,
//  -1, -1,  0,  1,  1,
//  -1,  0,  1,  1,  1,
//   0,  1,  1,  1,  1};
//
//k64_proc_convolve_setup_t k64_std_convolve_exaggerated_emboss = {
//  5, 5,                           // size
//  k64_std_convolve_exaggerated_emboss_kernel,      // kernel
//  1.0f,                           // factor
//  128.0f                            // bias
//};


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

