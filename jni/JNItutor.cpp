#include <jni.h>
#include<android/log.h>
extern "C"{
#define TAG "JNItutor"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__)

#define CAST_JNI_FN(fn) Java_com_example_jnitutor_MainActivity_##fn
#define CAST_JNI_IN(fn, arg...) CAST_JNI_FN(fn) (JNIEnv* env, jobject obj, arg)
#define CAST_JNI(fn, arg...) CAST_JNI_IN(fn, arg)

/*
JNI NOTE
java side will call "function_name(function_input/output)" and call to JNI(C/C++) side the following function


JAVA                JNI
boolean             jboolean                    C/C++ unsigned 8 bits
byte                jbyte                       C/C++ signed 8 bits
char                jchar                       C/C++ unsigned 16 bits
short               jshort                      C/C++ signed 16 bits
int                 jint                        C/C++ signed 32 bits
long                jlong                       C/C++ signed 64 bits
float               jfloat                      C/C++  32位浮点型
double              jdouble                     C/C++  64位浮点型
void                void                        N/A

//-----implement a JNI fucntion-----//

#define CAST_JNI_FN(fn) Java_com_example_jnitutor_MainActivity_##fn
#define CAST_JNI_IN(fn, arg...) CAST_JNI_FN(fn) (JNIEnv* env, jobject obj, arg)
#define CAST_JNI(fn, arg...) CAST_JNI_IN(fn, arg)
	JNIEXPORT jint JNICALL CAST_JNI(function_name, function_input or function_out){---something---}  // the native function should be the Fixed form like the left.
or
	JNIEXPORT jint JNICALL Java_com_example_jnitutor_MainActivity_function_name(JNIEnv* env, jobject obj, function_input or function_out){---something---}

and java side should be adding "native void function_name(input); ", then java can call function_name(input) directly.

JAVA Side                                                   JNI
---------------------------------------------------------------------------------------------------------------------
function_name1(jint i)  -------> Java_com_example_jnitutor_MainActivity_function_name1(JNIEnv* env, jobject obj, jint i)
function_name2(jstring s) -----> Java_com_example_jnitutor_MainActivity_function_name2(JNIEnv* env, jobject obj, jstring s)

// jint can use directly
// jstring cannot use directly, it should be convert by using " const char *NativeName = env->GetStringUTFChars(s, 0); "



//------new a java Class on the JNI side----//
	jclass  j_JNITutor;
	jobject j_mJNITutor;
	j_JNITutor = env->FindClass("com/example/jnitutor/MainActivity"); 			// this is just a form of class, not a instant of class
	jmethodID construction_id = env->GetMethodID(j_JNITutor,"<init>","()V"); 	// get the construct function id
	j_mJNITutor = env->NewObject(j_JNITutor,construction_id); 					// using the construct function to create a instant of j_JNITutor class
*/

JNIEXPORT void JNICALL CAST_JNI(CallJNIInt,jint input_int)
{
	LOGD("-----%d call me lo",input_int);
}

JNIEXPORT void JNICALL CAST_JNI(CallJNIString,jstring input_string)
{
	const char *NativeName = env->GetStringUTFChars(input_string, 0);
	LOGD("-----%s call me lo",NativeName);
}

JNIEXPORT void JNICALL CAST_JNI(CallJNIandCallback,jstring input_string)
{
	const char *NativeName = env->GetStringUTFChars(input_string, 0);
	LOGD("-----%s call me lo",NativeName);


	LOGD("JNI Call FunctionD!");
	// use the form of class to function FunctionD_id and call it back by using obj (it is java class handle)
	jclass j_JNITutor = env->FindClass("com/example/jnitutor/MainActivity");
	jmethodID FunctionD_id = env->GetMethodID(j_JNITutor,"FunctionD","()V");
	env->CallVoidMethod(obj,FunctionD_id);
}




}
