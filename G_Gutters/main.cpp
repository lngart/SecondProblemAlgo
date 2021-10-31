#include <iostream>
#include <limits>
#include <memory>
#include <unordered_map>
#include <vector>

template<typename T>
class IGraph
{
public:
    using vertex = T;

    virtual const std::vector<vertex> &get_neighbors(vertex v) = 0;
    virtual void add_edge(vertex from, vertex to) = 0;
    virtual size_t size() const = 0;
    virtual ~IGraph() {}
};

template<typename T>
class ListGraph : public IGraph<T>
{
    using typename IGraph<T>::vertex;

    std::unordered_map<vertex, std::vector<vertex>> vertices;

public:
    ListGraph(size_t num_vertices) : vertices(num_vertices) {}

    const std::vector<vertex> &get_neighbors(vertex v) override { return vertices[v]; }

    void add_edge(vertex from, vertex to) override { vertices[from].push_back(to); }

    inline size_t size() const override { return vertices.size(); }
};

inline bool is_valid_coordinates(int first_coordinate,
                                 int max_first_coordinate,
                                 int second_coordinate,
                                 int max_second_coordinate)
{
    return 0 <= first_coordinate && first_coordinate <= max_first_coordinate - 1
           && 0 <= second_coordinate && second_coordinate <= max_second_coordinate - 1;
}

template<typename T>
void find_connected_areas(std::vector<std::vector<T>> &local_map,
                          std::vector<std::vector<T>> &number_of_connected_area,
                          size_t cnt_connected_area,
                          int first_coordinate,
                          int second_coordinate)
{
    int map_length = number_of_connected_area.size(),
        map_width = number_of_connected_area[0].size();

    if (number_of_connected_area[first_coordinate][second_coordinate]
        != std::numeric_limits<T>::max())
        return;
    number_of_connected_area[first_coordinate][second_coordinate] = cnt_connected_area;

    if (is_valid_coordinates(first_coordinate - 1, map_length, second_coordinate, map_width)
        && local_map[first_coordinate][second_coordinate]
               == local_map[first_coordinate - 1][second_coordinate])
        find_connected_areas(local_map,
                             number_of_connected_area,
                             cnt_connected_area,
                             first_coordinate - 1,
                             second_coordinate);

    if (is_valid_coordinates(first_coordinate + 1, map_length, second_coordinate, map_width)
        && local_map[first_coordinate][second_coordinate]
               == local_map[first_coordinate + 1][second_coordinate])
        find_connected_areas(local_map,
                             number_of_connected_area,
                             cnt_connected_area,
                             first_coordinate + 1,
                             second_coordinate);

    if (is_valid_coordinates(first_coordinate, map_length, second_coordinate - 1, map_width)
        && local_map[first_coordinate][second_coordinate]
               == local_map[first_coordinate][second_coordinate - 1])
        find_connected_areas(local_map,
                             number_of_connected_area,
                             cnt_connected_area,
                             first_coordinate,
                             second_coordinate - 1);

    if (is_valid_coordinates(first_coordinate, map_length, second_coordinate + 1, map_width)
        && local_map[first_coordinate][second_coordinate]
               == local_map[first_coordinate][second_coordinate + 1])
        find_connected_areas(local_map,
                             number_of_connected_area,
                             cnt_connected_area,
                             first_coordinate,
                             second_coordinate + 1);
}

template<typename T>
void get_num_gutters(std::vector<std::vector<T>> &local_map)
{
    int map_length = local_map.size(), map_width = local_map[0].size();

    std::vector<std::vector<T>>
        number_of_connected_area(map_length,
                                 std::vector<T>(map_width, std::numeric_limits<T>::max()));

    size_t cnt_connected_area{0};
    for (int i{0}; i < map_length; ++i)
        for (int j{0}; j < map_width; ++j)
            if (number_of_connected_area[i][j] == std::numeric_limits<T>::max()) {
                ++cnt_connected_area;
                find_connected_areas(local_map, number_of_connected_area, cnt_connected_area, i, j);
            }

    std::unique_ptr<IGraph<T>> graph = std::make_unique<ListGraph<T>>(cnt_connected_area);
    for (int i{0}; i < map_length; ++i)
        for (int j{0}; j < map_width; ++j) {
            if (is_valid_coordinates(i - 1, map_length, j, map_width)
                && local_map[i - 1][j] < local_map[i][j])
                graph->add_edge(number_of_connected_area[i][j] - 1,
                                number_of_connected_area[i - 1][j] - 1);

            if (is_valid_coordinates(i + 1, map_length, j, map_width)
                && local_map[i + 1][j] < local_map[i][j])
                graph->add_edge(number_of_connected_area[i][j] - 1,
                                number_of_connected_area[i + 1][j] - 1);

            if (is_valid_coordinates(i, map_length, j - 1, map_width)
                && local_map[i][j - 1] < local_map[i][j])
                graph->add_edge(number_of_connected_area[i][j] - 1,
                                number_of_connected_area[i][j - 1] - 1);

            if (is_valid_coordinates(i, map_length, j + 1, map_width)
                && local_map[i][j + 1] < local_map[i][j])
                graph->add_edge(number_of_connected_area[i][j] - 1,
                                number_of_connected_area[i][j + 1] - 1);
        }

    size_t num_connected_area{0};
    for (size_t i{0}; i < cnt_connected_area; ++i)
        if (graph->get_neighbors(i).empty())
            ++num_connected_area;

    std::cout << num_connected_area;
}

template<typename T>
inline void initialization(std::vector<std::vector<T>> &local_map,
                           size_t &map_length,
                           size_t &map_width)
{
    std::cin >> map_length >> map_width;

    local_map.resize(map_length, std::vector<T>(map_width));
    for (size_t i{0}; i < map_length; ++i)
        for (size_t j{0}; j < map_width; ++j)
            std::cin >> local_map[i][j];
}

int main()
{
    size_t map_length{0}, map_width{0};
    std::vector<std::vector<int>> local_map;

    initialization(local_map, map_length, map_width);

    get_num_gutters(local_map);

    return 0;
}
