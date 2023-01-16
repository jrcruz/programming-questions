#include <iostream>
#include <vector>
#include <span>
#include <string>
#include <charconv>
#include <cctype>


// https://techdevguide.withgoogle.com/resources/former-interview-question-compression-and-decompression/#!


// Repeat <string> <count> times.
std::string expand(const std::string& string, int count)
{
    std::string ret{};
    for (int j = 0; j < count; ++j) {
        ret += string;
    }
    return ret;
}


// We are going to iterate over the string by "recursing forward" every expansion.
// If we come across an expansion, we return the string expanded the appropriate
//  number of times, and also the length of the substring that we've just
//  analyzed (up to the first ']' from the current index), so that we know how
//  much to jump forward in order to proceed with the iteration.
std::pair<std::string, int> recurse(const std::string& input)
{
    std::string ret;
    int j = 0;
    while (j < input.length()) {
        // If letter, accumulate.
        if (std::isalpha(input[j])) {
            ret += input[j];
            ++j;
        }

        // If digit, prepare to recurse down a new expansion.
        else if (std::isdigit(input[j])) {
            // Skip over the number.
            const int next_bracket = input.find('[', j);
            const int repeat_count = std::stoi(input.substr(j, next_bracket - j));
            j = next_bracket;

            // Skip over the opening bracket.
            ++j;

            // Parse inner text.
            auto [inner_text, text_length_to_skip] = recurse(input.substr(j));

            // Skip over the inner text.
            j += text_length_to_skip;

            // Skip over the closing bracket.
            ++j;

            // Expand inner text and append it to the return string, for this iteration.
            ret += expand(inner_text, repeat_count);
        }

        // Reached this recursion step's closing bracket.
        else if (input[j] == ']') {
            return {ret, j};
        }
    }
    // Consumed entire input string: ret is the fully expanded string.
    return {ret, j};
}


int main(int argc, const char* argv[])
{
    // const std::string input(argv[1]);

    const std::string input{"4[abf5[c2[m]]g]x13[k]"};
    auto [ret, _] = recurse(input);
    std::cout << ret << '\n';
}
