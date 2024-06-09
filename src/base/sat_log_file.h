#ifndef sat_log_file_included
#define sat_log_file_included
//----------------------------------------------------------------------

//#define SAT_LOG_TIME
//#define SAT_LOG_THREAD

//----------

#ifdef SAT_LOG_THREAD
  #ifdef SAT_LINUX
    #include <sys/syscall.h>
    #include <sys/unistd.h>
    #define gettid() syscall(SYS_gettid)
  #endif
  #ifdef SAT_WIN32
    #include <processthreadsapi.h>
  #endif
#endif

#ifdef SAT_LOG_TIME
  #include <sys/time.h> // gettimeofday
#endif

//#include "sat.h"
#include "base/sat_print.h"
#include "base/system/sat_cpu.h"
#include "base/system/sat_file.h"
#include "base/system/sat_paths.h"
#include "base/system/sat_time.h"
#include "plugin/lib/sat_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_LogFile {

//------------------------------
private:
//------------------------------

  SAT_Print*  MPrint                            = nullptr;
  SAT_File    MFile                             = {};
  char        MLogfileName[SAT_MAX_PATH_LENGTH] = {0};
  bool        MEnabled                          = false;
  char        MDesktopPath[SAT_MAX_PATH_LENGTH] = {0};
  char        MFilename[SAT_MAX_PATH_LENGTH*2]  = {0};
  char        MTempBuffer[SAT_MAX_PATH_LENGTH]  = {0};
  //char      MHomePath[SAT_MAX_PATH_LENGTH]    = {0};

  #ifdef SAT_LOG_TIME
    double MStartTime = 0.0;
  #endif

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

  bool initialize(SAT_Print* APrint) {
    //APrint->print("SAT_Log.initialize\n");
    MPrint = APrint;
    #ifdef SAT_INCLUDE_LOG

      #ifdef SAT_LOG_TIME
        time_start();
      #endif

      MEnabled = false;
      //MLogfileName = AName;
      SAT_File file = {};
      SAT_GetDesktopPath(MDesktopPath);
      strcat(MDesktopPath,SAT_LOG_DIRECTORY);
      if (file.direxists(MDesktopPath)) {
        SAT_CurrentTime time = {0};
        SAT_GetLocalTime(&time);
        sprintf(MLogfileName,"%02i-%s-%04i_%02i-%02i-%02i",time.day, SAT_MONTH_NAMES[time.month - 1], time.year, time.hour, time.minutes, time.seconds);
        #ifdef SAT_LINUX
          sprintf(MFilename,"%s/%s",MDesktopPath,MLogfileName);
        #else
          sprintf(MFilename,"%s\\%s",MDesktopPath,MLogfileName);
        #endif
        APrint->print("logfile: %s path: %s\n",MLogfileName,MFilename);
        MFile.open(MFilename,SAT_FILE_WRITE_TEXT);
        if (!MFile.isOpen()) {
          APrint->print("Log: couldn't open logfile '%s'\n",MFilename);
        }
        else {
          MEnabled = true;
          dprint("\n----------------------------------------------------------------------\n\n");
          print_header();
          dprint("\n----------\n\n");
        }
      }
      else {
        //APrint->print("Log: directory '%s' doesn't exist\n",MDesktopPath);
      }
    #else // no_log
    #endif

    return true;
  }

  //----------

  void cleanup() {
    #ifdef SAT_INCLUDE_LOG
      if (MEnabled) {
//        dprint("\n\n");
        MFile.close();
      }
    #else
    #endif
  }

//------------------------------
private: // time
//------------------------------

  #ifdef SAT_LOG_TIME

    void time_start() {
      struct timeval time;
      gettimeofday(&time,NULL);
      MStartTime = (double)time.tv_sec + (double)time.tv_usec * .000001;
    }

    //----------

    double time_elapsed() {
      struct timeval time;
      gettimeofday(&time,NULL);
      double currenttime = (double)time.tv_sec + (double)time.tv_usec * .000001;
      double elapsed = currenttime - MStartTime;
      return elapsed;
    }

  #endif // log time

