#include <iomanip>
#include "imgui.h"
#include <gui_vars.hpp>
#include <utils.hpp>
#include <gui/utils.hpp>

void RgbCycle::step(float inc) {
    if (this->phase >= 3) this->phase = 0;

    size_t next = (this->phase + 1) % 3;

    float* components[3] = { &this->color.x, &this->color.y, &this->color.z };
    float& cur = *components[this->phase];
    float& nx = *components[next];

    if (nx <= 1.0f) {
        nx += inc;
        if (nx > 1.0f) nx = 1.0f;
    } else if (cur >= 0.0f) {
        cur -= inc;
        if (cur < 0.0f) cur = 0.0f;
    } else {
        inc = next;
    }
}

void GuiUtils::hyperlink(const char* desc, const char* url) {
	ImGui::TextColored(ImVec4(0.0f, 0.5f, 1.0f, 1.0f), "%s", desc);

	if (ImGui::IsItemHovered()) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		ImGui::SetTooltip("%s", url);
		if (ImGui::IsMouseClicked(0)) GuiUtils::open_url(url);
	}

	ImGui::SetItemTooltip("%s", url); ImGui::SameLine(); help_marker("If your OS is neither Windows, Linux, or MacOS, the URL will be copied to your clipboard instead");
}

void GuiUtils::text_centered(const char* text, int type) {
    float window_width = ImGui::GetWindowSize().x;
    float text_width = ImGui::CalcTextSize(text).x;

    ImGui::SetCursorPosX((window_width - text_width) * 0.5);
    switch (type) {
      case 0:
        ImGui::TextUnformatted(text);
        break;   
      case 1:
        ImGui::TextDisabled("%s", text);
        break;
    }
}

void GuiUtils::text_centered_vh(const char* text, int type) {
    Rect window_data = GuiVars::get_current_window_data();
    ImVec2 text_size = ImGui::CalcTextSize(text);

    ImGui::SetCursorPos(ImVec2((window_data.w - text_size.x) * 0.5, (window_data.h * 0.5) - text_size.y));
    switch(type) {
      case 0:
        ImGui::TextUnformatted(text);
        break;
      case 1:
        ImGui::TextDisabled("%s", text);
        break;
    }
}

void GuiUtils::separator_heading(const char* heading) {
    ImGui::Spacing(); GuiUtils::text_centered(heading, 1);
    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
}

void GuiUtils::help_marker(const char* desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void GuiUtils::open_url(const char* url) {
#ifdef _WIN32
	ShellExecute(0, "open", url, 0, 0, SW_SHOWNORMAL);
#elif __linux__
	system(("xdg-open " + std::string(url)).c_str());
#elif __APPLE__
	system(("open " + std::string(url)).c_str());
#else
	ImGui::LogToClipboard();
	ImGui::LogText(url);
    ImGui::LogFinish();
#endif
}

void GuiUtils::draw_grid(size_t count, float thickness) {
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 window_pos = ImGui::GetWindowPos();
	ImVec2 window_size = ImGui::GetWindowSize();
	ImVec2 step = ImVec2(window_size.x / count, window_size.y / count);

	for (size_t i = 0; i < count; i++) {
		ImVec2 incremented = ImVec2(window_pos.x + step.x * i, window_pos.y + step.y * i);
	
		draw_list->AddLine(
			ImVec2(window_pos.x, incremented.y),
			ImVec2(window_pos.x + window_size.x, incremented.y),
			IM_COL32(88, 88, 88, 176),
			thickness
		);

		draw_list->AddLine(
			ImVec2(incremented.x, window_pos.y),
			ImVec2(incremented.x, window_pos.y + window_size.y),
			IM_COL32(88, 88, 88, 176),
			thickness
		);
	}
}

void GuiUtils::arrow(const ImVec2& p1, const ImVec2& p2, ImU32 col, float thickness, const char* text) {
    ImVec2 mid = ImVec2((p2.x + p1.x) * 0.5, (p2.y + p1.y) * 0.5);

    ImVec2 dir = ImVec2(p2.x - p1.x, p2.y - p1.y);
    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (len == 0.0) return;
    dir.x /= len;
    dir.y /= len;

    ImVec2 perp = ImVec2(-dir.y, dir.x);

    float size = 5.0f;
    ImVec2 left = ImVec2(mid.x - dir.x * size + perp.x * size, mid.y - dir.y * size + perp.y * size);
    ImVec2 right = ImVec2(mid.x - dir.x * size - perp.x * size, mid.y - dir.y * size - perp.y * size);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddLine(p1, p2, col, thickness);
    draw_list->AddLine(left, mid, col, thickness * 0.5);
    draw_list->AddLine(right, mid, col, thickness * 0.5);

    if (text) {
        float text_offset = 10.0f;
        ImVec2 text_pos = ImVec2(mid.x + perp.x * text_offset, mid.y + perp.y * text_offset);

        draw_list->AddText(text_pos, col, text);
    }
}

tm GuiUtils::as_localtime(time_t time) {
    return *localtime(&time);
}

std::string GuiUtils::format_time(tm localtime, const char* fmt) {
    return Utils::build_str(std::put_time(&localtime, fmt));
}

