#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "huffman.cpp"
#include "lz.cpp"

// Función para obtener el tamaño de la memoria utilizada en bytes




int main() {
    // Leer el archivo de entrada
    std::ifstream inputFile("input5.txt");
    if (!inputFile) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string text = buffer.str();
    HuffmanTree huffmanTree(text);
    std::string huffmanEncoded = huffmanTree.encode(text);
    std::string huffmanDecoded = huffmanTree.decode(huffmanEncoded);
    std::cout << "Huffman Compressed:" << huffmanEncoded << std::endl;
    std::cout << "Huffman Decompressed: " << huffmanDecoded << std::endl;
    
    
    std::string text2 = buffer.str();
    std::vector<std::pair<int, int>> lzCompressed = lzCompress(text2);
    std::string lzDecompressed = lzDecompress(lzCompressed);
    std::cout << "LZ Compressed: ";
    for (const auto& pair : lzCompressed) {
        if (pair.second == 0) {
            std::cout << "(" << static_cast<char>(pair.first) << ", " << pair.second << ") ";
        } else {
            std::cout << "(" << pair.first << ", " << pair.second << ") ";
        }
    }
  	std::cout <<  std::endl;
    std::cout << "LZ Decompressed: " << lzDecompressed << std::endl;

    return 0;
}
