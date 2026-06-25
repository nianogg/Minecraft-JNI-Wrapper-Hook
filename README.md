![C++](https://img.shields.io/badge/language-C++-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Status](https://img.shields.io/badge/status-active-brightgreen.svg)

# Minecraft JNI Wrapper

simple JNI helper base for minecraft internal clients in C++. it allows you to hook JVM functions and use reflection easily. :deal:

## Features

* **VTable Hooking:** Intercepts core JNI functions (`GetMethodID`, `FindClass`, `GetObjectField`).
* **Memory Wrappers:** Direct retrieval of fields (`float`, `double`, `boolean`) from the JVM.
* **Signature Resolution:** Bypasses static obfuscation using Type Signatures.
* **Hook3Hit:** Custom JMP instruction bypass for memory hook chains.

## Requirements
* **Compiler:** MSVC (Visual Studio 2019/2022).
* **Environment:** x64 architecture.
* **Dependencies:** Standard Windows SDK, Java Native Interface (JNI) headers.

## Usage

Initialize the wrapper in your main thread upon injection:

```cpp
if (JNIHelper::Initialize()) {
    printf("[+] JNI Hooked\n");
}
JMP Bypass Logic
Handles chained relative and absolute indirect jumps to resolve the final function address:

C++
void* JNIHelper::Hook3Hit(void* func) {
    uint8_t* f = (uint8_t*)func;
    
    if (f[0] == 0xE9) {
        int32_t offset1 = *(int32_t*)(f + 1);
        f = f + 5 + offset1;
        
        if (f[0] == 0xFF && f[1] == 0x25) {
            int32_t offset2 = *(int32_t*)(f + 2);
            void** ptrAddr = (void**)(f + 6 + offset2);
            f = (uint8_t*)*ptrAddr;
        }
        return (void*)f;
    }
    return func;
}
