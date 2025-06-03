#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <SDL_mixer.h>
#include "imgui.h"
#include <gui_vars.hpp>
#include <gui/utils.hpp>
#include <gui/workspace.hpp>

ImVec2 side_panel_size;
int vertex_id = 0;
int vertex_x = 0;
int vertex_y = 0;
int rect_padding = 8;

bool in_bounds = false;
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

Graph* bad_apple_frame;
rapidjson::Document bad_apple_data;
bool do_bad_apple = false;
bool bad_apple_initd = false;
int bad_apple_current = 0;
int bad_apple_speed = 2;
int bad_apple_weave = 50;

int rng_num_vertex = 0;
int rng_num_edge = 0;
int rng_xy_min[2] = {210, 65};
int rng_xy_max[2] = {1050, 735};

bool enable_add_vertex = true;
bool enable_dijkstra_selection = true;
bool enable_edge_dragging = false;
bool enable_deletion_mode = false;
bool highlight_dijkstra_selection = true;
bool highlight_dijkstra_path = true;
bool highlight_dijkstra_path_only = false;

void load_bad_apple_frame(int frame) {
    if (!bad_apple_initd) {
        std::ifstream ifs("/home/lisa/Downloads/dev/code/cpp/alpro/projek-akhir2/tmp/output-1000.json", std::ios::binary);
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        bad_apple_data.Parse(buffer.str().c_str());
        bad_apple_music = Mix_LoadWAV("/home/lisa/Downloads/dev/code/cpp/alpro/projek-akhir2/tmp/badapple.wav");
        Mix_PlayChannel(-1, bad_apple_music, 0);

        bad_apple_initd = true;
    }

    std::string key = std::to_string(frame);

    const rapidjson::Value& coords = bad_apple_data[key.c_str()];

    bad_apple_frame = new Graph();

    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();

    float scale_x = window_size.x / 1880;
    float scale_y = window_size.y / 1080;

    for (rapidjson::SizeType i = 0; i < coords.Size(); i++) {
        const rapidjson::Value& coord = coords[i];
        int pair[2];
        int x = coord[0].GetInt();
        int y = coord[1].GetInt();

        float scaled_x = x * scale_x + 80;
        float scaled_y = y * scale_y;
    
        pair[0] = static_cast<int>(scaled_y);
        pair[1] = static_cast<int>(scaled_x);
        bad_apple_frame->add_vertex(Utils::next_int(0, 2147483647), pair[0], pair[1]);
    }

    std::vector<Vertex*> top, bottom;

    for (Vertex* v : bad_apple_frame->vertices) {
        ((v->x + v->y) % 2 == 0 ? top : bottom).push_back(v);
    }

    std::sort(top.begin(), top.end(), [](Vertex* a, Vertex* b) { return a->x < b->x; });
    std::sort(bottom.begin(), bottom.end(), [](Vertex* a, Vertex* b) { return a->x < b->x; });

    size_t len = std::min(top.size(), bottom.size());
    for (size_t i = 0; i < len; ++i) {
        top[i]->add_edge(bottom[i]->id, Utils::dist(*top[i], *bottom[i]));
        if (i > 0) {
            top[i - 1]->add_edge(top[i]->id, Utils::dist(*top[i - 1], *top[i]));
            bottom[i - 1]->add_edge(bottom[i]->id, Utils::dist(*bottom[i - 1], *bottom[i]));
        }
    }
}

