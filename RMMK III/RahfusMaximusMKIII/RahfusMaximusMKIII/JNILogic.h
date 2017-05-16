#include <jni.h>
#ifndef _Included_JNILogic
#define _Included_JNILogic
#ifdef __cplusplus
extern "C" {
#endif
	JNIEXPORT void JNICALL Java_sc_rm_logic_JNILogic_initialize(JNIEnv*, jobject, jint, jint);
	JNIEXPORT void JNICALL Java_sc_rm_logic_JNILogic_calculateSendMove(JNIEnv*, jobject, jint, jint);
	JNIEXPORT void JNICALL Java_sc_rm_logic_JNILogic_gameEnded(JNIEnv*, jobject);
#ifdef __cplusplus
}
#endif
#endif