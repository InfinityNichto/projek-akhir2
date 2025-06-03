#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>

// deklarasi struct Edge
struct Edge {
    int target; // ID vertex target yang sedang ditunjuk oleh edge ini
    int weight; // bobot edge ini

    Edge(int target, int weight); // konstuktor
    std::string to_string(); // fungsi untuk mengubah edge -> string/teks
};

// deklarasi struct Vertex
struct Vertex {
    int id; // ID vertex ini
    int x, y;  // posisi vertex ini
    std::vector<Edge*> edges; // vertex ini mengelola sebuah list edge yang mengarah keluar dari vertex ini

    Vertex(int id, int x, int y); // konstruktor
    ~Vertex(); // destruktor
    Edge* get_edge(int to); // fungsi untuk mendapatkan suatu edge tertentu dari vertex ini
    void add_edge(int to, int weight); // fungsi untuk menambahkan edge baru ke vertex ini
    void add_edge(Edge& edge); // sama tetapi fungsi ini menerima sebuah Edge secara langsung
    void remove_edge(int to); // fungsi untuk menghapus suatu edge dari vertex ini
    void update_edge_weight(int to, int weight); // fungsi untuk mengupdate bobot suatu edge dari vertex ini
    std::string to_string(); // fungsi untuk mengubah vertex -> string/teks
    std::string to_string_detailed(); // sama tetapi lebih mendetail
};

// deklarasi struct Graph
struct Graph {
    std::vector<Vertex*> vertices; // graph ini mengelola sebuah list vertex yang terdapat didalam graph tersebut

    ~Graph(); // destruktor
    Vertex* get_vertex(int id); // fungsi untuk mendapatkan suatu vertex tertentu dari graph ini
    void add_vertex(int id, int x, int y); // fungsi untuk menambahkan vertex baru ke graph ini
    void add_vertex(Vertex& vertex); // sama tetapi fungsi ini menerima sebuah Vertex secara langsung
    void remove_vertex(int id); // fungsi untuk menghapus suatu vertex dari graph ini
    void remove_edge(Edge* edge); // fungsi untuk menghapus suatu edge dari graph ini
    std::vector<int> dijkstra(int start, int end); // fungsi yang melakukan algoritma dijkstra untuk mencari jalur terdekat
    std::vector<Vertex*> dijkstra(Vertex* start, Vertex* end); // sama tetapi fungsi ini menerima vertex start dan akhir secara langsung
    std::string to_string(); // fungsi untuk mengubah graph -> string/teks
    std::string to_string_detailed(); // sama tetapi lebih mendetail
};

#endif
