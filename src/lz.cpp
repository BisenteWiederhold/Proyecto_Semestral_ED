#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>

//Comprime una cadena de entrada utilizando el algoritmo Lempel-Ziv
std::vector<std::pair<int, int>> lzCompress(const std::string &input) {
    std::unordered_map<std::string, int> dictionary;
    std::vector<std::pair<int, int>> result;
    std::string buffer;
    //Este algoritmo construye un diccionario de secuencia encontradas de secuencia de pares
    for (size_t i = 0; i < input.length(); ++i) {
        buffer += input[i];
        if (dictionary.find(buffer) == dictionary.end()) {
            if (buffer.length() > 1) {
                std::string prefix = buffer.substr(0, buffer.length() - 1);
                result.push_back({dictionary[prefix], static_cast<int>(buffer.length() - 1)});
                dictionary[buffer] = i - buffer.length() + 1;
                buffer = buffer.substr(buffer.length() - 1);
            }
            if (dictionary.find(buffer) == dictionary.end()) {
        	    dictionary[buffer] = i - buffer.length() + 1;
    	        result.push_back({buffer[0], 0});
	            buffer.clear();
			}
        }
    }
    
    if (!buffer.empty()) {
        result.push_back({dictionary[buffer], static_cast<int>(buffer.length())});
    }
    
    return result;
}
//Descomprime una secuencia comprimida generada por el algoritmo Lempel-Ziv.
std::string lzDecompress(const std::vector<std::pair<int, int>> &compressed) {
    std::string result;
    std::string input; 
    int currentIndex = 0; 
    
    //Este algoritmo reconstruye la cadena original utilizando la secuencia de pares
    for (const auto &pair : compressed) {
        if (pair.second == 0) {
            result += pair.first; 
            input += pair.first; 
            currentIndex++;
        } else {
            std::string substring = input.substr(pair.first, pair.second); 
            result += substring; 
            input += substring; 
            currentIndex += pair.second;
        }
    }

    return result;

}
