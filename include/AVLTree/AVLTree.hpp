/**
 * @file AVLTree.hpp
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Classe que representa uma árvore AVL
 * @version 0.1
 * @date 02-09-2024
 *
 *
 */

#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "AVLNode.hpp"
#include "AVLTreeExceptions.hpp"
#include "AVLTreeIterator.hpp"

/**
 * @brief Classe que representa uma árvore AVL
 *
 * @tparam Key chave
 * @tparam Value valor
 * @tparam Less comparador de chaves (default = std::less<Key>)
 */
template <typename Key, typename Value, typename Less = std::less<Key>>
class AVLTree {
    using NodePtr = AVLNode<Key, Value>*;      // Ponteiro para um nó
    using NodePtrPtr = AVLNode<Key, Value>**;  // Ponteiro para um ponteiro de um nó

    std::size_t _size;  // Tamanho da árvore
    NodePtr _root;      // Raiz da árvore

    Less _less;  // Comparador de chaves

    mutable std::size_t _n_comparisons{};  // Número de comparações
    std::size_t _n_rotations{};            // Número de rotações

    //* FUNÇÕES AUXILIARES *//

    /**
     * @brief Função que retorna a altura de um nó
     *
     * @param node nó a ser analisado
     * @return int - altura do nó
     */
    int _height(NodePtr node) {
        return node ? node->height : 0;
    }

    /**
     * @brief Função que retorna o fator de balanceamento de um nó
     *
     * @param node nó a ser analisado
     * @return int - fator de balanceamento
     */
    int _balance(NodePtr node) {
        return _height(node->right) - _height(node->left);
    }

    /**
     * @brief Função que retorna o nó mínimo de uma subárvore
     *
     * @param node raiz da subárvore
     * @return AVLNode* - nó mínimo
     */
    NodePtr minimum(NodePtr node) {
        NodePtr current = node;
        while (current->left) {
            current = current->left;
        }
        return current;
    }

    /**
     * @brief Função que retorna o nó máximo de uma subárvore
     *
     * @param node raiz da subárvore
     * @return AVLNode* - nó máximo
     */
    NodePtr maximum(NodePtr node) {
        NodePtr current = node;
        while (current->right) {
            current = current->right;
        }
        return current;
    }

    /**
     * @brief Função que exibe a árvore
     *
     * @param cur nó atual
     * @param depth profundidade do nó
     * @param state estado do nó
     */
    void _display(NodePtr cur, int depth = 0, int state = 0) const {
        if (cur->right)
            _display(cur->right, depth + 1, 2);

        for (int i = 0; i < depth; i++)
            printf("     ");

        if (state == 1)  // left
            printf("└───");
        else if (state == 2)  // right
            printf("┌───");

        std::cout << "[" << cur->key << ", " << cur->value << "]" << std::endl;

        if (cur->left)
            _display(cur->left, depth + 1, 1);
    }

    /**
     * @brief Função auxiliar que limpa a árvore
     *
     * @param cur nó atual
     */
    void _clearHelper(NodePtr cur) {
        if (cur) {
            _clearHelper(cur->left);
            _clearHelper(cur->right);
            delete cur;
        }
    }

    //* ROTAÇÔES *//

    /**
     * @brief Função que realiza uma rotação à esquerda
     *
     * @param p nó pivô a ser rotacionado
     * @return AVLNode* - nova raiz da subárvore
     */
    NodePtr _rotateLeft(NodePtr p) {
        ++_n_rotations;
        NodePtr q = p->right;
        p->right = q->left;
        q->left = p;
        p->height = 1 + std::max(_height(p->left), _height(p->right));
        q->height = 1 + std::max(_height(q->left), _height(q->right));
        return q;
    }

    /**
     * @brief Função que realiza uma rotação à direita
     *
     * @param p nó pivô a ser rotacionado
     * @return AVLNode* - nova raiz da subárvore
     */
    NodePtr _rotateRight(NodePtr p) {
        ++_n_rotations;
        NodePtr q = p->left;
        p->left = q->right;
        q->right = p;
        p->height = 1 + std::max(_height(p->left), _height(p->right));
        q->height = 1 + std::max(_height(q->left), _height(q->right));
        return q;
    }

