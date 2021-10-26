#include <iostream>
#include <limits>
#include <memory>
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
void dfs(std::unique_ptr<IGraph<T>> &graph,
         T vertex,
         std::vector<Color> &colors,
         std::vector<T> &sorted_vertices,
         bool &is_cycle)
{
    colors[vertex] = Color::gray;
    auto neighbors = graph->get_neighbors(vertex);
    for (const auto &v : neighbors) {
        if (colors[v] == Color::gray)
            is_cycle = true;
        else if (colors[v] == Color::white)
            dfs(graph, v, colors, sorted_vertices, is_cycle);
    }

    colors[vertex] = Color::black;
    sorted_vertices.push_back(vertex);
}

template<typename T>
void topological_sort(std::unique_ptr<IGraph<T>> &graph)
{
    auto graph_size = graph->size();

    std::vector<Color> colors(graph_size, Color::white);
    std::vector<T> sorted_vertices;
    bool is_cycle{false};

    for (T vertex{0}; vertex < graph_size; ++vertex)
        if (colors[vertex] == Color::white)
            dfs(graph, vertex, colors, sorted_vertices, is_cycle);

    if (is_cycle)
        std::cout << -1;
    else
        for (int i = graph_size - 1; i >= 0; --i)
            std::cout << sorted_vertices[i] + 1 << " "; // We use numbering from 0
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph, size_t &num_vertices, size_t &num_edges)
{
    std::cin >> num_vertices >> num_edges;
    graph = std::make_unique<ListGraph<T>>(num_vertices);
}

template<typename T>
void adding_edge(std::unique_ptr<IGraph<T>> &graph, size_t num_edges)
{
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

    topological_sort(graph);

    return 0;
}
