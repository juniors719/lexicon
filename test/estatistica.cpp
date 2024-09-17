#include <iostream>

#include "../include/Lexicon.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> <dictionary_type>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::string dictionaryType = argv[2];

    std::vector<std::size_t> elapsed_times;
    std::size_t total_time = 0;
    std::size_t size_of_test = 50;

    for (int i = 0; i < size_of_test; i++) {
        Lexicon lexicon(filename, dictionaryType);
        try {
            lexicon.open_files();
            lexicon.run();
            // lexicon.write_output();
            std::size_t elapsed_time = lexicon.elapsed_time();
            std::cout << "Elapsed time: " << elapsed_time << "ms\n";
            elapsed_times.push_back(elapsed_time);
            total_time += elapsed_time;
            std::cout << "Atual average time: " << total_time / (i + 1) << "ms     |     " << i + 1 << "/" << size_of_test << std::endl;
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            return 1;
        }
        std::cout << "----------------------------------------\n";
    }

    std::size_t average_time = total_time / size_of_test;
    std::cout << "Average time: " << average_time << "ms\n";

    double variancia = 0;
    for (int i = 0; i < size_of_test; i++) {
        variancia += (elapsed_times[i] - average_time) * (elapsed_times[i] - average_time);
    }
    variancia /= size_of_test;
    double desvio_padrao = sqrt(variancia);
    std::cout << "Standard deviation: " << desvio_padrao << "ms\n";
    return 0;
}