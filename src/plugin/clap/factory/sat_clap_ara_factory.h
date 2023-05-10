#ifndef sat_clap_ara_factory_included
#define sat_clap_ara_factory_included
//----------------------------------------------------------------------

//#define SAT_PLUGIN_USE_ARA

#include "extern/ara/arasdk/ARACLAP.h"

//----------------------------------------------------------------------

// (ARA_CALL* initializeARAWithConfiguration) (const ARAInterfaceConfiguration * config);
// (ARA_CALL* uninitializeARA) (void);
// (ARA_CALL* createDocumentControllerWithDocument) (const ARADocumentControllerHostInstance * hostInstance, const ARADocumentProperties * properties);

//----------------------------------------------------------------------

//struct ARAFactory {
//  ARASize                               structSize;
//  ARAAPIGeneration                      lowestSupportedApiGeneration;
//  ARAAPIGeneration                      highestSupportedApiGeneration;
//  ARAPersistentID                       factoryID;
//  void                                  (ARA_CALL* initializeARAWithConfiguration) (const ARAInterfaceConfiguration * config);
//  void                                  (ARA_CALL* uninitializeARA) (void);
//  ARAUtf8String                         plugInName;
//  ARAUtf8String                         manufacturerName;
//  ARAUtf8String                         informationURL;
//  ARAUtf8String                         version;
//  const ARADocumentControllerInstance*  (ARA_CALL* createDocumentControllerWithDocument) (const ARADocumentControllerHostInstance * hostInstance, const ARADocumentProperties * properties);
//  ARAPersistentID                       documentArchiveID;
//  ARASize                               compatibleDocumentArchiveIDsCount;
//  const ARAPersistentID*                compatibleDocumentArchiveIDs;
//  ARASize                               analyzeableContentTypesCount;
//  const ARAContentType*                 analyzeableContentTypes;
//  ARAPlaybackTransformationFlags        supportedPlaybackTransformationFlags;
//  ARA_ADDENDUM(2_0_Final) ARABool       supportsStoringAudioFileChunks;
//};

struct ARAFactory SAT_ARA_FACTORY {
  .structSize                           = 0,
  .lowestSupportedApiGeneration         = 0,
  .highestSupportedApiGeneration        = 0,
  .factoryID                            = "",
  .initializeARAWithConfiguration       = nullptr,
  .uninitializeARA                      = nullptr,
  .plugInName                           = "",
  .manufacturerName                     = "",
  .informationURL                       = "",
  .version                              = "",
  .createDocumentControllerWithDocument = nullptr,
  .documentArchiveID                    = "",
  .compatibleDocumentArchiveIDsCount    = 0,
  .compatibleDocumentArchiveIDs         = nullptr,//(const ARAPersistentID* []) { nullptr, nullptr },
  .analyzeableContentTypesCount         = 0,
  .analyzeableContentTypes              = nullptr,//(const ARAPersistentID* []) { nullptr, nullptr },
  .supportedPlaybackTransformationFlags = 0,
  .supportsStoringAudioFileChunks       = false
};

//----------------------------------------------------------------------

uint32_t clap_ara_factory_get_factory_count_callback(const struct clap_ara_factory *factory) {
  return 1;
}

//----------

const ARA_NAMESPACE ARAFactory* clap_ara_factory_get_ara_factory_callback(const struct clap_ara_factory *factory, uint32_t index) {
}

//----------

const char* clap_ara_factory_get_plugin_id_callback(const struct clap_ara_factory *factory, uint32_t index) {
}

//----------------------------------------------------------------------

ARA_DRAFT struct clap_ara_factory SAT_CLAP_ARA_FACTORY {
  .get_factory_count  = clap_ara_factory_get_factory_count_callback,
  .get_ara_factory    = clap_ara_factory_get_ara_factory_callback,
  .get_plugin_id      = clap_ara_factory_get_plugin_id_callback
};

//----------------------------------------------------------------------
#endif