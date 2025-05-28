#include "imgui.h"
#include <gui_vars.hpp>
#include <gui/utils.hpp>
#include <gui/workspace.hpp>

ImVec2 side_panel_size;
int vertex_id = 0;
int vertex_x = 0;
int vertex_y = 0;
int rect_padding = 8;

bool left_click = false;
bool left_down = false;
bool is_hovering_vertex = false;
bool is_hovering_edge = false;
bool is_dragging_edge = false;
Vertex* hovered_vertex = nullptr;
Vertex* edged_vertex = nullptr;
Edge* hovered_edge = nullptr;
bool selecting_first_vertex_for_dijkstra = false;
bool selecting_second_vertex_for_dijkstra = false;
Vertex* dijkstra_first_vertex = nullptr;
Vertex* dijkstra_second_vertex = nullptr;
std::vector<Vertex*> dijsktra_computed_path = { };

bool enable_add_vertex = true;
bool enable_dijkstra_selection = true;
bool enable_edge_dragging = false;
bool enable_deletion_mode = false;
bool highlight_dijkstra_selection = true;
bool highlight_dijkstra_path = true;
bool highlight_dijkstra_path_only = false;

void Workspace::draw_the_whole_thing() {
    do_side_panel();
    do_display_panel();
    draw_imgui_stats();
    do_info_panel();
}

void Workspace::do_side_panel() {
    ImVec2 display = GuiVars::display_size();

    ImGui::SetNextWindowPos(ImVec2(0, GuiVars::prev_window_data.h));
    ImGui::SetNextWindowSize(ImVec2(display.x * 0.15625f, display.y - GuiVars::prev_window_data.h));
    ImGui::SetNextWindowBgAlpha(1.0f);

    ImGuiWindowFlags window_flags = GuiVars::common_window_flags;
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.16f, 0.16f, 0.16f, 0.5f));

    ImGui::Begin("Side bar", NULL, window_flags);

    GuiUtils::separator_heading("Controls");
    ImGui::Checkbox("Enable add vertex", &enable_add_vertex);
    ImGui::Checkbox("Enable dijkstra selection", &enable_dijkstra_selection);
    ImGui::Checkbox("Enable edge dragging", &enable_edge_dragging);
    ImGui::Checkbox("Enable deletion mode", &enable_deletion_mode);
    ImGui::Checkbox("Highlight dijkstra selection", &highlight_dijkstra_selection);
    ImGui::Checkbox("Highlight dijkstra path", &highlight_dijkstra_path);
    ImGui::Checkbox("Highlight dijkstra path only", &highlight_dijkstra_path_only);

    GuiUtils::separator_heading("Dijkstra");
    const char* dijkstra_first_btn = !selecting_first_vertex_for_dijkstra ? "Select first vertex" : "Selecting...##dijkstra1";
    const char* dijkstra_second_btn = !selecting_second_vertex_for_dijkstra ? "Select second vertex" : "Selecting...##dijkstra2";

    ImGui::BeginDisabled(selecting_first_vertex_for_dijkstra);
    if (ImGui::Button(dijkstra_first_btn)) {
        selecting_first_vertex_for_dijkstra = true;
    }
    ImGui::EndDisabled();

    std::string first_id_str = dijkstra_first_vertex ? std::to_string(dijkstra_first_vertex->id) : "";
    ImGui::SameLine(); ImGui::Text("%s", first_id_str.c_str());

    ImGui::BeginDisabled(selecting_second_vertex_for_dijkstra);
    if (ImGui::Button(dijkstra_second_btn)) {
        selecting_second_vertex_for_dijkstra = true;
    }
    ImGui::EndDisabled();

    std::string second_id_str = dijkstra_second_vertex ? std::to_string(dijkstra_second_vertex->id) : "";
    ImGui::SameLine(); ImGui::Text("%s", second_id_str.c_str());

    bool disable_compute =
        selecting_first_vertex_for_dijkstra ||
        selecting_second_vertex_for_dijkstra ||
        dijkstra_first_vertex == nullptr ||
        dijkstra_second_vertex == nullptr;

    ImGui::BeginDisabled(disable_compute);
    if (ImGui::Button("Compute")) {
        dijsktra_computed_path = current_graph->dijkstra(dijkstra_first_vertex, dijkstra_second_vertex);
    }
    ImGui::EndDisabled();

    GuiUtils::separator_heading("Graph summary");
    GuiUtils::text_wrapped("%s", current_graph->to_string_detailed().c_str());

    ImGui::PopStyleColor(1);
    GuiVars::update_prev_window_data();
    ImGui::End();
}

