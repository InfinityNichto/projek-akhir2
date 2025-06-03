#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <algorithm>
#include <random>
#include <sstream>
#include "imgui.h"
#include <graph.hpp>

struct Utils {
    static Graph* generate_random_graph(int vertices, int edges, int min_x, int max_x, int min_y, int max_y);
    static std::mt19937& rng();
    static int next_int(int min, int max);
    static float next_float(float min, float max);
    static bool next_bool(float percent = 0.5f);
    static std::string repeat(const std::string& str, std::size_t n);
    static float point_line_distance_squared(const ImVec2& p, const ImVec2& a, const ImVec2& b);
    static bool is_point_in_rect(const ImVec2& pt, const ImVec2& p1, const ImVec2& p2);
    static bool is_point_in_line(const ImVec2& p, const ImVec2& a, const ImVec2& b, float threshold = 1);
    static float dist(const Vertex& p1, const Vertex& p2);
    static float dist(const ImVec2& p1, const ImVec2& p2);
    static float dist(int x1, int y1, int x2, int y2);

    template<typename T> inline static bool vector_contains(std::vector<T>& vec, T& element, size_t* out_index = nullptr) {
        auto it = std::find(vec.begin(), vec.end(), element);
        if (it != vec.end()) {
            if (out_index) *out_index = std::distance(vec.begin(), it);
            return true;
        }
        return false;
    }

    template<typename T> inline static T vector_pick_random(std::vector<T>& vec) {
        if (0 > vec.size() - 1) return vec[0];
        return vec[Utils::next_int(0, vec.size() - 1)];
    }

    template <typename T> inline static std::vector<T> vector_pick_n_random(const std::vector<T>& vec, int n) {
        std::vector<T> copy = vec;
        std::random_shuffle(copy.begin(), copy.end());
        if (n > copy.size()) n = copy.size();
        return std::vector<T>(copy.begin(), copy.begin() + n);
    }

    template <typename... Args> inline static std::string build_str(Args... args) {
	    std::stringstream ss;
	    (ss << ... << args);
	    return ss.str();
    }
};

struct Rect {
    float x;
    float y;
    float w;
    float h;

    Rect(float x, float y, float w, float h);
    Rect(ImVec2 pos, float w, float h);
    Rect(float x, float y, ImVec2 size);
    Rect(ImVec2 pos, ImVec2 size);

    Rect(const Rect& other);
    Rect& operator=(const Rect& other);

    ImVec2 pos() const;
    ImVec2 size() const;
    ImVec2 top_left() const;
    ImVec2 top_right() const;
    ImVec2 bottom_left() const;
    ImVec2 bottom_right() const;
    ImVec2 mid_left() const;
    ImVec2 mid_right() const;
    ImVec2 mid_top() const;
    ImVec2 mid_bottom() const;
    ImVec2 center() const;

    std::string to_string();
};

#endif
