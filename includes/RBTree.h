#pragma once

#include <stdexcept>

#define RED true
#define BLACK false

template <typename K, typename V>
class RBTree {
    //** Node **//

    /**
     * @brief Um nó de uma árvore rubro-negra.
     * Possui um par chave-valor, ponteiros para o pai, filho a esquerda e filho a direita,
     *
     * @tparam K Tipo da chave
     * @tparam V Tipo do valor
     *
     */
    struct Node {
        K key;         // Chave
        V value;       // Valor
        Node* parent;  // Ponteiro para o pai
        Node* left;    // Ponteiro para o filho a esquerda
        Node* right;   // Ponteiro para o filho a direita
        bool color;    // Cor do nó [RED = true, BLACK = false]

        /**
         * @brief Construtor do nó
         *
         * @param key Chave
         * @param value Valor
         */
        Node(K key, V value)
            : value(value),
              key(key),
              parent(nullptr),
              left(nullptr),
              right(nullptr),
              color(RED) {}
    };

    //** Attributes **//

    Node* root;  // Raiz da árvore
    Node* NIL;   // Nó nulo

    //** Private Methods **//

    /**
     * @brief Função auxiliar para deletar uma subárvore
     *
     * @param node Nó raiz da subárvore
     */
    void _delete_subtree(Node* node) {
        if (node == NIL) return;

        _delete_subtree(node->left);
        _delete_subtree(node->right);

        delete node;
    }

