#include <stdio.h>
#include <graph.hpp>

int main() {
    Graph* map = new Graph();
    map->add_vertex(999, 0, 0);

    printf("%s, %s\n", map->to_string().c_str(), map->get_vertex(999)->to_string().c_str());
}
