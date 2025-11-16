// tree.h
// Cabecera del árbol binario (BST) simple para almacenar strings.
// Se usa para ejemplo: almacenar títulos y mostrarlos ordenados.

#ifndef TREE_H
#define TREE_H

#include <string>
#include <iostream>

struct Node {
    std::string data;  // dato almacenado (por ejemplo título)
    Node* left;        // hijo izquierdo
    Node* right;       // hijo derecho

    // constructor rápido que inicializa punteros a null
    Node(const std::string& value) : data(value), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    Node* root; // raíz del árbol

    // función recursiva privada para insertar y devolver el nodo (uso interno)
    Node* insert(Node* node, const std::string& value);

    // recorrido inorder recursivo
    void inOrder(Node* node);

public:
    // constructor: árbol vacío
    BinarySearchTree() : root(nullptr) {}

    // insertar valor público
    void insert(const std::string& value);

    // mostrar árbol en orden
    void display();
};

#endif // TREE_H