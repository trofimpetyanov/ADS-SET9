#ifndef STRING_SORT_TESTER_H
#define STRING_SORT_TESTER_H

#include <string>
#include <vector>
#include <chrono>
#include <functional>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "StringGenerator.h"

struct SortStats {
    std::string algorithmName;
    std::string dataType;
    int arraySize;
    double time_ms;
    long long comparisons;
};

using SortFunction = std::function<void(std::vector<std::string>&, long long&)>;

class StringSortTester {
public:
    StringSortTester(StringGenerator& generator, int minSize = 100, int maxSize = 3000, int step = 100, int runs = 5);

    static int compareStringsAndCount(const std::string& s1, const std::string& s2, long long& comparison_counter);

    SortStats testSort(SortFunction sortFunc, std::vector<std::string>& data, int num_runs);
    
    void runAllExperiments();
    void printResults(const std::vector<SortStats>& all_stats) const;
    void saveResultsToCSV(const std::vector<SortStats>& all_stats, const std::string& filename) const;

private:
    StringGenerator& strGen;
    int minArrSize;
    int maxArrSize;
    int arrStep;
    int numRuns;
    std::vector<SortStats> results;

    std::vector<std::string> generateData(const std::string& type, int size);

    void registerSortAlgorithm(const std::string& name, SortFunction func);

    struct RegisteredAlgorithm {
        std::string name;
        SortFunction func;
    };
    std::vector<RegisteredAlgorithm> algorithmsToTest;

};

#endif 