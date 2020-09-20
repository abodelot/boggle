#ifndef BOGGLE_HPP
#define BOGGLE_HPP

#include <map>
#include "Trie.hpp"

class Boggle {
public:
    static const int GRID_SIZE = 4;
    static const int ARRAY_SIZE = GRID_SIZE * GRID_SIZE;

    enum Lang {
        English = 0,
        French = 1,
    };

    Boggle();

    /**
     * Load list of valid words. File must contain one word per line.
     */
    bool load_dictionary(const std::string& path);

    /**
     * Generate a new random grid
     * @param lang: use dice set for given language
     */
    void generate_grid(Lang lang);

    /**
     * Define a custom grid from a string
     * @param chars: string of ARRAY_SIZE length
     */
    void set_grid(const std::string& chars);

    /**
     * Print grid to stdout
     */
    void print_grid() const;

    /**
     * Print grid solution to stdout
     */
    void print_solution() const;

    /**
     * Submit word and increase user score.
     */
    void submit_word(const std::string& word);

private:
    struct Vector2 {
        int x;
        int y;
    };

    void solve_grid();
    void explore(int i, int j, bool* visited, std::string& word);

    char grid_[GRID_SIZE][GRID_SIZE];

    Trie dict_;

    // List of valid word in current grid
    std::map<std::string, bool> words_;

    int user_score_;
    int max_score_;
};

#endif
