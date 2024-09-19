/**
 * @file Dictionary.hpp
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Classe base para estruturas de dados de dicionário.
 * @version 0.1
 * @date 03-09-2024
 *
 *
 */

#pragma once

#include <unicode/unistr.h>

#include <string>
#include <vector>

// Estruturas de dados
#include "./AVLTree/AVLTree.hpp"
#include "./HashTable_Chaining/HashTable_Chaining.hpp"
#include "./HashTable_OpenAd/HashTable_OpenAdressing.hpp"
#include "./RBTree/RBTree.hpp"

// Utilitários
#include "UnicodeCompare.hpp"
#include "Utilities.hpp"

/**
 * @brief Classe base para estruturas de dados de dicionário.
 *
 * Esta classe define uma interface comum para diferentes implementações de dicionário.
 * As classes derivadas devem implementar a funcionalidade de inserção, exibição e obtenção de dicionários ordenados,
 * bem como fornecer informações sobre o tamanho, rotações e comparações realizadas.
 */
class Dictionary {
   public:
    virtual ~Dictionary() = default;

    /**
     * @brief Insere uma palavra no dicionário.
     *
     * @param word Palavra a ser inserida.
     */
    virtual void insert(icu::UnicodeString word) = 0;

    /**
     * @brief Obtém uma representação em string do dicionário ordenado.
     *
     * @return std::string - Representação em string do dicionário ordenado.
     */
    virtual std::string getOrdenedDictionary() = 0;

    /**
     * @brief Exibe o dicionário na saída padrão.
     */
    virtual void show() = 0;

    /**
     * @brief Obtém o número total de palavras no dicionário.
     *
     * @return size_t - Número de palavras no dicionário.
     */
    virtual size_t size() const = 0;

    /**
     * @brief Obtém o número total de rotações realizadas.
     *
     * @return size_t - Número de rotações realizadas.
     */
    virtual size_t getRotations() const = 0;

    /**
     * @brief Obtém o número total de comparações realizadas.
     *
     * @return size_t - Número de comparações realizadas.
     */
    virtual size_t getComparisons() const = 0;
};

/**
 * @brief Implementação de um dicionário utilizando uma árvore AVL.
 *
 */
class AVL_Dictionary : public Dictionary {
   private:
    AVLTree<icu::UnicodeString, size_t, UnicodeCompare> avlTree;  // Árvore AVL que armazena as palavras e suas frequências

   public:
    ~AVL_Dictionary() {
        avlTree.clear();
    }

    void insert(icu::UnicodeString word) {
        try {
            avlTree.at(word)++;
        } catch (const AVL_ValueNotFoundException& e) {
            avlTree.insert(word, 1);
        }
    }

    std::string getOrdenedDictionary() {
        std::string output;
        for (auto [key, value] : avlTree) {
            std::string utf8_key;
            key.toUTF8String(utf8_key);
            output += utf8_key + " - " + std::to_string(value) + "\n";
        }
        return output;
    }

    void show() {
        avlTree.display();
    }

    size_t size() const {
        return avlTree.size();
    }

    size_t getRotations() const {
        return avlTree.rotations();
    }

    size_t getComparisons() const {
        return avlTree.comparisons();
    }
};

/**
 * @brief Implementação de um dicionário utilizando uma árvore Rubro-Negra.
 *
 */
class RB_Dictionary : public Dictionary {
   private:
    RBTree<icu::UnicodeString, size_t, UnicodeCompare> rbTree;  // Árvore Rubro-Negra que armazena as palavras e suas frequências

   public:
    ~RB_Dictionary() {
        rbTree.clear();
    }

    void insert(icu::UnicodeString word) {
        try {
            rbTree.search(word)++;
        } catch (const RB_ValueNotFoundException& e) {
            rbTree.insert(word, 1);
        }
    }

    std::string getOrdenedDictionary() {
        std::string output;
        for (auto [key, value] : rbTree) {
            std::string utf8_key;
            key.toUTF8String(utf8_key);
            output += utf8_key + " - " + std::to_string(value) + "\n";
        }
        return output;
    }

