#include <iostream>
#include "huffman.h"
#include "lz.h"
#include "performance.h"

int main() {
    std::string text = "tangananica-tanganana";

    // Huffman Coding
    auto start = Performance::now();
    std::unordered_map<char, std::string> huffmanCode = buildHuffmanTree(text);
    std::string huffmanEncoded = huffmanEncode(text, huffmanCode);
    auto end = Performance::now();
    auto huffmanEncodeTime = Performance::duration(start, end);

    start = Performance::now();
    std::string huffmanDecoded = huffmanDecode(huffmanEncoded, huffmanCode);
    end = Performance::now();
    auto huffmanDecodeTime = Performance::duration(start, end);

    std::cout << "Huffman Encoded: " << huffmanEncoded << std::endl;
    std::cout << "Huffman Decoded: " << huffmanDecoded << std::endl;
    std::cout << "Huffman Encode Time: " << huffmanEncodeTime << " ms" << std::endl;
    std::cout << "Huffman Decode Time: " << huffmanDecodeTime << " ms" << std::endl;

    // LZ Compression
    start = Performance::now();
    std::vector<LZPair> lzCompressed = lzCompress(text);
    end = Performance::now();
    auto lzCompressTime = Performance::duration(start, end);

    start = Performance::now();
    std::string lzDecompressed = lzDecompress(lzCompressed);
    end = Performance::now();
    auto lzDecompressTime = Performance::duration(start, end);

    std::cout << "LZ Compressed: ";
    for (const auto& pair : lzCompressed) {
        std::cout << "(" << pair.pos << ", " << pair.len << ", " << pair.nextChar << ") ";
    }
    std::cout << std::endl;
    std::cout << "LZ Decompressed: " << lzDecompressed << std::endl;
    std::cout << "LZ Compress Time: " << lzCompressTime << " ms" << std::endl;
    std::cout << "LZ Decompress Time: " << lzDecompressTime << " ms" << std::endl;

    return 0;
}
