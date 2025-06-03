#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <algorithm>
#include <sstream>
#include <graph.hpp>

// konstruktor Edge, menerima sebuah target dan weight untuk membuat sebuah edge baru
Edge::Edge(int target, int weight) : target(target), weight(weight) { }

// fungsi untuk mengubah suatu edge menjadi string/teks
std::string Edge::to_string() {
    std::ostringstream oss;
    oss << "[Edge: (target=" << this->target << ", weight=" << this->weight << ")]";
    return oss.str();
}

// konstruktor Vertex, menerima sebuah ID, dan koordinat [x, y] untuk membuat sebuah vertex baru
Vertex::Vertex(int id, int x, int y) : id(id), x(x), y(y) { }

// destruktor Vertex, menjelaskan ke C++ bagaimana cara menghapus vertex ini dari memori
Vertex::~Vertex() {
    // bilang ke C++ untuk iterasi terhadap semua edge didalam vertex ini ketika menghapus, kemudian...
    for (Edge* e : this->edges) {
        delete e; // hapus setiap edge itu dari
    }
}

// fungsi untuk mendapatkan suatu edge dari sebuah vertex. menerima sebuah integer "to" untuk menjelaskan
// edge mana yang akan diambil melalui target edge tersebut
Edge* Vertex::get_edge(int to) {
    // untuk setiap edge dari vertex ini...
    for (Edge* e : this->edges) {
        // jika targetnya sama dengan input "to" kita, maka...
        if (e->target == to) return e; // return edge tersebut
    }

    // ika tidak ketemu, maka print error, dan...
    fprintf(stderr, "get_edge: edge connecting vertex %d with %d is not found...", this->id, to);
    return nullptr; // return nullptr/kosong
}

// fungsi untuk menambahkan edge baru kedalam sebuah vertex
void Vertex::add_edge(int to, int weight) {
    // tambahkan edge baru tersebut ke "edges" melalui "push_back"
    this->edges.push_back(new Edge(to, weight));
}

// sama seperti yang diatas tetapi fungsi ini menerima sebuah objek Edge secara langsung
void Vertex::add_edge(Edge& edge) {
    // tambahkan objek edge tersebut ke "edges" melalui "push_back"
    this->edges.push_back(&edge);
}

// fungsi untuk menghapus suatu edge dari sebuah vertex
void Vertex::remove_edge(int to) {
    // dapatkan edge yang akan dihapus melalui targetnya ("to") menggunakan "get_edge"
    Edge* toRemove = this->get_edge(to);

    std::vector<Edge*>& es = this->edges; // ambil list edges dari vertex ini sebagai "es"
    es.erase(std::remove(es.begin(), es.end(), toRemove), es.end()); // hapus "toRemove" dari list edges
    delete toRemove; // kemudian hapus "toRemove" dari memori
}

// fungsi untuk mengupdate nilai weight suatu edge dari sebuah vertex
void Vertex::update_edge_weight(int to, int weight) {
    // dapatkan edge tersebut dan dapatkan weightnya juga, kemudian set menjadi nilai weight kita
    this->get_edge(to)->weight = weight;
}

// fungsi untuk mengubah suatu vertex menjadi string/teks
std::string Vertex::to_string() {
    std::ostringstream oss;
    oss << "[Vertex: (id=" << this->id << ", x=" << this->x << ", y=" << this->y << ", edges=" << this->edges.size() << ")]";
    return oss.str();
}

// sama seperti yang diatas tapi fungsi ini mengeluarkan representasi teks yang lebih mendetail
std::string Vertex::to_string_detailed() {
    std::ostringstream oss;
    oss << "[Vertex: (id=" << this->id << ", x=" << this->x << ", y=" << this->y << ", edges=" << this->edges.size() << ")]\n";
    for (Edge* e : this->edges) {
        oss << "-> " << e->to_string() << "\n";
    }
    return oss.str();
}

// destruktor Graph, menjelaskan ke C++ bagaimana cara menghapus graph ini dari memori
Graph::~Graph() {
    // bilang ke C++ untuk iterasi terhadap semua vertex didalam graph ini ketika menghapus, kemudian...
    for (Vertex* v : this->vertices) {
        delete v; // hapus setiap vertex itu dari memori
    }
}

// fungsi untuk mendapatkan suatu vertex dari sebuah graph
Vertex* Graph::get_vertex(int id) {
    // untuk semua vertex dari graph ini...
    for (Vertex* v : this->vertices) {
        // jika ID-nya sama dengan "id" input kita, maka...
        if (v->id == id) return v; // return vertex tersebut
    }

    // jika tidak ketemu, maka print error, dan...
    fprintf(stderr, "get_vertex: vertex with id %d not found...", id);
    return nullptr; // return nullptr/kosong
}

