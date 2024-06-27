#ifndef mip_lpc_included
#define mip_lpc_included
//----------------------------------------------------------------------

class MIP_Lpc {
};

#if 0

// http://www.musicdsp.org/showArchiveComment.php?ArchiveID=137

/*
  hmm... SetLength..
  change to static arrays? or pointers..
*/

/*
  find the P-order autocorrelation array, R, for the sequence x of
  length L and warping of lambda
*/

void MIP_AutoCorrelate(float* x, float* R, int32 P, float lambda, int32 l=-1) {
//var
//  dl,Rt     : TDoubleArray;
//  r1,r2,r1t : Double;
//  k,i       : Integer;

  // Initialization
  if (l==-1) l = Length(x);
  SetLength(dl,l);
  SetLength(Rt,l);
  R[0] := 0;
  Rt[0] := 0;
  r1 := 0;
  r2 := 0;
  r1t := 0;
  for k := 0 to l-1 {
    Rt[0] := Rt[0] + x[k] * x[k];
    dl[k] := r1 - lambda * (x[k]-r2);
    r1 := x[k];
    r2 := dl[k];
  }
  for i := 1 to P {
    Rt[i] := 0;
    r1 := 0;
    r2 := 0;
    for k := 0 to L-1 {
      Rt[i] := Rt[i] + dl[k] * x[k];
      r1t := dl[k];
      dl[k] := r1 - lambda * (r1t-r2);
      r1 := r1t;
      r2 := dl[k];
    }
  }
  for i := 1 to P do R[i] := Rt[i];
  setlength(Rt,0);
  setlength(dl,0);
}

//----------

/*
  Calculate the Levinson-Durbin recursion for the autocorrelation sequence
  R of length P+1 and return the autocorrelation coefficients a
  and reflection coefficients K
*/

void MIP_LevinsonRecursion(P:Integer; R,A,K:TSingleArray) {
var
  Am1       : TDoubleArray;
  i,j,s,m   : Integer;
  km,Em1,Em : Double;
  err       : Double;

  SetLength(Am1,62);
  if R[0] = 0.0 {
    for i := 1 to P {
      K[i] := 0.0;
      A[i] := 0.0;
    }
  }
  else {
    for j := 0 to P {
      A[0] := 0;
      Am1[0] := 0;
    }
    A[0] := 1;
    Am1[0] := 1;
    km := 0;
    Em1 := R[0];
    for m := 1 to P {
      err := 0.0;
      for j := 1 to m-1 do err := err + Am1[j] * R[m-j];
      km := ( R[m] - err ) / Em1;
      K[m-1] := -km;
      A[m] := km;
      for j := 1 to m-1 do A[j] := Am1[j] - km * Am1[m-j];
      Em := (1-km*km) * Em1;
      for s := 0 to P do Am1[s] := A[s];
      Em1 := Em;
    }
  }
}

#endif // 0

//----------------------------------------------------------------------
#endif
