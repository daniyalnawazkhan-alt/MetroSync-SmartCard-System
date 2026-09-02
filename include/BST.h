#ifndef BST_H
#define BST_H

#include <iostream>
#include <string>

struct BSTNode {
    std::string stationName;
    int passengerCount;
    BSTNode* left;
    BSTNode* right;

    BSTNode(std::string name, int count)
        : stationName(name), passengerCount(count), left(nullptr), right(nullptr) {}
};

class StationBST {
private:
    BSTNode* root;

    void destroyTree(BSTNode* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    BSTNode* insert(BSTNode* node, std::string name, int count) {
        if (!node) return new BSTNode(name, count);
        if (name < node->stationName)
            node->left = insert(node->left, name, count);
        else if (name > node->stationName)
            node->right = insert(node->right, name, count);
        else
            node->passengerCount += count;
        return node;
    }

    void inOrder(BSTNode* node) const {
        if (!node) return;
        inOrder(node->left);
        std::cout << "Station: " << node->stationName << " | Passengers: " << node->passengerCount << "\n";
        inOrder(node->right);
    }

public:
    StationBST() : root(nullptr) {}
    ~StationBST() { destroyTree(root); }

    void insertStation(std::string name, int count = 1) {
        root = insert(root, name, count);
    }

    void displaySorted() const {
        inOrder(root);
    }
};

#endif