// fungsi ini gunanya adalah untuk menggambar graph yang kita miliki ke area input
void draw_full_graph() {
    for (Vertex* v : current_graph->vertices) { // untuk semua vertex yang ada didalam graph kita...
        // siapkan 2 titik untuk menggambar sebuah kotak disekitar posisi vertex ini
        ImVec2 p1 = ImVec2(v->x - rect_padding, v->y - rect_padding); // titik kiri bawah
        ImVec2 p2 = ImVec2(v->x + rect_padding, v->y + rect_padding); // titik kanan atas
        ImU32 color = IM_COL32(252, 132, 232, 255); // warna untuk vertex tersebut

        // ini fungsinya untuk highlight vertex yang sedang dihover
        // jika mouse didalam area input dan mouse sedang diatas suatu vertex, maka...
        if (in_bounds && Utils::is_point_in_rect(GuiVars::mouse_pos, p1, p2)) {
            color = IM_COL32(250, 55, 182, 255); // ubah warnanya menjadi lebih terang (untuk meng-highlight)
            is_hovering_vertex = true; // set bahwa kita sedang meng-hover suatu vertex
            hovered_vertex = v;  // dan set vertex yang sedang dihover tersebut dari "v" (vertex sekarang)
        }

        // jika fitur highlight dijkstra selection aktif dan vertex sekarang adalah sebuah vertex pilihan dijkstra, maka...
        if (highlight_dijkstra_selection && (v == dijkstra_first_vertex || v == dijkstra_second_vertex)) {
            color = IM_COL32(255, 0, 94, 255); // ubah warnanya ke merah agar kelihatan
        }

        // ini fungsinya untuk highlight vertex-vertex dari path dijkstra yang telah dikomputasi (jika ada)
        size_t path_index = 0; // indeks setiap vertex didalam path tersebut
        // jika fitur highlight dijkstra path aktif dan vertex sekarang ada didalam path dijkstra, maka...
        if (highlight_dijkstra_path && Utils::vector_contains(dijsktra_computed_path, v, &path_index)) {
            color = IM_COL32(255, 0, 94, 255); // ubah warnanya ke merah agar kelihatan
            ImGui::SetCursorScreenPos(p2); ImGui::Text("%zu", path_index); // dan tulis indeks (urutan) si vertex tersebut didalam path
        }

        // if (do_bad_apple) {
        //     p1.x += rect_padding * 0.5, p1.y += rect_padding * 0.5;
        //     p2.x -= rect_padding * 0.5, p2.y -= rect_padding * 0.5;
        //     color = IM_COL32(255, 255, 255, 255);
        // }

        // akhirnya gambar vertex ini sebagai sebuah kotak
        ImGui::GetWindowDrawList()->AddRectFilled(p1, p2, color);

        for (Edge* e : v->edges) { // untuk semua edge yang ada didalam vertex sekarang...
            Vertex* v_to = current_graph->get_vertex(e->target); // dapatkan vertex tujuan dari edge sekarang

            // siapkan 2 titik untuk menggambar sebuah garis untuk edge ini
            ImVec2 from_p1 = ImVec2(v->x, v->y); // titik awal yang terletak di vertex awal (v)
            ImVec2 to_p2 = ImVec2(v_to->x, v_to->y); // titik akhir yang terletak di vertex tujuan (v_to)
            ImU32 to_color = IM_COL32(132, 232, 172, 255); // warna untuk edge tersebut
            int thickness = 1; // ketebalan garisnya

            // ini fungsinya untuk highlight edge yang sedang dihover
            // jika mouse didalam area input dan mouse sedang diatas suatu edge, maka...
            if (in_bounds && Utils::is_point_in_line(GuiVars::mouse_pos, from_p1, to_p2, thickness)) {
                to_color = IM_COL32(59, 227, 126, 255); // ubah warnanya menjadi lebih terang agar kelihatan
                is_hovering_edge = true; // set bahwa kita sedang meng-hover suatu edge
                hovered_edge = e; // dan set edge yang sedang dihover tersebut dari "e" (edge sekarang)
            }

            // jika fitur highlight dijkstra path aktif, maka...
            if (highlight_dijkstra_path) {
                // iterasi dari 0 sampai banyaknya vertex didalam path dijkstra
                for (size_t i = 0; i + 1 < dijsktra_computed_path.size(); i++) {
                    // jika vertex dijkstra sekarang sama dengan vertex kita, dan vertex dijkstra selanjutnya
                    // juga sama dengan vertex tujuan dari edge sekarang, maka...
                    if (dijsktra_computed_path[i] == v && dijsktra_computed_path[i + 1] == v_to) {
                        // bisa disimpulkan kita sedang berada di edge yang ada didalam path dijkstra
                        to_color = IM_COL32(255, 0, 94, 255); // ubah warnanya menjadi merah agar kelihatan
                        break; // selesai
                    }
                }
            }

            // if (do_bad_apple) {
            //     to_color = IM_COL32(255, 255, 255, 44);
            //     thickness = 1;
            // }

            // akhirnya gambar edge ini sebagai sebuah "arrow" (anak panah)
            GuiUtils::arrow(from_p1, to_p2, to_color, thickness, std::to_string(e->weight).c_str());
        }
    }
}

