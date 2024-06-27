
class MIP_EnvelopeFollower2 {
};

/*
  http://www.musicdsp.org/showArchiveComment.php?ArchiveID=97

  Envelope detector
  References : Posted by Bram
  Notes:
  Basicaly a one-pole LP filter with different coefficients for attack and
  release fed by the abs() of the signal. If you don't need different attack
  and decay settings, just use in->abs()->LP
  Code :

  //attack and release in milliseconds
  float ga = (float) exp(-1/(SampleRate*attack));
  float gr = (float) exp(-1/(SampleRate*release));
  float envelope=0;
  for(...)
  {
    //get your data into 'input'
    EnvIn = abs(input);
    if(envelope < EnvIn)
    {
      envelope *= ga;
      envelope += (1-ga)*EnvIn;
    }
    else
    {
      envelope *= gr;
      envelope += (1-gr)*EnvIn;
    }
    //envelope now contains.........the envelope ;)
  }

Comments:

arguru:
  Nice, just a typo: //attack and release is entered in SECONDS actually in
  this code ;)

antiprosynthesis:
  // Slightly faster version of the envelope follower using one multiply form.
  // attTime and relTime is in seconds
  float ga = exp(-1.0f/(sampleRate*attTime));
  float gr = exp(-1.0f/(sampleRate*relTime));
  float envOut = 0.0f;
  for( ... )
  {
    // get your data into 'input'
    envIn = fabs(input);
    if ( envOut < envIn )
      envOut = envIn + ga * (envOut - envIn);
    else
      envOut = envIn + gr * (envOut - envIn);
    // envOut now contains the envelope
  }

bob:
  Should use "fabsf" really.  :)
*/
