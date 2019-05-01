// For open-source license, please refer to [License](https://github.com/HikariObfuscator/Hikari/wiki/License).
#include "substrate.h"
#include <llvm/Transforms/Obfuscation/Obfuscation.h>
#include <llvm/Config/abi-breaking.h>
#include <string>
#include <mach-o/dyld.h>
#if LLVM_ENABLE_ABI_BREAKING_CHECKS==1
#error "Configure LLVM with -DLLVM_ABI_BREAKING_CHECKS=FORCE_OFF"
#endif
using namespace std;
void (*old_pmb)(void* dis,legacy::PassManagerBase &MPM);
Pass* (*old_get_LS)();
extern "C" Pass* _ZN4llvm21createLowerSwitchPassEv(){
  return old_get_LS();
}
static void new_pmb(void* dis,legacy::PassManagerBase &MPM){
  MPM.add(createObfuscationPass());
  old_pmb(dis,MPM);
}
static __attribute__((__constructor__)) void Inj3c73d(int argc, char* argv[]){
  char* executablePath=argv[0];
  //Initialize our own LLVM Library
  MSImageRef exeImagemage=MSGetImageByName(executablePath);
  errs()<<"Applying Apple Clang Hooks...\n";
  MSHookFunction((void*)MSFindSymbol(exeImagemage,"__ZN4llvm18PassManagerBuilder25populateModulePassManagerERNS_6legacy15PassManagerBaseE"),(void*)new_pmb,(void**)&old_pmb);
  old_get_LS=(Pass* (*)())MSFindSymbol(exeImagemage,"__ZN4llvm15callDefaultCtorIN12_GLOBAL__N_111LowerSwitchEEEPNS_4PassEv");
}
