#pragma once

#include "../game/RW/RenderWare.h"
#include "../vendor/imgui/imgui.h"

class CNewButtons {

public:
    CNewButtons();

    void Render();
    void ToggleState(bool bShow);

private:
    void LoadButtonsTextures();

private:
    RwTexture* btn_tab  = nullptr;
    RwTexture* btn_h    = nullptr;
    RwTexture* btn_f    = nullptr;
    RwTexture* btn_y    = nullptr;
    RwTexture* btn_n    = nullptr;
    RwTexture* btn_ctrl = nullptr;
    RwTexture* btn_two  = nullptr;
    RwTexture* btn_sl   = nullptr;
    RwTexture* btn_sr   = nullptr;
    RwTexture* btn_alt  = nullptr;
    RwTexture* btn_x	= nullptr;
    RwTexture* btn_bind	= nullptr;
    RwTexture* btn_inv	= nullptr;

    float m_fButWidth;
    float m_fButHeight;

    bool m_bIsItemShow;
    bool m_bIsShow;
};
