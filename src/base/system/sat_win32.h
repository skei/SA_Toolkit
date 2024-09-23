#ifndef sat_win32_included
#define sat_win32_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
// windows.h
//
//----------------------------------------------------------------------

#undef  _WIN32_WINNT
#define _WIN32_WINNT 0x0501

/*
  exclude APIs such as Cryptography, DDE, RPC, Shell, and Windows Sockets..
  If using gdiplus you cannot define WIN32_LEAN_AND_MEAN. Gdiplus needs some
  type definitions that are excluded by this macro.
*/

//#ifndef SAT_WIN32_GDIPLUS
#ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
#endif
//#endif

#include <windows.h>

//----------------------------------------------------------------------
//
// gdi
//
//----------------------------------------------------------------------

// see also:
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd145130%28v=vs.85%29.aspx

// missing raster ops
#define DSTCOPY   0x00AA0029
#define DSTERASE  0x00220326 // dest = dest & (~ src) :  DSna
//#define FORE_ROP3(ROP4) (0x00FFFFFF & (ROP4))
//#define BACK_ROP3(ROP4) (ROP3FromIndex (SwapROP3_SrcDst (BYTE ((ROP4) >> 24))))

//----------------------------------------------------------------------
//
// DllMain
//
//----------------------------------------------------------------------

/*
  if multiple DLLs all register a window class with the same name,
  the instance handle is used to tell them apart.
*/

//----------

//___SAT_UNUSED static
HINSTANCE SAT_GLOBAL_WIN32_INSTANCE = 0;

/*
  http://msdn.microsoft.com/en-us/library/ms682583%28v=vs.85%29.aspx
  http://stackoverflow.com/questions/4496233/which-is-called-first-dllmain-or-global-static-object-constructor
  Which is called first, DllMain() or global static object constructor?
  MSDN's DllMain documentation says:
  If your DLL is linked with the C run-time library (CRT), the entry point
  provided by the CRT calls the constructors and destructors for global and
  static C++ objects. Therefore, these restrictions for DllMain also apply to
  constructors and destructors and any code that is called from them.
  Since the code within DllMain may use the static objects, the static objects
  must be constructed before DllMain is run for DLL_PROCESS_ATTACH, and
  destroyed after it is run for DLL_PROCESS_DETACH.
*/

//----------

#ifdef SAT_PLUGIN

  //#warning "plugin"

  __SAT_EXTERNC
  BOOL APIENTRY
  DllMain(HINSTANCE hModule, DWORD reason, LPVOID lpReserved) {
    switch(reason) {

      case DLL_PROCESS_ATTACH:
        //SAT_PRINT("SAT_GLOBAL_WIN32_INSTANCE = hModule;\n");
        SAT_GLOBAL_WIN32_INSTANCE = hModule;
        //SAT_RegisterWindowClass(&s3_global_WinInstance);
        break;

      //case DLL_PROCESS_DETACH:
      //  //SAT_UnregisterWindowClass();
      //  break;

      //case DLL_THREAD_ATTACH:
      //  break;
      //case DLL_THREAD_DETACH:
      //  break;

      //default:
      //  break;

    }
    return TRUE;
  }

#endif // SAT_PLUGIN

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// #include <strsafe.h>

//void SAT_Win32Error(LPTSTR lpszFunction) {
void SAT_Win32PrintError(const char* txt) {

  // Retrieve the system error message for the last-error code

  DWORD dw = GetLastError();
  LPVOID lpMsgBuf;
  FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER |
    FORMAT_MESSAGE_FROM_SYSTEM |
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    dw,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR) &lpMsgBuf,
    0, NULL
  );

  // Display the error message and exit the process

  //  LPVOID lpDisplayBuf;
  //  lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
  //  StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s failed with error %d: %s"), lpszFunction, dw, lpMsgBuf);
  //  MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

  //SAT_PRINT("%s: %s\n",txt,lpMsgBuf);
  LocalFree(lpMsgBuf);

  //LocalFree(lpDisplayBuf);
  //ExitProcess(dw);

}

//----------------------------------------------------------------------
#endif
