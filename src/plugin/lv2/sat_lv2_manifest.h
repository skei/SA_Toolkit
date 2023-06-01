#ifndef sat_lv2_manifest_included
#define sat_lv2_manifest_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/sat_host_implementation.h"
#include "plugin/lv2/sat_lv2.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Lv2Manifest {
  
//------------------------------
private:
//------------------------------

  SAT_HostImplementation*         host            = nullptr;
  
  //const clap_host_t*              clap_host       = nullptr;
  //const clap_plugin_descriptor_t* clap_descriptor = nullptr;
  const clap_plugin_t*            clap_plugin     = nullptr;
  
  char* manifest_ttl  = (char*)malloc(SAT_LV2_MANIFEST_TTL_SIZE); // 64k
  char* plugin_ttl    = (char*)malloc(SAT_LV2_PLUGIN_TTL_SIZE);   // 64k

//------------------------------
public:
//------------------------------

  SAT_Lv2Manifest() {
    host          = new SAT_HostImplementation();
    //clap_host     = host->getHost();
    manifest_ttl  = (char*)malloc(SAT_LV2_MANIFEST_TTL_SIZE);
    plugin_ttl    = (char*)malloc(SAT_LV2_PLUGIN_TTL_SIZE);
  }
  
  //----------
  
  ~SAT_Lv2Manifest() {
    if (host) delete host;
    if (manifest_ttl) delete manifest_ttl;
    if (plugin_ttl) delete plugin_ttl;
  }
  
//------------------------------
public:
//------------------------------

  void export_ttl() {
  }

