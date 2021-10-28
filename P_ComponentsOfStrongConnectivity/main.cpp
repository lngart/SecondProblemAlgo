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

enum class Color { white, black };

template<typename T>
void topological_sort(std::unique_ptr<IGraph<T>> &graph,
                      T vertex,
                      std::vector<Color> &colors,
                      std::vector<T> &sorted_vertices)
{
    colors[vertex] = Color::black;
    for (const auto &neighbor : graph->get_neighbors(vertex))
        if (colors[neighbor] == Color::white)
            topological_sort(graph, neighbor, colors, sorted_vertices);

    sorted_vertices.push_back(vertex);
}

template<typename T>
void create_component_strong_connectivity(std::unique_ptr<IGraph<T>> &graph,
                                          T vertex,
                                          std::vector<Color> &colors,
                                          std::vector<T> &component_strong_connectivity)
{
    colors[vertex] = Color::black;

    component_strong_connectivity.push_back(vertex);

    for (const auto &neighbor : graph->get_neighbors(vertex))
        if (colors[neighbor] == Color::white)
            create_component_strong_connectivity(graph,
                                                 neighbor,
                                                 colors,
                                                 component_strong_connectivity);
}

template<typename T>
void get_component_strong_connectivity(std::unique_ptr<IGraph<T>> &graph,
                                       std::unique_ptr<IGraph<T>> &graph_with_inverted_edges)
{
    std::vector<Color> colors(graph->size(), Color::white);
    std::vector<T> sorted_vertices;

    for (T vertex{0}; vertex < graph->size(); ++vertex)
        if (colors[vertex] == Color::white)
            topological_sort(graph, vertex, colors, sorted_vertices);

    colors.assign(colors.size(), Color::white);

    std::vector<T> number_of_component_strong_connectivity(graph->size());
    T cnt_component_strong_connectivity{1};

    for (T vertex{0}; vertex < graph_with_inverted_edges->size(); ++vertex) {
        auto sorted_vertex = sorted_vertices[(sorted_vertices.size() - 1) - vertex];
        if (colors[sorted_vertex] == Color::white) {
            std::vector<T> component_strong_connectivity;
            create_component_strong_connectivity(graph_with_inverted_edges,
                                                 sorted_vertex,
                                                 colors,
                                                 component_strong_connectivity);

            for (auto it = component_strong_connectivity.begin();
                 it != component_strong_connectivity.end();
                 ++it)
                number_of_component_strong_connectivity[*it] = cnt_component_strong_connectivity;
            ++cnt_component_strong_connectivity;
        }
    }

    std::cout << cnt_component_strong_connectivity - 1 << std::endl;
    for (size_t i{0}; i < number_of_component_strong_connectivity.size(); ++i)
        std::cout << number_of_component_strong_connectivity[i] << " ";
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph,
                    std::unique_ptr<IGraph<T>> &graph_with_inverted_edges,
                    size_t &num_vertices,
                    size_t &num_edges)
{
    std::cin >> num_vertices >> num_edges;
    graph = std::make_unique<ListGraph<T>>(num_vertices);
    graph_with_inverted_edges = std::make_unique<ListGraph<T>>(num_vertices);
}

template<typename T>
void adding_edge(std::unique_ptr<IGraph<T>> &graph,
                 std::unique_ptr<IGraph<T>> &graph_with_inverted_edges,
                 size_t num_edges)
{
    for (size_t i{0}; i < num_edges; ++i) {
        T from{0}, to{0};
        std::cin >> from >> to;
        graph->add_edge(from - 1, to - 1);                     // We use numbering from 0
        graph_with_inverted_edges->add_edge(to - 1, from - 1); // We use numbering from 0
    }
}

int main()
{
    size_t num_vertices{0}, num_edges{0};
    std::unique_ptr<IGraph<size_t>> graph, graph_with_inverted_edges;

    initialization(graph, graph_with_inverted_edges, num_vertices, num_edges);

    adding_edge(graph, graph_with_inverted_edges, num_edges);

    get_component_strong_connectivity(graph, graph_with_inverted_edges);

    return 0;
}
