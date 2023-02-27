#ifndef sat_strutils_included
#define sat_strutils_included
//----------------------------------------------------------------------

// TODO: sort by category

#include "base/sat.h"
#include "base/utils/sat_random.h"

#ifdef SAT_LINUX
  #include <iconv.h>
#endif

//----------------------------------------------------------------------
//
// private:
//
//----------------------------------------------------------------------

bool _sat_str_match(char* wildcards, char* str);
bool _sat_str_scan(char*& wildcards, char*& str);

//----------

bool _sat_str_match(char* wildcards, char* str) {
  bool Yes = 1;
  //iterate and delete '?' and '*' one by one
  while(*wildcards != '\0' && Yes && *str != '\0') {
    if (*wildcards == '?') str ++;
    else if (*wildcards == '*') {
      Yes = _sat_str_scan(wildcards, str);
      wildcards --;
    }
    else {
      Yes = (*wildcards == *str);
      str ++;
    }
    wildcards ++;
  }
  while (*wildcards == '*' && Yes)  wildcards ++;
  return Yes && *str == '\0' && *wildcards == '\0';
}

//----------

bool _sat_str_scan(char*& wildcards, char*& str) {
  // remove the '?' and '*'
  for (wildcards ++; *str != '\0' && (*wildcards == '?' || *wildcards == '*'); wildcards ++)
    if (*wildcards == '?') str ++;
  while ( *wildcards == '*') wildcards ++;
  // if str is empty and wildcards has more characters or,
  // wildcards is empty, return
  if (*str == '\0' && *wildcards != '\0') return false;
  if (*str == '\0' && *wildcards == '\0')	return true;
  // else search substring
  else {
    char* wds_copy = wildcards;
    char* str_copy = str;
    bool  Yes     = 1;
    do {
      if (!_sat_str_match(wildcards,str))	str_copy++;
      wildcards = wds_copy;
      str = str_copy;
      while ((*wildcards != *str) && (*str != '\0')) str ++;
      wds_copy = wildcards;
      str_copy = str;
    } while ((*str != '\0') ? !_sat_str_match(wildcards, str) : (Yes = false) != false);
    if (*str == '\0' && *wildcards == '\0')	return true;
    return Yes;
  }
}

//----------------------------------------------------------------------
//
// public:
//
//----------------------------------------------------------------------

// modifies length

/*
void SAT_AppendString(char* buffer, const char* str) {
  strcat(buffer,str);
}
*/

//----------------------------------------------------------------------
// public:
//----------------------------------------------------------------------

#ifdef SAT_LINUX
char* SAT_AsciiToUTF16(char* AUTF16, char* AAscii) {
  iconv_t cd = iconv_open("UTF-16","ASCII");
  if (cd == (iconv_t)-1) return nullptr; // iconv_open failed
  size_t  in_size   = strlen(AAscii);
  size_t  out_size  = 1024;
  char*   in_ptr    = AAscii;
  char*   out_ptr   = AUTF16;
  int nconv = iconv(cd,&in_ptr,&in_size,&out_ptr,&out_size);
  if (nconv != 0) return nullptr; // unable to perform conversion
  return AUTF16;
}
#else
char* SAT_AsciiToUTF16(char* AUTF16, char* AAscii) {
  return AUTF16;
}
#endif


// ptr to (reversed) hex
// prefix + ptr2hex (inverted) + random number
// (for creating unique strings (based on pointers)
// TODO: hash?

void SAT_CreateUniqueString(char* buffer, const char* APrefix, void* APtr) {
  const char hex_table[17] = "0123456789ABCDEF";  // +'\0' = 17
  const uint32_t hex_size  = sizeof(void*) * 2;
  const uint32_t rand_size = sizeof(int32_t) * 2;
  //_resize_buffer( strlen(APrefix) + hex_size + 1 + rand_size );
  char* buf = buffer;//temp;
  while (*APrefix != '\0') *buf++ = *APrefix++;   // memset + update ptrs
  *buf++ = ':';
  intptr_t iptr = (intptr_t)APtr;
  for (uint32_t i=0; i<hex_size; i++) {
    int j = iptr & 0x0f;
    iptr >>= 4;
    *buf++ = hex_table[j];
  }
  *buf++ = ':';
  int32_t rnd = SAT_RandomInt();
  for (uint32_t i=0; i<rand_size; i++) {
    int j = rnd & 0x0f;
    rnd >>= 4;
    *buf++ = hex_table[j];
  }
  *buf++ = '\0';
}

