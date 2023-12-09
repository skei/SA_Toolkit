#ifndef mip_moog_filter_included
#define mip_moog_filter_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------

class MIP_MoogFilter {
};

//----------------------------------------------------------------------
#endif

//{$include mip.inc}
//unit mip_filter_moog;
//
//{
//  http://musicdsp.org/showArchiveComment.php?ArchiveID=253
//
//  Notes :
//  hacked from the exemple of user script in FL Edison
//
//  Code :
//  TLP24DB = class
//  constructor create;
//  procedure process(inp,Frq,Res:single;SR:integer);
//  public outlp:single;
//  end;
//  ----------------------------------------
//  implementation
//
//  constructor TLP24DB.create;
//  begin
//  end;
//
//  // the result is in outlp
//  // 1/ call MyTLP24DB.Process
//  // 2/then get the result from outlp.
//  // this filter have a fantastic sound w/a very special res
//  // _kd is the denormal killer value.
//
//}
//
////----------------------------------------------------------------------
//interface
////----------------------------------------------------------------------
//
//type
//
//  KFilter_Moog = class
//  private
//    FSamplerate : single;
//    f,p,k,t,t2 : single;
//    r : single;
//    y1, y2, y3, y4 : single;
//    oldx, oldy1, oldy2, oldy3: Single;
//  public
//    constructor create;
//    procedure   setSamplerate(ARate:Single);
//    procedure   setFreq(Freq:Single; SR:Single{Integer});
//    procedure   setFreq(Freq:Single);
//    procedure   setRes(Res:Single);
//    function    process(inp:Single) : Single;
//    function    process(inp:Single; Frq:Single; Res:Single; SR:Single{Integer}) : Single;
//  end;
//
//const
//  _kd = 1.0e-24; // ??
//
////----------------------------------------------------------------------
//implementation
////----------------------------------------------------------------------
//
//uses
//  mip_const;
//
////----------
//
//constructor KFilter_Moog.create;
//begin
//  inherited;
//  y1:=0;
//  y2:=0;
//  y3:=0;
//  y4:=0;
//  oldx:=0;
//  oldy1:=0;
//  oldy2:=0;
//  oldy3:=0;
//end;
//
////----------
//
//procedure KFilter_Moog.setSamplerate(ARate:Single);
//begin
//  FSamplerate := ARate;
//end;
//
////----------
//
//procedure KFilter_Moog.setFreq(Freq:Single; SR:Single{Integer});
//begin
//  f := (Freq+Freq) / SR;
//  p := f * (1.8-0.8*f);
//  k := p + p - 1.0;
//  t := (1.0-p) * 1.386249;
//  t2 := 12.0 + t * t;
//end;
//
////----------
//
//procedure KFilter_Moog.setFreq(Freq:Single);
//begin
//  setFreq(Freq,FSamplerate);
//end;
//
////----------
//
//procedure KFilter_Moog.setRes(Res:Single);
//begin
//  r := Res*(t2+6.0*t)/(t2-6.0*t);
//end;
//
////----------
//
//function KFilter_Moog.process(inp:Single) : Single;
//var
//  x : single;
//begin
//  x := inp - r*y4;
//  y1 := x*p + oldx*p - k*y1;
//  y2 := y1*p+oldy1*p - k*y2;
//  y3 := y2*p+oldy2*p - k*y3;
//  y4 := y3*p+oldy3*p - k*y4;
//  y4 := y4 - ((y4*y4*y4)/6.0);
//  oldx := x;
//  oldy1 := y1+_kd;
//  oldy2 := y2+_kd;;
//  oldy3 := y3+_kd;;
//  result := y4;
//end;
//
////----------
//
//function KFilter_Moog.process(inp:Single; Frq:Single; Res:Single; SR:Single{Integer}) : Single;
//var
//  x : single;
//begin
//  f := (Frq+Frq) / SR;
//  p := f*(1.8-0.8*f);
//  k := p+p-1.0;
//  t := (1.0-p)*1.386249;
//  t2 := 12.0+t*t;
//  r := res*(t2+6.0*t)/(t2-6.0*t);
//  x := inp - r*y4;
//  y1 := x*p + oldx*p - k*y1;
//  y2 := y1*p+oldy1*p - k*y2;
//  y3 := y2*p+oldy2*p - k*y3;
//  y4 := y3*p+oldy3*p - k*y4;
//  y4 := y4 - ((y4*y4*y4)/6.0);
//  oldx := x;
//  oldy1 := y1+_kd;
//  oldy2 := y2+_kd;;
//  oldy3 := y3+_kd;;
//  result := y4;
//end;
//
////----------------------------------------------------------------------
//end.
