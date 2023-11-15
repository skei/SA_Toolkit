#ifndef mip_decimator_filter_included
#define mip_decimator_filter_included
//----------------------------------------------------------------------

// Hiqh quality /2 decimators
// http://musicdsp.org/showArchiveComment.php?ArchiveID=231

// https://github.com/syanli/qmul-vamp-dsp/blob/master/dsp/dsp/rateconversion/Decimator.cpp

/*

  These are /2 decimators,
  Just instanciate one of them and use the Calc method to obtain one sample
  while inputing two. There is 5,7 and 9 tap versions.
  They are extracted/adapted from a tutorial code by Thierry Rochebois. The
  optimal coefficients are excerpts of Traitement numérique du signal, 5eme
  edition, M Bellanger, Masson pp. 339-346.

  //Filtres décimateurs
  // T.Rochebois
  // Based on
  //Traitement numérique du signal, 5eme edition, M Bellanger, Masson pp. 339-346

*/

//----------------------------------------------------------------------

class MIP_Decimator5Filter {
  private:
    float R1,R2,R3,R4,R5;
    float h0, h1, h3, h5;
  public:
    MIP_Decimator5Filter() {
      h0 = 346/692.0;
      h1 = 208/692.0;
      h3 = -44/692.0;
      h5 = 9/692.0;
      R1 = 0;
      R2 = 0;
      R3 = 0;
      R4 = 0;
      R5 = 0;
    }
    float process(float x0, float x1) {
      float h5x0 = h5 * x0;
      float h3x0 = h3 * x0;
      float h1x0 = h1 * x0;
      float R6   = R5 + h5x0;
      R5 = R4 + h3x0;
      R4 = R3 + h1x0;
      R3 = R2 + h1x0 + h0 * x1;
      R2 = R1 + h3x0;
      R1 = h5x0;
      return R6;
    }
};

//----------------------------------------------------------------------
// https://forum.cockos.com/showpost.php?p=1701930&postcount=44
//----------------------------------------------------------------------

// tap 11 -96 dB 21kHz at 88.2*2   ov8 -> ov4

/*
function dec2_96_11(x0 x1)
instance(R1 R2 R3 R4 R5 R6)
local(h5x0 h3x0 h1x0) (
  h5x0 = 0.007016005734703981 * x0;
  h3x0 = -0.05204656558631499 * x0;
  h1x0 = 0.2950377565605184 * x0;
  R6 = R5+h5x0;
  R5 = R4+h3x0;
  R4 = R3+h1x0;
  R3 = R2+h1x0+0.5000488550884885*x1;
  R2 = R1+h3x0;
  R1 = h5x0;
  R6;
);
*/

//----------------------------------------------------------------------

class MIP_Decimator7Filter {
  private:
    float R1,R2,R3,R4,R5,R6,R7;
    float h0,h1,h3,h5,h7;
  public:
    MIP_Decimator7Filter() {
      h0 = 802/1604.0;
      h1 = 490/1604.0;
      h3 = -116/1604.0;
      h5 = 33/1604.0;
      h7 = -6/1604.0;
      R1 = 0;
      R2 = 0;
      R3 = 0;
      R4 = 0;
      R5 = 0;
      R6 = 0;
      R7 = 0;
    }
    float process(float x0, float x1) {
      float h7x0 = h7 * x0;
      float h5x0 = h5 * x0;
      float h3x0 = h3 * x0;
      float h1x0 = h1 * x0;
      float R8   = R7 + h7x0;
      R7 = R6 + h5x0;
      R6 = R5 + h3x0;
      R5 = R4 + h1x0;
      R4 = R3 + h1x0 + h0 * x1;
      R3 = R2 + h3x0;
      R2 = R1 + h5x0;
      R1 = h7x0;
      return R8;
    }
};

//----------------------------------------------------------------------

class MIP_Decimator9Filter {
  private:
    float R1,R2,R3,R4,R5,R6,R7,R8,R9;
    float h0,h1,h3,h5,h7,h9;
  public:
    MIP_Decimator9Filter() {
      h0 = 8192/16384.0;
      h1 = 5042/16384.0;
      h3 = -1277/16384.0;
      h5 = 429/16384.0;
      h7 = -116/16384.0;
      h9 = 18/16384.0;
      R1 = 0;
      R2 = 0;
      R3 = 0;
      R4 = 0;
      R5 = 0;
      R6 = 0;
      R7 = 0;
      R8 = 0;
      R9 = 0;
    }
    float process(float x0, float x1) {
      float h9x0 = h9 * x0;
      float h7x0 = h7 * x0;
      float h5x0 = h5 * x0;
      float h3x0 = h3 * x0;
      float h1x0 = h1 * x0;
      float R10  = R9 + h9x0;
      R9 = R8 + h7x0;
      R8 = R7 + h5x0;
      R7 = R6 + h3x0;
      R6 = R5 + h1x0;
      R5 = R4 + h1x0 + h0 * x1;
      R4 = R3 + h3x0;
      R3 = R2 + h5x0;
      R2 = R1 + h7x0;
      R1 = h9x0;
      return R10;
    }
};