    /**
     * @brief Função auxiliar para inserir um nó na árvore
     *
     * @param key Chave do nó
     * @param value Valor do nó
     */
    void _rb_insert(const K& key, const V& value) {
        Node* z = new Node(key, value);
        Node* y = NIL;
        Node* x = root;

        while (x != NIL) {
            y = x;
            if (z->key < x->key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        z->parent = y;

        if (y == NIL) {
            root = z;
        } else if (z->key < y->key) {
            y->left = z;
        } else {
            y->right = z;
        }

        z->left = z->right = NIL;

        _rb_insert_fixup(z);
    }

    /**
     * @brief Função auxiliar para corrigir a árvore após a inserção de um nó
     *
     * @param z Nó inserido
     */
    void _rb_insert_fixup(Node* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;

                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        _leftRotate(z);
                    }

                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    _rightRotate(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;

                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        _rightRotate(z);
                    }

                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    _leftRotate(z->parent->parent);
                }
            }
        }

        root->color = BLACK;
    }

    /**
     * @brief Função auxiliar para remover um nó da árvore
     *
     * @param key Chave do nó a ser removido
     */
    void _remove(const K& key) {
        Node* p = root;
        while (p != NIL && p->key != key) {
            if (key < p->key)
                p = p->left;
            else
                p = p->right;
        }
        if (p != NIL) _rb_delete(p);
    }

    /**
     * @brief Função auxiliar para deletar um nó da árvore
     *
     * @param z Nó a ser deletado
     */
    void _rb_delete(Node* z) {
        Node* y = (z->left == NIL || z->right == NIL) ? z : _minimun(z->right);
        Node* x = (y->left == NIL) ? y->right : y->left;
        x->parent = y->parent;
        if (y->parent == NIL)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
        if (y != z) {
            z->key = y->key;
            z->value = y->value;
        }
        if (y->color == BLACK) _rb_delete_fixup(x);
        delete y;
    }

    /**
     * @brief Função auxiliar para encontrar o nó com a menor chave
     *
     * @param x Nó raiz da subárvore
     * @return Nó com a menor chave
     */
    Node* _minimun(Node* x) {
        while (x->left != NIL) x = x->left;
        return x;
    }

    /**
     * @brief Função auxiliar para corrigir a árvore após a remoção de um nó
     *
     * @param x Nó a ser corrigido
     */
    void _rb_delete_fixup(Node* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    _leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        _rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    _leftRotate(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    _rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        _leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    _rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    Node* _search(const K& key) {
        Node* p = root;
        while (p != NIL && p->key != key) {
            if (key < p->key)
                p = p->left;
            else
                p = p->right;
        }
        return p;
    }

    //** Rotations **//

    /**
     * @brief Função auxiliar para rotacionar um nó para a esquerda
     *
     * @param x Nó pivô que será rotacionado
     */
    void _leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;

        if (y->left != NIL) {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == NIL) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    /**
     * @brief Função auxiliar para rotacionar um nó para a direita
     *
     * @param x Nó pivô que será rotacionado
     */
    void _rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;

        if (y->right != NIL) {
            y->right->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == NIL) {
            root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }

        y->right = x;
        x->parent = y;
    }

    //** Auxiliar Methods **//

    /**
     * @brief Função auxiliar para mostrar a árvore
     *
     * @param node Nó raiz da subárvore
     * @param heranca String que representa a herança do nó
     */
    void _bshow(Node* node, std::string heranca) {
        if (node != NIL && (node->left != NIL || node->right != NIL))
            _bshow(node->right, heranca + "r");
        for (int i = 0; i < (int)heranca.size() - 1; i++)
            std::cout << (heranca[i] != heranca[i + 1] ? "│   " : "    ");
        if (heranca != "")
            std::cout << (heranca.back() == 'r' ? "┌───" : "└───");
        if (node == NIL) {
            std::cout << "#" << std::endl;
            return;
        }
        std::cout << (node->color == RED ? "\033[1;31m" : "") << "(" << node->key << ", " << node->value << ")" << (node->color == RED ? "\033[0m" : "") << std::endl;
        if (node != NIL && (node->left != NIL || node->right != NIL))
            _bshow(node->left, heranca + "l");
    }

    //** Sobrecarga de operadores **//

   public:
    /**
     * @brief Acessa o valor associado a uma chave na árvore.
     *
     * Retorna uma referência ao valor associado à chave especificada.
     * Lança uma exceção `std::out_of_range` se a chave não for encontrada na árvore.
     *
     * @param key A chave do elemento a ser acessado.
     * @return Referência ao valor associado à chave.
     * @throws std::out_of_range Se a chave não for encontrada.
     */
    V& operator[](const K& key) {
        Node* p = _search(key);
        if (p == NIL) throw std::out_of_range("Key not found");
        return p->value;
    }

    //** Iterator Classes **//

    /**
     * @brief Classe que representa um iterador da árvore rubro-negra
     *
     */
    class Iterator {
        Node* current;  // Ponteiro para o nó atual
        Node* NIL;      // Ponteiro para o nó nulo

       public:
        /**
         * @brief Construtor do iterador
         *
         * @param current Nó atual
         * @param NIL Nó nulo
         */
        Iterator(Node* current, Node* NIL) : current(current), NIL(NIL) {}

        /**
         * @brief Sobrecarga do operador de pré-incremento
         *
         * @return Referência para o iterador após o incremento
         */
        Iterator& operator++() {
            if (current->right != NIL) {
                current = current->right;
                while (current->left != NIL) {
                    current = current->left;
                }
            } else {
                Node* y = current->parent;
                while (y != NIL && current == y->right) {
                    current = y;
                    y = y->parent;
                }
                current = y;
            }

            return *this;
        }

        /**
         * @brief Sobrecarga do operador de pós-incremento
         *
         * @return Iterador antes do incremento
         */
        Iterator operator++(int) {
            Iterator it = *this;
            ++(*this);
            return it;
        }

        /**
         * @brief Decrementa o iterador para apontar para o elemento anterior na ordem de árvore.
         *
         * Move o iterador para o nó predecessor no percurso in-order da árvore.
         *
         * @pre O iterador não deve estar no início da árvore.
         *
         * @return Referência ao iterador após o decremento.
         * @throws std::out_of_range Se o iterador estiver no início da árvore.
         */
        Iterator& operator--() {
            if (current->left != NIL) {
                current = current->left;
                while (current->right != NIL) {
                    current = current->right;
                }
            } else {
                Node* y = current->parent;
                while (y != NIL && current == y->left) {
                    current = y;
                    y = y->parent;
                }
                current = y;
            }
            return *this;
        }

        /**
         * @brief Decrementa o iterador para apontar para o elemento anterior na ordem de árvore.
         *
         * Move o iterador para o nó predecessor no percurso in-order da árvore.
         *
         * @pre O iterador não deve estar no início da árvore.
         *
         * @return Iterador antes do decremento.
         * @throws std::out_of_range Se o iterador estiver no início da árvore.
         */
        Iterator operator--(int) {
            Iterator it = *this;
            --(*this);
            return it;
        }

        /**
         * @brief Compara dois iteradores para verificar se são diferentes.
         *
         * Dois iteradores são considerados diferentes se apontam para nós distintos na árvore.
         *
         * @param other Outro iterador a ser comparado.
         * @return `true` se os iteradores não apontam para o mesmo nó.
         *         `false` se os iteradores apontam para o mesmo nó ou ambos são inválidos.
         */
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

        /**
         * @brief Compara dois iteradores para verificar se são iguais.
         *
         * Dois iteradores são considerados iguais se apontam para o mesmo nó na árvore.
         *
         * @param other Outro iterador a ser comparado.
         * @return `true` se os iteradores apontam para o mesmo nó.
         *         `false` se os iteradores apontam para nós distintos ou ambos são inválidos.
         */
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        /**
         * @brief Retorna uma referência para o valor associado ao nó atual apontado pelo iterador.
         *
         * Permite acessar os membros do valor do nó atual através do iterador.
         *
         * @pre O iterador não pode estar inválido, ou seja, no final da árvore.
         *
         * @return Referência para o valor do nó atual.
         * @throws std::out_of_range Se o iterador estiver inválido.
         */
        V& operator*() {
            return current->value;
        }

        /**
         * @brief Retorna um ponteiro para o valor associado ao nó atual apontado pelo iterador.
         *
         * Permite acessar os membros do valor do nó atual através do iterador.
         *
         * @pre O iterador não pode estar inválido, ou seja, no final da árvore.
         *
         * @return Ponteiro para o valor do nó atual.
         * @throws std::out_of_range Se o iterador estiver inválido.
         */
        V* operator->() {
            return &(current->value);
        }
    };

    /**
     * @brief Classe que representa um iterador constante da árvore rubro-negra
     *
     */
    class ConstIterator : public Iterator {
       public:
        using Iterator::Iterator;  // Inherit constructors

        /**
         * @brief Retorna uma referência constante para o valor associado ao nó atual apontado pelo iterador.
         *
         * Permite acessar os membros do valor do nó atual através do iterador.
         *
         * @pre O iterador não pode estar inválido, ou seja, no final da árvore.
         *
         * @return Referência constante para o valor do nó atual.
         * @throws std::out_of_range Se o iterador estiver inválido.
         */
        const V& operator*() {
            return Iterator::operator*();
        }

        /**
         * @brief Retorna um ponteiro constante para o valor associado ao nó atual apontado pelo iterador.
         *
         * Permite acessar os membros do valor do nó atual através do iterador.
         *
         * @pre O iterador não pode estar inválido, ou seja, no final da árvore.
         *
         * @return Ponteiro constante para o valor do nó atual.
         * @throws std::out_of_range Se o iterador estiver inválido.
         */
        const V* operator->() {
            return Iterator::operator->();
        }
    };

    /**
     * @brief Classe que representa um iterador reverso da árvore rubro-negra
     *
     */
    class ReverseIterator : public Iterator {
       public:
        using Iterator::Iterator;

        /**
         * @brief Incrementa o iterador para apontar para o próximo elemento na ordem reversa.
         *
         * Move o iterador para o próximo elemento em direção ao início da sequência.
         * Equivalente a decrementar o iterador base.
         *
         * @return Referência ao iterador incrementado.
         */
        Iterator& operator++() {
            Iterator::operator--();
            return *this;
        }

        /**
         * @brief Incrementa o iterador para apontar para o próximo elemento na ordem reversa (pós-incremento).
         *
         * Retorna uma cópia do iterador antes do incremento.
         * Equivalente ao pós-decremento do iterador base.
         *
         * @return Cópia do iterador antes do incremento.
         */
        Iterator operator++(int) {
            Iterator it = *this;
            --(*this);
            return it;
        }

        /**
         * @brief Decrementa o iterador para apontar para o elemento anterior na ordem reversa.
         *
         * Move o iterador para o elemento anterior em direção ao final da sequência.
         * Equivalente a incrementar o iterador base.
         *
         * @return Referência ao iterador decrementado.
         */
        Iterator& operator--() {
            Iterator::operator++();
            return *this;
        }

        /**
         * @brief Decrementa o iterador para apontar para o elemento anterior na ordem reversa (pós-decremento).
         *
         * Retorna uma cópia do iterador antes do decremento.
         * Equivalente ao pós-incremento do iterador base.
         *
         * @return Cópia do iterador antes do decremento.
         */
        Iterator operator--(int) {
            Iterator it = *this;
            ++(*this);
            return it;
        };
    };

    /**
     * @brief Classe que representa um iterador reverso constante da árvore rubro-negra
     *
     */
    class ConstReverseIterator : public Iterator {
        using Iterator::Iterator;

        /**
         * @brief Retorna uma referência constante ao valor associado ao nó atual apontado pelo iterador reverso.
         *
         * Permite ler o valor do nó atual, mas não modificá-lo.
         *
         * @return Referência constante ao valor do nó atual.
         * @throws std::out_of_range Se o iterador estiver inválido (por exemplo, no final da sequência reversa).
         */
        const V& operator*() const {
            return ReverseIterator::operator*();
        }

        /**
         * @brief Retorna um ponteiro constante para o valor associado ao nó atual apontado pelo iterador reverso.
         *
         * Permite acessar os membros do valor do nó atual de forma semelhante a um ponteiro.
         *
         * @return Ponteiro constante para o valor do nó atual.
         * @throws std::out_of_range Se o iterador estiver inválido (por exemplo, no final da sequência reversa).
         */
        const V* operator->() const {
            return ReverseIterator::operator->();
        }
    };

    //** Iterator Methods **//

    /**
     * @brief Busca um elemento na árvore com a chave especificada.
     *
     * Retorna um iterador apontando para o elemento encontrado, caso exista.
     * Caso contrário, retorna um iterador igual ao retornado por `end()`.
     *
     * @param key A chave do elemento a ser buscado.
     * @return Um iterador para o elemento encontrado, ou `end()` se não encontrado.
     */
    Iterator find(const K& key) {
        return Iterator(_search(key), NIL);
    }

    /**
     * @brief Retorna um iterador que aponta para o primeiro elemento da árvore.
     *
     * O iterador retornado aponta para o menor elemento da árvore, ou seja, o nó mais à esquerda.
     *
     * @return Um iterador apontando para o primeiro elemento da árvore.
     */
    Iterator begin() {
        Node* current = root;
        while (current->left != NIL) {
            current = current->left;
        }

        return Iterator(current, NIL);
    }

    /**
     * @brief Retorna um iterador que aponta para o elemento após o último elemento da árvore.
     *
     * Este iterador é conhecido como "past-the-end" iterator e não deve ser desreferenciado.
     * É utilizado como limite superior em iteradores para percorrer a árvore.
     *
     * @return Um iterador que aponta para o elemento após o último elemento.
     */
    Iterator end() {
        return Iterator(NIL, NIL);
    }

    /**
     * @brief Retorna um iterador constante que aponta para o primeiro elemento da árvore.
     *
     * Retorna um iterador constante que referencia o menor elemento da árvore (in-order).
     *
     * @return Um iterador constante apontando para o primeiro elemento.
     */
    ConstIterator cbegin() const {
        Node* current = root;
        while (current->left != NIL) {
            current = current->left;
        }

        return ConstIterator(current, NIL);
    }

    /**
     * @brief Retorna um iterador constante que aponta para o elemento após o último elemento da árvore.
     *
     * Retorna um iterador constante que não referencia nenhum elemento válido. É utilizado como limite superior em iterações constantes.
     *
     * @return Um iterador constante que aponta para o elemento após o último.
     */
    ConstIterator cend() const {
        return ConstIterator(NIL, NIL);
    }

    /**
     * @brief Retorna um iterador reverso que aponta para o último elemento da árvore.
     *
     * Retorna um iterador reverso que referencia o maior elemento da árvore (in-order).
     *
     * @return Um iterador reverso apontando para o último elemento.
     */
    ReverseIterator rbegin() {
        Node* current = root;
        while (current->right != NIL) {
            current = current->right;
        }

        return ReverseIterator(current, NIL);
    }

    /**
     * @brief Retorna um iterador reverso que aponta para o elemento antes do primeiro elemento da árvore.
     *
     * Retorna um iterador reverso que não referencia nenhum elemento válido. É utilizado como limite superior em iterações reversas.
     *
     * @return Um iterador reverso que aponta para o elemento antes do primeiro.
     */
    ReverseIterator rend() {
        return ReverseIterator(NIL, NIL);
    }

    /**
     * @brief Retorna um iterador reverso constante que aponta para o elemento antes do primeiro elemento da árvore.
     *
     * Retorna um iterador reverso constante que não referencia nenhum elemento válido. É utilizado como limite superior em iterações reversas constantes.
     * Um iterador reverso constante é semelhante a um iterador reverso, mas não permite modificar os elementos da árvore.
     *
     * @return Um iterador reverso constante que aponta para o elemento antes do primeiro.
     */
    ConstReverseIterator crbegin() const {
        Node* current = root;
        while (current->right != NIL) {
            current = current->right;
        }

        return ConstReverseIterator(current, NIL);
    }

    /**
     * @brief Retorna um iterador reverso constante que aponta para o elemento após o último elemento da árvore.
     *
     * Retorna um iterador reverso constante que não referencia nenhum elemento válido. É utilizado como limite superior em iterações reversas constantes.
     * Um iterador reverso constante é semelhante a um iterador reverso, mas não permite modificar os elementos da árvore.
     *
     * @return Um iterador reverso constante que aponta para o elemento após o último.
     */
    ConstReverseIterator crend() const {
        return ConstReverseIterator(NIL, NIL);
    }

    //** Public Methods **//

   public:
    /**
     * @brief Construtor da árvore rubro-negra
     *
     */
    RBTree() {
        NIL = new Node(K(), V());
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL->parent = NIL;
        root = NIL;
    }

    /**
     * @brief Destrutor da árvore rubro-negra
     *
     */
    ~RBTree() {
        _delete_subtree(root);
        delete NIL;
    }

    /**
     * @brief Insere um novo elemento na árvore.
     *
     * Insere um par chave-valor na árvore, mantendo as propriedades da árvore rubro-negra.
     *
     * @param key A chave do novo elemento.
     * @param value O valor associado à chave.
     */
    void add(const K& key, const V& value) {
        _rb_insert(key, value);
    }

    /**
     * @brief Remove um elemento da árvore.
     *
     * Remove o elemento com a chave especificada da árvore, mantendo as propriedades da árvore rubro-negra.
     *
     * @param key A chave do elemento a ser removido.
     */
    void remove(const K& key) {
        _remove(key);
    }

    /**
     * @brief Exibe a árvore em formato de texto.
     *
     * Imprime uma representação textual da árvore, mostrando a estrutura e os valores dos nós.
     */
    void show() {
        _bshow(root, "");
    }
};