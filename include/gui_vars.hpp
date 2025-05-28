#ifndef GUI_VARS_HPP
#define GUI_VARS_HPP

#include "imgui.h"
#include <GLFW/glfw3.h>
#include <utils.hpp>
#include <graph.hpp>

// forward declaration
struct Rect;

inline static Graph* current_graph = new Graph();

struct GuiVars {
    inline static GLFWwindow* glfw_window = nullptr;
    static constexpr ImGuiWindowFlags common_window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
	            			   								ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus;
    inline static Rect prev_window_data = {0, 0, 0, 0};
    inline static ImVec2 mouse_pos = {0, 0};
    inline static float fps = 0;
    inline static long global_tick = 0;

    static ImVec2 display_size();
    static Rect get_current_window_data();
    static void update_prev_window_data();
    static void init_imgui_stats();
    static void update_imgui_stats();
    static time_t get_current_time();
};

#endif

