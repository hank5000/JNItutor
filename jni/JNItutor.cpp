#include <jni.h>
#include<android/log.h>
#include<string.h>


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


jobject CreateSimpleJavaClass(JNIEnv* env,char* class_path)
{
	jclass other_class_t = env->FindClass(class_path);
	jmethodID other_contructer_id = env->GetMethodID(other_class_t,"<init>","()V");
	jobject other_class_instant = env->NewObject(other_class_t,other_contructer_id);
	return other_class_instant;
}

typedef class JNIFastUseJavaClass
{
public:
	JNIEnv* mJNIEnv;
	jclass mjclass;
	JNIFastUseJavaClass(JNIEnv* env, char* path)
	{
		mJNIEnv = env;
		mjclass = mJNIEnv->FindClass(path);
	}
	/*
	 *  Use this function to set value of the class of java
	 *  If value type is int or String, then it can set directly
	 *  If value type is other class, then it need new first, and using class_path to set it.
	 *
	 *
	 *  var_name is meaning the variable name in class
	 *  value_type is meaning the value type in class :
	 *	  	 	type         		value_type
	 *--------------------------------------------
	 *         string             string/String
	 *    	  int/int[]             int/Int
	 *    	 byte/byte[]           byte/Byte
	 *     Other Structure        "Class Path"
	 *
	 *
	 */
	void setValue(jobject class_instant, char* var_name, char* value_type, void* data, int data_size)
	{
		if(value_type[0]!='L')
		{
			LOGD("set variable %s , type is %s",var_name, value_type);

			if(!strcmp("Int",value_type) || !strcmp("int",value_type))
			{
				LOGD("it is int");
				int* int_data = (int*) data;
				if(data_size==sizeof(int))
				{
					jfieldID field_id = mJNIEnv->GetFieldID(mjclass, var_name, "I");
					mJNIEnv->SetIntField(class_instant,field_id,*int_data);
				}
				else
				{
					int NumberOfInt = data_size/sizeof(int);
					jfieldID field_id = mJNIEnv->GetFieldID(mjclass, var_name, "[I");	// Translate :
					jintArray jFillIntArray = mJNIEnv->NewIntArray(NumberOfInt);		// 		int *jFillIntArray = new Int[NumberOfInt];
					mJNIEnv->SetIntArrayRegion(jFillIntArray,0,NumberOfInt,int_data);   // 		memcpy(jFillIntArray,int_data,sizeof(int)*NumberOfInt);
					mJNIEnv->SetObjectField(class_instant,field_id,jFillIntArray);		// 		class_instant->var_name = jFillIntArray;
					mJNIEnv->DeleteLocalRef(jFillIntArray);
				}
			}
			else if(!strcmp("string",value_type) || !strcmp("String",value_type))
			{
				char* str_data = (char*) data;
				LOGD("%s",str_data);
																									// Translate
				jstring jstr = mJNIEnv->NewStringUTF(str_data);										// String *jstr = new String(); memcpy(jstr,str_data,...)
				jfieldID field_id = mJNIEnv->GetFieldID(mjclass, var_name, "Ljava/lang/String;");
				mJNIEnv->SetObjectField(class_instant, field_id, jstr);								// class_instant->var_name = jstr;
				mJNIEnv->DeleteLocalRef(jstr);
			}
		}
		else
		{
			LOGD("set variable %s , type is %s",var_name,value_type);

			jobject* jobj_data = (jobject*) data;
			jfieldID field_id = mJNIEnv->GetFieldID(mjclass, var_name, value_type);
			mJNIEnv->SetObjectField(class_instant, field_id, *jobj_data);
		}
	}
} JavaClass;





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
#define USE_FASTDO 1

