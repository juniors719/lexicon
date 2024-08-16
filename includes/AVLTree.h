/**
 * @file AVLTree.h
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Implementação da árvore AVL
 * @version 0.1
 * @date 14-08-2024
 *
 *
 */

#pragma once

#include <stack>  // A pilha é usada para armazenar os nós visitados durante a travessia da árvore, na implementação do iterador

template <typename Key, typename Value>
class AVLTree {
   public:
    /**
     * @brief Estrutura de nó da árvore AVL
     *
     */
    struct Node {
        Key key{};      // Dado do nó
        Value value{};  // Valor do nó
        Node* left{};   // Filho esquerdo
        Node* right{};  // Filho direito
        int height{};   // Altura do nó

        /**
         * @brief Construtor da classe Node
         *
         * @param key Chave identificadora
         * @param value Valor do nó
         */
        Node(const Key& chave, const Value& valor) : key(chave), value(valor) {}

        /**
         * @brief Destrutor da classe Node
         *
         */
        ~Node() {
            delete left;
            delete right;
        };
    };

   private:
    Node* root{};  // Raiz da árvore

    /**
     * @brief Função mebro que calcula a altura de um nó
     *
     * @param node Nó da árvore a ser calculado
     * @return Altura do nó
     */
    int _height(Node* node) {
        return node ? node->height : 0;
    }

    /**
     * @brief Função membro que calcula o fator de balanceamento de um nó
     *
     * @param node Nó da árvore a ser calculado
     * @return Fator de balanceamento do nó
     */
    int _balance(Node* node) {
        return _height(node->right) - _height(node->left);
    }

    /**
     * @brief Função membro que realiza a rotação à esquerda
     *
     * @param p Nó pivot
     * @return Nova raiz da subárvore
     */
    Node* _rotateLeft(Node* p) {
        Node* q = p->right;
        p->right = q->left;
        q->left = p;
        p->height = 1 + std::max(_height(p->left), _height(p->right));
        q->height = 1 + std::max(_height(q->left), _height(q->right));
        return q;
    }

    /**
     * @brief Função membro que realiza a rotação à direita
     *
     * @param p Nó pivot
     * @return Nova raiz da subárvore
     */
    Node* _rotateRight(Node* p) {
        Node* q = p->left;
        p->left = q->right;
        q->right = p;
        p->height = 1 + std::max(_height(p->left), _height(p->right));
        q->height = 1 + std::max(_height(q->left), _height(q->right));
        return q;
    }

    /**
     * @brief Função membro que adiciona um nó na árvore.
     *
     * @param p Nó raiz da subárvore
     * @param key Chave do nó a ser adicionado
     * @param value Valor do nó a ser adicionado
     * @return Nova raiz da subárvore
     */
    Node* _add(Node* p, const Key& key, const Value& value) {
        if (!p) return new Node(key, value);  // Se o nó não existe, cria um novo nó

        if (key == p->key) {
            return p;
        }

        if (key < p->key) {
            p->left = _add(p->left, key, value);  // Se a chave é menor, adiciona na subárvore esquerda
        } else {
            p->right = _add(p->right, key, value);  // Se a chave é maior, adiciona na subárvore direita
        }

        p = _fixup_node(p, key);  // Corrige o balanceamento do nó
        return p;
    }

    /**
     * @brief Função membro que corrige o balanceamento de um nó no caso de inserção
     *
     * @param p Nó a ser corrigido
     * @param key Chave do nó adicionado
     * @return Nó corrigido
     */
    Node* _fixup_node(Node* p, const Key& key) {
        p->height = 1 + std::max(_height(p->left), _height(p->right));  // Atualiza a altura do nó

        int b = _balance(p);  // Calcula o fator de balanceamento

        if (b > 1 && key > p->right->key) {
            return _rotateLeft(p);  // Rotação à esquerda
        }

        if (b < -1 && key < p->left->key) {
            return _rotateRight(p);  // Rotação à direita
        }

        if (b > 1 && key < p->right->key) {
            p->right = _rotateRight(p->right);
            return _rotateLeft(p);  // Rotação dupla à esquerda
        }

        if (b < -1 && key > p->left->key) {
            p->left = _rotateLeft(p->left);
            return _rotateRight(p);  // Rotação dupla à direita
        }

        return p;
    }

    /**
     * @brief Função membro que remove um nó da árvore
     *
     * @param node Nó raiz da subárvore
     * @param key Chave do nó a ser removido
     * @return Nova raiz da subárvore
     */
    Node* _remove(Node* node, const Key& key) {
        if (node == nullptr) return nullptr;  // node nao encontrado
        if (key < node->key)
            node->left = _remove(node->left, key);
        else if (key > node->key)
            node->right = _remove(node->right, key);
        else if (!node->right) {
            Node* child = node->left;
            delete node;
            return child;
        } else {
            node->right = _remove_sucessor(node, node->right);
        }
        node = _fixup_deletion(node);
        return node;
    }

    /**
     * @brief Função membro que remove o sucessor de um nó.
     *
     * @param root Nó raiz da subárvore
     * @param node Nó a ser removido
     * @return Nova raiz da subárvore
     */
    Node* _remove_sucessor(Node* root, Node* node) {
        if (node->left)
            node->left = _remove_sucessor(root, node->left);
        else {
            root->key = node->key;
            root->value = node->value;
            Node* child = node->right;
            delete node;
            return child;
        }
        node = _fixup_deletion(node);
        return node;
    }

