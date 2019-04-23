#include "substrate.h"
#include <llvm/Transforms/Obfuscation/Obfuscation.h>
#include <llvm/Config/abi-breaking.h>
#include <string>
#include <libgen.h>
#if LLVM_ENABLE_ABI_BREAKING_CHECKS==1
#error "Configure LLVM with -DLLVM_ABI_BREAKING_CHECKS=FORCE_OFF"
#endif

using namespace std;
void (*old_pmb)(void* dis,legacy::PassManagerBase &MPM);
void* handle=nullptr;
static void new_pmb(void* dis,legacy::PassManagerBase &MPM){
  MPM.add(createObfuscationPass());
  old_pmb(dis,MPM);
}
static __attribute__((__constructor__)) void Inj3c73d(int argc, char* argv[]){
  char* executablePath=argv[0];
  //Initialize our own LLVM Library
  PassRegistry &Registry = *PassRegistry::getPassRegistry();
  initializeCore(Registry);
  initializeAnalysis(Registry);
  initializeTransformUtils(Registry);
  MSHookFunction((void*)MSFindSymbol(MSGetImageByName(executablePath),"__ZN4llvm18PassManagerBuilder25populateModulePassManagerERNS_6legacy15PassManagerBaseE"),(void*)new_pmb,(void**)&old_pmb);

}
