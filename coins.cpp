#include <string>
#include <iostream>
#include <ranges>
#include <exception>
#include <stack>
#include <vector>
#include <algorithm>
#include <ostream>


// https://leetcode.com/problems/coin-change/


namespace std
{
std::ostream& operator<<(std::ostream& o, const std::vector<int>& v)
{
    if (v.empty()) {
        return o << "[]";
    }
    o << "[";
    for (int j = 0; j < v.size() - 1; ++j) {
        o << v[j] << ", ";
    }
    o << v.back() << "]";
    return o;
}
} // namespace std


// Find the index of <val> in <v>. All values of <v> are unique.
int indexOf(int val, const std::vector<int>& v)
{
    for (int j = 0; j < v.size(); ++j) {
        if (v[j] == val) {
            return j;
        }
    }
    // Since we only ask for the indices of elements which are already in the
    //  vector, this only happens if <v> is empty, which is never.
    throw std::out_of_range("Couldn't find " + std::to_string(val) + " in the vector. Is it empty?");
}


// <coins> is the original vector with all the coin denominations, in ascending order.
// <current_selection> is the current selection of coins. When this adds up to
//  <amount>, we are done.
// <pos> is an index of <coins>, indicating which coin denomination we are
//  trying currently. It starts as the array's maximum index (so we try the
//  highest denomination coin first), and goes down as we overshoot the goal
//  amount. If <pos> reaches zero, then no coin we've tried works for the
//  current selection of coins (<selected_coins>), so we need to remove the
//  previously added coin and reset pos to point to its next lowest denomination coin.
std::vector<int> rechelp(const std::vector<int>& coins, int amount, std::vector<int>& current_selection, int pos)
{
    // Currently selected coins add to up <amount>, so we're done.
    if (amount == 0) {
        return current_selection;
    }

    // Exhausted every coin for the current coin selection.
    // We need to pop another coin from the selection and try the next lowest
    //  coin in its place.
    if (pos < 0) {
        // Popped the last coin from the current selection, and cannot pick
        //  another coin lower than it, which means the problem has no solution.
        if (current_selection.empty()) {
            return {};
        }
        amount += current_selection.back();
        // Next lower denomination coin to the one we're using.
        pos = indexOf(current_selection.back(), coins) - 1;
        current_selection.pop_back();
        return rechelp(coins, amount, current_selection, pos);
    }

    // Selection overshot the amount; we need to try the next lowest coin.
    if (amount < 0) {
        // Restore amount.
        amount += current_selection.back();
        current_selection.pop_back();
        // Try next number.
        pos--;
        return rechelp(coins, amount, current_selection, pos);
    }

    // Add a coin to the selection.
    current_selection.push_back(coins[pos]);
    amount -= current_selection.back();
    return rechelp(coins, amount, current_selection, pos);
}


std::vector<int> solve(const std::vector<int>& coins, int amount)
{
    std::vector<int> current_selection;
    // Start with no coins selected (empty <current_selection>). Start by
    //  selecting the highest denomination coins first (coins at the end of
    //  <coins>, so start <pos> there).
    return rechelp(coins, amount, current_selection, coins.size() - 1);
}


int main(int argc, const char* argv[])
{
    // int amount = std::stoi(argv[1]);
    // std::vector<int> coins;
    // for (int j = 2; j < argc; ++j) {
    //     coins.emplace_back(std::stoi(argv[j]));
    // }

    int amount = 15;
    std::vector<int> coins{5, 3, 2};

    // Coins need to be in ascending order, since we try the highest
    //  denomination coin first, and we do it from the end of the array.
    std::ranges::sort(coins);
    std::vector<int> selected_coins{solve(coins, amount)};
    std::cout << selected_coins.size() << ": " << selected_coins << '\n';
}
