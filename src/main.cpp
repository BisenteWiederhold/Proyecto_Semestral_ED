#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <sys/resource.h>
#include "huffman.h"
#include "lz.cpp"

// FunciÃ³n para obtener el tamaÃ±o de la memoria utilizada en bytes
size_t getMemoryUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}

void runExperiments(const std::string& text) {
    using namespace std::chrono;

    // Medir el tiempo y el tamaÃ±o de Huffman Encoding
    auto start = high_resolution_clock::now();
    HuffmanTree huffmanTree(text);
    std::string huffmanEncoded = huffmanTree.encode(text);
    auto end = high_resolution_clock::now();
    auto huffmanDurationCo = duration_cast<microseconds>(end - start).count();
    
    auto start = high_resolution_clock::now();
    HuffmanTree huffmanTree(text);
    std::string huffmanDecoded = huffmanTree.decode(huffmanEncoded);
    auto end = high_resolution_clock::now();
    size_t huffmanMemoryUsage = getMemoryUsage();
    auto huffmanDurationDes = duration_cast<microseconds>(end - start).count();

    std::cout << "Huffman Encoded: " << huffmanEncoded << std::endl;
    std::cout << "Huffman Decoded: " << huffmanDecoded << std::endl;

    // Medir el tiempo y el tamaÃ±o de LZ Compression
    start = high_resolution_clock::now();
    std::vector<std::pair<int, int>> lzCompressed = lzCompress(text);
    end = high_resolution_clock::now();
    auto lzDurationCo = duration_cast<microseconds>(end - start).count();
    
    start = high_resolution_clock::now();
    std::string lzDecompressed = lzDecompress(lzCompressed);
    end = high_resolution_clock::now();
    size_t lzMemoryUsage = getMemoryUsage();
    auto lzDurationDes = duration_cast<microseconds>(end - start).count();

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
    std::ofstream outputFile("experimentacion.csv");
    if (!outputFile) {
        std::cerr << "Error: Could not open output file." << std::endl;
        return;
    }
    outputFile << "input 1: Huffman Encoding time: " << huffmanDurationCo << " microseconds" << std::endl;
    outputFile << "input 1: Huffman Decoding time: " << huffmanDurationDes << " microseconds" << std::endl;
    outputFile << "input 1: Huffman Memory usage: " << huffmanMemoryUsage << " bytes" << std::endl;
    outputFile << "input 1: LZ Compression time: " << lzDurationCo << " microseconds" << std::endl;
    outputFile << "input 1: LZ Decompression time: " << lzDurationDes << " microseconds" << std::endl;
    outputFile << "input 1: LZ Memory usage: " << lzMemoryUsage << " bytes" << std::endl;

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
    int i=0;
    // Ejecutar experimentos
    while(i<20){
    	runExperiments(text);
    	i=i+1;
	}
    return 0;
}
