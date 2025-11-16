// tree.cpp
// Implementación del BST simple.

#include "tree.h"

// insertar recursivo: si el nodo es null, crear nuevo; si no, comparar y bajar por izquierda/derecha
Node* BinarySearchTree::insert(Node* node, const std::string& value) {
    if (node == nullptr) {
        // crear un nuevo nodo si alcanzamos una hoja
        return new Node(value);
    }
    if (value < node->data) {
        // si value es menor, insertar en subárbol izquierdo
        node->left = insert(node->left, value);
    } else {
        // si value es mayor o igual, insertar en subárbol derecho
        node->right = insert(node->right, value);
    }
    return node; // devolver referencia al nodo actual
}

// envoltura pública que actualiza la raíz
void BinarySearchTree::insert(const std::string& value) {
    root = insert(root, value);
}

// recorrido inorder recursivo para imprimir en orden
void BinarySearchTree::inOrder(Node* node) {
    if (!node) return;            // caso base: nodo nulo
    inOrder(node->left);          // recorrer izquierda
    std::cout << node->data << std::endl; // procesar nodo
    inOrder(node->right);         // recorrer derecha
}

// mostrar raíz usando inorder
void BinarySearchTree::display() {
    inOrder(root);
}