#ifndef sat_json_included
#define sat_json_included
//----------------------------------------------------------------------

// https://github.com/stbachmann/xjson

//----------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wsign-compare"

  #define XJSON_H_IMPLEMENTATION
  #include "extern/xjson/xjson.h"

#pragma GCC diagnostic pop

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class SAT_Json {

//------------------------------
private:
//------------------------------

  xjson*    MJson             = nullptr;
  //char*     MBuffer           = nullptr;
  //uint32_t  MBufferSize       = 0;
  //bool      MBufferAllocated  = false;

//------------------------------
public:
//------------------------------

  SAT_Json() {
    //MBufferSize       = ABufferSize;
    //MBuffer           = ABuffer;
    //MBufferAllocated  = false;

    MJson             = (xjson*)malloc(sizeof(xjson));
    memset(MJson,0,sizeof(xjson));
    xjson_set_string_allocator(MJson,sat_json_allocate_string);
  }

  virtual ~SAT_Json() {
    //if (MBuffer && MBufferAllocated) free(MBuffer);
  }

//------------------------------
private:
//------------------------------

  static
  char* sat_json_allocate_string(const char* str, size_t str_len, void* mem_ctx) {
    char* new_str = (char*)malloc(str_len + 1); // zero-terminate!
    memcpy(new_str, str, str_len);
    new_str[str_len] = '\0';
    return new_str;
  }

//------------------------------
public:
//------------------------------

  // Sets xjson to read-mode using the string pointed to by json_str up to length len

  void setupRead(const char* AString, uint32_t ALength) {
    xjson_setup_read(MJson,AString,ALength);
  }

  // Sets xjson to write-mode and the json is written to buffer. If pretty_print is set to true, it'll produce a more readable output

  void setupWrite(bool APrettyPrint, char* AString, uint32_t ALength) {
    xjson_setup_write(MJson,APrettyPrint,AString,ALength);
  }

  // Sets a custom string allocator method. Expects that the returned char* is zero-terminated!

  void setStringAllocator(char* (*AStringAllocator)(const char* AString, size_t ASize, void* AMemCtx)) {
    xjson_set_string_allocator(MJson,AStringAllocator);
  }

  // Returns either XJSON_STATE_READ or XJSON_STATE_WRITE

  uint32_t getState() {
    return xjson_get_state(MJson);
  }

  // Begins a json object scope, all future value calls will use this object until a new scope is introduced

  void objectBegin(const char* AKey) {
    xjson_object_begin(MJson,AKey);
  }

  // Ends a json object scope

  void objectEnd() {
    xjson_object_end(MJson);
  }

  // Begins an array, all future value calls will use this array until a new scope is introduced

  void arrayBegin(const char* AKey) {
    xjson_array_begin(MJson,AKey);
  }

  // Ends an array

  void arrayEnd() {
    xjson_array_end(MJson);
  }

  // Will return true if an array end has been reached. Use this to parse/write an array with loops

  bool arrayReachedEnd(uint32_t ACounter, uint32_t ASize) {
    return xjson_array_reached_end(MJson,ACounter,ASize);
  }

  // Reads/Writes just the key, it means next value call should not supply a key (nullptr). Useful for hashmaps
  void key(const char** AKey) {
    xjson_key(MJson,AKey);
  }

  void u8 (const char* AKey, uint8_t*  AVal) { xjson_u8(MJson,AKey,AVal); }
  void u16(const char* AKey, uint16_t* AVal) { xjson_u16(MJson,AKey,AVal); }
  void u32(const char* AKey, uint32_t* AVal) { xjson_u32(MJson,AKey,AVal); }
  void u64(const char* AKey, uint64_t* AVal) { xjson_u64(MJson,AKey,AVal); }

  void i8 (const char* AKey, int8_t* AVal) { xjson_i8(MJson,AKey,AVal); }
  void i16(const char* AKey, int16_t* AVal) { xjson_i16(MJson,AKey,AVal); }
  void i32(const char* AKey, int32_t* AVal) { xjson_i32(MJson,AKey,AVal); }
  void i64(const char* AKey, int64_t* AVal) { xjson_i64(MJson,AKey,AVal); }

  void f32(const char* AKey, float* AVal) { xjson_float(MJson,AKey,AVal); }
  void f64(const char* AKey, double* AVal) { xjson_double(MJson,AKey,AVal); }

  void boolean(const char* AKey, bool* AVal) { xjson_bool(MJson,AKey,AVal); }
  void string(const char* AKey, const char** AStr) { xjson_string(MJson,AKey,AStr); }

  bool error() {
    return MJson->error;
  }

  const char* errorMessage() {
    return MJson->error_message;
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif
