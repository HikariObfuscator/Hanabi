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
- Download [libsubstitute.dylib](https://github.com/HikariObfuscator/Hanabi/raw/master/libsubstitute.dylib) and move it to ``/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin ``
- See [Patching](#patching). Ignore library not found in that release version

# Building
- ``$(LLVM_SOURCE_PATH)`` The path that stored Hikari's main repo with submodules properly fetched. It's suggested to use a Hikari branch that matches your Apple Clang's LLVM version. See [Release Versioning Scheme](#release-versioning-scheme) to see how to find the LLVM version of your Clang
- ``${LLVM_BUILD_PATH}`` The path you prepare to build in. Note that you need a seperate folder and must not reuse existing build for upstream Hikari

## Obtaining Source
- ``git clone https://github.com/HikariObfuscator/Hanabi.git $(LLVM_SOURCE_PATH)/projects/``

## Build
- ``cmake $(LLVM_SOURCE_PATH) -DCMAKE_BUILD_TYPE=Release -DLLVM_ABI_BREAKING_CHECKS=FORCE_OFF -G Ninja``
- ``ninja LLVMHanabi``
- Copy ``$(LLVM_BUILD_PATH)/lib/libLLVMHanabiDeps.dylib`` to ``/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/``
- Copy ``$(LLVM_BUILD_PATH)/lib/libLLVMHanabi.dylib`` to ``/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/``
- Copy ``$(LLVM_SOURCE_PATH)/projects/Hanabi/libsubstitute.dylib`` to ``/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/``

# Patching

You need to build ``https://github.com/alexzielenski/optool`` and put it in your $PATH, then you need to patch two libraries into Clang/SwiftC.
**!!!ORDER IS VERY IMPORTANT!!!**
- ``sudo optool install -c load -p @executable_path/libLLVMHanabiDeps.dylib -t /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang``
- ``sudo optool install -c load -p @executable_path/libLLVMHanabi.dylib -t /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang``

# Known Issues
- LLVM 6.0.1 (which Xcode 10.1 and before is using) has bugs related to ``indirectbr`` CodeGeneration, you might get a crash if you enable ``INDIBRAN``. Try updating your Xcode version


# How it works
- Strictly speaking, many changes are done to the Hikari Core to reduce LLVM library dependencies.
- Loader's linking options is modified to link to no LLVM library and fully resolve them at runtime in a flat namespace, this loader is also known as ``libLLVMHanabi.dylib``
- Then, we ship a custom mimimal subset of LLVM Core Libraries which serves as the fallback plan for symbols that are not exported in Apple's binaries, this is known as ``libLLVMHanabiDeps.dylib``.
- By not linking the full LLVM suite, we are allowed to reduce build time and more importantly, allows us to pass arguments like we normally would. (``-mllvm``)


# Release Versioning Scheme
The releases has a versioning scheme like ``6.0@9ab263@LoaderV3`` where the first part represents LLVM base version, the second represents Hikari Core's git commit hash and the third one represents the Loader implementation version. In this case, it means the release is tested to work on a Xcode version that uses LLVM6.0. You can refer to ``Toolchain version history`` in [Xcode - Wikipedia](https://en.wikipedia.org/wiki/Xcode) and uses the ``LLVM`` column to find the matching Xcode version, which is this case is ``Xcode 10.0`` and ``Xcode 10.1``. This release uses [Core](https://github.com/HikariObfuscator/Core) at 9ab263 and V3 of the loader
# Credits

- Thanks to [@AloneMonkey](https://github.com/AloneMonkey) for compiling substitute and ship it with his amazing project [MonkeyDev](https://github.com/AloneMonkey/MonkeyDev/blob/master/MFrameworks/libsubstitute.dylib)
- Thanks to [@UESTC-LXY](https://github.com/UESTC-LXY) for testing and discussion because I didn't bother to do so.
- Thanks to[@qokelate](https://github.com/qokelate) for initially discovering the broken CMake script and testing the new fix as well as suggestions to this README
