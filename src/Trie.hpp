#ifndef TRIE_HPP
#define TRIE_HPP

#include <string>

/**
 * Lexicographic tree for storing valid words
 */
class Trie {
public:
    enum Result {
        Yes,
        No,
        Maybe
    };

    Trie();

    ~Trie();

    /**
     * Register a new word
     * @return true if word was added, false if already present
     */
    bool add_word(const std::wstring& word);

    /**
     * Number of registered words
     */
    size_t size() const;

    /**
     * Check if a word exists
     * @return Yes if word exists, No if it doesn't exist,
     * Maybe if word doesn't exist but is the begining of an existing word.
     */
    Result find(const std::string& string) const;

    /**
     * Delete al words
     */
    void clear();

private:
    struct Node {
        Node();
        ~Node();

        Node* children[26];
        bool word_end;
    };

    Node root_;
    size_t size_;
};

#endif