//------------------------------
public:
//------------------------------

  void print_header() {
    if (MEnabled) {

      SAT_CurrentTime time = {0};
      SAT_GetLocalTime(&time);

      const char* gui = "";
      const char* painter = "";

// SAT_NO_GUI
// SAT_GUI_SOFTWARE
// SAT_GUI_WAYLAND
// SAT_GUI_WIN32
// SAT_GUI_X11

      #ifdef SAT_NO_GUI
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

      dprint("Time: %02i.%02i.%04i (%02i.%02i.%02i)\n",time.day,time.month,time.year,time.hour,time.minutes,time.seconds);

      #ifdef SAT_DEBUG
        dprint("Build: Debug\n");
      #else
        dprint("Build: Release\n");
      #endif

      #ifdef SAT_LINUX
        dprint("OS: Linux\n");
      #endif
      #ifdef SAT_WIN32
        dprint("OS: Windows\n");
      #endif

      dprint("GUI: %s\n",gui);
      char buffer[SAT_MAX_PATH_LENGTH];
      dprint("Home path: %s\n",SAT_GetHomePath(buffer));
      dprint("Desktop path: %s\n",SAT_GetDesktopPath(buffer));
      dprint("Base filename: %s\n",SAT_GetBaseFilename(buffer));
      dprint("Base path: %s\n",SAT_GetBasePath(buffer));
      dprint("CLAP Version: %i.%i.%i\n",CLAP_VERSION_MAJOR,CLAP_VERSION_MINOR,CLAP_VERSION_REVISION);
      //dprint("Host name: %s\n",SAT_GetHostName(buffer));

      #ifdef SAT_PLUGIN_DSSI
        dprint("Wrapper: DSSI\n");
      #endif
      #ifdef SAT_PLUGIN_EXE
        dprint("Wrapper: EXE\n");
      #endif
      #ifdef SAT_PLUGIN_LADSPA
        dprint("Wrapper: LADSPA\n");
      #endif
      #ifdef SAT_PLUGIN_LV2
        dprint("Wrapper: LV2\n");
      #endif
      #ifdef SAT_PLUGIN_VST2
        dprint("Wrapper: VST2\n");
      #endif
      #ifdef SAT_PLUGIN_VST3
        dprint("Wrapper: VST3\n");
      #endif

    } // enabled
  }

//------------------------------
public:
//------------------------------

  //TODO: prefix, time..

  //----------

  //void operator () (const char* format, ...) {
  //}

  //----------

  void print_string(char* str, bool prefix=true) {
    if (MEnabled) {

      if (prefix) {
      
        #if defined(SAT_LOG_TIME) || defined (SAT_LOG_THREAD)
        
          char temp[1024];
          temp[0] = 0;

          #if defined(SAT_LINUX) || defined(SAT_MAC)
            uint32_t thread_id = gettid();
          #endif
          #ifdef SAT_WIN32
            uint32_t thread_id = GetCurrentThreadId();
          #endif

          #ifdef SAT_LOG_TIME
            double time = time_elapsed();
            #ifdef SAT_LOG_THREAD // time & thread_id
              #ifdef SAT_MAC
                snprintf(temp,sizeof(temp),"[%.6f,%08x] %s",time,thread_id,str);
              #else
                sprintf(temp,"[%.6f,%08x] %s",time,thread_id,str);
              #endif
            #else // time 
              sprintf(temp,"[%.6f] %s",time,str);
            #endif
          #else // thread_id
            sprintf(temp,"[%08x] %s",thread_id,str);
          #endif

          uint32_t len = strlen(temp);
          MFile.write(temp,len);

        #else // none

          //sprintf(temp,"%s",str);
          uint32_t len = strlen(str);
          MFile.write(str,len);


        #endif

      }

      else { //!prefix
        uint32_t len = strlen(str);
        MFile.write(str,len);
      }

      MFile.flush();

    }
  }

  //----------

  void print(const char* format, ...) {
    if (MEnabled) {
      if (MFile.isOpen()) {
        va_list args;
        va_start(args,format);
        #ifdef SAT_MAC
          vsnprintf(MTempBuffer,sizeof(MTempBuffer),format,args);
        #else
          vsprintf(MTempBuffer,format,args);
        #endif
        va_end(args);
        print_string(MTempBuffer);
      }
    }
  }

  // void print_if(bool cond, const char* format, ...) {
  //   if (MEnabled) {
  //     if (cond && MFile.isOpen()) {
  //       va_list args;
  //       va_start(args,format);
  //       vsprintf(MTempBuffer,format,args);
  //       va_end(args);
  //       print_string(MTempBuffer);
  //     }
  //   }
  // }

  //----------

  void dprint(const char* format, ...) {
    if (MEnabled) {
      if (MFile.isOpen()) {
        va_list args;
        va_start(args,format);
        #ifdef SAT_MAC
          vsnprintf(MTempBuffer,sizeof(MTempBuffer),format,args);
        #else
          vsprintf(MTempBuffer,format,args);
        #endif
        va_end(args);
        print_string(MTempBuffer,false);
      }
    }
  }

};

//----------------------------------------------------------------------
#endif
