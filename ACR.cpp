#include <stdio.h>
#include <jni.h>
#include <com_example_acr_acrjni.h>
#include <main.h>



JNIEXPORT void JNICALL Java_com_example_acr_acrjni_ACRLoadModelBuildNet
  (JNIEnv *env, jobject thiz, jstring modelPath, jstring cmdPath)
{
	const char *model,*cmd;
	model=env->GetStringUTFChars(modelPath,0);
	cmd=env->GetStringUTFChars(cmdPath,0);
	ACRLoadModelBuildNet(model,cmd);
}




JNIEXPORT jint JNICALL Java_com_example_acr_acrjni_ACRWavDecode
  (JNIEnv *env, jobject thiz, jstring wavPath)
{
	int id;
	const char *wav;
	wav=env->GetStringUTFChars(wavPath,0);
	id=ACRWavDecode(wav);
	return id;
}



JNIEXPORT void JNICALL Java_com_example_acr_acrjni_ACRFree
  (JNIEnv *, jobject)
{
	ACRFree();
}



