#ifndef sat_clap_host_implementation_included
#define sat_clap_host_implementation_included
//----------------------------------------------------------------------
// the host ptr you give to hosted plugins

#include "sat.h"
#include "plugin/lib/sat_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ClapHostImplementation {

//------------------------------
public:
//------------------------------

  SAT_ClapHostImplementation() {
  };

  //----------

  virtual ~SAT_ClapHostImplementation() {
  }

//------------------------------
public:
//------------------------------

  const clap_host_t* getClapHost() {
    return &MClapHost;
  }

//------------------------------
public:
//------------------------------

  virtual const void* get_extension(const char *extension_id) {
    //SAT_PRINT("extension id: %s\n",extension_id);
    return nullptr;
  }

  virtual void request_restart() {
    SAT_TRACE;
  }

  virtual void request_process() {
    SAT_TRACE;
  }
    
  virtual void request_callback() {
    SAT_TRACE;
  }

//------------------------------
private:
//------------------------------

  static
  const void* host_get_extension_callback(const struct clap_host *host, const char *extension_id) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->get_extension(extension_id);
  }

  static
  void host_request_restart_callback(const struct clap_host *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->request_restart();
  }

  static
  void host_request_process_callback(const struct clap_host *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->request_process();
  }
    
  static
  void host_request_callback_callback(const struct clap_host *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->request_callback();
  }

//------------------------------
private:
//------------------------------

  const clap_host_t MClapHost = {
    .clap_version     = CLAP_VERSION,
    .host_data        = this,
    .name             = "SAT_ClapHostImplementation",
    .vendor           = "skei.audio",
    .url              = "",
    .version          = "0,0,0",
    .get_extension     = host_get_extension_callback,
    .request_restart   = host_request_restart_callback,
    .request_process   = host_request_process_callback,
    .request_callback  = host_request_callback_callback
  };

};

//----------------------------------------------------------------------
#endif
