#include <iostream>

#include "../include/AVLTree/AVLTree.hpp"

int main() {
    AVLTree<std::string, int> tree;
    tree.insert("a", 1);
    tree.insert("b", 2);
    tree.insert("c", 3);

    for (auto [key, value] : tree) {
        std::cout << key << " " << value << std::endl;
    }
}