void draw_dijkstra_path() {
    for (size_t i = 0; i < dijsktra_computed_path.size(); i++) {
        Vertex* v = dijsktra_computed_path[i];
        ImVec2 p1 = ImVec2(v->x - rect_padding, v->y - rect_padding);
        ImVec2 p2 = ImVec2(v->x + rect_padding, v->y + rect_padding);
        ImU32 color = IM_COL32(255, 0, 94, 255);
        ImGui::SetCursorScreenPos(p2); ImGui::Text("%zu", i);
        ImGui::GetWindowDrawList()->AddRectFilled(p1, p2, color);

        if (i + 1 < dijsktra_computed_path.size()) {
            Vertex* v_to = dijsktra_computed_path[i + 1];
            ImVec2 from_p1 = ImVec2(v->x, v->y);
            ImVec2 to_p2 = ImVec2(v_to->x, v_to->y);
            ImU32 to_color = IM_COL32(255, 0, 94, 255);

            Edge* edge = v->get_edge(v_to->id);
            if (edge) GuiUtils::arrow(from_p1, to_p2, to_color, 2, std::to_string(edge->weight).c_str());
        }
    }
}

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
    
    GuiUtils::separator_heading("Random generator");
    ImGui::InputInt("Num vertices: ", &rng_num_vertex);
    ImGui::InputInt("Num edges: ", &rng_num_edge);
    ImGui::InputInt2("Min position: ", rng_xy_min);
    ImGui::InputInt2("Max position: ", rng_xy_max);
    if (ImGui::Button("Generate")) {
        current_graph = Utils::generate_random_graph(rng_num_vertex, rng_num_edge, rng_xy_min[0], rng_xy_max[0], rng_xy_min[1], rng_xy_max[1]);
        dijsktra_computed_path = { };
    }

    GuiUtils::separator_heading("Bad apple");
    ImGui::InputInt("Speed", &bad_apple_speed);
    ImGui::InputInt("Weave", &bad_apple_weave);
    if (ImGui::Button("Bad apple")) {
        do_bad_apple = !do_bad_apple;
    }
    ImGui::SameLine(); ImGui::Text("%d", bad_apple_current);

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

    in_bounds = GuiVars::mouse_pos.x >= window_pos.x &&
        GuiVars::mouse_pos.y >= window_pos.y &&
        GuiVars::mouse_pos.x <= window_pos.x + window_size.x &&
        GuiVars::mouse_pos.y <= window_pos.y + window_size.y;

    if (do_bad_apple && GuiVars::global_tick % bad_apple_speed == 0) {
        load_bad_apple_frame(bad_apple_current);
        current_graph = bad_apple_frame;

        bad_apple_current++;
        if (bad_apple_current == 6585) {
            bad_apple_current = 0;
            do_bad_apple = false;
        }
    }

    if (!highlight_dijkstra_path_only) {
        draw_full_graph();
    }

    if (highlight_dijkstra_path || highlight_dijkstra_path_only) {
        draw_dijkstra_path();
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

    // !! bagian input utama !! //
    // in_bounds = mouse didalam area input
    // left_click = tombol mouse diklik
    // left_down = tombol mouse diklik dan ditahan
    // is_hovering_vertex = kursor sedang diatas suatu vertex
    // is_dragging_edge = sedang menggeser suatu edge dalam sebuah sekuen input edge
    // enable_add_vertex = fitur untuk menambahkan vertex
    // enable_edge_dragging = fitur untuk menambahkan edge dengan cara menggeser ("dragging")

    // jika mouse didalam area input, mouse diklik, tidak sedang menghover suatu vertex, dan fitur add vertex aktif, maka...
    else if (in_bounds && left_click && !is_hovering_vertex && enable_add_vertex) {
        // buat sebuah vertex baru dengan ID random dan koordinat (x, y) sebagai koordinat mouse sekarang
        current_graph->add_vertex(Utils::next_int(0, 2147483647), x, y);
    }

    // jika mouse didalam area input, mouse diklik, sedang menghover suatu vertex, dan fitur dijkstra selection aktif, maka...
    else if (in_bounds && left_click && is_hovering_vertex && enable_dijkstra_selection) {
        // jika sedang memilih vertex pertama (awal) untuk dijkstra, maka...
        if (selecting_first_vertex_for_dijkstra) {
            dijkstra_first_vertex = hovered_vertex; // set vertex pertama dijkstra dengan vertex yang sedang dihover oleh kursor
            selecting_first_vertex_for_dijkstra = false; // selesaikan pemilihan vertex pertama dengan set ini ke false
        }
        // sama seperti yang diatas, tetapi untuk vertex kedua dijkstra (tujuan)
        else if (selecting_second_vertex_for_dijkstra) {
            dijkstra_second_vertex = hovered_vertex;
            selecting_second_vertex_for_dijkstra = false;
        }
    }

    // jika mouse didalam area input, mouse diklik dan ditahan, fitur edge dragging aktif, dan:
    // sedang menghover suatu vertex atau memang sedang menggeser edge baru, maka...
    else if (in_bounds && left_down && enable_edge_dragging && (is_hovering_vertex || is_dragging_edge)) {
        is_dragging_edge = true; // set ini menjadi true

        // kalau kita menggeser suatu edge keluar dari vertex awal, otomatis kita sudah tidak lagi menghover suatu vertex
        // oleh karena itu dilakukan sebuah check kalau mouse menghover sebuah vertex baru
        if (is_hovering_vertex) { // jika iya, maka...
            // jika vertex tujuan (hovered) dan awal (edged) valid, dan kedua vertex itu tidak sama, maka...
            if (hovered_vertex && edged_vertex && hovered_vertex != edged_vertex) {
                // buat sebuah edge baru diantara kedua vertex tersebut, dengan bobot adalah jaraknya
                edged_vertex->add_edge(hovered_vertex->id, Utils::dist(*edged_vertex, *hovered_vertex));
            }
            // update edged karena kita telah menemukan vertex baru (hovered)
            edged_vertex = hovered_vertex;
        }

        // akhirnya gambar edge yang lagi digeser ini sebagai "arrow" (anak panah)
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
	ImGui::Text("Mouse position: %d, %d", (int)GuiVars::mouse_pos.x, (int)GuiVars::mouse_pos.y);
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

