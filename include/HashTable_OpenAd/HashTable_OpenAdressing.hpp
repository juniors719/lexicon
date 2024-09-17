/**
 * @file HashTable_OpenAdressing.hpp
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Classe que representa uma tabela hash com tratamento de colisões por endereçamento aberto
 * @version 0.1
 * @date 10-09-2024
 *
 *
 */

#pragma once

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "HashTable_O_Exceptions.hpp"

/**
 * @brief Classe que representa uma tabela hash com tratamento de colisões por endereçamento aberto
 *
 * @tparam Key tipo da chave
 * @tparam Value tipo do valor
 * @tparam Hash tipo da função de hash (default = std::hash<Key>)
 */
template <typename Key, typename Value, typename Hash = std::hash<Key>>
class HashTable_OpenAddressing {
   private:
    /**
     * @brief Enum que representa o estado de uma entrada na tabela
     *
     */
    enum EntryState { EMPTY,
                      OCCUPIED,
                      DELETED };

    /**
     * @brief Struct que representa uma entrada na tabela
     * A entrada é composta por uma chave, um valor e um estado, que pode ser EMPTY, OCCUPIED ou DELETED.
     *
     * EMPTY: A entrada está vazia
     *
     * OCCUPIED: A entrada está ocupada
     *
     * DELETED: A entrada foi deletada
     *
     */
    struct Entry {
        Key key;
        Value value;
        EntryState state;

        Entry() : state(EMPTY) {}
        Entry(const Key& k, const Value& v, EntryState s = OCCUPIED) : key(k), value(v), state(s) {}
    };

    size_t m_number_of_elements;  // Número de elementos
    size_t m_table_size;          // Tamanho da tabela
    std::vector<Entry>* m_table;  // Tabela - Vetor de entradas
    float m_load_factor;          // Fator de carga - Limite para o número de elementos na tabela
    float m_max_load_factor;      // Fator de carga máximo
    Hash m_hashing;               // Função de hash

    mutable size_t _n_comparisons = 0;  // Número de comparações

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
     * @param i índice
     * @return size_t - índice da chave
     */
    size_t hash_code(const Key& k, size_t i) const {
        return (m_hashing(k) + i * hash2(k)) % m_table_size;
    }

    /**
     * @brief Função que retorna o índice da chave na tabela
     *
     * @param k chave
     * @return size_t - índice da chave
     */
    size_t hash2(const Key& k) const {
        return 1 + (m_hashing(k) % (m_table_size - 1));
    }

    /**
     * @brief Função que realoca a tabela
     *
     * @param new_size novo tamanho da tabela
     */
    void rehash(size_t new_size) {
        std::vector<Entry>* new_table = new std::vector<Entry>(new_size);
        for (size_t i = 0; i < m_table_size; ++i) {
            if ((*m_table)[i].state == OCCUPIED) {
                size_t j = 0;
                size_t index;
                do {
                    ++_n_comparisons;
                    index = (m_hashing((*m_table)[i].key) + j) % new_size;
                    j++;
                } while ((*new_table)[index].state == OCCUPIED);
                (*new_table)[index] = std::move((*m_table)[i]);
            }
            ++_n_comparisons;
        }
        delete m_table;
        m_table = new_table;
        m_table_size = new_size;
    }

