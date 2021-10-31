#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

template<typename T>
class IGraph
{
public:
    using vertex = T;

    virtual const std::vector<std::pair<vertex, size_t>> &get_neighbors(vertex v) const = 0;
    virtual void add_edge(vertex from, vertex to, size_t number_of_edge) = 0;
    virtual size_t get_num_edges() const = 0;
    virtual size_t get_incoming_degree_vertex(vertex v) const = 0;
    virtual size_t size() const = 0;
    virtual ~IGraph() {}
};

template<typename T>
class ListGraph : public IGraph<T>
{
    using typename IGraph<T>::vertex;

    std::vector<std::vector<std::pair<vertex, size_t>>> vertices;
    std::vector<size_t> incoming_degree_vertices;

public:
    ListGraph(size_t num_vertices) : vertices(num_vertices), incoming_degree_vertices(num_vertices)
    {}

    const std::vector<std::pair<vertex, size_t>> &get_neighbors(vertex v) const override
    {
        return vertices[v];
    }

    void add_edge(vertex from, vertex to, size_t number_of_edge) override
    {
        vertices[from].push_back({to, number_of_edge});
        ++incoming_degree_vertices[to];
    }

    size_t get_num_edges() const override
    {
        size_t num_edges{0};
        for (const auto &neighbor : vertices)
            num_edges += neighbor.size();

        return num_edges;
    }

    size_t get_incoming_degree_vertex(vertex v) const override
    {
        return incoming_degree_vertices[v];
    }

    inline size_t size() const override { return vertices.size(); }
};

enum class Color { white, black };

template<typename T>
void dfs(std::unique_ptr<IGraph<T>> &graph, T vertex, std::vector<Color> &colors)
{
    colors[vertex] = Color::black;
    for (const auto &neighbor : graph->get_neighbors(vertex))
        if (colors[neighbor.first] == Color::white)
            dfs(graph, neighbor.first, colors);
}

template<typename T>
bool is_euler_graph(std::unique_ptr<IGraph<T>> &graph)
{
    std::vector<Color> colors(graph->size(), Color::white);

    for (T vertex{0}; vertex < graph->size(); ++vertex) {
        const auto &neighbor = graph->get_neighbors(vertex);
        if (!neighbor.empty()) {
            dfs(graph, vertex, colors);
            break;
        }
    }

    for (T vertex{0}; vertex < graph->size(); ++vertex) {
        auto deg_in = graph->get_incoming_degree_vertex(vertex);
        if (deg_in != 0 && colors[vertex] == Color::white)
            return false;
    }

    return true;
}

template<typename T>
void find_euler_cycle(std::unique_ptr<IGraph<T>> &graph,
                      T vertex,
                      std::vector<size_t> &number_of_first_not_passed_edge,
                      std::vector<Color> &colors,
                      std::vector<T> &euler_cycle)
{
    const auto &neighbor = graph->get_neighbors(vertex);
    while (number_of_first_not_passed_edge[vertex] < neighbor.size()) {
        auto tmp_idx = number_of_first_not_passed_edge[vertex];
        ++number_of_first_not_passed_edge[vertex];

        auto ending_vertex_first_not_passed_edge = neighbor[tmp_idx].first,
             idx_of_first_not_passed_edge = neighbor[tmp_idx].second - 1;

        if (colors[idx_of_first_not_passed_edge] == Color::white) {
            colors[idx_of_first_not_passed_edge] = Color::black;
            find_euler_cycle(graph,
                             ending_vertex_first_not_passed_edge,
                             number_of_first_not_passed_edge,
                             colors,
                             euler_cycle);
            euler_cycle.push_back(ending_vertex_first_not_passed_edge);
        }
    }
}

template<typename T>
void get_euler_cycle(std::unique_ptr<IGraph<T>> &graph)
{
    std::cout.tie(0);

    std::vector<size_t> number_of_first_not_passed_edge(graph->size());
    std::vector<Color> colors(graph->get_num_edges(), Color::white);
    std::vector<T> euler_cycle;

    for (T vertex{0}; vertex < graph->size(); ++vertex)
        find_euler_cycle(graph, vertex, number_of_first_not_passed_edge, colors, euler_cycle);

    if (!euler_cycle.empty()) {
        euler_cycle.push_back(euler_cycle[0]);

        std::cout << euler_cycle.size() << " ";

        for (int i = euler_cycle.size() - 1; i >= 0; --i)
            std::cout << euler_cycle[i] + 1 << " ";
    } else
        std::cout << 0;
}

template<typename T>
void get_num_squares_in_route(std::unique_ptr<IGraph<T>> &routes)
{
    if (is_euler_graph(routes))
        get_euler_cycle(routes);
    else
        std::cout << 0 << std::endl;
}

template<typename T>
void initialization(std::unique_ptr<IGraph<T>> &graph, size_t &num_requests, size_t &num_vertices)
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    std::cin >> num_requests >> num_vertices;
    graph = std::make_unique<ListGraph<T>>(num_vertices);
}

template<typename T>
void build_routes(std::unique_ptr<IGraph<T>> &routes, size_t num_requests)
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    size_t cnt_edges{0};
    for (size_t i{0}; i < num_requests; ++i) {
        size_t num_stops{0};
        std::cin >> num_stops;

        T from{0};
        std::cin >> from;

        for (size_t i{1}; i < num_stops + 1; ++i) {
            ++cnt_edges;

            T to{0};
            std::cin >> to;

            routes->add_edge(from - 1, to - 1, cnt_edges); // We use numbering from 0

            from = to;
        }
    }
}

int main()
{
    size_t num_bus_routes{0}, num_squares{0};
    std::unique_ptr<IGraph<int>> routes;

    initialization(routes, num_bus_routes, num_squares);

    build_routes(routes, num_bus_routes);

    get_num_squares_in_route(routes);

    return 0;
}
