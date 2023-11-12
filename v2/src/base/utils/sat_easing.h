#ifndef sat_easing_included
#define sat_easing_included
//----------------------------------------------------------------------

// http://www.freebasic.net/forum/viewtopic.php?f=7&t=14962

//----------------------------------------------------------------------

/*
  t - current time in frames or seconds (time)
  b - start value (begin)
  c - change needed in value (change)
  d - Expected easing duration in frames or seconds (duration)
  
  t and d can be frames or seconds/milliseconds

  t - current time
  b - start value = 0
  c - end value   = 1
  d - total time  = 1
*/

//----------------------------------------------------------------------

//Const As Single Pi = Atn(1) * 4
//Const As Single HalfPi = Atn(1) * 2
//Const As Single TwoPi = Atn(1) * 8

//----------------------------------------------------------------------

/*
Function pow(a As Single, b As Single) As Single
    Return a ^ b
End Function
*/


/*
#ifdef FastMath

#undef sqr
#undef sin
#undef cos
#undef asin
#undef pow

Function Sqr(x As Single) As Single
    Asm
        mov eax, [x]
        Shr eax, 1
        add eax, 532365312
        mov [x], eax
    End Asm
    Return x
End Function


Function Sin(x As Single) As Single
    If x < -3.14159265f Then
        Do
            x += 6.28318531f
        Loop Until x >= -3.14159265f
    Elseif x > 3.14159265f Then
        Do
            x -= 6.28318531f
        Loop Until x <= 3.14159265f
    End If

    If x < 0.0f Then Return 1.27323954f * x + 0.405284735f * x * x Else Return 1.27323954f * x - 0.405284735f * x * x
End Function



Function Cos(x As Single) As Single
    x += 1.57079632f

    If x < -3.14159265f Then
        Do
            x += 6.28318531f
        Loop Until x >= -3.14159265f
    Elseif x > 3.14159265f Then
        Do
            x -= 6.28318531f
        Loop Until x <= 3.14159265f
    End If

    If x < 0.0f Then Return 1.27323954f * x + 0.405284735f * x * x Else Return 1.27323954f * x - 0.405284735f * x * x
End Function



Function Pow(Byval a As Double, Byval b As Double) As Double
    Dim As Integer tmp = (*(1 + Cptr(Integer Ptr, @a)))
    Dim As Integer tmp2 = Cint(b * (tmp - 1072632447) + 1072632447)
    Dim As Double p = 0.0

    *(1 + Cptr(Integer Ptr, @p)) = tmp2
    Return p
End Function



Function Asin(fValue As Single) As Single
    Dim As Single fRoot = (1.0f-fValue)
    fRoot = Sqr(fRoot)
    Dim As Single fResult = -0.0187293f
    fResult *= fValue
    fResult += 0.0742610f
    fResult *= fValue
    fResult -= 0.2121144f
    fResult *= fValue
    fResult += 1.5707288f
    fResult = HalfPi - fRoot*fResult
    Return fResult
End Function
#endif
*/

//----------------------------------------------------------------------

// simple linear tweening - no easing, no acceleration

//NO_INLINE
float SAT_LinearTween(float t, float b, float c, float d) {
  return c * t / d + b;
}

// quadratic easing in - accelerating from zero velocity

//NO_INLINE
float SAT_EaseInQuad(float t, float b, float c, float d) {
  t /= d;
  return c*t*t + b;
}



// quadratic easing out - decelerating to zero velocity

//NO_INLINE
float SAT_EaseOutQuad(float t, float b, float c, float d) {
  t /= d;
  return -c * t*(t-2) + b;
}



// quadratic easing in/out - acceleration until halfway, then deceleration

//NO_INLINE
float SAT_EaseInOutQuad(float t, float b, float c, float d) {
  t /= (d/2);
  if (t < 1) return c/2*t*t + b;
  t -= 1;
  return -c/2 * (t*(t-2) - 1) + b;
}



// quadratic easing out/in

