/**
 * @file RBTree.hpp
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Classe que representa uma árvore rubro-negra
 * @version 0.1
 * @date 17-09-2024
 *
 *
 */

#pragma once

#include <iostream>

#include "RBNode.hpp"
#include "RBTreeExceptions.hpp"
#include "RBTreeIterator.hpp"

/**
 * @brief Classe que representa uma árvore rubro-negra
 *
 * @tparam Key tipo da chave
 * @tparam Value tipo do valor
 * @tparam Less função de comparação (default = std::less<Key>)
 */
template <typename Key, typename Value, typename Less = std::less<Key>>
class RBTree {
    // Para facilitar a leitura do código, definimos os ponteiros para os nós
    using NodePtr = RBNode<Key, Value>*;

    NodePtr _root;  // Raiz da árvore
    NodePtr _NIL;   // Nó nulo

    Less _less;  // Função de comparação

    std::size_t m_size{};          // Tamanho da árvore
    std::size_t _n_comparisons{};  // Número de comparações
    std::size_t _n_rotations{};    // Número de rotações

    //* FUNÇÕES AUXILIARES *//

    /**
     * @brief Função que retorna o tamanho de uma subárvore
     *
     * @param node raiz da subárvore
     * @return std::size_t - tamanho da subárvore
     */
    std::size_t _size(NodePtr node) {
        if (node == _NIL) return 0;
        return 1 + _size(node->left) + _size(node->right);
    }

    /**
     * @brief Função auxiliar que delete uma subárvore
     *
     * @param node raiz da subárvore
     */
    void _delete_subtree(NodePtr node) {
        if (node == _NIL) return;

        _delete_subtree(node->left);
        _delete_subtree(node->right);

        delete node;
    }

    /**
     * @brief Função que retorna o nó mínimo de uma subárvore
     *
     * @param x raiz da subárvore
     * @return RBNode* - ponteiro para o nó mínimo
     */
    NodePtr _minimum(NodePtr x) {
        while (x->left != _NIL) {
            x = x->left;
            ++_n_comparisons;
        }
        return x;
    }

    /**
     * @brief Função que retorna o nó máximo de uma subárvore
     *
     * @param x raiz da subárvore
     * @return RBNode* - ponteiro para o nó máximo
     */
    NodePtr _maximum(NodePtr x) {
        while (x->right != _NIL) {
            x = x->right;
            ++_n_comparisons;
        }
        return x;
    }

    /**
     * @brief Função que procura um nó na árvore
     *
     * @param key chave do nó
     * @return RBNode* - ponteiro para o nó
     */
    NodePtr _search(const Key& key) {
        NodePtr p = _root;
        while (p != _NIL && p->key != key) {
            if (_less(key, p->key))
                p = p->left;
            else
                p = p->right;
            _n_comparisons += 3;  // 2 comparisons in the while and 1 in the if
        }
        return p;
    }

    //* ROTAÇÕES *//

