#ifndef sat_win32_dialogs_included
#define sat_win32_dialogs_included
//----------------------------------------------------------------------

#include "base/utils/sat_convert.h"

//----------------------------------------------------------------------
// win32
//----------------------------------------------------------------------

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645524%28v=vs.85%29.aspx
#ifdef SAT_WIN32

//TODO?

#include <windows.h>
#include <commdlg.h>

// link with comdlg32

//----------

const char* SAT_Dialog_ColorSelect(const char* ATitle, bool APalette=true) {
  CHOOSECOLOR choose_color;
  COLORREF custom_colors[16];
  ZeroMemory(&choose_color, sizeof(choose_color));
  choose_color.lStructSize = sizeof (CHOOSECOLOR);
  choose_color.rgbResult = RGB(0,0,0);//old_attr;
  choose_color.lpCustColors = custom_colors;
  choose_color.Flags = CC_ANYCOLOR | CC_RGBINIT | CC_FULLOPEN;
  if (ChooseColor(&choose_color) == TRUE) {
    SAT_Trace("%08x\n",choose_color.rgbResult); // ABGR
    //return (int) cc.rgbResult;
  } else {
    SAT_Trace("---\n");
    //return -1;
  }
  return nullptr;
}

const char* SAT_Dialog_Error(const char* ATitle, const char* AText) {
//  strcpy(sat_zenity_buffer,"zenity --error");
//  strcat(sat_zenity_buffer," --text='");    strcat(sat_zenity_buffer,AText);    strcat(sat_zenity_buffer,"'");
//  return sat_do_zenity();
  return nullptr;
}

const char* SDialog_FileOpen(const char* ATitle, bool AMultiple=false) {
//  strcpy(sat_zenity_buffer,"zenity --file-selection");
//  strcat(sat_zenity_buffer," --title='");    strcat(sat_zenity_buffer,ATitle);    strcat(sat_zenity_buffer,"'");
//  if (AMultiple) {
//    strcat(sat_zenity_buffer," --multiple");
//    strcat(sat_zenity_buffer," --separator=':'");
//  }
//  return sat_do_zenity();
  return nullptr;
}

const char* SAT_Dialog_FileSave(const char* ATitle, const char* AFilename) {
//  strcpy(sat_zenity_buffer,"zenity --file-selection");
//  strcat(sat_zenity_buffer," --save");
//  strcat(sat_zenity_buffer," --title='");    strcat(sat_zenity_buffer,ATitle);    strcat(sat_zenity_buffer,"'");
//  strcat(sat_zenity_buffer," --filename='"); strcat(sat_zenity_buffer,AFilename); strcat(sat_zenity_buffer,"'");
//  return sat_do_zenity();
  return nullptr;
}

const char* SAT_Dialog_Info(const char* ATitle, const char* AText) {
//  strcpy(sat_zenity_buffer,"zenity --info");
//  strcat(sat_zenity_buffer," --text='");    strcat(sat_zenity_buffer,AText);    strcat(sat_zenity_buffer,"'");
//  return sat_do_zenity();
  return nullptr;
}

const char* SAT_Dialog_Question(const char* ATitle, const char* AText) {
//  strcpy(sat_zenity_buffer,"zenity --question");
//  strcat(sat_zenity_buffer," --text='");    strcat(sat_zenity_buffer,AText);    strcat(sat_zenity_buffer,"'");
//  return sat_do_zenity();
  return nullptr;
}

const char* SAT_Dialog_Text(const char* ATitle, const char* AText, const char* AEntryText) {
//  strcpy(sat_zenity_buffer,"zenity --entry");
//  strcat(sat_zenity_buffer," --text='");    strcat(sat_zenity_buffer,AText);            strcat(sat_zenity_buffer,"'");
//  strcat(sat_zenity_buffer," --entry-text='");    strcat(sat_zenity_buffer,AEntryText); strcat(sat_zenity_buffer,"'");
//  return sat_do_zenity();
  return nullptr;
}

// segmentation fault

const char* SAT_Dialog_TextInfo(const char* ATitle, const char* AFilename, bool AEditable=false, bool AHtml=false, const char* AUrl=nullptr) {
//  strcpy(sat_zenity_buffer,"zenity --text-info");
//  strcat(sat_zenity_buffer," --filename='");       strcat(sat_zenity_buffer,AFilename);   strcat(sat_zenity_buffer,"'");
//  if (AEditable) strcat(sat_zenity_buffer," --editable");
//  if (AHtml) strcat(sat_zenity_buffer," --html");
//  if (AUrl) strcat(sat_zenity_buffer," --url='");  strcat(sat_zenity_buffer,AUrl);        strcat(sat_zenity_buffer,"'");
//  return sat_do_zenity();
  return nullptr;
}

const char* SAT_Dialog_Value(const char* ATitle, const char* AText, int AVal=0, int AMin=0, int AMax=100, int AStep=1) {
//  char temp[256];
//  strcpy(sat_zenity_buffer,"zenity --scale");
//  strcat(sat_zenity_buffer," --text='");       strcat(sat_zenity_buffer,AText); strcat(sat_zenity_buffer,"'");
//  strcat(sat_zenity_buffer," --value='");      strcat(sat_zenity_buffer, SIntToString(temp,AVal));  strcat(sat_zenity_buffer,"'");
//  strcat(sat_zenity_buffer," --min-value='");  strcat(sat_zenity_buffer, SIntToString(temp,AMin));  strcat(sat_zenity_buffer,"'");
//  strcat(sat_zenity_buffer," --max-value='");  strcat(sat_zenity_buffer, SIntToString(temp,AMax));  strcat(sat_zenity_buffer,"'");
//  strcat(sat_zenity_buffer," --step='");       strcat(sat_zenity_buffer, SIntToString(temp,AStep)); strcat(sat_zenity_buffer,"'");
//  //strcat(sat_zenity_buffer," --print-partial");
//  //strcat(sat_zenity_buffer," --hide-value");
//  return sat_do_zenity();
  return nullptr;
}

const char* SAT_Dialog_Warning(const char* ATitle, const char* AText) {
//  strcpy(sat_zenity_buffer,"zenity --warning");
//  strcat(sat_zenity_buffer," --text='");    strcat(sat_zenity_buffer,AText);    strcat(sat_zenity_buffer,"'");
//  return sat_do_zenity();
  return nullptr;
}

//----------------------------------------------------------------------
#endif

