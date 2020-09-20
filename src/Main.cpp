#include "Boggle.hpp"

#include <iostream>


void select_dice(Boggle& boggle)
{
    std::cout <<
        "Select dice:\n"
        "  1. Englih\n"
        "  2. French\n"
        "  3. Custom\n"
        ">> ";
    int choice;
    std::cin >> choice;
    switch (choice) {
    case 1:
        boggle.generate_grid(Boggle::English);
        break;
    case 2:
        boggle.generate_grid(Boggle::French);
        break;
    case 3: {
        std::string chars;
        std::cout << "Enter dice faces (" << Boggle::ARRAY_SIZE << " letters): ";
        std::cin >> chars;
        boggle.set_grid(chars);
        break;
    }
    default:
        std::cout << "Unsupported\n";
    }
}


void select_dictionary(Boggle& boggle)
{
    std::cout <<
        "Select dict:\n"
        "  1. English\n"
        "  2. French\n"
        "  3. Custom\n"
        ">> ";
    int choice;
    std::cin >> choice;
    std::string dict_path;
    switch (choice) {
    case 1:
        dict_path = "/usr/share/dict/american-english";
        break;
    case 2:
        dict_path = "/usr/share/dict/french";
        break;
    case 3: {
        std::cout << "Path: ";
        std::cin >> dict_path;
        break;
    }
    default:
        std::cout << "Unsupported\n";
    }
    if (!boggle.load_dictionary(dict_path)) {
        std::cerr << "cannot load " << dict_path << std::endl;
    }
}


int main()
{
    Boggle boggle;
    select_dictionary(boggle);
    select_dice(boggle);

    std::cout <<
        "Enter word, or:\n"
        "[g] print grid\n"
        "[n] next grid\n"
        "[s] print solution\n"
        "[q] quit\n\n";
    boggle.print_grid();
    std::string input;
    bool running = true;
    while (running) {
        std::cout << ">> ";
        std::cin >> input;
        if (input == "g") {
            boggle.print_grid();
        } else if (input == "s") {
            boggle.print_solution();
        } else if (input == "n") {
            select_dice(boggle);
            boggle.print_grid();
        } else if (input == "q") {
            running = false;
        } else {
            boggle.submit_word(input);
        }
    }

    return 0;
}
