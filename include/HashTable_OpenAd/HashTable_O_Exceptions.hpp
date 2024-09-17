/**
 * @file HashTable_O_Exceptions.hpp
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Classes de exceção para a tabela de dispersão com encadeamento
 * @version 0.1
 * @date 10-09-2024
 *
 *
 */

#pragma once

#include <stdexcept>

/**
 * @brief Classe de exceção para quando uma chave não é encontrada na tabela
 *
 */
class HTO_KeyNotFoundException : public std::exception {
   public:
    const char* what() const noexcept override { return "Key not found in the hash table"; }
};

/**
 * @brief Classe de exceção para quando a tabela está vazia
 *
 */
class HTO_EmptyTableException : public std::exception {
   public:
    const char* what() const noexcept override { return "Table is empty"; }
};

/**
 * @brief Classe de exceção para quando o fator de carga é inválido
 *
 */
class HTO_InvalidLoadFactorException : public std::exception {
   public:
    const char* what() const noexcept override { return "Invalid load factor"; }
};

/**
 * @brief Classe de exceção para quando uma chave já existe na tabela
 *
 */
class HTO_AlreadyExistsException : public std::exception {
   public:
    const char* what() const noexcept override { return "Key already exists in the hash table"; }
};

class HTO_TableFullException : public std::exception {
   public:
    const char* what() const noexcept override { return "Table is full"; }
};