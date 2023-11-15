#ifndef sat_string_included
#define sat_string_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/types/sat_buffer.h"
#include "base/utils/sat_convert.h"
#include "base/utils/sat_strutils.h"

//#define SAT_String SAT_DynamicString
//#define SAT_String SAT_StaticString

//----------------------------------------------------------------------
//
// static string
//
//----------------------------------------------------------------------

template <int SIZE=256>
class SAT_StaticString
//: public SAT_StaticBuffer<char,SIZE> {
: public SAT_StaticBuffer<char,SIZE> {
};

//

//----------------------------------------------------------------------
//
// dynamic string
//
//----------------------------------------------------------------------

class SAT_DynamicString
: public SAT_DynamicBuffer<char> {

//------------------------------
public:
//------------------------------

  SAT_DynamicString() {
  }

  //----------

  SAT_DynamicString(const SAT_DynamicString& str) {
    uint32_t size = strlen(str);
    resizeBuffer(size+1);
    strcpy(MBuffer,str);
  }

  //----------

  SAT_DynamicString(const char* str) {
    uint32_t size = strlen(str);
    resizeBuffer(size+1);
    strcpy(MBuffer,str);
  }

  //----------

  virtual ~SAT_DynamicString() {
    //SAT_Print;
  }


//------------------------------
public:
//------------------------------

  uint32_t length() {
    return strlen(MBuffer);
  }

  //----------

  void setString(const char* str) {
    uint32_t size = strlen(str);
    resizeBuffer(size+1);
    strcpy(MBuffer,str);
  }

  //----------

  void appendString(const char* str) {
    uint32_t size = length() + strlen(str);
    resizeBuffer(size+1);
    strcat(MBuffer,str);
  }

//------------------------------
public:
//------------------------------

  SAT_DynamicString& operator = (char* str) {
    setString(str);
    return *this;
  }

  //----------

  SAT_DynamicString& operator = (const char* str) {
    setString(str);
    return *this;
  }

  //----------

  SAT_DynamicString& operator = (SAT_DynamicString& str) {
    setString(str.ptr());
    return *this;
  }

  //----------

  SAT_DynamicString& operator = (int AValue) {
    char temp[16];
    char* str = SAT_IntToString(temp,AValue);
    setString(str);
    return *this;
  }

  //----------

  SAT_DynamicString& operator = (float AValue) {
    char temp[16];
    char* str = SAT_FloatToString(temp,AValue);
    setString(str);
    return *this;
  }

  //------------------------------

  SAT_DynamicString& operator += (char* str) {
    appendString(str);
    return *this;
  }

  //----------

  SAT_DynamicString& operator += (const char* str) {
    appendString(str);
    return *this;
  }

  //----------

  SAT_DynamicString& operator += (SAT_DynamicString& str) {
    appendString(str.ptr());
    return *this;
  }

  //----------

  SAT_DynamicString& operator += (const int i) {
    char temp[16];
    char* str = SAT_IntToString(temp,i);
    appendString(str);
    return *this;
  }

  //----------

  SAT_DynamicString& operator += (const float f) {
    char temp[16];
    char* str = SAT_FloatToString(temp,f);
    appendString(str);
    return *this;
  }

  //------------------------------

  bool operator == (const char* str) {
    return (strcmp(MBuffer,str) == 0) ? true : false;
  }

  //----------

  bool operator == (SAT_DynamicString& str) {
    return (strcmp(MBuffer,str.ptr()) == 0) ? true : false;
  }

  //----------

  bool operator != (const char* str) {
    return (strcmp(MBuffer,str) != 0) ? true : false;
  }

  //----------

  bool operator != (SAT_DynamicString& str) {
    return (strcmp(MBuffer,str.ptr()) != 0) ? true : false;
  }

//------------------------------
public: // strutils
//------------------------------

  char* createUnique(const char* APrefix, void* APtr) {
    uint32_t len = strlen(APrefix);
    const uint32_t hex_size  = sizeof(void*) * 2;   // 8*2 = 16
    const uint32_t rand_size = sizeof(int32_t) * 2; // 4*2 = 8
    // prefix + ':' + inv_ptr + ':' + random + '\0'
    resizeBuffer( len + 1 + hex_size + 1 + rand_size + 1);
    SAT_CreateUniqueString(MBuffer,APrefix,APtr);
    return MBuffer;
  }

  //----------

  char* find(char* str) {
    return SAT_FindString(MBuffer,str);
  }

  //----------

  uint32_t hash() {
    return SAT_HashString(MBuffer);
  }

  //----------

  char* lowerCase() {
    SAT_LowerCase(MBuffer);
    return MBuffer;
  }

  //----------

  void makeValidSymbol() {
    SAT_MakeValidSymbol(MBuffer);
  }

  //----------

  //void replaceFileExt(const char* ext) {
  //  SAT_ReplaceFileExt(const char* ext);
  //}

  //----------

  char* replace(const char* str1, const char* str2) {
    SAT_ReplaceString(MBuffer,str1,str2);
    return MBuffer;
  }

  //----------

  char* replace(const char c1, const char c2) {
    SAT_ReplaceChars(MBuffer,c1,c2);
    return MBuffer;
  }

  //----------

  char* reverse() {
    SAT_ReverseString(MBuffer);
    return MBuffer;
  }

  //----------

  bool searchWildcards(const char* wildcards) {
    return SAT_SearchWildcards(MBuffer,wildcards);
  }

  //----------

  //void stripFileExt(void) {
  //  SAT_stripFileExt(void);
  //}

  //----------

  //void stripPath() {
  //  SAT_StripPath(MBuffer);
  //}

  //----------

  char* upperCase() {
    SAT_UpperCase(MBuffer);
    return MBuffer;
  }

  //----------

};

