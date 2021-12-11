#include <iostream>
#include <limits>
#include <utility>
#include <vector>
#include <queue>

template <typename T>
class IGraph {
public:
    using vertex = T;

    virtual const std::vector<vertex> &get_neighbors(vertex v) const = 0;
    virtual void add_edge(vertex from, vertex to, T weight = 1) = 0;
    virtual T is_weighted() const = 0;
    virtual T get_weight_edge(vertex from, vertex to) const = 0;
    virtual size_t size() const = 0;
    virtual ~IGraph() {
    }
};

template <typename T>
class ListGraph : public IGraph<T> {
    using typename IGraph<T>::vertex;
    std::vector<std::vector<vertex>> vertices;
    std::vector<std::vector<vertex>> weight_edges;

    bool is_weighted_graph;

public:
    ListGraph(size_t num_vertices, bool is_weighted)
        : vertices(num_vertices)
        , weight_edges(num_vertices, std::vector<vertex>(num_vertices))
        , is_weighted_graph{is_weighted} {
    }

    const std::vector<vertex> &get_neighbors(vertex v) const override {
        return vertices.at(v);
    }

    void add_edge(vertex from, vertex to, T weight = 1) override {
        vertices.at(from).push_back(to);
        weight_edges.at(from).at(to) = weight;
    }

    T is_weighted() const override {
        return is_weighted_graph;
    }

    T get_weight_edge(vertex from, vertex to) const override {
        return weight_edges.at(from).at(to);
    }

    inline size_t size() const override {
        return vertices.size();
    }
};

template <typename T>
void bfs(IGraph<T> &graph, T from, std::vector<T> &distance) {
    std::queue<T> vertices_queue;
    vertices_queue.push(from);

    distance[from] = 0;

    while (!vertices_queue.empty()) {
        auto top_vertex = vertices_queue.front();
        vertices_queue.pop();
        const auto &neighbors = graph.get_neighbors(top_vertex);
        for (const auto &v : neighbors)
            if (distance[v] > distance[top_vertex] + graph.get_weight_edge(top_vertex, v)) {
                vertices_queue.push(v);
                distance[v] = distance[top_vertex] + graph.get_weight_edge(top_vertex, v);
            }
    }
}

template <typename T>
T find_shortest_way(IGraph<T> &graph, T starting_vertex, T ending_vertex) {
    std::vector<T> distance(graph.size(), std::numeric_limits<T>::max());

    bfs(graph, starting_vertex, distance);

    return distance[ending_vertex] == std::numeric_limits<T>::max() ? -1 : distance[ending_vertex];
}

template <typename T>
void print_shortest_way(IGraph<T> &graph, T starting_vertex, T ending_vertex) {
    T shortest_way = find_shortest_way(graph, starting_vertex, ending_vertex);

    std::cout << shortest_way;
}

template <typename T>
void initialization(size_t &num_vertices, size_t &num_edges, T &starting_vertex, T &ending_vertex) {
    std::cin >> num_vertices >> num_edges >> starting_vertex >> ending_vertex;
    --starting_vertex, --ending_vertex;  // We use numbering from 0
}

template <typename T>
void adding_edge(IGraph<T> &graph, size_t num_edges) {
    for (size_t i{0}; i < num_edges; ++i) {
        T from{0}, to{0}, weight{0};

        std::cin >> from >> to;

        if (graph.is_weighted()) {
            std::cin >> weight;
            graph.add_edge(from - 1, to - 1, weight);  // We use numbering from 0
        } else
            graph.add_edge(from - 1, to - 1);
    }
}

int main() {
    size_t num_vertices{0}, num_edges{0};
    int starting_vertex{0}, ending_vertex{0};

    initialization(num_vertices, num_edges, starting_vertex, ending_vertex);

    ListGraph<int> graph(num_vertices, true);

    adding_edge(graph, num_edges);

    print_shortest_way(graph, starting_vertex, ending_vertex);

    return 0;
}
