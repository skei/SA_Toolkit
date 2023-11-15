#ifndef mip_rbj_filter_included
#define mip_rbj_filter_included
//----------------------------------------------------------------------

// biquad

// http://www.musicdsp.org/archive.php?classid=3#225

/*
  RBJ Audio EQ Cookbook Filters
  A pascal conversion of arguru[AT]smartelectronix[DOT]com's
  c++ implementation.
  WARNING:This code is not FPU undernormalization safe.
  Filter Types
  0-LowPass
  1-HiPass
  2-BandPass CSG
  3-BandPass CZPG
  4-Notch
  5-AllPass
  6-Peaking
  7-LowShelf
  8-HiShelf
*/

//----------------------------------------------------------------------

/*

  http://www.kvraudio.com/forum/viewtopic.php?p=6371172#p6371172

  Is it possible to do a clean audio rate modulation of filter cutoff with a
  biquad like RBJ. I'm getting sporadic overflows/resonations of very high gain
  happening in and out as I move the cutoff manually while also modulating it
  at audio rate.
  The problem increases rapidly as I increase the modulation depth or speed of
  the modulator.
  ---
  No, it's not possible.
  Direct Form biquad filters as laid out in the RBJ document are not designed
  for parameter changes. While small changes over time are usually no problem,
  faster modulation commonly results in unwanted artifacts as you describe.

*/

//----------------------------------------------------------------------

class MIP_RbjFilter {

  private:

    float   b0a0,b1a0,b2a0,a1a0,a2a0;
    float   in1,in2,out1,out2;
    uint32_t  MFilterType;
    float   MFreq,MQ,MDBGain;
    bool    MQIsBandWidth;
    //fSampleRate : single;
    //fMaxBlockSize : integer;

  //public
  //  out1 : array of single;

  private:

    void setQ(float NewQ) {
      MQ = (1-NewQ) * 0.98;
    }

  public:

    MIP_RbjFilter() {
      MFilterType = 0;
      MFreq = 500;
      MQ = 0.3;
      MDBGain = 0;
      MQIsBandWidth = true;
      in1 = 0;
      in2 = 0;
      out1 = 0;
      out2 = 0;
    }

    //----------

    void calcFilterCoeffs(float ASampleRate, uint32_t AFilterType, float AFreq, float AQ, float ADBGain, bool AQIsBandWidth) {
      MFilterType = AFilterType;
      MFreq = AFreq;
      MQ = AQ;
      MDBGain = ADBGain;
      MQIsBandWidth = AQIsBandWidth;
      CalcFilterCoeffs(ASampleRate);
    }

    //----------

