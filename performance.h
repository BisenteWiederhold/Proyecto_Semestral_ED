#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

class PerformanceStats {
public:
    void measureExecutionTime(std::function<void()> func, const std::string& label) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << label << ": " << duration.count() << " microseconds" << std::endl;
    }

    void measureSizeInBytes(const std::string& data, const std::string& label) {
        std::cout << label << ": " << data.size() * sizeof(char) << " bytes" << std::endl;
    }

    void measureSizeInBytes(const std::vector<std::pair<std::string, int>>& data, const std::string& label) {
        int size = 0;
        for (const auto& pair : data) {
            size += pair.first.size() * sizeof(char) + sizeof(pair.second);
        }
        std::cout << label << ": " << size << " bytes" << std::endl;
    }

    void printStats() {
        std::cout << "----- Performance Stats -----" << std::endl;
        for (const auto& stat : stats) {
            std::cout << stat << std::endl;
        }
        std::cout << "-----------------------------" << std::endl;
    }

private:
    std::vector<std::string> stats;
};

#endif // PERFORMANCE_H
