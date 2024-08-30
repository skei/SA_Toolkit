/*
  use thread pool..
  MThreadPool->request_exec(MNumTasks);
  for example:
  * split audio into many frequency bands, process these with thread pool, combine/post-process..
  * multi-resolution processing, analysis..
  * short, medium, long fft/wavelet/convolution in separate threads..
  * haar, goertzel
  * https://iquilezles.org/articles/sincos/
*/