//----------------------------------------------------------------------
// https://forum.cockos.com/showpost.php?p=1701930&postcount=44
//----------------------------------------------------------------------

// tap 19 -96dB  19kHz at 88.2kHz  ov4 -> ov2

/*
function dec2_96_19(x0 x1)
instance(R1 R2 R3 R4 R5 R6 R7 R8 R9 R10)
local(h9x0 h7x0 h5x0 h3x0 h1x0)
(
  h9x0 =  0.0008092627249032843 * x0;
  h7x0 = -0.005912439782581228 * x0;
  h5x0 =  0.023933434769768926 * x0;
  h3x0 = -0.07551735475276798 * x0;
  h1x0 =  0.30669288913406306 * x0;
  R10 = R9+h9x0;
  R9  = R8+h7x0;
  R8  = R7+h5x0;
  R7  = R6+h3x0;
  R6  = R5+h1x0;
  R5  = R4+h1x0+0.5000155859774268*x1;
  R4  = R3+h3x0;
  R3  = R2+h5x0;
  R2  = R1+h7x0;
  R1  = h9x0;
  R10;
);
*/

//----------------------------------------------------------------------
// https://github.com/2DaT/Obxd/blob/master/Source/Engine/Decimator.h
//----------------------------------------------------------------------

// T.Rochebois

class MIP_Decimator17Filter {

  private:

    float h0,h1,h3,h5,h7,h9,h11,h13,h15,h17;
    float R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,R12,R13,R14,R15,R16,R17;

  public:

    MIP_Decimator17Filter() {
      reset();
      //h17 = (5.18944944e-005);
      //h15 = (-0.000572688237);
      //h13 = (0.00207426259);
      //h11 = (-0.00543170841);
      //h9  = (0.0120250406);
      //h7  = (-0.0240881704);
      //h5  = (0.0463142134);
      //h3  = (-0.0947515890);
      //h1  = (0.314356238);
      //h0  = (0.5);
      //R1=R2=R3=R4=R5=R6=R7=R8=R9=R10=R11=R12=R13=R14=R15=R16=R17=0;
    }

    void reset() {
      h17 = (5.18944944e-005);
      h15 = (-0.000572688237);
      h13 = (0.00207426259);
      h11 = (-0.00543170841);
      h9  = (0.0120250406);
      h7  = (-0.0240881704);
      h5  = (0.0463142134);
      h3  = (-0.0947515890);
      h1  = (0.314356238);
      h0  = (0.5);
      R1=R2=R3=R4=R5=R6=R7=R8=R9=R10=R11=R12=R13=R14=R15=R16=R17=0;
    }

    float process(float x0,float x1) {
      float h17x0 = h17 * x0;
      float h15x0 = h15 * x0;
      float h13x0 = h13 * x0;
      float h11x0 = h11 * x0;
      float h9x0  = h9 * x0;
      float h7x0  = h7 * x0;
      float h5x0  = h5 * x0;
      float h3x0  = h3 * x0;
      float h1x0  = h1 * x0;
      float R18   = R17 + h17x0;
      R17 = R16 + h15x0;
      R16 = R15 + h13x0;
      R15 = R14 + h11x0;
      R14 = R13 + h9x0;
      R13 = R12 + h7x0;
      R12 = R11 + h5x0;
      R11 = R10 + h3x0;
      R10 = R9  + h1x0;
      R9  = R8  + h1x0 + h0 * x1;
      R8  = R7  + h3x0;
      R7  = R6  + h5x0;
      R6  = R5  + h7x0;
      R5  = R4  + h9x0;
      R4  = R3  + h11x0;
      R3  = R2  + h13x0;
      R2  = R1  + h15x0;
      R1  = h17x0;
      return R18;
    }

};

//----------------------------------------------------------------------
// https://forum.cockos.com/showpost.php?p=1701930&postcount=44
//----------------------------------------------------------------------

// -96dB  17.5kHz at 44.1kHz   ov2 -> srate

/*
  // 16 x 30 +
  // scilab code:
  hn=eqfir(59,[0 .2;.3 0.5],[1 0],[1 1]);
  [hm,fr]=frmag(hn,512);
  plot2d(fr*88200,20*log10(hm),rect=[0,-110,22050,0.5],style=[2]);
  plot2d(44100-fr*88200,20*log10(hm),rect=[0,-110,22050,0.5],style=[5]);
  xgrid(1,1,7);
*/

