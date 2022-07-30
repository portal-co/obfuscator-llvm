#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Utils/CodeExtractor.h"
#include "llvm/IR/PassManager.h" // New PassManager

#include "Utils.h"
struct BB2Func : public llvm::PassInfoMixin<BB2Func>{

  bool runOnFunction(llvm::Function &F);
    llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &AM){
        runOnFunction(F);
        return llvm::PreservedAnalyses::none();
    }

  static bool isRequired() { return true; }
};