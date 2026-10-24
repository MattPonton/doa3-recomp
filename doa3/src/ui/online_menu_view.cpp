#include "online_menu_view.h"
#include "../game/online_menu.h"
#include "../game/video_settings.h"
#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include <d3d11.h>
#include <algorithm>
#include <cstdio>
#include <cmath>
extern "C" unsigned g_doa3_frames_presented;

static ImFont *s_title, *s_body, *s_heading, *s_info;
void InitOnlineMenuFonts()
{
    s_title = s_body = s_heading = s_info = nullptr;
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontDefault(); // Escape settings retain their own font.
    char windows[MAX_PATH], file[MAX_PATH];
    if (!GetWindowsDirectoryA(windows, MAX_PATH)) return;
    auto load = [&](const char *name, float size) -> ImFont * {
        snprintf(file, sizeof file, "%s\\Fonts\\%s", windows, name);
        return GetFileAttributesA(file) != INVALID_FILE_ATTRIBUTES
            ? io.Fonts->AddFontFromFileTTF(file, size) : nullptr;
    };
    s_title = load("timesbi.ttf", 72.f);
    s_body = load("arialbi.ttf", 54.f);
    s_heading = load("arialbd.ttf", 72.f);
    s_info = load("timesbd.ttf", 54.f);
}

namespace {
// Match the guest menu's 720x480 raster, independent of output/internal scale.
// Extra columns on ultrawide displays extend only the background; the HUD
// always occupies 720 native pixels. Escape settings use their normal target.
static ID3D11Device *s_device;
static ID3D11DeviceContext *s_context;
static ID3D11RenderTargetView *s_target;
static ID3D11ShaderResourceView *s_texture;
static ID3D11ShaderResourceView *s_labelTexture;
static ID3D11BlendState *s_compositeBlend;
static unsigned s_width;

void PrepareCarouselLabel()
{
    if(s_labelTexture || !s_device) return;
    const unsigned *pixels=online_menu_label_pixels();
    if(!pixels) return;
    D3D11_TEXTURE2D_DESC desc={};
    desc.Width=ONLINE_MENU_LABEL_WIDTH; desc.Height=ONLINE_MENU_LABEL_HEIGHT;
    desc.MipLevels=desc.ArraySize=desc.SampleDesc.Count=1;
    desc.Format=DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.Usage=D3D11_USAGE_IMMUTABLE; desc.BindFlags=D3D11_BIND_SHADER_RESOURCE;
    D3D11_SUBRESOURCE_DATA data={pixels,ONLINE_MENU_LABEL_WIDTH*4,0};
    ID3D11Texture2D *texture=nullptr;
    if(SUCCEEDED(s_device->CreateTexture2D(&desc,&data,&texture))) {
        s_device->CreateShaderResourceView(texture,nullptr,&s_labelTexture);
        texture->Release();
    }
}

void ReleaseMenuTarget()
{
    if(s_texture) { s_texture->Release(); s_texture=nullptr; }
    if(s_target) { s_target->Release(); s_target=nullptr; }
    s_width=0;
}

bool PrepareMenuTarget(unsigned width)
{
    if(!s_device || !s_context) return false;
    if(!s_compositeBlend) {
        D3D11_BLEND_DESC blend={};
        auto &rt=blend.RenderTarget[0];
        rt.BlendEnable=TRUE;
        // Rendering transparent artwork produces premultiplied RGB. Do not
        // multiply by alpha again when scaling it over the native carousel.
        rt.SrcBlend=rt.SrcBlendAlpha=D3D11_BLEND_ONE;
        rt.DestBlend=rt.DestBlendAlpha=D3D11_BLEND_INV_SRC_ALPHA;
        rt.BlendOp=rt.BlendOpAlpha=D3D11_BLEND_OP_ADD;
        rt.RenderTargetWriteMask=D3D11_COLOR_WRITE_ENABLE_ALL;
        if(FAILED(s_device->CreateBlendState(&blend,&s_compositeBlend))) return false;
    }
    if(s_width==width) return true;
    ReleaseMenuTarget();
    D3D11_TEXTURE2D_DESC desc={};
    desc.Width=width; desc.Height=480;
    desc.MipLevels=desc.ArraySize=desc.SampleDesc.Count=1;
    desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BindFlags=D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    ID3D11Texture2D *texture=nullptr;
    if(FAILED(s_device->CreateTexture2D(&desc,nullptr,&texture))) return false;
    HRESULT result=s_device->CreateRenderTargetView(texture,nullptr,&s_target);
    if(SUCCEEDED(result)) result=s_device->CreateShaderResourceView(texture,nullptr,&s_texture);
    texture->Release();
    if(FAILED(result)) { ReleaseMenuTarget(); return false; }
    s_width=width;
    return true;
}

void SetMenuCompositeBlend(const ImDrawList *, const ImDrawCmd *)
{
    auto *state=static_cast<ImGui_ImplDX11_RenderState *>(ImGui::GetPlatformIO().Renderer_RenderState);
    state->DeviceContext->OMSetBlendState(s_compositeBlend,nullptr,0xffffffff);
    state->DeviceContext->PSSetSamplers(0,1,&state->SamplerLinear);
}

void CompositeMenu(ImDrawList &art, ImDrawList *output, ImVec2 at, ImVec2 size)
{
    ImDrawData data;
    data.Valid=true;
    data.DisplaySize=ImVec2((float)s_width,480);
    data.FramebufferScale=ImVec2(1,1);
    data.Textures=&ImGui::GetPlatformIO().Textures;
    data.AddDrawList(&art);
    ID3D11RenderTargetView *previous[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT]={};
    ID3D11DepthStencilView *depth=nullptr;
    s_context->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT,previous,&depth);
    const float clear[4]={};
    s_context->ClearRenderTargetView(s_target,clear);
    s_context->OMSetRenderTargets(1,&s_target,nullptr);
    ImGui_ImplDX11_RenderDrawData(&data);
    s_context->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT,previous,depth);
    for(auto *target:previous) if(target) target->Release();
    if(depth) depth->Release();
    output->AddCallback(SetMenuCompositeBlend,nullptr);
    output->AddImage((ImTextureID)(intptr_t)s_texture,at,ImVec2(at.x+size.x,at.y+size.y));
    output->AddCallback(ImDrawCallback_ResetRenderState,nullptr);
}

