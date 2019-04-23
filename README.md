# 花火
Hassle-free Obfuscator-Enabled Apple Clang without any sort of compromise.
![](https://github.com/HikariObfuscator/NatsukoiHanabi/blob/master/Demo.jpg?raw=true)

# License
Please refer to [License](https://github.com/HikariObfuscator/Hikari/wiki/License).

Note that this linked version of license text overrides any artifact left in source code

# Must be this tall to ride
Due to its hackish nature (Which is why I don't want to do this in the first place), you should probably know some LLVM/macOS Hooking/Binary Patching and stuff to debug this thing

# Modifications
This project uses a slightly modified Hikari upstream ported back to LLVM 6.0.1 (The version Apple is using) to avoid any potential issues, although it should work with LLVM7.0 as base. Most of the modifications are done to load options without hacking ``llvm::cl`` as well as some LLVM building configurations.

# Building

## Obtaining Source
- ``git clone --recursive https://github.com/HikariObfuscator/Hanabi.git $(PATH/TO/LLVM_SRC_DIR)/projects``

## Build
- Follow your normal build procedure but build only the target ``LLVMHanabi`` which should hopefully build the dependencies as well
- Copy ``$(LLVM_BUILD_PATH)/lib/libLLVMHanabi.dylib`` to ``/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/``
- Copy ``$(HANABI_SOURCE_PATH)/libsubstitute.dylib`` to ``/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/``

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

Or alternatively, manually edit [LoadEnv() in Obfuscation.cpp](https://github.com/HikariObfuscator/NatsukoiHanabi/blob/master/LLVM/lib/Transforms/Obfuscation/Obfuscation.cpp#L59) to initialize the flags in a way you prefer

# Known Issues
- LLVM 6.0.1 (which Apple's Clang and this project is currently based on) has bugs related to ``indirectbr`` CodeGeneration, you might get a crash if you enable ``INDIBRAN``. Another more robust solution would be hook those parts and pipe the CodeGeneration pipeline back to LLVM7.0 but I couldn't be less bothered for that
- ~~Floating Point Instructions will sometimes result in an infinite loop in shipped LLVM's ``ConstantFP``(Thanks to @UESTC-LXY for discovering this)~~ This has been fixed by adding an option in upstream core to disable FP operations

# Future enhancements
- Hijacking PMB is IMHO a little bit too late into the compilation pipeline. A better approach would be hijacking Clang's raw AST, reuse our own shipped Clang CG, run through the rest of our own optimization pipeline then transfer the processed LLVM IR back to Apple LLVM? Maybe? We need more research into Apple's LLVM for this
- Mapping ``LLVMContext`` with Apple's Context. Since the majority of IR/Constant is binded with one specific context
- We can use LLVM's very own ``-mllvm -load`` options, however in my personal experience that thing is very troublesome and usually even more unstable.

# Credits

- Thanks to @AloneMonkey for compiling substitute and ship it with his amazing project [MonkeyDev](https://github.com/AloneMonkey/MonkeyDev/blob/master/MFrameworks/libsubstitute.dylib)
- Thanks to @UESTC-LXY for testing and discussion because I didn't bother to do so.
