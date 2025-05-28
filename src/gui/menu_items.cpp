#include <GLFW/glfw3.h>
#include "imgui.h"
#include <gui_vars.hpp>
#include <gui/utils.hpp>
#include <gui/menu_items.hpp>

void MenuItems::Edit::do_style_editor() {
	if (MenuItems::Edit::show_style_editor) {
		ImGui::Begin("Style Editor", &MenuItems::Edit::show_style_editor);
    	ImGui::ShowStyleEditor();
	    ImGui::End();
	}
}

void MenuItems::Edit::reset_styles() {
}

void MenuItems::File::do_exit() {
	glfwSetWindowShouldClose(GuiVars::glfw_window, GLFW_TRUE);
}

void MenuItems::Help::do_report() { }

void MenuItems::Help::do_resources(bool* show) {
	if (!(*show)) return;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.00f, 2.00f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.00f, 3.00f));
    ImGui::SetNextWindowSize(ImVec2(500,600), ImGuiCond_Once);

	if (ImGui::Begin("Resources", show, ImGuiWindowFlags_NoCollapse)) {
		ImGui::TextWrapped("Some useful resources for developers using Dear ImGui.");
	    ImGui::TextWrapped("Keep in mind that the most helpful resource will always be the Dear ImGui Demo (Tools > Demo Window) and imgui/imgui_demo.cpp.");
    	ImGui::Spacing();

		if (ImGui::CollapsingHeader("Dear ImGui", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Bullet();
            GuiUtils::hyperlink("Dear ImGui Source: github.com/ocornut/imgui", "https://github.com/ocornut/imgui");
			
			ImGui::Bullet();
	    	GuiUtils::hyperlink("Issues", "https://github.com/ocornut/imgui/issues");

			ImGui::Bullet();
			GuiUtils::hyperlink("Wiki", "https://github.com/ocornut/imgui/wiki");

			ImGui::Bullet();
			GuiUtils::hyperlink("Manual", "https://pthom.github.io/imgui_manual_online");

	   		ImGui::Bullet();
			GuiUtils::hyperlink("discourse.dearimgui.org", "https://discourse.dearimgui.org/");

    		ImGui::Bullet();
    		GuiUtils::hyperlink("Discussions", "https://github.com/ocornut/imgui/discussions");

    		ImGui::Bullet();
    		GuiUtils::hyperlink("How to open an Issue or Pull Request #2261", "https://github.com/ocornut/imgui/issues/2261");

    		ImGui::Bullet();
    		GuiUtils::hyperlink("Docking", "https://github.com/ocornut/imgui/wiki/Docking");

    		ImGui::Bullet();
    		GuiUtils::hyperlink("Gallery", "https://github.com/ocornut/imgui/issues/4451");

    		ImGui::Bullet();
    		GuiUtils::hyperlink("Software using DearImGui", "https://github.com/ocornut/imgui/wiki/Software-using-dear-imgui");
		}

    	ImGui::Spacing();
    	
		if (ImGui::CollapsingHeader("Cool Addons", ImGuiTreeNodeFlags_DefaultOpen)) {
        	ImGui::Bullet();
        	GuiUtils::hyperlink("HankiDesign/awesome-dear-imgui", "https://github.com/HankiDesign/awesome-dear-imgui");

        	ImGui::Bullet();
        	GuiUtils::hyperlink("issues/useful widgets", "https://github.com/ocornut/imgui/issues?q=label%%3A\"useful+widgets\"");

        	ImGui::Bullet();
        	GuiUtils::hyperlink("epezent/implot", "https://github.com/epezent/implot");

        	ImGui::Bullet();
        	GuiUtils::hyperlink("soufianekhiat/DearWidgets", "https://github.com/soufianekhiat/DearWidgets");

        	ImGui::Bullet();
        	GuiUtils::hyperlink("dfranx/ImFileDialog", "https://github.com/dfranx/ImFileDialog");

        	ImGui::Bullet();
        	GuiUtils::hyperlink("Toggle Button", "https://github.com/ocornut/imgui/issues/1537#issuecomment-355569554");

        	ImGui::Bullet();
        	GuiUtils::hyperlink("Knob", "https://github.com/ocornut/imgui/issues/942#issuecomment-268369298");

        	ImGui::Bullet();
        	GuiUtils::hyperlink("Nelarius/imnodes", "https://github.com/Nelarius/imnodes");

        	ImGui::Bullet();
        	GuiUtils::hyperlink("thedmd/imgui-node-editor", "https://github.com/thedmd/imgui-node-editor");

        	ImGui::Bullet();
        	GuiUtils::hyperlink("aiekick/ImGuiFontStudio", "https://github.com/aiekick/ImGuiFontStudio");

        	ImGui::Bullet();
    	    GuiUtils::hyperlink("Add extra keys #2625", "https://github.com/ocornut/imgui/pull/2625");
		}
    	
		ImGui::Spacing();

    	if (ImGui::CollapsingHeader("Frameworks", ImGuiTreeNodeFlags_DefaultOpen)) {
        	ImGui::Bullet();
        	GuiUtils::hyperlink("pthom/hello_imgui", "https://github.com/pthom/hello_imgui");

        	ImGui::Bullet();
        	GuiUtils::hyperlink("JamesBoer/ImFrame", "https://github.com/JamesBoer/ImFrame");

        	ImGui::Bullet();
        	GuiUtils::hyperlink("pr0g/sdl-bgfx-imgui-starter", "https://github.com/pr0g/sdl-bgfx-imgui-starter");

        	ImGui::Bullet();
        	GuiUtils::hyperlink("mahilab/mahi-gui", "https://github.com/mahilab/mahi-gui");
		}

		ImGui::PopStyleVar(2);
	    ImGui::End();
	}
}

void MenuItems::Help::do_about_window(bool* show) {
	if (!(*show)) return;

	ImGui::OpenPopup("About");
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5, 0.5));
    
	if (ImGui::BeginPopupModal("About", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        GuiUtils::text_centered("Map App", 0);
        ImGui::Separator();
        ImGui::Text("Version: 5a78acd (build 507)");
        ImGui::Text("Source:"); ImGui::SameLine(); GuiUtils::hyperlink("InfinityNichto/projek-akhir2", "https://github.com/InfinityNichto/projek-akhir2");
        ImGui::Text("ImGui: v1.91.5");
        ImGui::Separator();

        if (ImGui::Button("Back")) {
			ImGui::CloseCurrentPopup();
			*show = false;
		}

        ImGui::EndPopup();
    }
}

void MenuItems::Tools::do_metrics() {
    if (MenuItems::Tools::show_metrics) ImGui::ShowMetricsWindow(&MenuItems::Tools::show_metrics);
}

void MenuItems::Tools::do_stack_tool() {
    if (MenuItems::Tools::show_stack_tool) ImGui::ShowIDStackToolWindow(&MenuItems::Tools::show_stack_tool);
}

void MenuItems::Tools::do_demo_window() {
    if (MenuItems::Tools::show_demo_window) ImGui::ShowDemoWindow(&MenuItems::Tools::show_demo_window);
}

