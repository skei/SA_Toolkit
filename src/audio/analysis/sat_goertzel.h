#ifndef sat_goertzel_included
#define sat_goertzel_included
//----------------------------------------------------------------------

// http://www.musicdsp.org/showArchiveComment.php?ArchiveID=107

template <typename T>
T SAT_Goertzel(T* ABuffer, uint32_t ALength, T AFrequency, T ASamplerate) {
  T skn = 0;
  T skn1 = 0;
  T skn2 = 0;
  T temp1 = SAT_PI2 * AFrequency / ASamplerate;
  T temp2 = cosf(temp1);
  for (uint32_t i=0; i<ALength; i++) {
    skn2 = skn1;
    skn1 = skn;
    skn = 2*temp2*skn1 - skn2 + ABuffer[i];
  }
  return (skn - expf(-temp1)*skn1);
}

//----------

// Here's a variant on the theme that compensates for harmonics:
// http://www.musicdsp.org/showArchiveComment.php?ArchiveID=172

template <typename T>
T SAT_GoertzelHarmonics(T* ABuffer, uint32_t ALength, T AFrequency, T ASamplerate) {
  T qkn = 0;
  T qkn1 = 0;
  T wkn = SAT_PI2 * AFrequency / ASamplerate;
  T mk = 2 * cos(wkn);
  for (uint32_t i=0; i<ALength; i++) {
    T qkn2 = qkn1;
    qkn1 = qkn;
    qkn  = ABuffer[i] + mk*qkn1 - qkn2;
  }
  return sqrt(qkn*qkn + qkn1*qkn1 - mk*qkn*qkn1);
}

//----------------------------------------------------------------------
#endif

/*
  see also:
  http://stackoverflow.com/questions/11579367/implementation-of-goertzel-algorithm-in-c
*/