//NO_INLINE
float SAT_EaseOutInQuad(float t, float b, float c, float d) {
  if (t < (d/2)) return SAT_EaseOutQuad(t*2, b, c/2, d);
  return SAT_EaseInQuad((t*2)-d, b+c/2, c/2, d);
}



// cubic easing in - accelerating from zero velocity

//NO_INLINE
float SAT_EaseInCubic(float t, float b, float c, float d) {
  t /= d;
  return c*t*t*t + b;
}



// cubic easing out - decelerating to zero velocity

//NO_INLINE
float SAT_EaseOutCubic(float t, float b, float c, float d) {
  t /= d;
  t -= 1;
  return c*(t*t*t + 1) + b;
}



// cubic easing in/out - acceleration until halfway, then deceleration

//NO_INLINE
float SAT_EaseInOutCubic(float t, float b, float c, float d) {
  t /= d/2;
  if (t < 1) return c/2*t*t*t + b;
  t -= 2;
  return c/2*(t*t*t + 2) + b;
}



// cubic easing out/in

//NO_INLINE
float SAT_EaseOutInCubic(float t, float b, float c, float d) {
  if (t < d/2) return SAT_EaseOutCubic (t*2, b, c/2, d);
  return SAT_EaseInCubic((t*2)-d, b+c/2, c/2, d);
}



// quartic easing in - accelerating from zero velocity

//NO_INLINE
float SAT_EaseInQuart(float t, float b, float c, float d) {
  t /= d;
  return c*t*t*t*t + b;
}



// quartic easing out - decelerating to zero velocity

//NO_INLINE
float SAT_EaseOutQuart(float t, float b, float c, float d) {
  t /= d;
  t -= 1;
  return -c * (t*t*t*t - 1) + b;
}



// quartic easing in/out - acceleration until halfway, then deceleration

//NO_INLINE
float SAT_EaseInOutQuart(float t, float b, float c, float d) {
  t /= d/2;
  if (t < 1) return c/2*t*t*t*t + b;
  t -= 2;
  return -c/2 * (t*t*t*t - 2) + b;
}



// quartic easing out/in

//NO_INLINE
float SAT_EaseOutInQuart(float t, float b, float c, float d) {
  if (t < d/2) return SAT_EaseOutQuart (t*2, b, c/2, d);
  return SAT_EaseInQuart((t*2)-d, b+c/2, c/2, d);
}



// quintic easing in - accelerating from zero velocity

//NO_INLINE
float SAT_EaseInQuint(float t, float b, float c, float d) {
  t /= d;
  return c*t*t*t*t*t + b;
}



// quintic easing out - decelerating to zero velocity

//NO_INLINE
float SAT_EaseOutQuint(float t, float b, float c, float d) {
  t /= d;
  t -= 1;
  return c*(t*t*t*t*t + 1) + b;
}



// quintic easing in/out - acceleration until halfway, then deceleration

//NO_INLINE
float SAT_EaseInOutQuint(float t, float b, float c, float d) {
  t /= d/2;
  if (t < 1) return c/2*t*t*t*t*t + b;
  t -= 2;
  return c/2*(t*t*t*t*t + 2) + b;
}


// quintic easing out/in

//NO_INLINE
float SAT_EaseOutInQuint(float t, float b, float c, float d) {
  if (t < d/2) return SAT_EaseOutQuint (t*2, b, c/2, d);
  return SAT_EaseInQuint((t*2)-d, b+c/2, c/2, d);
}



// sinusoidal easing in - accelerating from zero velocity

//NO_INLINE
float SAT_EaseInSine(float t, float b, float c, float d) {
  return -c * cosf(t/d * SAT_PI05) + c + b;
}



// sinusoidal easing out - decelerating to zero velocity

//NO_INLINE
float SAT_EaseOutSine(float t, float b, float c, float d) {
  return c * sinf(t/d * SAT_PI05) + b;
}



// sinusoidal easing in/out - accelerating until halfway, then decelerating

//NO_INLINE
float SAT_EaseInOutSine(float t, float b, float c, float d) {
  return -c/2 * (cosf(SAT_PI*t/d) - 1) + b;
}



