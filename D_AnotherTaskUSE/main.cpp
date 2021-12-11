#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
#include <queue>

template <typename T>
struct Operations {
    Operations() {
    }

    static T increase_first_digit_if_less_nine(T number) {
        if (first_digit_is_less_nine(number))
            return number + digit_capacity;
        return number;
    }

    static T reduce_last_digit_if_more_one(T number) {
        if (last_digit_is_more_one(number))
            return number - 1;
        return number;
    }

    static T cyclic_shift_right(T number) {
        return (number % 10) * digit_capacity + number / 10;
    }

    static T cyclic_shift_left(T number) {
        return (number % digit_capacity) * 10 + number / digit_capacity;
    }

private:
    const static unsigned short digit_capacity = 1000;

    static bool first_digit_is_less_nine(T number) {
        return number / digit_capacity != 9;
    }

    static bool last_digit_is_more_one(T number) {
        return number % 10 != 1;
    }
};

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
        return vertices.at(v);
    }

    void add_edge(vertex from, vertex to) override {
        vertices.at(from).push_back(to);
    }

    inline size_t size() const override {
        return vertices.size();
    }
};

template <typename T>
void bfs(IGraph<T> &graph, T from, std::vector<T> &parent, std::vector<T> &distance) {
    distance[from] = 0;
    std::queue<T> vertices_queue;
    vertices_queue.push(from);
    while (!vertices_queue.empty()) {
        auto top_vertex = vertices_queue.front();
        vertices_queue.pop();
        const auto &neighbors = graph.get_neighbors(top_vertex);
        for (const auto &v : neighbors)
            if (distance[v] == std::numeric_limits<T>::max()) {
                vertices_queue.push(v);
                distance[v] = distance[top_vertex] + 1;
                parent[v] = top_vertex;
            }
    }
}

template <typename T>
void find_shortest_way(IGraph<T> &graph, std::vector<T> &vertices_on_way, T starting_vertex, T ending_vertex) {
    std::vector<T> parent(graph.size(), std::numeric_limits<T>::max()),
        distance(graph.size(), std::numeric_limits<T>::max());

    bfs(graph, starting_vertex, parent, distance);

    if (distance[ending_vertex] != std::numeric_limits<T>::max()) {
        auto current_vertex = ending_vertex;

        vertices_on_way.emplace_back(current_vertex);

        while (parent[current_vertex] != std::numeric_limits<T>::max()) {
            current_vertex = parent[current_vertex];
            vertices_on_way.emplace_back(current_vertex);
        }

        std::reverse(vertices_on_way.begin(), vertices_on_way.end());
    }
}

template <typename T>
void print_shortest_way(IGraph<int> &graph, T starting_number, T ending_number) {
    std::vector<T> way;

    find_shortest_way(graph, way, starting_number, ending_number);

    std::cout << way.size() << std::endl;
    for (const auto &number : way)
        std::cout << number << std::endl;
}

template <typename T>
void initialization(T &starting_vertex, T &ending_vertex) {
    std::cin >> starting_vertex >> ending_vertex;
}

template <typename T>
void build_graph_numbers(IGraph<T> &graph, const unsigned short min_number, const unsigned short max_number) {
    for (T current_number = min_number; current_number < max_number; ++current_number) {
        graph.add_edge(current_number, Operations<T>::increase_first_digit_if_less_nine(current_number));
        graph.add_edge(current_number, Operations<T>::reduce_last_digit_if_more_one(current_number));
        graph.add_edge(current_number, Operations<T>::cyclic_shift_right(current_number));
        graph.add_edge(current_number, Operations<T>::cyclic_shift_left(current_number));
    }
}

int main() {
    const unsigned short min_number = 1000, max_number = 10000;
    int starting_number{0}, ending_number{0};

    initialization(starting_number, ending_number);

    ListGraph<int> numbers(max_number);

    build_graph_numbers(numbers, min_number, max_number);

    print_shortest_way(numbers, starting_number, ending_number);

    return 0;
}