/*
function dec2_96_59(x0 x1)
instance(R1 R2 R3 R4 R5 R6 R7 R8 R9 R10 R11 R12 R13 R14 R15 R16 R17 R18 R19 R20 R21 R22 R23 R24 R25 R26 R27 R28 R29 R30)
local(h1x0 h3x0 h5x0 h7x0 h9x0 h11x0 h13x0 h15x0 h17x0 h19x0 h21x0 h23x0 h25x0 h27x0 h29x0)
(
  h1x0  = x0*0.3167967114236291;     h3x0  = x0*-0.10164126581474016;
  h5x0  = x0*0.056472878314649574;   h7x0  = x0*-0.03590356425550024;
  h9x0  = x0*0.023854182328325296;   h11x0 = x0*-0.015966909428751137;
  h13x0 = x0*0.010553528578562761;   h15x0 = x0*-0.006794829937625732;
  h17x0 = x0*0.004212042043030732;   h19x0 = x0*-0.002484841082005066;
  h21x0 = x0*0.0013753503068182293;  h23x0 = x0*-0.0007008749315804081;
  h25x0 = x0*0.00031898286827647583; h27x0 = x0*-0.00012279053509672562;
  h29x0 = x0*0.000035881628440564505;
  R30 = R29 + h29x0;
  R29 = R28 + h27x0;
  R28 = R27 + h25x0;
  R27 = R26 + h23x0;
  R26 = R25 + h21x0;
  R25 = R24 + h19x0;
  R24 = R23 + h17x0;
  R23 = R22 + h15x0;
  R22 = R21 + h13x0;
  R21 = R20 + h11x0;
  R20 = R19 +  h9x0;
  R19 = R18 +  h7x0;
  R18 = R17 +  h5x0;
  R17 = R16 +  h3x0;
  R16 = R15 +  h1x0;
  R15 = R14 + h1x0 + 0.50000063464071 * x1;
  R14 = R13 +  h3x0;
  R13 = R12 +  h5x0;
  R12 = R11 +  h7x0;
  R11 = R10 +  h9x0;
  R10 = R9  + h11x0;
  R9  = R8  + h13x0;
  R8  = R7  + h15x0;
  R7  = R6  + h17x0;
  R6  = R5  + h19x0;
  R5  = R4  + h21x0;
  R4  = R3  + h23x0;
  R3  = R2  + h25x0;
  R2  = R1  + h27x0;
  R1 = h29x0;
  R30;
);
*/

class MIP_Decimator59Filter {

  private:

    float h1,h3,h5,h7,h9,h11,h13,h15,h17,h19,h21,h23,h25,h27,h29;
    float R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,R12,R13,R14,R15,R16,R17,R18,R19,R20,R21,R22,R23,R24,R25,R26,R27,R28,R29;

  public:

    MIP_Decimator59Filter() {
      reset();
    }

    void reset() {
      h1  =  0.3167967114236291;
      h3  = -0.10164126581474016;
      h5  =  0.056472878314649574;
      h7  = -0.03590356425550024;
      h9  =  0.023854182328325296;
      h11 = -0.015966909428751137;
      h13 =  0.010553528578562761;
      h15 = -0.006794829937625732;
      h17 =  0.004212042043030732;
      h19 = -0.002484841082005066;
      h21 =  0.0013753503068182293;
      h23 = -0.0007008749315804081;
      h25 =  0.00031898286827647583;
      h27 = -0.00012279053509672562;
      R1=R2=R3=R4=R5=R6=R7=R8=R9=R10=R11=R12=R13=R14=R15=R16=R17=R18=R19=R20=R21=R22=R23=R24=R25=R26=R27=R28=R29=0;
    }

    float process(float x0,float x1) {
      float h1x0  = x0*h1;
      float h3x0  = x0*h3;
      float h5x0  = x0*h5;
      float h7x0  = x0*h7;
      float h9x0  = x0*h9;
      float h11x0 = x0*h11;
      float h13x0 = x0*h13;
      float h15x0 = x0*h15;
      float h17x0 = x0*h17;
      float h19x0 = x0*h19;
      float h21x0 = x0*h21;
      float h23x0 = x0*h23;
      float h25x0 = x0*h25;
      float h27x0 = x0*h27;
      float h29x0 = x0*h29;
      float R30 = R29 + h29x0;
      R29 = R28 + h27x0;
      R28 = R27 + h25x0;
      R27 = R26 + h23x0;
      R26 = R25 + h21x0;
      R25 = R24 + h19x0;
      R24 = R23 + h17x0;
      R23 = R22 + h15x0;
      R22 = R21 + h13x0;
      R21 = R20 + h11x0;
      R20 = R19 +  h9x0;
      R19 = R18 +  h7x0;
      R18 = R17 +  h5x0;
      R17 = R16 +  h3x0;
      R16 = R15 +  h1x0;
      R15 = R14 + h1x0 + 0.50000063464071 * x1;
      R14 = R13 +  h3x0;
      R13 = R12 +  h5x0;
      R12 = R11 +  h7x0;
      R11 = R10 +  h9x0;
      R10 = R9  + h11x0;
      R9  = R8  + h13x0;
      R8  = R7  + h15x0;
      R7  = R6  + h17x0;
      R6  = R5  + h19x0;
      R5  = R4  + h21x0;
      R4  = R3  + h23x0;
      R3  = R2  + h25x0;
      R2  = R1  + h27x0;
      R1  = h29x0;
      return R30;
    }

};


//----------------------------------------------------------------------
#endif
