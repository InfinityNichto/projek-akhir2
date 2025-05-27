#include <vector>
#include <random>
#include <graph.hpp>
#include <utils.hpp>

Graph Utils::generate_random_graph(int num_vertices, int num_edges, int min_x, int max_x, int min_y, int max_y, int min_weight, int max_weight) {
    std::vector<int> built_vertices = { };
    int current_vertex = next_int(-2147483648, 2147483647);

    for (int i = 0; i < num_vertices; i++) {
        while (Utils::vector_contains(built_vertices, current_vertex)) {
            current_vertex = next_int(-2147483648, 2147483647);
        }

        Vertex* new_vertex = new Vertex(current_vertex, next_int(min_x, max_x), next_int(min_y, max_y));

        for (int j = 0; j < num_edges; j++) {
            Edge* new_edge = new Edge(Utils::vector_pick_random(built_vertices, next_int(min_weight, max_weight));
        }
}

std::mt19937& Utils::rng() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

int Utils::next_int(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(this->rng());
}

float Utils::next_float(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(this->rng());
}

bool Utils::next_bool(float percent) {
    std::bernoulli_distribution dist(percent);
    return dist(this->rng());
}


