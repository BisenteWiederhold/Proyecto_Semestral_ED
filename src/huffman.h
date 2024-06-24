#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <unordered_map>

struct Node {
    char symbol;
    int frequency;
    Node* left, *right;
};

Node* getNode(char symbol, int frequency, Node* left, Node* right);
void encode(Node* root, std::string str, std::unordered_map<char, std::string>& HuffmanCode);
void decode(Node* root, int& index, std::string str);
void createTree(std::string text);

#endif // HUFFMAN_H
