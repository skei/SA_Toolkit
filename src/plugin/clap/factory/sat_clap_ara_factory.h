#ifndef sat_clap_ara_factory_included
#define sat_clap_ara_factory_included
//----------------------------------------------------------------------

//#define SAT_PLUGIN_USE_ARA

#include "extern/ara/arasdk/ARACLAP.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void ARA_CALL sat_ARAAssertFunction_callback(ARA_NAMESPACE ARAAssertCategory category, const void* problematicArgument, const char* diagnosis) {
}

//----------------------------------------------------------------------

//struct ARAInterfaceConfiguration {
//  ARASize             structSize;
//  ARAAPIGeneration    desiredApiGeneration;
//  ARAAssertFunction*  assertFunctionAddress;
//};

void ARA_CALL sat_initializeARAWithConfiguration_callback(const struct ARA_NAMESPACE ARAInterfaceConfiguration* config) {
  SAT_PRINT;
  //config->structSize            = sizeof(struct ARA_NAMESPACE ARAInterfaceConfiguration);
  //config->desiredApiGeneration  = 0;
  //config->assertFunctionAddress = sat_ARAAssertFunction_callback;
}

//----------

void ARA_CALL sat_uninitializeARA_callback(void) {
  SAT_PRINT;
}

//----------

//struct ARADocumentControllerInstance {
//  ARASize                               structSize;
//  ARADocumentControllerRef              documentControllerRef;
//  const ARADocumentControllerInterface* documentControllerInterface;
//};

//struct ARADocumentControllerHostInstance {
//  ARASize                                     structSize;
//  ARAAudioAccessControllerHostRef             audioAccessControllerHostRef;
//  const ARAAudioAccessControllerInterface*    audioAccessControllerInterface;
//  ARAArchivingControllerHostRef               archivingControllerHostRef;
//  const ARAArchivingControllerInterface*      archivingControllerInterface;
//  ARAContentAccessControllerHostRef           contentAccessControllerHostRef;
//  const ARAContentAccessControllerInterface*  contentAccessControllerInterface;
//  ARAModelUpdateControllerHostRef             modelUpdateControllerHostRef;
//  const ARAModelUpdateControllerInterface*    modelUpdateControllerInterface;
//  ARAPlaybackControllerHostRef                playbackControllerHostRef;
//  const ARAPlaybackControllerInterface*       playbackControllerInterface;
//};

//struct ARADocumentProperties {
//  ARASize       structSize;
//  ARAUtf8String name;
//};

const struct ARA_NAMESPACE ARADocumentControllerInstance* ARA_CALL sat_createDocumentControllerWithDocument_callback(const struct ARA_NAMESPACE ARADocumentControllerHostInstance* hostInstance, const struct ARA_NAMESPACE ARADocumentProperties* properties) {
  if (properties) { SAT_PRINT("name: %s\n",properties->name); }
  else { SAT_PRINT("properties = NULL\n"); }
  return nullptr;
}

//----------------------------------------------------------------------
//
// ara factory
//
//----------------------------------------------------------------------

const struct ARA_NAMESPACE ARAFactory SAT_ARA_FACTORY = {
  .structSize                           = sizeof(struct ARA_NAMESPACE ARAFactory),
  .lowestSupportedApiGeneration         = 0,
  .highestSupportedApiGeneration        = 0,
  .factoryID                            = SAT_VENDOR "/ara_factory",
  .initializeARAWithConfiguration       = sat_initializeARAWithConfiguration_callback,
  .uninitializeARA                      = sat_uninitializeARA_callback,
  .plugInName                           = "plugin name",
  .manufacturerName                     = SAT_VENDOR,
  .informationURL                       = SAT_URL,
  .version                              = SAT_VERSION,
  .createDocumentControllerWithDocument = sat_createDocumentControllerWithDocument_callback,
  .documentArchiveID                    = "",
  .compatibleDocumentArchiveIDsCount    = 0,
  .compatibleDocumentArchiveIDs         = nullptr,//(const ARAPersistentID* []) { nullptr, nullptr },
  .analyzeableContentTypesCount         = 0,
  .analyzeableContentTypes              = nullptr,//(const ARAPersistentID* []) { nullptr, nullptr },
  .supportedPlaybackTransformationFlags = 0,
  .supportsStoringAudioFileChunks       = false
};

//----------------------------------------------------------------------

uint32_t sat_clap_ara_factory_get_factory_count_callback(const struct clap_ara_factory *factory) {
  SAT_PRINT;
  return 1;
}

//----------

const ARA_NAMESPACE ARAFactory* sat_clap_ara_factory_get_ara_factory_callback(const struct clap_ara_factory *factory, uint32_t index) {
  SAT_PRINT;
  return &SAT_ARA_FACTORY;
}

//----------

/*
  return the id of the plugin that this factory is for..
  TODO: add to SAT_REGISTRY.. SAT_RegisterARA()?
*/

const char* sat_clap_ara_factory_get_plugin_id_callback(const struct clap_ara_factory *factory, uint32_t index) {
  SAT_PRINT;
  return nullptr;
}

//----------------------------------------------------------------------

ARA_DRAFT struct clap_ara_factory SAT_CLAP_ARA_FACTORY {
  .get_factory_count  = sat_clap_ara_factory_get_factory_count_callback,
  .get_ara_factory    = sat_clap_ara_factory_get_ara_factory_callback,
  .get_plugin_id      = sat_clap_ara_factory_get_plugin_id_callback
};

//----------------------------------------------------------------------
#endif