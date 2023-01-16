#include <iostream>
#include <vector>
#include <algorithm>


// https://techdevguide.withgoogle.com/resources/former-interview-question-volume-of-lakes/#!


// Pass over the array once to find the index of the tallest pillar. If there is no unique tallest pillar, choose the left-most one.
// Then, iterate over the array from the beginning to the chosen index, and backwards from the end to the chosen index.
// The current water level is the difference between the current pillar's height and the maximum height of any pillar seen thus far.
//
// The remaining pillars from the current pillar to the tallest pillar are going to be:
//  the same height: not interesting.
//  shorter: we are still in the same basin.
//  taller: initiate a new basin, updating the current maximum.
// Since we're iterating up to the The current maximum is always going to be monotonically increasing,
int secondSolution(const std::vector<int>& v)
{
    const auto index_max = std::max_element(std::cbegin(v), std::cend(v));
    int acc = 0;

    auto current_max_forward = std::cbegin(v);
    for (auto forward_iter = std::cbegin(v);
        forward_iter != index_max;
        ++forward_iter)
    {
        if (*forward_iter >= *current_max_forward) {
            current_max_forward = forward_iter;
        }
        else {
            acc += *current_max_forward - *forward_iter;
        }
    }

    auto current_max_backward = std::crbegin(v);
    for (auto backward_iter = std::crbegin(v);
         backward_iter != static_cast<std::vector<int>::const_reverse_iterator>(index_max);
         ++backward_iter)
    {
        if (*backward_iter >= *current_max_backward) {
            current_max_backward = backward_iter;
        }
        else {
            acc += *current_max_backward - *backward_iter;
        }
    }
    return acc;
}

// First solution.
// Iterate from left to right, counting the difference between the current
//  pillar and the tallest pillar so far.
// This works until we've seen the tallest pillar. After that, all values to the
//  right are going to be wrong (because we need to start new basins), so reset
//  the water count for those pillars.
// Then, iterate from right to left (or reverse the array and iterate from left
//  to right) to count the differente between the current pillar and the tallest
//  pillar so far. Again, this is going to overshoot everything to the left of
//  the tallest pillar, so we need to reset the water count in this iteration.
// This doesn't work if there is no unique tallest pillar: if there are two
//  tallest pillars, then the left->right iteration will discount until it
//  reaches the rightmost pillar, and the right->left iteration will discount
//  until it reaches the leftmost pillar, which means the water in the basin
//  between the two tallest pillars is counted twice.
// To get around this, make the rightmost pillar taller.
//  This never changes the solution.
// This is extremely inefficient... instead of going left->right, correcting
//  the overshoot, going right->left and correcting the overshoot again, we
//  can just go left->right until we reach the tallest pillar, and then
//  right->left until we reach the tallest pillar again.
int firstSolution(std::vector<int> v)
{
    // Increase rightmost tallest pillar by one.
    int max = 0;
    for (int j = 0; j < v.size(); ++j) {
        if (v[j] >= v[max]) {
            max = j;
        }
    }
    ++v[max];

    // Left to right iteration.
    int current_max = 0;
    int a = 0;
    for (int j = 1; j < v.size(); ++j) {
        if (v[j] >= v[current_max]) {
            current_max = j;
        }
        else {
            a += v[current_max] - v[j];
        }
        std::cout << "j = " << j << " acc = " << a << "\n";
    }
    // Discount overshoot after the rightmost pillar, up until the rightmost pillar.
    std::cout << "discount\n";
    for (int j = v.size() - 1; v[j] < v[current_max]; --j) {
        a -= v[current_max] - v[j];
        std::cout << "j = " << j << " acc = " << a << "\n";
    }

    // Right to left iteration.
    std::cout << "REVERSING\n";
    std::reverse(std::begin(v), std::end(v));
    current_max = 0;
    for (int j = 1; j < v.size(); ++j) {
        if (v[j] >= v[current_max]) {
            current_max = j;
        }
        else {
            a += v[current_max] - v[j];
        }
        std::cout << "j = " << j << " acc = " << a << "\n";
    }
    // Discount overshoot after the leftmost pillar, up until the leftmost pillar.
    std::cout << "discount\n";
    for (int j = v.size() - 1; v[j] < v[current_max]; --j) {
        a -= v[current_max] - v[j];
        std::cout << "j = " << j << " acc = " << a << "\n";
    }
    return a;
}


int main(int argc, const char* argv[])
{
    // std::vector<int> v;
    // for (int j = 1; j < argc; ++j) {
    //     v.push_back(std::stoi(argv[j]));
    // }

    std::vector<int> v{1, 3, 2, 4, 1, 3, 1, 4, 5, 2, 2, 1, 4, 2, 2};
    std::cout << secondSolution(v) << '\n';
}