    //* INSERÇÃO *//

    /**
     * @brief Função que insere um nó na árvore
     *
     * @param node raiz da subárvore
     * @param key chave do nó
     * @param value valor do nó
     * @return AVLNode* - nova raiz da subárvore
     */
    NodePtr _insert(NodePtr node, Key& key, Value& value) {
        if (!node) {
            ++_n_comparisons;
            ++_size;
            return new AVLNode(key, value);
        }
        if (key == node->key) {
            ++_n_comparisons;
            return node;
        }
        if (_less(key, node->key)) {
            ++_n_comparisons;
            node->left = _insert(node->left, key, value);
        } else {
            ++_n_comparisons;
            node->right = _insert(node->right, key, value);
        }
        node = _fixup_node(node, key);
        return node;
    }

    /**
     * @brief Função que corrige o balanceamento da árvore após a inserção de um nó.
     *
     * Esta função ajusta o balanceamento da árvore AVL após a inserção de um novo nó. Ela atualiza a altura dos
     * nós e realiza rotações para garantir que a árvore permaneça balanceada.
     *
     * @param node Raiz da subárvore que precisa de correção de balanceamento.
     * @param key Chave do nó inserido.
     * @return NodePtr - Nova raiz da subárvore após a correção de balanceamento.
     */
    NodePtr _fixup_node(NodePtr node, Key& key) {
        // Atualiza a altura do nó atual
        node->height = 1 + std::max(_height(node->left), _height(node->right));

        // Calcula o fator de balanceamento do nó atual
        int bal = _balance(node);

        // Se o nó estiver desbalanceado para a esquerda e a chave é menor que a chave do filho esquerdo
        if (bal < -1 && _less(key, node->left->key)) {
            return _rotateRight(node);  // Realiza rotação à direita
        }

        // Se o nó estiver desbalanceado para a esquerda e a chave é maior que a chave do filho esquerdo
        if (bal < -1 && _less(node->left->key, key)) {
            node->left = _rotateLeft(node->left);  // Realiza rotação à esquerda na subárvore esquerda
            return _rotateRight(node);             // Realiza rotação à direita
        }

        // Se o nó estiver desbalanceado para a direita e a chave é maior que a chave do filho direito
        if (bal > 1 && _less(node->right->key, key)) {
            return _rotateLeft(node);  // Realiza rotação à esquerda
        }

        // Se o nó estiver desbalanceado para a direita e a chave é menor que a chave do filho direito
        if (bal > 1 && _less(key, node->right->key)) {
            node->right = _rotateRight(node->right);  // Realiza rotação à direita na subárvore direita
            return _rotateLeft(node);                 // Realiza rotação à esquerda
        }

        // Se o nó estiver balanceado, retorna-o sem alterações
        return node;
    }

    //* REMOÇÃO *//

    /**
     * @brief Função que remove um nó da árvore.
     *
     * Esta função remove um nó da árvore AVL com base na chave fornecida. Após a remoção, ela ajusta a árvore
     * para garantir que as propriedades da árvore AVL sejam mantidas.
     *
     * @param node Raiz da subárvore onde a remoção será realizada.
     * @param key Chave do nó a ser removido.
     * @return NodePtr - Nova raiz da subárvore após a remoção e ajuste.
     */
    NodePtr _remove(NodePtr node, Key& key) {
        if (!node) {
            ++_n_comparisons;  // Conta uma comparação quando o nó é nulo
            return nullptr;    // Se o nó não existir, retorna nulo
        }

        // Se a chave a ser removida é menor que a chave do nó atual, vai para a subárvore esquerda
        if (_less(key, node->key)) {
            ++_n_comparisons;
            node->left = _remove(node->left, key);
        }
        // Se a chave a ser removida é maior que a chave do nó atual, vai para a subárvore direita
        else if (_less(node->key, key)) {
            ++_n_comparisons;
            node->right = _remove(node->right, key);
        }
        // Se a chave a ser removida é igual à chave do nó atual
        else {
            ++_n_comparisons;

            // Caso o nó a ser removido não tenha filho direito
            if (node->right == nullptr) {
                NodePtr child = node->left;  // O filho esquerdo substitui o nó removido
                delete node;                 // Remove o nó atual
                --_size;                     // Atualiza o tamanho da árvore
                return child;                // Retorna o novo nó raiz da subárvore
            }
            // Caso o nó a ser removido tenha um filho direito
            else {
                // Remove o sucessor do nó (o menor nó da subárvore direita)
                node->right = _remove_successor(node, node->right);
                --_size;  // Atualiza o tamanho da árvore
            }
        }

        // Ajusta a árvore para manter as propriedades da AVL após a remoção
        node = _fixup_deletion(node);
        return node;  // Retorna a nova raiz da subárvore
    }

