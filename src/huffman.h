#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <memory>

// Nodo del árbol de Huffman
struct Node {
    char symbol;
    int frequency;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

// Comparador para el priority_queue
class CompareNodes {
public:
    std::unordered_map<char, std::string> getCodes() const;
    std::string encode(const std::string& text) const;
    std::string decode(const std::string& encodedText) const;
    bool operator()(const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) const {
        return lhs->frequency > rhs->frequency;
    } 
};

// Clase del árbol de Huffman
class HuffmanTree {
public:
    HuffmanTree(const std::string& text);

    std::unordered_map<char, std::string> getCodes() const;

    std::string encode(const std::string& text) const;

    std::string decode(const std::string& encodedText) const;

private:
    std::shared_ptr<Node> root;
    std::unordered_map<char, std::string> codes;

    void buildTree(const std::string& text);

    void buildCodes(std::shared_ptr<Node> node, const std::string& code);
};

#endif // HUFFMAN_H
