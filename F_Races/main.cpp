#include <iostream>
#include <limits>
#include <memory>
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

    void add_edge(vertex from, vertex to) override { vertices.at(from).push_back(to); }

    inline size_t size() const override { return vertices.size(); }
};

template<typename T>
void bfs(std::unique_ptr<IGraph<T>> &graph, T from, std::vector<T> &distance)
{
    distance[from] = 0;

    std::queue<T> vertices_queue;
    vertices_queue.push(from);
    while (!vertices_queue.empty()) {
        auto top_vertex = vertices_queue.front();
        vertices_queue.pop();
        auto neighbors = graph->get_neighbors(top_vertex);
        for (const auto &v : neighbors)
            if (distance[v] == std::numeric_limits<T>::max()) {
                vertices_queue.push(v);
                distance[v] = distance[top_vertex] + 1;
            }
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

    std::cout << (distance[ending_vertex] == std::numeric_limits<T>::max()
                      ? -1
                      : distance[ending_vertex]);
}

template<typename T>
void create_track(std::unique_ptr<IGraph<T>> &track, std::vector<std::vector<char>> &appearance)
{
    size_t length = appearance.size(), width = appearance[0].size();
    int wall_position{0};

    for (size_t line{0}; line < length; ++line) {
        wall_position = -1;
        for (size_t position_in_line{0}; position_in_line < width; ++position_in_line) {
            if (appearance[line][position_in_line] == '#')
                wall_position = position_in_line;
            else {
                auto current_position = line * width + position_in_line,
                     next_position = line * width + ((wall_position + position_in_line + 1) >> 1);
                track->add_edge(current_position, next_position);
            }
        }
    }

    for (size_t line{0}; line < length; ++line) {
        wall_position = width;
        for (int position_in_line = width - 1; position_in_line >= 0; --position_in_line) {
            if (appearance[line][position_in_line] == '#')
                wall_position = position_in_line;
            else {
                auto current_position = line * width + position_in_line,
                     next_position = line * width + ((position_in_line + wall_position) >> 1);
                track->add_edge(current_position, next_position);
            }
        }
    }

    for (size_t column{0}; column < width; ++column) {
        wall_position = -1;
        for (size_t position_in_column{0}; position_in_column < length; ++position_in_column) {
            if (appearance[position_in_column][column] == '#')
                wall_position = position_in_column;
            else {
                auto current_position = position_in_column * width + column,
                     next_position = column
                                     + width * ((wall_position + position_in_column + 1) >> 1);
                track->add_edge(current_position, next_position);
            }
        }
    }

    for (size_t column{0}; column < width; ++column) {
        wall_position = length;
        for (int position_in_column = length - 1; position_in_column >= 0; --position_in_column) {
            if (appearance[position_in_column][column] == '#')
                wall_position = position_in_column;
            else {
                auto current_position = position_in_column * width + column,
                     next_position = column + width * ((wall_position + position_in_column) >> 1);
                track->add_edge(current_position, next_position);
            }
        }
    }
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph,
                    size_t &length,
                    size_t &width,
                    std::vector<std::vector<char>> &field)
{
    std::cin >> length >> width;
    graph = std::make_unique<ListGraph<T>>(length * width);
    field.resize(length, std::vector<char>(width));
}

template<typename T>
void filling_field(std::vector<std::vector<char>> &field, T &starting_cell, T &finishing_cell)
{
    auto length = field.size(), width = field[0].size();
    for (size_t i{0}; i < length; ++i)
        for (size_t j{0}; j < width; ++j) {
            std::cin >> field[i][j];
            if (field[i][j] == 'S')
                starting_cell = i * width + j;
            else if (field[i][j] == 'T')
                finishing_cell = i * width + j;
        }
}

int main()
{
    size_t length_track{0}, width_track{0};
    std::unique_ptr<IGraph<int>> track;

    int start_cell{0}, finish_cell{0};
    std::vector<std::vector<char>> appearance_track;

    initialization(track, length_track, width_track, appearance_track);
    filling_field(appearance_track, start_cell, finish_cell);
    create_track(track, appearance_track);

    get_shortest_way(track, start_cell, finish_cell);

    return 0;
}
