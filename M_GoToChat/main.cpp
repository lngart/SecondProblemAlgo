#include <iostream>
#include <limits>
#include <memory>
#include <unordered_map>
#include <unordered_set>
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

enum class Color { white, black };

template<typename T>
void bfs(std::unique_ptr<IGraph<T>> &graph,
         std::vector<T> &connectivity_components,
         size_t &num_connectivity_components)
{
    std::vector<Color> colors(graph->size(), Color::white);
    size_t current_connectivity_components{0};

    for (size_t vertex{0}; vertex < graph->size(); ++vertex) {
        if (colors[vertex] == Color::white) {
            ++current_connectivity_components;

            std::queue<T> vertices_queue;
            vertices_queue.push(vertex);

            colors[vertex] = Color::black;

            while (!vertices_queue.empty()) {
                auto top_vertex = vertices_queue.front();
                vertices_queue.pop();

                connectivity_components[top_vertex] = current_connectivity_components;

                auto neighbors = graph->get_neighbors(top_vertex);
                for (const auto &v : neighbors)
                    if (colors[v] == Color::white) {
                        vertices_queue.push(v);
                        colors[v] = Color::black;
                    }
            }
        }
    }

    num_connectivity_components = current_connectivity_components;
}

template<typename T>
void get_connectivity_components(std::unique_ptr<IGraph<T>> &graph)
{
    std::vector<T> connectivity_components(graph->size());
    size_t num_connectivity_components{0};

    bfs(graph, connectivity_components, num_connectivity_components);

    std::cout.tie(0);
    std::cout << num_connectivity_components << std::endl;

    std::unordered_map<T, std::unordered_set<T>> vertices_in_connectivity_component;
    for (size_t vertex{0}; vertex < graph->size(); ++vertex)
        vertices_in_connectivity_component[connectivity_components[vertex]].insert(vertex);

    for (const auto &component : vertices_in_connectivity_component) {
        std::cout << component.second.size() << std::endl;
        for (const auto &v : component.second)
            std::cout << v + 1 << " ";

        std::cout << std::endl;
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
    size_t num_freshmen{0}, num_pairs_freshmen{0};
    std::unique_ptr<IGraph<size_t>> chats;

    initialization(chats, num_freshmen, num_pairs_freshmen);

    adding_edge(chats, num_pairs_freshmen);

    get_connectivity_components(chats);

    return 0;
}
