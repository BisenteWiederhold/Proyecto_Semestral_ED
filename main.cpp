#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "huffman.h"
#include "performance.h"
#include "lz.cpp"

int main() {
    // Leer el archivo de entrada
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string text = buffer.str();

    // Crear el árbol de Huffman
    HuffmanTree huffmanTree(text);
    std::unordered_map<char, std::string> huffmanCodes = huffmanTree.getCodes();

    // Codificar el texto usando el árbol de Huffman
    std::string encodedText = huffmanTree.encode(text);

    // Comprimir el texto usando LZ
    std::vector<std::pair<std::string, int>> compressed = lzCompress(encodedText);

    // Mostrar las estadísticas de rendimiento
    PerformanceStats stats;
    stats.measureExecutionTime([&](){ huffmanTree.encode(text); }, "Huffman Encoding Time");
    stats.measureExecutionTime([&](){ huffmanTree.decode(encodedText); }, "Huffman Decoding Time");
    stats.measureExecutionTime([&](){ lzCompress(encodedText); }, "LZ Compression Time");
    stats.measureExecutionTime([&](){ lzDecompress(compressed); }, "LZ Decompression Time");
    stats.measureSizeInBytes(encodedText, "Huffman Encoded Size");
    stats.measureSizeInBytes(compressed, "LZ Compressed Size");

    stats.printStats();

    // Compilar pares de codificación y decodificación
    std::cout << "Encoded-Decoded Pairs:" << std::endl;
    for (size_t i = 0; i < text.size(); ++i) {
        std::cout << "(" << text[i] << ", " << encodedText[i] << ") ";
    }
    std::cout << std::endl;

    // Decodificar el texto usando el árbol de Huffman
    std::string decodedText = huffmanTree.decode(encodedText);

    // Mostrar los pares "Original-Decoded"
    std::cout << "Original-Decoded Pairs:" << std::endl;
    for (size_t i = 0; i < text.size(); ++i) {
        std::cout << "(" << text[i] << ", " << decodedText[i] << ") ";
    }
    std::cout << std::endl;

    return 0;
}
