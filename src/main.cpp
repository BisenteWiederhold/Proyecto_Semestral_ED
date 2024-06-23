#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <unistd.h>
#include <sys/resource.h>
#include "huffman.h"
#include "lz.cpp"

// Función para obtener el tamaño de la memoria utilizada en bytes en Linux
size_t getMemoryUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}

void runExperiments(const std::string& inputFile, const std::string& outputFile) {
    using namespace std::chrono;

    // Leer el archivo de entrada
    std::ifstream input(inputFile);
    if (!input) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string text = buffer.str();

    // Medir el tiempo y el tamaño de Huffman Encoding
    auto start = high_resolution_clock::now();
    HuffmanTree huffmanTree(text);
    std::string huffmanEncoded = huffmanTree.encode(text);
    auto end = high_resolution_clock::now();
    auto huffmanDurationCo = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    std::string huffmanDecoded = huffmanTree.decode(huffmanEncoded);
    end = high_resolution_clock::now();
    size_t huffmanMemoryUsage = getMemoryUsage();
    auto huffmanDurationDes = duration_cast<microseconds>(end - start).count();

    // Medir el tiempo y el tamaño de LZ Compression
    start = high_resolution_clock::now();
    std::vector<std::pair<int, int>> lzCompressed = lzCompress(text);
    end = high_resolution_clock::now();
    auto lzDurationCo = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    std::string lzDecompressed = lzDecompress(lzCompressed);
    end = high_resolution_clock::now();
    size_t lzMemoryUsage = getMemoryUsage();
    auto lzDurationDes = duration_cast<microseconds>(end - start).count();

    // Imprimir resultados del Huffman y LZ
    std::cout << "Resultados para " << inputFile << ":" << std::endl;
    std::cout << "Huffman Encoded: " << huffmanEncoded << std::endl;
    std::cout << "Huffman Decoded: " << huffmanDecoded << std::endl;
    std::cout << "Huffman Encoding time: " << huffmanDurationCo << " microseconds" << std::endl;
    std::cout << "Huffman Decoding time: " << huffmanDurationDes << " microseconds" << std::endl;
    std::cout << "Huffman Memory usage: " << huffmanMemoryUsage << " bytes" << std::endl;
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
    std::cout << "LZ Compression time: " << lzDurationCo << " microseconds" << std::endl;
    std::cout << "LZ Decompression time: " << lzDurationDes << " microseconds" << std::endl;
    std::cout << "LZ Memory usage: " << lzMemoryUsage << " bytes" << std::endl;

    // Guardar resultados en el archivo "experimentacion.csv"
    std::ofstream output(outputFile, std::ios_base::app);
    if (!output) {
        std::cerr << "Error: Could not open output file." << std::endl;
        return;
    }

    output << inputFile << "," << huffmanDurationCo << "," << huffmanDurationDes << "," << huffmanMemoryUsage << ","
           << lzDurationCo << "," << lzDurationDes << "," << lzMemoryUsage << std::endl;

    output.close();
}

int main() {
    // Procesar el archivo input.txt original
    runExperiments("input.txt", "experimentacion.csv");

    // Procesar los archivos input2.txt hasta input5.txt
    for (int i = 2; i <= 5; ++i) {
        std::string inputFile = "input" + std::to_string(i) + ".txt";
        runExperiments(inputFile, "experimentacion.csv");
    }

    return 0;
}
