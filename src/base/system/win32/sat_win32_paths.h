#ifndef sat_win32_paths_included
#define sat_win32_paths_included
//----------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "sat.h"
#include "base/sat_win32.h"

//#undef  _WIN32_WINNT
//#define _WIN32_WINNT 0x0501
////#ifndef SAT_WIN32_GDIPLUS
//#define  WIN32_LEAN_AND_MEAN
////#endif
//#include <windows.h>

#include <shlobj.h>

//----------------------------------------------------------------------

/*
  SAT_GetHomePath    '/home/<user>/'
  SAT_GetExeFilename '/home/<user>/Desktop/skeilib/bin/skeilib_debug'
  SAT_GetExePath     '/home/<user>/Desktop/skeilib/bin/'
*/

//----------------------------------------------------------------------

char* SAT_GetHomePath(char* ABuffer) {
  //strcat(ABuffer, getenv("HOME"));
  ABuffer[0] = '\0';
  // https://msdn.microsoft.com/en-us/library/windows/desktop/bb762181(v=vs.85).aspx
  WCHAR wch[MAX_PATH];
  if (SUCCEEDED(SHGetFolderPathW( NULL, CSIDL_PROFILE, NULL, 0, wch ))) {
    wcstombs(ABuffer,wch,SAT_MAX_PATH_LENGTH-1);
    strcat(ABuffer, (char*)"\\");
  }
  return ABuffer;
}

//----------

char* SAT_GetDesktopPath(char* ABuffer) {
  //strcat(ABuffer, getenv("HOME"));
  ABuffer[0] = '\0';
  // https://msdn.microsoft.com/en-us/library/windows/desktop/bb762181(v=vs.85).aspx
  WCHAR wch[MAX_PATH];
  if (SUCCEEDED(SHGetFolderPathW( NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, wch ))) {
    wcstombs(ABuffer,wch,SAT_MAX_PATH_LENGTH-1);
    strcat(ABuffer, (char*)"\\");
  }
  return ABuffer;
}

//----------

const char* SAT_GetExeFilename(char* ABuffer) {
  ABuffer[0] = '\0';
  //#if defined SAT_PLUGIN //|| defined SAT_PLUGIN_EXE
  //  GetModuleFileName(SAT_GLOBAL_WIN32_INSTANCE, ABuffer, SAT_MAX_PATH_LENGTH-1);
  //#else
    strcat(ABuffer,(char*)".\\");
  //#endif
  return ABuffer;//nullptr;
}

//----------

const char* SAT_GetExePath(char* ABuffer) {
  //#if defined SAT_PLUGIN //|| defined SAT_PLUGIN_EXE
  //  char filepath[SAT_MAX_PATH_LENGTH] = "";
  //  GetModuleFileName(SAT_GLOBAL_WIN32_INSTANCE, filepath, SAT_MAX_PATH_LENGTH-1);
  //  filepath[SAT_MAX_PATH_LENGTH-1] = '\0';
  //  const char* slash = strrchr(filepath,'\\');
  //  if (slash) {
  //    int len = (slash+1) - (char*)filepath;
  //    strncpy(ABuffer, filepath, len);
  //    ABuffer[len] = '\0';
  //  }
  //  else {
  //    strcat(ABuffer,(char*)".\\");
  //  }
  //#else
    strcat(ABuffer,(char*)".\\");
  //#endif
  return ABuffer;
}

//----------

const char* SAT_GetLibFilename(char* ABuffer) {
  ABuffer[0] = '\0';
  //#if defined SAT_PLUGIN //|| defined SAT_PLUGIN_EXE
    GetModuleFileName(SAT_GLOBAL_WIN32_INSTANCE, ABuffer, SAT_MAX_PATH_LENGTH-1);
  //#else
  //  strcat(ABuffer,(char*)".\\");
  //#endif
  return ABuffer;//nullptr;
}

//----------

const char* SAT_GetLibPath(char* ABuffer) {
  //#if defined SAT_PLUGIN //|| defined SAT_PLUGIN_EXE
    char filepath[SAT_MAX_PATH_LENGTH] = "";
    GetModuleFileName(SAT_GLOBAL_WIN32_INSTANCE, filepath, SAT_MAX_PATH_LENGTH-1);
    filepath[SAT_MAX_PATH_LENGTH-1] = '\0';
    const char* slash = strrchr(filepath,'\\');
    if (slash) {
      int len = (slash+1) - (char*)filepath;
      strncpy(ABuffer, filepath, len);
      ABuffer[len] = '\0';
    }
    else {
      strcat(ABuffer,(char*)".\\");
    }
  //#else
  //  strcat(ABuffer,(char*)".\\");
  //#endif
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


//----------

//const char* SAT_GetBaseFile(char* ABuffer) {
//  ABuffer[0] = '\0';
//  #if defined SAT_PLUGIN //|| defined SAT_PLUGIN_EXE
//    GetModuleFileName(SAT_GLOBAL_WIN32_INSTANCE, ABuffer, SAT_MAX_PATH_LENGTH-1);
//  #else
//    strcat(ABuffer,(char*)".\\");
//  #endif
//  return ABuffer;//nullptr;
//}

//----------

//char* SAT_GetBasePath(char* ABuffer) {
//  #if defined SAT_PLUGIN //|| defined SAT_PLUGIN_EXE
//    char filepath[SAT_MAX_PATH_LENGTH] = "";
//    GetModuleFileName(SAT_GLOBAL_WIN32_INSTANCE, filepath, SAT_MAX_PATH_LENGTH-1);
//    filepath[SAT_MAX_PATH_LENGTH-1] = '\0';
//    const char* slash = strrchr(filepath,'\\');
//    if (slash) {
//      int len = (slash+1) - (char*)filepath;
//      strncpy(ABuffer, filepath, len);
//      ABuffer[len] = '\0';
//    }
//    else {
//      strcat(ABuffer,(char*)".\\");
//    }
//  #else
//    strcat(ABuffer,(char*)".\\");
//  #endif
//  return ABuffer;
//}

//----------------------------------------------------------------------
#endif
