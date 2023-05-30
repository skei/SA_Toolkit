#ifndef sat_ladspa_utils_included
#define sat_ladspa_utils_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/ladspa/sat_ladspa.h"

//----------------------------------------------------------------------
#endif









#if 0



struct KODE_LadspaPorts {
  int*                  descriptors  = nullptr;
  char**                names        = nullptr;
  char*                 namesBuffer  = nullptr;
  LADSPA_PortRangeHint* rangeHints   = nullptr;
};

//----------------------------------------------------------------------

// returns numports

uint32_t KODE_LadspaSetupPorts(KODE_Descriptor* ADescriptor, KODE_LadspaPorts* APorts) {
  uint32_t numin      = ADescriptor->getNumInputs();
  uint32_t numout     = ADescriptor->getNumOutputs();
  uint32_t numpar     = ADescriptor->getNumParameters();
  uint32_t numports   = numin + numout + numpar;
  APorts->descriptors = (LADSPA_PortDescriptor*)malloc(numports * sizeof(LADSPA_PortDescriptor));
  APorts->names       = (char**)malloc(numports * sizeof(char*));
  APorts->namesBuffer = (char*)malloc(numports * KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH);
  APorts->rangeHints  = (LADSPA_PortRangeHint*)malloc(numports * sizeof(LADSPA_PortRangeHint));
  uint32_t i = 0;
  uint32_t port = 0;
  for (i=0; i<numin; i++) {
    //const char* port_name = "Audio Input";
    APorts->descriptors[port] = LADSPA_PORT_AUDIO | LADSPA_PORT_INPUT;
    //char* ptr = MPortNamesBuffer + (port * KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH);
    //KODE_Strncpy(ptr,port_name,KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH-1);
    //ptr[KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH] = 0;
    APorts->names[port] = (char*)"input";
    APorts->rangeHints[port].HintDescriptor = 0;
    //MPortRangeHints[port].LowerBound = 0;
    //MPortRangeHints[port].UpperBound = 0;
    port++;
  }
  for (i=0; i<numout; i++) {
    //const char* port_name = "Audio Output";
    APorts->descriptors[port] = LADSPA_PORT_AUDIO | LADSPA_PORT_OUTPUT;
    //char* ptr = MPortNames + (port * KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH);
    //KODE_Strncpy(ptr,port_name,KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH-1);
    //ptr[KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH] = 0;
    APorts->names[port] = (char*)"output";
    APorts->rangeHints[port].HintDescriptor = 0;
    //MPortRangeHints[port].LowerBound = 0;
    //MPortRangeHints[port].UpperBound = 0;
    port++;
  }
  for (i=0; i<numpar; i++) {
    KODE_Parameter* param = ADescriptor->getParameter(i);
    APorts->descriptors[port] = LADSPA_PORT_CONTROL | LADSPA_PORT_INPUT;
    //const char* pname = param->getName();
    //const char* port_name = pname;
    //KODE_Trace("%i %s\n",i,port_name);
    char* ptr = APorts->namesBuffer + (port * KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH);
    strncpy(ptr,param->getName(),KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH-1);
    ptr[KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH] = 0;
    APorts->names[port] = ptr;//(char*)"param";
    // LADSPA_HINT_TOGGLED, LADSPA_HINT_SAMPLE_RATE, LADSPA_HINT_LOGARITHMIC
    // LADSPA_HINT_INTEGER,
    APorts->rangeHints[port].HintDescriptor = LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
    APorts->rangeHints[port].LowerBound = param->getMinValue();
    APorts->rangeHints[port].UpperBound = param->getMaxValue();
    port++;
  }
  return port;
}

//----------

void KODE_LadspaCleanupPorts(KODE_LadspaPorts* APorts) {
  if (APorts->descriptors)   free(APorts->descriptors);
  if (APorts->names)         free(APorts->names);
  if (APorts->namesBuffer)   free(APorts->namesBuffer);
  if (APorts->rangeHints)    free(APorts->rangeHints);
}





#endif // 0


