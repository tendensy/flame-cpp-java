#include "../main.h"
#include "../game/RW/RenderWare.h"
#include "../settings.h"
#include "../game/game.h"
#include "../net/netgame.h"
#include "../util/patch.h"
#include "../util/util.h"
#include "interface.h"
#include "buttons.h"
#include "gamescreen.h"
#include "game/sprite2d.h"

extern CGUI *pGUI;
extern CGame *pGame;
extern CNetGame *pNetGame;
extern CSettings *pSettings;
extern CGameScreen *pGameScreen;

static int m_iContentId;

CInterface::CInterface() {
    m_iCurrentHudID = pSettings->Get().iHudStyle;
    m_iWantedLevel = 0;
    m_iContentId = 0;

    m_bRenderHud = true;
    m_bRenderMenu = false;
    iMileage = 0;
    speedFont = pGUI->LoadFont("flame_speed.ttf", 30);

  //  iBackground = (RwTexture*)LoadTextureFromDB("samp", "hud_bg");

    iHeart = (RwTexture*)LoadTextureFromDB("samp", "fh1_heart");
    iEat   = (RwTexture*)LoadTextureFromDB("samp", "fh1_eat");
    iShield= (RwTexture*)LoadTextureFromDB("samp", "fh1_shield");
    iWanted= (RwTexture*)LoadTextureFromDB("samp", "fh1_star");

    hud_bg = (RwTexture*)LoadTextureFromDB("samp", "lr_hud_bg");

    for (int i = 0; i < 47; i++)
    {
        iFist[i] = (RwTexture*)LoadTextureFromDB("samp", GetWeaponTextureName(i));
    }
    
//    iLogo = (RwTexture*)LoadTextureFromDB("samp", "flame_hud_1");

    /*engineTex0 		= (RwTexture*)LoadTextureFromDB("samp","speed_engine_off");
    engineTex1		= (RwTexture*)LoadTextureFromDB("samp","speed_engine_on");

    lockTex0		= (RwTexture*)LoadTextureFromDB("samp", "speed_key_off");
    lockTex1 		= (RwTexture*)LoadTextureFromDB("samp", "speed_key_on");

    lightTex0		= (RwTexture*)LoadTextureFromDB("samp", "speed_light_off");
    lightTex1 		= (RwTexture*)LoadTextureFromDB("samp", "speed_light_on");

    beltTex0 		= (RwTexture*)LoadTextureFromDB("samp", "speed_belt_off");
    beltTex1 		= (RwTexture*)LoadTextureFromDB("samp", "speed_belt_on");

    fuelTex   		        = (RwTexture*)LoadTextureFromDB("samp", "speed_fuel");
    healthTex               = (RwTexture*)LoadTextureFromDB("samp", "speed_health");   */
}

