#include "online_menu_view.h"
#include "../game/online_menu.h"
#include "../game/video_settings.h"
#include "imgui.h"
#include <algorithm>
#include <cstdio>
#include <cmath>
extern "C" unsigned g_doa3_frames_presented;

static ImFont *s_title, *s_body;
void InitOnlineMenuFonts()
{
    s_title = s_body = nullptr;
    // Keep the settings font unchanged. Match the native menu's italic serif
    // labels, with a larger atlas so the new text stays smooth at 1440p.
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    char windows[MAX_PATH], file[MAX_PATH];
    if (GetWindowsDirectoryA(windows, MAX_PATH)) {
        snprintf(file, sizeof file, "%s\\Fonts\\timesbi.ttf", windows);
        if (GetFileAttributesA(file) != INVALID_FILE_ATTRIBUTES)
            s_title = io.Fonts->AddFontFromFileTTF(file, 72.f);
        snprintf(file, sizeof file, "%s\\Fonts\\arial.ttf", windows);
        if (GetFileAttributesA(file) != INVALID_FILE_ATTRIBUTES)
            s_body = io.Fonts->AddFontFromFileTTF(file, 40.f);
    }
}

void DrawOnlineMainMenu()
{
    const OnlineMenuState state = online_menu_state(g_doa3_frames_presented);
    if (!state.visible) return;
    const ImVec2 display = ImGui::GetIO().DisplaySize;
    // Match presentation letterboxing, then use the same centered HUD canvas
    // as the native interface at 21:9/32:9. 4:3 keeps the game's stretching.
    const float aspect = video_display_aspect(video_get_aspect());
    float h = std::min(display.y, display.x / aspect), w = h * aspect;
    float x = (display.x - w) * .5f, y = (display.y - h) * .5f;
    float hudx, hudw;
    video_hud_region((unsigned)w, &hudx, &hudw);
    x += hudx;
    w = hudw;
    auto point = [&](float px, float py) { return ImVec2(x + px*w/640, y + py*h/480); };
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(display);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::Begin("##OnlineModeSelect", nullptr, ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImDrawList *draw = ImGui::GetWindowDrawList();
    auto text = [&](float cy, const char *label, float size, ImU32 color) {
        const float pixels = size*h/480;
        ImFont *font = size >= 19 ? s_title : s_body;
        if (!font) font = ImGui::GetFont();
        const ImVec2 extent = font->CalcTextSizeA(pixels, 10000, 0, label);
        ImVec2 at = point(320,cy); at.x -= extent.x*.5f;
        draw->AddText(font, pixels, ImVec2(at.x+1,at.y+2), IM_COL32(0,0,0,200),label);
        draw->AddText(font, pixels, at, color,label);
    };
    auto button = [&](const char *label, float cy, bool selected, int action, float halfWidth=105.f) {
        const ImVec2 a = point(320-halfWidth,cy), b = point(320+halfWidth,cy+30);
        ImGui::SetCursorScreenPos(a);
        bool clicked = ImGui::InvisibleButton(label, ImVec2(b.x-a.x,b.y-a.y));
        bool lit = selected || ImGui::IsItemHovered();
        draw->AddRectFilledMultiColor(a,b,
            lit ? IM_COL32(82,109,136,230) : IM_COL32(48,48,51,225), IM_COL32(8,9,14,230),
            IM_COL32(8,9,14,230), lit ? IM_COL32(41,69,97,230) : IM_COL32(26,26,30,225));
        draw->AddRect(a,b,lit ? IM_COL32(195,225,255,255) : IM_COL32(153,153,156,220),0,0, h/480);
        text(cy+5,label,19,lit ? IM_COL32_WHITE : IM_COL32(205,205,210,255));
        if (clicked) online_menu_queue(action);
    };
    const OnlineMenuSprite *sprites;
    const unsigned count=online_menu_sprites(g_doa3_frames_presented,&sprites);
    bool selectedOnline=false;
    for (unsigned i=0; i<count; ++i) {
        const auto &sprite=sprites[i];
        const int alpha=(int)(std::min(1.f,std::max(0.f,sprite.alpha))*255);
        const ImU32 color=IM_COL32(205,205,210,alpha);
        const ImVec2 a=point(sprite.x,sprite.y), b=point(sprite.x+sprite.w,sprite.y+sprite.h);
        if (sprite.label) {
            selectedOnline=true;
            ImFont *font=s_title ? s_title : ImGui::GetFont();
            float size=19*h/480;
            ImVec2 extent=font->CalcTextSizeA(size,10000,0,"ONLINE");
            ImVec2 pos((a.x+b.x-extent.x)*.5f,a.y);
            draw->AddText(font,size,ImVec2(pos.x+1,pos.y+2),IM_COL32(0,0,0,alpha),"ONLINE");
            draw->AddText(font,size,pos,IM_COL32(255,255,255,alpha),"ONLINE");
        } else {
            // A monochrome globe follows the exact position, scale and fade
            // submitted by the native carousel alongside its other icons.
            const ImVec2 center((a.x+b.x)*.5f,a.y+(b.y-a.y)*.42f);
            const float rx=(b.x-a.x)*.34f, ry=(b.y-a.y)*.30f;
            const float stroke=std::max(1.f,(b.y-a.y)*.025f);
            auto ellipse=[&](float ex,float ey) {
                for (int k=0;k<=64;++k) {
                    float angle=k*6.2831853f/64;
                    draw->PathLineTo(ImVec2(center.x+cosf(angle)*ex,center.y+sinf(angle)*ey));
                }
                draw->PathStroke(color,ImDrawFlags_Closed,stroke);
            };
            ellipse(rx,ry); ellipse(rx*.43f,ry);
            draw->AddLine(ImVec2(center.x-rx,center.y),ImVec2(center.x+rx,center.y),color,stroke);
            for (float dy : {-0.5f,0.5f})
                draw->AddLine(ImVec2(center.x-rx*.866f,center.y+ry*dy),
                              ImVec2(center.x+rx*.866f,center.y+ry*dy),color,stroke);
        }
    }
    if (state.page == ONLINE_MENU_CAROUSEL) {
        if (selectedOnline) text(428,"Host or join an online game",14,IM_COL32_WHITE);
    } else {
        // This screen is a frontend prototype. No session, socket, UPnP or
        // rollback API is called by either choice.
        draw->AddRectFilled(ImVec2(0,0),display,IM_COL32(0,0,0,195));
        draw->AddRectFilled(point(92,112),point(548,407),IM_COL32(17,19,26,245));
        draw->AddRect(point(92,112),point(548,407),IM_COL32(140,145,155,255));
        text(133,"ONLINE",30,IM_COL32_WHITE);
        draw->AddLine(point(115,176),point(525,176),IM_COL32(180,180,190,255));
        if (state.page == ONLINE_MENU_OPTIONS) {
            button("HOST",201,state.selection==0,ONLINE_MENU_OPEN_HOST);
            button("JOIN",246,state.selection==1,ONLINE_MENU_OPEN_JOIN);
            button("BACK",291,state.selection==2,ONLINE_MENU_BACK);
            text(344,"Menu preview - online play is not enabled",12,IM_COL32(180,184,195,255));
        } else {
            const bool host = state.page==ONLINE_MENU_HOST;
            text(199,host ? "HOST ROOM" : "JOIN ROOM",23,IM_COL32_WHITE);
            text(244,host ? "Create a room for another player to join." : "Join another player's room.",
                 13,IM_COL32(225,225,232,255));
            text(271,host ? "Room creation is not available in this preview." : "Joining is not available in this preview.",
                 12,IM_COL32(180,184,195,255));
            button("BACK",315,true,ONLINE_MENU_BACK);
        }
        text(379,"Up / Down: select     Confirm: choose     Back: return",10,IM_COL32(200,200,210,255));
    }
    ImGui::End();
    ImGui::PopStyleVar();
}
