/**
 * @file HashTable_Chaining.hpp
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Classe que representa uma tabela hash com tratação de colisões por encadeamento
 * @version 0.1
 * @date 10-09-2024
 *
 *
 */

#pragma once

#include <cmath>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "HashTable_C_Exceptions.hpp"

/**
 * @brief Classe que representa uma tabela hash com tratamento de colisões por encadeamento
 *
 * @tparam Key tipo da chave
 * @tparam Value tipo do valor
 * @tparam Hash tipo da função de hash (default = std::hash<Key>)
 */
template <typename Key, typename Value, typename Hash = std::hash<Key>>
class HashTable_Chaining {
   private:
    size_t m_number_of_elements;                             // Número de elementos
    size_t m_table_size;                                     // Tamanho da tabela
    std::vector<std::list<std::pair<Key, Value>>>* m_table;  // Tabela
    float m_load_factor;                                     // Fator de carga
    float m_max_load_factor;                                 // Fator de carga máximo
    Hash m_hashing;                                          // Função de hash

    mutable std::size_t _n_comparisons = 0;  // Número de comparações

    /**
     * @brief Função que retorna o próximo número primo após x
     *
     * @param x número atual para encontrar o próximo primo
     * @return size_t - próximo número primo
     */
    size_t get_next_prime(size_t x) {
        if (x <= 2) return 3;
        x = (x % 2 == 0) ? x + 1 : x;
        bool not_prime = true;
        while (not_prime) {
            not_prime = false;
            for (int i = 2; i <= sqrt(x); ++i) {
                if (x % i == 0) {
                    not_prime = true;
                    break;
                }
            }
            x += 2;
        }
        return x - 2;
    }

    /**
     * @brief Função que retorna o índice da chave na tabela
     *
     * @param k chave
     * @return size_t - índice da chave
     */
    size_t hash_code(const Key& k) const {
        return m_hashing(k) % m_table_size;
    }

    /**
     * @brief Função que realoca a tabela
     *
     * Essa função é chamada quando o fator de carga é maior que o fator de carga máximo
     * Cria uma nova tabela com o dobro do tamanho da tabela atual e realoca os elementos.
     *
     * @param new_size novo tamanho da tabela
     */
    void rehash(size_t new_size) {
        std::vector<std::list<std::pair<Key, Value>>>* new_table = new std::vector<std::list<std::pair<Key, Value>>>(new_size);
        for (size_t i = 0; i < m_table_size; ++i) {
            for (auto& par : (*m_table)[i]) {
                size_t index = m_hashing(par.first) % new_size;
                (*new_table)[index].push_back(par);
            }
        }
        delete m_table;
        m_table = new_table;
        m_table_size = new_size;
    }

   public:
    /**
     * @brief Constrói um novo objeto HashTable_Chaining
     *
     * @param tableSize tamanho da tabela (default = 19)
     */
    HashTable_Chaining(size_t tableSize = 19) {
        m_table_size = get_next_prime(tableSize);
        m_table = new std::vector<std::list<std::pair<Key, Value>>>(m_table_size);
        m_number_of_elements = 0;
        m_load_factor = 0.75;
        m_max_load_factor = 1.0;
    }

    /**
     * @brief Função que retorna o número de elementos na tabela
     *
     * @return size_t - número de elementos
     */
    size_t size() const {
        return m_number_of_elements;
    }

    /**
     * @brief Função que verifica se a tabela está vazia
     *
     * @return true caso a tabela esteja vazia, e false caso contrário
     */
    bool empty() const {
        return m_number_of_elements == 0;
    }

    /**
     * @brief Função que limpa a tabela
     *
     */
    void clear() {
        for (auto& lista : *m_table) {
            lista.clear();
        }
        m_number_of_elements = 0;
    }

    /**
     * @brief Função que retorna o fator de carga
     *
     * @return float - fator de carga
     */
    float load_factor() const {
        return m_load_factor;
    }

    /**
     * @brief Função que retorna o fator de carga máximo
     *
     * @return float - fator de carga máximo
     */
    float max_load_factor() const {
        return m_max_load_factor;
    }