    /**
     * @brief Função que remove o sucessor de um nó
     *
     * @param node nó a ser removido
     * @param successor sucessor do nó
     * @return AVLNode* - nova raiz da subárvore
     */
    NodePtr _remove_successor(NodePtr node, NodePtr successor) {
        if (successor->left != nullptr) {
            successor->left = _remove_successor(node, successor->left);
        } else {
            node->key = successor->key;
            node->value = successor->value;
            NodePtr temp = successor->right;
            delete successor;
            return temp;
        }
        successor = _fixup_deletion(successor);
        return successor;
    }

    /**
     * @brief Função que corrige o balanceamento da árvore AVL após a remoção de um nó.
     *
     * Esta função ajusta o balanceamento da árvore AVL após a remoção de um nó. Ela atualiza a altura dos nós
     * e realiza rotações para garantir que a árvore permaneça balanceada.
     *
     * @param node Raiz da subárvore que precisa de correção de balanceamento.
     * @return NodePtr - Nova raiz da subárvore após a correção de balanceamento.
     */
    NodePtr _fixup_deletion(NodePtr node) {
        if (node == nullptr) return nullptr;  // Se o nó for nulo, não há nada para corrigir

        // Atualiza a altura do nó atual
        node->height = 1 + std::max(_height(node->left), _height(node->right));

        // Calcula o fator de balanceamento do nó
        int bal = _balance(node);

        // Se o nó estiver desbalanceado para a esquerda
        if (bal < -1) {
            // Caso específico: Subárvore esquerda está desbalanceada e a subárvore esquerda da subárvore esquerda
            // tem altura menor ou igual à da subárvore direita
            if (_balance(node->left) <= 0) {
                return _rotateRight(node);  // Realiza rotação à direita
            }
            // Caso específico: Subárvore esquerda está desbalanceada e a subárvore esquerda da subárvore esquerda
            // tem altura maior que a da subárvore direita
            node->left = _rotateLeft(node->left);  // Realiza rotação à esquerda na subárvore esquerda
            return _rotateRight(node);             // Realiza rotação à direita
        }

        // Se o nó estiver desbalanceado para a direita
        if (bal > 1) {
            // Caso específico: Subárvore direita está desbalanceada e a subárvore direita da subárvore direita
            // tem altura maior ou igual à da subárvore esquerda
            if (_balance(node->right) >= 0) {
                return _rotateLeft(node);  // Realiza rotação à esquerda
            }
            // Caso específico: Subárvore direita está desbalanceada e a subárvore direita da subárvore direita
            // tem altura menor que a da subárvore esquerda
            node->right = _rotateRight(node->right);  // Realiza rotação à direita na subárvore direita
            return _rotateLeft(node);                 // Realiza rotação à esquerda
        }

        // Se o nó estiver balanceado, retorna-o sem mudanças
        return node;
    }

    //* BUSCA *//

    /**
     * @brief Função que busca um nó na árvore
     *
     * @param k chave do nó
     * @return AVLNode* - nó encontrado
     */
    NodePtr _findNode(Key k) const {
        NodePtr cur = _root;
        while (cur && cur->key != k) {
            if (_less(cur->key, k))
                cur = cur->right;
            else
                cur = cur->left;
            _n_comparisons += 3;
        }
        return cur;
    }

   public:
    //* CONSTRUTORES E DESTRUTORES *//

    /**
     * @brief Construtor da classe AVLTree
     *
     */
    AVLTree() : _size(0), _root(nullptr), _n_comparisons(0) {}

    /**
     * @brief Destrutor da classe AVLTree
     *
     */
    ~AVLTree() { clear(); }

