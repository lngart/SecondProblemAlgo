#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
#include <queue>

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
        vertices.at(to).push_back(from);
    }

    inline size_t size() const override {
        return vertices.size();
    }
};

struct Cell {
    Cell(int x_coordinate, int y_coordinate) : first_coordinate{x_coordinate}, second_coordinate{y_coordinate} {
    }

    int first_coordinate;
    int second_coordinate;

    Cell &operator--() {
        --first_coordinate, --second_coordinate;
        return *this;
    }
};

std::istream &operator>>(std::istream &in, Cell &cell) {
    in >> cell.first_coordinate >> cell.second_coordinate;
    return in;
}

struct Move {
    static const std::vector<Cell> moves;
};

const std::vector<Cell> Move::moves = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};

class Board {
    int num_cells_along_side;

public:
    Board(int board_num_cells_along_side) : num_cells_along_side(board_num_cells_along_side) {
    }

    bool cell_is_in_board(Cell cell) const {
        return (cell.first_coordinate >= 0 && cell.second_coordinate >= 0 &&
                cell.first_coordinate < num_cells_along_side && cell.second_coordinate < num_cells_along_side);
    }

    int convert_one_dimensional_coordinates(Cell cell) const {
        return cell.first_coordinate * num_cells_along_side + cell.second_coordinate;
    }

    size_t get_num_cells_along_side() const {
        return num_cells_along_side;
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
void print_shortest_way(IGraph<T> &graph, size_t num_cells_along_side, T starting_vertex, T ending_vertex) {
    std::vector<T> vertices_on_way;

    find_shortest_way(graph, vertices_on_way, starting_vertex, ending_vertex);

    std::cout << vertices_on_way.size() - 1 << '\n';

    for (const auto &vertex : vertices_on_way) {
        auto first_coordinate = vertex / num_cells_along_side, second_coordinate = vertex % num_cells_along_side;
        std::cout << first_coordinate + 1 << " " << second_coordinate + 1 << '\n';
    }
}

void initialization(size_t &num_cells_along_side, Cell &starting_cell, Cell &ending_cell) {
    std::cin >> num_cells_along_side >> starting_cell >> ending_cell;
    --starting_cell, --ending_cell;
}

void get_first_and_last_moves(int &first_move, int &last_move, Cell &starting_cell, Cell &ending_cell,
                              Board &chess_field) {
    first_move = chess_field.convert_one_dimensional_coordinates(starting_cell);
    last_move = chess_field.convert_one_dimensional_coordinates(ending_cell);
}

template <typename T>
void construct_knight_moves(IGraph<T> &knight_moves, Board &chess_field) {
    for (size_t first_coordinate{0}; first_coordinate < chess_field.get_num_cells_along_side(); ++first_coordinate)
        for (size_t second_coordinate{0}; second_coordinate < chess_field.get_num_cells_along_side();
             ++second_coordinate)
            for (const auto &move : Move::moves) {
                auto current_cell = Cell(first_coordinate, second_coordinate),
                     next_cell =
                         Cell(first_coordinate + move.first_coordinate, second_coordinate + move.second_coordinate);

                if (chess_field.cell_is_in_board(next_cell)) {
                    T from = chess_field.convert_one_dimensional_coordinates(current_cell),
                      to = chess_field.convert_one_dimensional_coordinates(next_cell);

                    knight_moves.add_edge(from, to);
                }
            }
}

int main() {
    size_t num_cells_along_side{0};
    Cell starting_cell{0, 0}, ending_cell{0, 0};
    int first_move{0}, last_move{0};

    initialization(num_cells_along_side, starting_cell, ending_cell);

    ListGraph<int> knight_moves(num_cells_along_side * num_cells_along_side);
    Board chess_field(num_cells_along_side);

    construct_knight_moves(knight_moves, chess_field);

    get_first_and_last_moves(first_move, last_move, starting_cell, ending_cell, chess_field);

    print_shortest_way(knight_moves, num_cells_along_side, first_move, last_move);

    return 0;
}
