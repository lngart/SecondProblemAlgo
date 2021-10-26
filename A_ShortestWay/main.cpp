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

    void add_edge(vertex from, vertex to) override
    {
        vertices.at(from).push_back(to);
        vertices.at(to).push_back(from);
    }

    inline size_t size() const override { return vertices.size(); }
};

template<typename T>
void bfs(std::unique_ptr<IGraph<T>> &graph, T from, std::vector<T> &parent, std::vector<T> &distance)
{
    distance[from] = 0;
    std::queue<T> vertices_queue;
    vertices_queue.push(from);
    while (!vertices_queue.empty()) {
        auto top_vertex = vertices_queue.front();
        vertices_queue.pop();
        auto neighbors = graph->get_neighbors(top_vertex);
        for (const auto &v : neighbors)
            if (distance[v] == std::numeric_limits<T>::max()) {
                vertices_queue.push(v);
                distance[v] = distance[top_vertex] + 1;
                parent[v] = top_vertex;
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
        T from{0}, to{0};
        std::cin >> from >> to;
        graph->add_edge(from - 1, to - 1); // We use numbering from 0
    }
}

template<typename T>
void get_shortest_way(std::unique_ptr<IGraph<T>> &graph, T starting_vertex, T ending_vertex)
{
    if (starting_vertex == ending_vertex) {
        std::cout << 0 << std::endl << 1;
        return;
    }
    std::vector<T> parent(graph->size(), std::numeric_limits<T>::max()),
        distance(graph->size(), std::numeric_limits<T>::max());
    bfs(graph, starting_vertex, parent, distance);

    if (parent[ending_vertex] == std::numeric_limits<T>::max())
        std::cout << -1;
    else {
        std::cout << distance[ending_vertex] << std::endl;

        std::vector<T> vertices_on_way;
        vertices_on_way.push_back(ending_vertex);

        auto current_vertex = ending_vertex;
        while (parent[current_vertex] != std::numeric_limits<T>::max()) {
            current_vertex = parent[current_vertex];
            vertices_on_way.push_back(current_vertex);
        }

        for (int j = vertices_on_way.size() - 1; j >= 0; --j)
            std::cout << vertices_on_way[j] + 1 << " "; // We use numbering from 0
    }
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
