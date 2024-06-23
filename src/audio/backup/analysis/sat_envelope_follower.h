
class MIP_EnvelopeFollower {
};

/*
  http://www.musicdsp.org/showArchiveComment.php?ArchiveID=136

  Envelope follower with different attack and release
  References : Posted by Bram
  Notes :
  xxxx_in_ms is xxxx in milliseconds ;-)

  init::

    attack_coef = exp(log(0.01)/( attack_in_ms * samplerate * 0.001));
    release_coef = exp(log(0.01)/( release_in_ms * samplerate * 0.001));
    envelope = 0.0;

  loop::

    tmp = fabs(in);
    if(tmp > envelope)
    envelope = attack_coef * (envelope - tmp) + tmp;
    else
    envelope = release_coef * (envelope - tmp) + tmp;

  Comments

  kampsax:
  the expressions of the form:
  xxxx_coef = exp(log(0.01)/( xxxx_in_ms * samplerate * 0.001));
  can be simplified a little bit to:
  xxxx_coef = pow(0.01, 1.0/( xxxx_in_ms * samplerate * 0.001));

*/
