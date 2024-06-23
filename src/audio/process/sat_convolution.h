#ifndef sat_convolution_included
#define sat_convolution_included
//----------------------------------------------------------------------

// BRUTE FORCE

// http://www.musicdsp.org/showArchiveComment.php?ArchiveID=65

/*
  "value" may be a suitable type like int or float.
  Parameter "size" is the size of the input signals and must be a power of 2.
  out and buffer must point to arrays of size 2*n.
*/

//----------------------------------------------------------------------
// 'private'
//----------------------------------------------------------------------

template<typename T>
void SAT_ConvolutionAdd(T* in1, T* in2, T* ou1, uint32_t size) {
  for (uint32_t i=0; i<size; i++) {
    ou1[i] = in1[i] + in2[i];
  }
}

//----------

template<typename T>
void SAT_ConvolutionSubFrom(T* in1, T* ou1, uint32_t size) {
  for (uint32_t i=0; i<size; i++) {
    ou1[i] = ou1[i] - in1[i];
  }
}

//----------

template<typename T>
void SAT_ConvolutionAddTo(T* in1, T* ou1, uint32_t size) {
  T* po = ou1;
  T* pi1 = in1;
  for (uint32_t i=0; i<size; i++) {
    ou1[i] = ou1[i] + in1[i];
    po++;
    pi1++;
  }
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  in1,in2 = input buffers
  ou1 = output..
*/

template<typename T>
void SAT_Convolution(T* in1, T* in2, T* ou1, T* buffer, uint32_t size) {
  T* tmp1 = buffer;
  T* tmp2 = &(buffer[(size/2)]);
  // clear output.
  for (uint32_t i=0; i<(size*2); i++) ou1[i] = 0;
  // Break condition for recursion: 1x1 convolution is multiplication.
  if (size==1)  {
    ou1[0] = in1[0] * in2[0];
    return;
  }
  // first calculate (A1 + A2 * z^-n)*(B1 + B2 * z^-n)*z^n
  SAT_ConvolutionAdd<T>(in1, &(in1[(size/2)]), tmp1, size/2);
  SAT_ConvolutionAdd<T>(in2, &(in1[(size/2)]), tmp2, size/2);
  SAT_Convolution(tmp1, tmp2, &(ou1[(size/2)]), &(buffer[size]), size/2);
  // then add A1*B1 and substract A1*B1*z^n
  SAT_Convolution<T>(in1, in2, tmp1, &(buffer[size]), size/2);
  SAT_ConvolutionAddTo<T>(ou1, tmp1, size);
  SAT_ConvolutionSubFrom<T>(&(ou1[(size/2)]), tmp1, size);
  // then add A2*B2 and substract A2*B2*z^-n
  SAT_Convolution<T>(&(in1[(size/2)]), &(in2[(size/2)]), tmp1, &(buffer[size]), size/2);
  SAT_ConvolutionAddTo<T>(&(ou1[size]), tmp1, size);
  SAT_ConvolutionSubFrom<T>(&(ou1[size]), tmp1, size);
}

//----------------------------------------------------------------------
#endif
