#ifndef GUI_UTILS_HPP
#define GUI_UTILS_HPP

#include "imgui.h"
#include <string>
#include <vector>

struct RgbCycle {
    ImVec4 color = {1, 0, 0, 1};
    size_t phase = 0;

    void step(float inc);
};

struct GuiUtils {
    static void hyperlink(const char* desc, const char* url);
    static void text_centered(const char* text, int type);
    static void text_centered_vh(const char* text, int type);
    static void help_marker(const char* desc);
    static void open_url(const char* url);
    static void cycle_rainbow(ImVec4* color, size_t* cycle_ptr, float inc);
    static void separator_heading(const char* heading);
    static void draw_grid(size_t count, float thickness);
    static void arrow(const ImVec2& p1, const ImVec2& p2, ImU32 col, float thickness, const char* text = nullptr);
    static tm as_localtime(time_t time);
    static std::string format_time(tm localtime, const char* fmt);

    template <typename... Args> inline static void text_hover_formatted(const char* fmt, Args... args) {
    	if (ImGui::IsItemHovered()) {
    		ImGui::SetTooltip(fmt, args...);
    	}
    }

    template <typename... Args> inline static void text_centered_formatted(const char* fmt, Args... args) {
    	size_t buf_len = std::snprintf(nullptr, 0, fmt, args...) + 1;
    	std::vector<char> buf(buf_len);
    	std::snprintf(buf.data(), buf_len, fmt, args...);
    	const char* text = buf.data();

    	float window_width = ImGui::GetWindowSize().x;
        float text_width = ImGui::CalcTextSize(text).x;

        ImGui::SetCursorPosX((window_width - text_width) * 0.5);
        ImGui::TextUnformatted(text);
    }

    template <typename... Args> inline static void text_wrapped(const char* fmt, Args... args) {
    	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + ImGui::GetContentRegionAvail().x);
    	ImGui::TextWrapped(fmt, args...);
    	ImGui::PopTextWrapPos();
    }

    template <typename... Args> inline static void log_clipboard(const char* fmt, Args... args) {
    	ImGui::LogToClipboard();
    	ImGui::LogText(fmt, args...);
    	ImGui::LogFinish();
    }

    template <typename... Args> inline static void log_tty(const char* fmt, Args... args) {
    	ImGui::LogToTTY();
    	ImGui::LogText(fmt, args...);
    	ImGui::LogFinish();
    }

    template <typename... Args> inline static void log_file(const char* path, const char* fmt, Args... args) {
    	ImGui::LogToFile(0, path);
    	ImGui::LogText(fmt, args...);
    	ImGui::LogFinish();
    }
};

#endif
