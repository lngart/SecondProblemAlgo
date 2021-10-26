#include <iostream>
#include <limits>
#include <memory>
#include <stack>
#include <utility>
#include <vector>
#include <queue>

template<typename T>
class IGraph
{
public:
    using vertex = T;

    virtual void add_vertex(size_t first_coordinate, size_t second_coordinate, vertex value) = 0;
    virtual vertex get_vertex(size_t first_coordinate, size_t second_coordinate) const = 0;
    virtual size_t size() const = 0;
    virtual ~IGraph() {}
};

template<typename T>
class MatrixGraph : public IGraph<T>
{
    using typename IGraph<T>::vertex;

    std::vector<std::vector<vertex>> distance;

public:
    MatrixGraph(size_t num_vertices)
        : distance(num_vertices,
                   std::vector<vertex>(num_vertices, std::numeric_limits<vertex>::max()))
    {}

    vertex get_vertex(size_t first_coordinate, size_t second_coordinate) const override
    {
        return distance.at(first_coordinate).at(second_coordinate);
    }

    void add_vertex(size_t first_coordinate, size_t second_coordinate, vertex value) override
    {
        distance.at(first_coordinate).at(second_coordinate) = value;
    }

    inline size_t size() const override { return distance.size(); }
};

template<typename T>
void find_shortest_way(std::unique_ptr<IGraph<T>> &graph,
                       T starting_x,
                       T starting_y,
                       std::vector<std::vector<std::pair<T, T>>> &parents)
{
    auto graph_size = graph->size();

    std::vector<std::pair<short, short>> shift_x_y
        = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};

    std::queue<std::pair<T, T>> queue_vertices;
    graph->add_vertex(starting_x, starting_y, 0);

    queue_vertices.push({starting_x, starting_y});
    while (!queue_vertices.empty()) {
        auto [first_coordinate, second_coordinate] = queue_vertices.front();
        queue_vertices.pop();
        for (size_t i{0}; i < shift_x_y.size(); ++i) {
            auto current_x = first_coordinate + shift_x_y[i].first,
                 current_y = second_coordinate + shift_x_y[i].second;

            if (current_x >= 0 && current_x < graph_size && current_y >= 0 && current_y < graph_size
                && graph->get_vertex(current_x, current_y)
                       > graph->get_vertex(first_coordinate, second_coordinate) + 1) {
                graph->add_vertex(current_x,
                                  current_y,
                                  graph->get_vertex(first_coordinate, second_coordinate) + 1);
                queue_vertices.push({current_x, current_y});

                parents[current_x][current_y] = {first_coordinate, second_coordinate};
            }
        }
    }
}

template<typename T>
void get_shortest_way(
    std::unique_ptr<IGraph<T>> &graph, T starting_x, T starting_y, T ending_x, T ending_y)
{
    auto graph_size = graph->size();
    std::vector<std::vector<std::pair<T, T>>>
        parents(graph_size,
                std::vector<std::pair<T, T>>(graph_size,
                                             {std::numeric_limits<T>::max(),
                                              std::numeric_limits<T>::max()}));

    find_shortest_way(graph, starting_x, starting_y, parents);
    std::cout << graph->get_vertex(ending_x, ending_y) << std::endl;

    std::stack<std::pair<T, T>> way;
    way.push({ending_x + 1, ending_y + 1});

    for (T current_x{ending_x}, current_y{ending_y};
         current_x != starting_x || current_y != starting_y;) {
        way.push(
            {parents[current_x][current_y].first + 1, parents[current_x][current_y].second + 1});
        T tmp_x = current_x, tmp_y = current_y;
        current_x = parents[tmp_x][tmp_y].first, current_y = parents[tmp_x][tmp_y].second;
    }

    while (!way.empty()) {
        std::cout << way.top().first << " " << way.top().second << std::endl;
        way.pop();
    }
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph,
                    size_t &num_vertices,
                    T &starting_x,
                    T &starting_y,
                    T &ending_x,
                    T &ending_y)
{
    std::cin >> num_vertices >> starting_x >> starting_y >> ending_x >> ending_y;
    --starting_x, --starting_y, --ending_x, --ending_y;
    graph = std::make_unique<MatrixGraph<T>>(num_vertices);
}

int main()
{
    size_t num_cells{0};
    int starting_x{0}, starting_y{0}, ending_x{0}, ending_y{0};
    std::unique_ptr<IGraph<int>> chess_field;

    initialization(chess_field, num_cells, starting_x, starting_y, ending_x, ending_y);

    get_shortest_way(chess_field, starting_x, starting_y, ending_x, ending_y);

    return 0;
}
