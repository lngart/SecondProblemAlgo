#include <iostream>
#include <limits>
#include <memory>
#include <stack>
#include <vector>
#include <queue>

template<typename T>
struct Operations
{
    Operations() {}

    T increase_first_digit(T number)
    {
        if (number / digit_capacity != 9)
            return number + digit_capacity;
        return number;
    }

    T decrease_number(T number)
    {
        if (number % 10 != 1)
            return number - 1;
        return number;
    }

    T cyclic_shift_right(T number) { return (number % 10) * digit_capacity + number / 10; }

    T cyclic_shift_left(T number)
    {
        return (number % digit_capacity) * 10 + number / digit_capacity;
    }

private:
    const unsigned short digit_capacity = 1000;
};

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
void apply_operations(std::unique_ptr<IGraph<T>> &graph, T number)
{
    Operations<T> operations;
    graph->add_edge(number, operations.increase_first_digit(number));
    graph->add_edge(number, operations.decrease_number(number));
    graph->add_edge(number, operations.cyclic_shift_right(number));
    graph->add_edge(number, operations.cyclic_shift_left(number));
}

template<typename T>
void bfs(std::unique_ptr<IGraph<T>> &graph,
         T starting_vertex,
         T ending_vertex,
         std::vector<T> &parents)
{
    std::vector<Color> colors(graph->size(), Color::white);
    std::queue<T> vertices_queue;
    vertices_queue.push(starting_vertex);
    colors[starting_vertex] = Color::black;

    while (!vertices_queue.empty()) {
        auto top_vertex = vertices_queue.front();
        vertices_queue.pop();

        if (top_vertex == ending_vertex)
            break;

        apply_operations(graph, top_vertex);
        std::vector<T> neighbors = graph->get_neighbors(top_vertex);

        for (unsigned short i{0}; i < neighbors.size(); ++i)
            if (colors[neighbors[i]] == Color::white) {
                colors[neighbors[i]] = Color::black;
                parents[neighbors[i]] = top_vertex;
                vertices_queue.push(neighbors[i]);
            }
    }
}

template<typename T>
void get_way(std::unique_ptr<IGraph<T>> &graph, T starting_number, T ending_number)
{
    T number = ending_number;

    std::vector<T> parents(graph->size(), std::numeric_limits<T>::max());

    bfs(graph, starting_number, ending_number, parents);

    std::stack<T> way;
    while (number != std::numeric_limits<T>::max()) {
        way.push(number);
        number = parents[number];
    }

    std::cout << way.size() << std::endl;
    while (!way.empty()) {
        std::cout << way.top() << std::endl;
        way.pop();
    }
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph,
                    size_t num_vertices,
                    T &starting_vertex,
                    T &ending_vertex)
{
    graph = std::make_unique<ListGraph<int>>(num_vertices);
    std::cin >> starting_vertex >> ending_vertex;
}

int main()
{
    const unsigned short max_number = 10000;
    std::unique_ptr<IGraph<int>> numbers;
    int starting_number{0}, ending_number{0};

    initialization(numbers, max_number, starting_number, ending_number);

    get_way(numbers, starting_number, ending_number);

    return 0;
}