    void _leftRotate(NodePtr x) {
        NodePtr y = x->right;
        x->right = y->left;

        if (y->left != _NIL) {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == _NIL) {
            _root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    void _rightRotate(NodePtr x) {
        NodePtr y = x->left;
        x->left = y->right;

        if (y->right != _NIL) {
            y->right->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == _NIL) {
            _root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }

        y->right = x;
        x->parent = y;
    }

    /**
     * @brief Função que faz o balanceamento da árvore após a inserção de um nó.
     *
     * Esta função ajusta a árvore para manter as propriedades de balanceamento de uma árvore Red-Black
     * após a inserção de um novo nó.
     *
     * @param z O nó recém-inserido que pode precisar de ajustes para manter o balanceamento.
     */
    void _insert_fixup(NodePtr z) {
        // Enquanto o nó z tiver um pai vermelho, há a possibilidade de violar as propriedades da árvore Red-Black.
        while (z->parent->color == RED) {
            // Caso o pai de z seja o filho esquerdo do avô
            if (z->parent == z->parent->parent->left) {
                ++_n_comparisons;
                NodePtr y = z->parent->parent->right;  // Tio do nó z

                // Caso o tio de z seja vermelho
                if (y->color == RED) {
                    // Caso 1: O pai e o tio de z são vermelhos
                    z->parent->color = BLACK;        // O pai de z se torna preto
                    y->color = BLACK;                // O tio de z se torna preto
                    z->parent->parent->color = RED;  // O avô de z se torna vermelho
                    z = z->parent->parent;           // Move o nó z para o avô
                } else {
                    // Caso o tio de z seja preto
                    if (z == z->parent->right) {
                        ++_n_comparisons;
                        z = z->parent;   // Move o nó z para o pai
                        _leftRotate(z);  // Realiza uma rotação à esquerda para manter o balanceamento
                        ++_n_rotations;
                    }

                    // Ajusta as cores e realiza uma rotação à direita
                    z->parent->color = BLACK;         // O pai de z se torna preto
                    z->parent->parent->color = RED;   // O avô de z se torna vermelho
                    _rightRotate(z->parent->parent);  // Realiza uma rotação à direita no avô de z
                    ++_n_rotations;
                }
            } else {
                // Caso o pai de z seja o filho direito do avô
                NodePtr y = z->parent->parent->left;  // Tio do nó z
                if (y->color == RED) {
                    // Caso 1: O pai e o tio de z são vermelhos
                    z->parent->color = BLACK;        // O pai de z se torna preto
                    y->color = BLACK;                // O tio de z se torna preto
                    z->parent->parent->color = RED;  // O avô de z se torna vermelho
                    z = z->parent->parent;           // Move o nó z para o avô
                } else {
                    // Caso o tio de z seja preto
                    if (z == z->parent->left) {
                        ++_n_comparisons;
                        z = z->parent;    // Move o nó z para o pai
                        _rightRotate(z);  // Realiza uma rotação à direita para manter o balanceamento
                        ++_n_rotations;
                    }

                    // Ajusta as cores e realiza uma rotação à esquerda
                    z->parent->color = BLACK;        // O pai de z se torna preto
                    z->parent->parent->color = RED;  // O avô de z se torna vermelho
                    _leftRotate(z->parent->parent);  // Realiza uma rotação à esquerda no avô de z
                    ++_n_rotations;
                }
            }
        }

        // Garante que a raiz da árvore seja preta
        _root->color = BLACK;
    }

    /**
     * @brief Ajusta a árvore após a remoção de um nó para manter o balanceamento.
     *
     * Esta função corrige possíveis violações das propriedades de balanceamento de uma árvore Red-Black
     * após a remoção de um nó. Ela realiza rotações e mudanças de cores conforme necessário para
     * garantir que a árvore permaneça balanceada e as propriedades da árvore Red-Black sejam mantidas.
     *
     * @param x O nó que está causando uma possível violação de balanceamento após a remoção.
     */
    void _remove_fixup(NodePtr x) {
        // Enquanto x não for a raiz e x for preto, o balanceamento da árvore pode estar comprometido.
        while (x != _root && x->color == BLACK) {
            ++_n_comparisons;
            if (x == x->parent->left) {
                ++_n_comparisons;
                NodePtr w = x->parent->right;  // Irmão do nó x

                // Caso o irmão de x seja vermelho
                if (w->color == RED) {
                    w->color = BLACK;        // O irmão se torna preto
                    x->parent->color = RED;  // O pai de x se torna vermelho
                    _leftRotate(x->parent);  // Realiza uma rotação à esquerda
                    w = x->parent->right;    // Atualiza o irmão após a rotação
                    ++_n_rotations;
                }

                // Caso ambos os filhos do irmão sejam pretos
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;  // O irmão se torna vermelho
                    x = x->parent;   // Move x para o pai
                } else {
                    // Caso o filho direito do irmão seja preto
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;  // O filho esquerdo do irmão se torna preto
                        w->color = RED;          // O irmão se torna vermelho
                        _rightRotate(w);         // Realiza uma rotação à direita
                        w = x->parent->right;    // Atualiza o irmão após a rotação
                        ++_n_rotations;
                    }

                    // Ajusta as cores e realiza uma rotação à esquerda
                    w->color = x->parent->color;  // O irmão assume a cor do pai de x
                    x->parent->color = BLACK;     // O pai de x se torna preto
                    w->right->color = BLACK;      // O filho direito do irmão se torna preto
                    _leftRotate(x->parent);       // Realiza uma rotação à esquerda no pai de x
                    ++_n_rotations;
                    x = _root;  // Move x para a raiz
                }
            } else {
                NodePtr w = x->parent->left;  // Irmão do nó x

                // Caso o irmão de x seja vermelho
                if (w->color == RED) {
                    w->color = BLACK;         // O irmão se torna preto
                    x->parent->color = RED;   // O pai de x se torna vermelho
                    _rightRotate(x->parent);  // Realiza uma rotação à direita
                    w = x->parent->left;      // Atualiza o irmão após a rotação
                    ++_n_rotations;
                }

                // Caso ambos os filhos do irmão sejam pretos
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;  // O irmão se torna vermelho
                    x = x->parent;   // Move x para o pai
                } else {
                    // Caso o filho esquerdo do irmão seja preto
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;  // O filho direito do irmão se torna preto
                        w->color = RED;           // O irmão se torna vermelho
                        _leftRotate(w);           // Realiza uma rotação à esquerda
                        w = x->parent->left;      // Atualiza o irmão após a rotação
                        ++_n_rotations;
                    }

                    // Ajusta as cores e realiza uma rotação à direita
                    w->color = x->parent->color;  // O irmão assume a cor do pai de x
                    x->parent->color = BLACK;     // O pai de x se torna preto
                    w->left->color = BLACK;       // O filho esquerdo do irmão se torna preto
                    _rightRotate(x->parent);      // Realiza uma rotação à direita no pai de x
                    ++_n_rotations;
                    x = _root;  // Move x para a raiz
                }
            }
        }

        // Garante que o nó x seja preto
        x->color = BLACK;
    }

   public:
    //* CONSTRUTORES E DESTRUTORES *//

    /**
     * @brief Constrói um novo objeto RBTree
     *
     */
    RBTree() {
        _root = _NIL = new RBNode<Key, Value>(Key(), Value(), nullptr, nullptr, nullptr, BLACK);
        _NIL->left = _NIL->right = _NIL->parent = _NIL;
    }

    /**
     * @brief Destrói o objeto RBTree
     *
     */
    ~RBTree() {
        _delete_subtree(_root);
        delete _NIL;
    }

    //* FUNÇÕES PÚBLICAS *//

    /**
     * @brief Limpa a árvore
     *
     */
    void clear() {
        _delete_subtree(_root);
        _root = _NIL;
    }

    /**
     * @brief Insere um nó na árvore
     *
     * Se a chave já existir na árvore, uma exceção é lançada.
     *
     * @param key chave
     * @param value valor
     */
    void insert(Key key, Value value) {
        NodePtr c = _root;
        NodePtr p = _NIL;

        while (c != _NIL) {
            p = c;
            if (key == c->key) {
                throw RB_AlreadyExistsException();
            } else if (_less(key, c->key)) {
                ++_n_comparisons;
                c = c->left;
            } else {
                c = c->right;
            }
            _n_comparisons += 2;  // 1 comparison in the while and 1 in the else
        }

        NodePtr n = new RBNode<Key, Value>(key, value, p, _NIL, _NIL, RED);

        if (p == _NIL) {
            _root = n;
        } else if (_less(key, p->key)) {
            ++_n_comparisons;
            p->left = n;
        } else {
            p->right = n;
        }
        ++_n_comparisons;

        _insert_fixup(n);
        m_size++;
    }

    /**
     * @brief Remove um nó da árvore
     *
     * Se a chave não existir na árvore, uma exceção é lançada.
     *
     * @param key chave
     */
    void remove(const Key& key) {
        NodePtr z = _root;
        while (z != _NIL && key != z->key) {
            if (_less(key, z->key)) {
                z = z->left;
            } else {
                z = z->right;
            }
            _n_comparisons += 3;  // 2 comparisons in the while and 1 in the if
        }

        if (z != _NIL) {
            _remove(z);
            m_size--;
        }
        ++_n_comparisons;
    }

    /**
     * @brief Retorna o valor associado a uma chave
     *
     * Se a chave não existir na árvore, uma exceção é lançada.
     *
     * @param key chave
     * @return Value& valor associado à chave
     */
    Value& search(const Key& key) {
        NodePtr node = _search(key);
        if (node == _NIL) {
            throw RB_ValueNotFoundException();
        }
        ++_n_comparisons;
        return node->value;
    }

    /**
     * @brief Verifica se a árvore contém uma chave
     *
     * @param key chave
     * @return true se a árvore contém a chave, caso contrário, retorna false
     */
    bool contains(const Key& key) {
        return _search(key) != _NIL;
    }

    /**
     * @brief Retorna a quantidade de elementos na árvore
     *
     * @return std::size_t - quantidade de elementos
     */
    std::size_t size() const {
        return m_size;
    }

    /**
     * @brief Retorna o número de comparações realizadas
     *
     * @return std::size_t - número de comparações
     */
    std::size_t comparisons() const {
        return _n_comparisons;
    }

    /**
     * @brief Retorna o número de rotações realizadas
     *
     * @return std::size_t - número de rotações
     */
    std::size_t rotations() const {
        return _n_rotations;
    }

    //* ITERATOR *//

    using iterator = RBTreeIterator<Key, Value>;

    /**
     * @brief Retorna um iterador para o início da árvore
     *
     * @return iterator
     */
    iterator begin() {
        return iterator(_root, _NIL);
    }

    /**
     * @brief Retorna um iterador para o final da árvore
     *
     * @return iterator
     */
    iterator end() {
        return iterator(_NIL, _NIL);
    }
};