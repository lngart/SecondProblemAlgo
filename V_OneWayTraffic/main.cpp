#include <algorithm>
#include <iostream>
#include <map>
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
void find_bridges(std::unique_ptr<IGraph<T>> &graph,
                  T vertex,
                  T parent_vertex,
                  std::vector<Color> &colors,
                  std::vector<size_t> &time_in,
                  std::vector<size_t> &time_up,
                  size_t &current_time,
                  std::set<size_t> &bridges,
                  std::map<std::pair<T, T>, size_t> &number_of_vertices)
{
    colors[vertex] = Color::black;

    time_in[vertex] = current_time;
    time_up[vertex] = current_time;
    ++current_time;

    auto neighbors = graph->get_neighbors(vertex);
    for (const auto &neighbor : neighbors)
        if (neighbor != parent_vertex) {
            if (colors[neighbor] == Color::black)
                time_up[vertex] = std::min(time_up[vertex], time_in[neighbor]);
            else {
                find_bridges(graph,
                             neighbor,
                             vertex,
                             colors,
                             time_in,
                             time_up,
                             current_time,
                             bridges,
                             number_of_vertices);
                time_up[vertex] = std::min(time_up[vertex], time_up[neighbor]);
                if (time_up[neighbor] > time_in[vertex]) {
                    std::pair<T, T> current_edge = {std::min(vertex, neighbor),
                                                    std::max(vertex, neighbor)};
                    bridges.insert(number_of_vertices[current_edge]);
                }
            }
        }
}

template<typename T>
void get_two_way_streets(std::unique_ptr<IGraph<T>> &roads,
                         std::map<std::pair<T, T>, size_t> &number_of_streets)
{
    std::vector<Color> colors(roads->size(), Color::white);
    std::vector<size_t> time_in(roads->size());
    std::vector<size_t> time_up(roads->size());
    size_t current_time{1};
    std::set<size_t> two_way_streets;

    for (T road{0}; road < roads->size(); ++road)
        if (colors[road] == Color::white)
            find_bridges(roads,
                         road,
                         std::numeric_limits<T>::max(),
                         colors,
                         time_in,
                         time_up,
                         current_time,
                         two_way_streets,
                         number_of_streets);

    std::cout << two_way_streets.size() << std::endl;
    for (auto it = two_way_streets.begin(); it != two_way_streets.end(); ++it)
        std::cout << *it + 1 << std::endl; // We use numbering from 0
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph, size_t &num_vertices, size_t &num_edges)
{
    std::cin >> num_vertices >> num_edges;
    graph = std::make_unique<ListGraph<T>>(num_vertices);
}

template<typename T>
void build_roads(std::unique_ptr<IGraph<T>> &roads,
                 std::map<std::pair<T, T>, size_t> &number_of_streets,
                 size_t num_streets)
{
    for (size_t i{0}; i < num_streets; ++i) {
        T from{0}, to{0};
        std::cin >> from >> to;
        roads->add_edge(from - 1, to - 1); // We use numbering from 0
        std::pair<T, T> current_street = {std::min(from - 1, to - 1), std::max(from - 1, to - 1)};
        number_of_streets[current_street] = i; // We use numbering from 0
    }
}

int main()
{
    size_t num_squares{0}, num_streets{0};
    std::unique_ptr<IGraph<int>> roads;
    std::map<std::pair<int, int>, size_t> number_of_streets;

    initialization(roads, num_squares, num_streets);

    build_roads(roads, number_of_streets, num_streets);

    get_two_way_streets(roads, number_of_streets);

    return 0;
}
