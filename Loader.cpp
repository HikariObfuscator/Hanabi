#include "substrate.h"
#include <llvm/Transforms/Obfuscation/Obfuscation.h>
#include <llvm/Config/abi-breaking.h>
#include <llvm/IR/Constants.h>
#include <string>
#include <mach-o/dyld.h>
#if LLVM_ENABLE_ABI_BREAKING_CHECKS==1
#error "Configure LLVM with -DLLVM_ABI_BREAKING_CHECKS=FORCE_OFF"
#endif
extern const struct mach_header _mh_dylib_header;
using namespace std;
void (*old_pmb)(void* dis,legacy::PassManagerBase &MPM);
void* handle=nullptr;
static void new_pmb(void* dis,legacy::PassManagerBase &MPM){
  MPM.add(createObfuscationPass());
  old_pmb(dis,MPM);
}

Constant* (*image_CFP_get)(LLVMContext* ctx,APFloat& ap);
static Constant* new_CFP_get(LLVMContext* ctx,APFloat& ap){
  return image_CFP_get(ctx,ap);
}
Constant* (*image_CI_getIT)(IntegerType*, unsigned long long, bool);
static Constant* new_CI_getIT(IntegerType* a, unsigned long long b, bool c){
  return image_CI_getIT(a,b,c);
}

Constant* (*image_CI_getT)(Type*, unsigned long long, bool);
static Constant* new_CI_getT(Type* a, unsigned long long b, bool c){
  return image_CI_getT(a,b,c);
}
static __attribute__((__constructor__)) void Inj3c73d(int argc, char* argv[]){
  char* executablePath=argv[0];
  Dl_info dl_info;
  dladdr((void *)Inj3c73d, &dl_info);
  const char* currPath=dl_info.dli_fname;
  //Initialize our own LLVM Library
  MSImageRef exeImagemage=MSGetImageByName(executablePath);
  PassRegistry &Registry = *PassRegistry::getPassRegistry();
  initializeCore(Registry);
  initializeAnalysis(Registry);
  initializeTransformUtils(Registry);
  MSImageRef selfImage=MSGetImageByName(currPath);
  errs()<<"Applying Apple Clang Hooks...\n";
  MSHookFunction((void*)MSFindSymbol(exeImagemage,"__ZN4llvm18PassManagerBuilder25populateModulePassManagerERNS_6legacy15PassManagerBaseE"),(void*)new_pmb,(void**)&old_pmb);
  errs()<<"Applying Loader Internal Hooks...\n";
  //This is applied on a case-by-case
  image_CFP_get=(Constant* (*)(LLVMContext* ctx,APFloat& ap))MSFindSymbol(exeImagemage,"__ZN4llvm10ConstantFP3getERNS_11LLVMContextERKNS_7APFloatE");
  MSHookFunction((void*)MSFindSymbol(selfImage,"__ZN4llvm10ConstantFP3getERNS_11LLVMContextERKNS_7APFloatE"),(void*)new_CFP_get,nullptr);

  image_CI_getIT=(Constant* (*)(IntegerType*, unsigned long long, bool))MSFindSymbol(exeImagemage,"__ZN4llvm11ConstantInt3getEPNS_11IntegerTypeEyb");
  MSHookFunction((void*)MSFindSymbol(selfImage,"__ZN4llvm11ConstantInt3getEPNS_11IntegerTypeEyb"),(void*)new_CI_getIT,nullptr);

  image_CI_getT= (Constant* (*)(Type*, unsigned long long, bool))MSFindSymbol(exeImagemage,"__ZN4llvm11ConstantInt3getEPNS_4TypeEyb");
  MSHookFunction((void*)MSFindSymbol(selfImage,"__ZN4llvm11ConstantInt3getEPNS_4TypeEyb"),(void*)new_CI_getT,nullptr);
}