void CInterface::RenderHud() const {
    if (!m_bRenderHud) return;

    ImGuiIO& io = ImGui::GetIO();

    // HUD BLACK ONLINE

    // --> Disable rendering default hud --> --> --> --> -->
    if (pSettings->Get().iHUD) 
    {
	*(uint8_t*)(g_libGTASA+0x7165E8) = 0;
    }
    else *(uint8_t*)(g_libGTASA+0x7165E8) = 1;

    if(!pGame->FindPlayerPed()->m_bPlayerControllable) return;

    if (pSettings && pSettings->Get().iHUD && pNetGame->GetGameState() == GAMESTATE_CONNECTED) // --> HUD #2 --> --> --> --> -->
    {   
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0x00, 0x00, 0x00, 0x00).Value);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(0x00, 0x00, 0x00, 0x00).Value);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(0x00, 0x00, 0x00, 0x00).Value);

        ImGui::Begin("__hud_blackonline", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings
        );

        ImGui::GetStyle().ButtonTextAlign = ImVec2(0.5f, 0.5f);
        ImGui::GetStyle().WindowPadding = ImVec2(8, 8);
        ImGui::GetStyle().FrameBorderSize = 0.0f;

        ImDrawList* dList = ImGui::GetOverlayDrawList();

        float wPX = ImGui::GetWindowPos().x;
        float wPY = ImGui::GetWindowPos().y;

        CPlayerPed *pPlayerPed = pGame->FindPlayerPed();

        if(pPlayerPed)
        {
            //        -> pos X   value
            float health  = pGUI->ScaleX(140) + pGUI->ScaleX(pPlayerPed->GetHealth()) * 2;
            float eat     = pGUI->ScaleX(140) + pGUI->ScaleX((float)pGUI->m_Eat) * 2;
            float armour  = pGUI->ScaleX(140) + pGUI->ScaleX(pPlayerPed->GetArmour()) * 2;

            if (pPlayerPed->GetHealth() >= 100)
            {
                health  = pGUI->ScaleX(140) + pGUI->ScaleX(100) * 2;
            }

	    //dList->AddImage(iBackground->raster, ImVec2(io.DisplaySize.x - pGUI->ScaleX(400), ImVec2(io.DisplaySize.x - pGUI->ScaleX(400), ImVec2(io.DisplaySize.x - pGUI->ScaleX(400), pGUI->ScaleX(18))); // background
	    //ImGui::Image(iBackground->raster, ImVec2(/*io.DisplaySize.x - */pGUI->ScaleX(370), pGUI->ScaleY(350))); // ШИРИНА, ВЫСОТА

            unsigned int money = pGame->GetLocalMoney();
            int patrons = pPlayerPed->FindWeaponSlot(pPlayerPed->GetCurrentWeapon())->dwAmmo;
            int patronsInClip = pPlayerPed->FindWeaponSlot(pPlayerPed->GetCurrentWeapon())->dwAmmoInClip;
            int currentWeaponID = pPlayerPed->GetCurrentWeapon();
            char buffer[24] = "";

            ImVec2 pos = ImVec2(wPX + pGUI->ScaleX(345), wPY + pGUI->ScaleX(102));

            // --> HP --> --> --> --> -->
            sprintf(buffer, "%.0f", pPlayerPed->GetHealth());
            pGUI->RenderText(pos, ImColor(255, 255, 255, 255), false, buffer, nullptr, pGUI->ScaleY(18.0f));
            dList->AddImage(iHeart->raster, ImVec2(pos.x - pGUI->ScaleX(240), pos.y - pGUI->ScaleX(3)), ImVec2(pos.x - pGUI->ScaleX(219), pos.y + pGUI->ScaleX(18)));

            dList->AddRectFilled(ImVec2(wPX + pGUI->ScaleX(140), wPY + pGUI->ScaleX(115)), ImVec2(wPX + pGUI->ScaleX(340), wPY + pGUI->ScaleX(105)), ImColor(0.0f, 0.0f, 0.0f, 0.8f));
            dList->AddRectFilled(ImVec2(wPX + pGUI->ScaleX(140), wPY + pGUI->ScaleX(115)), ImVec2(wPX + health, wPY + pGUI->ScaleX(105)), ImColor(254, 0, 0, 255));
            // --> ARMOUR --> --> --> --> -->
	    pos.y += pGUI->ScaleX(30);

            sprintf(buffer, "%.0f", pPlayerPed->GetArmour());
            pGUI->RenderText(pos, ImColor(255, 255, 255, 255), false, buffer, nullptr, pGUI->ScaleY(18.0f));
            dList->AddImage(iShield->raster, ImVec2(pos.x - pGUI->ScaleX(240), pos.y - pGUI->ScaleX(3)), ImVec2(pos.x - pGUI->ScaleX(219), pos.y + pGUI->ScaleX(18)));

            dList->AddRectFilled(ImVec2(wPX + pGUI->ScaleX(140), wPY + pGUI->ScaleX(145)), ImVec2(wPX + pGUI->ScaleX(340), wPY + pGUI->ScaleX(135)), ImColor(0.0f, 0.0f, 0.0f, 0.8f));
            dList->AddRectFilled(ImVec2(wPX + pGUI->ScaleX(140), wPY + pGUI->ScaleX(145)), ImVec2(wPX + armour, wPY + pGUI->ScaleX(135)), ImColor(0, 120, 255, 255));
            
            // --> EAT --> --> --> --> -->
            pos.y += pGUI->ScaleX(30);

            sprintf(buffer, "%.0f", (float)pGUI->m_Eat);
            pGUI->RenderText(pos, ImColor(255, 255, 255, 255), false, buffer, nullptr, pGUI->ScaleY(18.0f));
            dList->AddImage(iEat->raster, ImVec2(pos.x - pGUI->ScaleX(240), pos.y - pGUI->ScaleX(3)), ImVec2(pos.x - pGUI->ScaleX(219), pos.y + pGUI->ScaleX(18)));

            dList->AddRectFilled(ImVec2(wPX + pGUI->ScaleX(140), wPY + pGUI->ScaleX(175)), ImVec2(wPX + pGUI->ScaleX(340), wPY + pGUI->ScaleX(165)), ImColor(0.0f, 0.0f, 0.0f, 0.8f));
            dList->AddRectFilled(ImVec2(wPX + pGUI->ScaleX(140), wPY + pGUI->ScaleX(175)), ImVec2(wPX + eat, wPY + pGUI->ScaleX(165)), ImColor(255, 168, 0, 255));
            
            pos = ImVec2(wPX + pGUI->ScaleX(40), wPY + pGUI->ScaleX(170));
            sprintf(buffer, "%d-%d", patronsInClip, patrons - patronsInClip);

            pGUI->RenderText(pos, ImColor(255, 255, 255, 255), false, buffer, nullptr, pGUI->ScaleY(20.0f));

            // --> TIME --> --> --> --> -->
            time_t i; struct tm* tm;
            time(&i); tm = localtime(&i);

            sprintf(buffer, "%02i:%02i", tm->tm_hour, tm->tm_min);

            pos = ImVec2(wPX + pGUI->ScaleX(240), wPY + pGUI->ScaleX(40));
            pGUI->RenderText(pos, ImColor(255, 255, 255, 255), false, buffer, nullptr, pGUI->ScaleY(40));

            // --> MONEY --> --> --> --> -->
            sprintf(buffer, "%s", formatInt(money).c_str());

            pos = ImVec2(wPX - pGUI->ScaleX(20), wPY + pGUI->ScaleX(200));
            pGUI->RenderText(pos, ImColor(255, 255, 255, 255), false, buffer, nullptr, pGUI->ScaleY(30.0f));

            pos = ImVec2(pos.x + ImGui::CalcTextSize(buffer).x * (30 / pSettings->Get().fFontSize), pos.y);
            pGUI->RenderText(pos, ImColor(113, 228, 0, 255), false, "р", nullptr, pGUI->ScaleY(30.0f));

            // --> WANTED --> --> --> --> -->
            if (m_iWantedLevel != 0)
            {
                pos = ImVec2(pos.x + ImGui::GetFontSize() * 1.2, pos.y - pGUI->ScaleX(3));

                for (int idx = 0; idx < m_iWantedLevel; idx++) {
                    float mult = 30.0f * (float)idx;
                    dList->AddImage(iWanted->raster, ImVec2(pos.x + mult, pos.y), ImVec2(pos.x + pGUI->ScaleX(30.0f) + mult, pos.y + pGUI->ScaleX(30.0f)));
                }
            }
        }

        ImGui::SetWindowPos(ImVec2(io.DisplaySize.x - pGUI->ScaleX(400), 0));
        ImGui::End();
        ImGui::PopStyleColor(3);

        if(pPlayerPed)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0x00, 0x00, 0x00, 0x00).Value);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(0x00, 0x00, 0x00, 0x00).Value);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(0x00, 0x00, 0x00, 0x00).Value);

            ImGui::Begin("__lr_hud_fist", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings
                    );
            ImGui::SetWindowPos(ImVec2(wPX - pGUI->ScaleX(25), wPY + pGUI->ScaleX(23)));

            ImGui::GetStyle().ButtonTextAlign = ImVec2(0.5f, 0.5f);
            ImGui::GetStyle().WindowPadding = ImVec2(8, 8);
            ImGui::GetStyle().FrameBorderSize = 0.0f;

            int patronsInClip = pPlayerPed->FindWeaponSlot(pPlayerPed->GetCurrentWeapon())->dwAmmoInClip;
            int currentWeaponID = pPlayerPed->GetCurrentWeapon();

            if (ImGui::ImageButton(iFist[currentWeaponID]->raster, ImVec2(pGUI->ScaleX(170), pGUI->ScaleX(170))))
            {
                pNetGame->SendChatCommand(cryptor::create("/setplayergun_878787", 21).decrypt());
            }

            ImGui::End();
            ImGui::PopStyleColor(3);
        }
    }    
}

