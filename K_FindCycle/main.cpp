#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

template <typename T>
class IGraph {
public:
    using vertex = T;

    virtual const std::vector<vertex> &get_neighbors(vertex v) const = 0;
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

    const std::vector<vertex> &get_neighbors(vertex v) const override {
        return vertices.at(v);
    }

    void add_edge(vertex from, vertex to) override {
        vertices.at(from).push_back(to);
    }

    inline size_t size() const override {
        return vertices.size();
    }
};

enum class Color { white, gray, black };

template <typename T>
void find_cycle(IGraph<T> &graph, std::vector<T> &vertices_in_cycle) {
    std::vector<T> way(graph.size());
    std::stack<T> stack_vertices;
    std::vector<Color> colors(graph.size(), Color::white);

    for (T vertex{0}; vertex < graph.size(); ++vertex) {
        if (colors[vertex] == Color::white) {
            stack_vertices.push(vertex);

            while (!stack_vertices.empty()) {
                auto current_vertex = stack_vertices.top();
                if (colors[current_vertex] == Color::gray) {
                    colors[current_vertex] = Color::black;
                    stack_vertices.pop();
                    continue;
                }

                colors[current_vertex] = Color::gray;

                const auto &neighbors = graph.get_neighbors(current_vertex);
                for (const auto &v : neighbors) {
                    if (colors[v] == Color::white) {
                        way[v] = current_vertex;
                        stack_vertices.push(v);
                    } else if (colors[v] == Color::gray) {
                        for (auto vertex_in_cycle = current_vertex; vertex_in_cycle != v;
                             vertex_in_cycle = way[vertex_in_cycle])
                            vertices_in_cycle.emplace_back(vertex_in_cycle);

                        vertices_in_cycle.emplace_back(v);

                        std::reverse(vertices_in_cycle.begin(), vertices_in_cycle.end());

                        return;
                    }
                }
            }
        }
    }
}

template <typename T>
bool is_there_cycle(std::vector<T> &vertices_in_cycle) {
    return !vertices_in_cycle.empty();
}

template <typename T>
void print_cycle_if_is_there(IGraph<T> &graph) {
    std::vector<T> vertices_in_cycle;

    find_cycle(graph, vertices_in_cycle);

    if (is_there_cycle(vertices_in_cycle)) {
        std::cout << "YES\n";

        for (const auto &vertex : vertices_in_cycle)
            std::cout << vertex + 1 << " ";
    } else
        std::cout << "NO";
}

void initialization(size_t &num_vertices, size_t &num_edges) {
    std::cin >> num_vertices >> num_edges;
}

template <typename T>
void adding_edge(IGraph<T> &graph, size_t num_edges) {
    for (size_t i{0}; i < num_edges; ++i) {
        T from{0}, to{0};
        std::cin >> from >> to;
        graph.add_edge(from - 1, to - 1);  // We use numbering from 0
    }
}

int main() {
    size_t num_vertices{0}, num_edges{0};

    initialization(num_vertices, num_edges);

    ListGraph<size_t> graph(num_vertices);

    adding_edge(graph, num_edges);

    print_cycle_if_is_there(graph);

    return 0;
}
