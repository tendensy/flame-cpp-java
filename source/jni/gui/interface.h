#pragma once

#include "../vendor/imgui/imgui.h"
#include "../game/RW/RenderWare.h"
#include "../game/common.h"

enum typeState
{
    OFF = 0,
    ON
};

class CInterface {

public:
    CInterface();
    ~CInterface();

    void RenderHud() const;
    void RenderMenu();
    void RenderSpeedometer();
    //void RenderClientInfo();

    static void SetupKeyboardStyle();

    void ToggleHudState() { m_bRenderHud = !m_bRenderHud; }
    void ToggleMenuState() { m_bRenderMenu = !m_bRenderMenu; }
    //void ToggleInfoState() { m_bRenderInfo = !m_bRenderInfo; }

    void SetWantedLevel(int level) { m_iWantedLevel = level; }

    void SetHudID(int id) { m_iCurrentHudID = id; }
    int GetHudID() const { return m_iCurrentHudID; }

    void EngineState(bool b) { bEngine = b; }
    void LightState(bool b) { bLight = b; }
    void LockState(bool b) { bLock = b; }
    void BeltState(bool b) { bBelt = b; }

private:
    int m_iCurrentHudID;
    int m_iWantedLevel;
	
	int m_iCurrentVolumePlayer;

    bool m_bRenderHud;
    bool m_bRenderMenu;
    //bool m_bRenderInfo;

    RwTexture* hud_bg, *iHeart, *iEat, *iShield, *iWanted;
    RwTexture *iFist[47]; 

    //RECT m_rectArea;

    ImFont* speedFont;
    RwTexture *engineTex0, *engineTex1,
     *lockTex0, *lockTex1, 
     *lightTex0, *lightTex1,
     *beltTex0, *beltTex1,
     *fuelTex, 
     *healthTex; 
     //*arrowLTex[2], 
     //*arrowRTex[2];
    bool bEngine, bLight, bLock, bBelt;
    int iMileage;
};
