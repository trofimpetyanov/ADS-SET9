#ifndef STRING_GENERATOR_H
#define STRING_GENERATOR_H

#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <vector>

class StringGenerator {
public:
    StringGenerator(int minLen = 10, int maxLen = 200, const std::string& charSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{};':\",./<>?");

    std::string generateRandomString();
    std::vector<std::string> generateRandomStringArray(int size);
    std::vector<std::string> generateReverseSortedArray(int size);
    std::vector<std::string> generateAlmostSortedArray(int size, int swaps = -1);
    std::vector<std::string> generateArrayWithCommonPrefix(int size, int prefixLength = 5);


private:
    int minStrLen;
    int maxStrLen;
    std::string characters;
    std::mt19937 rng;
};

#endif 