void CInterface::RenderMenu() {
    if (!m_bRenderMenu) return;
    SaveMenuSettings();

    pGUI->SetupDefaultStyle();

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("#menu", nullptr,
                 ImGuiWindowFlags_NoTitleBar
                 | ImGuiWindowFlags_NoResize
                 | ImGuiWindowFlags_NoSavedSettings
                 | ImGuiWindowFlags_NoScrollbar
                 | ImGuiWindowFlags_NoMove
    );

    ImGui::GetBackgroundDrawList()->AddRectFilled(
            ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y),
            ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + ImGui::GetFontSize() * 2),
            0xFF000000
    );

    ImGui::Text("Настройки клиента");
    ImGui::Separator();

    ImGui::BeginChild("#__menu", ImVec2(-1, ImGui::GetWindowSize().y - ImGui::GetFontSize() * 4.7f), false);

    ImGui::Separator();

    // --> Fist Click Selector --> --> --> --> -->
    const char* s_items[] = { "Меню клиента", "Список игроков", "Статистика", "Время", "Action Menu" };
    static const char* s_item_current = s_items[pSettings->Get().iFistClick];

    ImGui::Text("Выбор действия при нажатии на фист:");

    ImGui::SetNextItemWidth(-1);
    if (ImGui::BeginCombo("#__fist_click", s_item_current))
    {
        for (auto & s_item : s_items)
        {
            bool is_selected = (s_item_current == s_item);
            if (ImGui::Selectable(s_item, is_selected))
                s_item_current = s_item;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if(s_item_current == s_items[0]) pSettings->Get().iFistClick = 0;
    else if(s_item_current == s_items[1]) pSettings->Get().iFistClick = 1;
    else if(s_item_current == s_items[2]) pSettings->Get().iFistClick = 2;
    else if(s_item_current == s_items[3]) pSettings->Get().iFistClick = 3;
    else if(s_item_current == s_items[4]) pSettings->Get().iFistClick = 4;

    // --> Texture Memory Limit Selector --> --> --> --> -->
    const char* h_items[] = { "Выключен", "250 MB", "500 MB" };
    static const char* h_item_current = h_items[pSettings->Get().iTextureLimit];

    ImGui::Text("Увеличение памяти текстур:");

    ImGui::SetNextItemWidth(-1);
    if (ImGui::BeginCombo("#__texture_limit", h_item_current))
    {
        for (auto & h_item : h_items)
        {
            bool is_selected = (h_item_current == h_item);
            if (ImGui::Selectable(h_item, is_selected))
                h_item_current = h_item;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if(h_item_current == h_items[0]) pSettings->Get().iTextureLimit = 0;
    else if(h_item_current == h_items[1]) pSettings->Get().iTextureLimit = 1;
    else if(h_item_current == h_items[2]) pSettings->Get().iTextureLimit = 2;

	// --> KeyBoard Selector --> --> --> --> -->
    const char* k_items[] = { "Стандартная", "Стандартная с фоном", "Android клавиатура" };
    static const char* k_item_current = k_items[pSettings->Get().iKeyboard];

    ImGui::Text("Выбор клавиатуры:");

    ImGui::SetNextItemWidth(-1);
    if (ImGui::BeginCombo("#__select_keyboard", k_item_current))
    {
        for (auto & k_item : k_items)
        {
            bool is_selected = (k_item_current == k_item);
            if (ImGui::Selectable(k_item, is_selected))
                k_item_current = k_item;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if(k_item_current == k_items[0]) pSettings->Get().iKeyboard = 0;
    else if(k_item_current == k_items[1]) pSettings->Get().iKeyboard = 1;
    else if(k_item_current == k_items[2]) pSettings->Get().iKeyboard = 2;

    // --> Checkbox`s --> --> --> --> -->
    ImGui::Checkbox("Время в чате", &pGUI->timestamp);
    ImGui::Checkbox("Фон 3D текста", &pGUI->bLabelBackground);
    ImGui::Checkbox("Квадратный радар", &pSettings->Get().szRadar);
    ImGui::Checkbox("Затухание чата", &pSettings->Get().iChatShadow);
	//TODO: ImGui::Checkbox("Сглаживание текстур", &pSettings->Get().iAntiAliasing);
    //ImGui::Checkbox("Android клавиатура", &pSettings->Get().iAndroidKeyboard);
    ImGui::Checkbox("Счетчик FPS", &pSettings->Get().iFPSCounter);
	ImGui::Checkbox("Индикатор подключения", &pSettings->Get().szConnectIndicator);
	ImGui::Checkbox("Прозрачная обводка радара", &pSettings->Get().iRadarAlpha);
	ImGui::Checkbox("Новый HUD", &pSettings->Get().iHUD);
	if(ImGui::Checkbox("Новые кнопки", &pSettings->Get().iNewButtons))//;
	{
		if(!pSettings->Get().iNewButtons) 
		{
			pGameScreen->GetButtons()->ToggleState(true);
			pGameScreen->GetNewButtons()->ToggleState(false);
		}
		else {
			pGameScreen->GetNewButtons()->ToggleState(true);
			pGameScreen->GetButtons()->ToggleState(false);
		}
	}
	ImGui::Checkbox("Показ координат", &pSettings->Get().iCoord);
	
	//ImGui::Text("Настройки голосового чата\n");
    //ImGui::Checkbox("Голосовой чат", &pSettings->Get().bVoiceChatEnable);
	
	//ImGui::Text("Функции после применения которых необходимо перезайти\n");
    ImGui::Checkbox("Полный экран", &pSettings->Get().iCutout);
    if(ImGui::Checkbox("Пониженная графика (для слаб.устр)", &pSettings->Get().bnoFX))
    {
        if(pSettings->Get().bnoFX)
	{
		//CPatch::NOP(g_libGTASA + 0x39B36A, 2);
		CPatch::WriteMemory(g_libGTASA + 0x52DD38, "\x00\x20\x70\x47", 4); // return 0 CCoronas::RenderReflections
		CPatch::NOP(g_libGTASA + 0x39AD14, 1); // skip render clouds, sunrefl, raineffect 
	}
	else
	{
		//CPatch::WriteMemory(g_libGTASA + 0x39B36A, "\xFF\xF7\x5D\xFD", 4);
		CPatch::WriteMemory(g_libGTASA + 0x52DD38, "\x2D\xE9\xF0\x4F", 4); // CCoronas::RenderReflections 
		CPatch::WriteMemory(g_libGTASA + 0x39AD14, "\xB3\xB3\x03\x2B", 4); 
	}

    }
   /* if (ImGui::Button("Информация", ImVec2(-1, -1)))
    {
	ToggleInfoState();
    }	*/
    static float fTempDist = pSettings->Get().fDrawPedDist;
    //ImGui::SliderFloat("Дистанция прорисовки игроков", &fTempDist, 10.0f, 150.0f);
    pSettings->Get().fDrawPedDist = fTempDist;
    ImGui::Separator();
    ImGui::EndChild();

    if (ImGui::Button("Закрыть", ImVec2(-1, -1)))
        m_bRenderMenu = false;
	//ImGui::SameLine(0, 5);

    ImGui::SetWindowSize(ImVec2(pGUI->ScaleY(1500), pGUI->ScaleY(700)));
    ImGui::SetWindowPos(ImVec2(((io.DisplaySize.x - ImGui::GetWindowSize().x) / 2), ((io.DisplaySize.y - ImGui::GetWindowSize().y) / 2)));

    ImGui::End();
}

void CInterface::RenderSpeedometer()
{   
    if (!pGame || !pNetGame || !pGame->FindPlayerPed()->IsInVehicle()) return;
	if (pGame->FindPlayerPed()->IsAPassenger()) return;

    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("LR_SPEED_NEW", nullptr,
                 ImGuiWindowFlags_NoTitleBar
                 | ImGuiWindowFlags_NoResize
                 | ImGuiWindowFlags_NoSavedSettings
                 | ImGuiWindowFlags_NoBackground
                 | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
    );

    ImVec2 _pos(
            ImGui::GetWindowPos().x + ImGui::GetWindowSize().x / 2,
            ImGui::GetWindowPos().y + ImGui::GetWindowSize().y / 2
    );
    ImGuiWindow* win = ImGui::GetCurrentWindow();


    // ---> Render icons -- -- -- -- --;

    
    // fuel
    ImVec2 sizeFuel(121 * 0.26, 145 * 0.26);
    ImVec2 _a(_pos.x - pGUI->ScaleY(200.0f + sizeFuel.x), _pos.y - pGUI->ScaleY(sizeFuel.y / 2));
    ImVec2 _b(_a.x + pGUI->ScaleY(sizeFuel.x), _a.y + pGUI->ScaleY(sizeFuel.y));
    win->DrawList->AddImage(fuelTex->raster, _a, _b);
    char buff[0x20];
    sprintf(buff, "%dл", pGUI->m_fuel);
    win->DrawList->AddText(nullptr, 
        ImGui::GetFontSize() * 0.9, 
        ImVec2(_b.x - pGUI->ScaleY(sizeFuel.x / 2) - (ImGui::CalcTextSizeByFontSize(ImGui::GetFontSize() * 0.9, buff).x / 2), _b.y), 
        ImColor(255, 255, 255, 255), 
        buff, nullptr); 

    // health
    ImVec2 sizeHealth(150 * 0.26, 150 * 0.26);
    _a = ImVec2(_pos.x + pGUI->ScaleY(200), _pos.y - pGUI->ScaleY(sizeHealth.y / 2));
    _b = ImVec2(_a.x + pGUI->ScaleY(sizeHealth.x), _a.y + pGUI->ScaleY(sizeHealth.y));
    win->DrawList->AddImage(healthTex->raster, _a, _b);
    sprintf(buff, "%0.f%%", pGame->FindPlayerPed()->GetGtaVehicle()->fHealth / 10);
    win->DrawList->AddText(nullptr, 
        ImGui::GetFontSize() * 0.9, 
        ImVec2(_b.x - pGUI->ScaleY(sizeHealth.x / 2) - (ImGui::CalcTextSizeByFontSize(ImGui::GetFontSize() * 0.9, buff).x / 2), _b.y), 
        ImColor(255, 255, 255, 255), 
        buff, nullptr); 

    win->DrawList->AddRectFilled(
        ImVec2(_pos.x - pGUI->ScaleY(60), _pos.y + pGUI->ScaleY(70)),
        ImVec2(_pos.x + pGUI->ScaleY(60), _pos.y + pGUI->ScaleY(105)),
        ImColor(0.0f, 0.0f, 0.0f, 0.65f), 10.0f
        );

    sprintf(buff, "%06d", iMileage);
    win->DrawList->AddText(nullptr, 
        ImGui::GetFontSize() * 0.9, 
        ImVec2(_pos.x - (ImGui::CalcTextSizeByFontSize(ImGui::GetFontSize() * 0.9, buff).x / 2), _pos.y + pGUI->ScaleY(75)), 
        ImColor(255, 255, 255, 255), 
        buff, nullptr); 

    win->DrawList->AddRectFilled(
        ImVec2(_pos.x - pGUI->ScaleY(110), _pos.y + pGUI->ScaleY(135)),
        ImVec2(_pos.x + pGUI->ScaleY(110), _pos.y + pGUI->ScaleY(180)),
        ImColor(0.35, 0.35, 0.35, 0.50f), 20.0f
        );
    
    _a = ImVec2(_pos.x - pGUI->ScaleY(95), _pos.y + pGUI->ScaleY(145));
    _b = ImVec2(_a.x + pGUI->ScaleY(104 * 0.3), _a.y + pGUI->ScaleY(76 * 0.3));
    win->DrawList->AddImage(bEngine ? engineTex1->raster : engineTex0->raster, _a, _b);

    _a = ImVec2(_pos.x - pGUI->ScaleY(45), _pos.y + pGUI->ScaleY(145));
    _b = ImVec2(_a.x + pGUI->ScaleY(136 * 0.3), _a.y + pGUI->ScaleY(84 * 0.3));
    win->DrawList->AddImage(bLight ? lightTex1->raster : lightTex0->raster, _a, _b);

    _a = ImVec2(_pos.x - pGUI->ScaleY(-15), _pos.y + pGUI->ScaleY(143));
    _b = ImVec2(_a.x + pGUI->ScaleY(103 * 0.3), _a.y + pGUI->ScaleY(104 * 0.3));
    win->DrawList->AddImage(bBelt ? beltTex1->raster : beltTex0->raster, _a, _b);
    
    _a = ImVec2(_pos.x - pGUI->ScaleY(-65), _pos.y + pGUI->ScaleY(143));
    _b = ImVec2(_a.x + pGUI->ScaleY(96 * 0.3), _a.y + pGUI->ScaleY(96 * 0.3));
    win->DrawList->AddImage(bLock ? lockTex1->raster : lockTex0->raster, _a, _b);
    
    // ---> Render icons -- -- -- -- --

    // arrows
    //CSprite2d* sprite = new CSprite2d();
    ImVec2 arrowSize(136 * 0.3, 108 * 0.3);
    _a = ImVec2(_pos.x - pGUI->ScaleY(125 + arrowSize.x), _pos.y + pGUI->ScaleY(142));
    _b = ImVec2(_a.x + pGUI->ScaleY(arrowSize.x), _a.y + pGUI->ScaleY(arrowSize.y));
    //sprite->m_pTexture = arrowLTex[OFF];
    //if(arrowLTex[OFF])win->DrawList->AddImage((ImTextureID)arrowLTex[OFF]->raster, _a, _b);
    //sprite->Draw(_a.x, _a.y, pGUI->ScaleY(arrowSize.x), pGUI->ScaleY(arrowSize.y), 0xFFFFFFFF);

    _a = ImVec2(_pos.x + pGUI->ScaleY(125), _pos.y + pGUI->ScaleY(142));
    _b = ImVec2(_a.x + pGUI->ScaleY(arrowSize.x), _a.y + pGUI->ScaleY(arrowSize.y));
    //if(arrowRTex[OFF])win->DrawList->AddImage((ImTextureID)arrowRTex[OFF]->raster, _a, _b);
    //sprite->m_pTexture = arrowLTex[OFF];
    //sprite->Draw(_a.x - pGUI->ScaleY(arrowSize.x), _a.y, -pGUI->ScaleY(arrowSize.x), pGUI->ScaleY(arrowSize.y), 0xFFFFFFFF);
    
    //delete sprite;
    int i_speed = 0;
    double d_speed = 0.0;

    CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
    CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
    if(pPlayerPed)
    {
        CVehicle *pVehicle = nullptr;
        VEHICLEID id = pVehiclePool->FindIDFromGtaPtr(pPlayerPed->GetGtaVehicle());
        pVehicle = pVehiclePool->GetAt(id);
        
        if(pVehicle)
        {
            VECTOR vecMoveSpeed;
            pVehicle->GetMoveSpeedVector(&vecMoveSpeed);

            i_speed = sqrt((vecMoveSpeed.X * vecMoveSpeed.X) + (vecMoveSpeed.Y * vecMoveSpeed.Y) + (vecMoveSpeed.Z * vecMoveSpeed.Z)) * 183;
        }
    }

    d_speed = i_speed * 0.018333333333;

    char speedText[10];
    sprintf(speedText, " %i ", i_speed);

    win->DrawList->AddText(speedFont, ImGui::GetFontSize() * 2.4, ImVec2(_pos.x - (ImGui::CalcTextSizeByFontSize(ImGui::GetFontSize() * 2.4 ,speedText).x / 2), _pos.y - ImGui::GetFontSize() * 1.5), ImColor(255, 255, 255, 255), speedText, nullptr);
    win->DrawList->AddText(speedFont, ImGui::GetFontSize() * 1.0, ImVec2(_pos.x - (ImGui::CalcTextSize("км/ч").x / 2), _pos.y + ImGui::GetFontSize() * 0.8), ImColor(0x9F, 0x2B, 0xDE, 255), "км/ч", nullptr);
    {
        // ---> Render speed progress bar -- -- -- -- --

        // --> Background -- -- -- -- --
        const float a0 = (2.3) / 6.0f * 2.0f * IM_PI;
        const float a1 = (5.7 + 1.0f) / 6.0f * 2.0f * IM_PI;
        win->DrawList->PathArcTo(_pos, pGUI->ScaleY(140), a0, a1, 50);
        win->DrawList->PathStroke(ImColor(38, 38, 38, 200), false, 7.0f);

        // --> Line filling -- -- -- -- --
        const float a01 = (2.3) / 6.0f * 2.0f * IM_PI;
        const float a11 = (1.3 + d_speed + 1.0f) / 6.0f * 2.0f * IM_PI;
        win->DrawList->PathArcTo(_pos, pGUI->ScaleY(140), a01, a11, 50);
        win->DrawList->PathStroke(ImColor(201, 56, 253, 0xFF), false, 7.0f);

        const float a001 = (1.28 + d_speed + 1.0f) / 6.0f * 2.0f * IM_PI;
        const float a011 = (1.3 + d_speed + 1.0f) / 6.0f * 2.0f * IM_PI;
        win->DrawList->PathArcTo(_pos, pGUI->ScaleY(133), a001, a011, 50);
        win->DrawList->PathStroke(ImColor(0xFF, 0xFF, 0xFF, 0xFF), false, 20.0f);


        // ---> Render speed progress bar -- -- -- -- --
    }

    {
        // ---> Render line -- -- -- -- --

        const float a0 = (2.3) / 6.0f * 2.0f * IM_PI;
        const float a1 = (5.7 + 1.0f) / 6.0f * 2.0f * IM_PI;
        win->DrawList->PathArcTo(_pos, pGUI->ScaleY(117), a0, a1, 50);
        win->DrawList->PathStroke(ImColor(38, 38, 38, 200), false, ImMax(2.0f, 6.0f));

        win->DrawList->AddLine(ImVec2(_pos.x, _pos.y - pGUI->ScaleY(95)), ImVec2(_pos.x, _pos.y - pGUI->ScaleY(117)), ImColor(255, 255, 255, 255), 4.5);
        win->DrawList->AddLine(ImVec2(_pos.x - pGUI->ScaleY(78.5), _pos.y - pGUI->ScaleY(85)), ImVec2(_pos.x - pGUI->ScaleY(66), _pos.y - pGUI->ScaleY(72)), ImColor(0xDF, 0xDF, 0xDF, 255), 2.5);
        win->DrawList->AddLine(ImVec2(_pos.x + pGUI->ScaleY(78.5), _pos.y - pGUI->ScaleY(85)), ImVec2(_pos.x + pGUI->ScaleY(66), _pos.y - pGUI->ScaleY(72)), ImColor(0xDF, 0xDF, 0xDF, 255), 2.5);

        win->DrawList->AddLine(ImVec2(_pos.x - pGUI->ScaleY(115.5), _pos.y - pGUI->ScaleY(10)), ImVec2(_pos.x - pGUI->ScaleY(93), _pos.y - pGUI->ScaleY(10)), ImColor(0xDF, 0xDF, 0xDF, 255), 2.5);
        win->DrawList->AddLine(ImVec2(_pos.x + pGUI->ScaleY(115.5), _pos.y - pGUI->ScaleY(10)), ImVec2(_pos.x + pGUI->ScaleY(93), _pos.y - pGUI->ScaleY(10)), ImColor(0xDF, 0xDF, 0xDF, 255), 2.5);
        // ---> Render line -- -- -- -- --
    }

    {
        int maxFuel = 100;
        // ---> Render fuel progress bar -- -- -- -- --

        // --> Background -- -- -- -- --

        const float s0 = (-0.7854*5 - (-0.7854 / 4)) - (-0.7854*3 + (-0.7854 / 4)); // dist radians
        const float f = pGUI->m_fuel; // target
        const float aa1 = (-0.7854*5 - (-0.7854 / 4) )- ((s0 / maxFuel) * f); // filling rad to rad
    
        ImGui::GetBackgroundDrawList()->PathArcTo(_pos, pGUI->ScaleY(180), -0.7854*5 - (-0.7854 / 4), -0.7854*3 + (-0.7854 / 4), 50);
        ImGui::GetBackgroundDrawList()->PathStroke(ImColor(38, 38, 38, 255), false, ImMax(2.0f, 7.0f));
        ImGui::GetBackgroundDrawList()->PathArcTo(_pos, pGUI->ScaleY(180), -0.7854*5 - (-0.7854 / 4), aa1, 50);
        ImGui::GetBackgroundDrawList()->PathStroke(ImColor(0xFF, 0xA8, 0x26, 255), false, ImMax(2.0f, 7.0f));

        // ---> Render fuel progress bar -- -- -- -- --

        // Render Vehicle Health

        CVehicle *pVehicle = nullptr;

        VEHICLEID id = pVehiclePool->FindIDFromGtaPtr(pPlayerPed->GetGtaVehicle());
        pVehicle = pVehiclePool->GetAt(id);
        
        const float s = (0.7854 + (-0.7854 / 4)) - (-0.7854 + (-0.7854 / 4));
        float fHealth = pVehicle->GetHealth();
        if(fHealth > 1000.0f) fHealth = 1000.0f;
        const float a111 = (0.7854 - (-0.7854 / 4)) - ((s / 1000) * fHealth);
        ImGui::GetBackgroundDrawList()->PathArcTo(_pos, pGUI->ScaleY(180), -0.7854 - (-0.7854 / 4), 0.7854 + (-0.7854 / 4), 50);
        ImGui::GetBackgroundDrawList()->PathStroke(ImColor(38, 38, 38, 255), false, ImMax(2.0f, 7.0f));

        ImGui::GetBackgroundDrawList()->PathArcTo(_pos, pGUI->ScaleY(180), a111, 0.7854 + (-0.7854 / 4), 50);
        ImGui::GetBackgroundDrawList()->PathStroke(ImColor( 0xFF, 0x26, 0x26, 255), false, ImMax(2.0f, 7.0f));
        
        // Render Vehicle Health
    }

    ImGui::SetWindowSize(ImVec2(pGUI->ScaleY(600), pGUI->ScaleY(400)));
    ImGui::SetWindowPos(ImVec2(
            io.DisplaySize.x / 2 - ImGui::GetWindowWidth() / 2,
            io.DisplaySize.y - ImGui::GetWindowHeight() * 0.97f
    ));
    ImGui::End();
}