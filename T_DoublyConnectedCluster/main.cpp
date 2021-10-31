#include <algorithm>
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

    void add_edge(vertex from, vertex to) override
    {
        vertices[from].push_back(to);
        vertices[to].push_back(from);
    }

    inline size_t size() const override { return vertices.size(); }
};

enum class Color { white, black };

template<typename T>
void dfs(std::unique_ptr<IGraph<T>> &graph,
         T vertex,
         T parent_vertex,
         std::vector<Color> &colors,
         std::vector<size_t> &time_in,
         std::vector<size_t> &time_up,
         size_t &current_time)
{
    colors[vertex] = Color::black;

    time_in[vertex] = current_time;
    time_up[vertex] = current_time;
    ++current_time;

    for (const auto &neighbor : graph->get_neighbors(vertex))
        if (neighbor != parent_vertex) {
            if (colors[neighbor] == Color::black)
                time_up[vertex] = std::min(time_up[vertex], time_in[neighbor]);
            else {
                dfs(graph, neighbor, vertex, colors, time_in, time_up, current_time);
                time_up[vertex] = std::min(time_up[vertex], time_up[neighbor]);
            }
        }
}

template<typename T>
void find_biconnected_component(std::unique_ptr<IGraph<T>> &graph,
                                T vertex,
                                std::vector<size_t> &time_in,
                                std::vector<size_t> &time_up,
                                size_t current_biconnected_component,
                                size_t &cnt_biconnected_component,
                                std::vector<size_t> &number_of_biconnected_component)
{
    number_of_biconnected_component[vertex] = current_biconnected_component;

    for (const auto &neighbor : graph->get_neighbors(vertex))
        if (number_of_biconnected_component[neighbor] == std::numeric_limits<size_t>::max()) {
            if (time_up[neighbor] > time_in[vertex]) {
                ++cnt_biconnected_component;
                find_biconnected_component(graph,
                                           neighbor,
                                           time_in,
                                           time_up,
                                           cnt_biconnected_component,
                                           cnt_biconnected_component,
                                           number_of_biconnected_component);
            } else
                find_biconnected_component(graph,
                                           neighbor,
                                           time_in,
                                           time_up,
                                           current_biconnected_component,
                                           cnt_biconnected_component,
                                           number_of_biconnected_component);
        }
}

template<typename T>
void get_biconnected_components(std::unique_ptr<IGraph<T>> &graph,
                                std::vector<size_t> &number_of_biconnected_component,
                                size_t &cnt_biconnected_component)
{
    std::vector<Color> colors(graph->size(), Color::white);
    std::vector<size_t> time_in(graph->size());
    std::vector<size_t> time_up(graph->size());
    size_t current_time{1};

    for (T vertex{0}; vertex < graph->size(); ++vertex)
        if (colors[vertex] == Color::white)
            dfs(graph, vertex, std::numeric_limits<T>::max(), colors, time_in, time_up, current_time);

    for (T vertex{0}; vertex < graph->size(); ++vertex)
        if (number_of_biconnected_component[vertex] == std::numeric_limits<size_t>::max()) {
            find_biconnected_component(graph,
                                       vertex,
                                       time_in,
                                       time_up,
                                       cnt_biconnected_component,
                                       cnt_biconnected_component,
                                       number_of_biconnected_component);
            ++cnt_biconnected_component;
        }
}

template<typename T>
void get_num_hypertunel(std::unique_ptr<IGraph<T>> &graph)
{
    std::vector<size_t> number_of_biconnected_component(graph->size(),
                                                        std::numeric_limits<size_t>::max());
    size_t cnt_biconnected_component{0};

    get_biconnected_components(graph, number_of_biconnected_component, cnt_biconnected_component);

    std::vector<size_t> number_of_neighboring_biconnected_component(cnt_biconnected_component);

    for (T vertex{0}; vertex < graph->size(); ++vertex)
        for (const auto &neighbor : graph->get_neighbors(vertex))
            if (number_of_biconnected_component[vertex]
                != number_of_biconnected_component[neighbor]) {
                auto tmp_index = number_of_biconnected_component[vertex];
                ++number_of_neighboring_biconnected_component[tmp_index];
            }

    size_t cnt_hypertunel{0};

    for (const auto &num_neighbor : number_of_neighboring_biconnected_component)
        if (num_neighbor == 1)
            ++cnt_hypertunel;

    std::cout << ((cnt_hypertunel + 1) >> 1);
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph, size_t &num_vertices, size_t &num_edges)
{
    std::cin >> num_vertices >> num_edges;
    graph = std::make_unique<ListGraph<T>>(num_vertices);
}

template<typename T>
void create_campus(std::unique_ptr<IGraph<T>> &graph, size_t num_edges)
{
    for (size_t i{0}; i < num_edges; ++i) {
        T from{0}, to{0};
        std::cin >> from >> to;
        graph->add_edge(from - 1, to - 1); // We use numbering from 0
    }
}

int main()
{
    size_t num_buildings{0}, num_built_hypertonels{0};
    std::unique_ptr<IGraph<int>> campus;

    initialization(campus, num_buildings, num_built_hypertonels);

    create_campus(campus, num_built_hypertonels);

    get_num_hypertunel(campus);

    return 0;
}
