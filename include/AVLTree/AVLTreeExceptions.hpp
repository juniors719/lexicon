/**
 * @file AVLTreeExceptions.hpp
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Classes de exceção para a árvore AVL
 * @version 0.1
 * @date 03-09-2024
 *
 *
 */

#include <stdexcept>  // std::exception

/**
 * @brief Classe de exceção para quando um valor não é encontrado na árvore
 *
 */
class AVL_ValueNotFoundException : public std::exception {
   public:
    const char* what() const noexcept override { return "Value not found in the tree"; }
};

/**
 * @brief Classe de exceção para quando a árvore está vazia
 *
 */
class AVL_EmptyTreeException : public std::exception {
   public:
    const char* what() const noexcept override { return "Tree is empty"; }
};

/**
 * @brief Classe de exceção para quando um valor já existe na árvore
 *
 */
class AVL_AlreadyExistsException : public std::exception {
   public:
    const char* what() const noexcept override { return "Value already exists in the tree"; }
};