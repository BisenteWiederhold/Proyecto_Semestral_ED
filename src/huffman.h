#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>

// Estructura para el nodo del árbol de Huffman
struct HuffmanNode {
    char data;
    int frequency;
    HuffmanNode *left, *right;

    HuffmanNode(char data, int frequency) {
        left = right = nullptr;
        this->data = data;
        this->frequency = frequency;
    }
};

// Comparador para la cola de prioridad
struct compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return (l->frequency > r->frequency);
    }
};

// Función para imprimir los códigos de Huffman
void printCodes(struct HuffmanNode* root, std::string str, std::unordered_map<char, std::string>& huffmanCode) {
    if (!root)
        return;

    if (root->data != '$')
        huffmanCode[root->data] = str;

    printCodes(root->left, str + "0", huffmanCode);
    printCodes(root->right, str + "1", huffmanCode);
}

// Construcción del árbol de Huffman y generación de códigos
std::unordered_map<char, std::string> buildHuffmanTree(const std::string& text) {
    std::unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }

    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, compare> minHeap;

    for (auto pair : freq) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    while (minHeap.size() != 1) {
        HuffmanNode *left = minHeap.top(); minHeap.pop();
        HuffmanNode *right = minHeap.top(); minHeap.pop();

        int sum = left->frequency + right->frequency;
        HuffmanNode *top = new HuffmanNode('$', sum);
        top->left = left;
        top->right = right;

        minHeap.push(top);
    }

    std::unordered_map<char, std::string> huffmanCode;
    printCodes(minHeap.top(), "", huffmanCode);

    return huffmanCode;
}

// Codificación de Huffman
std::string huffmanEncode(const std::string& text, std::unordered_map<char, std::string>& huffmanCode) {
    std::string encodedString = "";
    for (char ch : text) {
        encodedString += huffmanCode[ch];
    }
    return encodedString;
}

// Decodificación de Huffman
std::string huffmanDecode(const std::string& encodedString, std::unordered_map<char, std::string>& huffmanCode) {
    std::string decodedString = "";
    std::string currentCode = "";
    std::unordered_map<std::string, char> reverseHuffmanCode;
    for (auto pair : huffmanCode) {
        reverseHuffmanCode[pair.second] = pair.first;
    }

    for (char bit : encodedString) {
        currentCode += bit;
        if (reverseHuffmanCode.find(currentCode) != reverseHuffmanCode.end()) {
            decodedString += reverseHuffmanCode[currentCode];
            currentCode = "";
        }
    }

    return decodedString;
}

#endif // HUFFMAN_H
