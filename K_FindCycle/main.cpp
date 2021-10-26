#include <iostream>
#include <memory>
#include <stack>
#include <unordered_set>
#include <utility>
#include <vector>
#include <queue>

template<typename T>
class IGraph
{
public:
    using vertex = T;

    virtual std::vector<vertex> get_neighbors(vertex v) const = 0;
    virtual void add_edge(vertex from, vertex to) = 0;
    virtual size_t size() const = 0;
    virtual ~IGraph() {}
};

template<typename T>
class ListGraph : public IGraph<T>
{
    using typename IGraph<T>::vertex;
    std::vector<std::vector<vertex>> vertices;

public:
    ListGraph(size_t num_vertices) : vertices(num_vertices) {}

    std::vector<vertex> get_neighbors(vertex v) const override { return vertices.at(v); }

    void add_edge(vertex from, vertex to) override { vertices.at(from).push_back(to); }

    inline size_t size() const override { return vertices.size(); }
};

enum class Color { white, gray, black };

template<typename T>
void get_cycle(std::unique_ptr<IGraph<T>> &graph)
{
    std::stack<T> stack_vertices;
    std::vector<T> way(graph->size());
    std::vector<Color> colors(graph->size(), Color::white);

    for (T vertex{0}; vertex < graph->size(); ++vertex) {
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

                auto neighbors = graph->get_neighbors(current_vertex);
                for (const auto &v : neighbors) {
                    if (colors[v] == Color::white) {
                        way[v] = current_vertex;
                        stack_vertices.push(v);
                    } else if (colors[v] == Color::gray) {
                        std::cout.tie(0);
                        std::cout << "YES" << std::endl;

                        std::stack<T> vertices_in_cycle;
                        for (auto vertex_in_cycle = current_vertex; vertex_in_cycle != v;
                             vertex_in_cycle = way[vertex_in_cycle])
                            vertices_in_cycle.push(vertex_in_cycle);
                        vertices_in_cycle.push(v);

                        while (!vertices_in_cycle.empty()) {
                            std::cout << vertices_in_cycle.top() + 1 << " ";
                            vertices_in_cycle.pop();
                        }
                        return;
                    }
                }
            }
        }
    }
    std::cout << "NO";
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph, size_t &num_vertices, size_t &num_edges)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    std::cin >> num_vertices >> num_edges;
    graph = std::make_unique<ListGraph<T>>(num_vertices);
}

template<typename T>
void adding_edge(std::unique_ptr<IGraph<T>> &graph, size_t num_edges)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    for (size_t i{0}; i < num_edges; ++i) {
        T from{0}, to{0};
        std::cin >> from >> to;
        graph->add_edge(from - 1, to - 1); // We use numbering from 0
    }
}

int main()
{
    size_t num_vertices{0}, num_edges{0};
    std::unique_ptr<IGraph<size_t>> graph;

    initialization(graph, num_vertices, num_edges);

    adding_edge(graph, num_edges);

    get_cycle(graph);

    return 0;
}