    void CalcFilterCoeffs(float ASampleRate) {
      float a0,a1,a2,b0,b1,b2;
      float alpha,beta,omega;
      float tsin,tcos;

      //peaking, LowShelf or HiShelf

      if (MFilterType>=6) {
        float A = pow(10.0,(MDBGain/40.0));
        omega = MIP_PI2*MFreq/ASampleRate;
        tsin = sin(omega);
        tcos = cos(omega);
        if (MQIsBandWidth) alpha = tsin*sinh(log2(2.0)/2.0*MQ*omega/tsin);
        else alpha = tsin/(2.0*MQ);
        beta = sqrt(A)/MQ;

        // peaking

        if (MFilterType==6) {
          b0 = 1.0+alpha*A;
          b1 = -2.0*tcos;
          b2 = 1.0-alpha*A;
          a0 = 1.0+alpha/A;
          a1 = -2.0*tcos;
          a2 = 1.0-alpha/A;
        }
        else

        // lowshelf

        if (MFilterType==7) {
          b0 = (A*((A+1.0)-(A-1.0)*tcos+beta*tsin));
          b1 = (2.0*A*((A-1.0)-(A+1.0)*tcos));
          b2 = (A*((A+1.0)-(A-1.0)*tcos-beta*tsin));
          a0 = ((A+1.0)+(A-1.0)*tcos+beta*tsin);
          a1 = (-2.0*((A-1.0)+(A+1.0)*tcos));
          a2 = ((A+1.0)+(A-1.0)*tcos-beta*tsin);
        }

        // hishelf

        if (MFilterType==8) {
          b0 = (A*((A+1.0)+(A-1.0)*tcos+beta*tsin));
          b1 = (-2.0*A*((A-1.0)+(A+1.0)*tcos));
          b2 = (A*((A+1.0)+(A-1.0)*tcos-beta*tsin));
          a0 = ((A+1.0)-(A-1.0)*tcos+beta*tsin);
          a1 = (2.0*((A-1.0)-(A+1.0)*tcos));
          a2 = ((A+1.0)-(A-1.0)*tcos-beta*tsin);
        }
      }

      //other filter types

      else {
        omega = MIP_PI2*MFreq/ASampleRate;
        tsin = sin(omega);
        tcos = cos(omega);
        if (MQIsBandWidth) alpha = tsin*sinh(log2(2)/2*MQ*omega/tsin);
        else alpha = tsin/(2*MQ);

        //lowpass

        if (MFilterType==0) {
          b0 = (1-tcos)/2;
          b1 = 1-tcos;
          b2 = (1-tcos)/2;
          a0 = 1+alpha;
          a1 = -2*tcos;
          a2 = 1-alpha;
        }
        else

        // hipass

        if (MFilterType==1) {
          b0 = (1+tcos)/2;
          b1 = -(1+tcos);
          b2 = (1+tcos)/2;
          a0 = 1+alpha;
          a1 = -2*tcos;
          a2 = 1-alpha;
        }
        else

        // bandpass CSG

        if (MFilterType==2) {
          b0 = tsin/2;
          b1 = 0;
          b2 = -tsin/2;
          a0 = 1+alpha;
          a1 = -1*tcos;
          a2 = 1-alpha;
        }
        else

        // bandpass CZPG

        if (MFilterType==3) {
          b0 = alpha;
          b1 = 0.0;
          b2 = -alpha;
          a0 = 1.0+alpha;
          a1 = -2.0*tcos;
          a2 = 1.0-alpha;
        }
        else

        // notch

        if (MFilterType==4) {
          b0 = 1.0;
          b1 = -2.0*tcos;
          b2 = 1.0;
          a0 = 1.0+alpha;
          a1 = -2.0*tcos;
          a2 = 1.0-alpha;
        }
        else

        // allpass

        if (MFilterType==5) {
          b0 = 1.0-alpha;
          b1 = -2.0*tcos;
          b2 = 1.0+alpha;
          a0 = 1.0+alpha;
          a1 = -2.0*tcos;
          a2 = 1.0-alpha;
        }

      }
      b0a0 = b0/a0;
      b1a0 = b1/a0;
      b2a0 = b2/a0;
      a1a0 = a1/a0;
      a2a0 = a2/a0;
    }

    //----------

    float process(float AInput) {
      float n =  b0a0*AInput + b1a0*in1 + b2a0*in2 - a1a0*out1 - a2a0*out2;
      in2 = in1;
      in1 = AInput;
      out2 = out1;
      out1 = n;
      return n;
    }

    //----------

    /*
    the process method is overloaded.
    use Process(input:single):single;
    for per sample processing
    use Process(Input:psingle;sampleframes:integer);
    for block processing. The input is a pointer to
    the start of an array of single which contains
    the audio data.
    i.e.
    RBJFilter.Process(@WaveData[0],256);
    }
    {procedure ZFilterRBJ.Process(Input:psingle;sampleframes:integer);
    var
     i:integer;
     LastOut:single;
    {
     for i = 0 to SampleFrames-1 do
     {
       // filter
       LastOut =  b0a0*(input^)+ b1a0*in1 + b2a0*in2 - a1a0*out1 - a2a0*out2;
       //LastOut = input^;
       // push in/out buffers
       in2 = in1;
       in1 = input^;
       out2 = out1;
       out1 = LastOut;
       Out1[i] = LastOut;
       inc(input);
     }
    }
    */

  public:

    uint32_t  filterType(void)    { return MFilterType; }
    float   freq(void)          { return MFreq; }
    float   q(void)             { return MQ; }
    float   dbGain(void)        { return MDBGain; }
    bool    qIsBandWidth(void)  { return MQIsBandWidth; }

    void    filterType(uint32_t AType)          { MFilterType = AType; }
    void    freq(float AFreq)                 { MFreq = AFreq; }
    void    q(float AQ)                       { MQ = AQ; }
    void    dbGain(float ADBGain)             { MDBGain = ADBGain; }
    void    qIsBandWidth(float AQisBandwidth) { MQIsBandWidth = AQisBandwidth; }

};

//----------------------------------------------------------------------
#endif

