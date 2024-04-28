#include <dlfcn.h>
#include "main.h"
#include "game/game.h"
#include "game/RW/RenderWare.h"
#include "net/netgame.h"
#include "gui/gui.h"
#include "gui/CFontRenderer.h"
#include "chatwindow.h"
#include "spawnscreen.h"
#include "playertags.h"
#include "dialog.h"
#include "keyboard.h"
#include "settings.h"
#include "scoreboard.h"
#include "game/snapshothelper.h"
#include "deathmessage.h"
#include "util/patch.h"
#include "checkfilehash.h"
#include "GButton.h"
#include "game/audiostream.h"
#include "gui/buttons.h"
#include "gui/interface.h"
#include "game/materialtext.h"
#include "game/loadingscreen.h"
#include "gui/gamescreen.h"
#include "jniutil.h"
#include "game/radar.h"
#include "game/customplate.h"
#include "game/firstperson.h"

uintptr_t g_libGTASA = 0;
char* g_pszStorage = nullptr;

CGame *pGame = nullptr;
CNetGame *pNetGame = nullptr;
CChatWindow *pChatWindow = nullptr;
CSpawnScreen *pSpawnScreen = nullptr;
CPlayerTags *pPlayerTags = nullptr;
CDialogWindow *pDialogWindow = nullptr;
CScoreBoard *pScoreBoard = nullptr;
CSnapShotHelper* pSnapShotHelper = nullptr;
CDeathMessage *pDeathMessage = nullptr;
CAudioStream *pAudioStream = nullptr;
CLoadingScreen* pLoadingScreen = nullptr;
CJavaWrapper *pJavaWrapper = nullptr;
CFirstPersonCamera *pFirstPersonCamera = nullptr;

CGUI *pGUI = nullptr;
CKeyBoard *pKeyBoard = nullptr;
CDebug *pDebug = nullptr;
CSettings *pSettings = nullptr;
CGButton *pGButton = nullptr;
CMaterialText *pMaterialText = nullptr;
CGameScreen* pGameScreen = nullptr;

#include "jniutil.h"

JavaVM* CMain::javaVM = nullptr;
char* CMain::BaseStorage = nullptr;
char* CMain::RootStorage = nullptr;

void InstallSpecialHooks();
void InitRenderWareFunctions();
void ApplyPatches_level0();
void MainLoop();

void CMain::Terminate()
{
	if(pNetGame && pNetGame->GetGameState() == GAMESTATE_CONNECTED) {
		pNetGame->GetRakClient()->Disconnect(500);
		Log("[RakNet] Disconnect from crash called."); 
     	}
	SaveMenuSettings();
	Log("CMain::Terminate called.");
}

void PrintSymbols(void* pc, void* lr)
{
	Dl_info info_pc, info_lr;
	if (dladdr(pc, &info_pc) != 0)
	{
		CrashLog("PC: %s", info_pc.dli_sname);
	}
	if (dladdr(lr, &info_lr) != 0)
	{
		CrashLog("LR: %s", info_lr.dli_sname);
	}
}

void handler(int signum, siginfo_t *info, void* contextPtr)
{
	auto* context = (ucontext_t*)contextPtr;

	CMain::Terminate();

	if(info->si_signo == SIGSEGV)
	{	
		CrashLog("REAL RUSSIA CLIENT CRASHED! :(");
		CrashLog("backtrace:");
		CrashLog("1: libGTASA.so + 0x%X", context->uc_mcontext.arm_pc - g_libGTASA);
		CrashLog("2: libGTASA.so + 0x%X", context->uc_mcontext.arm_lr - g_libGTASA);
		CrashLog("3: libsamp.so + 0x%X", context->uc_mcontext.arm_pc - FindLibrary("libsamp.so"));
		CrashLog("4: libsamp.so + 0x%X", context->uc_mcontext.arm_lr - FindLibrary("libsamp.so"));
		CrashLog("BUILD DATE: %s", __DATE__);
		CrashLog("VERSION: %s", CLIENT_VER);
		CrashLog("Type crash: ingame");
		CrashLog("BUILD APK VRS: b16");
		CrashLog("Patches: installed; Ingame patches: installed");
		PrintSymbols((void*)(context->uc_mcontext.arm_pc), (void*)(context->uc_mcontext.arm_lr));

		Log("Real Russia CLIENT CRASHED! :(");
		Log("backtrace:");
		Log("1: libGTASA.so + 0x%X", context->uc_mcontext.arm_pc - g_libGTASA);
		Log("2: libGTASA.so + 0x%X", context->uc_mcontext.arm_lr - g_libGTASA);
		Log("3: libsamp.so + 0x%X", context->uc_mcontext.arm_pc - FindLibrary("libsamp.so"));
		Log("4: libsamp.so + 0x%X", context->uc_mcontext.arm_lr - FindLibrary("libsamp.so"));
		Log("BUILD DATE: %s", __DATE__);
		Log("VERSION: %s", CLIENT_VER);
		Log("Type crash: ingame");
		Log("BUILD APK VRS: b16");
		Log("Patches: installed; Ingame patches: installed");
		exit(0);
	}

	return;
}

void ProtectLibs()
{
	while (true)
	{
		/* code */
	}
	
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{	
	CMain::SetVM(vm);
	g_libGTASA = FindLibrary(cryptor::create("libGTASA.so", 12).decrypt());
	if (!g_libGTASA) std::terminate();
	srand(time(nullptr));

	CPatch::InitHookStuff();
	CMain::SetRootStorage("/data/data/com.realrussia.game/");
	CMain::SetBaseStorage("/storage/emulated/0/FussRussia/");

	LoadBassLibrary();
	InitRenderWareFunctions();
	ApplyPatches_level0();
	InstallSpecialHooks();
	//CMain::InitSAMP();

	struct sigaction act{};
	act.sa_sigaction = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &act, nullptr);

	//std::thread thr(ProtectLibs);
    //thr.join();

	return JNI_VERSION_1_4;
}