void Workspace::do_display_panel() {
    ImVec2 display = GuiVars::display_size();
    ImVec2 next_pos = ImVec2(GuiVars::prev_window_data.w, GuiVars::prev_window_data.y);
    ImVec2 next_size = ImVec2(display.x * 0.625, GuiVars::prev_window_data.h);

    ImGui::SetNextWindowPos(next_pos);
    ImGui::SetNextWindowSize(next_size);
    ImGui::SetNextWindowBgAlpha(1);

    ImGuiWindowFlags window_flags = GuiVars::common_window_flags & ~(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.11, 0.11, 0.11, 0.25));

    ImGui::Begin("Item display", NULL, window_flags);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    GuiUtils::draw_grid(25, 1);
    draw_imgui_stats();

    int x = GuiVars::mouse_pos.x, y = GuiVars::mouse_pos.y;
    left_click = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    left_down = ImGui::IsMouseDown(ImGuiMouseButton_Left);

    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();
    ImVec2 local_mouse = ImVec2(GuiVars::mouse_pos.x - window_pos.x, GuiVars::mouse_pos.y - window_pos.y);

    bool in_bounds = GuiVars::mouse_pos.x >= window_pos.x &&
        GuiVars::mouse_pos.y >= window_pos.y &&
        GuiVars::mouse_pos.x <= window_pos.x + window_size.x &&
        GuiVars::mouse_pos.y <= window_pos.y + window_size.y;

    for (Vertex* v : current_graph->vertices) {
        ImVec2 p1 = ImVec2(v->x - rect_padding, v->y - rect_padding);
        ImVec2 p2 = ImVec2(v->x + rect_padding, v->y + rect_padding);
        ImU32 color = IM_COL32(252, 132, 232, 255);

        if (in_bounds && Utils::is_point_in_rect(GuiVars::mouse_pos, p1, p2)) {
            color = IM_COL32(250, 55, 182, 255);
            is_hovering_vertex = true;
            hovered_vertex = v;
        }

        if (highlight_dijkstra_selection && (v == dijkstra_first_vertex || v == dijkstra_second_vertex)) {
            color = IM_COL32(255, 0, 94, 255);
        }

        size_t path_index = 0;
        if (highlight_dijkstra_path && Utils::vector_contains(dijsktra_computed_path, v, &path_index)) {
            color = IM_COL32(192, 134, 235, 255);
            ImGui::SetCursorScreenPos(p2); ImGui::Text("%zu", path_index);
        } else if (highlight_dijkstra_path_only) break;

        ImGui::GetWindowDrawList()->AddRectFilled(p1, p2, color);

        for (Edge* e : v->edges) {
            Vertex* v_to = current_graph->get_vertex(e->target);
            ImVec2 from_p1 = ImVec2(v->x, v->y);
            ImVec2 to_p2 = ImVec2(v_to->x, v_to->y);
            ImU32 to_color = IM_COL32(132, 232, 172, 255);

            if (in_bounds && Utils::is_point_in_line(GuiVars::mouse_pos, from_p1, to_p2, 1)) {
                to_color = IM_COL32(59, 227, 126, 255);
                is_hovering_edge = true;
                hovered_edge = e;
            }

            if (highlight_dijkstra_path) {
                for (size_t i = 0; i + 1 < dijsktra_computed_path.size(); i++) {
                    if (dijsktra_computed_path[i] == v && dijsktra_computed_path[i + 1] == v_to) {
                        to_color = IM_COL32(192, 134, 235, 255);
                        break;
                    }
                }
            }

            GuiUtils::arrow(from_p1, to_p2, to_color, 2, std::to_string(e->weight).c_str());
        }
    }

    if (in_bounds && left_click && enable_deletion_mode) {
        if (hovered_vertex != nullptr) {
            current_graph->remove_vertex(hovered_vertex->id);
            hovered_vertex = nullptr;
        } else if (hovered_edge != nullptr) {
            current_graph->remove_edge(hovered_edge);
            hovered_edge = nullptr;
        }
    }
    else if (in_bounds && left_click && !is_hovering_vertex && enable_add_vertex) {
        current_graph->add_vertex(Utils::next_int(0, 2147483647), x, y);
    } else if (in_bounds && left_click && is_hovering_vertex && enable_dijkstra_selection) {
        if (selecting_first_vertex_for_dijkstra) {
            dijkstra_first_vertex = hovered_vertex;
            selecting_first_vertex_for_dijkstra = false;
        } else if (selecting_second_vertex_for_dijkstra) {
            dijkstra_second_vertex = hovered_vertex;
            selecting_second_vertex_for_dijkstra = false;
        }
    } else if (in_bounds && left_down && enable_edge_dragging && (is_hovering_vertex || is_dragging_edge)) {
        is_dragging_edge = true;
        if (is_hovering_vertex) {
            if (hovered_vertex && edged_vertex && hovered_vertex != edged_vertex) {
                edged_vertex->add_edge(hovered_vertex->id, Utils::dist(*edged_vertex, *hovered_vertex));
            }
            edged_vertex = hovered_vertex;
        }

        GuiUtils::arrow(ImVec2(edged_vertex->x, edged_vertex->y), GuiVars::mouse_pos, IM_COL32(232, 250, 172, 255), 2);
    }

    ImGui::PopStyleColor(1);
    ImGui::PopStyleVar(2);
    side_panel_size = GuiVars::prev_window_data.size();
    GuiVars::update_prev_window_data();
    ImGui::End();
}

