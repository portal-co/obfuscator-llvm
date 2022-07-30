#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "RenameFnsPass.h"
#include "SplitBasicBlocks.h"
#include "Substitution.h"
#include "Flattening.h"
#include "BogusControlFlow.h"
#include "BB2Func.h"
#include "StringObfuscator.h"

using namespace llvm;

// Flags for obfuscation
static cl::opt<std::string> AesSeed("aesSeed", cl::init(""), cl::desc("seed for the AES-CTR PRNG"));
/*
static void loadPass(const PassManagerBuilder &Builder, legacy::PassManagerBase &PM) {
  // Initialization of the global cryptographically
  // secure pseudo-random generator
  if(!AesSeed.empty()) {
    if(!llvm::cryptoutils->prng_seed(AesSeed.c_str())) {
      exit(1);
    }
  }
  PM.add(createSplitBasicBlock());
  PM.add(createBogus());
  PM.add(createFlattening());
  PM.add(createSubstitution());
}

// These constructors add our pass to a list of global extensions.
static RegisterStandardPasses clangtoolLoader_Ox(PassManagerBuilder::EP_OptimizerLast, loadPass);
static RegisterStandardPasses clangtoolLoader_O0(PassManagerBuilder::EP_EnabledOnOptLevel0, loadPass);
// Note: The location EP_OptimizerLast places this pass at the end of the list
// of *optimizations*. That means on -O0, it does not get run.
//
// In general, adding your pass twice will cause it to run twice, but in this
// particular case, the two are disjoint (EP_EnabledOnOptLevel0 only runs if
// you're in -O0, and EP_OptimizerLast only runs if you're not). You can check
// include/llvm/Transforms/IPO/PassManagerBuilder.h header and
// lib/Transforms/IPO/PassManagerBuilder.cpp file for the exact behavior.
*/
namespace{
  struct MainState{
    bool isCringe = false;
  };
  MainState st;
};
bool &_g_isCringe(){return st.isCringe;};
PassPluginLibraryInfo getObfuscatorLLVMPluginInfo(){
  return {LLVM_PLUGIN_API_VERSION, "ObfuscatorLLVM", LLVM_VERSION_STRING, [](PassBuilder &PB){
    PB.registerVectorizerStartEPCallback([](FunctionPassManager &PM, OptimizationLevel level){
      if(!AesSeed.empty()) {
        if(!llvm::cryptoutils->prng_seed(AesSeed.c_str())) {
          exit(1);
        }
      }
      PM.addPass(SplitBasicBlockPass());
      PM.addPass(BogusControlFlowPass());
      PM.addPass(FlatteningPass());
      PM.addPass(SubstitutionPass());
      PM.addPass(BB2Func());
      PM.addPass(BogusControlFlowPass(1));
      PM.addPass(llvm::RenameFnsPass());
    });
    	PB.registerPipelineParsingCallback([](StringRef Name, ModulePassManager &MPM,
        ArrayRef<PassBuilder::PipelineElement>) {
          					if(Name == "string-obfuscator-pass"){
						MPM.addPass(StringObfuscatorModPass(st.isCringe));
						return true;
					}
					return false;
        });
  }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getObfuscatorLLVMPluginInfo();
}
