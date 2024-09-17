/**
 * @file RBTreeIterator.hpp
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Classe que representa um iterador para a árvore rubro-negra
 * @version 0.1
 * @date 17-09-2024
 *
 *
 */

#pragma once

/**
 * @brief Classe que representa um iterador para a árvore rubro-negra
 *
 * @tparam Key tipo da chave
 * @tparam Value tipo do valor
 */
template <typename Key, typename Value>
class RBTreeIterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::pair<Key, Value>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;
    using node_ptr = RBNode<Key, Value>*;

   private:
    node_ptr current;  // Ponteiro para o nó atual
    node_ptr nil;      // Ponteiro para o nó nulo

   public:
    /**
     * @brief Constrói um novo objeto RBTreeIterator
     *
     * @param root raiz da árvore
     * @param nil nó nulo
     */
    explicit RBTreeIterator(node_ptr root, node_ptr nil) : current(nil), nil(nil) {
        if (root != nil) {
            current = _minimum(root);  // Encontra o menor elemento da árvore
        }
    }

    /**
     * @brief Sobrecarga do operador de referência
     *
     * @return std::pair<Key, Value>& - referência para o par chave-valor
     */
    value_type operator*() const {
        return {current->key, current->value};
    }

    /**
     * @brief Sobrecarga do operador de pré-incremento
     *
     * @return RBTreeIterator&
     */
    RBTreeIterator& operator++() {
        if (current == nil) {
            throw std::out_of_range("Iterator cannot be incremented past end");
        }
        if (current->right != nil) {
            current = _minimum(current->right);
        } else {
            node_ptr p = current->parent;
            while (p != nil && current == p->right) {
                current = p;
                p = p->parent;
            }
            current = p;
        }
        return *this;
    }

    /**
     * @brief Sorecarga do operador de pós-incremento
     *
     * @return RBTreeIterator
     */
    RBTreeIterator operator++(int) {
        RBTreeIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    /**
     * @brief Sobrecarga do operador de pré-decremento
     *
     * @return RBTreeIterator&
     */
    RBTreeIterator& operator--() {
        if (current == nil) {
            throw std::out_of_range("Iterator cannot be decremented before begin");
        }
        if (current->left != nil) {
            current = _maximum(current->left);
        } else {
            node_ptr p = current->parent;
            while (p != nil && current == p->left) {
                current = p;
                p = p->parent;
            }
            current = p;
        }
        return *this;
    }

    /**
     * @brief Sobrecarga do operador de pós-decremento
     *
     * @return RBTreeIterator
     */
    RBTreeIterator operator--(int) {
        RBTreeIterator tmp = *this;
        --(*this);
        return tmp;
    }

    /**
     * @brief Sobrecarga do operador de igualdade
     *
     * @param other outro iterador
     * @return true se os iteradores são iguais, caso contrário, retorna false
     */
    bool operator==(const RBTreeIterator& other) const {
        return current == other.current;
    }

    /**
     * @brief Sobrecarga do operador de diferença
     *
     * @param other outro iterador
     * @return true se os iteradores são diferentes, caso contrário, retorna false
     */
    bool operator!=(const RBTreeIterator& other) const {
        return current != other.current;
    }

   private:
    /**
     * @brief Função que retorna o nó mínimo de uma subárvore
     *
     * @param x raiz da subárvore
     * @return RBNode* - ponteiro para o nó mínimo
     */
    node_ptr _minimum(node_ptr x) const {
        while (x->left != nil) {
            x = x->left;
        }
        return x;
    }

    /**
     * @brief Função que retorna o nó máximo de uma subárvore
     *
     * @param x raiz da subárvore
     * @return RBNode* - ponteiro para o nó máximo
     */
    node_ptr _maximum(node_ptr x) const {
        while (x->right != nil) {
            x = x->right;
        }
        return x;
    }
};
