#ifndef sat_lv2_utils_included
#define sat_lv2_utils_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/lv2/sat_lv2.h"

//----------------------------------------------------------------------
#endif









#if 0


#include "kode.h"
#include "plugin/lv2/kode_lv2.h"

//----------------------------------------------------------------------

void KODE_Lv2PrintFeatures(const LV2_Feature* const* features) {
  uint32_t i = 0;
  while ( features[i] ) {
    const char* uri = features[i]->URI;
    KODE_DPrint("	 %i: %s\n",i,uri);
    i++;
  }
}

//----------

void* KODE_Lv2FindFeature(const char* name, const LV2_Feature* const* features) {
  for (int i=0; features[i]; ++i) {
    if (!strcmp(features[i]->URI, name)) {
      return features[i]->data;
    }
  }
  return nullptr;
}

//----------

LV2_URID KODE_Lv2MapUrid(const char* name, LV2_URID_Map* urid_map) {
  return urid_map->map(urid_map->handle, name);
}

//----------------------------------------------------------------------
//
// manifest
//
//----------------------------------------------------------------------

//#ifdef KODE_PLUGIN_LV2_DUMPTTL

void KODE_Lv2CreateManifest(KODE_Descriptor* descriptor, char* manifest_ttl, char* plugin_ttl/*, char* editor_ttl*/) {

  char temp[256] = {0};

  //------------------------------
  // manifest.ttl
  //------------------------------

  /*

    http://mountainbikesandtrombones.blogspot.com/2014/08/making-lv2-plugin-gui-yes-in-inkscape.html

    <http://infamousplugins.sourceforge.net/plugs.html#stuck>
      a lv2:Plugin, lv2:DelayPlugin ;
      lv2:binary <stuck.so> ;
      rdfs:seeAlso <stuck.ttl> .
    <http://infamousplugins.sourceforge.net/plugs.html#stuck_ui>
      a ui:X11UI;
      ui:binary <stuckui.so>
      lv2:extensionData ui:idle; .
  */

  if (manifest_ttl) {
    manifest_ttl[0] = 0;

    strcat(manifest_ttl,"@prefix lv2:  <http://lv2plug.in/ns/lv2core#> .\n");
    strcat(manifest_ttl,"@prefix rdf:  <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n");
    strcat(manifest_ttl,"@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .\n");
    #ifndef KODE_NO_GUI
    strcat(manifest_ttl,"@prefix ui:   <http://lv2plug.in/ns/extensions/ui#> .\n");
    #endif
    strcat(manifest_ttl,"\n");

    sprintf(temp,"<urn:%s/%s>\n",descriptor->getAuthor(),descriptor->getName());        //"urn:skei.audio/kode_debug";
    strcat(manifest_ttl,temp);
    strcat(manifest_ttl,"  a lv2:Plugin ;\n");                                     // , lv2:InstrumentPlugin ;\n");
    sprintf(temp,"  lv2:binary <%s.so>  ;\n",descriptor->getName());                    // TODO: this.filename?
    strcat(manifest_ttl,temp);
    sprintf(temp,"  rdfs:seeAlso <%s.ttl> .\n",descriptor->getName());
    strcat(manifest_ttl,temp);
    strcat(manifest_ttl,"\n");

    #ifndef KODE_NO_GUI
    sprintf(temp,"<urn:%s/%s_ui>\n",descriptor->getAuthor(),descriptor->getName()); //"urn:skei.audio/kode_debug_ui";
    strcat(manifest_ttl,temp);
    strcat(manifest_ttl,"  a ui:X11UI ;\n");
    sprintf(temp,"  ui:binary <%s.so>  .\n",descriptor->getName());                     // TODO: this.filename?
    strcat(manifest_ttl,temp);
    //strcat(manifest_ttl,"  lv2:extensionData ui:idle; .\n");
    strcat(manifest_ttl,"\n");
    #endif

  }

  //------------------------------
  // plugin.ttl
  //------------------------------

  if (plugin_ttl) {
    plugin_ttl[0] = 0;

    strcat(plugin_ttl,"@prefix doap:   <http://usefulinc.com/ns/doap#> .\n");
    strcat(plugin_ttl,"@prefix lv2:    <http://lv2plug.in/ns/lv2core#> .\n");
    strcat(plugin_ttl,"@prefix rdf:    <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n");
    strcat(plugin_ttl,"@prefix rdfs:   <http://www.w3.org/2000/01/rdf-schema#> .\n");
    strcat(plugin_ttl,"@prefix atom:   <http://lv2plug.in/ns/ext/atom#> .\n");
    strcat(plugin_ttl,"@prefix midi:   <http://lv2plug.in/ns/ext/midi#> .\n");
    #ifndef KODE_NO_GUI
    strcat(plugin_ttl,"@prefix ui:     <http://lv2plug.in/ns/extensions/ui#> .\n");
    #endif
    strcat(plugin_ttl,"@prefix urid:   <http://lv2plug.in/ns/ext/urid#> .\n");
    strcat(plugin_ttl,"\n");

    sprintf(temp,"<urn:%s/%s>\n",descriptor->getAuthor(),descriptor->getName()); // "urn:skei.audio/kode_debug";
    strcat(plugin_ttl,temp);
    strcat(plugin_ttl,"  a lv2:Plugin ;\n"); // , lv2:InstrumentPlugin ;\n");
    sprintf(temp,"  doap:name           \"%s\" ;\n",descriptor->getName());
    strcat(plugin_ttl,temp);
    strcat(plugin_ttl,"  lv2:optionalFeature lv2:hardRTCapable ;\n");
    strcat(plugin_ttl,"  lv2:requiredFeature urid:map ;\n");

    #ifndef KODE_NO_GUI
    // editor?
    #endif

    //----------
    // ports
    //----------

    uint32_t  numin     = descriptor->getNumInputs();
    uint32_t  numout    = descriptor->getNumOutputs();
    uint32_t  numpar    = descriptor->getNumParameters();
    uint32_t  numports  = numin + numout + numpar;
    if (descriptor->canReceiveMidi()) { numports += 1; }
    uint32_t  i = 0;
    uint32_t  p = 0;
    char      symbol[64];
    char      name[64];

    // every port symbol must be unique and a valid C identifier
    // and the indices must start at 0 and be contiguous

    if (numports > 0) {
      strcat(plugin_ttl,"  lv2:port [\n");

      //----------
      // audio inputs
      //----------

      for (i=0; i<numin; i++) {
        sprintf(name,"Input %i",i);
        strcpy(symbol,name);
//        KODE_MakeValidSymbol(symbol);
        strcat(plugin_ttl,"    a lv2:AudioPort , lv2:InputPort ;\n");
        sprintf(temp,     "    lv2:index         %i ;\n",p);
        strcat(plugin_ttl,temp);
        sprintf(temp,     "    lv2:symbol        \"%s\" ;\n",symbol);
        strcat(plugin_ttl,temp);
        sprintf(temp,     "    lv2:name          \"%s\" ;\n",name);
        strcat(plugin_ttl,temp);
        p++;
        if (p < numports) strcat(plugin_ttl,"	] , [\n");
        else strcat(plugin_ttl," ] .\n");
      } // numin

      //----------
      // audio outputs
      //----------

      for (i=0; i<numout; i++) {
        sprintf(name,"Output %i",i);
        strcpy(symbol,name);
//        KODE_MakeValidSymbol(symbol);
        strcat(plugin_ttl,"    a lv2:AudioPort , lv2:OutputPort ;\n");
        sprintf(temp,     "    lv2:index         %i ;\n",p);
        strcat(plugin_ttl,temp);
        sprintf(temp,     "    lv2:symbol        \"%s\" ;\n",symbol);
        strcat(plugin_ttl,temp);
        sprintf(temp,     "    lv2:name          \"%s\" ;\n",name);
        strcat(plugin_ttl,temp);
        p++;
        if (p < numports) strcat(plugin_ttl,"  ] , [\n");
        else strcat(plugin_ttl," ] .\n");
      } // numout

      //----------
      // parameters
      //----------

      for (i=0; i<numpar; i++) {
        KODE_Parameter* par = descriptor->getParameter(i);
        strcpy(name,par->getName());
        strcpy(symbol,name);
//        KODE_MakeValidSymbol(symbol);
        strcat(plugin_ttl,"    a lv2:InputPort , lv2:ControlPort ;\n");
        sprintf(temp,     "    lv2:index         %i ;\n",p);
        strcat(plugin_ttl,temp);
        sprintf(temp,     "    lv2:symbol        \"%s\" ;\n",symbol);
        strcat(plugin_ttl,temp);
        sprintf(temp,     "    lv2:name          \"%s\" ;\n",name);
        strcat(plugin_ttl,temp);
        sprintf(temp,     "    lv2:default       %f ;\n",    par->getDefValue());
        strcat(plugin_ttl,temp);
        sprintf(temp,     "    lv2:minimum       %f ;\n",    par->getMinValue());
        strcat(plugin_ttl,temp);
        sprintf(temp,     "    lv2:maximum       %f ;\n",    par->getMaxValue());
        strcat(plugin_ttl,temp);
        p++;
        if (p < numports) strcat(plugin_ttl," ] , [\n");
        else strcat(plugin_ttl," ] .\n");
      } // numpar

      //----------
      // midi input
      //----------

      if (descriptor->canReceiveMidi()) {
        //strcat(plugin_ttl,"    a lv2:InputPort , atom:AtomPort ;\n");
        strcat(plugin_ttl,"    a atom:AtomPort , lv2:InputPort ;\n");
        strcat(plugin_ttl,"    atom:bufferType   atom:Sequence ;\n");
        strcat(plugin_ttl,"    atom:supports     midi:MidiEvent ;\n");
        strcat(plugin_ttl,"    lv2:designation   lv2:control ;\n");
        sprintf(temp,     "    lv2:index         %i ;\n",p);
        strcat(plugin_ttl,temp);
        strcat(plugin_ttl,"    lv2:symbol        \"midi_in\" ;\n");
        strcat(plugin_ttl,"    lv2:name          \"Midi in\" ;\n");
        p++;
        if (p < numports) strcat(plugin_ttl," ] , [\n");
        else strcat(plugin_ttl," ] .\n");
      } // midi in

    } // ports

    //----------

    strcat(plugin_ttl,"\n");

  }

  //------------------------------
  // editor.ttl
  //------------------------------

  // ???

}

//----------

void KODE_Lv2WriteManifest(KODE_Descriptor* ADescriptor) {
  char* manifest_ttl  = (char*)malloc(KODE_LV2_MANIFEST_TTL_SIZE);
  char* plugin_ttl    = (char*)malloc(KODE_LV2_PLUGIN_TTL_SIZE);
  KODE_Lv2CreateManifest(ADescriptor,manifest_ttl,plugin_ttl);
  FILE* fp = nullptr;
  fp = fopen("manifest.ttl","w");
  if (fp) fprintf(fp,"%s",manifest_ttl);
  char buffer[KODE_MAX_PATH_LENGTH] = {0};
  buffer[0] = 0;
  strcat(buffer,ADescriptor->getName());
  strcat(buffer,".ttl");
  fp = fopen(buffer,"w");
  if (fp) fprintf(fp,"%s",plugin_ttl);
  free(manifest_ttl);
  free(plugin_ttl);
}


#endif // 0


