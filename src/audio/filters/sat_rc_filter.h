#ifndef sat_rc_filter_included
#define sat_rc_filter_included
//----------------------------------------------------------------------

//#include <math.h> // expf
#include "base/util/sat_fast_math.h" // SAT_ExpF

//----------

template <class T>
class SAT_RcFilter {

  private:

    T MValue;
    T MTarget;
    T MWeight;
    T MSampleRate;

  public:

    void  setSampleRate(T ARate) { MSampleRate = ARate; }
    void  setValue(T AValue)     { MValue = AValue; }
    void  setTarget(T AValue)    { MTarget = AValue; }
    void  setWeight(T AValue)    { MWeight = AValue; }
    T     getValue(void)         { return MValue; }
    T     getTarget(void)        { return MTarget; }
    T     getWeight(void)         { return MWeight; }

  public:

    SAT_RcFilter() {
      MValue  = 0;
      MTarget = 0;  // 1
      MWeight = 0;  // 1
    }

    ~SAT_RcFilter() {
    }

    void setup(T AValue=0, T ATarget=0, T AWeight=0) {
      MValue  = AValue;
      MTarget = ATarget;
      MWeight = AWeight;
    }

    void setWeightInv(T w) {
      if (w>0) MWeight = 1.0f / w;
      //else w = 1.0f; // cppcheck
      else MWeight = 1.0f;
    }

    //void setSampleRate(T ARate) {
    //  MSampleRate = ARate;
    //}

    void setFrequency(T AFrequency/*, T ASampleRate*/) {
      //if (ASampleRate > 0)

        //MWeight = 1 - expf(-SAT_PI2 * AFrequency / MSampleRate );
        MWeight = 1 - SAT_ExpF(-SAT_PI2 * AFrequency / MSampleRate );

      //else
      //  MWeight = 0;
    }

    /*
      ATime is the time it takes the filter to decay to
      36.8% of its initial input or reach 63.2% of its final output.
    */

    void setTime(T ATime) {

      //if (ATime > 0) MWeight = 1 - expf(-1 / ATime);
      if (ATime > 0) MWeight = 1 - SAT_ExpF(-1 / ATime);
      else MWeight = 0;
    }

    T process(void) {
      MValue += (MTarget - MValue) * MWeight;
      return MValue;
    }

    T process(T AValue) {
      MTarget = AValue;
      return this->process();
    }

    T process_hi(T AValue) {
      MTarget = AValue;
      return AValue - this->process();
    }

};

//----------------------------------------------------------------------
#endif

//----------------------------------------------------------------------

/*
  http://www.kvraudio.com/forum/viewtopic.php?p=1489784&sid=88215ddaa307770244bedf534c013fb6#p1489784
  y[n] = y[n-1] + g*(x[n] - y[n-1])
  g = 1-exp(-2*pi*f/fs)
  f = 1/(2*pi*R*C)
*/

