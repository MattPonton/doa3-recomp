#include "combat_overlay.h"
#include "imgui.h"
#include "../game/combat_debug.h"
#include "../game/video_settings.h"
#include "../online/netplay_session.h"
#include <cmath>

void DrawCombatDebugMenu()
{
    if (!ImGui::CollapsingHeader("Combat diagnostics")) return;
    unsigned flags = combat_debug_flags();
    bool hit = (flags & COMBAT_HITBOXES) != 0;
    bool hurt = (flags & COMBAT_HURTBOXES) != 0;
    bool frames = (flags & COMBAT_FRAMES) != 0;
    ImGui::BeginDisabled(netplay_active() != 0);
    bool changed = ImGui::Checkbox("Show hitboxes", &hit);
    changed |= ImGui::Checkbox("Show hurtboxes", &hurt);
    changed |= ImGui::Checkbox("Show attack frame data", &frames);
    if (changed) combat_debug_set_flags((hit ? COMBAT_HITBOXES : 0) |
        (hurt ? COMBAT_HURTBOXES : 0) | (frames ? COMBAT_FRAMES : 0));
    ImGui::EndDisabled();
    ImGui::TextWrapped("Red/pink solid spheres: hitboxes. Green/cyan wireframes: hurtboxes. Amber: an attack already registered contact. Volumes draw through characters and scenery.");
    ImGui::TextWrapped("Frame data measures strikes as you play: first active frame, active-frame count, and time after the last active frame. Cancels and interruptions shorten the observation. It does not report frame advantage or throw range.");
    ImGui::TextDisabled("Session only. Unavailable during online play.");
}

