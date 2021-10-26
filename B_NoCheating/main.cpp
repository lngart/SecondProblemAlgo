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

enum class Color { white, red, blue };

template<typename T>
bool is_bipartite(std::unique_ptr<IGraph<T>> &graph, std::vector<Color> &colors, T starting_vertex)
{
    colors[starting_vertex] = Color::red;

    std::queue<T> vertices_queue;
    vertices_queue.push(starting_vertex);
    while (!vertices_queue.empty()) {
        auto top_vertex = vertices_queue.front();
        vertices_queue.pop();
        auto neighbors = graph->get_neighbors(top_vertex);
        for (const auto &v : neighbors) {
            if (colors[top_vertex] == Color::red) {
                if (colors[v] == Color::white) {
                    vertices_queue.push(v);
                    colors[v] = Color::blue;
                } else if (colors[v] == Color::red)
                    return false;
            } else if (colors[top_vertex] == Color::blue) {
                if (colors[v] == Color::white) {
                    vertices_queue.push(v);
                    colors[v] = Color::red;
                } else if (colors[v] == Color::blue)
                    return false;
            }
        }
    }
    return true;
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph, size_t &num_vertices, size_t &num_edges)
{
    std::cin >> num_vertices >> num_edges;
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
inline void is_possible_separate_students(std::unique_ptr<IGraph<T>> &graph)
{
    bool is_possible{true};
    std::vector<Color> colors(graph->size(), Color::white);
    for (T current_vertex{0}; current_vertex < graph->size() && is_possible; ++current_vertex)
        if (colors[current_vertex] == Color::white)
            is_possible = is_possible && is_bipartite(graph, colors, current_vertex);

    std::cout << (is_possible ? "YES" : "NO");
}

int main()
{
    size_t num_students{0}, num_pairs_students{0};
    std::unique_ptr<IGraph<size_t>> students;

    initialization(students, num_students, num_pairs_students);

    adding_edge(students, num_pairs_students);

    is_possible_separate_students(students);

    return 0;
}
