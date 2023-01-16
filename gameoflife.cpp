#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <tuple>
#include <chrono>
#include <thread>


// https://leetcode.com/problems/game-of-life/


class Game
{
    std::vector<int> _board;
    const int _rows;
    const int _columns;

    int neighbours(int x, int y) {
        int total = 0;
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                // Don't count myself as a possible neighbour.
                if (j == 0 and k == 0) {
                    continue;
                }
                const int pos_x = x + j;
                const int pos_y = y + k;
                // Wrap around the board if we're at an edge.
                total += get(pos_x < 0 ? _rows    + pos_x : pos_x % _rows,
                             pos_y < 0 ? _columns + pos_y : pos_y % _columns);
            }
        }
        return total;
    }

public:
    Game (std::vector<int>&& board, int rows, int columns)
    : _board(std::move(board))
    , _rows(rows)
    , _columns(columns)
    {;}


    int  get(int x, int y) const { return _board[x * _columns + y]; }
    int& get(int x, int y)       { return _board[x * _columns + y]; }


    // Cycle once through the game of life.
    // We cannot update the board in-place when we're finding out which
    //  positions change, so we need to accumulate the changes and perform
    //  them at the end.
    void next()
    {
        std::vector<std::tuple<int, int, int>> actions;
        for (int j = 0; j < _rows; ++j) {
            for (int k = 0; k < _columns; ++k) {
                const int alive_neighbours = neighbours(j, k);
                // Cell dies next turn if it is alive and surrounded by less
                //  than 2 or more than 3 neighbours.
                if (get(j, k) > 0 and (alive_neighbours < 2 or alive_neighbours > 3)) {
                    actions.emplace_back(j, k, 0);
                }
                // Cell lives next turn if it is surrounded by 3 neighbours,
                //  no matter its previous state.
                else if (alive_neighbours == 3) {
                    actions.emplace_back(j, k, 1);
                }
            }
        }
        // Perform all changes.
        for (auto[x, y, action] : actions) {
            get(x, y) = action;
        }
    }


    friend std::ostream& operator<<(std::ostream& o, const Game& board)
    {
        for (int j = 0; j < board._rows; ++j) {
            for (int k = 0; k < board._columns; ++k) {
                o << board.get(j, k) << ' ';
            }
            o << '\n';
        }
        return o;
    }
};


// Board is read from stdin.
int main(int argc, const char* argv[])
{
    std::vector<int> b;
    int rows = 0;
    int cols = 0;

    std::cout << "Input:\n";
    // Read rows.
    std::string s;
    while (std::getline(std::cin, s)) {
        std::cout << s << "\n";
        int n;
        cols = 0;
        std::stringstream ss(s);
        // Read columns.
        while (ss >> n) {
            b.push_back(n);
            ++cols;
        }
        ++rows;
    }
    std::cout << "Rows: " << rows << ", cols: " << cols << "\n\n";

    Game game(std::move(b), rows, cols);
    int plays = 10;
    while (plays-- != 0) {
        game.next();
        std::cout << game << '\n';
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
}