// sinusoidal easing out/in

//NO_INLINE
float SAT_EaseOutInSine(float t, float b, float c, float d) {
  if (t < d/2) return SAT_EaseOutSine (t*2, b, c/2, d);
  return SAT_EaseInSine((t*2)-d, b+c/2, c/2, d);
}



// exponential easing in - accelerating from zero velocity

//NO_INLINE
float SAT_EaseInExpo(float t, float b, float c, float d) {
  return c * pow( 2, 10 * (t/d - 1) ) + b;
}



// exponential easing out - decelerating to zero velocity

//NO_INLINE
float SAT_EaseOutExpo(float t, float b, float c, float d) {
  return c * ( -pow( 2, -10 * t/d ) + 1 ) + b;
}



// exponential easing in/out - accelerating until halfway, then decelerating

//NO_INLINE
float SAT_EaseInOutExpo(float t, float b, float c, float d) {
  t /= d/2;
  if (t < 1) return c/2 * pow( 2, 10 * (t - 1) ) + b;
  t -= 1;
  return c/2 * ( -pow( 2, -10 * t) + 2 ) + b;
}



// exponential easing out/in

//NO_INLINE
float SAT_EaseOutInExpo(float t, float b, float c, float d) {
  if (t < d/2) return SAT_EaseOutExpo (t*2, b, c/2, d);
  return SAT_EaseInExpo((t*2)-d, b+c/2, c/2, d);
}



// circular easing in - accelerating from zero velocity

//NO_INLINE
float SAT_EaseInCirc(float t, float b, float c, float d) {
  t /= d;
  return -c * (sqrtf(1 - t*t) - 1) + b;   // Sqr
}



// circular easing out - decelerating to zero velocity

//NO_INLINE
float SAT_EaseOutCirc(float t, float b, float c, float d) {
  t /= d;
  t -= 1;
  return c * sqrtf(1 - t*t) + b; // Sqr
}



// circular easing in/out - acceleration until halfway, then deceleration

//NO_INLINE
float SAT_EaseInOutCirc(float t, float b, float c, float d) {
  t /= d/2;
  if (t < 1) return -c/2 * (sqrtf(1 - t*t) - 1) + b;
  t -= 2;
  return c/2 * (sqrtf(1 - t*t) + 1) + b;
}



// circular easing out/in

//NO_INLINE
float SAT_EaseOutInCirc(float t, float b, float c, float d) {
  if (t < d/2) return SAT_EaseOutCirc (t*2, b, c/2, d);
  return SAT_EaseInCirc((t*2)-d, b+c/2, c/2, d);
}



// elastic easing in

//NO_INLINE
float SAT_EaseInElastic(float t, float b, float c, float d) {
  float s = 1.70158f;
  float p = 0;
  float a = c;
  if (t == 0) return b;
  t /= d;
  if (t == 1) return b + c;
  p = d * 0.3f;
  if (a < fabsf(c)) {
    a = c;
    s = p / 4;
  }
  else {
    s = p / SAT_PI2 * asinf(c / a);
  }
  t -= 1;
  return -(a * powf(2,10*t) * sinf( (t*d-s)*SAT_PI2/p )) + b;
}

// elastic easing out


//NO_INLINE
float SAT_EaseOutElastic(float t, float b, float c, float d) {
  float s = 1.70158f;
  float p = 0;
  float a = c;

  if (t == 0) return b;
  t /= d;
  if (t == 1) return b + c;
  p = d * 0.3f;

  if (a < fabsf(c)) {
    a = c;
    s = p / 4;
  }
  else {
    s = p / SAT_PI2 * asinf(c / a);
  }
  return a * powf(2,-10*t) * sinf( (t*d-s)*SAT_PI2/p ) + c + b;
}



// elastic easing in/out

