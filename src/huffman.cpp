#include "huffman.h"


std::unordered_map<char, std::string> HuffmanTree::getCodes() const {
    return codes;
}

// Constructor de HuffmanTree
HuffmanTree::HuffmanTree(const std::string& text) {
    buildTree(text);
    buildCodes(root, "");
}

// Codifica un texto usando el árbol de Huffman
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

// Decodifica un texto codificado usando el árbol de Huffman
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

// Método privado para construir el árbol de Huffman
void HuffmanTree::buildTree(const std::string& text) {
    // Contar las frecuencias de los caracteres
    std::unordered_map<char, int> frequencies;
    for (char c : text) {
        frequencies[c]++;
    }

    // Crear una priority_queue para construir el árbol
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNodes> pq;
    for (const auto& entry : frequencies) {
        pq.push(std::make_shared<Node>(Node{entry.first, entry.second, nullptr, nullptr}));
    }

    // Construir el árbol de Huffman
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

// Método privado para construir los códigos Huffman
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
