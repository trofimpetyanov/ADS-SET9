#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

const int ALPHABET_SIZE = 256;

inline int char_at(const std::string& s, int d) {
    if (d < s.length())
        return (unsigned char)s[d];
    else
        return -1;
}

void msd_sort_recursive(std::vector<std::string>& arr, int lo, int hi, int d, std::vector<std::string>& aux) {
    if (hi <= lo) {
        return;
    }

    std::vector<int> freqs(ALPHABET_SIZE + 1, 0); 
    for (int i = lo; i <= hi; ++i) {
        freqs[char_at(arr[i], d) + 1]++;
    }

    std::vector<int> starting_indices(ALPHABET_SIZE + 2, 0);
    for (int k_bucket = 0; k_bucket < ALPHABET_SIZE + 1; ++k_bucket) {
        starting_indices[k_bucket + 1] = starting_indices[k_bucket] + freqs[k_bucket];
    }

    for (int i = lo; i <= hi; ++i) {
        int bucket_k = char_at(arr[i], d) + 1;
        aux[lo + starting_indices[bucket_k]++] = arr[i];
    }

    for (int i = lo; i <= hi; ++i) {
        arr[i] = aux[i];
    }

    int current_array_pos = lo;
    for (int k_bucket = 0; k_bucket <= ALPHABET_SIZE; ++k_bucket) {
        int count_for_this_bucket = freqs[k_bucket];
        if (count_for_this_bucket == 0) continue;

        int sub_array_lo = current_array_pos;
        int sub_array_hi = current_array_pos + count_for_this_bucket - 1;

        if (k_bucket > 0) { 
            msd_sort_recursive(arr, sub_array_lo, sub_array_hi, d + 1, aux);
        }
        current_array_pos += count_for_this_bucket;
    }
}

void msd_radix_sort(std::vector<std::string>& arr) {
    if (arr.empty()) return;
    int n = arr.size();
    std::vector<std::string> aux(n);
    msd_sort_recursive(arr, 0, n - 1, 0, aux);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;
    std::vector<std::string> strings(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> strings[i];
    }

    msd_radix_sort(strings);

    for (int i = 0; i < n; ++i) {
        std::cout << strings[i] << std::endl;
    }

    return 0;
} 