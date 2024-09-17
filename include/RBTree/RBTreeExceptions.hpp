/**
 * @file RBTreeExceptions.hpp
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Classes de exceção para a árvore rubro-negra
 * @version 0.1
 * @date 17-09-2024
 *
 *
 */

#include <stdexcept>

/**
 * @brief Classe de exceção para quando um valor não é encontrado na árvore
 *
 */
class RB_ValueNotFoundException : public std::exception {
   public:
    const char* what() const noexcept override { return "Value not found in the tree"; }
};

/**
 * @brief Classe de exceção para quando a árvore está vazia
 *
 */
class RB_EmptyTreeException : public std::exception {
   public:
    const char* what() const noexcept override { return "Tree is empty"; }
};

/**
 * @brief Classe de exceção para quando um índice é inválido
 *
 */
class RB_InvalidIndexException : public std::exception {
   public:
    const char* what() const noexcept override { return "Invalid index"; }
};

/**
 * @brief Classe de exceção para quando um valor já existe na árvore
 *
 */
class RB_AlreadyExistsException : public std::exception {
   public:
    const char* what() const noexcept override { return "Value already exists in the tree"; }
};