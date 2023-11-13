#!/usr/bin/tcc -run

#include <stdio.h>
//#include <stdlib.h>
//#include <dlfcn.h>

#define NUM 16

int main (int argc, char **argv) {
  FILE *in, *out;
  unsigned char a[NUM];
  unsigned int i,c,size = 0;
  fprintf(stderr,"Hello World!!\n");
  if (argc!=4) {
    fprintf(stderr,"usage: bin2h inputfile output.h name[]\n");
    return 1;
  }
  if (!(in=fopen(argv[1],"rb"))) {
    fprintf(stderr,"error: can't open %s.\n",argv[1]);
    return 1;
  }
  if (!(out=fopen(argv[2],"wt"))) {
    fclose(in);
    fprintf(stderr,"error: can't create %s.\n",argv[2]);
    return 1;
  }
  if (argv[3] == NULL) {
    printf("error: name[] required\n");
    return 1;
  }
  fseek (in, 0, SEEK_END);
  size = ftell(in);
  fseek (in, 0, SEEK_SET);
  fprintf (out,"const unsigned %s_size = %u;\n", argv[3], size);
  fprintf (out,"const unsigned char %s[] = \n{\n", argv[3]);
  while ((c=fread(a,1,NUM,in))!=0) {
  fprintf(out,"  ");
  if (c == NUM) {
    for (i=0; i<c; i++) fprintf(out,"0x%02x, ",a[i]);
      fprintf(out,"\n");
    } else {
      for (i=0; i<c-1; i++) fprintf(out,"0x%02x, ",a[i]);
      fprintf(out,"0x%02x\n",a[c-1]);
    }
  }
  fprintf(out,"};\n");
  fprintf(out,"// '%s' : %i\n",argv[1],size);
  fclose (out);
  fclose (in);
  return 0;
}

