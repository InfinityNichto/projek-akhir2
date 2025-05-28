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
    void add_edge(Edge& edge);
    void remove_edge(int to);
    void update_edge_weight(int to, int weight);
    std::string to_string();
    std::string to_string_detailed();

    ~Vertex();
};

struct Graph {
    std::vector<Vertex*> vertices;

    Vertex* get_vertex(int id);
    void add_vertex(int id, int x, int y);
    void add_vertex(Vertex& vertex);
    void remove_vertex(int id);
    void remove_edge(Edge* edge);
    std::vector<int> dijkstra(int start, int end);
    std::vector<Vertex*> dijkstra(Vertex* start, Vertex* end);
    std::string to_string();
    std::string to_string_detailed();

    ~Graph();
};

#endif
