#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using WordPair = std::tuple<std::string, std::string>;
using StackEntry = std::tuple<std::string, std::size_t>;

std::vector<std::string> read_dictionary(const std::string &file_name) {
    std::ifstream input(file_name);
    if (!input) {
        std::cerr << "Unable to open dictionary file " << file_name << '\n';
        std::exit(2);
    }

    std::vector<std::string> words;
    std::string word;
    while (input >> word) {
        words.push_back(word);
    }
    return words;
}

std::vector<WordPair> read_start_target_words(const std::string &file_name) {
    std::ifstream input(file_name);
    if (!input) {
        std::cerr << "Unable to open start-target file " << file_name << '\n';
        std::exit(2);
    }

    std::vector<WordPair> word_pairs;
    std::string start;
    std::string target;
    while (input >> start >> target) {
        word_pairs.push_back({start, target});
    }
    return word_pairs;
}

std::size_t index_of_word(const std::vector<std::string> &words, const std::string &word) {
    for (std::size_t index = 0; index < words.size(); ++index) {
        if (words[index] == word) {
            return index;
        }
    }
    return words.size();
}

int positional_difference(
    const std::string &first,
    const std::string &second
) {
    int differences = 0;
    for (std::size_t position = 0; position < first.size(); ++position) {
        if (first[position] != second[position]) {
            ++differences;
        }
    }
    return differences;
}

std::size_t find_successor_from(
    const std::string &word,
    const std::vector<std::string> &words,
    const std::vector<bool> &used,
    std::size_t where_to_start
) {
    for (std::size_t index = where_to_start; index < words.size(); ++index) {
        if (!used[index] && positional_difference(word, words[index]) == 1) {
            return index;
        }
    }
    return words.size();
}

std::vector<std::string> find_path(
    const std::vector<std::string> &words,
    const std::string &start,
    const std::string &target
) {
    const std::size_t start_index = index_of_word(words, start);
    const std::size_t target_index = index_of_word(words, target);
    if (start_index == words.size() || target_index == words.size()) {
        return {};
    }

    // Each search begins with fresh state. The vector named stack is used as
    // a stack: its last element is the top.
    std::vector<bool> used(words.size(), false);
    std::vector<StackEntry> stack;
    stack.push_back({start, start_index});
    used[start_index] = true;

    std::size_t where_to_start_successor_search = 0;

    while (!stack.empty()) {
        const auto [word, word_index] = stack.back();

        if (word == target) {
            // Since the bottom of the stack is the start word, iterating over
            // the vector produces the completed ladder in the desired order.
            std::vector<std::string> path;
            for (const auto &[path_word, path_word_index] : stack) {
                (void)path_word_index;
                path.push_back(path_word);
            }
            return path;
        }

        // Loop invariant: stack contains the current ladder; every word on it
        // is marked used; and the next successor search for its top word begins
        // at where_to_start_successor_search.
        const std::size_t successor_index = find_successor_from(
            word,
            words,
            used,
            where_to_start_successor_search
        );

        if (successor_index != words.size()) {
            // Descend to the first available successor. A new top word has not
            // had any of its successors examined, so its search begins at zero.
            stack.push_back({words[successor_index], successor_index});
            used[successor_index] = true;
            where_to_start_successor_search = 0;
        } else {
            // This word is exhausted. After removing it, resume its parent's
            // search immediately after the dictionary position just explored.
            stack.pop_back();
            where_to_start_successor_search = word_index + 1;
        }
    }

    return {};
}

void print_path(
    const std::vector<std::string> &path,
    const std::string &start,
    const std::string &target
) {
    if (path.empty()) {
        std::cout << "No ladder from " << start << " to " << target
                  << " exists.\n\n";
        return;
    }

    std::cout << "A ladder from " << start << " to " << target << " is:\n";
    for (const std::string &word : path) {
        std::cout << word << '\n';
    }
    std::cout << '\n';
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0]
                  << " dictionary-file start-target-file\n";
        return 1;
    }

    const std::vector<std::string> words = read_dictionary(argv[1]);
    const std::vector<WordPair> word_pairs = read_start_target_words(argv[2]);

    for (const auto &[start, target] : word_pairs) {
        print_path(find_path(words, start, target), start, target);
    }

    return 0;
}
