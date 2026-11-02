#include "performance_overlay.h"
#include "imgui.h"
#include <windows.h>
#include <algorithm>
#include <cmath>

extern "C" {
UINT d3d8_GetBackbufferWidth(void);
UINT d3d8_GetBackbufferHeight(void);
}

namespace {
constexpr int history_size = 240;
bool enabled;
float history[history_size];
int count, next;
LARGE_INTEGER frequency, previous, refreshed;
float average_ms, worst_ms, percentile_ms;

void ClearHistory()
{
    count = next = 0;
    previous.QuadPart = refreshed.QuadPart = 0;
    average_ms = worst_ms = percentile_ms = 0;
}

float HistoryValue(void *, int index)
{
    int start = count == history_size ? next : 0;
    return history[(start + index) % history_size];
}
}

bool PerformanceOverlayEnabled() { return enabled; }
void TogglePerformanceOverlay() { enabled = !enabled; ClearHistory(); }   /* 1.0.2: L3 + Y */
void ResetPerformanceOverlay()
{
    enabled = false;
    ClearHistory();
}

void TickPerformanceOverlay()
{
    // Called at the same point immediately before each host presentation.
    // No timer queries, sorting, allocations or file writes while disabled.
    if (!enabled) return;
    if (!frequency.QuadPart && !QueryPerformanceFrequency(&frequency)) return;
    LARGE_INTEGER now;
    if (!QueryPerformanceCounter(&now)) return;
    if (previous.QuadPart) {
        float ms = static_cast<float>(1000.0 * (now.QuadPart - previous.QuadPart) / frequency.QuadPart);
        if (std::isfinite(ms) && ms > 0) {
            history[next] = ms;
            next = (next + 1) % history_size;
            if (count < history_size) ++count;
        }
    }
    previous = now;
    // Readable statistics refresh four times per second; the graph updates
    // every frame. FPS is frames / elapsed time, not averaged instantaneous FPS.
    if (count && (!refreshed.QuadPart ||
        now.QuadPart - refreshed.QuadPart >= frequency.QuadPart / 4)) {
        float sorted[history_size];
        double sum = 0;
        for (int i=0; i<count; ++i) { sorted[i] = history[i]; sum += history[i]; }
        std::sort(sorted, sorted + count);
        average_ms = static_cast<float>(sum / count);
        worst_ms = sorted[count - 1];
        percentile_ms = sorted[(99 * count + 99) / 100 - 1];
        refreshed = now;
    }
}

/* 0.2.18: one checkbox in the Video tab (was Debug > Performance). */
void DrawPerformanceDebugMenu()
{
    if (ImGui::Checkbox("Show performance overlay", &enabled)) ClearHistory();
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip)) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 30.0f);
        ImGui::TextUnformatted("Presentation FPS and frame intervals over the latest 240 frames: a frame-time "
                               "graph, average, 99th percentile and worst frame. Includes frame pacing and "
                               "presentation waits; not GPU render time or simulation FPS. Session only; "
                               "nothing is written to disk.");
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void DrawPerformanceOverlay()
{
    if (!enabled) return;
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x - 12,
                                viewport->WorkPos.y + 12), ImGuiCond_Always, ImVec2(1,0));
    ImGui::SetNextWindowBgAlpha(.8f);
    if (ImGui::Begin("Performance overlay", nullptr, ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing)) {
        if (count && average_ms > 0) {
            ImGui::Text("Presentation: %.1f FPS", 1000.f / average_ms);
            ImGui::Text("Frame: %.2f ms   Avg: %.2f ms",
                HistoryValue(nullptr,count-1), average_ms);
            ImGui::Text("P99: %.2f ms   Worst: %.2f ms", percentile_ms, worst_ms);
            // Limit the graph's scale after a long stall without hiding its
            // measured value in the numeric readout above.
            float scale = std::fmin(100.f, std::fmax(33.34f, worst_ms * 1.1f));
            ImGui::PlotLines("##frame_times", HistoryValue, nullptr, count, 0,
                nullptr, 0, scale, ImVec2(260,65));
            ImGui::TextDisabled("Last %d frames | 60 FPS = 16.67 ms", count);
        } else ImGui::TextUnformatted("Performance: collecting frames...");
        ImGui::TextDisabled("Render resolution: %u x %u",
            d3d8_GetBackbufferWidth(),d3d8_GetBackbufferHeight());
    }
    ImGui::End();
}
