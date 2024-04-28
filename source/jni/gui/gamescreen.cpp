#include "main.h"
#include "gamescreen.h"
#include "gui.h"

extern CGUI* pGUI;

CGameScreen::CGameScreen()
{
    m_pInterface    = new CInterface();
    m_pButtons      = new CButtons();
    m_pNewButtons   = new CNewButtons();
}

void CGameScreen::LoadAllTextures()
{
    panelBackground = LoadTex("speed_panel_backround");
    panelEngine[0]  = LoadTex("speed_panel_engineOff"); panelEngine[1] = LoadTex("speed_panel_engineOn");
}

void CGameScreen::Render()
{
    Speedometer();
}

void CGameScreen::Speedometer()
{   
    /*ImGuiIO& io = ImGui::GetIO();
    ImVec2 windowSize(pGUI->ScaleY(400), pGUI->ScaleY(400));
    ImVec2 windowPos(io.DisplaySize.x / 2 - (windowSize.x / 2), io.DisplaySize.y - windowSize.y );
    ImDrawList* window = ImGui::GetBackgroundDrawList();
// Navigation panel
    // background
    window->AddImage(panelBackground->raster,
                    ImVec2(windowPos.x + pGUI->ScaleY(), windowPos.y + pGUI->ScaleY(350)))*/
}