    void show() {
        // TODO
    }

    size_t size() const {
        return rbTree.size();
    }

    size_t getRotations() const {
        return rbTree.rotations();
    }

    size_t getComparisons() const {
        return rbTree.comparisons();
    }
};

/**
 * @brief Implementação de um dicionário utilizando uma tabela hash com tratamento de colisões por encadeamento.
 *
 */
class HashTableC_Dictionary : public Dictionary {
   private:
    HashTable_Chaining<icu::UnicodeString, size_t, hash_unicode> hashTable;  // Tabela de dispersão que armazena as palavras e suas frequências

   public:
    ~HashTableC_Dictionary() {
        hashTable.clear();
    }

    void insert(icu::UnicodeString word) {
        try {
            hashTable.at(word)++;
        } catch (const HTC_KeyNotFoundException& e) {
            hashTable.add(word, 1);
        }
    }

    std::string getOrdenedDictionary() {
        std::string output;
        std::vector<std::pair<icu::UnicodeString, size_t>> elements;

        auto start = std::chrono::high_resolution_clock::now();

        // Inserindo os elementos da tabela em um vetor
        for (auto [key, value] : hashTable) {
            elements.emplace_back(key, value);
        }

        // Ordenando o vetor de elementos
        std::sort(elements.begin(), elements.end(), [](const auto& a, const auto& b) {
            UnicodeCompare cmp;
            return cmp(a.first, b.first);
        });

        auto finish = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
        std::cout << "Ordenation elapsed time: " << elapsed.count() << " ms" << std::endl;

        // Gerando a saída
        for (auto& [key, value] : elements) {
            std::string utf8_key;
            key.toUTF8String(utf8_key);
            output += utf8_key + " - " + std::to_string(value) + "\n";
        }
        return output;
    }

    void show() {
        std::cout << hashTable;
    }

    size_t size() const {
        return hashTable.size();
    }

    size_t getRotations() const {
        return 0;
    }

    size_t getComparisons() const {
        return hashTable.getComparisons();
    }
};

/**
 * @brief Implementação de um dicionário utilizando uma tabela hash com tratamento de colisões por endereçamento aberto.
 *
 */
class HashTableOA_Dictionary : public Dictionary {
   private:
    HashTable_OpenAddressing<icu::UnicodeString, size_t, hash_unicode> hashTable;  // Tabela de dispersão que armazena as palavras e suas frequências

   public:
    ~HashTableOA_Dictionary() {
        hashTable.clear();
    }

    void insert(icu::UnicodeString word) {
        try {
            hashTable.at(word)++;
        } catch (const HTO_KeyNotFoundException& e) {
            hashTable.add(word, 1);
        }
    }

    std::string getOrdenedDictionary() {
        std::string output;
        std::vector<std::pair<icu::UnicodeString, size_t>> elements;

        auto start = std::chrono::high_resolution_clock::now();

        // Inserindo os elementos da tabela em um vetor
        for (auto [key, value] : hashTable) {
            elements.emplace_back(key, value);
        }

        // Ordenando o vetor de elementos
        std::sort(elements.begin(), elements.end(), [](const auto& a, const auto& b) {
            UnicodeCompare cmp;
            return cmp(a.first, b.first);
        });

        auto finish = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
        std::cout << "Ordenation elapsed time: " << elapsed.count() << " ms" << std::endl;

        // Gerando a saída
        for (auto& [key, value] : elements) {
            std::string utf8_key;
            key.toUTF8String(utf8_key);
            output += utf8_key + " - " + std::to_string(value) + "\n";
        }
        return output;
    }

    void show() {
        std::cout << hashTable;
    }

    size_t size() const {
        return hashTable.size();
    }

    size_t getRotations() const {
        return 0;
    }

    size_t getComparisons() const {
        return hashTable.getComparisons();
    }
};
