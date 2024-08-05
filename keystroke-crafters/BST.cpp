#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

struct bstNode {
    string data;
    bstNode* left;
    bstNode* right;
    bstNode(string a) {
        data = a;
        left = nullptr;
        right = nullptr;
    }
};

class BST {
private:
    bstNode* root;
    int size;

    // Recusively inserts a node into the BST
    bstNode* insert(bstNode* node, string a) {
        if (node == nullptr) {
            return new bstNode(a);
        }
        if (a < node->data) {
            node->left = insert(node->left, a);
        } else {
            node->right = insert(node->right, a);
        }
        return node;
    }

    // Traverses the words in an in-order traversal
    void inOrder(bstNode* node, vector<string>& words) {
        if (node != nullptr) {
            inOrder(node->left, words);
            words.push_back(node->data);
            inOrder(node->right, words);
        }
    }

    // Finds the minimum value node in the tree
    bstNode* findMin(bstNode* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    // Recursively removes a node from the BST
    bstNode* remove(bstNode* node, string a) {
        if (node == nullptr) return node;
        if (a < node->data) {
            node->left = remove(node->left, a);
        } else if (a > node->data) {
            node->right = remove(node->right, a);
        } else {
            if (node->left == nullptr) {
                bstNode* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                bstNode* temp = node->left;
                delete node;
                return temp;
            }
            bstNode* temp = findMin(node->right);
            node->data = temp->data;
            node->right = remove(node->right, temp->data);
        }
        return node;
    }

public:
    BST() {
        root = nullptr;
        size = 0;
    }

    // Adds a word to the BST
    void addElement(string a) {
        root = insert(root, a);
        size++;
    }

    // Initializes the BST with the first 10 words from the data set
    void initializeBST() {
        ifstream file("google-books-common-words.txt");
        if (!file) {
            cout << "Failed to open file" << endl;
            return;
        }
        string word;
        getline(file, word);
        for (int i = 0; i < 10; i++) {
            word = word.substr(0, word.find(' '));
            addElement(word);
            getline(file, word);
        }
    }

    // Retrieves a random element from the BST and removes it
    string getElement() {
        if (size == 0) return "";

        vector<string> words;
        inOrder(root, words);

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, words.size() - 1);
        int index = dis(gen);

        string a = words[index];
        root = remove(root, a);
        size--;

        return a;
    }
};
