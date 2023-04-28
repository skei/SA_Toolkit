#ifndef mip_convolution_included
#define mip_convolution_included
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

void MIP_ConvolutionAdd(float* in1, float* in2, float* ou1, uint32_t size) {
  for (uint32_t i=0; i<size; i++) {
    ou1[i] = in1[i] + in2[i];
  }
}

//----------

void MIP_ConvolutionSubFrom(float* in1, float* ou1, uint32_t size) {
  for (uint32_t i=0; i<size; i++) {
    ou1[i] = ou1[i] - in1[i];
  }
}

//----------

void MIP_ConvolutionAddTo(float* in1, float* ou1, uint32_t size) {
  float* po = ou1;
  float* pi1 = in1;
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

void MIP_Convolution(float* in1, float* in2, float* ou1, float* buffer, uint32_t size) {
  float* tmp1 = buffer;
  float* tmp2 = &(buffer[(size/2)]);
  // clear output.
  for (uint32_t i=0; i<(size*2); i++) ou1[i] = 0;
  // Break condition for recursion: 1x1 convolution is multiplication.
  if (size==1)  {
    ou1[0] = in1[0] * in2[0];
    return;
  }
  // first calculate (A1 + A2 * z^-n)*(B1 + B2 * z^-n)*z^n
  MIP_ConvolutionAdd(in1, &(in1[(size/2)]), tmp1, size/2);
  MIP_ConvolutionAdd(in2, &(in1[(size/2)]), tmp2, size/2);
  MIP_Convolution(tmp1, tmp2, &(ou1[(size/2)]), &(buffer[size]), size/2);
  // then add A1*B1 and substract A1*B1*z^n
  MIP_Convolution(in1, in2, tmp1, &(buffer[size]), size/2);
  MIP_ConvolutionAddTo(ou1, tmp1, size);
  MIP_ConvolutionSubFrom(&(ou1[(size/2)]), tmp1, size);
  // then add A2*B2 and substract A2*B2*z^-n
  MIP_Convolution(&(in1[(size/2)]), &(in2[(size/2)]), tmp1, &(buffer[size]), size/2);
  MIP_ConvolutionAddTo(&(ou1[size]), tmp1, size);
  MIP_ConvolutionSubFrom(&(ou1[size]), tmp1, size);
}

//----------------------------------------------------------------------
#endif
