#ifndef sat_inifile_included
#define sat_inifile_included
//----------------------------------------------------------------------

#include "base/system/sat_file.h"
#include "base/utils/sat_convert.h"
#include "base/utils/sat_tokenizer.h"
#include "base/utils/sat_strutils.h"
#include "base/system/sat_paths.h"

//----------------------------------------------------------------------

#define SAT_INI_KEY_NAME_LENGTH      127
#define SAT_INI_KEY_VALUE_LENGTH     127
#define SAT_INI_SECTION_NAME_LENGTH  127

//----------

// -> SAT_IniKeyValuePair

struct SAT_IniKey {
  char name[SAT_INI_KEY_NAME_LENGTH+1] = {0};
  char value[SAT_INI_KEY_VALUE_LENGTH+1] = {0};
};

typedef SAT_Array<SAT_IniKey*> SAT_IniKeys;

//----------

struct SAT_IniSection {
  char name[SAT_INI_SECTION_NAME_LENGTH+1] = {0};
  SAT_IniKeys keys;
};

typedef SAT_Array<SAT_IniSection*> SAT_IniSections;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class SAT_IniFile {

//------------------------------
private:
//------------------------------

  SAT_IniSections  MSections;

//------------------------------
public:
//------------------------------

  SAT_IniFile() {
  }

  //----------

  ~SAT_IniFile() {
    #ifndef SAT_NO_AUTODELETE
      deleteSections();
    #endif
  }

//------------------------------
public:
//------------------------------

  void appendSection(const char* ASection) {

printf("appendSection: %s\n",ASection);

    SAT_IniSection* section = new SAT_IniSection();
    SAT_Strlcpy(section->name,ASection,SAT_INI_SECTION_NAME_LENGTH);
    MSections.append(section);
  }

  //----------

  void deleteSections() {
    for (uint32_t i=0; i<MSections.size(); i++) {
      SAT_IniSection* section = MSections[i];
      if (section) {
        for (uint32_t j=0; j<section->keys.size(); j++) {
          if (section->keys[j]) delete section->keys[j];
        }
      }
      delete MSections[i];
    }
  }

  //----------

  SAT_IniSection* findSection(const char* ASection) {
    for (uint32_t i=0; i<MSections.size(); i++) {
      SAT_IniSection* section = MSections[i];
      if (strcmp(section->name, ASection) == 0) {
        return section;
      }
    }
    return  nullptr;
  }

  //----------

  void appendKey(const char* ASection, const char* AName, const char* AValue) {

printf("appendKey: section '%s' name '%s' value '%s'\n",ASection,AName,AValue);

    SAT_IniSection* section = findSection(ASection);
    SAT_IniKey* key = new SAT_IniKey();
    SAT_Strlcpy(key->name,AName,SAT_INI_KEY_NAME_LENGTH);
    SAT_Strlcpy(key->value,AValue,SAT_INI_KEY_VALUE_LENGTH);
    section->keys.append(key);
  }

  //----------

  SAT_IniKey* findKey(const char* ASection, const char* AKey) {
    SAT_IniSection* section = findSection(ASection);
    if (section) {
      for (uint32_t i=0; i<section->keys.size(); i++) {
        SAT_IniKey* key =section->keys[i];
        if (strcmp(key->name, AKey) == 0) return key;
      }
    }
    return nullptr;
  }

  //----------

  uint32_t getNumSections() {
    return MSections.size();
  }

  SAT_IniSection* getSection(uint32_t AIndex) {
    return MSections[AIndex];
  }

  uint32_t getNumKeys(SAT_IniSection* ASection) {
    return ASection->keys.size();
  }

  SAT_IniKey* getKey(SAT_IniSection* ASection, uint32_t AIndex) {
    return ASection->keys[AIndex];
  }

//------------------------------
public:
//------------------------------

  const char* getString(const char* ASection, const char* AKey) {
    SAT_IniKey* key = findKey(ASection,AKey);
    if (key) {
      return key->value;
    }
    return nullptr;
  }

  //----------

  float getFloat(const char* ASection, const char* AKey) {
    SAT_IniKey* key = findKey(ASection,AKey);
    if (key) {
      float f = SAT_StringToFloat((char*)key->value);
      return f;
    }
    return 0.0f;
  }

  //----------

  int32_t getInt(const char* ASection, const char* AKey) {
    SAT_IniKey* key = findKey(ASection,AKey);
    if (key) {
      int32_t i = SAT_StringToInt((char*)key->value);
      return i;
    }
    return 0;
  }

//------------------------------
public:
//------------------------------

  bool load(void) {
    char filename[256];
    SAT_GetBaseFilename(filename);
    SAT_StripFileExt(filename);
    strcat(filename,".ini");

    printf("loading '%s'\n",filename);

    return load(filename);
  }

  //----------

  bool load(const char* filename) {
    SAT_File file;
    if (file.exists(filename)) {
      file.open(filename,SAT_FILE_READ_BINARY);
      void* buffer = nullptr;
      uint32_t buffersize = file.length();
      if (buffersize>0) {
        buffer = malloc(buffersize);
        file.read(buffer,buffersize);
      }
      file.close();
      parse(buffer,buffersize);
      free(buffer);
      return true;
    }
    return false;
  }

  //----------

  bool save(void) {
    char filename[256];
    SAT_GetBaseFilename(filename);
    SAT_StripFileExt(filename);
    strcat(filename,".ini");
    return save(filename);
  }

  //----------

  bool save(const char* filename) {
    SAT_File file;
    //if (file.exists(filename)) {
    //  SAT_DPrint("%s already exists. overwriting\n",filename);
    //}
    file.open(filename,SAT_FILE_WRITE_TEXT);
    for (uint32_t i=0; i<MSections.size(); i++) {
      SAT_IniSection* section = MSections[i];
      // [
      file.write((void*)"[",1);
      // section name
      const char* txt = section->name;
      uint32_t len = strlen(txt);
      file.write((void*)txt,len);
      // ]
      file.write((void*)"]\n",2);
      for (uint32_t j=0; j<section->keys.size(); j++) {
        SAT_IniKey* key =section->keys[j];
        // key.name
        const char* txt = key->name;
        uint32_t len = strlen(txt);
        bool has_spaces = false;
        for (uint32_t s=0; s<len; s++) {
          if (txt[s] == ' ') {
            has_spaces = true;
            break;
          }
        }
        if (has_spaces) {
          // "key"
          file.write((void*)"\"",1);
          file.write((void*)txt,len);
          file.write((void*)"\"",1);
        }
        else {
          // key
          file.write((void*)txt,len);
        }
        // =
        file.write((void*)" = ",3);
        txt = key->value;
        len = strlen(txt);
        has_spaces = false;
        for (uint32_t s=0; s<len; s++) {
          if (txt[s] == ' ') {
            has_spaces = true;
            break;
          }
        }
        if (has_spaces) {
          // "value"
          file.write((void*)"\"",1);
          file.write((void*)txt,len);
          file.write((void*)"\"",1);
        }
        else {
          // value
          file.write((void*)txt,len);
        }
        file.write((void*)"\n",1);
      }
    }
    file.close();
    return false;
  }

  //----------

  //void unload(void) {
  //  if (MBuffer) SAT_Free(MBuffer);
  //  MBuffer = nullptr;
  //}

  //----------

  // todo:
  // parser->getNameValuePair(..)

  void parse(void* ABuffer, uint32_t ABufferSize) {
    SAT_Tokenizer* tokenizer = new SAT_Tokenizer();
    tokenizer->tokenize((const char*)ABuffer,ABufferSize);

    const char* section = "";
    uint32_t t = 0;
    while (t < tokenizer->getNumTokens()) {
      char* token = tokenizer->getToken(t++);
      uint32_t tlen = strlen(token);
      if ((token[0] == '[') && (token[tlen-1] == ']')) {
        token[tlen-1] = 0;
        section = token + 1;
        appendSection(section);
      }
      else {
        const char* equal = tokenizer->getToken(t++);
        if ((equal[0] == '=') && (equal[1] == 0)) {
          const char* value = tokenizer->getToken(t++);
          appendKey(section,token,value);
        }
      }
    }

  }

  //----------

};

//----------------------------------------------------------------------
#endif
