#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <algorithm>
#include <random>
#include "graph.hpp"

struct Utils {
    static Graph* generate_random_graph(int vertices, int edges, int min_x, int max_x, int min_y, int max_y, int min_weight, int max_weight);
    static std::mt19937& rng();
    static int next_int(int min, int max);
    static float next_float(float min, float max);
    static bool next_bool(float percent = 0.5f);
    static std::string repeat(const std::string& str, std::size_t n);

    template<typename T>
    static bool vector_contains(std::vector<T>& vec, T& element) {
        return std::find(vec.begin(), vec.end(), element) != vec.end(); 
    }
    
    template<typename T>
    static T vector_pick_random(std::vector<T>& vec) {
        return vec[Utils::next_int(0, vec.size() - 1)];
    }
};

#endif
