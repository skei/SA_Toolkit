
#include "base/sat.h"
#include "base/utils/sat_jit.h"
//#include "base/utils/sat_vm.h"
//#include "base/utils/sat_vm_compiler.h"
#include "plugin/sat_plugin.h"
#include "audio/sat_audio_utils.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

static
uint32_t test_jit_magic_value = 0xCACACACA;
  
//----------

static
SAT_JitOpcode test_jit_opcodes[] = {
//        8                              8            16,16             64
  { .op = SAT_JIT_OP_LOADI,  .args_map = 0, .args = { 0, 1 }},
  { .op = SAT_JIT_OP_LOADI,  .args_map = 0, .args = { 1, 5 }},
  { .op = SAT_JIT_OP_LOADI,  .args_map = 2, .args = { 3    }}, { .raw = (uint64_t)&test_jit_magic_value },
  { .op = SAT_JIT_OP_LOAD,   .args_map = 0, .args = { 3, 3 }},
  { .op = SAT_JIT_OP_LABEL,  .args_map = 0, .args = { 0    }},
  { .op = SAT_JIT_OP_ADD,    .args_map = 0, .args = { 3, 0 }},
  { .op = SAT_JIT_OP_ADD,    .args_map = 0, .args = { 2, 0 }},
  { .op = SAT_JIT_OP_CMP,    .args_map = 0, .args = { 1, 2 }},
  { .op = SAT_JIT_OP_BRANCH, .args_map = 0, .args = { SAT_JIT_BC_NOTEQUAL, 0 }},
  { .op = SAT_JIT_OP_LOADI,  .args_map = 2, .args = { 0    }}, { .raw = (uint64_t)&test_jit_magic_value },
  { .op = SAT_JIT_OP_STORE,  .args_map = 0, .args = { 0, 3 }},
  { .op = SAT_JIT_OP_EXIT                                   },
  { .op = SAT_JIT_END_OF_CODE                               }
};

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t test_jit_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/test_jit",
  .name         = "test_jit",
  .vendor       = "skei.audio",
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "a nice plugin",
  .features     = (const char* []) {
                    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    nullptr
                  }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class test_jit_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  SAT_Jit JIT = {};

//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(test_jit_plugin)
  
//------------------------------
public:
//------------------------------

  bool init() final {
    
    test_jit();
    
    registerDefaultExtensions();    
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();
    
    SAT_Parameter* param1 = new SAT_Parameter("Param1", .025);
    appendParameter(param1);
    
    SAT_Parameter* param2 = new SAT_Parameter("Param2", 0.75);
    appendParameter(param2);
    
    setInitialEditorSize(600,400,1.333);
    
    return SAT_Plugin::init();
  }
  
  //----------

  bool initEditorWindow(SAT_Editor* AEditor/*, SAT_Window* AWindow*/) final {
    SAT_Window* window = AEditor->getWindow();
    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
    window->appendRootWidget(panel);
    
    panel->setFillGradient(true);
    panel->setGradientColors(SAT_Green,0.2,0.2);
    
    return true;
  }

  //----------
  
  bool handleParamValue(const clap_event_param_value_t* event) override {
    SAT_Print("param_id %i value %f\n",event->param_id,event->value);
    return true;
  }
  
  //----------

  void processAudio(SAT_ProcessContext* AContext) final {
    float** inputs = AContext->process->audio_inputs[0].data32;
    float** outputs = AContext->process->audio_outputs[0].data32;
    uint32_t length = AContext->process->frames_count;
    SAT_CopyStereoBuffer(outputs,inputs,length);
    double scale = getParameterValue(0) + getModulationValue(0);
    scale = SAT_Clamp(scale,0,1);
    SAT_ScaleStereoBuffer(outputs,scale,length);
  }
  
//------------------------------
private:
//------------------------------
  
  #define ENABLE_PPRINT
  #define ENABLE_INTERPRETER
  #define ENABLE_JIT
//  #define ENABLE_BYTECODE_DUMP
//  #define ENABLE_COMPILED_DUMP

  //----------

  void test_jit() {
    
    #ifdef ENABLE_PPRINT
      JIT.printOpcodes(test_jit_opcodes);
    #endif

    #ifdef ENABLE_BYTECODE_DUMP
      // count size
      uint32_t num = 0;
      for (num = 0; true; ) {
        SAT_JitOpcode* opcode = &test_jit_opcodes[num];
        num += JIT.getOpcodeLength(opcode);
        if (opcode->op == SAT_JIT_END_OF_CODE) {
          break;
        }
      }
      // write to file
      FILE* bf = fopen("bytecode.bin", "wb");
      fwrite(test_jit_opcodes, 1, num*8, bf);
      fclose(bf);
    #endif

    #ifdef ENABLE_INTERPRETER
      test_jit_magic_value = 0xDEADC0D9;
      SAT_DPrint("Magic value before is %x\n", test_jit_magic_value);
      JIT.interpret(test_jit_opcodes);
      SAT_DPrint("Magic value after is %x\n", test_jit_magic_value);
    #endif

    #ifdef ENABLE_JIT
      JIT.compile(test_jit_opcodes);
      #ifdef ENABLE_COMPILED_DUMP
        void* code = JIT.getCompiledCode();
        uint32_t codesize = JIT.getCompiledCodeSize();
        FILE* cf = fopen("executable.bin", "wb");
        fwrite(code, 1, codesize, cf);
        fclose(cf);
      #endif
      test_jit_magic_value = 0xDEADBEEA;
      SAT_DPrint("Magic value before is %x\n", test_jit_magic_value);
      JIT.execute();
      SAT_DPrint("Magic value after is %x\n", test_jit_magic_value);
    #endif
    
  }

  //----------

  #undef ENABLE_PPRINT
  #undef ENABLE_INTERPRETER
  #undef ENABLE_JIT
  #undef ENABLE_BYTECODE_DUMP
  #undef ENABLE_COMPILED_DUMP

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(test_jit_descriptor,test_jit_plugin)
#endif
