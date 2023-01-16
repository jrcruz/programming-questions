#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>
#include <array>
#include <unordered_map>
#include <set>
#include <tuple>


// https://leetcode.com/problems/3sum/


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


// We want to find a,b,c such that a + b + c = 0, which is equivalent to
//  a + b = -c, which is just the twoSum problem with -c as the target.
std::vector<std::array<int, 3>> solveThreeSum(const std::vector<int>& numbers)
{
    std::vector<std::array<int, 3>> ret;
    for (int j = 0; j < numbers.size(); ++j) {
        auto [p1, p2] = solveTwoSum(-numbers[j], numbers);
        // Solving twoSum with fixed number[j] didn't work, so skip to the next one.
        if (p1 == -1 and p2 == -1) {
            continue;
        }
        // Solution is only valid if the returned numbers are not the current
        //  target, so we don't use the same position in the array twice.
        if (j != p1 and j != p2) {
            ret.push_back({numbers[j], numbers[p1], numbers[p2]});
            // Sort the triple so we can filter out duplicates later.
            std::sort(std::begin(ret.back()), std::end(ret.back()));
        }
    }
    // Return only distinct triples.
    return {std::begin(ret), std::unique(std::begin(ret), std::end(ret)) };
}


int main(int argc, const char* argv[])
{
    // std::vector<int> all;
    // for (int j = 1; j < argc; ++j) {
    //     all.push_back(std::stoi(argv[j]));
    // }

    std::vector<int> all{-1, 0, 1, 2, -1, -4};
    // std::vector<int> all{0, 0, 0};
    // std::vector<int> all{0, 1, 1};

    for (auto triples = solveThreeSum(all); auto& [p1, p2, p3] : triples) {
        std::cout << "(" << p1 << ", " << p2 << ", " << p3 << ")\n";
    }
}
