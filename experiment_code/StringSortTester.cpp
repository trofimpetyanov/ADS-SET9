#include "StringSortTester.h"
#include "StringGenerator.h" 
#include "SortAlgosForTester.h" 
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <vector>
#include <string>
#include <chrono>
#include <iomanip> 
#include <map>

StringSortTester::StringSortTester(StringGenerator& generator, int minSize, int maxSize, int step, int runs)
    : strGen(generator), minArrSize(minSize), maxArrSize(maxSize), arrStep(step), numRuns(runs) {

    registerSortAlgorithm("A1m MergeSort (Adapted)", SortAlgosForTester::adapted_A1m_MergeSort);
    registerSortAlgorithm("A1q TernaryQuickSort (Adapted)", SortAlgosForTester::adapted_A1q_TernaryQuickSort);
    registerSortAlgorithm("A1r MSD RadixSort (Adapted)", SortAlgosForTester::adapted_A1r_MSDRadixSort);
    registerSortAlgorithm("A1rq MSD+QuickSort (Adapted)", SortAlgosForTester::adapted_A1rq_MSDRadixQuickSort);
    registerSortAlgorithm("Standard MergeSort", SortAlgosForTester::standard_MergeSort);
    registerSortAlgorithm("Standard QuickSort", SortAlgosForTester::standard_QuickSort);
}

int StringSortTester::compareStringsAndCount(const std::string& s1, const std::string& s2, long long& comparison_counter) {
    comparison_counter += std::min(s1.length(), s2.length()) + 1; 
    if (s1 < s2) return -1;
    if (s1 > s2) return 1;
    return 0;
}

SortStats StringSortTester::testSort(SortFunction sort_func, std::vector<std::string>& original_data, int num_runs) {
    SortStats stats;
    if (original_data.empty() && num_runs <=0) {
      stats.time_ms = 0;
      stats.comparisons = 0;
      return stats;
    }
    if (num_runs <= 0) num_runs = 1; 

    if (!original_data.empty()) {
        std::vector<std::string> data_for_comparison_count = original_data;
        stats.comparisons = 0; 
        sort_func(data_for_comparison_count, stats.comparisons);
    } else {
        stats.comparisons = 0;
    }

    std::chrono::duration<double, std::milli> total_duration(0.0);

    for (int i = 0; i < num_runs; ++i) {
        std::vector<std::string> data_to_sort = original_data; 
        long long dummy_comparison_counter = 0; 
        
        auto start_time = std::chrono::high_resolution_clock::now();
        if (!data_to_sort.empty()) {
            sort_func(data_to_sort, dummy_comparison_counter); 
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        total_duration += (end_time - start_time);
    }

    stats.time_ms = total_duration.count() / num_runs;
    
    return stats;
}

void StringSortTester::runAllExperiments() {
    results.clear();
    std::vector<std::string> dataTypes = {"Random", "ReverseSorted", "AlmostSorted", "CommonPrefix"};

    for (int size = minArrSize; size <= maxArrSize; size += arrStep) {
        std::cout << "Processing array size: " << size << std::endl;
        for (const auto& dataType : dataTypes) {
            std::cout << "  Data type: " << dataType << std::endl;
            std::vector<std::string> baseData = generateData(dataType, size);
            if (baseData.empty()) {
                std::cerr << "Failed to generate data for type: " << dataType << " size: " << size << std::endl;
                continue;
            }

            for (const auto& algo : algorithmsToTest) {
                SortStats currentStats;
                currentStats = testSort(algo.func, baseData, numRuns); 
                currentStats.algorithmName = algo.name;
                currentStats.dataType = dataType;
                currentStats.arraySize = size;
                results.push_back(currentStats);
                 std::cout << "    Algorithm: " << algo.name << "... Done. Time: " 
                          << std::fixed << std::setprecision(3) << currentStats.time_ms << "ms, Comps: " 
                          << currentStats.comparisons << std::endl;
            }
        }
    }
    std::cout << "\n--- Experiment Results ---" << std::endl;
    printResults(results);
    saveResultsToCSV(results, "sort_experiment_results.csv");
    std::cout << "\nExperiments finished. Results saved to sort_experiment_results.csv" << std::endl;
     std::cout << "String sort experiments finished." << std::endl;
}

void StringSortTester::printResults(const std::vector<SortStats>& all_stats) const {
    std::cout << std::left << std::setw(30) << "Algorithm" 
              << std::setw(18) << "Data Type" 
              << std::setw(12) << "Array Size" 
              << std::setw(15) << "Time (ms)" 
              << std::setw(20) << "Char Comparisons" << std::endl;
    std::cout << std::string(95, '-') << std::endl;

    for (const auto& stats : all_stats) {
        std::cout << std::left << std::setw(30) << stats.algorithmName
                  << std::setw(18) << stats.dataType
                  << std::setw(12) << stats.arraySize
                  << std::fixed << std::setprecision(3) << std::setw(15) << stats.time_ms
                  << std::setw(20) << stats.comparisons << std::endl;
    }
    std::cout << std::string(95, '-') << std::endl;
}

void StringSortTester::saveResultsToCSV(const std::vector<SortStats>& all_stats, const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    outFile << "Algorithm,DataType,ArraySize,Time_ms,CharComparisons\n";

    for (const auto& stats : all_stats) {
        outFile << "\"" << stats.algorithmName << "\"," 
                << "\"" << stats.dataType << "\"," 
                << stats.arraySize << "," 
                << std::fixed << std::setprecision(5) << stats.time_ms << "," 
                << stats.comparisons << "\n";
    }
    outFile.close();
}

std::vector<std::string> StringSortTester::generateData(const std::string& type, int size) {
    if (type == "Random") return strGen.generateRandomStringArray(size);
    if (type == "ReverseSorted") return strGen.generateReverseSortedArray(size);
    if (type == "AlmostSorted") return strGen.generateAlmostSortedArray(size);
    if (type == "CommonPrefix") return strGen.generateArrayWithCommonPrefix(size); 
    return {}; 
}

void StringSortTester::registerSortAlgorithm(const std::string& name, SortFunction func) {
    algorithmsToTest.push_back({name, func});
} 