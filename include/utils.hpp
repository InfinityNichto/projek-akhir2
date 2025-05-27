#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <algorithm>
#include <random>
#include "graph.hpp"

struct Utils {
    Graph generate_random_graph(int vertices, int edges, int min_x, int min_y, int min_weight, int max_weight);
    std::mt19937& rng();
    int next_int(int min, int max);
    float next_float(float min, float max);
    bool next_bool(float percent = 0.5f);

    template<typename T>
    bool vector_contains(std::vector<T>& vec, T& element) {
        return std::find(vec.begin(), vec.end(), element) != vec.end(); 
    }
    
    template<typename T>
    T vector_pick_random(std::vector<T>& vec) {
        return vec[this->next_int(0, vec.size())];
    }
};

#endif
