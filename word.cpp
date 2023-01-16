#include <iostream>
#include <string>
#include <ranges>
#include <algorithm>
#include <vector>
#include <ostream>


// https://techdevguide.withgoogle.com/resources/former-interview-question-word-squares/#!


using seq = std::vector<std::string>;


// Check if a given sequence of words is a word square.
bool isWordSquare(const seq& words)
{
    for (int j = 0; j < words.size(); ++j) {
        for (int k = 0; k < words.size(); ++k) {
            if (words[j][k] != words[k][j]) {
                return false;
            }
        }
    }
    return true;
}


// Check whether, for the current selection of words (<selected_words>), we
//  can select word <candidate> (index into <words>) and still keep the word
//  square constraints.
bool canSelectWord(int candidate,
                   const std::vector<int>& selected_words,
                   const seq& words)
{
    const std::string& candidate_word = words[candidate];
    // Number of the word we're inserting.
    const int insertion_number = selected_words.size();
    for (int j = 0; j < selected_words.size(); ++j) {
        const std::string& selected_word = words[selected_words[j]];

        // If current selection word is the candidate, skip it.
        // We need to do this by index because we could have repeated words.
        if (candidate == selected_words[j]) {
            return false;
        }
        // See if word square constraint is still valid:
        // char c of candidate_word x = char x of selection_word c,
        //  for all selection_words,
        //  where c is the index of the currently selected word,
        //  where x is the index number of the candidate word, if it were
        //   inserted in the selected array (i.e., number of selected words
        //   thus far).
        if (candidate_word[j] != selected_word[insertion_number]) {
            return false;
        }
    }
    return true;
}


std::vector<std::vector<int>>
getAllWordSquaresForWord(std::vector<int> selected_words,
                         const seq& words)
{
    // All words have the same length. Since we only add valid words to the selection,
    // when the selection is the same length as a word, we've found a word square.
    if (selected_words.size() == words.front().length()) {
        return {selected_words};
    }

    std::vector<std::vector<int>> ret;
    for (int j = 0; j < words.size();  ++j) {
        // Only select a valid word.
        if (canSelectWord(j, selected_words, words)) {
            selected_words.push_back(j);
            for (auto all_word_square = getAllWordSquaresForWord(selected_words, words);
                 auto& word_square : all_word_square)
            {
                // Don't add duplicates.
                if (std::ranges::count(ret, word_square) == 0) {
                    ret.emplace_back(std::move(word_square));
                }
            }
        }
    }
    return ret;
}


int main()
{
    seq EXERCISE {"AREA", "BALL", "DEAR", "LADY", "LEAD", "YARD"};

    // Try to build a word square for all the words in the input vector.
    for (int j = 0; j < EXERCISE.size(); ++j) {
        std::vector<int> selected_words{j};
        // A given word may start more than one word square.
        for (auto squares_found = getAllWordSquaresForWord(selected_words, EXERCISE);
             const auto& word_square: squares_found)
        {
            for (int index: word_square) {
                std::cout << EXERCISE[index] << '\n';
            }
            std::cout << '\n';
        }
    }
}
