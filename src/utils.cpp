#include <vector>
#include <random>
#include <graph.hpp>
#include <utils.hpp>

Graph* Utils::generate_random_graph(int num_vertices, int num_edges, int min_x, int max_x, int min_y, int max_y) {
    std::vector<int> built_vertices;
    std::vector<Vertex*> vertex_list;
    Graph* graph = new Graph();

    while ((int)built_vertices.size() < num_vertices) {
        int id = next_int(0, 2147483647);
        if (!Utils::vector_contains(built_vertices, id)) {
            built_vertices.push_back(id);
            int x = next_int(min_x, max_x);
            int y = next_int(min_y, max_y);
            Vertex* v = new Vertex(id, x, y);
            vertex_list.push_back(v);
            graph->add_vertex(*v);
        }
    }

    for (int i = 0; i < num_vertices - 1; i++) {
        Vertex* from = vertex_list[i];
        Vertex* to = vertex_list[i + 1];

        int dx = from->x - to->x;
        int dy = from->y - to->y;
        int dist = (int)(std::sqrt(dx * dx + dy * dy));

        from->add_edge(to->id, dist);
    }

    for (Vertex* from : vertex_list) {
        int max_additional = num_edges - (int)from->edges.size();
        int edges_to_add = next_int(0, max_additional);

        for (int j = 0; j < edges_to_add; j++) {
            int target_id = vector_pick_random(built_vertices);

            if (target_id == from->id || from->get_edge(target_id)) continue;

            Vertex* target_vertex = graph->get_vertex(target_id);
            if (!target_vertex) continue;

            int dx = from->x - target_vertex->x;
            int dy = from->y - target_vertex->y;
            int dist = (int)(std::sqrt(dx * dx + dy * dy));

            from->add_edge(target_id, dist);
        }
    }

    return graph;
}

std::string Utils::repeat(const std::string& str, std::size_t n) {
    std::string result;
    result.reserve(str.size() * n);

    for (std::size_t i = 0; i < n; i++) {
        result += str;
    }

    return result;
}

std::mt19937& Utils::rng() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

int Utils::next_int(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng());
}

float Utils::next_float(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng());
}

bool Utils::next_bool(float percent) {
    std::bernoulli_distribution dist(percent);
    return dist(rng());
}

float Utils::point_line_distance_squared(const ImVec2& p, const ImVec2& a, const ImVec2& b) {
    ImVec2 ab = ImVec2(b.x - a.x, b.y - a.y);
    ImVec2 ap = ImVec2(p.x - a.x, p.y - a.y);

    float ab_len_sq = ab.x * ab.x + ab.y * ab.y;
    if (ab_len_sq == 0) return (p.x - a.x) * (p.x - a.x) + (p.y - a.y) * (p.y - a.y);

    float t = (ap.x * ab.x + ap.y * ab.y) / ab_len_sq;
    t = std::max(0.0f, std::min(1.0f, t));

    ImVec2 projection = ImVec2(a.x + t * ab.x, a.y + t * ab.y);
    float dx = p.x - projection.x;
    float dy = p.y - projection.y;
    return dx * dx + dy * dy;
}

bool Utils::is_point_in_rect(const ImVec2& pt, const ImVec2& p1, const ImVec2& p2) {
    return pt.x >= p1.x && pt.x <= p2.x && pt.y >= p1.y && pt.y <= p2.y;
}

bool Utils::is_point_in_line(const ImVec2& p, const ImVec2& a, const ImVec2& b, float threshold) {
    return Utils::point_line_distance_squared(p, a, b) < (threshold * threshold);
}

float Utils::dist(const Vertex& p1, const Vertex& p2) {
    return sqrtf(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

float Utils::dist(const ImVec2& p1, const ImVec2& p2) {
    return sqrtf(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

float Utils::dist(int x1, int y1, int x2, int y2) {
    return sqrtf(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

Rect::Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
Rect::Rect(ImVec2 pos, float w, float h) : x(pos.x), y(pos.y), w(w), h(h) {}
Rect::Rect(float x, float y, ImVec2 size) : x(x), y(y), w(size.x), h(size.y) {}
Rect::Rect(ImVec2 pos, ImVec2 size) : x(pos.x), y(pos.y), w(size.x), h(size.y) {}

Rect::Rect(const Rect& other) = default;
Rect& Rect::operator=(const Rect& other) = default;

ImVec2 Rect::pos() const { return ImVec2(x, y); }
ImVec2 Rect::size() const { return ImVec2(w, h); }
ImVec2 Rect::top_left() const { return ImVec2(x, y); }
ImVec2 Rect::top_right() const { return ImVec2(x + w, y); }
ImVec2 Rect::bottom_left() const { return ImVec2(x, y + h); }
ImVec2 Rect::bottom_right() const { return ImVec2(x + w, y + h); }
ImVec2 Rect::mid_left() const { return ImVec2(x, h * 0.5 + y); }
ImVec2 Rect::mid_right() const { return ImVec2(x + w, h * 0.5 + y); }
ImVec2 Rect::mid_top() const { return ImVec2(w * 0.5 + x, y); }
ImVec2 Rect::mid_bottom() const { return ImVec2(w * 0.5 + x, y + h); }
ImVec2 Rect::center() const { return ImVec2(w * 0.5 + x, h * 0.5 + y); }

std::string Rect::to_string() {
    return Utils::build_str("[Rect: ", x, ", ", y, ", ", w, ", ", h, "]");
}

