// Print branch probabilities in a program
//
// By Andrew Santosa <santosa_1999@yahoo.com>
// This software is in the public domain.

#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/SourceMgr.h"

#include <set>
#include <vector>

class PrintBranchProbability : public llvm::ModulePass {
public:
  static char ID;

  PrintBranchProbability() : ModulePass(ID) {}

  virtual bool runOnModule(llvm::Module &M) {
    for (llvm::Module::iterator func = M.begin(), fe = M.end(); func != fe;
         ++func) {
      if (func->isDeclaration())
        continue;

      const llvm::BranchProbabilityInfo &BPI =
          getAnalysis<llvm::BranchProbabilityInfo>(*func);

      for (llvm::Function::iterator bi = func->begin(), be = func->end();
           bi != be; ++bi) {
        llvm::TerminatorInst *ti = bi->getTerminator();
        llvm::errs() << "BRANCH: ";
        ti->dump();
        unsigned numSuccessors = ti->getNumSuccessors();
        for (unsigned i = 0; i < numSuccessors; ++i) {
          llvm::BranchProbability prob = BPI.getEdgeProbability(&(*bi), i);
          llvm::errs() << "EDGE " << i << " PROBABILITY: ";
          prob.dump();
        }
      }
    }

    return false; // does not modify program
  }

  virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const {
    AU.setPreservesAll();
    AU.addRequired<llvm::BranchProbabilityInfo>();
  }
};

char PrintBranchProbability::ID = 0;
static llvm::RegisterPass<PrintBranchProbability>
X("print-branch-probability", "Print branch probability", false, false);

/*
 * Main.
 */
int main(int argc, char **argv) {
  if (argc < 2) {
    llvm::errs() << "Filename unspecified\n";
    return 1;
  }

  llvm::LLVMContext &Context = llvm::getGlobalContext();
  llvm::SMDiagnostic Err;
  llvm::Module *M = ParseIRFile(argv[1], Err, Context);

  if (M == 0) {
    llvm::errs() << "ERROR: failed to load " << argv[0] << "\n";
    return 1;
  }

  llvm::PassManager PM;
  PM.add(new llvm::BranchProbabilityInfo());
  PM.add(new PrintBranchProbability());
  PM.run(*M);

  return 0;
}
