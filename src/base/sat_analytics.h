#ifndef sat_analytics_included
#define sat_analytics_included
//----------------------------------------------------------------------

#include "base/sat_print.h"

//----------------------------------------------------------------------

/**
 * @class SAT_Analytics
 * @brief A class for collecting and reporting analytics within the SAT framework.
 * 
 * This class is responsible for tracking various parameters and modifications 
 * between different components (Host, GUI, Audio) and providing a way to report 
 * these analytics.
 */
class SAT_Analytics {

//------------------------------
private:
//------------------------------

  SAT_Print* MPrint = nullptr; ///< Pointer to the SAT_Print instance for logging analytics.

  #ifdef SAT_INCLUDE_ANALYTICS
    uint32_t _ParamFromHostToGui  = 0; ///< Counter for parameters sent from Host to GUI.
    uint32_t _ModFromHostToGui    = 0; ///< Counter for modifications sent from Host to GUI.
    uint32_t _ParamFromGuiToHost  = 0; ///< Counter for parameters sent from GUI to Host.
    uint32_t _ParamFromGuiToAudio = 0; ///< Counter for parameters sent from GUI to Audio.
  #endif

//------------------------------
public:
//------------------------------

  /**
   * @brief Default constructor for SAT_Analytics.
   */
  SAT_Analytics() {
  }

  //----------

  /**
   * @brief Destructor for SAT_Analytics.
   */
  ~SAT_Analytics() {
  }

//------------------------------
public:
//------------------------------

  /**
   * @brief Initializes the SAT_Analytics instance with a SAT_Print instance.
   * @param APrint Pointer to a SAT_Print instance for logging.
   * @return true if initialization is successful, otherwise false.
   */
  bool initialize(SAT_Print* APrint) {
    MPrint = APrint;
    #ifdef SAT_INCLUDE_ANALYTICS
    #endif
    return true;
  }

  /**
   * @brief Cleans up the SAT_Analytics instance, dumping analytics if enabled.
   */
  void cleanup() {
    #ifdef SAT_INCLUDE_ANALYTICS
      dumpAnalytics();
    #endif
  }

//------------------------------
public:
//------------------------------

  /**
   * @brief Dumps the collected analytics to the SAT_Print instance.
   */
  void dumpAnalytics() {
    #ifdef SAT_INCLUDE_ANALYTICS
      MPrint->print("ANALYTICS:\n");
      MPrint->print("  _ParamFromHostToGui  %i\n",_ParamFromHostToGui);
      MPrint->print("  _ModFromHostToGui    %i\n",_ModFromHostToGui);
      MPrint->print("  _ParamFromGuiToHost  %i\n",_ParamFromGuiToHost);
      MPrint->print("  _ParamFromGuiToAudio %i\n",_ParamFromGuiToAudio);
    #endif
  }

//------------------------------
public: // queues
//------------------------------

  /**
   * @brief Sets the count for parameters sent from Host to GUI.
   * @param count The new count to set.
   */
  void set_ParamFromHostToGui(uint32_t count) {
    #ifdef SAT_INCLUDE_ANALYTICS
      if (count > _ParamFromHostToGui) _ParamFromHostToGui = count;
    #endif
  }

  /**
   * @brief Sets the count for modifications sent from Host to GUI.
   * @param count The new count to set.
   */
  void set_ModFromHostToGui(uint32_t count) {
    #ifdef SAT_INCLUDE_ANALYTICS
      if (count > _ModFromHostToGui) _ModFromHostToGui = count;
    #endif
  }

  /**
   * @brief Sets the count for parameters sent from GUI to Host.
   * @param count The new count to set.
   */
  void set_ParamFromGuiToHost(uint32_t count) {
    #ifdef SAT_INCLUDE_ANALYTICS
      if (count > _ParamFromGuiToHost) _ParamFromGuiToHost = count;
    #endif
  }

  /**
   * @brief Sets the count for parameters sent from GUI to Audio.
   * @param count The new count to set.
   */
  void set_ParamFromGuiToAudio(uint32_t count) {
    #ifdef SAT_INCLUDE_ANALYTICS
      if (count > _ParamFromGuiToAudio) _ParamFromGuiToAudio = count;
    #endif
  }

};

//----------------------------------------------------------------------
#endif