#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

template <typename T>
class IGraph {
public:
    using vertex = T;

    virtual const std::vector<vertex> &get_neighbors(vertex v) const = 0;
    virtual void add_edge(vertex from, vertex to) = 0;
    virtual std::shared_ptr<IGraph<T>> invert_edges() = 0;
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
        return vertices[v];
    }

    void add_edge(vertex from, vertex to) override {
        vertices[from].push_back(to);
    }

    std::shared_ptr<IGraph<T>> invert_edges() override {
        std::shared_ptr<IGraph<T>> graph_with_inverted_edges = std::make_shared<ListGraph<T>>(vertices.size());

        for (vertex v = 0; v < vertices.size(); ++v)
            for (const auto &neighbor : vertices[v])
                graph_with_inverted_edges->add_edge(neighbor, v);

        return graph_with_inverted_edges;
    }

    inline size_t size() const override {
        return vertices.size();
    }
};

enum class Color { white, black };

template <typename T>
void dfs(IGraph<T> &graph, T vertex, std::vector<Color> &colors, std::vector<T> &sorted_vertices) {
    colors[vertex] = Color::black;

    for (const auto &neighbor : graph.get_neighbors(vertex))
        if (colors[neighbor] == Color::white)
            dfs(graph, neighbor, colors, sorted_vertices);

    sorted_vertices.push_back(vertex);
}

template <typename T>
void arrange_vertices(IGraph<T> &graph, std::vector<T> &sorted_vertices) {
    std::vector<Color> colors(graph.size(), Color::white);

    for (T vertex{0}; vertex < graph.size(); ++vertex)
        if (colors[vertex] == Color::white)
            dfs(graph, vertex, colors, sorted_vertices);

    std::reverse(sorted_vertices.begin(), sorted_vertices.end());
}

template <typename T>
void create_component_strong_connectivity(IGraph<T> &graph, T vertex, std::vector<Color> &colors,
                                          std::vector<T> &component_strong_connectivity) {
    colors[vertex] = Color::black;

    component_strong_connectivity.push_back(vertex);

    for (const auto &neighbor : graph.get_neighbors(vertex))
        if (colors[neighbor] == Color::white)
            create_component_strong_connectivity(graph, neighbor, colors, component_strong_connectivity);
}

template <typename T>
void find_component_strong_connectivity(IGraph<T> &graph, std::vector<T> &number_of_component_strong_connectivity) {
    auto graph_with_inverted_edges = graph.invert_edges();

    std::vector<T> sorted_vertices;
    std::vector<Color> colors(graph.size(), Color::white);

    arrange_vertices(graph, sorted_vertices);

    T cnt_component_strong_connectivity{1};

    for (T vertex{0}; vertex < graph_with_inverted_edges->size(); ++vertex) {
        auto sorted_vertex = sorted_vertices[vertex];
        if (colors[sorted_vertex] == Color::white) {
            std::vector<T> component_strong_connectivity;

            create_component_strong_connectivity(*graph_with_inverted_edges, sorted_vertex, colors,
                                                 component_strong_connectivity);

            for (const auto &v : component_strong_connectivity)
                number_of_component_strong_connectivity[v] = cnt_component_strong_connectivity;

            ++cnt_component_strong_connectivity;
        }
    }
}

template <typename T>
T find_quantity_of_components_strong_connectivity(std::vector<T> &number_of_component_strong_connectivity) {
    return *std::max_element(number_of_component_strong_connectivity.begin(),
                             number_of_component_strong_connectivity.end());
}

template <typename T>
void print_component_strong_connectivity(IGraph<T> &graph) {
    std::vector<T> number_of_component_strong_connectivity(graph.size());

    find_component_strong_connectivity(graph, number_of_component_strong_connectivity);

    const auto quantity_of_components_strong_connectivity =
        find_quantity_of_components_strong_connectivity(number_of_component_strong_connectivity);
    std::cout << quantity_of_components_strong_connectivity << std::endl;

    for (size_t i{0}; i < number_of_component_strong_connectivity.size(); ++i)
        std::cout << number_of_component_strong_connectivity[i] << " ";
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

    print_component_strong_connectivity(graph);

    return 0;
}
