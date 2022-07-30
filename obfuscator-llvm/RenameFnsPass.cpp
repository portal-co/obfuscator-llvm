//===- RenameFns.cpp - SplitBasicBlokc Obfuscation pass--------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the split basic block pass
//
//===----------------------------------------------------------------------===//

#include "CryptoUtils.h"
#include "RenameFnsPass.h"
#include "Utils.h"

#define DEBUG_TYPE "split"

using namespace llvm;

// Stats
STATISTIC(Rename, "Functions renamed");

static cl::opt<bool> RenFlag("rename", cl::init(false),
                           cl::desc("Enable function renaming"));


namespace {
struct RenameFns : public FunctionPass {
  static char ID; // Pass identification, replacement for typeid

  RenameFns() : FunctionPass(ID) {}

  bool runOnFunction(Function &F);
};
} // namespace

char RenameFns::ID = 0;
static RegisterPass<RenameFns> X("rnfn", "Rename functions");

Pass *llvm::createRenameFns() {
  return new RenameFns();
}

PreservedAnalyses RenameFnsPass::run(Function &F, FunctionAnalysisManager &AM){
  RenameFns split;
  split.runOnFunction(F);
  return PreservedAnalyses::none();
}

bool RenameFns::runOnFunction(Function &F) {
  Function *tmp = &F;
  if(!toObfuscate(RenFlag, tmp, "rename")){
    return false;
  }
  // if(!tmp->isDeclaration()){
  //   if(!tmp->isIntrinsic() && !tmp->hasExternalLinkage()){
      auto s = tmp->getName().str();
      tmp->setName(remap(s.c_str()));
  //   }
  // }

  return false;
}
