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

template<typename T>
void get_num_broken_piggy_banks(std::unique_ptr<IGraph<T>> &graph)
{
    size_t num_cycles{0};

    std::vector<T> way(graph->size(), std::numeric_limits<T>::max());
    for (T vertex{0}; vertex < graph->size(); ++vertex) {
        if (way[vertex] == std::numeric_limits<T>::max()) {
            auto starting_vertex = vertex, vertex_in_way = vertex;
            auto neighbors = graph->get_neighbors(vertex_in_way);

            for (; way[vertex_in_way] == std::numeric_limits<T>::max();
                 vertex_in_way = neighbors[0]) {
                way[vertex_in_way] = starting_vertex;
                neighbors = graph->get_neighbors(vertex_in_way);
            }

            if (way[vertex_in_way] == starting_vertex)
                ++num_cycles;
        }
    }

    std::cout << num_cycles;
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph, size_t &num_vertices)
{
    std::cin >> num_vertices;
    graph = std::make_unique<ListGraph<T>>(num_vertices);
}

template<typename T>
void adding_edge(std::unique_ptr<IGraph<T>> &graph, size_t num_vertices)
{
    for (size_t i{0}; i < num_vertices; ++i) {
        T to{0};
        std::cin >> to;
        graph->add_edge(i, to - 1); // We use numbering from 0
    }
}

int main()
{
    size_t num_piggy_banks{0};
    std::unique_ptr<IGraph<size_t>> piggy_banks;

    initialization(piggy_banks, num_piggy_banks);

    adding_edge(piggy_banks, num_piggy_banks);

    get_num_broken_piggy_banks(piggy_banks);

    return 0;
}