// fungsi untuk menambahkan sebuah vertex baru kedalam sebuah graph 
void Graph::add_vertex(int id, int x, int y) {
    // tambahkan vertex baru tersebut ke "vertices" melalui "push_back"
    this->vertices.push_back(new Vertex(id, x, y));
}

// sama seperti yang diatas tetapi fungsi ini menerima sebuah objek Vertex secara langsung
void Graph::add_vertex(Vertex& vertex) {
    // tambahkan objek vertex tersebut ke "vertices" melalui "push_back"
    this->vertices.push_back(&vertex);
}

// fungsi untuk menghapus suatu vertex dari sebuah graph
void Graph::remove_vertex(int id) {
    // dapatkan vertex yang akan dihapus melalui ID-nya menggunakan "get_vertex"
    Vertex* toRemove = this->get_vertex(id);
    // dapatkan list edges-nya juga sebagai "copy"
    std::vector<Edge*> copy = toRemove->edges;

    // loop dibawah ini fungsinya untuk menghapus edge-edge yang keluar dari "toRemove"
    for (Edge* e : copy) { // untuk semua edge didalam list edge ini...
        toRemove->remove_edge(e->target); // hapus edge tersebut dari "toRemove"
    }

    // loop dibawah ini fungsinya untuk menghapus edge-edge yang masuk menuju ke "toRemove"
    for (Vertex* v : this->vertices) { // untuk semua vertex yang ada didalam graph ini...
        std::vector<Edge*> edgesCopy = v->edges; // dapatkan list edge-nya sebagai "edgesCopy"

        for (Edge* e : edgesCopy) { // untuk semua edge didalam list edge tersebut...
            // jika edge tersebut targeetnya menuju ke vertex yang ingin kita hapus, maka...
            if (e->target == id) v->remove_edge(id); // hapus edge tersebut
        }
    }

    // hapus "toRemove" dari vertices
    vertices.erase(std::remove(vertices.begin(), vertices.end(), toRemove), vertices.end());
    delete toRemove; // hapus "toRemove" dari memori
}

// fungsi untuk menghapus suatu edge dari sebuah graph
void Graph::remove_edge(Edge* toRemove) {
    for (Vertex* v : this->vertices) { // untuk semua vertex yang ada didalam graph ini...
        std::vector<Edge*>& edges = v->edges; // dapatkan list edge-nya sebagai "edges"

        // cari edge yang ingin kita hapus ("toRemove") dari "edges"
        auto it = std::find(edges.begin(), edges.end(), toRemove);
        if (it != edges.end()) { // jika ketemu, maka...
            edges.erase(it); // hapus "toRemove" dari list
            delete toRemove; // hapus "toRemove" dari memori
            return; // selesai
        }
    }
}

