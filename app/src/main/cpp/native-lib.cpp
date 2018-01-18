#include <jni.h>
#include <string>
#include "record.h"

using namespace android;
extern "C"
JNIEXPORT jstring

JNICALL
Java_com_capsane_dexposeddemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++ \nDexposed针对DVM，需要Android版本小于5.0";
    return env->NewStringUTF(hello.c_str());
}
