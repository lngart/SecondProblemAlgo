#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

template <typename T>
class IGraph {
public:
    using vertex = T;

    virtual const std::vector<vertex> &get_neighbors(vertex v) const = 0;
    virtual void add_edge(vertex from, vertex to) = 0;
    virtual size_t size() const = 0;
    virtual ~IGraph() {
    }
};

template <typename T>
class ListGraph : public IGraph<T> {
    using typename IGraph<T>::vertex;

    std::vector<std::vector<vertex>> vertices;

public:
    ListGraph(size_t num_vertices) : vertices(num_vertices) {
    }

    const std::vector<vertex> &get_neighbors(vertex v) const override {
        return vertices[v];
    }

    void add_edge(vertex from, vertex to) override {
        vertices[from].push_back(to);
        vertices[to].push_back(from);
    }

    inline size_t size() const override {
        return vertices.size();
    }
};

enum class Color { white, black };

template <typename T>
struct articulation_points_search_data {
    articulation_points_search_data(size_t size)
        : colors(size, Color::white), time_in(size), time_up(size), current_time{0} {
    }

    std::vector<Color> &get_colors() {
        return colors;
    }

    std::vector<size_t> &get_time_in() {
        return time_in;
    }

    std::vector<size_t> &get_time_up() {
        return time_up;
    }

    size_t &get_current_time() {
        return current_time;
    }

    std::set<T> &get_set_articulation_points() {
        return set_articulation_points;
    }

    std::vector<Color> colors;
    std::vector<size_t> time_in, time_up;
    size_t current_time;
    std::set<T> set_articulation_points;
};

template <typename T>
void find_articulation_points(IGraph<T> &graph, T vertex, T parent_vertex,
                              articulation_points_search_data<T> &articulation_points) {

    auto &colors = articulation_points.get_colors(), &time_in = articulation_points.get_time_in(),
         &time_up = articulation_points.get_time_up(), &current_time = articulation_points.get_current_time(),
         &set_articulation_points = articulation_points.get_set_articulation_points();

    T num_child_vertices{0};
    colors[vertex] = Color::black;

    time_in[vertex] = current_time;
    time_up[vertex] = current_time;
    ++current_time;

    for (const auto &neighbor : graph.get_neighbors(vertex))
        if (neighbor != parent_vertex) {

            if (colors[neighbor] == Color::black)
                time_up[vertex] = std::min(time_up[vertex], time_in[neighbor]);
            else {
                find_articulation_points(graph, neighbor, vertex, articulation_points);

                time_up[vertex] = std::min(time_up[vertex], time_up[neighbor]);

                if (time_up[neighbor] >= time_in[vertex] && parent_vertex != std::numeric_limits<T>::max())
                    set_articulation_points.insert(vertex);

                ++num_child_vertices;
            }
        }

    if (parent_vertex == std::numeric_limits<T>::max() && num_child_vertices > 1)
        set_articulation_points.insert(vertex);
}

template <typename T>
void get_articulation_points(IGraph<T> &graph, std::set<T> &set_articulation_points) {
    articulation_points_search_data<T> articulation_points(graph.size());

    auto colors = articulation_points.get_colors();

    for (T vertex{0}; vertex < graph.size(); ++vertex)
        if (colors[vertex] == Color::white)
            find_articulation_points(graph, vertex, std::numeric_limits<T>::max(), articulation_points);

    auto found_set_articulation_points = articulation_points.get_set_articulation_points();

    set_articulation_points.insert(found_set_articulation_points.begin(), found_set_articulation_points.end());
}

template <typename T>
void print_articulation_points(IGraph<T> &graph) {
    std::set<T> set_articulation_points;

    get_articulation_points(graph, set_articulation_points);

    std::cout << set_articulation_points.size() << std::endl;
    for (const auto &vertex : set_articulation_points)
        std::cout << vertex + 1 << std::endl;  // We use numbering from 0
}

void initialization(size_t &num_vertices, size_t &num_edges) {
    std::cin >> num_vertices >> num_edges;
}

template <typename T>
void adding_edge(IGraph<T> &graph, size_t num_edges) {
    for (size_t i{0}; i < num_edges; ++i) {
        T from{0}, to{0};
        std::cin >> from >> to;
        graph.add_edge(from - 1, to - 1);  // We use numbering from 0
    }
}

int main() {
    size_t num_vertices{0}, num_edges{0};

    initialization(num_vertices, num_edges);

    ListGraph<size_t> graph(num_vertices);

    adding_edge(graph, num_edges);

    print_articulation_points(graph);

    return 0;
}
