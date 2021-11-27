#include <iostream>
#include <limits>
#include <memory>
#include <utility>
#include <vector>
#include <queue>

template <typename T>
class IGraph {
public:
    using vertex = T;

    virtual std::vector<vertex> get_neighbors(vertex v) const = 0;
    virtual void add_edge(vertex from, vertex to) = 0;
    virtual size_t size() const = 0;
    virtual ~IGraph() {
    }
};

template <typename T>
class ListGraph : public IGraph<T> {
    using typename IGraph<T>::vertex;
    std::vector<std::vector<vertex>> vertices;

public:
    ListGraph(size_t num_vertices) : vertices(num_vertices) {
    }

    std::vector<vertex> get_neighbors(vertex v) const override {
        return vertices.at(v);
    }

    void add_edge(vertex from, vertex to) override {
        vertices.at(from).push_back(to);
        vertices.at(to).push_back(from);
    }

    inline size_t size() const override {
        return vertices.size();
    }
};

enum class Color { white, red, blue };

Color get_invert_color(Color color) {
    if (color == Color::blue)
        return Color::red;
    else if (color == Color::red)
        return Color::blue;
    else
        return color;
}

template <typename T>
bool BFS(IGraph<T> &graph, std::vector<Color> &colors, T starting_vertex) {
    colors[starting_vertex] = Color::red;

    std::queue<T> vertices_queue;
    vertices_queue.push(starting_vertex);

    while (!vertices_queue.empty()) {
        auto top_vertex = vertices_queue.front();
        vertices_queue.pop();

        auto neighbors = graph.get_neighbors(top_vertex);
        for (const auto &v : neighbors) {
            if (colors[v] == Color::white) {
                vertices_queue.push(v);
                colors[v] = get_invert_color(colors[top_vertex]);
            } else if (colors[v] == colors[top_vertex])
                return false;
        }
    }
    return true;
}

template <typename T>
bool is_bipartite_graph(IGraph<T> &graph) {
    bool is_bipartite{true};
    std::vector<Color> colors(graph.size(), Color::white);

    for (T current_vertex{0}; current_vertex < graph.size() && is_bipartite; ++current_vertex)
        if (colors[current_vertex] == Color::white)
            is_bipartite = is_bipartite && BFS(graph, colors, current_vertex);

    return is_bipartite;
}

template <typename T>
void print_is_possible_separate_students(IGraph<T> &students) {
    std::cout << (is_bipartite_graph(students) ? "YES" : "NO");
}

void initialization(size_t &num_students, size_t &num_pairs_students) {
    std::cin >> num_students >> num_pairs_students;
}

template <typename T>
void build_graph_from_pairs_students(IGraph<T> &students, size_t num_pairs_students) {
    for (size_t i{0}; i < num_pairs_students; ++i) {
        T from{0}, to{0};
        std::cin >> from >> to;
        students.add_edge(from - 1, to - 1);  // We use numbering from 0
    }
}

int main() {
    size_t num_students{0}, num_pairs_students{0};
    initialization(num_students, num_pairs_students);

    ListGraph<size_t> students(num_students);

    build_graph_from_pairs_students(students, num_pairs_students);

    print_is_possible_separate_students(students);

    return 0;
}