#if USE_FASTDO
JNIEXPORT void JNICALL CAST_JNI(CallJNIandFillUpSomething, jobject somethingclass_instant)
{
	//// the somthingclass_instant should be instant, not a null pointer
	//// so somethingclass_instant should be new in the java side first.

	LOGD("Come to CallJNIandFillUpSomething in C");
	jclass somthing_class_type = env->FindClass("com/example/jnitutor/SomethingClass");

	env->NewGlobalRef(somethingclass_instant);

	jfieldID fieldId_mInt = env->GetFieldID(somthing_class_type,"mInt","I");
	jfieldID fieldId_mIntArray = env->GetFieldID(somthing_class_type,"mIntArray","[I");
	jfieldID fieldId_mString = env->GetFieldID(somthing_class_type,"mString","Ljava/lang/String;");
	jfieldID fieldId_mOtherClass = env->GetFieldID(somthing_class_type,"mOtherClass","Lcom/example/jnitutor/OtherClass;");

	char* SomeCls_path = "com/example/jnitutor/SomethingClass";
	JNIFastUseJavaClass* SomeCls_fastDo = new JNIFastUseJavaClass(env,SomeCls_path);

	// set int
	int FillInt = 5;
	SomeCls_fastDo->setValue(somethingclass_instant, "mInt","int",(void*)(&FillInt),sizeof(int));

	// set int array
	int NumOfInt = 2;
	int FillIntArray[NumOfInt];
	for(int i=0;i<NumOfInt;i++)
	{
		FillIntArray[i] = i + 1;
	}

	SomeCls_fastDo->setValue(somethingclass_instant, "mIntArray","int",(void*)(&FillIntArray),sizeof(int)*2);

	// set String
	jstring jstr;
	char* JNIStr = "This sentence is Filled in JNI";
	SomeCls_fastDo->setValue(somethingclass_instant, "mString" , "String", (void*)(JNIStr), NULL);

	// set private other class
    jobject other_class_instant = CreateSimpleJavaClass(env,"com/example/jnitutor/OtherClass");
    JNIFastUseJavaClass* other_class_fastdo = new JNIFastUseJavaClass(env, "com/example/jnitutor/OtherClass");
	int OtherInt = 6;
    other_class_fastdo->setValue(other_class_instant,"mOtherInt","Int",(void*)(&OtherInt),sizeof(int));

	//---TRY DO USE---//
	SomeCls_fastDo->setValue(somethingclass_instant, "mOtherClass" , "Lcom/example/jnitutor/OtherClass;", (void*)(&other_class_instant), NULL);
	////////////////////
	env->DeleteLocalRef(other_class_instant);
	env->DeleteLocalRef(somethingclass_instant);
}

#else
////

JNIEXPORT void JNICALL CAST_JNI(CallJNIandFillUpSomething, jobject somethingclass_instant)
{
	//// the somthingclass_instant should be instant, not a null pointer
	//// so somethingclass_instant should be new in the java side first.

	LOGD("Come to CallJNIandFillUpSomething in C");
	jclass somthing_class_type = env->FindClass("com/example/jnitutor/SomethingClass");

	env->NewGlobalRef(somethingclass_instant);

	jfieldID fieldId_mInt = env->GetFieldID(somthing_class_type,"mInt","I");
	jfieldID fieldId_mIntArray = env->GetFieldID(somthing_class_type,"mIntArray","[I");
	jfieldID fieldId_mString = env->GetFieldID(somthing_class_type,"mString","Ljava/lang/String;");
	jfieldID fieldId_mOtherClass = env->GetFieldID(somthing_class_type,"mOtherClass","Lcom/example/jnitutor/OtherClass;");

	char* SomeCls_path = "com/example/jnitutor/SomethingClass";
	JNIFastUseJavaClass* SomeCls_fastDo = new JNIFastUseJavaClass(env,SomeCls_path);

	// set int
	int FillInt = 5;
	env->SetIntField(somethingclass_instant, fieldId_mInt, FillInt);



	// set int array
	int NumOfInt = 2;
	int FillIntArray[NumOfInt];
	for(int i=0;i<NumOfInt;i++)
	{
		FillIntArray[i] = i + 1;
	}
	jintArray jFillIntArray = env->NewIntArray(NumOfInt);
	env->SetIntArrayRegion(jFillIntArray,0,NumOfInt,FillIntArray);
	env->SetObjectField(somethingclass_instant,fieldId_mIntArray,jFillIntArray);
	env->DeleteLocalRef(jFillIntArray);



	// set String
	jstring jstr;
	char* JNIStr = "This sentence is Filled in JNI";
	jstr = env->NewStringUTF(JNIStr);
	env->SetObjectField(somethingclass_instant, fieldId_mString, jstr);
	env->DeleteLocalRef(jstr);


	// set private other class

	// new the otherclass
	jclass other_class_t = env->FindClass("com/example/jnitutor/OtherClass");
	jmethodID other_contructer_id = env->GetMethodID(other_class_t,"<init>","()V");
	jobject other_class_instant = env->NewObject(other_class_t,other_contructer_id);
	// fill the other class member mOtherInt
	int OtherInt = 6;
	jfieldID fieldId_other_int = env->GetFieldID(other_class_t, "mOtherInt" , "I");
	env->SetIntField(other_class_instant, fieldId_other_int, OtherInt);
	// Set OtherClass to SomethingClass
	env->SetObjectField(somethingclass_instant, fieldId_mOtherClass , other_class_instant);
	env->DeleteLocalRef(other_class_instant);


	env->DeleteLocalRef(somethingclass_instant);
}

#endif



}