//----------

/*
  assume APos is valid
*/

char* SAT_DeleteChar(char* ABuffer, int32_t APos) {
  if (!ABuffer) return ABuffer;
  int32_t len = strlen(ABuffer);
  for (int32_t i=APos; i<len; i++) ABuffer[i] = ABuffer[i+1];
  return ABuffer;
}


//----------

// returns first occurence of 'str'
// or NULL if not found

char* SAT_FindString(char* buffer, char* str) {
  return strstr(buffer,str);
}

//----------

// /home/skei/test.so -> test.so
// returns ptr to first character after last /

const char* SAT_GetFilenameFromPath(const char* APath) {
  if (APath) {
    const char* slash     = strrchr(APath,'/');
    const char* backslash = strrchr(APath,'\\');
    if (slash) {
      return slash + 1;
    }
    else if (backslash) {
      return backslash + 1;
    }
  }
  return nullptr;
}

//----------


// returns 'exe', not '.exe'

const char* SAT_GetFileExt(/*const*/ char* APath) {
  if (APath) {
    char* dot;
    dot = strrchr(APath,'.');
    if (dot) return dot+1;
  }
  return nullptr;
}

//----------

// /home/skei/test -> /home/skei/

// modifies content of APath

//const char* SAT_GetPath(/*const*/ char* APath) {
//  if (APath) {
//    char *slash, *backslash;
//    slash     = SAT_Strrchr(APath,'/');
//    backslash = SAT_Strrchr(APath,'\\');//+1;
//    if (slash) *(slash+1) = '\0';
//    else if (backslash) *(backslash+1) = '\0'; // see above
//    return APath;
//  }
//  return nullptr;
//}

//----------

// /home/skei/test.so -> /home/skei/
// copies src to dst, inserts a 0 after the last /

char* SAT_GetPathOnly(char* ADst, const char* ASrc) {
  if (ADst && ASrc) {
    strcpy(ADst,ASrc);
    char* slash     = strrchr(ADst,'/');
    char* backslash = strrchr(ADst,'\\');
    if (slash) {
      slash[1] = 0;
      return ADst;
    }
    else if (backslash) {
      backslash[1] = 0;
      return ADst;
    }
  }
  return nullptr;
}

//----------

// https://stackoverflow.com/questions/7666509/hash-function-for-string

uint32_t SAT_HashString(const char* buffer) {
  char* ptr = (char*)buffer;
  unsigned long h = 5381;
  int c;
  while ((c = *ptr++)) {
    h = ((h << 5) + h) + c; // h * 33 + c
  }
  return h;
}

//----------

/*
  assume APos is valid
*/

char* SAT_InsertChar(char* ABuffer, int32_t APos, char AChar) {
//var
//  i,len : longint;
  if (!ABuffer) return ABuffer;
  int32_t len = strlen(ABuffer);
  for (int32_t i=(len+1); i>=(APos+1);i--) ABuffer[i] = ABuffer[i-1];
  ABuffer[APos] = AChar;
  return ABuffer;
}

//----------

bool SAT_IsEqual(const char* str1, const char* str2) {
  if (strcmp(str1,str2) == 0) return true;
  return false;
}

//----------

// A -> a

void SAT_LowerCase(char* buffer) {
  char* ptr = buffer;
  int c;
  while ((c = *ptr)) {
    if ((c>='A') && (c<='Z')) *ptr = c + 0x20;
    ptr++;
  }
}

//----------

void SAT_MakeValidSymbol(char* buffer) {
  for (uint32_t i=0; i<strlen(buffer); i++) {
    unsigned char c = buffer[i];
    if ((c<32) || (c>127)) buffer[i] = '_';
    else buffer[i] = SAT_CSYMBOLS[c];
  }
}

//----------

uint32_t SAT_ParseVersionString(const char* version) {
  int major = 0;
  int minor = 0;
  int revision = 0;
  sscanf(version,"%d.%d.%d",&major,&minor,&revision);
  major &= 0xff;
  minor &= 0xff;
  revision &= 0xffff;
  return (major << 24) + (minor << 16) + revision;
}

