#include <vector>
#include <string>
#include <unordered_map>



std::vector<std::pair<int, int>> lzCompress(const std::string &input) {
    std::unordered_map<std::string, int> dictionary;
    std::vector<std::pair<int, int>> result;
    std::string buffer;

    int position = 0;
    for (char c : input) {
        std::string current = buffer + c;
        if (dictionary.find(current) != dictionary.end()) {
            buffer = current;
        } else {
            if (!buffer.empty()) {
                if (dictionary.find(buffer) == dictionary.end()) {
                    dictionary[buffer] = dictionary.size();
                }
                result.push_back({dictionary[buffer], static_cast<int>(buffer.length())});
                buffer.clear();
            } else {
                result.push_back({static_cast<int>(c), 0});
            }
        }
        position++;
    }

    if (!buffer.empty()) {
        result.push_back({dictionary[buffer], static_cast<int>(buffer.length())});
    }

    return result;
}

std::string lzDecompress(const std::vector<std::pair<int, int>>& compressed) {
    std::unordered_map<int, std::string> dictionary;
    std::string result;

    for (int i = 0; i < 256; ++i) {
        dictionary[i] = std::string(1, static_cast<char>(i));
    }

    for (const auto& pair : compressed) {
        std::string entry;
        if (pair.first < dictionary.size()) {
            entry = dictionary[pair.first];
        } else {
            entry = dictionary[pair.first / 256] + dictionary[pair.first % 256][0];
        }

        result += entry;

        if (dictionary.size() < 4096) {
            dictionary[dictionary.size()] = dictionary[pair.first / 256] + entry[0];
        }
    }

    return result;
}
