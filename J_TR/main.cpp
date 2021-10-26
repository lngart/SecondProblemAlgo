#include <iostream>
#include <limits>
#include <memory>
#include <utility>
#include <vector>
#include <queue>

enum class Fare { FareAccordingRules = 0, FareAgainstRules = 1 };

template<typename T>
class IGraph
{
public:
    using vertex = T;

    virtual const std::vector<std::pair<vertex, Fare>> &get_neighbors(vertex v) const = 0;
    virtual void add_edge(vertex from, vertex to) = 0;
    virtual size_t size() const = 0;
    virtual ~IGraph() {}
};

template<typename T>
class ListGraph : public IGraph<T>
{
    using typename IGraph<T>::vertex;

    std::vector<std::vector<std::pair<vertex, Fare>>> vertices;

public:
    ListGraph(size_t num_vertices) : vertices(num_vertices) {}

    const std::vector<std::pair<vertex, Fare>> &get_neighbors(vertex v) const override
    {
        return vertices[v];
    }

    void add_edge(vertex from, vertex to) override
    {
        vertices[from].push_back({to, Fare::FareAccordingRules});
        vertices[to].push_back({from, Fare::FareAgainstRules});
    }

    inline size_t size() const override { return vertices.size(); }
};

template<typename T>
void bfs(std::unique_ptr<IGraph<T>> &graph, T from, std::vector<T> &distance)
{
    distance[from] = 0;

    std::queue<T> vertices_queue;
    vertices_queue.push(from);

    while (!vertices_queue.empty()) {
        auto top_vertex = vertices_queue.front();
        vertices_queue.pop();

        for (const auto &neighbor : graph->get_neighbors(top_vertex)) {
            T to = neighbor.first, cost = static_cast<T>(neighbor.second);
            if (distance[to] > distance[top_vertex] + cost) {
                vertices_queue.push(to);
                distance[to] = distance[top_vertex] + cost;
            }
        }
    }
}

template<typename T>
void get_cheapest_way(std::unique_ptr<IGraph<T>> &transport_system, T start, T finish)
{
    std::cout.tie(0);
    if (start == finish) {
        std::cout << 0 << std::endl;
        return;
    }

    std::vector<T> distance(transport_system->size(), std::numeric_limits<T>::max());
    bfs(transport_system, start, distance);

    std::cout << (distance[finish] == std::numeric_limits<T>::max() ? -1 : distance[finish])
              << std::endl;
}

template<typename T>
void request_processing(std::unique_ptr<IGraph<T>> &transport_system)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    size_t num_request{0};
    std::cin >> num_request;

    for (size_t i{0}; i < num_request; ++i) {
        T start{0}, finish{0};
        std::cin >> start >> finish;
        get_cheapest_way(transport_system, start - 1, finish - 1); // We use numbering from 0
    }
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
    size_t num_squares{0}, num_lanes_traffic{0};
    std::unique_ptr<IGraph<int>> transport_system;

    initialization(transport_system, num_squares, num_lanes_traffic);

    adding_edge(transport_system, num_lanes_traffic);

    request_processing(transport_system);

    return 0;
}
