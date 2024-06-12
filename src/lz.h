#ifndef LZ_H
#define LZ_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

// Estructura para representar un par (p, l)
struct LZPair {
    int pos;
    int len;
    char nextChar;

    LZPair(int p, int l, char c) : pos(p), len(l), nextChar(c) {}
};

// Compresión LZ
std::vector<LZPair> lzCompress(const std::string& text) {
    std::vector<LZPair> compressed;
    std::unordered_map<std::string, int> dictionary;

    int i = 0;
    while (i < text.size()) {
        std::string match = "";
        int j = i;
        while (j < text.size() && dictionary.find(match + text[j]) != dictionary.end()) {
            match += text[j];
            j++;
        }

        if (j == text.size()) {
            compressed.push_back(LZPair(dictionary[match], match.size(), '\0'));
        } else {
            if (match.empty()) {
                compressed.push_back(LZPair(0, 0, text[j]));
                dictionary[std::string(1, text[j])] = i;
            } else {
                compressed.push_back(LZPair(dictionary[match], match.size(), text[j]));
                dictionary[match + text[j]] = i;
            }
        }
        i += match.size() + 1;
    }

    return compressed;
}

// Descompresión LZ
std::string lzDecompress(const std::vector<LZPair>& compressed) {
    std::string decompressed = "";

    for (const LZPair& pair : compressed) {
        if (pair.pos == 0 && pair.len == 0) {
            decompressed += pair.nextChar;
        } else {
            decompressed += decompressed.substr(pair.pos, pair.len) + pair.nextChar;
        }
    }

    return decompressed;
}

#endif // LZ_H
