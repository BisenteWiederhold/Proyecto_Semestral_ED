#include "lz.h"
#include <unordered_map>
#include <sstream>

std::vector<std::pair<std::string, int>> lzCompress(const std::string& input) {
    std::vector<std::pair<std::string, int>> compressed;
    std::unordered_map<std::string, int> dictionary;
    int dictSize = 1;

    std::string w;
    for (char c : input) {
        std::string wc = w + c;
        if (dictionary.find(wc) != dictionary.end()) {
            w = wc;
        } else {
            compressed.push_back({w.empty() ? std::string(1, c) : w, dictionary[w]});
            dictionary[wc] = dictSize++;
            w = c;
        }
    }

    if (!w.empty()) {
        compressed.push_back({w, dictionary[w]});
    }

    return compressed;
}

std::string lzDecompress(const std::vector<std::pair<std::string, int>>& compressed) {
    if (compressed.empty()) {
        return "";
    }

    std::unordered_map<int, std::string> dictionary;
    int dictSize = 0; // Cambiamos a 0 para que coincida con los índices de compressed

    std::stringstream decompressed;
    std::string w;

    for (const auto& pair : compressed) {
        std::string entry;
        if (pair.second == 0) {
            entry = pair.first;
        } else if (dictionary.find(pair.second) != dictionary.end()) {
            entry = dictionary[pair.second];
        } else {
            // Si no se encuentra en el diccionario, se añade una nueva entrada
            entry = w + w[0];
        }

        decompressed << entry;

        if (!entry.empty()) {
            dictionary[++dictSize] = entry + entry[0]; // Usamos dictSize+1 para la siguiente entrada
        }
    }

    return decompressed.str();
}