void Workspace::draw_imgui_stats() {
	ImGui::SetCursorPosY(GuiVars::prev_window_data.h - 57);
	ImGui::BeginDisabled();
	ImGui::Text("Mouse position: %f, %f", GuiVars::mouse_pos.x, GuiVars::mouse_pos.y);
	ImGui::Text("Performance: %.2f FPS", GuiVars::fps);
    ImGui::Text("Tick: %ld/%ld ticks", GuiVars::global_tick, GuiVars::global_tick % (int)GuiVars::fps);
	ImGui::EndDisabled();
}

void Workspace::do_info_panel() {
	ImGui::SetNextWindowPos(ImVec2(GuiVars::prev_window_data.x + GuiVars::prev_window_data.w, GuiVars::prev_window_data.y));
	ImGui::SetNextWindowSize(ImVec2(GuiVars::display_size().x * 0.21875, side_panel_size.y));
	ImGui::SetNextWindowBgAlpha(1);

	ImGuiWindowFlags window_flags = GuiVars::common_window_flags & ~(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.13f, 0.13f, 0.13f, 0.25f));

	ImGui::Begin("Properties", NULL, window_flags);

    GuiUtils::separator_heading("Hovered items");

    GuiUtils::text_wrapped("Vertex: %s", hovered_vertex ? hovered_vertex->to_string().c_str() : "nullptr");
    if (is_hovering_vertex) {
        is_hovering_vertex = false;
        hovered_vertex = nullptr;
    }

    GuiUtils::text_wrapped("Edge: %s", hovered_edge ? hovered_edge->to_string().c_str() : "nullptr");
    if (is_hovering_edge) {
        is_hovering_edge = false;
        hovered_edge = nullptr;
    }

    GuiUtils::separator_heading("Dijkstra path");
    if (!dijsktra_computed_path.empty()) {
        GuiUtils::text_wrapped("Start: %s", dijkstra_first_vertex->to_string().c_str());
        GuiUtils::text_wrapped("End: %s", dijkstra_second_vertex->to_string().c_str());

        ImGui::Text("Path:");
        int i = 0;
        for (Vertex* v : dijsktra_computed_path) {
            GuiUtils::text_wrapped("%d: %s", i, v->to_string().c_str());
            i++;
        }
    }

	ImGui::PopStyleColor(1);
	ImGui::End();
}

