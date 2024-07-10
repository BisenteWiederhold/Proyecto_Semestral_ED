#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include "lz.cpp"
#include "huffman.h"

// Función para obtener el tamaño de la memoria utilizada en bytes
size_t getMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.PeakWorkingSetSize;
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
    auto end = high_resolution_clock::now();
    auto huffmanDurationCo = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    std::string huffmanEncoded = huffmanTree.encode(text);
    std::string huffmanDecoded = huffmanTree.decode(huffmanEncoded);
    end = high_resolution_clock::now();
    auto huffmanDurationDes = duration_cast<microseconds>(end - start).count();

    size_t huffmanMemoryUsage = getMemoryUsage();

    // Medir el tiempo y el tamaño de LZ Compression
    start = high_resolution_clock::now();
    auto lzCompressed = lzCompress(text);
    end = high_resolution_clock::now();
    auto lzDurationCo = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    std::string lzDecompressed = lzDecompress(lzCompressed);
    end = high_resolution_clock::now();
    auto lzDurationDes = duration_cast<microseconds>(end - start).count();
    size_t lzMemoryUsage = getMemoryUsage();

    // Imprimir resultados del Huffman y LZ
    std::cout << "Resultados para " << inputFile << ":" << std::endl;
    std::cout << "Huffman Encoding time: " << huffmanDurationCo << " microseconds" << std::endl;
    std::cout << "Huffman Decoding time: " << huffmanDurationDes << " microseconds" << std::endl;
    std::cout << "Huffman Memory usage: " << huffmanMemoryUsage << " bytes" << std::endl;
    std::cout << "LZ Compressed: ";
    for (const auto& pair : lzCompressed) {
        if (pair.second == 0) {
            std::cout << "(" << static_cast<char>(pair.first) << ", 0) ";
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
    std::vector<std::string> inputFiles = {
        "input.txt",
        "input2.txt",
        "input3.txt",
        "input4.txt",
        "input5.txt"
    };

    std::string outputFile = "experimentacion.csv";

    // Escribir encabezados en el archivo de salida
    std::ofstream output(outputFile);
    if (output) {
        output << "Archivo,Tiempo_Huffman_Compresion,Tiempo_Huffman_Descompresion,Memoria_Huffman,Tiempo_LZ_Compresion,Tiempo_LZ_Descompresion,Memoria_LZ" << std::endl;
        output.close();
    } else {
        std::cerr << "Error: Could not open output file." << std::endl;
        return 1;
    }

    for (const auto& inputFile : inputFiles) {
        runExperiments(inputFile, outputFile);
    }

    return 0;
}
