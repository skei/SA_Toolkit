

#if 0

#ifndef sat_vst3_utils_included
#define sat_vst3_utils_included
//----------------------------------------------------------------------

// TODO: move to str/string files/classes..

#include "plugin/vst3/sat_vst3.h"

#include <string>
#include <locale>
#include <codecvt>

char16_t* vst3_str16cpy(char16_t* destination, const char16_t* source) {
  char16_t* temp = destination;
  while((*temp++ = *source++) != 0) ;
  return destination;
}

//----------

void VST3_CharToUtf16(const void* C, void* U) {
  std::string source = (char*)C;
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
  std::u16string dest = convert.from_bytes(source);
  vst3_str16cpy((char16_t*)U,dest.data());
}

//----------

void VST3_Utf16ToChar(const void* U, void* C) {
  std::u16string source = (char16_t*)U;
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
  std::string dest = convert.to_bytes(source);
  strcpy((char*)C,dest.data());
}

//----------

//bool sat_vst3_iid_equal(const void* iid1, const void* iid2) {
//  uint8_t* ptr1 = (uint8_t*)iid1;
//  uint8_t* ptr2 = (uint8_t*)iid2;
//  for (uint32_t i=0; i<16; i++) {
//    if (*ptr1++ != *ptr2++) return false;
//  }
//  return true;
//}

//----------

bool VST3_iidEqual(const void* iid1, const void* iid2) {
  uint32_t* ptr1 = (uint32_t*)iid1;
  uint32_t* ptr2 = (uint32_t*)iid2;
  if (ptr1[0] != ptr2[0]) return false;
  if (ptr1[1] != ptr2[1]) return false;
  if (ptr1[2] != ptr2[2]) return false;
  if (ptr1[3] != ptr2[3]) return false;
  return true;
}

//----------

//#define strncpy8 strncpy

//char* strncpy8(char* destination, const char* source, size_t num) {
//  memset(destination,0,num);
//  strncpy(destination,source,num-1);
//  return destination;
//}

//----------



void VST3_PrintIID(const void* iid) {
  const char hex_table[] = "0123456789ABCDEF";
  uint8_t* ptr = (uint8_t*)iid;
  for (uint32_t i=0; i<4; i++) {
    SAT_DPrint("%c",hex_table[ ptr[i] >> 4 ]);
    SAT_DPrint("%c",hex_table[ ptr[i] & 15 ]);
  }
  SAT_DPrint("-");
  for (uint32_t i=4; i<8; i++) {
    SAT_DPrint("%c",hex_table[ ptr[i] >> 4 ]);
    SAT_DPrint("%c",hex_table[ ptr[i] & 15 ]);
  }
  SAT_DPrint("-");
  for (uint32_t i=8; i<12; i++) {
    SAT_DPrint("%c",hex_table[ ptr[i] >> 4 ]);
    SAT_DPrint("%c",hex_table[ ptr[i] & 15 ]);
  }
  SAT_DPrint("-");
  for (uint32_t i=12; i<16; i++) {
    SAT_DPrint("%c",hex_table[ ptr[i] >> 4 ]);
    SAT_DPrint("%c",hex_table[ ptr[i] & 15 ]);
  }
}


//----------------------------------------------------------------------
#endif

#endif // 0