    //* FUNÇÕES PÚBLICAS *//

    /**
     * @brief Função que insere um nó na árvore
     *
     * @param k chave do nó
     * @param v valor do nó
     */
    void insert(Key k, Value v) {
        _root = _insert(_root, k, v);
    }

    /**
     * @brief Função que remove um nó da árvore
     *
     * @param k chave do nó
     */
    void erase(Key k) {
        _root = _remove(_root, k);
    }

    /**
     * @brief Função que limpa a árvore
     *
     */
    void clear() {
        _clearHelper(_root);
        _root = nullptr;
        _size = 0;
    }

    /**
     * @brief Função que verifica se a árvore está vazia
     *
     * @return Retorna true se a árvore estiver vazia, caso contrário, retorna false
     */
    bool empty() const {
        return _size == 0;
    }

    /**
     * @brief Função que retorna o tamanho da árvore
     *
     * @return std::size_t - tamanho da árvore
     */
    std::size_t size() const {
        return _size;
    }

    /**
     * @brief Função que retorna o número de rotações
     *
     * @return std::size_t - número de rotações
     */
    std::size_t rotations() const {
        return _n_rotations;
    }

    /**
     * @brief Função que retorna o número de comparações
     *
     * @return std::size_t - número de comparações
     */
    std::size_t comparisons() const {
        return _n_comparisons;
    }

    /**
     * @brief Função que verifica se a árvore contém uma chave
     *
     * @param k chave
     * @return Retorna true se a árvore contém a chave, caso contrário, retorna false
     */
    bool contains(Key k) const {
        return _findNode(k) != nullptr;
    }

    /**
     * @brief Função que retorna o valor associado a uma chave
     *
     * @param k chave
     * @return Value - valor associado à chave
     */
    const Value& at(Key k) const {
        NodePtr cur = _findNode(k);
        if (!cur) {
            ++_n_comparisons;
            throw AVL_ValueNotFoundException();
        }
        return cur->value;
    }

    /**
     * @brief Função que retorna o valor associado a uma chave
     *
     * @param k chave
     * @return Value - valor associado à chave
     */
    Value& at(Key k) {
        NodePtr cur = _findNode(k);
        if (!cur) {
            ++_n_comparisons;
            throw AVL_ValueNotFoundException();
        }
        return cur->value;
    }

    /**
     * @brief Sobrecarga do operador []
     *
     * @param k chave
     * @return Value& - valor associado à chave
     */
    const Value& operator[](Key k) const {
        return at(k);
    }

    /**
     * @brief Sobrecarga do operador []
     *
     * @param k chave
     * @return Value& - valor associado à chave
     */
    Value& operator[](Key k) {
        return at(k);
    }

    /**
     * @brief Função que exibe a árvore.
     * Caso a árvore esteja vazia, exibe "Empty tree"
     *
     */
    void display() const {
        if (_root)
            _display(_root);
        else
            std::cout << "Empty tree" << std::endl;
    }

    /**
     * @brief Função que retorna o número de comparações
     *
     * @return std::size_t - número de comparações
     */
    std::size_t getComparisons() const {
        return _n_comparisons;
    }

    /**
     * @brief Função que retorna o número de rotações
     *
     * @return std::size_t - número de rotações
     */
    std::size_t getRotations() const {
        return _n_rotations;
    }

    //* ITERADORES *//

    using iterator = AVLTreeIterator<Key, Value>;  // Iterador da árvore

    /**
     * @brief Função que retorna um iterador para o início da árvore
     *
     * @return iterator - iterador para o início da árvore
     */
    iterator begin() {
        return iterator(_root);
    }

    /**
     * @brief Função que retorna um iterador para o final da árvore
     *
     * @return iterator - iterador para o final da árvore
     */
    iterator end() {
        return iterator(nullptr);
    }

    /**
     * @brief Função que retorna um iterador para um nó específico
     *
     * @param k chave do nó
     * @return iterator - iterador para o nó
     */
    iterator find(Key k) {
        NodePtr cur = _findNode(k);
        if (!cur) {
            ++_n_comparisons;
            throw AVL_ValueNotFoundException();
        }
        return iterator(_root, cur);
    }
};