//----------------------------------------------------------------------

template <class T>
SAT_DynamicString operator + (T var, const SAT_DynamicString& str)  {
  //SAT_Print;
  SAT_DynamicString svar = var;
  return svar += str;
}

//----------------------------------------------------------------------
#endif





























//
//  SAT_StaticString(const SAT_StaticString& str) {
//    SAT_StaticString* sstr = (SAT_StaticString*)&str;
//    strcpy(MBuffer,sstr->ptr());
//  }
//
//  SAT_StaticString(const char* str) {
//    strcpy(MBuffer,str);
//  }
//
////------------------------------
//public:
////------------------------------
//
//  const char* ptr() {
//    return MBuffer;
//  }
//
//  //----------
//
//  uint32_t length() {
//    return strlen(MBuffer);
//  }
//
//  //----------
//
//  bool empty(void) {
//    if (MBuffer[0] == 0) return true;
//    return false;
//  }
//
//  //----------
//
//  bool notEmpty(void) {
//    if (MBuffer[0] == 0) return false;
//    return true;
//  }
//
//  //----------
//
//  void clear(bool AErase=false) {
//    if (AErase) memset(MBuffer,0,sizeof(MBuffer));
//    else MBuffer[0] = 0;
//  }
//
////------------------------------
//public:
////------------------------------
//
//  const char& operator [] (uint32_t AIndex) const {
//    return MBuffer[AIndex];
//  }
//
//  //----------
//
//  operator char* () {
//    return MBuffer;
//  }
//
//  //----------
//
//  operator const char* ()	const {
//    return MBuffer;
//  }
//
//  //----------
//
//  SAT_StaticString& operator = (char* str) {
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator = (const char* str) {
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator = (/*const*/ SAT_StaticString& str) {
//    strcpy(MBuffer,str.ptr());
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator = (int AValue) {
//    char temp[16];
//    char* str = SAT_IntToString(temp,AValue);
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator = (float AValue) {
//    char temp[16];
//    char* str = SAT_FloatToString(temp,AValue);
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator += (char* str) {
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator += (const char* str) {
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator += (/*const*/ SAT_StaticString& str) {
//    strcat(MBuffer,str.ptr());
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator += (const int i) {
//    char temp[16];
//    char* str = SAT_IntToString(temp,i);
//    //SAT_Trace("%s\n",str);
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator += (const float f) {
//    char temp[16];
//    char* str = SAT_FloatToString(temp,f);
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  bool operator == (const char* str) {
//    return (strcmp(MBuffer,str) == 0) ? true : false;
//  }
//
//  //----------
//
//  bool operator == (SAT_StaticString& str) {
//    return (strcmp(MBuffer,str.ptr()) == 0) ? true : false;
//  }
//
//  //----------
//
//  bool operator != (const char* str) {
//    return (strcmp(MBuffer,str) != 0) ? true : false;
//  }
//
//  //----------
//
//  bool operator != (SAT_StaticString& str) {
//    return (strcmp(MBuffer,str.ptr()) != 0) ? true : false;
//  }
//
////------------------------------
//private:
////------------------------------
//
//  bool _match(char* wildcards, char* str) {
//    bool Yes = 1;
//    //iterate and delete '?' and '*' one by one
//    while(*wildcards != '\0' && Yes && *str != '\0') {
//      if (*wildcards == '?') str ++;
//      else if (*wildcards == '*') {
//        Yes = _scan(wildcards, str);
//        wildcards --;
//      }
//      else {
//        Yes = (*wildcards == *str);
//        str ++;
//      }
//      wildcards ++;
//    }
//    while (*wildcards == '*' && Yes)  wildcards ++;
//    return Yes && *str == '\0' && *wildcards == '\0';
//  }
//
//  //----------
//
//  bool _scan(char*& wildcards, char*& str) {
//    // remove the '?' and '*'
//    for (wildcards ++; *str != '\0' && (*wildcards == '?' || *wildcards == '*'); wildcards ++)
//      if (*wildcards == '?') str ++;
//    while ( *wildcards == '*') wildcards ++;
//    // if str is empty and wildcards has more characters or,
//    // wildcards is empty, return
//    if (*str == '\0' && *wildcards != '\0') return false;
//    if (*str == '\0' && *wildcards == '\0')	return true;
//    // else search substring
//    else {
//      char* wds_copy = wildcards;
//      char* str_copy = str;
//      bool  Yes     = 1;
//      do {
//        if (!_match(wildcards,str))	str_copy++;
//        wildcards = wds_copy;
//        str = str_copy;
//        while ((*wildcards != *str) && (*str != '\0')) str ++;
//        wds_copy = wildcards;
//        str_copy = str;
//      } while ((*str != '\0') ? !_match(wildcards, str) : (Yes = false) != false);
//      if (*str == '\0' && *wildcards == '\0')	return true;
//      return Yes;
//    }
//  }
//
////------------------------------
//public:
////------------------------------
//
//  // search the match string
//  // wildcards can be '?' and '*' combination
//  // return value : true (pattern matchs string), false (no match)
//
//  bool search(const char* wildcards) {
//    return _match((char*)wildcards, MBuffer);
//  }
//
//  //----------
//
//  // returns first occurence of 'str'
//  // or NULL if not found
//
//  char* find(char* str) {
//    return strstr(MBuffer,str);
//  }
//
//  //----------
//
//  void append(const char* str) {
//    strcat(MBuffer,str);
//  }
//
//  //----------
//
//  void append(/*const*/SAT_StaticString& str) {
//    strcat(MBuffer,str.ptr());
//  }
//
//  //----------
//
//  // replace all instances of c1 with c2
//
//  void replaceChars(const char c1, const char c2) {
//    char *pos;
//    while ((pos = strchr(MBuffer,c1))) {
//      *pos = c2;
//    }
//  }
//
//  //----------
//
//  // replace all occurences of str1 with str2
//  // assumes final size will be < 1024
//  // be careful about recursion! replace r with r2 for example.. :-/
//  // todo: wipe out src after replacing, so we don't re-find it?
//
//  void replace(const char* str1, const char* str2) {
//    //_grow_buffer(1024);
//    char temp[1024];
//    uint32_t length = strlen(MBuffer);
//    memcpy(temp,MBuffer,length+1);
//    char *pos;
//    int clen1 = strlen(str1);
//    int clen2 = strlen(str2);
//    while ((pos = strstr(temp,str1))) {
//      //char* bufpos = pos - temp + MBuffer;
//      memmove(pos + clen2, pos + clen1, strlen(pos) - clen1 + 1);
//      memcpy(pos, str2, clen2);
//      //memset(pos , 1, clen1);
//    }
//    //_resize_buffer( strlen(MBuffer) );
//    strcpy(MBuffer,temp);
//  }
//
//  //----------
//
//  void lowerCase(void) {
//    char* ptr = MBuffer;
//    int c;
//    while ((c = *ptr)) {
//      if ((c>='A') && (c<='Z')) *ptr = c + 0x20;
//      ptr++;
//    }
//  }
//
//  //----------
//
//  void upperCase(void) {
//    char* ptr = MBuffer;
//    int c;
//    while ((c = *ptr)) {
//      if ((c>='a') && (c<='z')) *ptr = c - 0x20;
//      ptr++;
//    }
//  }
//
//  //----------
//
//  void reverse(void) {
//    int i;
//    int len = strlen(MBuffer) - 1;
//    int mid = (len % 2) ? (len / 2) : ((len + 1) / 2);
//    for (i=0; i<=mid; ++i) {
//      char buf = MBuffer[i];
//      MBuffer[i] = MBuffer[len - i];
//      MBuffer[len - i] = buf;
//    }
//  }
//
//  //----------
//
//  void makeValidSymbol(void) {
//    for (uint32_t i=0; i<strlen(MBuffer); i++) {
//      unsigned char c = MBuffer[i];
//      if ((c<32) || (c>127)) MBuffer[i] = '_';
//      else MBuffer[i] = SAT_VALID_CSYMBOLS[c];
//    }
//  }
//
//  //----------
//
//  // ptr to (reversed) hex
//  // prefix + ptr2hex (inverted) + random number
//  // (for creating unique strings (based on pointers)
//  // TODO: hash?
//
//  void createUnique(const char* APrefix, void* APtr) {
//    const char hex_table[17] = "0123456789ABCDEF";  // +'\0' = 17
//    const uint32_t hex_size  = sizeof(void*) * 2;
//    const uint32_t rand_size = sizeof(int32_t) * 2;
//    //_resize_buffer( strlen(APrefix) + hex_size + 1 + rand_size );
//    char* buf = MBuffer;//temp;
//    while (*APrefix != '\0') *buf++ = *APrefix++;   // memset + update ptrs
//    intptr_t iptr = (intptr_t)APtr;
//    for (uint32_t i=0; i<hex_size; i++) {
//      int j = iptr & 0x0f;
//      iptr >>= 4;
//      *buf++ = hex_table[j];
//    }
//    *buf++ = ':';
//    int32_t rnd = SAT_RandomInt();
//    for (uint32_t i=0; i<rand_size; i++) {
//      int j = rnd & 0x0f;
//      rnd >>= 4;
//      *buf++ = hex_table[j];
//    }
//    *buf++ = '\0';
//  }
//
//  //----------
//
//  // https://stackoverflow.com/questions/7666509/hash-function-for-string
//
//  uint32_t hash(void) {
//    char* ptr = MBuffer;
//    unsigned long h = 5381;
//    int c;
//    while ((c = *ptr++)) {
//      h = ((h << 5) + h) + c; // h * 33 + c
//    }
//    return h;
//  }
//
//  //----------
//
//  // problem if there is a dot in the path
//  // todo: find last dot..
//
//  /*
//  void stripFileExt(void) {
//    char *dot;
//    dot = strrchr(MBuffer,'.');
//    if (dot) {
//      int32_t len = dot - MBuffer;
//      _resize_buffer(len);
//    }
//  }
//  */
//
//  //----------
//
//  /*
//  void replaceFileExt(const char* ext) {
//    stripFileExt();
//    append(".");
//    append(ext);
//  }
//  */
//
//  //----------
//
//  // /home/skei/test -> test
//
//  /*
//  void stripPath(void) {
//    const char *slash, *backslash;
//    slash     = strrchr(MBuffer,'/');
//    backslash = strrchr(MBuffer,'\\');//+1;
//    if (slash) {
//      _copy_buffer(slash + 1);
//    }
//    else if (backslash) {
//      _copy_buffer(backslash + 1); // see above
//    }
//  }
//  */
//
//
//};
//
//
////----------
//
////template <class T>
////SAT_StaticString operator + (T var, const SAT_StaticString& str)  {
////  SAT_StaticString svar = var;
////  return svar += str;
////}
//
//template <int SIZE, class T>
//SAT_StaticString<SIZE> operator + (T var, const SAT_StaticString<SIZE>& str)  {
//  SAT_StaticString<SIZE> svar = var;
//  return svar += str;
//}
//
//#endif // 0





//----------------------------------------------------------------------
//
//----------------------------------------------------------------------





//#if 0
//
//// todo
//// inherit from SAT_DynamicBuffer
//
//class SAT_DynamicString {
//
//private:
//
//  uint32_t  MLength;
//  uint32_t  MBufferSize;
//  char*     MBuffer;
//
//private:
//
//  void resize(uint32_t ASize) {
//  }
//
//};
//
////----------
//
//typedef SAT_StaticString<SAT_MAX_STRING_LENGTH> SAT_String;
//
//#endif // 0







//#include "base/sat_convert.h"
//#include "base/sat_random.h"
//
//template <int SIZE>
//class SAT_StaticString {
//
//private:
//
//  //char MBuffer[SAT_MAX_STRING_LENGTH];
//  char MBuffer[SIZE];
//
////------------------------------
////
////------------------------------
//
//public:
//
//  SAT_StaticString() {
//    MBuffer[0] = 0;
//  }
//
//  SAT_StaticString(const SAT_StaticString& str) {
//    SAT_StaticString* sstr = (SAT_StaticString*)&str;
//    strcpy(MBuffer,sstr->ptr());
//  }
//
//  SAT_StaticString(const char* str) {
//    strcpy(MBuffer,str);
//  }
//
////------------------------------
//public:
////------------------------------
//
//  const char* ptr() {
//    return MBuffer;
//  }
//
//  //----------
//
//  uint32_t length() {
//    return strlen(MBuffer);
//  }
//
//  //----------
//
//  bool empty(void) {
//    if (MBuffer[0] == 0) return true;
//    return false;
//  }
//
//  //----------
//
//  bool notEmpty(void) {
//    if (MBuffer[0] == 0) return false;
//    return true;
//  }
//
//  //----------
//
//  void clear(bool AErase=false) {
//    if (AErase) memset(MBuffer,0,sizeof(MBuffer));
//    else MBuffer[0] = 0;
//  }
//
////------------------------------
//public:
////------------------------------
//
//  const char& operator [] (uint32_t AIndex) const {
//    return MBuffer[AIndex];
//  }
//
//  //----------
//
//  operator char* () {
//    return MBuffer;
//  }
//
//  //----------
//
//  operator const char* ()	const {
//    return MBuffer;
//  }
//
//  //----------
//
//  SAT_StaticString& operator = (char* str) {
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator = (const char* str) {
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator = (/*const*/ SAT_StaticString& str) {
//    strcpy(MBuffer,str.ptr());
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator = (int AValue) {
//    char temp[16];
//    char* str = SAT_IntToString(temp,AValue);
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator = (float AValue) {
//    char temp[16];
//    char* str = SAT_FloatToString(temp,AValue);
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator += (char* str) {
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator += (const char* str) {
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator += (/*const*/ SAT_StaticString& str) {
//    strcat(MBuffer,str.ptr());
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator += (const int i) {
//    char temp[16];
//    char* str = SAT_IntToString(temp,i);
//    //SAT_Trace("%s\n",str);
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  SAT_StaticString& operator += (const float f) {
//    char temp[16];
//    char* str = SAT_FloatToString(temp,f);
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  bool operator == (const char* str) {
//    return (strcmp(MBuffer,str) == 0) ? true : false;
//  }
//
//  //----------
//
//  bool operator == (SAT_StaticString& str) {
//    return (strcmp(MBuffer,str.ptr()) == 0) ? true : false;
//  }
//
//  //----------
//
//  bool operator != (const char* str) {
//    return (strcmp(MBuffer,str) != 0) ? true : false;
//  }
//
//  //----------
//
//  bool operator != (SAT_StaticString& str) {
//    return (strcmp(MBuffer,str.ptr()) != 0) ? true : false;
//  }
//
////------------------------------
//private:
////------------------------------
//
//  bool _match(char* wildcards, char* str) {
//    bool Yes = 1;
//    //iterate and delete '?' and '*' one by one
//    while(*wildcards != '\0' && Yes && *str != '\0') {
//      if (*wildcards == '?') str ++;
//      else if (*wildcards == '*') {
//        Yes = _scan(wildcards, str);
//        wildcards --;
//      }
//      else {
//        Yes = (*wildcards == *str);
//        str ++;
//      }
//      wildcards ++;
//    }
//    while (*wildcards == '*' && Yes)  wildcards ++;
//    return Yes && *str == '\0' && *wildcards == '\0';
//  }
//
//  //----------
//
//  bool _scan(char*& wildcards, char*& str) {
//    // remove the '?' and '*'
//    for (wildcards ++; *str != '\0' && (*wildcards == '?' || *wildcards == '*'); wildcards ++)
//      if (*wildcards == '?') str ++;
//    while ( *wildcards == '*') wildcards ++;
//    // if str is empty and wildcards has more characters or,
//    // wildcards is empty, return
//    if (*str == '\0' && *wildcards != '\0') return false;
//    if (*str == '\0' && *wildcards == '\0')	return true;
//    // else search substring
//    else {
//      char* wds_copy = wildcards;
//      char* str_copy = str;
//      bool  Yes     = 1;
//      do {
//        if (!_match(wildcards,str))	str_copy++;
//        wildcards = wds_copy;
//        str = str_copy;
//        while ((*wildcards != *str) && (*str != '\0')) str ++;
//        wds_copy = wildcards;
//        str_copy = str;
//      } while ((*str != '\0') ? !_match(wildcards, str) : (Yes = false) != false);
//      if (*str == '\0' && *wildcards == '\0')	return true;
//      return Yes;
//    }
//  }
//
////------------------------------
//public:
////------------------------------
//
//  // search the match string
//  // wildcards can be '?' and '*' combination
//  // return value : true (pattern matchs string), false (no match)
//
//  bool search(const char* wildcards) {
//    return _match((char*)wildcards, MBuffer);
//  }
//
//  //----------
//
//  // returns first occurence of 'str'
//  // or NULL if not found
//
//  char* find(char* str) {
//    return strstr(MBuffer,str);
//  }
//
//  //----------
//
//  void append(const char* str) {
//    strcat(MBuffer,str);
//  }
//
//  //----------
//
//  void append(/*const*/SAT_StaticString& str) {
//    strcat(MBuffer,str.ptr());
//  }
//
//  //----------
//
//  // replace all instances of c1 with c2
//
//  void replaceChars(const char c1, const char c2) {
//    char *pos;
//    while ((pos = strchr(MBuffer,c1))) {
//      *pos = c2;
//    }
//  }
//
//  //----------
//
//  // replace all occurences of str1 with str2
//  // assumes final size will be < 1024
//  // be careful about recursion! replace r with r2 for example.. :-/
//  // todo: wipe out src after replacing, so we don't re-find it?
//
//  void replace(const char* str1, const char* str2) {
//    //_grow_buffer(1024);
//    char temp[1024];
//    uint32_t length = strlen(MBuffer);
//    memcpy(temp,MBuffer,length+1);
//    char *pos;
//    int clen1 = strlen(str1);
//    int clen2 = strlen(str2);
//    while ((pos = strstr(temp,str1))) {
//      //char* bufpos = pos - temp + MBuffer;
//      memmove(pos + clen2, pos + clen1, strlen(pos) - clen1 + 1);
//      memcpy(pos, str2, clen2);
//      //memset(pos , 1, clen1);
//    }
//    //_resize_buffer( strlen(MBuffer) );
//    strcpy(MBuffer,temp);
//  }
//
//  //----------
//
//  void lowerCase(void) {
//    char* ptr = MBuffer;
//    int c;
//    while ((c = *ptr)) {
//      if ((c>='A') && (c<='Z')) *ptr = c + 0x20;
//      ptr++;
//    }
//  }
//
//  //----------
//
//  void upperCase(void) {
//    char* ptr = MBuffer;
//    int c;
//    while ((c = *ptr)) {
//      if ((c>='a') && (c<='z')) *ptr = c - 0x20;
//      ptr++;
//    }
//  }
//
//  //----------
//
//  void reverse(void) {
//    int i;
//    int len = strlen(MBuffer) - 1;
//    int mid = (len % 2) ? (len / 2) : ((len + 1) / 2);
//    for (i=0; i<=mid; ++i) {
//      char buf = MBuffer[i];
//      MBuffer[i] = MBuffer[len - i];
//      MBuffer[len - i] = buf;
//    }
//  }
//
//  //----------
//
//  void makeValidSymbol(void) {
//    for (uint32_t i=0; i<strlen(MBuffer); i++) {
//      unsigned char c = MBuffer[i];
//      if ((c<32) || (c>127)) MBuffer[i] = '_';
//      else MBuffer[i] = SAT_VALID_CSYMBOLS[c];
//    }
//  }
//
//  //----------
//
//  // ptr to (reversed) hex
//  // prefix + ptr2hex (inverted) + random number
//  // (for creating unique strings (based on pointers)
//  // TODO: hash?
//
//  void createUnique(const char* APrefix, void* APtr) {
//    const char hex_table[17] = "0123456789ABCDEF";  // +'\0' = 17
//    const uint32_t hex_size  = sizeof(void*) * 2;
//    const uint32_t rand_size = sizeof(int32_t) * 2;
//    //_resize_buffer( strlen(APrefix) + hex_size + 1 + rand_size );
//    char* buf = MBuffer;//temp;
//    while (*APrefix != '\0') *buf++ = *APrefix++;   // memset + update ptrs
//    intptr_t iptr = (intptr_t)APtr;
//    for (uint32_t i=0; i<hex_size; i++) {
//      int j = iptr & 0x0f;
//      iptr >>= 4;
//      *buf++ = hex_table[j];
//    }
//    *buf++ = ':';
//    int32_t rnd = SAT_RandomInt();
//    for (uint32_t i=0; i<rand_size; i++) {
//      int j = rnd & 0x0f;
//      rnd >>= 4;
//      *buf++ = hex_table[j];
//    }
//    *buf++ = '\0';
//  }
//
//  //----------
//
//  // https://stackoverflow.com/questions/7666509/hash-function-for-string
//
//  uint32_t hash(void) {
//    char* ptr = MBuffer;
//    unsigned long h = 5381;
//    int c;
//    while ((c = *ptr++)) {
//      h = ((h << 5) + h) + c; // h * 33 + c
//    }
//    return h;
//  }
//
//  //----------
//
//  // problem if there is a dot in the path
//  // todo: find last dot..
//
//  /*
//  void stripFileExt(void) {
//    char *dot;
//    dot = strrchr(MBuffer,'.');
//    if (dot) {
//      int32_t len = dot - MBuffer;
//      _resize_buffer(len);
//    }
//  }
//  */
//
//  //----------
//
//  /*
//  void replaceFileExt(const char* ext) {
//    stripFileExt();
//    append(".");
//    append(ext);
//  }
//  */
//
//  //----------
//
//  // /home/skei/test -> test
//
//  /*
//  void stripPath(void) {
//    const char *slash, *backslash;
//    slash     = strrchr(MBuffer,'/');
//    backslash = strrchr(MBuffer,'\\');//+1;
//    if (slash) {
//      _copy_buffer(slash + 1);
//    }
//    else if (backslash) {
//      _copy_buffer(backslash + 1); // see above
//    }
//  }
//  */
//
//
//};
//
//
////----------
//
////template <class T>
////SAT_StaticString operator + (T var, const SAT_StaticString& str)  {
////  SAT_StaticString svar = var;
////  return svar += str;
////}
//
//template <int SIZE, class T>
//SAT_StaticString<SIZE> operator + (T var, const SAT_StaticString<SIZE>& str)  {
//  SAT_StaticString<SIZE> svar = var;
//  return svar += str;
//}





//----------------------------------------------------------------------
//
//----------------------------------------------------------------------





//// todo
//// inherit from SAT_DynamicBuffer
//
//class SAT_DynamicString {
//
//private:
//
//  uint32_t  MLength;
//  uint32_t  MBufferSize;
//  char*     MBuffer;
//
//private:
//
//  void resize(uint32_t ASize) {
//  }
//
//};
//
////----------
//
//typedef SAT_StaticString<SAT_MAX_STRING_LENGTH> SAT_String;