// Artwork and text share the native canvas, including horizontal stretching.
struct MenuCanvas {
    ImDrawList *draw;
    float x, y, w, h;
    ImVec2 point(float px, float py) const { return ImVec2(x+px*w/640, y+py*h/480); }
    void rect(float l,float t,float r,float b,ImU32 color) const {
        draw->AddRectFilled(point(l,t),point(r,b),color);
    }
    void gradient(float l,float t,float r,float b,ImU32 left,ImU32 right) const {
        draw->AddRectFilledMultiColor(point(l,t),point(r,b),left,right,right,left);
    }
    void line(float l,float t,float r,float b,ImU32 color,float width=1) const {
        draw->AddLine(point(l,t),point(r,b),color,std::max(1.f,width*h/480));
    }
    void text(float px,float py,const char *label,float size,ImFont *font,
              ImU32 color=IM_COL32_WHITE,float align=0,bool shadow=true) const {
        if (!font) font=ImGui::GetFont();
        const float pixels=size*h/480, stretch=(w/640)/(h/480);
        const float width=font->CalcTextSizeA(pixels,10000,0,label).x*stretch;
        ImVec2 at=point(px,py); at.x-=width*align;
        auto pass=[&](ImVec2 pos,ImU32 ink) {
            const int start=draw->VtxBuffer.Size;
            draw->AddText(font,pixels,pos,ink,label);
            for(int i=start;i<draw->VtxBuffer.Size;++i)
                draw->VtxBuffer[i].pos.x=pos.x+(draw->VtxBuffer[i].pos.x-pos.x)*stretch;
        };
        if(shadow) {
            const int alpha=(color>>IM_COL32_A_SHIFT)&255;
            pass(ImVec2(at.x+w/640,at.y+1.5f*h/480),IM_COL32(0,0,0,alpha));
        }
        pass(at,color);
    }
    void globe(float cx,float cy,float rx,float ry,float alpha) const {
        const int a=(int)(std::max(0.f,std::min(1.f,alpha))*255);
        const ImU32 dark=IM_COL32(38,38,38,a), silver=IM_COL32(183,183,183,a);
        auto ellipse=[&](float ex,float ey,ImU32 color,float thickness,float dx=0,float dy=0) {
            for(int k=0;k<64;++k) {
                const float angle=k*6.2831853f/64;
                draw->PathLineTo(point(cx+dx+cosf(angle)*ex,cy+dy+sinf(angle)*ey));
            }
            draw->PathStroke(color,ImDrawFlags_Closed,std::max(1.f,thickness*h/480));
        };
        // Charcoal edging and subdued silver match the other mode icons.
        ellipse(rx,ry,dark,5,1.5f,2); ellipse(rx,ry,silver,2.7f);
        ellipse(rx*.43f,ry,dark,4.5f); ellipse(rx*.43f,ry,silver,2);
        ellipse(rx,ry*.42f,dark,4.5f); ellipse(rx,ry*.42f,silver,2);
        line(cx-rx,cy,cx+rx,cy,dark,4); line(cx-rx,cy,cx+rx,cy,silver,1.8f);
        for(int pass=0;pass<2;++pass) {
            for(int k=0;k<55;++k) {
                const float t=(-.3f+k/54.f*1.75f)*3.14159265f;
                const float ex=cosf(t)*rx*1.27f, ey=sinf(t)*ry*.34f;
                draw->PathLineTo(point(cx+ex*.94f-ey*.34f,cy-ex*.34f+ey*.94f));
            }
            draw->PathStroke(pass ? silver : dark,0,std::max(1.f,(pass ? 2.8f : 6.f)*h/480));
        }
    }
    void information(const char *description) const {
        gradient(0,426,640,446,IM_COL32(35,35,35,255),IM_COL32(3,3,3,255));
        gradient(0,426,137,446,IM_COL32(114,114,114,255),IM_COL32(231,231,231,255));
        ImVec2 wedge[]={point(137,426),point(149,446),point(137,446)};
        draw->AddConvexPolyFilled(wedge,3,IM_COL32(231,231,231,255));
        line(0,446,640,446,IM_COL32(102,102,102,255));
        text(43,426,"information",17,s_info,IM_COL32(16,16,16,255),0,false);
        text(155,427,description,14,s_body);
    }
};
}