//NO_INLINE
float SAT_EaseInOutElastic(float t, float b, float c, float d) {
  float s = 1.70158f;
  float p = 0;
  float a = c;

  if (t == 0) return b;
  t /= d / 2;
  if (t == 2) return b + c;
  p = d * (0.3 * 1.5);
  if (a < fabsf(c)) {
    a = c;
    s = p / 4;
  }
  else {
    s = p / SAT_PI2 * asinf(c / a);
  }
  if (t < 1) {
    t -= 1;
    return -.5*(a*powf(2,10*t) * sinf( (t*d-s)*SAT_PI2/p )) + b;
  }
  t -= 1;
  return a*powf(2,-10*t) * sinf( (t*d-s)*SAT_PI2/p )*.5 + c + b;
}



// elastic easing out/in

//NO_INLINE
float SAT_EaseOutInElastic(float t, float b, float c, float d) {
  if (t < d/2) return SAT_EaseOutElastic (t*2, b, c/2, d);
  return SAT_EaseInElastic((t*2)-d, b+c/2, c/2, d);
}



// back easing in

//NO_INLINE
float SAT_EaseInBack(float t, float b, float c, float d) {
  float s = 1.70158f;
  t /= d;
  return c*t*t*((s+1)*t - s) + b;
}



// back easing out

//NO_INLINE
float SAT_EaseOutBack(float t, float b, float c, float d) {
  float s = 1.70158f;
  t = t/d-1;
  return c*(t*t*((s+1)*t + s) + 1) + b;
}



// back easing in/out

//NO_INLINE
float SAT_EaseInOutBack(float t, float b, float c, float d) {
  float s = 1.70158f;
  t /= d/2;
  s *= 1.525;
  if (t < 1) return c/2*(t*t*((s+1)*t - s)) + b;
  t -= 2;
  return c/2*(t*t*((s+1)*t + s) + 2) + b;
}



// back easing out/in

float SAT_EaseOutInBack(float t, float b, float c, float d) {
  if (t < d/2) return SAT_EaseOutBack (t*2, b, c/2, d);
  return SAT_EaseInBack((t*2)-d, b+c/2, c/2, d);
}



// bounce easing out

//NO_INLINE
float SAT_EaseOutBounce(float t, float b, float c, float d) {
  t /= d;
  if (t < 1 / 2.75)
    return c*(7.5625*t*t) + b;
  else if (t < 2/2.75) {
    t -= 1.5/2.75;
    return c*(7.5625*t*t + .75) + b;
  }
  else if (t < 2.5/2.75) {
    t -= 2.25/2.75;
    return c*(7.5625*t*t + .9375) + b;
  }
  else {
    t -= 2.625/2.75;
    return c*(7.5625*t*t + .984375) + b;
  }
}



// bounce easing in

//NO_INLINE
float SAT_EaseInBounce(float t, float b, float c, float d) {
  return c - SAT_EaseOutBounce(d - t, 0, c, d) + b;
}



// bounce easing in/out

//NO_INLINE
float SAT_EaseInOutBounce(float t, float b, float c, float d) {
  if (t < d/2) return SAT_EaseInBounce(t*2, 0, c, d) * .5 + b;
  return SAT_EaseOutBounce (t*2-d, 0, c, d) * .5 + c*.5 + b;
}



// bounce easing out/in

