#include <iostream>
#include <memory>
#include <utility>
#include <vector>

template<typename T>
class IGraph
{
public:
    using vertex = T;

    virtual const std::vector<std::pair<vertex, T>> &get_neighbors(vertex v) const = 0;
    virtual void add_edge(vertex from, vertex to, T weight) = 0;
    virtual inline size_t size() const = 0;
    virtual ~IGraph() {}
};

template<typename T>
class ListGraph : public IGraph<T>
{
    using typename IGraph<T>::vertex;

    std::vector<std::vector<std::pair<vertex, T>>> vertices;

public:
    ListGraph(size_t num_vertices) : vertices(num_vertices) {}

    const std::vector<std::pair<vertex, T>> &get_neighbors(vertex v) const override
    {
        return vertices[v];
    }

    void add_edge(vertex from, vertex to, T weight) override
    {
        vertices[from].push_back({to, weight});
        vertices[to].push_back({from, weight});
    }

    inline size_t size() const override { return vertices.size(); }
};

template<typename T>
void dfs(T current_vertex,
         T value,
         bool &is_permutation,
         std::unique_ptr<IGraph<T>> &graph,
         std::vector<T> &labels,
         std::vector<T> &is_valid,
         std::vector<T> &vertices_permutation)
{
    labels[current_vertex] = value;

    for (const auto &neighbor : graph->get_neighbors(current_vertex)) {
        auto next_vertex = neighbor.first, weight = neighbor.second;
        if (labels[next_vertex] != value) {
            vertices_permutation[next_vertex] = weight - vertices_permutation[current_vertex];

            if (vertices_permutation[next_vertex] < 0
                || vertices_permutation[next_vertex] > graph->size() - 1
                || is_valid[vertices_permutation[next_vertex]] == value) {
                is_permutation = false;
                return;
            }

            is_valid[vertices_permutation[next_vertex]] = value;

            dfs(next_vertex, value, is_permutation, graph, labels, is_valid, vertices_permutation);

            if (!is_permutation)
                return;
        } else if (weight - vertices_permutation[current_vertex]
                   != vertices_permutation[next_vertex]) {
            is_permutation = false;
            return;
        }
    }

    is_permutation = true;
}

template<typename T>
bool find_vertices_permutation(T value,
                               std::unique_ptr<IGraph<T>> &graph,
                               std::vector<T> &vertices_permutation,
                               std::vector<T> &labels,
                               std::vector<T> &is_valid)
{
    vertices_permutation[0] = value;

    bool is_permutation{false};

    dfs(0, value, is_permutation, graph, labels, is_valid, vertices_permutation);

    return is_permutation;
}

template<typename T>
void get_vertices_permutation(std::unique_ptr<IGraph<T>> &graph)
{
    std::vector<T> vertices_permutation(graph->size());
    std::vector<T> labels(graph->size());
    std::vector<T> is_valid(graph->size());

    for (T vertex{0}; vertex < graph->size(); ++vertex)
        if (find_vertices_permutation(vertex, graph, vertices_permutation, labels, is_valid))
            break;

    std::cout.tie(0);
    for (size_t i{0}; i < vertices_permutation.size(); ++i)
        std::cout << vertices_permutation[i] + 1 << " "; // We use numbering from 0
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
        T from{0}, to{0}, weight{0};
        std::cin >> from >> to >> weight;
        graph->add_edge(from - 1, to - 1, weight - 2); // We use numbering from 0
    }
}

int main()
{
    size_t num_vertices{0}, num_edges{0};
    std::unique_ptr<IGraph<int>> graph;

    initialization(graph, num_vertices, num_edges);

    adding_edge(graph, num_edges);

    get_vertices_permutation(graph);

    return 0;
}