void InitOnlineMenuRenderer(ID3D11Device *device, ID3D11DeviceContext *context)
{
    ShutdownOnlineMenuRenderer();
    s_device=device; s_context=context;
    if(s_device) s_device->AddRef();
    if(s_context) s_context->AddRef();
}

void ShutdownOnlineMenuRenderer()
{
    ReleaseMenuTarget();
    if(s_labelTexture) { s_labelTexture->Release(); s_labelTexture=nullptr; }
    if(s_compositeBlend) { s_compositeBlend->Release(); s_compositeBlend=nullptr; }
    if(s_context) { s_context->Release(); s_context=nullptr; }
    if(s_device) { s_device->Release(); s_device=nullptr; }
}

void DrawOnlineMainMenu()
{
    const OnlineMenuState state=online_menu_state(g_doa3_frames_presented);
    if(!state.visible) return;
    const ImVec2 display=ImGui::GetIO().DisplaySize;
    if(display.x<=0 || display.y<=0) return;
    const float aspect=video_display_aspect(video_get_aspect());
    const float h=std::min(display.y,display.x/aspect), fullw=h*aspect;
    const float fullx=(display.x-fullw)*.5f, y=(display.y-h)*.5f;
    float hudx,hudw;
    video_hud_region((unsigned)fullw,&hudx,&hudw);
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(display);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(0,0));
    ImGui::Begin("##OnlineModeSelect",nullptr,ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImDrawList *output=ImGui::GetWindowDrawList();
    ImDrawList art(ImGui::GetDrawListSharedData());
    const unsigned nativeWidth=(unsigned)std::lround(720.f*fullw/hudw);
    const bool native=PrepareMenuTarget(nativeWidth);
    art._ResetForNewFrame();
    art.PushTexture(ImGui::GetIO().Fonts->TexRef);
    art.PushClipRect(ImVec2(0,0),ImVec2((float)nativeWidth,480));
    MenuCanvas c=native ? MenuCanvas{&art,(nativeWidth-720.f)*.5f,0,720,480} :
                         MenuCanvas{output,fullx+hudx,y,hudw,h};
    const OnlineMenuSprite *sprites;
    const unsigned count=online_menu_sprites(g_doa3_frames_presented,&sprites);
    int selectedAlpha=0;
    if(state.page==ONLINE_MENU_CAROUSEL) {
        PrepareCarouselLabel();
        for(unsigned i=0;i<count;++i) {
            const auto &sprite=sprites[i];
            const int alpha=(int)(std::min(1.f,std::max(0.f,sprite.alpha))*255);
            if(sprite.label) {
                selectedAlpha=std::max(selectedAlpha,alpha);
                if(s_labelTexture) {
                    const float width=sprite.w*ONLINE_MENU_LABEL_WIDTH/127.f;
                    const float left=sprite.x+(sprite.w-width)*.5f;
                    // The borrowed letters mostly come from the bright ends
                    // of native labels. Match their average silver intensity,
                    // and restore a separate drop shadow outside the outline.
                    const float dx=sprite.w/127.f, dy=sprite.h/12.f;
                    const ImTextureID texture=(ImTextureID)(intptr_t)s_labelTexture;
                    auto shadow=[&](float ox,float oy,int opacity) {
                        c.draw->AddImage(texture,
                            c.point(left+ox,sprite.y+oy),
                            c.point(left+width+ox,sprite.y+sprite.h+oy),
                            ImVec2(0,0),ImVec2(1,1),IM_COL32(0,0,0,alpha*opacity/255));
                    };
                    shadow(dx*.5f,dy*.75f,64);
                    shadow(dx*1.5f,dy*1.25f,64);
                    shadow(dx,dy,176);
                    c.draw->AddImage((ImTextureID)(intptr_t)s_labelTexture,
                        c.point(left,sprite.y),c.point(left+width,sprite.y+sprite.h),
                        ImVec2(0,0),ImVec2(1,1),IM_COL32(235,235,235,alpha));
                } else {
                    c.text(sprite.x+sprite.w*.5f,sprite.y,"ONLINE",19,s_title,
                           IM_COL32(235,235,235,alpha),.5f);
                }
            } else {
                c.globe(sprite.x+sprite.w*.5f,sprite.y+sprite.h*.36f,
                        sprite.w*.30f,sprite.h*.24f,sprite.alpha);
            }
        }
        if(selectedAlpha)
            c.text(155,427,"Select an online game mode",14,s_body,IM_COL32(255,255,255,selectedAlpha));
    } else {
        // Native-style full menu; Host/Join remain frontend previews only.
        const float bx=native ? 0 : fullx, by=native ? 0 : y;
        const float bw=native ? (float)nativeWidth : fullw, bh=native ? 480 : h;
        c.draw->AddRectFilled(ImVec2(bx,by),ImVec2(bx+bw,by+bh),IM_COL32(30,30,30,255));
        for(int row=25;row<480;row+=5)
            c.draw->AddLine(ImVec2(bx,by+row*bh/480),ImVec2(bx+bw,by+row*bh/480),
                           IM_COL32(12,12,12,255),std::max(1.f,bh/480));
        c.gradient(0,72,640,416,IM_COL32(65,65,65,95),IM_COL32(0,0,0,145));
        c.rect(0,0,640,42,IM_COL32(0,0,0,255));
        ImVec2 band[]={c.point(252,40),c.point(640,40),c.point(640,68),c.point(286,68)};
        c.draw->AddConvexPolyFilled(band,4,IM_COL32(0,0,0,255));
        for(int row=0;row<42;row+=3)
            c.gradient(250,(float)row,640,(float)row+1,IM_COL32(0,0,0,0),IM_COL32(80,80,80,120));
        c.text(610,8,"DEAD OR ALIVE 3",34,s_body,IM_COL32(66,66,66,150),1,false);
        c.text(600,34,"ONLINE",28,s_heading,IM_COL32_WHITE,1);
        c.text(37,16,"MODE SELECT",15,s_title);
        c.line(42,102,598,102,IM_COL32(126,126,126,255));
        c.globe(157,233,70,70,.85f);
        auto button=[&](const char *label,float row,bool lit) {
            c.gradient(288,row,578,row+36,lit ? IM_COL32(139,139,139,255) : IM_COL32(43,43,43,240),IM_COL32(0,0,0,200));
            c.line(288,row,578,row,lit ? IM_COL32(239,239,239,255) : IM_COL32(87,87,87,255));
            c.line(288,row+36,578,row+36,IM_COL32(95,95,95,255));
            if(lit) {
                ImVec2 arrow[]={c.point(278,row+10),c.point(286,row+18),c.point(278,row+26)};
                c.draw->AddConvexPolyFilled(arrow,3,IM_COL32_WHITE);
                c.rect(288,row,291,row+36,IM_COL32(231,231,231,255));
            }
            c.text(310,row+4,label,23,s_title,lit ? IM_COL32_WHITE : IM_COL32(173,173,173,255));
        };
        const char *description="Return to mode select";
        if(state.page==ONLINE_MENU_OPTIONS) {
            button("HOST GAME",166,state.selection==0);
            button("JOIN GAME",220,state.selection==1);
            button("BACK",274,state.selection==2);
            description=state.selection==0 ? "Create a room for another player" :
                        state.selection==1 ? "Join another player's room" : description;
            c.text(433,338,"Network play is not available yet",11,s_body,IM_COL32(175,175,175,255),.5f);
        } else {
            const bool host=state.page==ONLINE_MENU_HOST;
            c.text(310,151,host ? "HOST GAME" : "JOIN GAME",23,s_title);
            c.line(288,184,578,184,IM_COL32(153,153,153,255));
            c.text(310,207,host ? "Create your room" : "Find a player's room",15,s_body);
            c.text(310,242,host ? "Hosting is not available yet." : "Joining is not available yet.",12,s_body,
                   IM_COL32(184,184,184,255));
            button("BACK",298,true);
            description="Return to the previous menu";
        }
        c.information(description);
        c.text(600,459,"Up / Down: Select    Confirm: Choose    Back: Return",10,s_body,
               IM_COL32(190,190,190,255),1);
    }
    art.PopClipRect();
    art.PopTexture();
    if(native) CompositeMenu(art,output,ImVec2(fullx,y),ImVec2(fullw,h));
    ImGui::End();
    ImGui::PopStyleVar();
}
