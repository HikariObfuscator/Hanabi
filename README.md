# 花火
Hassle-free Obfuscator-Enabled Apple Clang without any sort of compromise.
![](https://github.com/HikariObfuscator/NatsukoiHanabi/blob/master/Demo.jpg?raw=true)

# License
Please refer to [License](https://github.com/HikariObfuscator/Hikari/wiki/License).

Note that this linked version of license text overrides any artifact left in source code

# Must be this tall to ride
Due to its hackish nature (Which is why I don't want to do this in the first place), you should probably know some LLVM/macOS Hooking/Binary Patching and stuff to debug this thing

# Using Release Builds
- Extract the zipped file and move two extracted dylibs under ``/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin ``
- Run ``optool install -c load -p @executable_path/libLLVMHanabi.dylib -t /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -b /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/``

# Building
- ``$(LLVM_SOURCE_PATH)`` The path that stored Hikari's main repo with submodules properly fetched
- ``${LLVM_BUILD_PATH}`` The path you prepare to build in. Note that you need a seperate folder and must not reuse existing build for upstream Hikari

## Obtaining Source
- ``git clone https://github.com/HikariObfuscator/Hanabi.git $(LLVM_SOURCE_PATH)/projects/``

## Build
- ``cmake $(LLVM_SOURCE_PATH) -DCMAKE_BUILD_TYPE=Release -DLLVM_ABI_BREAKING_CHECKS=FORCE_OFF -G Ninja``
- ``ninja LLVMHanabi``
- Copy ``$(LLVM_BUILD_PATH)/lib/libLLVMHanabi.dylib`` to ``/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/``
- Copy ``$(LLVM_SOURCE_PATH)/projects/Hanabi/libsubstitute.dylib`` to ``/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/``

# Patching

You need to build ``https://github.com/alexzielenski/optool`` and put it in your $PATH, then
``optool install -c load -p @executable_path/libLLVMHanabi.dylib -t /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang`` (Remember to backup your original Clang first)

# Using

Due to many LLVM internal design choices, you can no longer pass options from command line and instead you'll have to use environment variables. Currently it supports the following:  

- SPLITOBF EnableBasicBlockSplit
- SUBOBF EnableSubstitution
- ALLOBF EnableAllObfuscation
- FCO EnableFunctionCallObfuscate
- STRCRY EnableStringEncryption
- INDIBRAN EnableIndirectBranching
- FUNCWRA EnableFunctionWrapper
- BCFOBF EnableBogusControlFlow
- ACDOBF EnableAntiClassDump
- CFFOBF EnableFlattening

Basically it means you will need to follow the following steps:

- Open up a terminal
- export the env vars you need
- ``/Applications/Xcode.app/Contents/MacOS/Xcode``

This should get you a properly initialized Xcode.

Or alternatively, manually edit [LoadEnv() in Obfuscation.cpp](https://github.com/HikariObfuscator/Core/blob/master/Obfuscation.cpp#L59) to initialize the flags in a way you prefer

# Known Issues
- LLVM 6.0.1 (which Xcode 10.1 and before is using) has bugs related to ``indirectbr`` CodeGeneration, you might get a crash if you enable ``INDIBRAN``. Try updating your Xcode version

# How it works
Strictly speaking, many changes are done to the Hikari Core to reduce LLVM library dependencies. This plus a custom ``CMakeLists.txt`` allows us to redirect almost all Hikari Core's LLVM API Usage back to Apple Clang's implementation. The rest is just plain version-by-version analysis to manually resolve the remaining symbols that are not exported. Since probably 99.99% of the LLVM APIs are redirected back, this solution has the maximum compatibility when properly compiled and injected, comparing to previous naive implementations.

# Release Versioning Scheme
The releases has a versioning scheme like ``6.0@9ab263`` where the first part represents LLVM base version and the second represents Hikari Core's git commit hash. In this case, it means the release is tested to work on a Xcode version that uses LLVM6.0. You can refer to ``Toolchain version history`` in [Xcode - Wikipedia](https://en.wikipedia.org/wiki/Xcode) and uses the ``LLVM`` column to find the matching Xcode version, which is this case is ``Xcode 10.0`` and ``Xcode 10.1``.
# Credits

- Thanks to [@AloneMonkey](https://github.com/AloneMonkey) for compiling substitute and ship it with his amazing project [MonkeyDev](https://github.com/AloneMonkey/MonkeyDev/blob/master/MFrameworks/libsubstitute.dylib)
- Thanks to [@UESTC-LXY](https://github.com/UESTC-LXY) for testing and discussion because I didn't bother to do so.
- Thanks to[@qokelate](https://github.com/qokelate) for initially discovering the broken CMake script and testing the new fix as well as suggestions to this README
