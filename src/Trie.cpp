#include "Trie.hpp"

#include <iostream>
#include <stdexcept>


static wchar_t unaccent(wchar_t c)
{
    switch (c) {
    case L'à':
    case L'â':
    case L'ä':
        return 'a';
    case L'é':
    case L'è':
    case L'ê':
    case L'ë':
        return 'e';
    case L'ì':
    case L'î':
    case L'ï':
        return 'i';
    case L'ò':
    case L'ô':
    case L'ö':
        return 'o';
    case L'ù':
    case L'û':
    case L'ü':
        return 'u';
    case L'ý':
    case L'ŷ':
    case L'ÿ':
        return 'y';
    case L'ç':
        return 'c';
    }
    return c;
}


Trie::Trie():
    size_(0)
{
}


Trie::~Trie()
{
}


bool Trie::add_word(const std::wstring& string)
{
    Node* n = &root_;
    size_t size = string.size();
    for (size_t i = 0; i < size; ++i) {
        wchar_t c = unaccent(string[i]);
        int index = c - 'a';
        if (index >= 0 && index < 26) {
            Node** next = &n->children[index];
            if (*next == nullptr) {
                *next = new Node();
            }
            n = *next;
        } else {
            // Unsupported character => invalid word
            return false;
        }
    }

    if (n->word_end) {
        // Word already present
        return false;
    }

    n->word_end = true;
    ++size_;
    return true;
}


size_t Trie::size() const
{
    return size_;
}


Trie::Result Trie::find(const std::string& string) const
{
    const Node* n = &root_;
    size_t size = string.size();
    for (size_t i = 0; i < size; ++i) {
        char current = string[i];
        if (current >= 'A' && current <= 'Z') {
            current += 32;
        }
        int index = current - 'a';
        if (index >= 0 && index < 26) {
            n = n->children[index];
            if (n == nullptr) {
                return Trie::No;
            }
        } else {
            // Invalid input
            return Trie::No;
        }
    }
    return n->word_end ? Trie::Yes : Trie::Maybe;
}


Trie::Node::Node():
    word_end(false)
{
    for (int i = 0; i < 26; ++i) {
        this->children[i] = nullptr;
    }
}


Trie::Node::~Node()
{
    for (int i = 0; i < 26; ++i) {
        if (children[i] != nullptr) {
            delete children[i];
        }
    }
}