   public:
    /**
     * @brief Constrói um novo objeto HashTable_OpenAddressing
     *
     * @param tableSize tamanho da tabela (default = 19)
     */
    HashTable_OpenAddressing(size_t tableSize = 19) {
        m_table_size = get_next_prime(tableSize);
        m_table = new std::vector<Entry>(m_table_size);
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
     * @brief Função que retorna se a tabela está vazia
     *
     * @return Retorna true se a tabela estiver vazia, false caso contrário
     */
    bool empty() const {
        return m_number_of_elements == 0;
    }

    /**
     * @brief Função que limpa a tabela
     *
     */
    void clear() {
        for (auto& entry : *m_table) {
            entry.state = EMPTY;
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
     * @brief Destrói o objeto HashTable_OpenAddressing
     *
     */
    ~HashTable_OpenAddressing() {
        delete m_table;
    }

    /**
     * @brief Função que adiciona um par chave-valor na tabela.
     *
     * Se a chave já existir na tabela, uma exceção é lançada.
     * Se a tabela estiver cheia, uma exceção é lançada.
     *
     * @param k chave
     * @param v valor
     */
    bool add(const Key& k, const Value& v) {
        if (static_cast<float>(m_number_of_elements) / m_table_size > m_load_factor) {
            rehash(get_next_prime(m_table_size * 2));
        }
        size_t i = 0;
        size_t index;
        do {
            ++_n_comparisons;
            index = hash_code(k, i);
            if ((*m_table)[index].state == EMPTY || (*m_table)[index].state == DELETED) {
                ++_n_comparisons;
                (*m_table)[index] = Entry(k, v, OCCUPIED);
                m_number_of_elements++;
                return true;
            } else if ((*m_table)[index].state == OCCUPIED && (*m_table)[index].key == k) {
                ++_n_comparisons;
                return false;
            }
            i++;
        } while (i < m_table_size);
        return false;
    }

    /**
     * @brief Função que remove uma chave da tabela
     *
     * @param k chave
     * @return true se a chave foi removida, false caso contrário
     */
    bool remove(const Key& k) {
        size_t i = 0;
        size_t index;
        do {
            ++_n_comparisons;
            index = hash_code(k, i);
            if ((*m_table)[index].state == OCCUPIED && (*m_table)[index].key == k) {
                (*m_table)[index].state = DELETED;
                m_number_of_elements--;
                return true;
            } else if ((*m_table)[index].state == EMPTY) {
                return false;
            }
            i++;
        } while (i < m_table_size);
        return false;
    }

    /**
     * @brief Função que redimensiona a tabela.
     * Só redimensiona a tabela se o número de elementos for maior que o tamanho da tabela multiplicado pelo fator de carga.
     *
     * @param n novo tamanho da tabela
     */
    void reserve(size_t n) {
        if (m_number_of_elements > m_table_size * m_load_factor) {
            rehash(get_next_prime(n));
        }
    }

    /**
     * @brief Função que define o fator de carga.
     * O fator de carga deve ser maior que 0 e menor ou igual ao fator de carga máximo.
     *
     * Se o fator de carga for inválido, uma exceção é lançada.
     * Se o fator de carga for válido, a tabela é redimensionada.
     *
     * @param lf fator de carga
     */
    void load_factor(float lf) {
        if (lf <= 0 || lf > m_max_load_factor) {
            throw HTO_InvalidLoadFactorException();
        }
        m_load_factor = lf;
        reserve(m_number_of_elements);
    }

    /**
     * @brief Sobrecarga do operador []
     *
     * Se a chave não existir na tabela, ela é adicionada com o valor padrão.
     *
     * @param k chave
     * @return Value& - referência para o valor
     */
    Value& operator[](const Key& k) {
        size_t i = 0;
        size_t index;
        do {
            ++_n_comparisons;
            index = hash_code(k, i);
            if ((*m_table)[index].state == OCCUPIED && (*m_table)[index].key == k) {
                ++_n_comparisons;
                return (*m_table)[index].value;
            } else if ((*m_table)[index].state == EMPTY) {
                ++_n_comparisons;
                break;
            }
            i++;
        } while (i < m_table_size);
        add(k, Value());
        return (*m_table)[index].value;
    }

    /**
     * @brief Sobrecarga do operador []
     *
     * Se a chave não existir na tabela, uma exceção é lançada.
     *
     * @param k chave
     * @return Value& - referência constante para o valor
     */
    const Value& operator[](const Key& k) const {
        size_t i = 0;
        size_t index;
        do {
            ++_n_comparisons;
            index = hash_code(k, i);
            if ((*m_table)[index].state == OCCUPIED && (*m_table)[index].key == k) {
                ++_n_comparisons;
                return (*m_table)[index].value;
            } else if ((*m_table)[index].state == EMPTY) {
                ++_n_comparisons;
                break;
            }
            i++;
        } while (i < m_table_size);
        throw HTO_KeyNotFoundException();
    }

    /**
     * @brief Função que retorna se a chave existe na tabela
     *
     * @param k chave
     * @return true se a chave existe na tabela, false caso contrário
     */
    bool contains(const Key& k) {
        size_t i = 0;
        size_t index;
        do {
            ++_n_comparisons;
            index = hash_code(k, i);
            if ((*m_table)[index].state == OCCUPIED && (*m_table)[index].key == k) {
                ++_n_comparisons;
                return true;
            } else if ((*m_table)[index].state == EMPTY) {
                ++_n_comparisons;
                return false;
            }
            i++;
        } while (i < m_table_size);
        return false;
    }

    /**
     * @brief Função que retorna o valor associado a uma chave
     *
     * Se a chave não existir na tabela, uma exceção é lançada.
     *
     * @param k chave
     * @return Value& - referência para o valor
     */
    Value& at(const Key& k) {
        size_t i = 0;
        size_t index;
        do {
            ++_n_comparisons;
            index = hash_code(k, i);
            if ((*m_table)[index].state == OCCUPIED && (*m_table)[index].key == k) {
                ++_n_comparisons;
                return (*m_table)[index].value;
            } else if ((*m_table)[index].state == EMPTY) {
                ++_n_comparisons;
                break;
            }
            i++;
        } while (i < m_table_size);
        throw HTO_KeyNotFoundException();
    }

    /**
     * @brief Função que retorna o valor associado a uma chave
     *
     * Se a chave não existir na tabela, uma exceção é lançada.
     *
     * @param k chave
     * @return Value& - referência constante para o valor
     */
    const Value& at(const Key& k) const {
        size_t i = 0;
        size_t index;
        do {
            ++_n_comparisons;
            index = hash_code(k, i);
            if ((*m_table)[index].state == OCCUPIED && (*m_table)[index].key == k) {
                ++_n_comparisons;
                return (*m_table)[index].value;
            } else if ((*m_table)[index].state == EMPTY) {
                ++_n_comparisons;
                break;
            }
            i++;
        } while (i < m_table_size);
        throw HTC_KeyNotFoundException();
    }

    /**
     * @brief Sobrecarga do operador <<
     *
     * @param os stream de saída
     * @param ht tabela hash
     * @return std::ostream& - stream de saída
     */
    friend std::ostream& operator<<(std::ostream& os, const HashTable_OpenAddressing& ht) {
        for (size_t i = 0; i < ht.m_table_size; ++i) {
            os << i << ": ";
            if ((*ht.m_table)[i].state == OCCUPIED) {
                os << "(" << (*ht.m_table)[i].key << ", " << (*ht.m_table)[i].value << ")";
            } else if ((*ht.m_table)[i].state == DELETED) {
                os << "DELETED";
            } else {
                os << "EMPTY";
            }
            os << std::endl;
        }
        return os;
    }

    //* ITERATOR *//

    /**
     * @brief Classe que representa um iterador para a tabela hash
     *
     */
    class Iterator {
       private:
        size_t m_index;                        // Índice
        const HashTable_OpenAddressing* m_ht;  // Ponteiro para a tabela hash
        std::pair<Key, Value> current_pair;    // Par chave-valor atual

       public:
        /**
         * @brief Constrói um novo objeto Iterator
         *
         * @param ht ponteiro para a tabela hash
         * @param index índice
         */
        Iterator(const HashTable_OpenAddressing* ht, size_t index) : m_ht(ht), m_index(index) {
            if (m_index < m_ht->m_table_size && (*m_ht->m_table)[m_index].state != OCCUPIED) {
                operator++();
            }
        }

        /**
         * @brief Sobrecarga do operador de referência
         *
         * @return std::pair<Key, Value>& - referência para o par chave-valor
         */
        std::pair<Key, Value>& operator*() {
            current_pair = std::make_pair((*m_ht->m_table)[m_index].key, (*m_ht->m_table)[m_index].value);
            return current_pair;
        }

        /**
         * @brief Sobrecarga do operador de seta
         *
         * @return std::pair<Key, Value>* - ponteiro para o par chave-valor
         */
        std::pair<Key, Value>* operator->() {
            current_pair = std::make_pair((*m_ht->m_table)[m_index].key, (*m_ht->m_table)[m_index].value);
            return &current_pair;
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
            m_index++;
            while (m_index < m_ht->m_table_size && (*m_ht->m_table)[m_index].state != OCCUPIED) {
                m_index++;
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
         * @brief Sobrecarga do operador de diferença
         *
         * @param other outro iterador
         * @return true se os iteradores forem diferentes, false caso contrário
         */
        bool operator!=(const Iterator& other) const {
            return m_index != other.m_index;
        }

        /**
         * @brief Sobrecarga do operador de igualdade
         *
         * @param other outro iterador
         * @return true se os iteradores forem iguais, false caso contrário
         */
        bool operator==(const Iterator& other) const {
            return m_index == other.m_index;
        }
    };

    /**
     * @brief Função que retorna um iterador para o início da tabela
     *
     * @return Iterator - iterador
     */
    Iterator begin() const {
        size_t index = 0;
        if ((*m_table)[index].state != OCCUPIED) {
            index++;
        }
        return Iterator(this, index);
    }

    /**
     * @brief Função que retorna um iterador para o final da tabela
     *
     * @return Iterator - iterador
     */
    Iterator end() const {
        return Iterator(this, m_table_size);
    }

    /**
     * @brief Função que retorna o número de comparações
     *
     * @return size_t - número de comparações
     */
    size_t getComparisons() const {
        return _n_comparisons;
    }
};
