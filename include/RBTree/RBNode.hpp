/**
 * @file RBNode.hpp
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Classe que representa um nó de uma árvore rubro-negra
 * @version 0.1
 * @date 17-09-2024
 *
 *
 */

#pragma once

// Definição das cores usando valores booleanos. RED = false, BLACK = true
#define RED false
#define BLACK true

/**
 * @brief Classe que representa um nó de uma árvore rubro-negra
 *
 * @tparam Key tipo da chave
 * @tparam Value tipo do valor
 */
template <typename Key, typename Value>
struct RBNode {
    Key key;         // Chave
    Value value;     // Valor
    RBNode* parent;  // Ponteiro para o pai
    RBNode* left;    // Ponteiro para o filho à esquerda
    RBNode* right;   // Ponteiro para o filho à direita
    bool color;      // Cor do nó

    /**
     * @brief Constrói um novo objeto RBNode
     *
     * @param key chave
     * @param value valor
     * @param parent pai (default = nullptr)
     * @param left filho à esquerda (default = nullptr)
     * @param right filho à direita (default = nullptr)
     * @param color cor do nó (default = RED)
     */
    RBNode(Key key,
           Value value,
           RBNode* parent = nullptr,
           RBNode* left = nullptr,
           RBNode* right = nullptr,
           bool color = RED) : key(key),
                               value(value),
                               parent(parent),
                               left(left),
                               right(right),
                               color(color) {}
};