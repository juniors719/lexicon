/**
 * @file AVLTreeIterator.hpp
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Classe que representa um iterador para a árvore AVL
 * @version 0.1
 * @date 17-09-2024
 *
 *
 */

#pragma once

#include <functional>  // std::less
#include <stack>       // Para a pilha de travessia

#include "AVLNode.hpp"

/**
 * @brief Classe que representa um iterador para a árvore AVL
 *
 * @tparam Key chave
 * @tparam Value valor
 */
template <typename Key, typename Value>
class AVLTreeIterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;  // Iterador bidirecional
    using value_type = std::pair<Key, Value>;                   // Tipo do valor
    using difference_type = std::ptrdiff_t;                     // Tipo da diferença
    using pointer = value_type*;                                // Ponteiro
    using reference = value_type&;                              // Referência
    using node_ptr = AVLNode<Key, Value>*;                      // Ponteiro para um nó

   private:
    node_ptr current;                      // Nó atual
    std::stack<node_ptr> traversal_stack;  // Pilha de travessia

    /**
     * @brief Função que empilha os nós à esquerda do nó passado
     *
     * @param node nó
     */
    void push_left(node_ptr node) {
        while (node) {
            traversal_stack.push(node);
            node = node->left;
        }
    }

   public:
    /**
     * @brief Constrói um novo objeto AVLTreeIterator
     *
     * @param root raiz da árvore
     */
    explicit AVLTreeIterator(node_ptr root) {
        push_left(root);
        if (!traversal_stack.empty()) {
            current = traversal_stack.top();
            traversal_stack.pop();
        } else
            current = nullptr;
    }

    /**
     * @brief Constrói um novo objeto AVLTreeIterator
     *
     * @param root raiz da árvore
     * @param key chave do nó
     */
    explicit AVLTreeIterator(node_ptr root, Key key) {
        node_ptr node = root;
        while (node) {
            traversal_stack.push(node);
            if (node->key == key) {
                current = node;
                return;
            }
            if (node->key > key) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        throw std::runtime_error("Node not found in AVL tree");
    }

    /**
     * @brief Constrói um novo objeto AVLTreeIterator
     *
     * @param root raiz da árvore
     * @param cur nó atual
     */
    explicit AVLTreeIterator(node_ptr root, node_ptr cur) {
        node_ptr node = root;
        while (node) {
            traversal_stack.push(node);
            if (node == cur) {
                current = node;
                return;
            }
            if (node->key > cur->key) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        current = nullptr;
    }

    /**
     * @brief Sobrecarga do operador de desreferência
     *
     * @return std::pair<Key, Value> - par chave-valor do nó atual
     */
    value_type operator*() const {
        return {current->key, current->value};
    }

    /**
     * @brief Sobrecarga do operador de seta
     *
     * @return std::pair<Key, Value>& - referencia para o par chave-valor do nó atual
     */
    reference operator->() const {
        return current->value;
    }

    /**
     * @brief Sobrecarga do operador de pré-incremento
     *
     * @return AVLTreeIterator& - iterador incrementado
     */
    AVLTreeIterator& operator++() {
        if (current->right) {
            push_left(current->right);
        }
        if (!traversal_stack.empty()) {
            current = traversal_stack.top();
            traversal_stack.pop();
        } else {
            current = nullptr;
        }
        return *this;
    }

    /**
     * @brief Sobrecarga do operador de pós-incremento
     *
     * @return AVLTreeIterator - iterador incrementado
     */
    AVLTreeIterator operator++(int) {
        AVLTreeIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    /**
     * @brief Sobrecarga do operador de igualdade
     *
     * @param other outro iterador a ser comparado
     * @return true caso os iteradores apontem para o mesmo nó, caso contrário, false
     */
    bool operator==(const AVLTreeIterator& other) const {
        return current == other.current;
    }

    /**
     * @brief Sobrecarga do operador de diferença
     *
     * @param other outro iterador a ser comparado
     * @return true caso os iteradores apontem para nós diferentes, caso contrário, false
     */
    bool operator!=(const AVLTreeIterator& other) const {
        return current != other.current;
    }
};