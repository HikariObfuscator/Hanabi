# 夏恋花火
Hassle-free Obfuscator-Enabled Apple Clang without any sort of compromise. More to come later

# Building
Create a folder called ``build/`` in project root, inside it build the attached LLVM with ``-DCMAKE_BUILD_TYPE=Release -DLLVM_ABI_BREAKING_CHECKS=FORCE_OFF -G Ninja`` and ``ninja libLLVM.dylib``
