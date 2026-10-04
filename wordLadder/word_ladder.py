"""Starter for the CS 460 word-ladder project."""

import sys


def read_dictionary(file_name):
    """Read one word per line and return the words in their original order."""
    with open(file_name, encoding="utf-8") as input_file:
        return [line.strip() for line in input_file if line.strip()]


def read_start_target_words(file_name):
    """Read the start-target pairs in their original order."""
    word_pairs = []
    with open(file_name, encoding="utf-8") as input_file:
        for line in input_file:
            if line.strip():
                start, target = line.split()
                word_pairs.append((start, target))
    return word_pairs


def index_of_word(words, word):
    """Return word's position, or len(words) if word is not present."""
    for position, current_word in enumerate(words):
        if current_word == word:
            return position

    return len(words)

    # raise NotImplementedError("Complete index_of_word")


def positional_difference(first, second):
    """Return the number of positions at which two words differ."""

    differences = 0
    for i in range(len(first)):
        if first[i] != second[i]:
            differences += 1;

    return differences

    # raise NotImplementedError("Complete positional_difference")


def find_successor_from(
    word,
    words,
    used,
    where_to_start
):
    """Return the first eligible successor index, or len(words) if none exists."""

    for i in range(where_to_start, len(words)):
        if not used[i] and positional_difference(word, words[i]) == 1:
            return i

    return len(words)

    # raise NotImplementedError("Complete find_successor_from")


def find_path(words, start, target):
    """Return the ladder found by the required iterative DFS, or an empty list."""

    start_index = index_of_word(words, start)
    target_index = index_of_word(words, target)

    if start_index == len(words) or target_index == len(words):
        return []

    used = [False] * len(words)
    stack = []
    stack.append((start, start_index))
    used[start_index] = True
    where_to_start_successor_search = 0;

    while stack:
        word, word_index = stack[-1]
        if word == target:
            path = []
            for path_word, path_word_index in stack:
                #(void)path_word_index;
                path.append(path_word)
            return path

        successor_index = find_successor_from(word, words, used, where_to_start_successor_search)

        if successor_index != len(words):
            stack.append((words[successor_index], successor_index))
            used[successor_index] = True;
            where_to_start_successor_search = 0;
        else:
            stack.pop()
            where_to_start_successor_search = word_index + 1

    return []

    # raise NotImplementedError("Complete find_path")


def print_path(path, start, target):
    """Print one search result using the required format."""
    if not path:
        print(f"No ladder from {start} to {target} exists.\n")
        return

    print(f"A ladder from {start} to {target} is:")
    for word in path:
        print(word)
    print()


def main():
    if len(sys.argv) != 3:
        print(
            f"Usage: {sys.argv[0]} dictionary-file start-target-file",
            file=sys.stderr,
        )
        return 1

    try:
        words = read_dictionary(sys.argv[1])
        word_pairs = read_start_target_words(sys.argv[2])
    except OSError as error:
        print(error, file=sys.stderr)
        return 2

    for start, target in word_pairs:
        path = find_path(words, start, target)
        print_path(path, start, target)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
