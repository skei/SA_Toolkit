#ifndef sat_mac_paths_included
#define sat_mac_paths_included
//----------------------------------------------------------------------

// parts of the code by lubomir i ivanov (for axonlib)
// used with permission

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // readlink
#include <dlfcn.h>

#include "sat.h"

/*
  SAT_GetHomePath    '/home/<user>/'
  SAT_GetExeFilename '/home/<user>/Desktop/skeilib/bin/skeilib_debug'
  SAT_GetExePath     '/home/<user>/Desktop/skeilib/bin/'
*/

//----------------------------------------------------------------------

char* SAT_GetHomePath(char* ABuffer) {
  ABuffer[0] = '\0';
  char* env = getenv("HOME");
  if (env) strcat(ABuffer,env);
  //KStrcat(ABuffer, (char*)"\\");
  strcat(ABuffer, (char*)"/");
  return ABuffer;
}

//----------

char* SAT_GetDesktopPath(char* ABuffer) {
  ABuffer[0] = '\0';
  char* env = getenv("HOME");
  if (env) strcat(ABuffer,env);
  //KStrcat(ABuffer, (char*)"\\");
  strcat(ABuffer, (char*)"/Desktop/");
  return ABuffer;
}

//----------

const char* SAT_GetExeFilename(char* ABuffer) {
  ABuffer[0] = '\0';
  int32_t len = readlink("/proc/self/exe",ABuffer,SAT_MAX_PATH_LENGTH-1);
  if (len != -1) {
    ABuffer[len] = '\0';
  }
  return ABuffer;
}

//----------

const char* SAT_GetExePath(char* ABuffer) {
  char filepath[SAT_MAX_PATH_LENGTH] = "";
  long rd_res = readlink("/proc/self/exe", filepath, SAT_MAX_PATH_LENGTH);
  filepath[SAT_MAX_PATH_LENGTH-1] = '\0';
  if (rd_res) {
    const char* slash = strrchr(filepath, '/');
    if (slash) {
      int len = (slash + 1) - (char*)filepath;
      SAT_Strlcpy(ABuffer, filepath, len);
      ABuffer[len] = '\0';
    }
    else {
      //SStrcat(ABuffer, (char*)"./");
      //SStrcat(ABuffer, (char*)"/"); // -> "/" -> root :-/
    }
  }
  return ABuffer;
}

//----------

const char* SAT_GetLibFilename(char* ABuffer) {
  ABuffer[0] = '\0';
  Dl_info dli;
  dladdr(__func__, &dli);
  //int32 len = SAT_Strlen(dli.dli_fname);
  strcpy(ABuffer,dli.dli_fname);
  return ABuffer;//nullptr;
}

//----------

const char* SAT_GetLibPath(char* ABuffer) {
  Dl_info dli;
  dladdr(__func__, &dli);
  const char* slash = strrchr(dli.dli_fname, '/');
  if (slash) {
    int len = (slash + 1) - (char*)dli.dli_fname;
    SAT_Strlcpy(ABuffer, dli.dli_fname, len);
    ABuffer[len] = '\0';
  }
  else {
    strcat(ABuffer, (char*)"./");
  }
  return ABuffer;
}

//----------------------------------------------------------------------

#ifdef SAT_PLUGIN
  #define SAT_GetBaseFilename  SAT_GetLibFilename
  #define SAT_GetBasePath      SAT_GetLibPath
#else
  #define SAT_GetBaseFilename  SAT_GetExeFilename
  #define SAT_GetBasePath      SAT_GetExePath
#endif

//----------------------------------------------------------------------
#endif
