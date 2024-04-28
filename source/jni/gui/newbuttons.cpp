/*ILYA KRUTOY*/
#include "main.h"
#include "../game/game.h"
#include "net/netgame.h"
#include "dialog.h"
#include "GButton.h"
#include "scoreboard.h"
#include "keyboard.h"
#include "newbuttons.h"
#include "../settings.h"

extern CKeyBoard *pKeyBoard;
extern CDialogWindow *pDialogWindow;
extern CGame *pGame;
extern CScoreBoard *pScoreBoard;
extern CNetGame *pNetGame;
extern CGUI *pGUI;
extern CGButton *pGButton;
extern CSettings *pSettings;

CNewButtons::CNewButtons() {

    m_bIsItemShow = false;
    m_bIsShow = false;

    LoadButtonsTextures();
}

void CNewButtons::Render() {
    if (pKeyBoard->IsOpen() || pDialogWindow->m_bIsActive || !m_bIsShow) {
        return;
    }

    /*m_fButWidth = ImGui::CalcTextSize("QWERTY").x;//QWERT
    m_fButHeight = ImGui::CalcTextSize("QWER").x;
*/
    m_fButWidth = ImGui::CalcTextSize("QWER").x;//QWERT
    m_fButHeight = ImGui::CalcTextSize("QWE").x;

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0x00, 0x00, 0x00, 0x00).Value);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(0x00, 0x00, 0x00, 0x00).Value);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(0x00, 0x00, 0x00, 0x00).Value);

    CPlayerPed* pPlayerPed = pGame->FindPlayerPed();
    if (pPlayerPed) {
        ImGuiIO& io = ImGui::GetIO();

        ImGui::GetStyle().ButtonTextAlign = ImVec2(0.5f, 0.5f);
        ImGui::GetStyle().WindowPadding = ImVec2(8, 8);
        ImGui::GetStyle().FrameBorderSize = 0.0f;

        ImGui::Begin("_new_extra_keyboard", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings);

        if (!pScoreBoard->GetState()) 
		{
            if (ImGui::ImageButton((ImTextureID)btn_tab->raster, ImVec2(m_fButWidth, m_fButHeight))) // TAB
                pScoreBoard->Toggle();
        }
        else 
		{
            if (ImGui::ImageButton((ImTextureID)btn_x->raster, ImVec2(m_fButWidth, m_fButHeight)))
                pScoreBoard->Toggle();

            m_bIsItemShow = false;
        }

        ImGui::SameLine(0, 3);

        if (m_bIsItemShow) 
		{
            if (ImGui::ImageButton((ImTextureID)btn_sl->raster, ImVec2(m_fButWidth, m_fButHeight)))
                m_bIsItemShow = !m_bIsItemShow;

            ImGui::SameLine(0, 3);

            if (ImGui::ImageButton((ImTextureID)btn_alt->raster, ImVec2(m_fButWidth, m_fButHeight))) {
                LocalPlayerKeys.bKeys[ePadKeys::KEY_WALK] = true;
		LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE] = true; 
	    }

            ImGui::SameLine(0, 3);

            if (ImGui::ImageButton((ImTextureID)btn_ctrl->raster, ImVec2(m_fButWidth, m_fButHeight)))
                LocalPlayerKeys.bKeys[ePadKeys::KEY_ACTION] = true;

            ImGui::SameLine(0, 3);

            if (ImGui::ImageButton((ImTextureID)btn_f->raster, ImVec2(m_fButWidth, m_fButHeight)))
                LocalPlayerKeys.bKeys[ePadKeys::KEY_SECONDARY_ATTACK] = true;

            ImGui::SameLine(0, 3);

            if (ImGui::ImageButton((ImTextureID)btn_h->raster, ImVec2(m_fButWidth, m_fButHeight)))
                LocalPlayerKeys.bKeys[ePadKeys::KEY_CTRL_BACK] = true;

            ImGui::SameLine(0, 3);

            if (ImGui::ImageButton((ImTextureID)btn_y->raster, ImVec2(m_fButWidth, m_fButHeight)))
                LocalPlayerKeys.bKeys[ePadKeys::KEY_YES] = true;

            ImGui::SameLine(0, 3);

            if (ImGui::ImageButton((ImTextureID)btn_n->raster, ImVec2(m_fButWidth, m_fButHeight)))
                LocalPlayerKeys.bKeys[ePadKeys::KEY_NO] = true;

            ImGui::SameLine(0, 3);
        }
        else 
		{
            if (ImGui::ImageButton((ImTextureID)btn_sr->raster, ImVec2(m_fButWidth, m_fButHeight)))
                m_bIsItemShow = !m_bIsItemShow;

            ImGui::SameLine(0, 3);

            if (pPlayerPed->IsInVehicle())
            {
			if (ImGui::ImageButton((ImTextureID)btn_ctrl->raster, ImVec2(m_fButWidth, m_fButHeight)))
				LocalPlayerKeys.bKeys[ePadKeys::KEY_ACTION] = true;
            }
            else
            {
                if (ImGui::ImageButton((ImTextureID)btn_alt->raster, ImVec2(m_fButWidth, m_fButHeight))) {
                    LocalPlayerKeys.bKeys[ePadKeys::KEY_WALK] = true;
		    LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE] = true; 
		}
            }

	    ImGui::ItemSize(ImVec2(0, 5));
	
	    if (ImGui::ImageButton((ImTextureID)btn_bind->raster, ImVec2(m_fButWidth, m_fButHeight)))
                    pNetGame -> SendChatCommand("/binder");

	    ImGui::SameLine(0, 3);

	    if (ImGui::ImageButton((ImTextureID)btn_inv->raster, ImVec2(m_fButWidth, m_fButHeight)))
	    	pNetGame -> SendChatCommand("/inv");

            ImGui::SameLine(0, 3);
        }

        ImGui::SetWindowSize(ImVec2(-1, -1));
        ImGui::SetWindowPos(ImVec2(2.0f, (io.DisplaySize.y / 3) - (m_fButWidth / 2) + io.DisplaySize.y / 30));

        ImGui::End();
        ImGui::PopStyleColor(3);
    }
}

void CNewButtons::LoadButtonsTextures()
{
    btn_tab  = (RwTexture*)LoadTextureFromDB("samp", "tab");
    btn_h    = (RwTexture*)LoadTextureFromDB("samp", "h");
    btn_f    = (RwTexture*)LoadTextureFromDB("samp", "f");
    btn_y    = (RwTexture*)LoadTextureFromDB("samp", "y");
    btn_n    = (RwTexture*)LoadTextureFromDB("samp", "n");
    btn_ctrl = (RwTexture*)LoadTextureFromDB("samp", "ctrl");
    btn_two  = (RwTexture*)LoadTextureFromDB("samp", "two");
    btn_sl   = (RwTexture*)LoadTextureFromDB("samp", "sl");
    btn_sr   = (RwTexture*)LoadTextureFromDB("samp", "sr");
    btn_alt  = (RwTexture*)LoadTextureFromDB("samp", "alt");
    btn_x = (RwTexture*)LoadTextureFromDB("samp", "x");
    btn_inv = (RwTexture*)LoadTextureFromDB("samp", "inv");
    btn_bind = (RwTexture*)LoadTextureFromDB("samp", "bind");
}

void CNewButtons::ToggleState(bool bShow)
{
    m_bIsShow = bShow;
}