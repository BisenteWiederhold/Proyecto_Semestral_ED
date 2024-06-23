#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <sys/resource.h>
#include "huffman.h"
#include "lz.cpp"

// Función para obtener el tamaño de la memoria utilizada en bytes
size_t getMemoryUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}

void runExperiments(const std::string& text) {
    using namespace std::chrono;

    // Medir el tiempo y el tamaño de Huffman Encoding
    auto start = high_resolution_clock::now();
    HuffmanTree huffmanTree(text);
    std::string huffmanEncoded = huffmanTree.encode(text);
    std::string huffmanDecoded = huffmanTree.decode(huffmanEncoded);
    auto end = high_resolution_clock::now();
    size_t huffmanMemoryUsage = getMemoryUsage();
    auto huffmanDuration = duration_cast<microseconds>(end - start).count();

    std::cout << "Huffman Encoded: " << huffmanEncoded << std::endl;
    std::cout << "Huffman Decoded: " << huffmanDecoded << std::endl;

    // Medir el tiempo y el tamaño de LZ Compression
    start = high_resolution_clock::now();
    std::vector<std::pair<int, int>> lzCompressed = lzCompress(text);
    std::string lzDecompressed = lzDecompress(lzCompressed);
    end = high_resolution_clock::now();
    size_t lzMemoryUsage = getMemoryUsage();
    auto lzDuration = duration_cast<microseconds>(end - start).count();

    std::cout << "LZ Compressed: ";
    for (const auto& pair : lzCompressed) {
        if (pair.second == 0) {
            std::cout << "(" << static_cast<char>(pair.first) << ", " << pair.second << ") ";
        } else {
            std::cout << "(" << pair.first << ", " << pair.second << ") ";
        }
    }
    std::cout << std::endl;
    std::cout << "LZ Decompressed: " << lzDecompressed << std::endl;

    // Guardar resultados en el archivo "experimentacion.txt"
    std::ofstream outputFile("experimentacion.txt");
    if (!outputFile) {
        std::cerr << "Error: Could not open output file." << std::endl;
        return;
    }

    outputFile << "Huffman Encoding successful: " << (text == huffmanDecoded ? "Yes" : "No") << std::endl;
    outputFile << "Huffman Encoding time: " << huffmanDuration << " microseconds" << std::endl;
    outputFile << "Huffman Memory usage: " << huffmanMemoryUsage << " bytes" << std::endl;
    outputFile << "LZ Compression successful: " << (text == lzDecompressed ? "Yes" : "No") << std::endl;
    outputFile << "LZ Compression time: " << lzDuration << " microseconds" << std::endl;
    outputFile << "LZ Memory usage: " << lzMemoryUsage << " bytes" << std::endl;

    outputFile.close();
}

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

    // Ejecutar experimentos
    runExperiments(text);

    return 0;
}
