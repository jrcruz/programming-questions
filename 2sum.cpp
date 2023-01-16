#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <unordered_map>


// https://leetcode.com/problems/two-sum/


// We want to find x, y such that x + y = target, equivalent to x = target - y.
// To do this, we see cycle through y and see if target - y is in the array.
//  If yes, then that's our x, and we're done.
// This way of solving the problem only iterates over the number array twice
//  (rather than the brute force N^2 solution). Since our reverse index is an
//  hash-map, the lookup is constant amortized, which is negligible.
std::pair<int, int> solveTwoSum(int target, const std::vector<int>& numbers)
{
    // Create reverse index so we can retrieve x's index quickly, given target - y.
    // If target - y isn't in the reverse index, then the current choice of y
    //  doesn't produce a solution.
    std::unordered_map<int, int> m;
    for (int j = 0; j < numbers.size(); ++j) {
        m[numbers[j]] = j;
    }

    for (int j = 0; j < numbers.size(); ++j) {
        const int complement = target - numbers[j];
        // (complement) x's index  can't be equal to (current number) y's index.
        if (m.contains(complement) and m[complement] != j) {
            return std::minmax(j, m[complement]);
        }
    }
    return {-1, -1};
}


int main(int argc, const char* argv[])
{
    // const int target = std::stoi(argv[1]);
    // std::vector<int> all;
    // for (int j = 2; j < argc; ++j) {
    //     all.push_back(std::stoi(argv[j]));
    // }

    // const int target = 9;
    // std::vector<int> all{2,7,11,15};

    const int target = 6;
    std::vector<int> all{3, 2, 4};

    // const int target = 6;
    // std::vector<int> all{3, 3};

    auto [p1, p2] = solveTwoSum(target, all);
    std::cout << "(" << p1 << ", " << p2 << ")\n";
}
