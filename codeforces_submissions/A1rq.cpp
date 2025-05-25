#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

const int MSD_ALPHABET_SIZE = 256;
const int MSD_TO_QUICKSORT_CUTOFF = 74;

inline int charAt(const std::string& s, int d) {
    if (d < s.length())
        return (unsigned char)s[d]; 
    else
        return -1;
}

void quickSort3Way(std::vector<std::string>& arr, int low, int high, int d) {
    if (high <= low) {
        return;
    }

    int lt = low;
    int gt = high;
    int v = charAt(arr[low], d);
    int i = low + 1;

    while (i <= gt) {
        int t = charAt(arr[i], d);
        if (t < v)
            std::swap(arr[lt++], arr[i++]);
        else if (t > v)
            std::swap(arr[i], arr[gt--]);
        else
            i++;
    }

    quickSort3Way(arr, low, lt - 1, d);
    if (v != -1) {
        quickSort3Way(arr, lt, gt, d + 1);
    }
    quickSort3Way(arr, gt + 1, high, d);
}

void msd_radix_quick_sort_recursive(std::vector<std::string>& arr, int lo, int hi, int d, std::vector<std::string>& aux) {
    if (hi <= lo) {
        return;
    }

    if ((hi - lo + 1) < MSD_TO_QUICKSORT_CUTOFF) {
        quickSort3Way(arr, lo, hi, d);
        return;
    }

    std::vector<int> freqs(MSD_ALPHABET_SIZE + 1, 0);
    for (int i = lo; i <= hi; ++i) {
        freqs[charAt(arr[i], d) + 1]++;
    }

    std::vector<int> starting_indices(MSD_ALPHABET_SIZE + 2, 0);
    for (int k_bucket = 0; k_bucket < MSD_ALPHABET_SIZE + 1; ++k_bucket) {
        starting_indices[k_bucket + 1] = starting_indices[k_bucket] + freqs[k_bucket];
    }

    for (int i = lo; i <= hi; ++i) {
        int bucket_k = charAt(arr[i], d) + 1;
        aux[lo + starting_indices[bucket_k]++] = arr[i];
    }

    for (int i = lo; i <= hi; ++i) {
        arr[i] = aux[i];
    }

    int current_array_pos = lo;
    for (int k_bucket = 0; k_bucket <= MSD_ALPHABET_SIZE; ++k_bucket) {
        int count_for_this_bucket = freqs[k_bucket];
        if (count_for_this_bucket == 0) continue;

        int sub_array_lo = current_array_pos;
        int sub_array_hi = current_array_pos + count_for_this_bucket - 1;

        if (k_bucket > 0) { 
            msd_radix_quick_sort_recursive(arr, sub_array_lo, sub_array_hi, d + 1, aux);
        }
        current_array_pos += count_for_this_bucket;
    }
}

void msd_radix_quick_sort(std::vector<std::string>& arr) {
    if (arr.empty()) return;
    int n = arr.size();
    std::vector<std::string> aux(n);
    msd_radix_quick_sort_recursive(arr, 0, n - 1, 0, aux);
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

    msd_radix_quick_sort(strings);

    for (int i = 0; i < n; ++i) {
        std::cout << strings[i] << std::endl;
    }

    return 0;
} 