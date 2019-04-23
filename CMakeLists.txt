add_library(LLVMHanabi SHARED
  ${CMAKE_CURRENT_LIST_DIR}/Loader.cpp
)
add_dependencies(LLVMHanabi
  LLVMAnalysis
  LLVMCore
  LLVMTransformUtils
  LLVMSupport
  LLVMObfuscation
)
target_link_libraries(LLVMHanabi PRIVATE LLVMAnalysis LLVMObfuscation LLVMTransformUtils LLVMSupport LLVMCore ${CMAKE_CURRENT_LIST_DIR}/libsubstitute.dylib)
#target_link_options(LLVMHanabi PRIVATE "-undefined dynamic_lookup")
