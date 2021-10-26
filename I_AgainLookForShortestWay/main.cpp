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

    virtual std::vector<std::pair<vertex, T>> get_neighbors(vertex v) const = 0;
    virtual void add_edge(vertex from, vertex to, T weight) = 0;
    virtual unsigned short size() const = 0;
    virtual ~IGraph() {}
};

template<typename T>
class ListGraph : public IGraph<T>
{
    using typename IGraph<T>::vertex;
    std::vector<std::vector<std::pair<vertex, T>>> vertices;

public:
    ListGraph(unsigned short num_vertices) : vertices(num_vertices) {}

    std::vector<std::pair<vertex, T>> get_neighbors(vertex v) const override { return vertices[v]; }

    void add_edge(vertex from, vertex to, T weight) override
    {
        vertices[from].push_back({to, weight});
    }

    inline unsigned short size() const override { return vertices.size(); }
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
            if (distance[v.first] > distance[top_vertex] + v.second) {
                vertices_queue.push(v.first);
                distance[v.first] = distance[top_vertex] + v.second;
            }
    }
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph,
                    unsigned short &num_vertices,
                    unsigned &num_edges,
                    T &starting_vertex,
                    T &ending_vertex)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cin >> num_vertices >> num_edges >> starting_vertex >> ending_vertex;
    --starting_vertex, --ending_vertex; // We use numbering from 0
    graph = std::make_unique<ListGraph<T>>(num_vertices);
}

template<typename T>
void adding_edge(std::unique_ptr<IGraph<T>> &graph, unsigned num_edges)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    for (unsigned i{0}; i < num_edges; ++i) {
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

    std::cout.tie(0);
    if (distance[ending_vertex] == std::numeric_limits<T>::max())
        std::cout << -1;
    else
        std::cout << distance[ending_vertex];
}

int main()
{
    unsigned short num_vertices{0};
    unsigned num_edges{0}, starting_vertex{0}, ending_vertex{0};
    std::unique_ptr<IGraph<unsigned>> graph;

    initialization(graph, num_vertices, num_edges, starting_vertex, ending_vertex);

    adding_edge(graph, num_edges);

    get_shortest_way(graph, starting_vertex, ending_vertex);

    return 0;
}