    /**
     * @brief Destrói o objeto HashTable_Chaining
     *
     */
    ~HashTable_Chaining() {
        delete m_table;
    }

    /**
     * @brief Função que adiciona um par chave-valor na tabela
     * Retorna uma exceção caso a chave já exista na tabela
     *
     * @param k chave
     * @param v valor
     */
    void add(const Key& k, const Value& v) {
        if (static_cast<float>(m_number_of_elements) / m_table_size > m_load_factor) {
            rehash(m_table_size * 2);
        }
        size_t index = hash_code(k);
        for (auto& list : (*m_table)[index]) {
            if (list.first == k) {
                throw HTC_AlreadyExistsException();
            }
        }
        (*m_table)[index].push_back({k, v});
        m_number_of_elements++;
    }

    /**
     * @brief Função que remove um par chave-valor da tabela
     * Retorna uma exceção caso a chave não exista na tabela
     *
     * @param k chave
     */
    void remove(const Key& k) {
        size_t index = hash_code(k);
        for (auto it = (*m_table)[index].begin(); it != (*m_table)[index].end(); ++it) {
            if (it->first == k) {
                ++_n_comparisons;
                (*m_table)[index].erase(it);
                --m_number_of_elements;
            }
        }
        throw HTC_KeyNotFoundException();
    }

    /**
     * @brief Função que retorna o tamanho da tabela
     * Só fará a realocação se o número de elementos for maior que o tamanho da tabela multiplicado pelo fator de carga
     *
     * @param n novo tamanho da tabela
     *
     * @return size_t - tamanho da tabela
     */
    void reserve(size_t n) {
        if (m_number_of_elements > m_table_size * m_load_factor) {
            rehash(n);
        }
    }

    /**
     * @brief Função que define o fator de carga
     * Retorna uma exceção caso o fator de carga seja inválido.
     * A função reserve é chamada para realocar a tabela.
     *
     * @param lf fator de carga
     */
    void load_factor(float lf) {
        if (lf <= 0 || lf > m_max_load_factor) {
            throw HTC_InvalidLoadFactorException();
        }
        m_load_factor = lf;
        reserve(m_number_of_elements);
    }

    /**
     * @brief Sobrecarga do operador []
     * Retorna uma referência para o valor associado à chave.
     * Caso a chave não exista, a função add é chamada.
     *
     * @param k chave
     * @return Value& - referência para o valor associado à chave
     */
    Value& operator[](const Key& k) {
        size_t index = hash_code(k);
        for (auto& par : (*m_table)[index]) {
            if (par.first == k) {
                ++_n_comparisons;
                return par.second;
            }
        }
        add(k, Value());
        return (*m_table)[index].back().second;
    }

    /**
     * @brief Sobrecarga do operador []
     * Retorna uma referência constante para o valor associado à chave.
     * Retorna uma exceção caso a chave não exista na tabela.
     *
     * @param k chave
     * @return const Value& - referência constante para o valor associado à chave
     */
    const Value& operator[](const Key& k) const {
        size_t index = hash_code(k);
        for (auto& par : (*m_table)[index]) {
            if (par.first == k) {
                ++_n_comparisons;
                return par.second;
            }
        }
        throw HTC_KeyNotFoundException();
    }

