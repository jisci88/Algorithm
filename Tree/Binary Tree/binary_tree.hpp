#pragma once


#include <memory>

template<typename T>
class Node {
public:
    T data;
    std::unique_ptr<Node<T>> left;
    std::unique_ptr<Node<T>> right;
    Node(T d) : data{d}, left{nullptr}, right{nullptr} {}
};

template<typename T>
class BinaryTree {
public:
    std::unique_ptr<Node<T>> root;
    BinaryTree() : root{nullptr} {}

    void insert(T d);
};

template<typename T>
void BinaryTree<T>::insert(T d) {
        if (!root) {
            root = std::make_unique<Node<T>>(d);
            return;
        }

        Node<T>* curr{root.get()};
        while (curr) {
            if (d > curr->data) {
                if (!curr->right) {
                    curr->right = std::make_unique<Node<T>>(d);
                    break;
                }
                curr = curr->right.get();
            }
            else {
                if (!curr->left) {
                    curr->left = std::make_unique<Node<T>>(d);
                    break;
                }
                curr = curr->left.get();
            }
        }
    }
