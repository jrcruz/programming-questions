#include <iostream>
#include <set>
#include <ios>
#include <vector>
#include <bit>


// https://leetcode.com/problems/number-of-excellent-pairs/


// Count number of set bits in integer <x>.
int countSet(unsigned x)
{
    int count = 0;
    for (unsigned j = 0; j < sizeof(unsigned); ++j) {
        if ((x bitand (1 << j)) != 0) {
            ++count;
        }
    }
    return count;
}


int solve(const std::vector<int>& numbers, int k)
{
    std::set<std::pair<int, int>> found;
    for (unsigned x : numbers) {
        for (unsigned y: numbers) {
            // if (std::popcount(x bitor y) + std::popcount(x bitand y) >= k) {
            if (countSet(x bitor y) + countSet(x bitand y) >= k) {
                found.emplace(x, y);
            }
        }
    }
    return found.size();
}


int main()
{
    std::vector<int> all{1, 2, 3, 1};
    int k = 3;
    std::cout << solve(all, k) << '\n';
}