namespace {
struct View {
    const float *matrix;
    ImDrawList *draw;
    float x, y, w, h;
    void line(const float a[3], const float b[3], ImU32 color) const {
        float p[4];
        if (!combat_debug_project_line(matrix, a, b, p)) return;
        draw->AddLine(ImVec2(x+(p[0]+1)*w*.5f, y+(1-p[1])*h*.5f),
                      ImVec2(x+(p[2]+1)*w*.5f, y+(1-p[3])*h*.5f), color, 1.5f);
    }
    void triangle(const float a[3], const float b[3], const float c[3], ImU32 color) const {
        float polygon[12][4], clipped[12][4];
        const float *vertices[3] = {a,b,c};
        int count = 3;
        for (int i=0; i<3; ++i) {
            for (int j=0; j<4; ++j) {
                polygon[i][j] = vertices[i][0]*matrix[j] + vertices[i][1]*matrix[4+j]
                    + vertices[i][2]*matrix[8+j] + matrix[12+j];
                if (!std::isfinite(polygon[i][j])) return;
            }
        }
        // Clip surfaces before division so flying through a hitbox cannot
        // produce inverted or enormous triangles at the near plane.
        for (int plane=0; plane<7 && count; ++plane) {
            auto distance = [plane](const float *p) {
                switch (plane) {
                case 0: return p[3]-.0001f;
                case 1: return p[2];
                case 2: return p[3]-p[2];
                case 3: return p[3]+p[0];
                case 4: return p[3]-p[0];
                case 5: return p[3]+p[1];
                default: return p[3]-p[1];
                }
            };
            int next = 0;
            for (int i=0; i<count; ++i) {
                const float *p=polygon[i], *q=polygon[(i+1)%count];
                float dp=distance(p), dq=distance(q);
                if (dp >= 0) {
                    for (int j=0; j<4; ++j) clipped[next][j]=p[j];
                    ++next;
                }
                if ((dp < 0) != (dq < 0)) {
                    float t=dp/(dp-dq);
                    for (int j=0; j<4; ++j) clipped[next][j]=p[j]+t*(q[j]-p[j]);
                    ++next;
                }
            }
            count=next;
            for (int i=0; i<count; ++i)
                for (int j=0; j<4; ++j) polygon[i][j]=clipped[i][j];
        }
        if (count < 3) return;
        ImVec2 points[12];
        for (int i=0; i<count; ++i)
            points[i]=ImVec2(x+(polygon[i][0]/polygon[i][3]+1)*w*.5f,
                            y+(1-polygon[i][1]/polygon[i][3])*h*.5f);
        draw->AddConvexPolyFilled(points,count,color);
    }
};
void solid_sphere(const View &view, const CombatVolume &v, ImU32 color)
{
    constexpr int slices=24, rings=12;
    float points[rings+1][slices][3];
    for (int i=0; i<=rings; ++i) {
        float latitude=i*3.14159265359f/rings;
        for (int j=0; j<slices; ++j) {
            float longitude=j*6.28318530718f/slices;
            points[i][j][0]=v.a[0]+v.radius*std::sin(latitude)*std::cos(longitude);
            points[i][j][1]=v.a[1]+v.radius*std::cos(latitude);
            points[i][j][2]=v.a[2]+v.radius*std::sin(latitude)*std::sin(longitude);
        }
    }
    // Both surfaces contribute a light tint; no mesh-edge antialiasing,
    // which would leave dark seams between the translucent triangles.
    ImDrawListFlags old_flags=view.draw->Flags;
    view.draw->Flags &= ~ImDrawListFlags_AntiAliasedFill;
    color=(color & ~IM_COL32_A_MASK) | (80u << IM_COL32_A_SHIFT);
    for (int i=0; i<rings; ++i) for (int j=0; j<slices; ++j) {
        int next=(j+1)%slices;
        if (i) view.triangle(points[i][j],points[i+1][j],points[i][next],color);
        if (i+1<rings) view.triangle(points[i][next],points[i+1][j],points[i+1][next],color);
    }
    view.draw->Flags=old_flags;
}
void circle(const View &view, const float center[3], const float u[3],
            const float v[3], float radius, ImU32 color)
{
    float previous[3];
    for (int j=0; j<3; ++j) previous[j] = center[j] + radius*u[j];
    for (int i=1; i<=24; ++i) {
        float angle = i*(6.28318530718f/24), next[3];
        for (int j=0; j<3; ++j)
            next[j] = center[j] + radius*(u[j]*std::cos(angle) + v[j]*std::sin(angle));
        view.line(previous, next, color);
        for (int j=0; j<3; ++j) previous[j] = next[j];
    }
}
void shape(const View &view, const CombatVolume &v)
{
    ImU32 color = v.attack ? (v.side ? IM_COL32(255,85,170,240) : IM_COL32(255,70,55,240))
                          : (v.side ? IM_COL32(60,210,255,185) : IM_COL32(60,255,125,185));
    if (v.spent) color = IM_COL32(255,190,55,200);
    if (v.attack && !v.cylinder) {
        solid_sphere(view,v,color);
        return;
    }
    if (!v.cylinder) {
        const float axes[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
        for (int i=0; i<3; ++i) circle(view, v.a, axes[i], axes[(i+1)%3], v.radius, color);
        return;
    }
    float d[3], u[3], w[3];
    float length = 0;
    for (int j=0; j<3; ++j) { d[j]=v.b[j]-v.a[j]; length+=d[j]*d[j]; }
    if (length < 1e-10f) return;
    length = std::sqrt(length);
    for (float &x : d) x /= length;
    // Cross with the least parallel of world up and world right.
    if (std::fabs(d[1]) < .9f) { u[0]=-d[2]; u[1]=0; u[2]=d[0]; }
    else { u[0]=0; u[1]=d[2]; u[2]=-d[1]; }
    length=std::sqrt(u[0]*u[0]+u[1]*u[1]+u[2]*u[2]);
    for (float &x : u) x /= length;
    w[0]=d[1]*u[2]-d[2]*u[1]; w[1]=d[2]*u[0]-d[0]*u[2]; w[2]=d[0]*u[1]-d[1]*u[0];
    circle(view, v.a, u, w, v.radius, color);
    circle(view, v.b, u, w, v.radius, color);
    for (int i=0; i<8; ++i) {
        float a[3], b[3], angle=i*6.28318530718f/8;
        for (int j=0; j<3; ++j) {
            float offset=v.radius*(u[j]*std::cos(angle)+w[j]*std::sin(angle));
            a[j]=v.a[j]+offset; b[j]=v.b[j]+offset;
        }
        view.line(a,b,color);
    }
}
void timing(const char *label, const CombatTiming &t)
{
    if (!t.valid) { ImGui::TextDisabled("%s: waiting for a strike", label); return; }
    ImGui::Text("%s: move %04X%s", label, t.move,
        t.interrupted ? " (interrupted/chained)" : (t.partial ? " (partial)" : ""));
    if (t.first_active) {
        if (t.partial) ImGui::Text("First observed active: %u", t.first_active);
        else ImGui::Text("Startup / first active: %u", t.first_active);
        ImGui::Text("Active: %u   Last active: %u", t.active_frames, t.last_active);
        ImGui::Text("Recovery observed: %u%s", t.frame-t.last_active, t.ended ? "" : " (counting)");
    } else ImGui::Text("Elapsed: %u   No active frames yet", t.frame);
}
}

void DrawCombatDebugOverlay()
{
    unsigned flags = combat_debug_flags();
    if (!flags) return;
    CombatSnapshot snapshot;
    if (!combat_debug_snapshot(&snapshot)) return;
    ImVec2 display = ImGui::GetIO().DisplaySize;
    if (display.x <= 0 || display.y <= 0) return;
    float aspect = video_get_aspect() == VIDEO_ASPECT_16_9 ? 16.f/9.f : 4.f/3.f;
    float w = display.x, h = display.y;
    if (w/h > aspect) w=h*aspect; else h=w/aspect;
    View view = {snapshot.matrix, ImGui::GetBackgroundDrawList(),
        std::floor((display.x-w)*.5f), std::floor((display.y-h)*.5f), w, h};
    view.draw->PushClipRect(ImVec2(view.x,view.y), ImVec2(view.x+w,view.y+h), true);
    // Fill attacks first so body wireframes remain legible where they overlap.
    for (int attack=1; attack>=0; --attack)
        for (unsigned i=0; i<snapshot.count; ++i) {
            const CombatVolume &v = snapshot.volumes[i];
            if (v.attack == attack && (flags & (attack ? COMBAT_HITBOXES : COMBAT_HURTBOXES)))
                shape(view,v);
        }
    view.draw->PopClipRect();
    if (!(flags & COMBAT_FRAMES)) return;
    ImGui::SetNextWindowPos(ImVec2(view.x+12,view.y+h-12), ImGuiCond_Always, ImVec2(0,1));
    ImGui::SetNextWindowBgAlpha(.8f);
    if (ImGui::Begin("Attack frame data", nullptr, ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing)) {
        if (ImGui::BeginTable("Combat sides",2, ImGuiTableFlags_SizingFixedFit)) {
            for (unsigned side=0; side<2; ++side) {
                ImGui::TableNextColumn();
                const CombatFighter &f=snapshot.fighter[side];
                ImGui::Text("P%u   Move %04X   Frame %u",side+1,f.move,f.frame);
                ImGui::Text("%s   HP %u%s", f.active ? "Active" : (f.current.valid ?
                    (f.current.last_active ? "Recovery" : "Startup") : "Neutral / other"),
                    f.health, f.contact ? "   Contact" : "");
                timing("Current",f.current);
                ImGui::Separator();
                timing("Previous",f.previous);
            }
            ImGui::EndTable();
        }
        ImGui::TextDisabled("Observed strike timing; cancels shorten recovery. No frame advantage.");
    }
    ImGui::End();
}
