#include "StringGenerator.h"
#include <algorithm>
#include <chrono>
#include <iostream>

StringGenerator::StringGenerator(int minLen, int maxLen, const std::string& charSet)
    : minStrLen(minLen), maxStrLen(maxLen), characters(charSet) {
    std::random_device rd;
    rng.seed(rd());
}

std::string StringGenerator::generateRandomString() {
    std::uniform_int_distribution<int> len_dist(minStrLen, maxStrLen);
    int length = len_dist(rng);
    std::uniform_int_distribution<int> char_dist(0, characters.length() - 1);
    std::string s(length, ' ');
    for (int i = 0; i < length; ++i) {
        s[i] = characters[char_dist(rng)];
    }
    return s;
}

std::vector<std::string> StringGenerator::generateRandomStringArray(int size) {
    std::vector<std::string> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = generateRandomString();
    }
    return arr;
}

std::vector<std::string> StringGenerator::generateReverseSortedArray(int size) {
    std::vector<std::string> arr = generateRandomStringArray(size);
    std::sort(arr.begin(), arr.end(), std::greater<std::string>());
    return arr;
}

std::vector<std::string> StringGenerator::generateAlmostSortedArray(int size, int swaps) {
    std::vector<std::string> arr = generateRandomStringArray(size);
    std::sort(arr.begin(), arr.end());
    int num_swaps = (swaps == -1) ? std::max(1, size / 20) : swaps;
    std::uniform_int_distribution<int> dist(0, size - 1);
    for (int i = 0; i < num_swaps; ++i) {
        int idx1 = dist(rng);
        int idx2 = dist(rng);
        if (idx1 != idx2) {
            std::swap(arr[idx1], arr[idx2]);
        }
    }
    return arr;
}

std::vector<std::string> StringGenerator::generateArrayWithCommonPrefix(int size, int prefixLength) {
    if (prefixLength <= 0 || prefixLength > minStrLen) {
        std::cerr << "Warning: Invalid prefixLength. Generating random array instead." << std::endl;
        return generateRandomStringArray(size);
    }

    std::vector<std::string> arr(size);
    std::string commonPrefixStr(prefixLength, ' ');
    std::uniform_int_distribution<int> char_dist(0, characters.length() - 1);

    for(int i = 0; i < prefixLength; ++i) {
        commonPrefixStr[i] = characters[char_dist(rng)];
    }

    for (int i = 0; i < size; ++i) {
        std::uniform_int_distribution<int> len_dist(minStrLen - prefixLength, maxStrLen - prefixLength);
        int suffixLength = len_dist(rng);
        std::string suffix(suffixLength, ' ');
        for (int j = 0; j < suffixLength; ++j) {
            suffix[j] = characters[char_dist(rng)];
        }
        arr[i] = commonPrefixStr + suffix;
    }
    
    std::shuffle(arr.begin(), arr.end(), rng);
    return arr;
} 