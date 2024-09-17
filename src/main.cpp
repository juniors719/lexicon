#include <iostream>

#include "../include/Lexicon.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> <dictionary_type>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::string dictionaryType = argv[2];

    Lexicon lexicon(filename, dictionaryType);
    try {
        lexicon.open_files();
        lexicon.run();
        lexicon.write_output();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}