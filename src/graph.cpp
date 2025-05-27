#include <stdio.h>
#include <stdint.h>
#include <algorithm>
#include <sstream>
#include <graph.hpp>

Edge::Edge(int target, int weight) : target(target), weight(weight) { }

std::string Edge::to_string() {
    std::ostringstream oss;
    oss << "[Edge: (target=" << this->target << ", weight=" << this->weight << ")]";
    return oss.str();
}

Vertex::Vertex(int id, int x, int y) : id(id), x(x), y(y) { }

Vertex::~Vertex() {
    for (Edge* e : this->edges) {
        delete e;
    }
}

Edge* Vertex::get_edge(int to) {
    for (Edge* e : this->edges) {
        if (e->target == to) return e;
    }

    fprintf(stderr, "get_edge: edge connecting vertex %d with %d is not found...", this->id, to);
    return nullptr;
}

void Vertex::add_edge(int to, int weight) {
    this->edges.push_back(new Edge(to, weight));
}

void Vertex::add_edge(Edge& edge) {
    this->edges.push_back(&edge);
}

void Vertex::remove_edge(int to) {
    Edge* toRemove = this->get_edge(to);

    std::vector<Edge*>& es = this->edges;
    es.erase(std::remove(es.begin(), es.end(), toRemove), es.end());
    delete toRemove;
}

void Vertex::update_edge_weight(int to, int weight) {
    this->get_edge(to)->weight = weight;
}

std::string Vertex::to_string() {
    std::ostringstream oss;
    oss << "[Vertex: (id=" << this->id << ", x=" << this->x << ", y=" << this->y << ", edges=" << this->edges.size() << ")]";
    return oss.str();
}

std::string Vertex::to_string_detailed() {
    std::ostringstream oss;
    oss << "[Vertex: (id=" << this->id << ", x=" << this->x << ", y=" << this->y << ", edges=" << this->edges.size() << ")]\n";
    for (Edge* e : this->edges) {
        oss << "-> " << e->to_string() << "\n";
    }
    return oss.str();
}

Graph::~Graph() {
    for (Vertex* v : this->vertices) {
        delete v;
    }
}

Vertex* Graph::get_vertex(int id) {
    for (Vertex* v : this->vertices) {
        if (v->id == id) return v;
    }

    fprintf(stderr, "get_vertex: vertex with id %d not found...", id);
    return nullptr;
}

void Graph::add_vertex(int id, int x, int y) {
    this->vertices.push_back(new Vertex(id, x, y));
}

void Graph::add_vertex(Vertex& vertex) {
    this->vertices.push_back(&vertex);
}

void Graph::remove_vertex(int id) {
    Vertex* toRemove = this->get_vertex(id);
    std::vector<Edge*> copy = toRemove->edges;

    for (Edge* e : copy) {
        toRemove->remove_edge(e->target);
    }

    for (Vertex* v : this->vertices) {
        std::vector<Edge*> edgesCopy = v->edges;

        for (Edge* e : edgesCopy) {
            if (e->target == id) v->remove_edge(id);
        }
    }

    vertices.erase(std::remove(vertices.begin(), vertices.end(), toRemove), vertices.end());
    delete toRemove;
}

std::string Graph::to_string() {
    std::ostringstream oss;
    oss << "[Graph: (vertices=" << this->vertices.size() << ")]";
    return oss.str();
}

std::string Graph::to_string_detailed() {
    std::ostringstream oss;
    oss << "[Graph: (vertices=" << this->vertices.size() << ")]";
    for (Vertex* v : this->vertices) {
        oss << "-> " << v->to_string_detailed();
    }
    return oss.str();
}