// fungsi untuk mencari jalur terdekat antara "start" dan "end" menggunakan algoritma dijkstra
std::vector<int> Graph::dijkstra(int start, int end) {
    // pertama siapkan data-data yang diperlukan. "x" merujuk ke vertex yang sedang diakses melalui indeks, misalkan
    // indeks = 2, maka vertex "x" = vertices[2]

    std::vector<int> ids; // "ids" untuk menyimpan id-id vertex yang ada
    std::vector<int> dist; // "dist" untuk menyimpan jarak terpendek yang sejauh ini ditemukan dari "x" ke "start"
    std::vector<int> prev; // "prev" untuk menyimpan sebuah jalur terdekat kasaran untuk mencapai "x"
    std::vector<bool> visited; // "visited" untuk menyimpan informasi apakah vertex ini sudah dikunjungi atau belum

    // loop dibawah fungsinya adalah untuk inisialisasi data diatas
    int n = vertices.size(); // "n" = vertices.size(), artinya "n" = jumlah vertex yang ada
    for (int i = 0; i < n; ++i) { // iterasi dari vertex 0 sampai vertex ke-n
        ids.push_back(vertices[i]->id); // simpan ID vertex yang sedang diiterasi ke "ids"

        // karena graph belum dijelajahi, maka...
        dist.push_back(2147483647); // set semua "dist" ke ∞, disini kita menggunakan nilai max dari integer
        prev.push_back(-1); // set semua "prev" ke -1, sebab tidak diketahui vertex mana untuk dikunjungi sebelum "x"
        visited.push_back(false); // set semua "visited" ke false, karena belum ada vertex yang dikunjungi
    }

    // khusus untuk vertex "start", karena jarak dari "start" ke "start" itu sendiri adalah 0, kita set "dist"-nya ke 0 juga
    int start_index = -1; // -1 karena belum ketemu
    for (int i = 0; i < n; ++i) { // iterasi dari vertex 0 ke vertex ke-n
        if (vertices[i]->id == start) { // jika vertex tersebut ID-nya sama dengan "start", maka...
            dist[i] = 0; // set "dist"-nya menjadi 0
            start_index = i; // dan simpan indeks yang ditemukan ke "start_index"
            break; // selesai
        }
    }

    while (true) { // loop utama
        // langkah 1: cari suatu vertex yang belum dikunjungi dan memiliki "dist" terkecil. sebut saja vertex ini sebagai "u"
        int u_index = -1; // indeks vertex "u" didalam array "vertices"
        int min_dist = 2147483647; // sebuah variabel pembantu untuk mencari nilai "dist" terkecil
        for (int i = 0; i < n; ++i) { // iterasi dari vertex 0 ke vertex ke-n
            // jika vertex tersebut belum dikunjungi dan "dist"-nya lebih kecil dari "min_dist"
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i]; // update nilai "min_dist" dengan nilai minimal yang terbaru
                u_index = i; // simpan indeks ini ke "u_index"
            }
        } // setelah loop selesai kita harusnya akan mendapatkan vertex "u" yang kita inginkan

        // tetapi jika masih tidak ditemukan "u", maka...
        if (u_index == -1) break; // keluar dari loop
        else visited[u_index] = true; // jika tidak, set "visited" untuk "u" sebagai true ("u" telah dikunjungi)

        // jika "u" berada di vertex tujuan, maka...
        if (ids[u_index] == end) break; // keluar dari loop, karena kita telah sampai di tujuan

        // dapatkan vertex aslinya "u" dari "vertices" menggunakan indeksnya
        Vertex* u = vertices[u_index];
        for (int i = 0; i < u->edges.size(); ++i) { // untuk setiap edge dari "u"...
            Edge* edge = u->edges[i]; // ambil edgenya
            int v_id = edge->target; // ambil juga ID vertex yang ditunjuknya. sebut saja vertex ini sebagai "v"

            // fungsi dari variabel ini adalah untuk memperbarui nilai "dist" jika ditemukan sebuah jarak yang lebih pendek
            int alt = dist[u_index] + edge->weight;

            for (int j = 0; j < n; ++j) { // iterasi dari vertex 0 ke vertex-n
                // jika vertex yang sedang diiterasi adalah "v", dan "alt" lebih kecil dari "dist" milik "v", maka...
                if (ids[j] == v_id && alt < dist[j]) {
                    dist[j] = alt; // kita update "dist"-nya "v" menjadi nilai baru yang lebih kecil ini ("alt")
                    prev[j] = u_index; // kemudian kita tambahkan "u" kedalam jalur kasaran tadi ("prev")
                }
            }
        }
        // loop terus berulang sampai "u" berada di vertex tujuan
    }

    // setelah loop selesai, waktunya kita membuat jalur ("path") final yang akan direturn
    std::vector<int> path;

    // pertama temukan dulu indeks untuk vertex tujuan
    int end_index = -1; // -1 karena belum ketemu
    for (int i = 0; i < n; ++i) { // iterasi dari vertex 0 ke vertex ke-n
        if (ids[i] == end) { // jika vertex tersebut ID-nya sama dengan "end", maka...
            end_index = i; // simpan indeks yang ditemukan ke "end_index"
            break; // selesai
        }
    }

    int u = end_index; // mulai dari vertex tujuan, sebagai "u"
    while (u != -1) { // selama "u" masih sebuah vertex (tidak -1/kosong)
        path.insert(path.begin(), ids[u]); // masukkan "u" kedalam "path"
        u = prev[u]; // jalankan "u" ke vertex selanjutnya di "prev"
    }

    // jika path akhir kosong atau awalnya tidak sesuai dengan "start", maka...
    if (path.size() == 0 || path[0] != start) path.clear(); // clear "path"-nya

    return path; // akhirnya kita return "path"-nya
}

 // sama seperti yang diatas tetapi fungsi ini menerima vertex start dan akhir secara langsung
std::vector<Vertex*> Graph::dijkstra(Vertex* start, Vertex* end) {
    // konversi "start" dan "end" ke ID-nya agar bisa diproses oleh fungsi dijkstra diatas
    std::vector<int> path = dijkstra(start->id, end->id);
    // sebuah path yang merupakan representasi vertex secara langsung dari path yang didapat diatas
    std::vector<Vertex*> vertices_path = { };

    for (int id : path) { // untuk setiap ID vertex dari "path"...
        // masukkan ID tersebut sebagai vertex menuju ke "vertices_path"
        vertices_path.push_back(this->get_vertex(id));
    }

    return vertices_path; // return path dalam bentuk vertex tersebut
}

// fungsi untuk mengubah suatu graph menjadi string/teks
std::string Graph::to_string() {
    std::ostringstream oss;
    oss << "[Graph: (vertices=" << this->vertices.size() << ")]";
    return oss.str();
}

// sama seperti yang diatas tapi fungsi ini mengeluarkan representasi teks yang lebih mendetail
std::string Graph::to_string_detailed() {
    std::ostringstream oss;
    oss << "[Graph: (vertices=" << this->vertices.size() << ")]";
    for (Vertex* v : this->vertices) {
        oss << "-> " << v->to_string_detailed();
    }
    return oss.str();
}

