#pragma once
#include "main.h"

class CJavaWrapper
{
public:
    CJavaWrapper(JNIEnv *env, jobject activity);
    ~CJavaWrapper() {};

    const char* GetClipboardString();
    void SetUseFullScreen(int b);
    void ShowClientSettings();
    void ShowInputLayout();
    void HideInputLayout();

    void MakeDialog(int dialogId, int dialogTypeId, char* caption, char* content, char* leftBtnText, char* rightBtnText);
    void ShowNotification(int type, char* text, int duration, char* actionforBtn, char* textBtn);
	
    jobject activity;
    jmethodID s_SetUseFullScreen;
    jmethodID s_ShowClientSettings;
    jmethodID s_ShowInputLayout;
    jmethodID s_HideInputLayout;
    jmethodID s_GetKeyboardString;

    jmethodID s_MakeDialog;
    jmethodID s_showNotification;
};