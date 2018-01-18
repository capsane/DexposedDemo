#include <jni.h>
#include <string>
#include "record.h"

using namespace android;
extern "C"
JNIEXPORT jstring

JNICALL
Java_com_capsane_dexndk_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

//    start(1,2,3,4,5,6);

    return env->NewStringUTF(hello.c_str());
}