//----------

/*
void SAT_ReplaceFileExt(const char* ext) {
  SAT_StripFileExt();
  append(".");
  append(ext);
}
*/

//----------

// replace all occurences of str1 with str2
// assumes final size will be < 1024
// be careful about recursion! replace r with r2 for example.. :-/
// todo: wipe out src after replacing, so we don't re-find it?

void SAT_ReplaceString(char* buffer, const char* str1, const char* str2) {
  //_grow_buffer(1024);
  char temp[1024];
  uint32_t length = strlen(buffer);
  memcpy(temp,buffer,length+1);
  char *pos;
  int clen1 = strlen(str1);
  int clen2 = strlen(str2);
  while ((pos = strstr(temp,str1))) {
    //char* bufpos = pos - temp + MBuffer;
    memmove(pos + clen2, pos + clen1, strlen(pos) - clen1 + 1);
    memcpy(pos, str2, clen2);
    //memset(pos , 1, clen1);
  }
  //_resize_buffer( strlen(MBuffer) );
  strcpy(buffer,temp);
}

//----------

// replace all instances of c1 with c2

void SAT_ReplaceChars(char* buffer, const char c1, const char c2) {
  char *pos;
  while ((pos = strchr(buffer,c1))) {
    *pos = c2;
  }
}

//----------

void SAT_ReverseString(char* buffer) {
  int i;
  int len = strlen(buffer) - 1;
  int mid = (len % 2) ? (len / 2) : ((len + 1) / 2);
  for (i=0; i<=mid; ++i) {
    char buf = buffer[i];
    buffer[i] = buffer[len - i];
    buffer[len - i] = buf;
  }
}

//----------

// search the match string
// wildcards can be '?' and '*' combination
// return value : true (pattern matchs string), false (no match)

bool SAT_SearchWildcards(char* buffer, const char* wildcards) {
  return _sat_str_match((char*)wildcards, buffer);
}

//----------

//size_t strlcpy(char *dest, const char *src, size_t size) {
//  size_t ret = strlen(src);
//  if (size) {
//    size_t len = (ret >= size) ? size - 1 : ret;
//    memcpy(dest, src, len);
//    dest[len] = '\0';
//  }
//  return ret;
//}

// strlcpy/cat

void SAT_Strlcpy(char* dst, const char* src, uint32_t length) {
  if (strlen(src) >= length) {
    strncpy(dst,src,length-1);
    dst[length-1] = 0;
  }
  else {
    strcpy(dst,src);
  }
}

//----------

// file.txt -> file
// problem if there is a dot in the path
// todo: find last dot..
// modifies APath

char* SAT_StripFileExt(char* APath) {
  if (APath) {
    char* dot = strrchr(APath,'.');
    if (dot) {
      //int32_t len = dot - APath;
      *dot = '\0'; // return dot + 1;
      return APath;
    }
  }
  return nullptr;
}

//----------

// /home/skei/test -> test

char* SAT_StripPath(char* APath) {
  if (APath) {
    char *slash, *backslash;
    slash     = strrchr(APath,'/');
    backslash = strrchr(APath,'\\');//+1;
    if (slash) return slash + 1;
    else if (backslash) return backslash + 1; // see above
  }
  return nullptr;
}

//----------

// a -> A

void SAT_UpperCase(char* buffer) {
  char* ptr = buffer;
  int c;
  while ((c = *ptr)) {
    if ((c>='a') && (c<='z')) *ptr = c - 0x20;
    ptr++;
  }
}

//----------

#ifdef SAT_LINUX
char* SAT_UTF16ToAscii(char* AAscii, char* AUTF16) {
  iconv_t cd = iconv_open("ASCII","UTF-16");
  if (cd == (iconv_t)-1) return nullptr; // iconv_open failed
  size_t  in_size   = strlen(AAscii);
  size_t  out_size  = 1024;
  char*   in_ptr    = AUTF16;
  char*   out_ptr   = AAscii;
  int nconv = iconv(cd,&in_ptr,&in_size,&out_ptr,&out_size);
  if (nconv != 0) return nullptr; // unable to perform conversion
  return AAscii;
}
#else
char* SAT_UTF16ToAscii(char* AAscii, char* AUTF16) {
  return AUTF16;
}
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------


//----------------------------------------------------------------------
#endif

