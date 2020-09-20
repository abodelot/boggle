#include "Boggle.hpp"

#include <algorithm>
#include <codecvt>
#include <cstring>
#include <fstream>
#include <iostream>
#include <locale>
#include <random>


static const char* DICE[][Boggle::ARRAY_SIZE] = {
    {
        // 0 => English
        "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
        "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
        "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
        "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
    }, {
        // 1 => French
        "ETUKNO", "EVGTIN", "DECAMP", "IELRUW",
        "EHIFSE", "RECALS", "ENTDOS", "OFXRIA",
        "NAVEDZ", "EIOATA", "GLENYU", "BMAQJO",
        "TLIBRA", "SPULTE", "AIMSOR", "ENHRIS"
    }
};


int points_for_word_len(size_t size)
{
    if (size >= 8) {
        return 11;
    }
    if (size >= 7) {
        return 5;
    }
    if (size >= 6) {
        return 3;
    }
    if (size >= 5) {
        return 2;
    }
    return 1;
}


Boggle::Boggle():
    user_score_(0),
    max_score_(0)
{
}


bool Boggle::load_dictionary(const std::string& path)
{
    // Set the global locale before loading utf-8 file
    std::locale::global(
        std::locale(std::locale(""), new std::codecvt_utf8<wchar_t>)
    );

    std::wifstream file(path.c_str());
    if (file) {
        std::wstring line;
        size_t lines = 0;
        while (std::getline(file, line)) {
            if (line.size() >= 3) {
                dict_.add_word(line);
            }
            ++lines;
        }
        std::cout << "loaded " << dict_.size() << " words from " << path
            << " (file total: " << lines << ")." << std::endl;
        return true;
    }
    return false;
}


void Boggle::generate_grid(Lang lang)
{
    const char** raw_dice = DICE[lang];
    std::vector<const char*> dice;
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        dice.push_back(raw_dice[i]);
    }

    // Shuffle dice
    std::random_device rd;
    std::shuffle(dice.begin(), dice.end(), rd);

    // Random die face generator
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uniform_dist(0, 5);

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int die_index = i * GRID_SIZE + j;
            int die_face = uniform_dist(rng);
            grid_[i][j] = dice[die_index][die_face];
        }
    }
    solve_grid();
}


void Boggle::set_grid(const std::string& chars)
{
    if (chars.size() != ARRAY_SIZE) {
        std::cerr << "Expected string of size " << ARRAY_SIZE
            << ", got " << chars.size() << std::endl;
        return;
    }

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int index = i * GRID_SIZE + j;
            grid_[i][j] = chars[index];
        }
    }
    solve_grid();
}


void Boggle::submit_word(const std::string& word)
{
    std::string uppercase(word);
    for (size_t i = 0; i < word.size(); ++i) {
        char c = word[i];
        if (c >= 'a' && c <= 'z') {
            uppercase[i] = c - 32;
        }
    }

    const std::map<std::string, bool>::iterator it = words_.find(uppercase);
    if (it != words_.end()) {
        if (!it->second) {
            int points = points_for_word_len(uppercase.size());
            // Flag word as already guessed
            it->second = true;
            user_score_ += points;
            std::cout << "+" << points << "! (" << user_score_ << ")" << std::endl;
        } else {
            std::cout << "Word is already guessed" << std::endl;
        }
    } else if (dict_.find(uppercase) == Trie::Yes) {
        std::cout << "Word is not in grid" << std::endl;
    } else {
        std::cout << "Not a valid word" << std::endl;
    }
}


void Boggle::print_grid() const
{
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            std::cout << grid_[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}


void Boggle::print_solution() const
{
    for (auto it = words_.begin(); it != words_.end(); ++it) {
        std::cout << (it->second ? "* " : "  ") << it->first << " ("
            << points_for_word_len(it->first.size()) << ")" << std::endl;
    }
    std::cout << "score: " << user_score_ << "/" << max_score_ << std::endl;
}


void Boggle::solve_grid()
{
    user_score_ = 0;
    max_score_ = 0;
    words_.clear();

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            bool visited[ARRAY_SIZE];
            for (size_t k = 0; k < ARRAY_SIZE; ++k) {
                visited[k] = false;
            }

            std::string word;
            explore(i, j, visited, word);
        }
    }
}


void Boggle::explore(int i, int j, bool* visited, std::string& word)
{
    // Coords must be valid
    if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE) {
        int index = i * GRID_SIZE + j;
        // Explore cell if not already visited
        if (!visited[index]) {
            bool visited_cpy[ARRAY_SIZE];
            std::memcpy(visited_cpy, visited, ARRAY_SIZE);
            visited_cpy[index] = true;

            std::string word_cpy(word);
            word_cpy += grid_[i][j];

            switch (dict_.find(word_cpy)) {
            case Trie::Yes: {
                auto result = words_.emplace(word_cpy, false);
                // If word is discovered for the first time
                if (result.second) {
                    max_score_ += points_for_word_len(word_cpy.size());
                }
            }
            case Trie::Maybe:
                // Explore 8 surrouding cells
                explore(i - 1, j - 1, visited_cpy, word_cpy);
                explore(i - 1, j,     visited_cpy, word_cpy);
                explore(i - 1, j + 1, visited_cpy, word_cpy);
                explore(i,     j - 1, visited_cpy, word_cpy);
                explore(i,     j + 1, visited_cpy, word_cpy);
                explore(i + 1, j - 1, visited_cpy, word_cpy);
                explore(i + 1, j,     visited_cpy, word_cpy);
                explore(i + 1, j + 1, visited_cpy, word_cpy);
                break;
            case Trie::No:
                break;
            }
        }
    }
}
