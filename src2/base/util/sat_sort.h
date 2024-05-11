#ifndef sat_sort_included
#define sat_sort_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

void SAT_BubbleSort(int a[], int length);
void SAT_InsertSort(int* a, int length);
void SAT_RadixSort(long *source, long *dest, uint32_t N, int byte);

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

//void KBubbleSort (int a[], int length) {
void SAT_BubbleSort (int* a, int length) {
	int i, j, temp;
  for (i = 0; i < length; i++) {
    for (j = 0; j < length - 1; j++) {
      if (a[j + 1] < a[j]) {
        temp = a[j];
        a[j] = a[j + 1];
        a[j + 1] = temp;
      }
    }
  }
}

//----------

// nice for small-ish data sets.. ???

void SAT_InsertSort(int* a, int length) {
  int i, j, value;
  for(i = 1 ; i < length ; i++) {
    value = a[i];
    for (j = i - 1; j >= 0 && a[j] > value; j--)
      a[j + 1] = a[j];
    a[j + 1] = value;
  }
}

//----------

void SAT_RadixSort(long* source, long* dest, uint32_t N, int byte) {
  uint32_t i;
  long count[256];
  long index[256];
  memset(count, 0, sizeof(count));
  for (i=0; i<N; i++) count[ ((source[i]) >> (byte*8)) & 0xff ]++;
  index[0] = 0;
  for (i=1; i<256; i++) index[i] = index[i-1]+count[i-1];
  for (i=0; i<N; i++) dest[ index[ ((source[i])>>(byte*8))&0xff ]++ ] = source[i];
}

//----------------------------------------------------------------------
#endif
