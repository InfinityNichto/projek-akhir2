#include <chrono>
#include "imgui.h"
#include <gui_vars.hpp>

ImVec2 GuiVars::display_size() {
    return ImGui::GetIO().DisplaySize;
}

Rect GuiVars::get_current_window_data() {
    return Rect(ImGui::GetWindowPos(), ImGui::GetWindowSize());
}

void GuiVars::update_prev_window_data() {
    Rect current_data = GuiVars::get_current_window_data();
    GuiVars::prev_window_data = current_data;
}

void GuiVars::init_imgui_stats() {
	ImGuiIO io = ImGui::GetIO();
    GuiVars::mouse_pos = io.MousePos;
    GuiVars::fps = io.Framerate;
}

void GuiVars::update_imgui_stats() {
	ImGuiIO io = ImGui::GetIO();
    GuiVars::mouse_pos = io.MousePos;
    GuiVars::fps = io.Framerate;
    GuiVars::global_tick++;
}

// thanks, stack overflow
time_t GuiVars::get_current_time() {
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

