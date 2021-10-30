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
void find_articulation_points(std::unique_ptr<IGraph<T>> &graph,
                              T vertex,
                              T parent_vertex,
                              std::vector<Color> &colors,
                              std::vector<size_t> &time_in,
                              std::vector<size_t> &time_up,
                              size_t &current_time,
                              std::vector<bool> &articulation_points)
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
                    articulation_points[vertex] = true;
                ++num_child_vertices;
            }
        }

    if (parent_vertex == std::numeric_limits<T>::max() && num_child_vertices > 1)
        articulation_points[vertex] = true;
}

template<typename T>
void get_non_removable_pillows(std::unique_ptr<IGraph<T>> &graph, size_t num_skyscrapers)
{
    std::vector<Color> colors(graph->size(), Color::white);
    std::vector<size_t> time_in(graph->size());
    std::vector<size_t> time_up(graph->size());
    size_t current_time{1};
    std::vector<bool> articulation_points(graph->size(), false);

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

    size_t cnt_articulation_points{0};
    for (size_t i{num_skyscrapers}; i < graph->size(); ++i)
        if (articulation_points[i])
            ++cnt_articulation_points;

    std::cout << cnt_articulation_points << std::endl;

    if (cnt_articulation_points > 0)
        for (size_t i = num_skyscrapers; i < graph->size(); ++i)
            if (articulation_points[i])
                std::cout << i - (num_skyscrapers - 1) << " ";
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph,
                    size_t &num_skyscrapers,
                    size_t &num_magnetic_pillows)
{
    std::cin >> num_skyscrapers >> num_magnetic_pillows;
    graph = std::make_unique<ListGraph<T>>(num_skyscrapers + num_magnetic_pillows);
}

template<typename T>
void graph_construction(std::unique_ptr<IGraph<T>> &graph, size_t num_magnetic_pillows)
{
    auto num_skyscrapers = graph->size() - num_magnetic_pillows;
    for (size_t i{0}; i < num_magnetic_pillows; ++i) {
        T first_skyscraper{0}, second_skyscraper{0}, third_skyscraper{0};
        std::cin >> first_skyscraper >> second_skyscraper >> third_skyscraper;
        graph->add_edge(first_skyscraper - 1, num_skyscrapers + i); // We use numbering from 0
        graph->add_edge(second_skyscraper - 1, num_skyscrapers + i);
        graph->add_edge(third_skyscraper - 1, num_skyscrapers + i);
    }
}

int main()
{
    size_t num_skyscrapers{0}, num_magnetic_pillows{0};
    std::unique_ptr<IGraph<int>> graph_pillows_and_skyscrapers;

    initialization(graph_pillows_and_skyscrapers, num_skyscrapers, num_magnetic_pillows);

    graph_construction(graph_pillows_and_skyscrapers, num_magnetic_pillows);

    get_non_removable_pillows(graph_pillows_and_skyscrapers, num_skyscrapers);

    return 0;
}
