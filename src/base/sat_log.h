#ifndef sat_log_included
#define sat_log_included
//----------------------------------------------------------------------

//#include "sat.h"
#include "base/debug/sat_debug_print.h"
#include "base/system/sat_file.h"
#include "base/system/sat_paths.h"
#include "base/system/sat_time.h"
#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Log {

//------------------------------
private:
//------------------------------

  SAT_File    MFile                             = {};
  char        MLogfileName[SAT_MAX_PATH_LENGTH] = {0};
  bool        MEnabled                          = false;
  char        MDesktopPath[SAT_MAX_PATH_LENGTH] = {0};
  char        MFilename[SAT_MAX_PATH_LENGTH*2]  = {0};
  char        MTempBuffer[SAT_MAX_PATH_LENGTH]  = {0};
  //char      MHomePath[SAT_MAX_PATH_LENGTH]    = {0};

//------------------------------
public:
//------------------------------

  SAT_Log() {
    #if !defined (SAT_NO_LOG)
    #else
    #endif
  }

  //----------

  ~SAT_Log() {
    #if !defined (SAT_NO_LOG)
    #else
    #endif
  }

//------------------------------
public:
//------------------------------

  void initialize(SAT_DebugPrint* APrint) {
    #if !defined (SAT_NO_LOG)
      MEnabled = false;
      //MLogfileName = AName;
      SAT_File file = {};
      SAT_GetDesktopPath(MDesktopPath);
      strcat(MDesktopPath,SAT_LOG_DIRECTORY);
      if (file.direxists(MDesktopPath)) {
        SAT_CurrentTime time = {0};
        SAT_GetLocalTime(&time);
        sprintf(MLogfileName,"%02i-%s-%04i_%02i-%02i-%02i",time.day, SAT_MONTH_NAMES[time.month], time.year, time.hour, time.minutes, time.seconds);
        #ifdef SAT_LINUX
          sprintf(MFilename,"%s/%s",MDesktopPath,MLogfileName);
        #else
          sprintf(MFilename,"%s\\%s",MDesktopPath,MLogfileName);
        #endif
        printf("logfile: %s path: %s\n",MLogfileName,MFilename);
        MFile.open(MFilename,SAT_FILE_WRITE_TEXT);
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
    #else // no_log
    #endif
  }

  //----------

  void cleanup() {
    #if !defined (SAT_NO_LOG)
      if (MEnabled) {
        print("\n\n");
        MFile.close();
      }
    #else
    #endif
  }

//------------------------------
public:
//------------------------------

  void print_header() {
    if (MEnabled) {
      SAT_CurrentTime time = {0};
      SAT_GetLocalTime(&time);
      //
      const char* gui = "";
      const char* painter = "";


// SAT_GUI_NONE
// SAT_GUI_SOFTWARE
// SAT_GUI_WAYLAND
// SAT_GUI_WIN32
// SAT_GUI_X11

      #ifdef SAT_GUI_NONE
        gui = "NONE";
      #endif
      #ifdef SAT_GUI_SOFTWARE
        gui = "SOFTWARE";
      #endif
      #ifdef SAT_GUI_WAYLAND
        gui = "WAYLAND";
      #endif
      #ifdef SAT_GUI_WIN32
        gui = "WIN32";
      #endif
      #ifdef SAT_GUI_X11
        gui = "X11";
      #endif

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

    } // enabled
  }

//------------------------------
public:
//------------------------------

  //void operator () (const char* format, ...) {
  //}

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

};

//----------------------------------------------------------------------
#endif
