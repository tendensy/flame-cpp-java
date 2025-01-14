#include "jniutil.h"

CJavaWrapper::CJavaWrapper(JNIEnv *env, jobject activity)
{
    this->activity = env->NewGlobalRef(activity);

    jclass clas = env->GetObjectClass(activity);
    jclass nvEventClass = env->GetObjectClass(activity);
    
    this->s_SetUseFullScreen = env->GetMethodID(clas, "setUseFullscreen", "(I)V");
    this->s_ShowClientSettings = env->GetMethodID(clas, "showClientSettings", "()V");
    this->s_ShowInputLayout = env->GetMethodID(clas, "showInputLayout", "()V");
    this->s_HideInputLayout = env->GetMethodID(clas, "hideInputLayout", "()V");
    this->s_GetKeyboardString = env->GetMethodID(clas, "getClipboardText", "()[B");
   
    this->s_MakeDialog = env->GetMethodID(nvEventClass, "showDialog", "(IILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    this->s_showNotification = env->GetMethodID(nvEventClass, "showNotification","(ILjava/lang/String;ILjava/lang/String;Ljava/lang/String;)V");
	
    env->DeleteLocalRef((jobject)clas);

    Log("Inject JavaWrapper..");
}

void CJavaWrapper::SetUseFullScreen(int b)
{
    JNIEnv* p;
    CMain::javaVM->GetEnv((void**)&p, JNI_VERSION_1_6);
    p->CallVoidMethod(this->activity, this->s_SetUseFullScreen, b);
}

void CJavaWrapper::ShowClientSettings()
{
    JNIEnv* p;
    CMain::javaVM->GetEnv((void**)&p, JNI_VERSION_1_6);
    p->CallVoidMethod(this->activity, this->s_ShowClientSettings);
}

void CJavaWrapper::ShowInputLayout()
{
    JNIEnv* p;
    CMain::javaVM->GetEnv((void**)&p, JNI_VERSION_1_6);
    p->CallVoidMethod(this->activity, this->s_ShowInputLayout);
}

void CJavaWrapper::HideInputLayout()
{
    JNIEnv* p;
    CMain::javaVM->GetEnv((void**)&p, JNI_VERSION_1_6);
    p->CallVoidMethod(this->activity, this->s_HideInputLayout);
}

const char* CJavaWrapper::GetClipboardString()
{
    JNIEnv* p;
    CMain::javaVM->GetEnv((void**)&p, JNI_VERSION_1_6);
    jstring res =  static_cast<jstring>(p->CallObjectMethod(this->activity, this->s_GetKeyboardString));
    jboolean iscopy;
    const char *tempString = p->GetStringUTFChars(res, &iscopy);
    if(iscopy == JNI_TRUE) p->ReleaseStringUTFChars(res, tempString);
    p->DeleteLocalRef(res);

    return tempString;
}

void CJavaWrapper::MakeDialog(int dialogId, int dialogTypeId, char* caption, char* content, char* leftBtnText, char* rightBtnText)
{
    JNIEnv* env;
    CMain::javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (!env)
    {
	   Log("No env");
	   return;
    }

    jclass strClass = env->FindClass("java/lang/String");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jstring encoding = env->NewStringUTF("UTF-8");
    jbyteArray bytes = env->NewByteArray(strlen(caption));
    env->SetByteArrayRegion(bytes, 0, strlen(caption), (jbyte*)caption);
    jstring str1 = (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
    
    jclass strClass1 = env->FindClass("java/lang/String");
    jmethodID ctorID1 = env->GetMethodID(strClass1, "<init>", "([BLjava/lang/String;)V");
    jstring encoding1 = env->NewStringUTF("UTF-8");
    jbyteArray bytes1 = env->NewByteArray(strlen(content));
    env->SetByteArrayRegion(bytes1, 0, strlen(content), (jbyte*)content);
    jstring str2 = (jstring)env->NewObject(strClass1, ctorID1, bytes1, encoding1);
    
    jclass strClass2 = env->FindClass("java/lang/String");
    jmethodID ctorID2 = env->GetMethodID(strClass2, "<init>", "([BLjava/lang/String;)V");
    jstring encoding2 = env->NewStringUTF("UTF-8");
    jbyteArray bytes2 = env->NewByteArray(strlen(leftBtnText));
    env->SetByteArrayRegion(bytes2, 0, strlen(leftBtnText), (jbyte*)leftBtnText);
    jstring str3 = (jstring)env->NewObject(strClass2, ctorID2, bytes2, encoding2);
    
    jclass strClass3 = env->FindClass("java/lang/String");
    jmethodID ctorID3 = env->GetMethodID(strClass3, "<init>", "([BLjava/lang/String;)V");
    jstring encoding3 = env->NewStringUTF("UTF-8");
    jbyteArray bytes3 = env->NewByteArray(strlen(rightBtnText));
    env->SetByteArrayRegion(bytes3, 0, strlen(rightBtnText), (jbyte*)rightBtnText);
    jstring str4 = (jstring)env->NewObject(strClass3, ctorID3, bytes3, encoding3);

    env->CallVoidMethod(this->activity, this->s_MakeDialog, dialogId, dialogTypeId, str1, str2, str3, str4);
}

void CJavaWrapper::ShowNotification(int type, char* text, int duration, char* actionforBtn, char* textBtn) 
{
	JNIEnv* env;
    CMain::javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (!env)
	{
		Log("No env");
		return;
	}

	jclass strClass = env->FindClass("java/lang/String");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jstring encoding = env->NewStringUTF("UTF-8");

    jbyteArray bytes = env->NewByteArray(strlen(text));
    env->SetByteArrayRegion(bytes, 0, strlen(text), (jbyte*)text);
    jstring jtext = (jstring) env->NewObject(strClass, ctorID, bytes, encoding);

	bytes = env->NewByteArray(strlen(actionforBtn));
    env->SetByteArrayRegion(bytes, 0, strlen(actionforBtn), (jbyte*)actionforBtn);
    jstring jactionforBtn = (jstring) env->NewObject(strClass, ctorID, bytes, encoding);

	bytes = env->NewByteArray(strlen(textBtn));
    env->SetByteArrayRegion(bytes, 0, strlen(textBtn), (jbyte*)textBtn);
    jstring jtextBtn = (jstring) env->NewObject(strClass, ctorID, bytes, encoding);

	env->CallVoidMethod(this->activity, this->s_showNotification, type, jtext, duration, jactionforBtn, jtextBtn);
}
