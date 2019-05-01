cmake_minimum_required(VERSION "3.13.0")
add_library(LLVMHanabiDeps SHARED #This is for linking a minimum subset of LLVM needed which serves as the escape plan
  ${CMAKE_CURRENT_LIST_DIR}/Dummy.cpp
)
add_dependencies(LLVMHanabiDeps
  LLVMCore
  LLVMSupport
)
add_library(LLVMHanabi SHARED
  ${CMAKE_CURRENT_LIST_DIR}/Loader.cpp
)
add_dependencies(LLVMHanabi
  LLVMHanabiDeps
  LLVMObfuscation
)
target_link_libraries(LLVMHanabi PRIVATE LLVMObfuscation ${CMAKE_CURRENT_LIST_DIR}/libsubstitute.dylib)
target_link_options(LLVMHanabi PRIVATE -undefined PRIVATE dynamic_lookup PRIVATE -flat_namespace)
target_compile_options(LLVMHanabi PRIVATE -Wno-dollar-in-identifier-extension PRIVATE -Wno-variadic-macros PRIVATE)

target_link_libraries(LLVMHanabiDeps PRIVATE LLVMCore LLVMSupport ${CMAKE_CURRENT_LIST_DIR}/libsubstitute.dylib)
target_link_options(LLVMHanabiDeps PRIVATE -undefined PRIVATE dynamic_lookup PRIVATE -all_load)
