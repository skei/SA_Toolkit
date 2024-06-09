#ifndef sat_linux_dialogs_included
#define sat_linux_dialogs_included
//----------------------------------------------------------------------

#include "base/utils/sat_convert.h"

// uses zenity
// gnome?

static char sat_zenity_buffer[1024];
static char sat_zenity_filename[1024];

//----------

const char* sat_do_zenity(void) {
  FILE *f = popen(sat_zenity_buffer,"r");
  if ( fgets(sat_zenity_filename,1024,f) ) {
    return sat_zenity_filename;
  }
  else {
    return "";//nullptr;
  }
}

//----------

const char* SAT_Dialog_ColorSelect(const char* ATitle, bool APalette=true) {
  strcpy(sat_zenity_buffer,"zenity --color-selection");
  strcat(sat_zenity_buffer," --title='");    strcat(sat_zenity_buffer,ATitle);    strcat(sat_zenity_buffer,"'");
  if (APalette) strcat(sat_zenity_buffer," --show-palette");
  return sat_do_zenity();
}

const char* Dialog_Error(const char* ATitle, const char* AText) {
  strcpy(sat_zenity_buffer,"zenity --error");
  strcat(sat_zenity_buffer," --text='");    strcat(sat_zenity_buffer,AText);    strcat(sat_zenity_buffer,"'");
  return sat_do_zenity();
}

const char* SAT_Dialog_FileOpen(const char* ATitle, bool AMultiple=false) {
  strcpy(sat_zenity_buffer,"zenity --file-selection");
  strcat(sat_zenity_buffer," --title='");    strcat(sat_zenity_buffer,ATitle);    strcat(sat_zenity_buffer,"'");
  if (AMultiple) {
    strcat(sat_zenity_buffer," --multiple");
    strcat(sat_zenity_buffer," --separator=':'");
  }
  return sat_do_zenity();
}

const char* SAT_Dialog_FileSave(const char* ATitle, const char* AFilename) {
  strcpy(sat_zenity_buffer,"zenity --file-selection");
  strcat(sat_zenity_buffer," --save");
  strcat(sat_zenity_buffer," --title='");    strcat(sat_zenity_buffer,ATitle);    strcat(sat_zenity_buffer,"'");
  strcat(sat_zenity_buffer," --filename='"); strcat(sat_zenity_buffer,AFilename); strcat(sat_zenity_buffer,"'");
  return sat_do_zenity();
}

const char* SAT_Dialog_Info(const char* ATitle, const char* AText) {
  strcpy(sat_zenity_buffer,"zenity --info");
  strcat(sat_zenity_buffer," --text='");    strcat(sat_zenity_buffer,AText);    strcat(sat_zenity_buffer,"'");
  return sat_do_zenity();
}

const char* SAT_Dialog_Question(const char* ATitle, const char* AText) {
  strcpy(sat_zenity_buffer,"zenity --question");
  strcat(sat_zenity_buffer," --text='");    strcat(sat_zenity_buffer,AText);    strcat(sat_zenity_buffer,"'");
  return sat_do_zenity();
}

const char* SAT_Dialog_Text(const char* ATitle, const char* AText, const char* AEntryText) {
  strcpy(sat_zenity_buffer,"zenity --entry");
  strcat(sat_zenity_buffer," --text='");    strcat(sat_zenity_buffer,AText);            strcat(sat_zenity_buffer,"'");
  strcat(sat_zenity_buffer," --entry-text='");    strcat(sat_zenity_buffer,AEntryText); strcat(sat_zenity_buffer,"'");
  return sat_do_zenity();
}

// segmentation fault

const char* SAT_Dialog_TextInfo(const char* ATitle, const char* AFilename, bool AEditable=false, bool AHtml=false, const char* AUrl=nullptr) {
  strcpy(sat_zenity_buffer,"zenity --text-info");
  strcat(sat_zenity_buffer," --filename='");       strcat(sat_zenity_buffer,AFilename);   strcat(sat_zenity_buffer,"'");
  if (AEditable) strcat(sat_zenity_buffer," --editable");
  if (AHtml) strcat(sat_zenity_buffer," --html");
  if (AUrl) {
    strcat(sat_zenity_buffer," --url='");
    strcat(sat_zenity_buffer,AUrl);
    strcat(sat_zenity_buffer,"'");
  }
  return sat_do_zenity();
}

const char* SAT_Dialog_Value(const char* ATitle, const char* AText, int AVal=0, int AMin=0, int AMax=100, int AStep=1) {
  char temp[256];
  strcpy(sat_zenity_buffer,"zenity --scale");
  strcat(sat_zenity_buffer," --text='");       strcat(sat_zenity_buffer,AText); strcat(sat_zenity_buffer,"'");
  strcat(sat_zenity_buffer," --value='");      strcat(sat_zenity_buffer, SAT_IntToString(temp,AVal));  strcat(sat_zenity_buffer,"'");
  strcat(sat_zenity_buffer," --min-value='");  strcat(sat_zenity_buffer, SAT_IntToString(temp,AMin));  strcat(sat_zenity_buffer,"'");
  strcat(sat_zenity_buffer," --max-value='");  strcat(sat_zenity_buffer, SAT_IntToString(temp,AMax));  strcat(sat_zenity_buffer,"'");
  strcat(sat_zenity_buffer," --step='");       strcat(sat_zenity_buffer, SAT_IntToString(temp,AStep)); strcat(sat_zenity_buffer,"'");
  //strcat(sat_zenity_buffer," --print-partial");
  //strcat(sat_zenity_buffer," --hide-value");
  return sat_do_zenity();
}

const char* SAT_Dialog_Warning(const char* ATitle, const char* AText) {
  strcpy(sat_zenity_buffer,"zenity --warning");
  strcat(sat_zenity_buffer," --text='");    strcat(sat_zenity_buffer,AText);    strcat(sat_zenity_buffer,"'");
  return sat_do_zenity();
}

//----------------------------------------------------------------------
#endif