//------------------------------
private:
//------------------------------

  //----------

  void create_manifest_ttl() {
    char temp[256] = {0};
    memset(manifest_ttl,0,SAT_LV2_MANIFEST_TTL_SIZE);
    strcat(manifest_ttl,"@prefix lv2:  <http://lv2plug.in/ns/lv2core#> .\n");
    strcat(manifest_ttl,"@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .\n");
    #ifndef SAT_NO_GUI
      //strcat(manifest_ttl,"@prefix ui:   <http://lv2plug.in/ns/extensions/ui#> .\n");
    #endif
    strcat(manifest_ttl,"\n");
    uint32_t num_plugins = SAT_GLOBAL.REGISTRY.getNumDescriptors();
    //SAT_HostImplementation* host = new SAT_HostImplementation();
    //const clap_host_t* clap_host = host->getHost();
    for (uint32_t i=0; i<num_plugins; i++) {
      const clap_plugin_descriptor_t* clap_descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(i);
      //const clap_plugin_t* clap_plugin = SAT_CreatePlugin(i,clap_descriptor,clap_host);
      sprintf(temp,"<urn:%s>\n",clap_descriptor->id); // "urn:skei.audio/test_plugin";
      strcat(manifest_ttl,temp);
      strcat(manifest_ttl,"  a lv2:Plugin ;\n"); // , lv2:InstrumentPlugin ;\n");
      sprintf(temp,"  lv2:binary <%s.so>  ;\n",clap_descriptor->name); // TODO: this.filename?
      strcat(manifest_ttl,temp);
      sprintf(temp,"  rdfs:seeAlso <%s.ttl> .\n",clap_descriptor->name);
      strcat(manifest_ttl,temp);
      strcat(manifest_ttl,"\n");
      //clap_plugin->destroy(clap_plugin);
      #ifndef SAT_NO_GUI
        //sprintf(temp,"<urn:%s_ui>\n",clap_descriptor->id);                        //"urn:skei.audio/sat_debug_ui";
        //strcat(manifest_ttl,temp);
        //strcat(manifest_ttl,"  a ui:X11UI ;\n");
        //sprintf(temp,"  ui:binary <%s.so>  .\n",clap_descriptor->name);           // TODO: this.filename?
        //strcat(manifest_ttl,temp);
        ////strcat(manifest_ttl,"  lv2:extensionData ui:idle; .\n");
        //strcat(manifest_ttl,"\n");
      #endif
    }
    //delete host;
    FILE* fp = nullptr;
    fp = fopen("manifest.ttl","w");
    if (fp) fprintf(fp,"%s",manifest_ttl);
    fclose(fp);
  }
    
  //----------
  
  void create_plugin_ttl() {
    char temp[256] = {0};
    memset(plugin_ttl,0,SAT_LV2_PLUGIN_TTL_SIZE);

    strcat(plugin_ttl,"@prefix doap:   <http://usefulinc.com/ns/doap#> .\n");
    strcat(plugin_ttl,"@prefix lv2:    <http://lv2plug.in/ns/lv2core#> .\n");
    strcat(plugin_ttl,"@prefix rdf:    <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n");
    strcat(plugin_ttl,"@prefix rdfs:   <http://www.w3.org/2000/01/rdf-schema#> .\n");
    strcat(plugin_ttl,"@prefix atom:   <http://lv2plug.in/ns/ext/atom#> .\n");
    strcat(plugin_ttl,"@prefix midi:   <http://lv2plug.in/ns/ext/midi#> .\n");
    #ifndef SAT_NO_GUI
    strcat(plugin_ttl,"@prefix ui:     <http://lv2plug.in/ns/extensions/ui#> .\n");
    #endif
    strcat(plugin_ttl,"@prefix urid:   <http://lv2plug.in/ns/ext/urid#> .\n");
    strcat(plugin_ttl,"\n");

    SAT_HostImplementation* host = new SAT_HostImplementation();
    const clap_host_t* clap_host = host->getHost();

    uint32_t num_plugins = SAT_GLOBAL.REGISTRY.getNumDescriptors();
    for (uint32_t i=0; i<num_plugins; i++) {
      const clap_plugin_descriptor_t* clap_descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(i);
      const clap_plugin_t* clap_plugin = SAT_CreatePlugin(i,clap_descriptor,clap_host);
      
      sprintf(temp,"<urn:%s>\n",clap_descriptor->id);
      strcat(plugin_ttl,temp);
      strcat(plugin_ttl,"  a lv2:Plugin ;\n"); // , lv2:InstrumentPlugin ;\n");
      sprintf(temp,"  doap:name           \"%s\" ;\n",clap_descriptor->name);
      strcat(plugin_ttl,temp);
      strcat(plugin_ttl,"  lv2:optionalFeature lv2:hardRTCapable ;\n");
      strcat(plugin_ttl,"  lv2:requiredFeature urid:map ;\n");

      #ifndef SAT_NO_GUI
      // editor?
      #endif


      //----------
      // ports
      //----------

//      uint32_t  numin     = descriptor->getNumInputs();
//      uint32_t  numout    = descriptor->getNumOutputs();
//      uint32_t  numpar    = clap_plugin->getNumParameters();
//      uint32_t  numports  = numin + numout + numpar;

//      if (descriptor->canReceiveMidi()) { numports += 1; }

//      uint32_t  i = 0;
//      uint32_t  p = 0;
//      char      symbol[64];
//      char      name[64];

      // every port symbol must be unique and a valid C identifier
      // and the indices must start at 0 and be contiguous

//      if (numports > 0) {
//        strcat(plugin_ttl,"  lv2:port [\n");

        //----------
        // audio inputs
        //----------
        

//         for (i=0; i<numin; i++) {
//          sprintf(name,"Input %i",i);
//          strcpy(symbol,name);
//          //SAT_MakeValidSymbol(symbol);
//          strcat(plugin_ttl,"    a lv2:AudioPort , lv2:InputPort ;\n");
//          sprintf(temp,     "    lv2:index         %i ;\n",p);
//          strcat(plugin_ttl,temp);
//          sprintf(temp,     "    lv2:symbol        \"%s\" ;\n",symbol);
//          strcat(plugin_ttl,temp);
//          sprintf(temp,     "    lv2:name          \"%s\" ;\n",name);
//          strcat(plugin_ttl,temp);
//          p++;
//          if (p < numports) strcat(plugin_ttl,"	] , [\n");
//          else strcat(plugin_ttl," ] .\n");
//        } // numin

        //----------
        // audio outputs
        //----------

//        for (i=0; i<numout; i++) {
//          sprintf(name,"Output %i",i);
//          strcpy(symbol,name);
//          //SAT_MakeValidSymbol(symbol);
//          strcat(plugin_ttl,"    a lv2:AudioPort , lv2:OutputPort ;\n");
//          sprintf(temp,     "    lv2:index         %i ;\n",p);
//          strcat(plugin_ttl,temp);
//          sprintf(temp,     "    lv2:symbol        \"%s\" ;\n",symbol);
//          strcat(plugin_ttl,temp);
//          sprintf(temp,     "    lv2:name          \"%s\" ;\n",name);
//          strcat(plugin_ttl,temp);
//          p++;
//          if (p < numports) strcat(plugin_ttl,"  ] , [\n");
//          else strcat(plugin_ttl," ] .\n");
//        } // numout

        //----------
        // parameters
        //----------

//        for (i=0; i<numpar; i++) {
//          SAT_Parameter* par = descriptor->getParameter(i);
//          strcpy(name,par->getName());
//          strcpy(symbol,name);
//          //SAT_MakeValidSymbol(symbol);
//          strcat(plugin_ttl,"    a lv2:InputPort , lv2:ControlPort ;\n");
//          sprintf(temp,     "    lv2:index         %i ;\n",p);
//          strcat(plugin_ttl,temp);
//          sprintf(temp,     "    lv2:symbol        \"%s\" ;\n",symbol);
//          strcat(plugin_ttl,temp);
//          sprintf(temp,     "    lv2:name          \"%s\" ;\n",name);
//          strcat(plugin_ttl,temp);
//          sprintf(temp,     "    lv2:default       %f ;\n",    par->getDefValue());
//          strcat(plugin_ttl,temp);
//          sprintf(temp,     "    lv2:minimum       %f ;\n",    par->getMinValue());
//          strcat(plugin_ttl,temp);
//          sprintf(temp,     "    lv2:maximum       %f ;\n",    par->getMaxValue());
//          strcat(plugin_ttl,temp);
//          p++;
//          if (p < numports) strcat(plugin_ttl," ] , [\n");
//          else strcat(plugin_ttl," ] .\n");
//        } // numpar

        //----------
        // midi input
        //----------

//        if (descriptor->canReceiveMidi()) {
//          //strcat(plugin_ttl,"    a lv2:InputPort , atom:AtomPort ;\n");
//          strcat(plugin_ttl,"    a atom:AtomPort , lv2:InputPort ;\n");
//          strcat(plugin_ttl,"    atom:bufferType   atom:Sequence ;\n");
//          strcat(plugin_ttl,"    atom:supports     midi:MidiEvent ;\n");
//          strcat(plugin_ttl,"    lv2:designation   lv2:control ;\n");
//          sprintf(temp,     "    lv2:index         %i ;\n",p);
//          strcat(plugin_ttl,temp);
//          strcat(plugin_ttl,"    lv2:symbol        \"midi_in\" ;\n");
//          strcat(plugin_ttl,"    lv2:name          \"Midi in\" ;\n");
//          p++;
//          if (p < numports) strcat(plugin_ttl," ] , [\n");
//          else strcat(plugin_ttl," ] .\n");
//        } // midi in
//
//      } // ports

      //----------

      strcat(plugin_ttl,"\n");
      
      clap_plugin->destroy(clap_plugin);
    } // for all plugins
    delete host;
    
    // write manifest.ttl
    FILE* fp = nullptr;
    fp = fopen("plugin.ttl","w");
    if (fp) fprintf(fp,"%s",manifest_ttl);
    fclose(fp);
    
  }
    



  //----------
  
  void create_gui_ttl() {
  }

  //----------
  
  void write_ttl() {
//    // write manifest.ttl
//    FILE* fp = nullptr;
//    fp = fopen("manifest.ttl","w");
//    if (fp) fprintf(fp,"%s",manifest_ttl);
//    fclose(fp);
//    
//    // write [plugin].ttl
//    char buffer[SAT_MAX_PATH_LENGTH] = {0};
//    strcpy(buffer,clap_descriptor->name);
//    SAT_MakeValidSymbol(buffer);
//    strcat(buffer,".ttl");
//    fp = fopen(buffer,"w");
//    if (fp) fprintf(fp,"%s",plugin_ttl);
//    fclose(fp);
  }

};

//----------------------------------------------------------------------
#endif




























#if 0

  void SAT_Lv2CreateManifest(const clap_plugin_t* plugin, char* manifest_ttl, char* plugin_ttl/*, char* editor_ttl*/) {


  }

#endif // 0



