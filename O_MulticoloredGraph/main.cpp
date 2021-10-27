#include <iostream>
#include <memory>
#include <vector>

template<typename T>
class IGraph
{
public:
    using vertex = T;

    virtual const std::vector<vertex> &get_neighbors(vertex v) const = 0;
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

    const std::vector<vertex> &get_neighbors(vertex v) const override { return vertices[v]; }

    void add_edge(vertex from, vertex to) override { vertices[from].push_back(to); }

    inline size_t size() const override { return vertices.size(); }
};

enum class Color { white, gray, black };

template<typename T>
void search_cycle(std::unique_ptr<IGraph<T>> &graph,
                  T vertex,
                  std::vector<Color> &colors,
                  bool &is_cycle)
{
    if (is_cycle)
        return;

    colors[vertex] = Color::gray;
    for (const auto &neighbor : graph->get_neighbors(vertex)) {
        if (colors[neighbor] == Color::gray) {
            is_cycle = true;
            return;
        } else if (colors[neighbor] == Color::white)
            search_cycle(graph, neighbor, colors, is_cycle);
        if (is_cycle)
            return;
    }

    colors[vertex] = Color::black;
}

template<typename T>
void is_there_cycle(std::unique_ptr<IGraph<T>> &graph)
{
    std::vector<Color> colors(graph->size(), Color::white);
    bool is_cycle{false};

    for (T vertex{0}; vertex < graph->size(); ++vertex)
        if (colors[vertex] == Color::white) {
            search_cycle(graph, vertex, colors, is_cycle);
            if (is_cycle)
                break;
        }

    std::cout << (is_cycle ? "NO" : "YES");
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph, size_t &num_vertices)
{
    std::cin >> num_vertices;
    graph = std::make_unique<ListGraph<T>>(num_vertices);
}

template<typename T>
void adding_edge(std::unique_ptr<IGraph<T>> &graph)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    std::string edge_coloring;
    for (T current_vertex{0}; current_vertex < graph->size() - 1; ++current_vertex) {
        T next_vertex{current_vertex};

        std::cin >> edge_coloring;

        for (const auto &color_current_edge : edge_coloring) {
            ++next_vertex;
            if (color_current_edge == 'R')
                graph->add_edge(next_vertex, current_vertex);
            else if (color_current_edge == 'B')
                graph->add_edge(current_vertex, next_vertex);
        }
    }
}

int main()
{
    size_t num_vertices{0};
    std::unique_ptr<IGraph<size_t>> graph;

    initialization(graph, num_vertices);

    adding_edge(graph);

    is_there_cycle(graph);

    return 0;
}
