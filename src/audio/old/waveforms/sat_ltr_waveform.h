
class MIP_LtrWaveform {
};

//
//{$include mip.inc}
//unit mip_osc_ltr;
//
////----------------------------------------------------------------------
//interface
////----------------------------------------------------------------------
//
////function KOsc_Saw_Sinc(t,dt:Single) : Single; inline;
//
//procedure process(in1,in2,in3,in4,in5:Single; var out1,out2:Single);
//
////----------------------------------------------------------------------
//implementation
////----------------------------------------------------------------------
//
//uses
//  mip_math;
//
////function KOsc_Saw_Sinc(t,dt:Single) : Single; inline;
////begin
////  result := 0;
////end;
//
//// in1 0 = sine, 1 = saw or triangle else (2) = pulse
//// in2 p
//// in3 slope (padd)
//// in4 pm
//// in5 sync (ramp with FM and sync)
//
//var
//  tanh_buf : array[0..16383] of Single;
//  xlog_buf : array[0..16383] of Single;
//
//var //class-variables..
//  h1 : Single = 0;
//  h2 : Single = 0;
//  h3 : Single = 0;
//  h4 : Single = 0;
//
////----------------------------------------------------------------------
//
//function KWrap(AValue:Single; AMin:Single=-1; AMax:Single=1) : Single;
//begin
//  AValue := frac(AValue+1);
//end;
//
////----------------------------------------------------------------------
//
//procedure process(in1:LongInt; in2,in3,in4,in5:Single; var out1,out2:Single);
//var
//  dx,dx1,w1,w2,lx,w3 : Single;
//  ramp,ramp2 : single;
//begin
//  dx  := 0;
//  dx1 := 0;
//  w1  := 0;
//  w2  := 0;
//  lx  := 0;
//  w3  := 0;
//
//  //ramp = (in5) ? 0 : wrap(in2 + in4 + h1, -1, 1); // ramp with FM and sync
//
//  if in5 = 1 then ramp := 0
//  else ramp := KWrap( in2+in4+h1, -1, 1);
//  ramp2 := (ramp * 0.5) + 0.5; // utility ramp in 0~1 range
//
//  case in1 of
//    0: // sine
//      begin
//        out1 := triangle(ramp2,in3);
//        dx1 := delta(out1);
//        out1 = cycle(wrap(out1 + .25, 0, 1), index="phase");
//        out2 = out1;
//        if (in3 > 0 & in3 <=.99) then
//        begin
//          if (dx1 > 0 & in3 < .5) out1 *= (in3 *2);
//          else if (dx1 >.5) out1 *= (2 - in3 *2);
//          else -1;
//        end;
//      end;
//    1: // saw/tri
//      begin
//        dx = delta(ramp2) ;
//        if (in3 >.99) then // falling saw
//        begin
//          if (dx < 0) out1 = 0; // LTR
//          else out1 = ramp;
//        end
//        else if (in3 < .01) then // rising saw
//        begin
//          if (dx < 0) out1 = 0; // LTR
//          else out1 = neg(ramp);
//        end
//        else // variable slope
//        begin
//          if (dx < 0) out1 = -1; // LTR on fall-to-rise, LTR on rise-to-fall
//          else if (ramp2 ? in3 & h2 < in3) out1 = 1;
//          else out1 = triangle(ramp2, in3) * 2 -1; // Not in transition region
//        end;
//        out2 = triangle(ramp2, in5) *2 - 1;
//      end;
//    2: // pulse
//      begin
//        w0 = in3 * .9 + .05;
//        w1 = 1 - abs(in3 -.5) * .9;
//        if (ramp2 < .5) w2 = ramp2 * w1 * 2;
//        else w2 = w1 + (ramp2 -.5) * 2 * (1-w1);
//        out1 = cycle(w2, index="phase");
//        out1 *= peek (xlog_buf, in6, interp="none");
//        out1 = tanh(out1);
//        out2 = (ramp2 > w0)? 1 : -1;
//      end;
//  end;
//
//  h1 := ramp; // FM phase accumulator
//  h2 := ramp2; // previous ramp value for ELTRs
//  h3 := in4; // previous FM value for delta correction
//  out1 := dcblock(out1);
//end;
//
////----------------------------------------------------------------------
//end.
//
//// http://www.yofiel.com/software/cycling-74-patches/antialiased-oscillators
//
//(*
//
///* *************************************************************/
//// LTR Antialiased Oscillators
///* *************************************************************/
//Buffer tanh_buf("tanh_buf"); Buffer xlog_buf("xlog_buf");
//History h1 (0); History h2 (0); History h3 (0); History h4 (0);
//dx=0; dx1 = 0; w1 = 0; w2 = 0; lx = 0;w3=0;
//ramp = (in5)? 0 : wrap(in2 + in4 + h1, -1, 1); // ramp with FM and sync
//ramp2 = ramp *.5 + .5; // utility ramp in 0~1 range
//if (in1 == 0) { // SINE
//out1 = triangle(ramp2,in3);
//dx1 = delta(out1);
//out1 = cycle(wrap(out1 + .25, 0, 1), index="phase");
//out2 = out1;
//if (in3 > 0 & in3 <=.99){
//if (dx1 > 0 & in3 < .5) out1 *= (in3 *2);
//else if (dx1 >.5) out1 *= (2 - in3 *2);
//else -1;
//}
//} else if (in1==1) ( // SAW OR TRIANGLE
//dx = delta(ramp2) ;
//if (in3 >.99) { // falling saw
//if (dx < 0) out1 = 0; // LTR
//else out1 = ramp;
//} else if (in3 < .01) { // rising saw
//if (dx < 0) out1 = 0; // LTR
//else out1 = neg(ramp);
//} else { // variable slope
//if (dx < 0) out1 = -1; // LTR on fall-to-rise
//// LTR on rise-to-fall
//else if (ramp2 ? in3 & h2 < in3) out1 = 1;
//else // Not in transition region
//out1 = triangle(ramp2, in3) * 2 -1;
//}
//out2 = triangle(ramp2, in5) *2 - 1;
//} else { // PULSE
//w0 = in3 * .9 + .05;
//w1 = 1 - abs(in3 -.5) * .9;
//if (ramp2 < .5) w2 = ramp2 * w1 * 2;
//else w2 = w1 + (ramp2 -.5) * 2 * (1-w1);
//out1 = cycle(w2, index="phase");
//out1 *= peek (xlog_buf, in6, interp="none");
//out1 = tanh(out1);
//out2 = (ramp2 > w0)? 1 : -1;
//}
//h1 = ramp; // FM phase accumulator
//h2 = ramp2; // previous ramp value for ELTRs
//h3 = in4; // previous FM value for delta correction
//out1 = dcblock(out1);
//
//*)
//