/*
  http://www.kvraudio.com/forum/viewtopic.php?p=1489786&sid=88215ddaa307770244bedf534c013fb6#p1489786

  delta = input - buffer
  buffer = buffer + scale * delta
  expr(n,r) = exp(ln(ln(r))-ln(ln(n)))
  nroot(n,r) = n^(1/r)
  pos(spd,smp) = (1-spd)^smp
  spd(pos,smp) = 1-nroot(pos,smp)
  smp(spd,pos) = expr(1-spd,pos)
  expr() = anti-power
  nroot() = nth root
  pos() = position arrived at speed in samples
  spd() = speed required to arrive at position in samples
  smp() = samples required to arrive at position with speed

  http://www.kvraudio.com/forum/viewtopic.php?p=1495892&sid=88215ddaa307770244bedf534c013fb6#p1495892

  when processing, you do this:

  {
    delta = input - buffer
    buffer = buffer + scale * delta
  }

  delta is the difference between the buffer and the input signal. the term
  delta is usually used to represent any difference between two things. that
  delta is scaled by some fractional amount, and then added to the buffer. if
  we were to start with the buffer containing 1.0, and the input 0.0, with a
  scale of 0.5, i will plot the delta and buffer values for you in pairs.

  -1.0, 1.0
  -0.5, 0.5
  -0.25, 0.25
  -0.125, 0.125
  -0.0625, 0.0625

  as you can see, every sample the buffer's stored value will get closer to the
  input value by the fractional amount of the scale parameter. because we
  always move by a fractional amount of the delta, the delta will get
  constantly smaller as we iterate, and infact will never fully reach the
  input.

  expr(n,r) = exp(ln(ln(r))-ln(ln(n)))

  this function, is an anti-power, so the following is true;

  a = n^p, expr(a,n) = p

  this allows to 'undo' a exponent if we know the original number operated on,
  we can get back the power raised to.

  nroot(n,r) = n^(1/r)

  the nth root function gives us the root of a number by using a power. (which
  infact uses the log and exp functions as in the anti-power function replacing
  the subtraction with an addition, but an anti-power isnt defined in the
  standard c math library since it is less frequency used.) it is the third
  function in the relationship of the function power. using the three functions
  of power, root, and apower, you can calculate many different functions. this
  is pretty basic algebra.

  the following is true;

  nroot(n, 2) = sqrt(n)

  pos(spd,smp) = (1-spd)^smp

  this is the basic function for calculating how the first function (the filter
  function) will operate. we know that every iteration the speed will decrease
  by multiplication with the fractional value scale. by generalizing the case
  we will use the function in, we can calculate the value that will be stored
  in the buffer based upon the scale (or speed) and the number of iterations.
  in order to calculate moving from a number other than 1.0 to 0.0, you simply
  scale the output, the behaviour of the function is some math term i cant be
  bothered to remember (linear? i think it is something else too.) it is
  independant of the parameters input or internal conditions, so it is easy to
  calculate in relative terms like this.

  spd(pos,smp) = 1-nroot(pos,smp)

  following on that function, we can develop a function to find what speed will
  be required to achieve a relative change in position over a certain number of
  iterations. we use the 2nd function of the power, the nth root, to calculate
  the second function of the 'filter' function shown above.

  smp(spd,pos) = expr(1-spd,pos)

  we then can calculate the third permutation, as how many iterations it will
  take to arrive at a relative position at a certain speed. this is the same,
  and uses the third relative of the power function to calculate the third
  relative of our 'filter' function.

  because of the relationships here and the predictablity of these functions,
  many uses are possible.

  one is calculating the position in any number of iterations, making real time
  unimportant, much like the functions of newtonian physics. though unlike
  newtonian physics which have more variables involved, we will not lose
  percision when calculating in larger slides of time. this means we can
  calculate (as implemented in xhip) the position of envelopes or other
  components using this 'filter' function without actually iterating the full
  function. it is only required to store the number of iterations elapsed
  between a parameter change or other interaction which will change the
  behaviour.

  another is also implemented in xhip, though it has not yet been released,
  adjustment of speeds to ensure a predictable linear relationship to time for
  the same components in certain situations.

  there are many more, though i've already said too much, what advantage can i
  hope to have if i tell everyone all the 'secrets' of basic highschool
  algebra? :O

  of course i make it clear by that statement why i'm explaining this.
  'developers' of bad synthesizer software dont even have the ability to
  comprehend what i've said even with such clear explainations, so i feel no
  fear.

  the number stored by 'delta' can be considered highpass, and the number
  stored by 'buffer' can be considered lowpass.

  more variables are involved, but a rc filter is calculated in exactly the
  same way. the rate of charge is equal to the delta of voltage on the sides of
  the capacitor divided by the input resistance.

  note that because we're working in iterations here, there is a one iteration
  delay in the phase value of the 'buffer' variable, and this also causes
  simmilar phase difference for the 'delta' value. the 'delta' value can for
  most perposes be considered immediate though.

  http://www.kvraudio.com/forum/viewtopic.php?p=1496921&sid=88215ddaa307770244bedf534c013fb6#p1496921

  yeah i know, i remember how much it was sucking in school. if only they would
  provide a single realistic example (instead of abstract) for realistic
  functions, leaving the abstract definitions to abstract functions! human
  memory works by assigning different levels of importance to different sences
  based upon some percived event that is taking place. if you get your arm
  chopped off somewhere, you'll remember everything about that event because
  it is considered important. obviously you want to avoid situations where you
  might end up getting the other arm wacked off :)

  if something seems unimportant to you, you simply will not remember it or
  even begin to try to understand it, this would obviously be a waste!

  i didnt honestly learn any of this stuff in school either, i've had to do a
  lot of work to gain even a basic understanding of these concepts even though
  i now recall nearly all of this type of thing being outlined in basic math
  classes. oh well.

  the cutoff frequency relates directly to the scale factor.

  scale = (2 * frequency) / rate
  frequency = (scale / 2) * rate

  yes, it really is that simple. the 'correct' definition of 'cutoff' here is
  the lowest frequency at which the phase shift crosses 45 degrees (although
  for all practical purposes this can be ignored.)

  this happens to be the -3db point for a single stage, and that is where the
  common misconception of "cutoff = -3db" comes from. infact, the correct
  definition for any multiple stage system with feedback is where the phase
  first crosses 180 degrees. this 180 degree point obviously is where
  oscillation will begin. complex systems may have many 180 degree crossings
  which is why the definition is "the lowest frequency at which.

  for a system with cascaded stages without feedback, the definition is a bit
  sketchy. some people use -3db, but such a definition really does not make
  much sence anymore.

  for synthesizers, generally you'll be using multiple stages with feedback, or
  a single stage without. this means the cutoff point is well defined so these
  things are easy to deal with.

  http://xhip.cjb.net/temp/4stage.png

  yes, pathetic, i did draw that in ms paint.. :/

  each one of the blocks at the top is a "lossy integrator" for lack of a
  better term, it is the 'filter' function i showed above. this is the typical
  implementation of a '24db lowpass filter' in all synthesizers. the problem
  with its implementation is that each stage adds one iteration of delay to the
  signal meaning by the time you get to the feedback scaling and subtract from
  the input, you have four iterations of delay. for the correct frequencies,
  phases, and scales to work it requires a zero delay. obviously a delay also
  exists in analog electronics but it is equiv. to using a very high sample
  rate (depends upon several factors).

  you can achieve the same results using other methods though, so not all is
  lost. most of the software out there uses this simple method though, and
  attempts to compensate (always aproximate, imperfect) the effects of the
  delay involved

  http://www.kvraudio.com/forum/viewtopic.php?p=4293217#p4293217

  time in samples to reach level = log(1.0 - level) / log(1.0 - coefficient)
  so say you're decaying from 1.0 to 0.0, you want to figure out how long it
  takes to get within 1/100th of that point:
  time in samples = log(0.01) / log(1.0 - coefficient)
  percentage = (1 - coefficient) ^ samples
  position = lerp(start, end, percentage)
  coefficient = 1 - percentage ^ (1 / samples)

  ----------

  http://www.kvraudio.com/forum/viewtopic.php?p=2936365#p2936365

  If you have an RC filter, the input is constant, and you need to process a certain amount of samples but don't need the intermediate sample values (for example, "analog-style" ADSR envelopes), there's a neat thing you can do to make it an O(1) operation

  #define THRESHOLD (0.001f) // distance to input value, increase for more accuracy
  // number of samples at sampleRate
  float timeConstant = speedInMillisecondsToReachThreshold * sampleRate * 0.001f;
  // adjusted rate
  timeConstant = timeConstant / (float) numSamplesToProcess;
  // coefficient, adjusted to the new sampleRate
  float coeff = powf(1.0f / THRESHOLD, -1.0f / timeConstant);
  // one "iteration"
  y0 = coeff * y0 + (1.0f - coeff) * input;

*/
