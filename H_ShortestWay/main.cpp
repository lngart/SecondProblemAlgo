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
    virtual void add_edge(vertex from, vertex to, T weight) = 0;
    virtual T get_weight_edge(vertex from, vertex to) const = 0;
    virtual size_t size() const = 0;
    virtual ~IGraph() {}
};

template<typename T>
class ListGraph : public IGraph<T>
{
    using typename IGraph<T>::vertex;
    std::vector<std::vector<vertex>> vertices;
    std::vector<std::vector<vertex>> weight_edges;

public:
    ListGraph(size_t num_vertices)
        : vertices(num_vertices), weight_edges(num_vertices, std::vector<vertex>(num_vertices))
    {}

    std::vector<vertex> get_neighbors(vertex v) const override { return vertices.at(v); }

    void add_edge(vertex from, vertex to, T weight) override
    {
        vertices.at(from).push_back(to);
        weight_edges.at(from).at(to) = weight;
    }

    T get_weight_edge(vertex from, vertex to) const override
    {
        return weight_edges.at(from).at(to);
    }

    inline size_t size() const override { return vertices.size(); }
};

template<typename T>
void bfs(std::unique_ptr<IGraph<T>> &graph, T from, std::vector<T> &distance)
{
    std::queue<T> vertices_queue;
    vertices_queue.push(from);

    distance[from] = 0;

    while (!vertices_queue.empty()) {
        auto top_vertex = vertices_queue.front();
        vertices_queue.pop();
        auto neighbors = graph->get_neighbors(top_vertex);
        for (const auto &v : neighbors)
            if (distance[v] > distance[top_vertex] + graph->get_weight_edge(top_vertex, v)) {
                vertices_queue.push(v);
                distance[v] = distance[top_vertex] + graph->get_weight_edge(top_vertex, v);
            }
    }
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph,
                    size_t &num_vertices,
                    size_t &num_edges,
                    T &starting_vertex,
                    T &ending_vertex)
{
    std::cin >> num_vertices >> num_edges >> starting_vertex >> ending_vertex;
    --starting_vertex, --ending_vertex; // We use numbering from 0
    graph = std::make_unique<ListGraph<T>>(num_vertices);
}

template<typename T>
void adding_edge(std::unique_ptr<IGraph<T>> &graph, size_t num_edges)
{
    for (size_t i{0}; i < num_edges; ++i) {
        T from{0}, to{0}, weight{0};
        std::cin >> from >> to >> weight;
        graph->add_edge(from - 1, to - 1, weight); // We use numbering from 0
    }
}

template<typename T>
void get_shortest_way(std::unique_ptr<IGraph<T>> &graph, T starting_vertex, T ending_vertex)
{
    if (starting_vertex == ending_vertex) {
        std::cout << 0;
        return;
    }
    std::vector<T> distance(graph->size(), std::numeric_limits<T>::max());
    bfs(graph, starting_vertex, distance);

    if (distance[ending_vertex] == std::numeric_limits<T>::max())
        std::cout << -1;
    else
        std::cout << distance[ending_vertex] << std::endl;
}

int main()
{
    size_t num_vertices{0}, num_edges{0}, starting_vertex{0}, ending_vertex{0};
    std::unique_ptr<IGraph<size_t>> graph;

    initialization(graph, num_vertices, num_edges, starting_vertex, ending_vertex);

    adding_edge(graph, num_edges);

    get_shortest_way(graph, starting_vertex, ending_vertex);

    return 0;
}
