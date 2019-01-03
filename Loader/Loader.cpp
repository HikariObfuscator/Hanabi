#include "substrate.h"
#include <llvm/Transforms/Obfuscation/Obfuscation.h>
#include <string>
#include <libgen.h>
using namespace std;
int llvm::DisableABIBreakingChecks=1;
void (*old_pmb)(void* dis,legacy::PassManagerBase &MPM);
void* handle=nullptr;
static void new_pmb(void* dis,legacy::PassManagerBase &MPM){
  printf("Injecting Custom Obfuscation Pass\n");
  Pass* (*ptr)()=(Pass* (*)())dlsym(handle,"__ZN4llvm21createObfuscationPassEv");
  printf("llvm::createObfuscationPass() Pointer: %p\n",ptr);
  MPM.add(ptr());
  printf("Added Obfuscation Pass\n");
  old_pmb(dis,MPM);
  printf("Done PMB Hijacking\n");
}
static __attribute__((__constructor__)) void Inj3c73d(int argc, char* argv[]){
  char* executablePath=argv[0];
  char* folderPath=dirname(executablePath);
  stringstream ss;
  ss<<folderPath<<"/libLLVM.dylib";
  printf("Loading libLLVM.dylib from Path:%s\n",ss.str().c_str());
  handle=dlopen(ss.str().c_str(),RTLD_NOW|RTLD_GLOBAL);
  if(handle==NULL){
    printf("LLVMObfuscation.dylib Pointer:%p\n",handle);
    printf("Dylib Loading Error:%s\n",dlerror());
  }
  else{
    printf("Performing Hook\n");
    MSHookFunction((void*)MSFindSymbol(MSGetImageByName(executablePath),"__ZN4llvm18PassManagerBuilder25populateModulePassManagerERNS_6legacy15PassManagerBaseE"),(void*)new_pmb,(void**)&old_pmb);
  }
}
