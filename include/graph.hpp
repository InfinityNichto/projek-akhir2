#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>

struct Edge {
    int target;
    int weight;

    Edge(int target, int weight);
    std::string to_string();
};

struct Vertex {
    int id;
    int x, y;
    std::vector<Edge*> edges;

    Vertex(int id, int x, int y);
    Edge* get_edge(int to);
    void add_edge(int to, int weight);
    void remove_edge(int to);
    void update_edge_weight(int to, int weight);
    std::string to_string();

    ~Vertex();
};

struct Graph {
    std::vector<Vertex*> vertices;

    Vertex* get_vertex(int id);
    void add_vertex(int id, int x, int y);
    void remove_vertex(int id);
    std::string to_string();

    ~Graph();
};

#endif