void JNI_OnUnload(JavaVM *vm, void *reserved)
{
	
}

extern "C" {
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_initSAMP(JNIEnv *pEnv, jobject thiz)
	{
		CMain::InitSAMP();
		pJavaWrapper = new CJavaWrapper(pEnv, thiz);
		pJavaWrapper->SetUseFullScreen(pSettings->Get().iCutout);
	}
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_onInputEnd(JNIEnv *pEnv, jobject thiz, jbyteArray str)
	{
		pKeyBoard->OnNewKeyboardInput(pEnv, thiz, str);
	}
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_onEventBackPressed(JNIEnv *pEnv, jobject thiz)
	{
		pKeyBoard->Close();
		pChatWindow->OnExitFromInput();
	}
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_sendDialogResponse(JNIEnv* pEnv, jobject thiz, jint i3, jint i, jint i2, jbyteArray str)
	{
		jboolean isCopy = true;
		jbyte* pMsg = pEnv->GetByteArrayElements(str, &isCopy);
		jsize length = pEnv->GetArrayLength(str);

		std::string szStr((char*)pMsg, length);

		if(pNetGame) 
		{
			pNetGame->SendDialogResponse(i, i3, i2, (char*)szStr.c_str());
			pGame->FindPlayerPed()->TogglePlayerControllable(true);
		}

		pEnv->ReleaseByteArrayElements(str, pMsg, JNI_ABORT);
	}
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_sendCommand(JNIEnv* pEnv, jobject thiz, jbyteArray str)
	{
		jboolean isCopy = true;

		jbyte* pMsg = pEnv->GetByteArrayElements(str, &isCopy);
		jsize length = pEnv->GetArrayLength(str);

		std::string szStr((char*)pMsg, length);

		if(pNetGame) pNetGame->SendChatCommand((char*)szStr.c_str());

		pEnv->ReleaseByteArrayElements(str, pMsg, JNI_ABORT);
	}
	JNIEXPORT void JNICALL Java_com_nvidia_devtech_NvEventQueueActivity_togglePlayer(JNIEnv* pEnv, jobject thiz, jint toggle) 
	{
		if(toggle)
			pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->TogglePlayerControllable(false);
		else
			pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->TogglePlayerControllable(true);
	}
}

void CMain::InitSAMP()
{	
	if ( !*(uintptr_t *)(g_libGTASA + 0x61B298) && !((int (*)(const char *))(g_libGTASA + 0x179A20))("glAlphaFuncQCOM") )
  	{
    	CPatch::NOP(g_libGTASA + 0x1A6164, 4);
    	CPatch::WriteMemory(g_libGTASA + 0x1A6164, "\x70\x47", 2);
  	}
	
    Log("Initializing SAMP..");
	g_pszStorage = CMain::GetBaseStorage();	
	pSettings = new CSettings();
	CRadarRect::SetEnabled(pSettings->Get().szRadar);
	//if(!FileCheckSum()) std::terminate();
}

void CMain::InitInMenu()
{
	// TODO: сделать здесь проверку если уже всё инициализировано
	pGame 			= new CGame();
	pLoadingScreen 	= new CLoadingScreen();
	pGame->InitInMenu();

	pGUI 			= new CGUI();
	pGameScreen		= new CGameScreen();

	pGButton 		= new CGButton();
	pKeyBoard 		= new CKeyBoard();
	pFirstPersonCamera	= new CFirstPersonCamera();
	pChatWindow 	= new CChatWindow();
	pSpawnScreen 	= new CSpawnScreen();
	pPlayerTags 	= new CPlayerTags();
	pDialogWindow 	= new CDialogWindow();
	pScoreBoard 	= new CScoreBoard();
	pSnapShotHelper = new CSnapShotHelper();
	pDeathMessage 	= new CDeathMessage();
	pAudioStream 	= new CAudioStream();
	pMaterialText   = new CMaterialText();
	
	CFontRenderer::Initialise();
	CCustomPlateManager::Initialise();

	pDebug			= new CDebug();
}

void CMain::InitInGame()
{
	static bool bGameInited = false;
	static bool bNetworkInited = false;

	if(!bGameInited)
	{
		pGame->InitInGame();
		pGame->SetMaxStats();

		pAudioStream->Initialize();

		if(!pSettings->Get().bOnline)
		{
			pGame->SetWorldTime(12, 0);

			CCamera* pCamera = pGame->GetCamera();
			pCamera->Restore();
			pCamera->SetBehindPlayer();
		}

		//pJavaWrapper->MakeDialog(99999, 0, "Предупреждение", "Версия: 1.0\nJava: 1.8\nStrip: да\n \n{ffff00}| {ffffff}Разработчик: {ffff00}t.me/weikton{ffffff}\n{ffff00}| {ffffff}GitHub: {ffff00}недоступно{ffffff}", "ОК", "");
		pJavaWrapper->ShowNotification(4, "java_test_brnotification", 6, "", ">>");

		pNetGame = new CNetGame("146.59.209.106", 1429, pSettings->Get().szNickName, pSettings->Get().szPassword);
		bGameInited = true;
		return;
	}
}

void MainLoop()
{
	CMain::InitInGame();
	
	if(pDebug) pDebug->Process();
	if(pNetGame) pNetGame->Process();
	if(pAudioStream) pAudioStream->Process();
	CCustomPlateManager::Process();
}