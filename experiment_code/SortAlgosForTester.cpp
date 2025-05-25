#include "SortAlgosForTester.h"
#include "StringSortTester.h"
#include <algorithm>
#include <vector>

namespace SortAlgosForTester {

namespace {
void standard_Merge(std::vector<std::string>& arr, int left, int mid, int right, long long& comparison_counter) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<std::string> L(n1);
    std::vector<std::string> R(n2);
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    int i = 0;
    int j = 0;
    int k = left;
    while (i < n1 && j < n2) {
        if (StringSortTester::compareStringsAndCount(L[i], R[j], comparison_counter) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void standard_MergeSort_Recursive(std::vector<std::string>& arr, int left, int right, long long& comparison_counter) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        standard_MergeSort_Recursive(arr, left, mid, comparison_counter);
        standard_MergeSort_Recursive(arr, mid + 1, right, comparison_counter);
        standard_Merge(arr, left, mid, right, comparison_counter);
    }
}
}

void standard_MergeSort(std::vector<std::string>& arr, long long& comparison_counter) {
    if (arr.empty()) return;
    standard_MergeSort_Recursive(arr, 0, arr.size() - 1, comparison_counter);
}

namespace {
int standard_Partition_Lomuto(std::vector<std::string>& arr, int low, int high, long long& comparison_counter) {
    std::string pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (StringSortTester::compareStringsAndCount(arr[j], pivot, comparison_counter) <= 0) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void standard_QuickSort_Recursive(std::vector<std::string>& arr, int low, int high, long long& comparison_counter) {
    if (low < high) {
        int pi = standard_Partition_Lomuto(arr, low, high, comparison_counter);
        standard_QuickSort_Recursive(arr, low, pi - 1, comparison_counter);
        standard_QuickSort_Recursive(arr, pi + 1, high, comparison_counter);
    }
}
}

void standard_QuickSort(std::vector<std::string>& arr, long long& comparison_counter) {
    if (arr.empty()) return;
    standard_QuickSort_Recursive(arr, 0, arr.size() - 1, comparison_counter);
}

void adapted_A1m_MergeSort(std::vector<std::string>& arr, long long& comparison_counter) {
    if (arr.empty()) return;
    standard_MergeSort_Recursive(arr, 0, arr.size() - 1, comparison_counter);
}

static void ternary_quicksort_recursive_for_tester(std::vector<std::string>& arr, int low, int high, int d, long long& comparison_counter) {
    if (high <= low) {
        return;
    }
    int lt = low;
    int gt = high;
    comparison_counter++;
    int v = charAt_for_tester(arr[low], d);
    int i = low + 1;
    while (i <= gt) {
        comparison_counter++;
        int t = charAt_for_tester(arr[i], d);
        if (t < v)
            std::swap(arr[lt++], arr[i++]);
        else if (t > v)
            std::swap(arr[i], arr[gt--]);
        else
            i++;
    }
    ternary_quicksort_recursive_for_tester(arr, low, lt - 1, d, comparison_counter);
    if (v != -1) {
        ternary_quicksort_recursive_for_tester(arr, lt, gt, d + 1, comparison_counter);
    }
    ternary_quicksort_recursive_for_tester(arr, gt + 1, high, d, comparison_counter);
}

void adapted_A1q_TernaryQuickSort(std::vector<std::string>& arr, long long& comparison_counter) {
    if (arr.empty()) return;
    ternary_quicksort_recursive_for_tester(arr, 0, arr.size() - 1, 0, comparison_counter);
}

namespace { 
const int A1R_MSD_ALPHABET_SIZE_INTERNAL = 256;
void adapted_A1r_msd_sort_recursive_internal(
    std::vector<std::string>& arr, int lo, int hi, int d,
    std::vector<std::string>& aux, long long& comparison_counter)
{
    if (hi <= lo) {
        return;
    }
    std::vector<int> freqs(A1R_MSD_ALPHABET_SIZE_INTERNAL + 2, 0);
    for (int i = lo; i <= hi; ++i) {
        comparison_counter++;
        freqs[charAt_for_tester(arr[i], d) + 1]++;
    }
    std::vector<int> starting_indices(A1R_MSD_ALPHABET_SIZE_INTERNAL + 2, 0);
    for (int k_bucket_val = 0; k_bucket_val < A1R_MSD_ALPHABET_SIZE_INTERNAL + 1; ++k_bucket_val) {
        starting_indices[k_bucket_val + 1] = starting_indices[k_bucket_val] + freqs[k_bucket_val];
    }
    for (int i = lo; i <= hi; ++i) {
        int bucket_idx = charAt_for_tester(arr[i], d) + 1;
        aux[lo + starting_indices[bucket_idx]++] = arr[i];
    }
    for (int i = lo; i <= hi; ++i) {
        arr[i] = aux[i];
    }
    int current_array_pos = lo;
    for (int k_bucket_freq_idx = 0; k_bucket_freq_idx <= A1R_MSD_ALPHABET_SIZE_INTERNAL; ++k_bucket_freq_idx) {
        int count_for_this_bucket = freqs[k_bucket_freq_idx];
        if (count_for_this_bucket == 0) continue;
        int sub_array_lo = current_array_pos;
        int sub_array_hi = current_array_pos + count_for_this_bucket - 1;
        if (k_bucket_freq_idx > 0) { 
            adapted_A1r_msd_sort_recursive_internal(arr, sub_array_lo, sub_array_hi, d + 1, aux, comparison_counter);
        }
        current_array_pos += count_for_this_bucket;
    }
}
}

void adapted_A1r_MSDRadixSort(std::vector<std::string>& arr, long long& comparison_counter) {
    if (arr.empty()) return;
    int n = arr.size();
    std::vector<std::string> aux(n);
    adapted_A1r_msd_sort_recursive_internal(arr, 0, n - 1, 0, aux, comparison_counter);
}

namespace {
const int A1RQ_MSD_ALPHABET_SIZE_INTERNAL = 256;
const int A1RQ_MSD_TO_QUICKSORT_CUTOFF_INTERNAL = 74;

void adapted_A1rq_msd_radix_quick_sort_recursive_internal(
    std::vector<std::string>& arr, int lo, int hi, int d,
    std::vector<std::string>& aux, long long& comparison_counter)
{
    if (hi <= lo) {
        return;
    }

    if ((hi - lo + 1) < A1RQ_MSD_TO_QUICKSORT_CUTOFF_INTERNAL) {
        ternary_quicksort_recursive_for_tester(arr, lo, hi, d, comparison_counter);
        return;
    }

    std::vector<int> freqs(A1RQ_MSD_ALPHABET_SIZE_INTERNAL + 2, 0);
    for (int i = lo; i <= hi; ++i) {
        comparison_counter++;
        freqs[charAt_for_tester(arr[i], d) + 1]++;
    }

    std::vector<int> starting_indices(A1RQ_MSD_ALPHABET_SIZE_INTERNAL + 2, 0);
    for (int k_bucket_val = 0; k_bucket_val < A1RQ_MSD_ALPHABET_SIZE_INTERNAL + 1; ++k_bucket_val) {
        starting_indices[k_bucket_val + 1] = starting_indices[k_bucket_val] + freqs[k_bucket_val];
    }

    for (int i = lo; i <= hi; ++i) {
        int bucket_idx = charAt_for_tester(arr[i], d) + 1;
        aux[lo + starting_indices[bucket_idx]++] = arr[i];
    }

    for (int i = lo; i <= hi; ++i) {
        arr[i] = aux[i];
    }

    int current_array_pos = lo;
    for (int k_bucket_freq_idx = 0; k_bucket_freq_idx <= A1RQ_MSD_ALPHABET_SIZE_INTERNAL; ++k_bucket_freq_idx) {
        int count_for_this_bucket = freqs[k_bucket_freq_idx];
        if (count_for_this_bucket == 0) continue;

        int sub_array_lo = current_array_pos;
        int sub_array_hi = current_array_pos + count_for_this_bucket - 1;

        if (k_bucket_freq_idx > 0) { 
            adapted_A1rq_msd_radix_quick_sort_recursive_internal(arr, sub_array_lo, sub_array_hi, d + 1, aux, comparison_counter);
        }
        current_array_pos += count_for_this_bucket;
    }
}
}

void adapted_A1rq_MSDRadixQuickSort(std::vector<std::string>& arr, long long& comparison_counter) {
    if (arr.empty()) return;
    int n = arr.size();
    std::vector<std::string> aux(n);
    adapted_A1rq_msd_radix_quick_sort_recursive_internal(arr, 0, n - 1, 0, aux, comparison_counter);
}

} 