    /**
     * @brief Função que verifica se uma chave existe na tabela
     *
     * @param k chave
     * @return true caso a chave exista na tabela, ou false caso contrário
     */
    bool contains(const Key& k) {
        size_t index = hash_code(k);
        for (auto& par : (*m_table)[index]) {
            if (par.first == k) {
                ++_n_comparisons;
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Função que retorna o valor associado a uma chave
     * Retorna uma exceção caso a chave não exista na tabela
     *
     * @param k chave
     * @return Value& - referência para o valor associado à chave
     */
    Value& at(const Key& k) {
        size_t index = hash_code(k);
        for (auto& par : (*m_table)[index]) {
            if (par.first == k) {
                ++_n_comparisons;
                return par.second;
            }
        }
        throw HTC_KeyNotFoundException();
    }

    /**
     * @brief Função constante que retorna o valor associado a uma chave
     * Retorna uma exceção caso a chave não exista na tabela
     *
     * @param k chave
     * @return Value& - referência para o valor associado à chave
     */
    const Value& at(const Key& k) const {
        size_t index = hash_code(k);
        for (auto& par : (*m_table)[index]) {
            if (par.first == k) {
                ++_n_comparisons;
                return par.second;
            }
        }
        throw HTC_KeyNotFoundException();
    }

    /**
     * @brief Sobrecarga do operador <<
     * Imprime a tabela
     *
     * @param os stream de saída
     * @param ht tabela
     * @return std::ostream& - stream de saída
     */
    friend std::ostream& operator<<(std::ostream& os, const HashTable_Chaining& ht) {
        for (size_t i = 0; i < ht.m_table_size; ++i) {
            os << i << ": ";
            for (auto& par : (*ht.m_table)[i]) {
                os << "(" << par.first << ", " << par.second << ") ";
            }
            os << std::endl;
        }
        return os;
    }

    /**
     * @brief Função que retorna o número de comparações
     *
     * @return std::size_t - número de comparações
     */
    std::size_t getComparisons() const {
        return _n_comparisons;
    }

    //* ITERATOR *//

    /**
     * @brief Classe que representa um iterador para a tabela hash
     *
     */
    class Iterator {
       private:
        size_t m_index;                                            // Índice da tabela
        size_t m_list_index;                                       // Índice da lista
        const HashTable_Chaining* m_ht;                            // Ponteiro para a tabela
        typename std::list<std::pair<Key, Value>>::iterator m_it;  // Iterador para a lista

       public:
        /**
         * @brief Constrói um novo objeto Iterator
         *
         * @param ht tabela
         * @param index índice da tabela
         * @param list_index índice da lista
         */
        Iterator(const HashTable_Chaining* ht, size_t index, size_t list_index) : m_ht(ht), m_index(index), m_list_index(list_index) {
            if (m_index < m_ht->m_table_size) {
                m_it = (*m_ht->m_table)[m_index].begin();
                for (size_t i = 0; i < m_list_index; ++i) {
                    m_it++;
                }
            }
        }

        /**
         * @brief Sobrecarga do operador de referência
         *
         * @return std::pair<Key, Value>& - referência para o par chave-valor
         */
        std::pair<Key, Value>& operator*() {
            return *m_it;
        }

        /**
         * @brief Sobrecarga do operador de seta
         *
         * @return std::pair<Key, Value>* - ponteiro para o par chave-valor
         */
        std::pair<Key, Value>* operator->() {
            return &(*m_it);
        }

        /**
         * @brief Sobrecarga do operador de pré-incremento
         *
         * @return Iterator& - referência para o iterador
         */
        Iterator& operator++() {
            if (m_index >= m_ht->m_table_size) {
                return *this;
            }
            m_it++;
            while (m_index < m_ht->m_table_size && m_it == (*m_ht->m_table)[m_index].end()) {
                m_index++;
                m_it = (*m_ht->m_table)[m_index].begin();
            }
            return *this;
        }

        /**
         * @brief Sobrecarga do operador de pós-incremento
         *
         * @param int
         * @return Iterator - iterador
         */
        Iterator& operator++(int) {
            Iterator it = *this;
            ++(*this);
            return it;
        }

        /**
         * @brief Sobrecarga do operador de pós-incremento
         *
         * @param int
         * @return Iterator - iterador
         */
        bool operator!=(const Iterator& other) const {
            return m_index != other.m_index;
        }

        /**
         * @brief Sobrecarga do operador de igualdade
         *
         * @param other outro iterador
         * @return bool - true se os iteradores são iguais, ou false caso contrário
         */
        bool operator==(const Iterator& other) const {
            return m_index == other.m_index;
        }
    };

    /**
     * @brief Cria um iterador para o início da tabela
     *
     * @return Iterator - iterador para o início da tabela
     */
    Iterator begin() const {
        size_t index = 0;
        while (index < m_table_size && (*m_table)[index].empty()) {
            index++;
        }
        return Iterator(this, index, 0);
    }

    /**
     * @brief Cria um iterador para o final da tabela
     *
     * @return Iterator - iterador para o final da tabela
     */
    Iterator end() const {
        return Iterator(this, m_table_size, 0);
    }
};