#include <ios>
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

    virtual void add_vertex(size_t first_coordinate, size_t second_coordinate, vertex value) = 0;
    virtual vertex get_vertex(size_t first_coordinate, size_t second_coordinate) const = 0;
    virtual std::pair<size_t, size_t> size() const = 0;
    virtual ~IGraph() {}
};

template<typename T>
class MatrixGraph : public IGraph<T>
{
    using typename IGraph<T>::vertex;

    std::vector<std::vector<vertex>> vertices;

public:
    MatrixGraph(size_t height, size_t length) : vertices(height, std::vector<vertex>(length)) {}

    vertex get_vertex(size_t first_coordinate, size_t second_coordinate) const override
    {
        return vertices.at(first_coordinate).at(second_coordinate);
    }

    void add_vertex(size_t first_coordinate, size_t second_coordinate, vertex value) override
    {
        vertices.at(first_coordinate).at(second_coordinate) = value;
    }

    std::pair<size_t, size_t> size() const override
    {
        return {vertices.size(), vertices.at(0).size()};
    }
};

template<typename T>
void get_distance_to_nearest_sub(std::unique_ptr<IGraph<T>> &graph)
{
    auto [height, length] = graph->size();

    std::vector<std::vector<T>> distance(height,
                                         std::vector<T>(length, std::numeric_limits<T>::max()));

    std::vector<std::pair<short, short>> shift_x_y = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    std::queue<std::pair<T, T>> queue_vertices;

    for (size_t i{0}; i < height; ++i)
        for (size_t j{0}; j < length; ++j)
            if (graph->get_vertex(i, j) == 1) {
                distance[i][j] = 0;
                queue_vertices.push({i, j});
            }

    while (!queue_vertices.empty()) {
        auto [current_x, current_y] = queue_vertices.front();
        queue_vertices.pop();

        for (unsigned short i{0}; i < shift_x_y.size(); ++i) {
            std::pair<T, T> neighbor = {current_x + shift_x_y[i].first,
                                        current_y + shift_x_y[i].second};
            if (neighbor.first >= 0 && neighbor.first < height && neighbor.second >= 0
                && neighbor.second < length
                && distance[neighbor.first][neighbor.second] > distance[current_x][current_y] + 1) {
                distance[neighbor.first][neighbor.second] = distance[current_x][current_y] + 1;
                queue_vertices.push({neighbor.first, neighbor.second});
            }
        }
    }

    for (const auto &row : distance) {
        for (const auto &elem : row)
            std::cout << elem << " ";
        std::cout << std::endl;
    }
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph, size_t &height, size_t &length)
{
    std::cin >> height >> length;
    graph = std::make_unique<MatrixGraph<T>>(height, length);

    for (size_t i{0}; i < height; ++i)
        for (size_t j{0}; j < length; ++j) {
            T value{0};
            std::cin >> value;
            graph->add_vertex(i, j, value);
        }
}

int main()
{
    std::ios_base::sync_with_stdio(false);

    size_t height{0}, length{0};
    std::unique_ptr<IGraph<size_t>> graph;
    initialization(graph, height, length);

    get_distance_to_nearest_sub(graph);

    return 0;
}
