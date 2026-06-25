#pragma once
#include <jni.h>
#include <jvmti.h>
#include <vector>
#include <string>
 
class JNIHelper {
public:
    static JavaVM* g_jvm;
    static JNIEnv* g_env;
 
    static bool Initialize();
    static bool IsReady() { return g_env != nullptr; }
    static jclass FindClassByName(const char* name);
 
    static jobject GetFieldObject(JNIEnv* env, jobject obj, const char* fieldName);
    static double GetDoubleField(JNIEnv* env, jobject obj, const char* fieldName);
    static float GetFloatField(JNIEnv* env, jobject obj, const char* fieldName);
 
    // VTable Typedefs
    typedef jclass(JNICALL* tFindClass)(JNIEnv*, const char*);
    typedef jmethodID(JNICALL* tGetMethodID)(JNIEnv*, jclass, const char*, const char*);
    typedef jobject(JNICALL* tGetObjectField)(JNIEnv*, jobject, jfieldID);
 
    static tFindClass oFindClass;
    static tGetMethodID oGetMethodID;
    static tGetObjectField oGetObjectField;
 
    static jfieldID FindFieldByType(JNIEnv* env, jclass clazz, const char* typeSig);
 
private:
    static void* Hook(void* func);
    static void* Hook3Hit(void* func);
    static void HookFunctions();
};