    /**
     * @brief Função membro que corrige o balanceamento de um nó no caso de remoção
     *
     * @param node Nó a ser corrigido
     * @return Nó corrigido
     */
    Node* _fixup_deletion(Node* node) {
        node->height = 1 + std::max(_height(node->left), _height(node->right));
        int bal = _balance(node);
        if (bal > 1 && _balance(node->right) >= 0) return _rotateLeft(node);
        if (bal > 1 && _balance(node->right) < 0) {
            node->right = _rotateRight(node->right);
            return _rotateLeft(node);
        }
        if (bal < -1 && _balance(node->left) <= 0) return _rotateRight(node);
        if (bal < -1 && _balance(node->left) > 0) {
            node->left = _rotateLeft(node->left);
            return _rotateRight(node);
        }
        return node;
    }

    /*     void _bshow(Node* node, std::string heranca) {
            if (node != nullptr && (node->left != nullptr || node->right != nullptr))
                _bshow(node->right, heranca + "r");
            for (int i = 0; i < (int)heranca.size() - 1; i++)
                std::cout << (heranca[i] != heranca[i + 1] ? "│   " : "    ");
            if (heranca != "")
                std::cout << (heranca.back() == 'r' ? "┌───" : "└───");
            if (node == nullptr) {
                std::cout << "#" << std::endl;
                return;
            }
            std::cout << node->data << std::endl;
            if (node != nullptr && (node->left != nullptr || node->right != nullptr))
                _bshow(node->left, heranca + "l");
        } */

    /**
     * @brief Método privado que encontra um nó na árvore. Retorna nullptr se não encontrar.
     *
     * @param key Chave do nó a ser encontrado
     * @return Ponteiro do tipo Node para o nó encontrado
     */
    Node* _find(const Key& key) {
        Node* p = root;
        while (p && p->key != key) {
            if (key < p->key)
                p = p->left;
            else
                p = p->right;
        }
        return p;
    }

   public:
    AVLTree() = default;

    ~AVLTree() {
        delete root;
    }

    void clear() {
        delete root;
        root = nullptr;
    }

    void add(const Key& key, const Value& value) {
        root = _add(root, key, value);
    }

    void remove(const Key& data) {
        root = _remove(root, data);
    }

    int height() {
        return _height(root);
    }

    // void show() {
    //     _bshow(root, "");
    // }

    bool contains(const Key& key) {
        return _find(key) != nullptr;
    }

    //* Class Iterator  *//

   public:
    /**
     * @brief Classe Iterator para percorrer a árvore
     *
     */
    class Iterator {
        Node* current;            // Nó atual
        std::stack<Node*> nodes;  // Pilha de nós já visitados

       public:
        /**
         * @brief Construtor da classe Iterator
         *
         * @param root Nó raiz da árvore
         */
        Iterator(Node* root) : current(root) {
            while (current) {
                nodes.push(current);
                current = current->left;
            }
            if (!nodes.empty()) {
                current = nodes.top();
                nodes.pop();
            }
        }

        /**
         * @brief Sobrecarga do operador de desreferência
         *
         * @return Referência para o dado do nó atual
         */
        Value& operator*() {
            return current->value;
        }

        /**
         * @brief Sobrecarga do operador de incremento
         *
         * @return Referência para o próprio objeto
         */
        Iterator& operator++() {
            if (current->right) {
                current = current->right;
                while (current) {
                    nodes.push(current);
                    current = current->left;
                }
            }
            if (!nodes.empty()) {
                current = nodes.top();
                nodes.pop();
            } else {
                current = nullptr;
            }
            return *this;
        }

        /**
         * @brief Sobrecaarga do operador de comparação de desigualdade
         *
         * @param other Outro objeto da classe Iterator
         * @return true
         * @return false
         */
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

        /**
         * @brief Sobrecaarga do operador de comparação de igualdade
         *
         * @param other Outro objeto da classe Iterator
         * @return true
         * @return false
         */
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        /**
         * @brief Sobrecarga do operador de pós-incremento para a classe Iterator.
         *
         * @details Incrementa o iterador para o próximo elemento da árvore AVL.
         *
         * @return Uma referência para o iterador atualizado.
         */
        Iterator& operator++(int) {
            return ++(*this);
        }

        /**
         * @brief Sobrecarga do operador de pré-decremento para o iterador.
         *
         * Move o iterador para o elemento anterior na árvore AVL.
         *
         * @return Referência para o iterador atualizado.
         */
        Iterator& operator--() {
            if (current->left) {
                current = current->left;
                while (current) {
                    nodes.push(current);
                    current = current->right;
                }
            }
            if (!nodes.empty()) {
                current = nodes.top();
                nodes.pop();
            } else {
                current = nullptr;
            }
            return *this;
        }

        /**
         * @brief Sobrecarga do operador de pós-decremento para a classe Iterator.
         *
         * @details Move o iterador para o elemento anterior na árvore AVL.
         *
         * @return Uma referência para o iterador atualizado.
         */
        Iterator& operator--(int) {
            return --(*this);
        }

        /**
         * @brief Sobrecarga do operador de acesso "->".
         *
         * Retorna uma referência para o objeto apontado pelo ponteiro atual.
         *
         * @return Referência para o objeto apontado pelo ponteiro atual.
         */
        Value& operator->() {
            return current->value;
        }
    };

    /**
     * @brief Método que retorna um iterador para o início da árvore AVL.
     *
     * @return Iterador para o início da árvore AVL.
     */
    Iterator begin() {
        return Iterator(root);
    }

    /**
     * @brief Método que retorna um iterador para o final da árvore AVL.
     *
     * @return Iterador para o final da árvore AVL.
     */
    Iterator end() {
        return Iterator(nullptr);
    }

    /**
     * @brief Método que retorna um iterador para o nó com a chave especificada.
     *
     * @param key Chave a ser encontrada.
     * @return Iterador para o nó com a chave especificada.
     */
    Iterator find(const Key& key) {
        return Iterator(_find(key));
    }
};