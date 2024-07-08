#include "huffman.h"
#include <iostream>
#include <fstream>
#include <sstream>

//almacena los códigos de Huffman y devuelve el "codes"  que contiene los códigos 
std::unordered_map<char, std::string> HuffmanTree::getCodes() const {
    return codes;
}

//genera el arbol y genera los códigos de los caracteres desde la raíz del árbol Huffman.
HuffmanTree::HuffmanTree(const std::string& text) {
	
    buildTree(text);
    buildCodes(root, "");
}

//Codifica un texto utilizando los códigos almacenados.
std::string HuffmanTree::encode(const std::string& text) const {
    std::string encodedText;
    for (char c : text) {
        if (codes.find(c) != codes.end()) {
            encodedText += codes.at(c);
        } else {
            std::cerr << "Error: Character '" << c << "' not found in codes." << std::endl;
        }
    }
    return encodedText;
}

//Vuelve a decodificar en función de cada bit utilizando el árbol de Huffman
std::string HuffmanTree::decode(const std::string& encodedText) const {
    std::string decodedText;
    std::shared_ptr<Node> current = root;
    for (char bit : encodedText) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        if (!current->left && !current->right) {
            decodedText += current->symbol;
            current = root;
        }
    }
    return decodedText;
}

//Construye el árbol de Huffman en función de las frecuencias de los nodos.
void HuffmanTree::buildTree(const std::string& text) {
    std::unordered_map<char, int> frequencies;
    for (char c : text) {
        frequencies[c]++;
    }

    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNodes> pq;
    for (const auto& entry : frequencies) {
        pq.push(std::make_shared<Node>(Node{entry.first, entry.second, nullptr, nullptr}));
    }

    while (pq.size() > 1) {
        auto left = pq.top();
        pq.pop();
        auto right = pq.top();
        pq.pop();

        auto parent = std::make_shared<Node>(Node{'\0', left->frequency + right->frequency, left, right});
        pq.push(parent);
    }

    root = pq.top();
}

//Asigna códigos de Huffman recoriendo el arbol asignado 1 y 0 para marcar el recorido
void HuffmanTree::buildCodes(std::shared_ptr<Node> node, const std::string& code) {
    if (!node) {
        return;
    }

    if (!node->left && !node->right) {
        codes[node->symbol] = code;
    } else {
        buildCodes(node->left, code + "0");
        buildCodes(node->right, code + "1");
    }
}
