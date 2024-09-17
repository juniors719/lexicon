/**
 * @file Lexicon.hpp
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Classe que gerencia a leitura de palavras, processamento e armazenamento em diferentes tipos de dicionários.
 * @version 0.1
 * @date 03-09-2024
 *
 *
 */

#pragma once

#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "Dictionary.hpp"

/**
 * @brief Classe que gerencia a leitura de palavras, processamento e armazenamento em diferentes tipos de dicionários.
 *
 * A classe Lexicon lê palavras de um arquivo de entrada, processa essas palavras e as armazena em um dicionário
 * baseado em uma estrutura de dados especificada (AVL, Rubro-Negra, Tabela Hash com Encadeamento ou Endereçamento Aberto).
 * Após o processamento, gera um arquivo de saída com as palavras ordenadas e informações sobre o tempo de execução,
 * o número de palavras, comparações e rotações (se aplicável).
 */
class Lexicon {
    std::ifstream input_file;           // Arquivo de entrada para leitura das palavras
    std::ofstream output_file;          // Arquivo de saída para escrita das palavras ordenadas
    std::string input_filename;         // Nome do arquivo de entrada
    std::string output_filename;        // Nome do arquivo de saída
    std::string dictionary_type;        // Tipo de dicionário utilizado ("avl", "rb", "htc", "hto")
    std::chrono::milliseconds elapsed;  // Tempo para processamento das palavras e criação do dicionário,
                                        // sem considerar a escrita do arquivo de saída e ordenação (casos da HashTable)

    Dictionary* dictionary;  // Dicionário utilizado para armazenar as palavras

    /**
     * @brief Cria um nome de arquivo com base na data e hora atuais.
     *
     * @return std::string - Nome do arquivo gerado.
     */
    std::string create_filename() {
        std::time_t now = std::time(nullptr);
        std::tm* timeinfo = std::localtime(&now);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H-%M-%S", timeinfo);
        return buffer;
    }

    /**
     * @brief Grava uma palavra no arquivo de saída.
     *
     * @param word Palavra a ser gravada.
     */
    void write_output(std::string word) {
        output_file << word << std::endl;
    }

    /**
     * @brief Processa uma palavra UTF-8, convertendo-a para Unicode e limpando caracteres indesejados.
     *
     * @param utf8_word Palavra em formato UTF-8.
     * @return icu::UnicodeString - Palavra processada em formato Unicode.
     */
    icu::UnicodeString processWord(const std::string& utf8_word) {
        icu::UnicodeString word = icu::UnicodeString::fromUTF8(utf8_word);
        icu::UnicodeString lowercaseWord = word.toLower();

        icu::UnicodeString cleanedUstr;
        bool hasHyphen = false;
        bool started = false;

        for (int32_t i = 0; i < lowercaseWord.length(); ++i) {
            UChar32 c = lowercaseWord.char32At(i);

            if (u_isalpha(c)) {
                cleanedUstr.append(c);
                hasHyphen = false;
                started = true;
            } else if (c == 0x002D && !hasHyphen && started) {
                cleanedUstr.append(c);
                hasHyphen = true;
            } else if (started) {
                // Interrompe o loop se um caractere não alfabético for encontrado após o início
                break;
            }
        }

        // Remove hífens finais
        while (cleanedUstr.length() > 0 && cleanedUstr.char32At(cleanedUstr.length() - 1) == 0x002D) {
            cleanedUstr.truncate(cleanedUstr.length() - 1);
        }

        return cleanedUstr;
    }

   public:
    /**
     * @brief Construtor da classe Lexicon.
     *
     * Inicializa o tipo de dicionário com base no parâmetro fornecido e define os nomes dos arquivos de entrada e saída.
     *
     * @param inp_file Nome do arquivo de entrada.
     * @param dictionary_type Tipo de dicionário a ser usado ("avl", "rb", "htc", "hto").
     * @throws std::runtime_error - Se o tipo de dicionário for inválido.
     */
    Lexicon(std::string inp_file, std::string dictionary_type) : dictionary_type(dictionary_type) {
        if (dictionary_type == "avl")
            dictionary = new AVL_Dictionary();
        else if (dictionary_type == "rb")
            dictionary = new RB_Dictionary();
        else if (dictionary_type == "htc")
            dictionary = new HashTableC_Dictionary();
        else if (dictionary_type == "hto")
            dictionary = new HashTableOA_Dictionary();
        else
            throw std::runtime_error("Tipo de dicionário inválido");
        this->input_filename = "../data/in/" + inp_file;
        this->output_filename = "../data/out/" + create_filename() + "_" + dictionary_type + ".txt";
    }

    /**
     * @brief Destrutor da classe Lexicon.
     *
     * Libera a memória alocada para o dicionário e fecha os arquivos abertos.
     */
    ~Lexicon() {
        delete dictionary;
        input_file.close();
        output_file.close();
    }

    /**
     * @brief Abre o arquivos de entrada.
     *
     * Abre o arquivo de entrada para leitura.
     * Lança uma exceção se o arquivo de entrada não puder ser aberto.
     */
    void open_files() {
        input_file.open(input_filename);
        if (!input_file.is_open()) {
            throw std::runtime_error("Falha ao abrir arquivo de entrada");
        } else
            std::cout << "Arquivo de entrada aberto com sucesso: " << input_filename << std::endl;
    }

    /**
     * @brief Executa o processamento das palavras.
     *
     * Lê palavras do arquivo de entrada, processa cada palavra e insere no dicionário.
     * Mede o tempo total de execução do processamento.
     */
    void run() {
        auto start = std::chrono::high_resolution_clock::now();  // inicio da contagem de milisegundos
        std::string word;
        while (input_file >> word) {
            icu::UnicodeString processed_word = processWord(word);
            if (!processed_word.isEmpty()) dictionary->insert(processed_word);
        }
        auto finish = std::chrono::high_resolution_clock::now();  // fim da contagem de milisegundos
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    }

    /**
     * @brief Gera o arquivo de saída com os resultados do processamento.
     *
     * Cria um arquivo de saída com o número de palavras, tempo de execução, comparações e rotações (se aplicável),
     * além de gravar as palavras ordenadas no arquivo.
     *
     * @throws std::runtime_error - Se o arquivo de saída não puder ser gerado.
     */
    void write_output() {
        output_file.open(output_filename);
        if (!output_file.is_open()) {
            throw std::runtime_error("Falha ao gerar arquivo de saída");
        } else
            std::cout << "Arquivo de saída gerado com sucesso: " << output_filename << std::endl;
        output_file << "---------- LEXICON ----------\n\n";
        output_file << "Número de palavras: " << dictionary->size() << "\n";
        output_file << "Tempo de execução: " << elapsed.count() << " ms\n";
        output_file << "Quantidade de comparações: " << dictionary->getComparisons() << std::endl;
        if (dictionary_type == "avl" || dictionary_type == "rb") {
            output_file << "Quantidade de rotações: " << dictionary->getRotations() << std::endl;
        }
        output_file << "\n\nPalaras ordenadas:\n\n";

        output_file << dictionary->getOrdenedDictionary();
    }

    /**
     * @brief Obtém o tempo total de execução em milissegundos.
     *
     * @return size_t - Tempo total de execução em milissegundos.
     */
    size_t elapsed_time() { return elapsed.count(); }
};