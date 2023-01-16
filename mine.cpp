#include <iostream>
#include <vector>
#include <variant>
#include <utility>
#include <queue>
#include <sstream>
#include <stack>
#include <random>
#include <unordered_map>


// https://techdevguide.withgoogle.com/resources/former-interview-question-minesweeper/#!


enum class GameState { VICTORY, CONTINUE, FAIL };

// We use the following characters for the map:
// '-': Unexplored space.
// '.': Revealed space with no mine.
// 'x': Mine. Only shown to the user at the end of the game.
// '!': Flag on a spot without a mine.
// 'X': Mine with a flag on top. only shown to the user at the end of the game. All 'x's need to be converted to 'X's to end the game.
class Board
{
    using Coord = std::pair<int, int>;
    const int _rows;
    const int _columns;
    int _remaining_mines;
    std::vector<char> _board;

public:
    Board(int rows, int columns, float ratio)
    : _rows{rows}
    , _columns{columns}
    , _remaining_mines{0}
    , _board(rows * columns, '-')
    {
        std::random_device gen;
        std::bernoulli_distribution dist(ratio);
        for (int j = 0; j < _rows; ++j) {
            for (int k = 0; k < _columns; ++k) {
                if (dist(gen)) {
                    get(j, k) = 'x';
                    ++_remaining_mines;
                }
            }
        }
    }


    GameState play(int a, int b, char flag)
    {
        const int x = _rows - b;
        const int y = a - 1;
        if (x < 0 or x > _rows or y < 0 or y > _columns) {
            return GameState::CONTINUE;
        }

        switch (flag)
        {
        case '!':
            if (get(x, y) == '-') {
                get(x, y) = '!';
            }
            else if (get(x, y) == 'x') {
                get(x, y) = 'X';
                if (--_remaining_mines == 0) {
                    return GameState::VICTORY;
                }
            }
            break;

        case '0':
            if (get(x, y) == '!') {
                get(x, y) = '-';
            }
            else if (get(x, y) == 'X') {
                get(x, y) = 'x';
            }
            break;

        default:
            break;
        }
        return GameState::CONTINUE;
    }


    GameState play(int a, int b)
    {
        const int x = _rows - b;
        const int y = a - 1;
        if (x < 0 or x > _rows or y < 0 or y > _columns) {
            return GameState::CONTINUE;
        }

        // Don't play in a flagged space.
        if (get(x, y) == '!' or get(x, y) == 'X') {
            return GameState::CONTINUE;
        }

        // Selected mine: end the game.
        if (get(x, y) == 'x') {
            return GameState::FAIL;
        }
        // Selected unseen space: expand selection.
        if (get(x, y) == '-') {
            expand(x, y);
        }
        return GameState::CONTINUE;
    }


    void print(bool show_mines) const
    {
        for (int j = 0; j < _rows ; ++j) {
            for (int k = 0; k < _columns ; ++k) {
                const char c = get(j, k);
                if (not show_mines and c == 'X') {
                    std::cout << '!' << ' ';
                }
                else if (not show_mines and c == 'x') {
                    std::cout << '-' << ' ';
                }
                else {
                    std::cout << c << ' ';
                }
            }
            std::cout << '\n';
        }
    }


private:
    char & get(int x, int y) 	   { return _board[x * _columns + y]; }
    char & get(Coord pair)         { return get(std::get<0>(pair), std::get<1>(pair)); }
    char   get(int x, int y) const { return _board[x * _columns + y]; }


    std::pair<std::vector<Coord>, int> find_neighbours(Coord c)
    {
        const int x = std::get<0>(c);
        const int y = std::get<1>(c);
        int close_mines = 0;
        std::vector<Coord> neighbour_coords;
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                // Don't add myself again.
                if (j == 0 and k == 0) {
                    continue;
                }
                const int pos_x = x + j;
                const int pos_y = y + k;
                // Boundary check
                if (pos_x < 0 or pos_x >= _rows or pos_y < 0 or pos_y >= _columns) {
                    continue;
                }
                // Already seen neighbour.
                if (get(pos_x, pos_y) == '.') {
                    continue;
                }
                // Neighbour is empty and unexplored: add to frontier.
                if (get(pos_x, pos_y) == '-') {
                    neighbour_coords.emplace_back(pos_x, pos_y);
                }
                // Neighbour is a mine. don't add it, but count for numbering purposes.
                if (get(pos_x, pos_y) == 'x' or get(pos_x, pos_y) == 'X') {
                    ++close_mines;
                }
            }
        }
        // If we've found mines in the neighbourhood, then we're in the
        // frontier region, so we want to
        if (close_mines != 0) {
            return {{}, close_mines};
        }
        return {neighbour_coords, close_mines};
    }


    // BFS to reveal. Stop expanding the frontier when it reaches a mine.
    void expand(int start_x, int start_y)
    {
        std::vector<Coord> frontier{{start_x, start_y}};
        frontier.reserve(32);
        do {
            Coord coord_to_expand = frontier.back();
            frontier.pop_back();
            auto [neighbours, neighbours_who_are_mines] = find_neighbours(coord_to_expand);
            // Only expand if there are no mines around, otherwise show a hint
            //  saying how many mines are in the neighbourhood of the current
            //  coordinate.
            if (neighbours_who_are_mines == 0) {
                get(coord_to_expand) = '.';
                for (Coord coord : neighbours) {
                    frontier.push_back(coord);
                }
            }
            else {
                // Write int as char.
                get(coord_to_expand) = 48 + neighbours_who_are_mines;
            }
        } while (not frontier.empty());
    }
};


int main(int argc, const char* argv[])
{
    if (argc < 3) {
        std::cout << "./mine <rows> <columns> [<percentage of mines (default 0.1)>]\n";
        return 1;
    }
    const int rows = std::stoi(argv[1]);
    const int cols = std::stoi(argv[2]);
    const float ratio = argc > 3 ? std::stof(argv[3]) : 0.1;

    Board board(rows, cols, ratio);
    while (true) {
        board.print(false);
        std::cout << "Input x and y coordinates to reveal a square.\n"
                  << "Optionally also 'x' to flag a mine and '0' to clear the flag.\n";

        std::string input;
        std::getline(std::cin, input);
        std::stringstream ss(input);

        int guess_x = -1;
        int guess_y = -1;
        std::string command{};
        ss >> guess_x >> guess_y >> command;
        std::cout << "read " << guess_x << " " << guess_y << " :" << command << ":\n";

        if (GameState play_result = command.empty() ? board.play(guess_x, guess_y)
                                                    : board.play(guess_x, guess_y, command.front());
            play_result == GameState::FAIL)
        {
            board.print(true);
            std::cout << "You've stepped on a mine!\n";
            break;
        }
        else if (play_result == GameState::VICTORY) {
            board.print(true);
            std::cout << "Victory!\n";
            break;
        }
    }
}
