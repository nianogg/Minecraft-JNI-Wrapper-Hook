#include "jni_helper.h"
#include <Windows.h>
 
JavaVM* JNIHelper::g_jvm = nullptr;
JNIEnv* JNIHelper::g_env = nullptr;
 
JNIHelper::tFindClass JNIHelper::oFindClass = nullptr;
JNIHelper::tGetMethodID JNIHelper::oGetMethodID = nullptr;
JNIHelper::tGetObjectField JNIHelper::oGetObjectField = nullptr;
 
void* JNIHelper::Hook(void* func) {
    uint8_t* f = (uint8_t*)func;
    if (f[0] == 0xE9) return (void*)(f + 5);
    if (f[0] == 0x49 && f[1] == 0xBA) return (void*)*(uint64_t*)(f + 2);
    return func;
}
 
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
 
void JNIHelper::HookFunctions() {
    void** vt = *(void***)g_env;
    oFindClass = (tFindClass)Hook(vt[6]);
    oGetObjectField = (tGetObjectField)Hook3Hit(vt[95]); 
}
 
bool JNIHelper::Initialize() {
    HMODULE jvmdll = GetModuleHandleA("jvm.dll");
    if (!jvmdll) return false;
 
    typedef jint(JNICALL* p_GetEnv)(JavaVM*, JNIEnv**, jint);
    p_GetEnv GetEnv = (p_GetEnv)((uintptr_t)jvmdll + 0x144080);
 
    if (GetEnv(nullptr, &g_env, JNI_VERSION_1_8) != JNI_OK) return false;
    if (g_env->GetJavaVM(&g_jvm) != JNI_OK || !g_jvm) return false;
 
    HookFunctions();
    return true; 
}
