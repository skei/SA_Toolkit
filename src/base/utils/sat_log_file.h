#ifndef sat_logfile_included
#define sat_logfile_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/system/sat_file.h"
#include "base/system/sat_paths.h"
#include "base/system/sat_time.h"

#include "plugin/clap/sat_clap.h"

//#define SAT_LOG_FILE_APPEND
//#ifdef SAT_LOGGING

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#ifdef SAT_LOGGING

class SAT_LogFile {

//------------------------------
private:
//------------------------------

  SAT_File    MFile                             = {};
  const char* MLogfileName                      = "sat_log.txt";
  bool        MEnabled                          = false;
  char        MDesktopPath[SAT_MAX_PATH_LENGTH] = {0};
  char        MFilename[SAT_MAX_PATH_LENGTH*2]  = {0};
  char        MTempBuffer[SAT_MAX_PATH_LENGTH]  = {0};
  //char      MHomePath[SAT_MAX_PATH_LENGTH]    = {0};

//------------------------------
public:
//------------------------------

  SAT_LogFile() {
  }

  //----------

  ~SAT_LogFile() {
  }

//------------------------------
public:
//------------------------------

  void initialize(const char* AName="sat_log.txt") {
    MEnabled = false;
    //MLogfileName = AName;
    SAT_File file = {};
    SAT_GetDesktopPath(MDesktopPath);
    strcat(MDesktopPath,"skei.audio");
    if (file.direxists(MDesktopPath)) {

      #ifdef SAT_LINUX
        sprintf(MFilename,"%s/%s",MDesktopPath,MLogfileName);
        //snprintf(MFilename,SAT_MAX_STRING_LENGTH,"%s/%s",MDesktopPath,MLogfileName);
      #else
        sprintf(MFilename,"%s\\%s",MDesktopPath,MLogfileName);
      #endif

      #ifdef SAT_LOG_FILE_APPEND
        MFile.open(MFilename,SAT_FILE_APPEND_TEXT);
      #else
        MFile.open(MFilename,SAT_FILE_WRITE_TEXT);
      #endif

      if (!MFile.isOpen()) {
        printf("Log: couldn't open logfile '%s'\n",MFilename);
      }
      else {
        MEnabled = true;
        print("\n----------------------------------------------------------------------\n\n");
        print_header();
        print("\n----------\n\n");
      }

    }
    else {
      //printf("Log: directory '%s' doesn't exist\n",MDesktopPath);
    }
  }

  //----------

  void cleanup(const char* AName="sat_log.txt") {
    if (MEnabled) {
      print("\n\n");
      MFile.close();
    }
  }

//------------------------------
public:
//------------------------------

  void print_header() {

    if (MEnabled) {

      SAT_CurrentTime time = {0};
      SAT_GetLocalTime(&time);

      //------------------------------
      //
      //------------------------------

      const char* gui = "";
      const char* painter = "";

      #ifdef SAT_NO_GUI
        gui = "SAT_NO_GUI";
      #endif

      #ifdef SAT_GUI_WIN32
        gui = "SAT_GUI_WIN32";
      #endif

      #ifdef SAT_GUI_XCB
        gui = "SAT_GUI_XCB";
      #endif

      #ifdef SAT_NO_PAINTER
        gui = "SAT_NO_PAINTER";
      #endif

      #ifdef SAT_PAINTER_GDI
        painter = "SAT_PAINER_GDI";
      #endif

      #ifdef SAT_PAINTER_GLX
        painter = "SAT_PAINER_GLX";
      #endif

      #ifdef SAT_PAINTER_NVG
        painter = "SAT_PAINER_NVG";
      #endif

      #ifdef SAT_PAINTER_WGL
        painter = "SAT_PAINER_WGL";
      #endif

      #ifdef SAT_PAINTER_XCB
        painter = "SAT_PAINER_XCB";
      #endif

      //------------------------------
      //
      //------------------------------

      print("Time: %02i.%02i.%04i (%02i.%02i.%02i)\n",time.day,time.month,time.year,time.hour,time.minutes,time.seconds);

      #ifdef SAT_DEBUG
        print("Build: Debug\n");
      #else
        print("Build: Release\n");
      #endif

      #ifdef SAT_LINUX
        print("OS: Linux\n");
      #endif

      #ifdef SAT_WIN32
        print("OS: Windows\n");
      #endif

      print("GUI: %s\n",gui);
      print("Painter: %s\n",painter);

      char buffer[SAT_MAX_PATH_LENGTH];
      print("Home path: %s\n",SAT_GetHomePath(buffer));
      print("Desktop path: %s\n",SAT_GetDesktopPath(buffer));
      print("Base filename: %s\n",SAT_GetBaseFilename(buffer));
      print("Base path: %s\n",SAT_GetBasePath(buffer));

      print("CLAP Version: %i.%i.%i\n",CLAP_VERSION_MAJOR,CLAP_VERSION_MINOR,CLAP_VERSION_REVISION);

      #ifdef SAT_PLUGIN_EXE
        print("Wrapper: EXE\n");
      #endif

      #ifdef SAT_PLUGIN_VST2
        print("Wrapper: VST2\n");
      #endif

      #ifdef SAT_PLUGIN_VST3
        print("Wrapper: VST3\n");
      #endif

      //------------------------------
      //
      //------------------------------

    } // enabled

  }

//------------------------------
public:
//------------------------------

  //void operator () (const char* format, ...) {}

  //----------

  void print_string(char* str) {
    if (MEnabled) {
      uint32_t len = strlen(str);
      MFile.write(str,len);
      MFile.flush();
    }
}

  //----------

  void print(const char* format, ...) {
    if (MEnabled) {
      if (MFile.isOpen()) {
        va_list args;
        va_start(args,format);
        vsprintf(MTempBuffer,format,args);
        va_end(args);
        print_string(MTempBuffer);
      }
    }
  }

  void print_if(bool cond, const char* format, ...) {
    if (MEnabled) {
      if (cond && MFile.isOpen()) {
        va_list args;
        va_start(args,format);
        vsprintf(MTempBuffer,format,args);
        va_end(args);
        print_string(MTempBuffer);
      }
    }
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------

//#else // SAT_LOGGING
//
//class SAT_LogFile {
//public:
//  SAT_LogFile(const char* AName) {}
//  ~SAT_LogFile() {}
//public:
//  void print_header() {}
//  void print_string(char* str) {}
//  void print(const char* format, ...) {}
//  void print_if(bool cond, const char* format, ...) {}
//};
//
//#endif // SAT_LOGGING

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//SAT_LogFile LOG = SAT_LogFile(SAT_LOGFILE_NAME);

//----------------------------------------------------------------------
#endif
