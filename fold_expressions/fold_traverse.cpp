#include <cstdint>
#include <iostream>

struct Node {
    int32_t value;
    Node * left;
    Node * right;
    Node(int32_t v = 0) : value(v), left(nullptr), right(nullptr) {}
};

auto left = &Node::left;
auto right = &Node::right;
// pointer to class member

template <typename T, typename ... TP>
Node * traverse(T np, TP ... paths) {
    return (np ->* ... ->* paths);
    // fold expression
    // When called with traverse(root, left, right)
    // It will be expanded to:
    // return ((root -> left) -> right)
}

int main() {
    // Just as an example.
    // Not a good coding style in practice.
    Node * root = new Node(1);
    root -> left = new Node(2);
    root -> right = new Node(3);
    root -> left -> left = new Node(4);
    root -> left -> right = new Node(5);
    root -> right -> left = new Node(6);
    root -> right -> right = new Node(7);
    root -> right -> left -> left = new Node(8);
    
    Node * node1 = traverse(root, left, left);
    Node * node2 = traverse(root, right, left, left);

    std::cout << "node1: " << node1->value << std::endl;
    std::cout << "node2: " << node2->value << std::endl;
    return 0;
}