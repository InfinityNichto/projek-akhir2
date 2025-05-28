#include <GLFW/glfw3.h>
#include "imgui.h"
#include <gui_vars.hpp>
#include <utils.hpp>
#include <gui/utils.hpp>
#include <gui/nav_bar.hpp>
#include <gui/menu_items.hpp>
#include <gui/workspace.hpp>

void NavBar::draw_window() {
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(GuiVars::display_size().x, 46));
	ImGui::SetNextWindowBgAlpha(1);

	ImGuiWindowFlags window_flags = GuiVars::common_window_flags | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	if (ImGui::Begin("##menu_bar", NULL, window_flags)) {
        NavBar::do_menu_bar();

        MenuItems::Edit::do_style_editor();
        MenuItems::Tools::do_metrics();
        MenuItems::Tools::do_stack_tool();
        MenuItems::Tools::do_demo_window();
        MenuItems::Help::do_resources(&MenuItems::Help::show_resources);
        MenuItems::Help::do_about_window(&MenuItems::Help::show_about_window);

		MenuItems::Edit::do_style_editor();
        MenuItems::Tools::do_metrics();
        MenuItems::Tools::do_stack_tool();
        MenuItems::Tools::do_demo_window();
        MenuItems::Help::do_resources(&MenuItems::Help::show_resources);
        MenuItems::Help::do_about_window(&MenuItems::Help::show_about_window);

        NavBar::do_tab_bar();

        GuiVars::update_prev_window_data();
		ImGui::End();
	}
}

void NavBar::do_menu_bar() {
	if (!ImGui::BeginMenuBar()) return;

	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("Exit")) MenuItems::File::do_exit();
		ImGui::SetItemTooltip("Exit the program");

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Edit")) {
		if (ImGui::MenuItem("Style Editor", NULL, MenuItems::Edit::show_style_editor)) MenuItems::Edit::show_style_editor = !MenuItems::Edit::show_style_editor;
		ImGui::SetItemTooltip("Edit current layout styling");

		ImGui::BeginDisabled();
			if (ImGui::MenuItem("Reset")) MenuItems::Edit::reset_styles();
			ImGui::SetItemTooltip("Reset layout styling"); ImGui::SameLine(); GuiUtils::help_marker("Unimplemented");
		ImGui::EndDisabled();

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Tools")) {

		if (ImGui::MenuItem("Metrics/Debugger", NULL, MenuItems::Tools::show_metrics)) MenuItems::Tools::show_metrics = !MenuItems::Tools::show_metrics;
		ImGui::SetItemTooltip("Show utility window for metrics and debugging");

		if (ImGui::MenuItem("ID stack tool", NULL, MenuItems::Tools::show_stack_tool)) MenuItems::Tools::show_stack_tool = !MenuItems::Tools::show_stack_tool;
		ImGui::SetItemTooltip("Show utility window for inspecting element ID stack");

		if (ImGui::MenuItem("Demo window", NULL, MenuItems::Tools::show_demo_window)) MenuItems::Tools::show_demo_window = !MenuItems::Tools::show_demo_window;
		ImGui::SetItemTooltip("Show the demo window");

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help")) {
		ImGui::BeginDisabled();
			if (ImGui::MenuItem("Guide")) MenuItems::Help::do_report();
			ImGui::SetItemTooltip("Open the guide document"); ImGui::SameLine(); GuiUtils::help_marker("Unimplemented");
		ImGui::EndDisabled();

		if (ImGui::MenuItem("Resources")) MenuItems::Help::show_resources = !MenuItems::Help::show_resources;
		ImGui::SetItemTooltip("Open useful resources for ImGui development");

		if (ImGui::MenuItem("About")) MenuItems::Help::show_about_window = !MenuItems::Help::show_about_window;
		ImGui::SetItemTooltip("About this program");

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
}

void NavBar::do_tab_bar() {
	if (!ImGui::BeginTabBar("##tab_bar")) return;

	if (ImGui::BeginTabItem("Workspace")) {
        Workspace::draw_the_whole_thing();
        ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Logs")) {
		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();
}


