/**
 * @file AVLNode.hpp
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Struct que representa um nó de uma árvore AVL
 * @version 0.1
 * @date 02-09-2024
 *
 *
 */

#pragma once

#include <string>

/**
 * @brief Struct que representa um nó de uma árvore AVL
 *
 * @tparam Key tipo da chave
 * @tparam Value tipo do valor
 */
template <typename Key, typename Value>
struct AVLNode {
    Key key{};          // Chave
    Value value{};      // Valor
    AVLNode* left{};    // Filho à esquerda
    AVLNode* right{};   // Filho à direita
    unsigned height{};  // Altura do nó

    /**
     * @brief Construtor da struct AVLNode
     *
     * @param key chave
     * @param value valor
     */
    AVLNode(Key& key, Value& value)
        : key(key),
          value(value),
          left(nullptr),
          right(nullptr),
          height(1) {}
};