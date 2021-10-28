#include <algorithm>
#include <iostream>
#include <memory>
#include <set>
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
void find_articulation_points(std::unique_ptr<IGraph<T>> &graph,
                              T vertex,
                              T parent_vertex,
                              std::vector<Color> &colors,
                              std::vector<size_t> &time_in,
                              std::vector<size_t> &time_up,
                              size_t &current_time,
                              std::set<T> &articulation_points)
{
    T num_child_vertices{0};
    colors[vertex] = Color::black;

    time_in[vertex] = current_time;
    time_up[vertex] = current_time;
    ++current_time;

    for (const auto &neighbor : graph->get_neighbors(vertex))
        if (neighbor != parent_vertex) {
            if (colors[neighbor] == Color::black)
                time_up[vertex] = std::min(time_up[vertex], time_in[neighbor]);
            else {
                find_articulation_points(graph,
                                         neighbor,
                                         vertex,
                                         colors,
                                         time_in,
                                         time_up,
                                         current_time,
                                         articulation_points);
                time_up[vertex] = std::min(time_up[vertex], time_up[neighbor]);
                if (time_up[neighbor] >= time_in[vertex]
                    && parent_vertex != std::numeric_limits<T>::max())
                    articulation_points.insert(vertex);
                ++num_child_vertices;
            }
        }

    if (parent_vertex == std::numeric_limits<T>::max() && num_child_vertices > 1)
        articulation_points.insert(vertex);
}

template<typename T>
void get_articulation_points(std::unique_ptr<IGraph<T>> &graph)
{
    std::vector<Color> colors(graph->size(), Color::white);
    std::vector<size_t> time_in(graph->size());
    std::vector<size_t> time_up(graph->size());
    size_t current_time{1};
    std::set<T> articulation_points;

    for (T vertex{0}; vertex < graph->size(); ++vertex)
        if (colors[vertex] == Color::white)
            find_articulation_points(graph,
                                     vertex,
                                     std::numeric_limits<T>::max(),
                                     colors,
                                     time_in,
                                     time_up,
                                     current_time,
                                     articulation_points);

    std::cout << articulation_points.size() << std::endl;
    for (auto it = articulation_points.begin(); it != articulation_points.end(); ++it)
        std::cout << *it + 1 << std::endl; // We use numbering from 0
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
    std::unique_ptr<IGraph<int>> graph;

    initialization(graph, num_vertices, num_edges);

    adding_edge(graph, num_edges);

    get_articulation_points(graph);

    return 0;
}