//NO_INLINE
float SAT_EaseOutInBounce(float t, float b, float c, float d) {
  if (t < d/2) return SAT_EaseOutBounce (t*2, b, c/2, d);
  return SAT_EaseInBounce((t*2)-d, b+c/2, c/2, d);
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// Mirroring of all easing floats

//NO_INLINE
float fixTime(float t, float d) {
  if (t < d/2) {
    t = t * 2;
  }
  else {
    t = t - d/2;
    t = d - t*2;
  }
  return t;
}

//----------

// #define SAT_LINEAR_TWEEN        1
// #define SAT_EASE_IN_QUAD        2
// #define SAT_EASE_OUT_QUAD       3
// #define SAT_EASE_INOUT_QUAD     4
// #define SAT_EASE_OUTIN_QUAD     5
// #define SAT_EASE_IN_CUBIC       6
// #define SAT_EASE_OUT_CUBIC      7
// #define SAT_EASE_INOUT_CUBIC    8
// #define SAT_EASE_OUTIN_CUBIC    9
// #define SAT_EASE_IN_QUART       10
// #define SAT_EASE_OUT_QUART      11
// #define SAT_EASE_INOUT_QUART    12
// #define SAT_EASE_OUTIN_QUART    13
// #define SAT_EASE_IN_QUINT       14
// #define SAT_EASE_OUT_QUINT      15
// #define SAT_EASE_INOUT_QUINT    16
// #define SAT_EASE_OUTIN_QUINT    17
// #define SAT_EASE_IN_SINE        18
// #define SAT_EASE_OUT_SINE       19
// #define SAT_EASE_INOUT_SINE     20
// #define SAT_EASE_OUTIN_SINE     21
// #define SAT_EASE_IN_EXPO        22
// #define SAT_EASE_OUT_EXPO       23
// #define SAT_EASE_INOUT_EXPO     24
// #define SAT_EASE_OUTIN_EXPO     25
// #define SAT_EASE_IN_CIRC        26
// #define SAT_EASE_OUT_CIRC       27
// #define SAT_EASE_INOUT_CIRC     28
// #define SAT_EASE_OUTIN_CIRC     29
// #define SAT_EASE_IN_ELASTIC     30
// #define SAT_EASE_OUT_ELASTIC    31
// #define SAT_EASE_INOUT_ELASTIC  32
// #define SAT_EASE_OUTIN_ELASTIC  33
// #define SAT_EASE_IN_BACK        34
// #define SAT_EASE_OUT_BACK       35
// #define SAT_EASE_INOUT_BACK     36
// #define SAT_EASE_OUTIN_BACK     37
// #define SAT_EASE_OUT_BOUNCE     38
// #define SAT_EASE_IN_BOUNCE      39
// #define SAT_EASE_INOUT_BOUNCE   40
// #define SAT_EASE_OUTIN_BOUNCE   41

float SAT_Easing(uint32_t typ, float t, float b, float c, float d, bool AMirror=false) {
  if (AMirror) t = fixTime(t,d);
  switch (typ) {
    case 1:   return SAT_LinearTween(t,b,c,d);
    case 2:   return SAT_EaseInQuad(t,b,c,d);
    case 3:   return SAT_EaseOutQuad(t,b,c,d);
    case 4:   return SAT_EaseInOutQuad(t,b,c,d);
    case 5:   return SAT_EaseOutInQuad(t,b,c,d);
    case 6:   return SAT_EaseInCubic(t,b,c,d);
    case 7:   return SAT_EaseOutCubic(t,b,c,d);
    case 8:   return SAT_EaseInOutCubic(t,b,c,d);
    case 9:   return SAT_EaseOutInCubic(t,b,c,d);
    case 10:  return SAT_EaseInQuart(t,b,c,d);
    case 11:  return SAT_EaseOutQuart(t,b,c,d);
    case 12:  return SAT_EaseInOutQuart(t,b,c,d);
    case 13:  return SAT_EaseOutInQuart(t,b,c,d);
    case 14:  return SAT_EaseInQuint(t,b,c,d);
    case 15:  return SAT_EaseOutQuint(t,b,c,d);
    case 16:  return SAT_EaseInOutQuint(t,b,c,d);
    case 17:  return SAT_EaseOutInQuint(t,b,c,d);
    case 18:  return SAT_EaseInSine(t,b,c,d);
    case 19:  return SAT_EaseOutSine(t,b,c,d);
    case 20:  return SAT_EaseInOutSine(t,b,c,d);
    case 21:  return SAT_EaseOutInSine(t,b,c,d);
    case 22:  return SAT_EaseInExpo(t,b,c,d);
    case 23:  return SAT_EaseOutExpo(t,b,c,d);
    case 24:  return SAT_EaseInOutExpo(t,b,c,d);
    case 25:  return SAT_EaseOutInExpo(t,b,c,d);
    case 26:  return SAT_EaseInCirc(t,b,c,d);
    case 27:  return SAT_EaseOutCirc(t,b,c,d);
    case 28:  return SAT_EaseInOutCirc(t,b,c,d);
    case 29:  return SAT_EaseOutInCirc(t,b,c,d);
    case 30:  return SAT_EaseInElastic(t,b,c,d);
    case 31:  return SAT_EaseOutElastic(t,b,c,d);
    case 32:  return SAT_EaseInOutElastic(t,b,c,d);
    case 33:  return SAT_EaseOutInElastic(t,b,c,d);
    case 34:  return SAT_EaseInBack(t,b,c,d);
    case 35:  return SAT_EaseOutBack(t,b,c,d);
    case 36:  return SAT_EaseInOutBack(t,b,c,d);
    case 37:  return SAT_EaseOutInBack(t,b,c,d);
    case 38:  return SAT_EaseOutBounce(t,b,c,d);
    case 39:  return SAT_EaseInBounce(t,b,c,d);
    case 40:  return SAT_EaseInOutBounce(t,b,c,d);
    case 41:  return SAT_EaseOutInBounce(t,b,c,d);
    default:  return 0.0;
  }
}

//----------------------------------------------------------------------

/*


//NO_INLINE
Function Easing(method As Integer, toMirror As Integer, t As Single, b As Single, c As Single, d As Single) As Single
    If toMirror = 1 Then t = fixTime(t, d)

    Select Case method
        Case 1: Return linearTween(t, b, c, d)
        Case 2: Return easeInQuad(t, b, c, d)
        Case 3: Return easeOutQuad(t, b, c, d)
        Case 4: Return easeInOutQuad(t, b, c, d)
        Case 5: Return easeOutInQuad(t, b, c, d)
        Case 6: Return easeInCubic(t, b, c, d)
        Case 7: Return easeOutCubic(t, b, c, d)
        Case 8: Return easeInOutCubic(t, b, c, d)
        Case 9: Return easeOutInCubic(t, b, c, d)
        Case 10: Return easeInQuart(t, b, c, d)
        Case 11: Return easeOutQuart(t, b, c, d)
        Case 12: Return easeInOutQuart(t, b, c, d)
        Case 13: Return easeOutInQuart(t, b, c, d)
        Case 14: Return easeInQuint(t, b, c, d)
        Case 15: Return easeOutQuint(t, b, c, d)
        Case 16: Return easeInOutQuint(t, b, c, d)
        Case 17: Return easeOutInQuint(t, b, c, d)
        Case 18: Return easeInSine(t, b, c, d)
        Case 19: Return easeOutSine(t, b, c, d)
        Case 20: Return easeInOutSine(t, b, c, d)
        Case 21: Return easeOutInSine(t, b, c, d)
        Case 22: Return easeInExpo(t, b, c, d)
        Case 23: Return easeOutExpo(t, b, c, d)
        Case 24: Return easeInOutExpo(t, b, c, d)
        Case 25: Return easeOutInExpo(t, b, c, d)
        Case 26: Return easeInCirc(t, b, c, d)
        Case 27: Return easeOutCirc(t, b, c, d)
        Case 28: Return easeInOutCirc(t, b, c, d)
        Case 29: Return easeOutInCirc(t, b, c, d)
        Case 30: Return easeInElastic(t, b, c, d)
        Case 31: Return easeOutElastic(t, b, c, d)
        Case 32: Return easeInOutElastic(t, b, c, d)
        Case 33: Return easeOutInElastic(t, b, c, d)
        Case 34: Return easeInBack(t, b, c, d)
        Case 35: Return easeOutBack(t, b, c, d)
        Case 36: Return easeInOutBack(t, b, c, d)
        Case 37: Return easeOutInBack(t, b, c, d)
        Case 38: Return easeInBounce(t, b, c, d)
        Case 39: Return easeOutBounce(t, b, c, d)
        Case 40: Return easeInOutBounce(t, b, c, d)
        Case 41: Return easeOutInBounce(t, b, c, d)
    End Select
End Function
*/

//----------------------------------------------------------------------
#endif
