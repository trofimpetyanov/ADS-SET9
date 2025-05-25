#ifndef SORT_ALGOS_FOR_TESTER_H
#define SORT_ALGOS_FOR_TESTER_H

#include <vector>
#include <string>
#include <algorithm>
#include "StringSortTester.h"

namespace SortAlgosForTester {

void standard_MergeSort(std::vector<std::string>& arr, long long& comparison_counter);
void standard_QuickSort(std::vector<std::string>& arr, long long& comparison_counter);

void adapted_A1m_MergeSort(std::vector<std::string>& arr, long long& comparison_counter);
void adapted_A1q_TernaryQuickSort(std::vector<std::string>& arr, long long& comparison_counter);
void adapted_A1r_MSDRadixSort(std::vector<std::string>& arr, long long& comparison_counter);
void adapted_A1rq_MSDRadixQuickSort(std::vector<std::string>& arr, long long& comparison_counter);

inline int charAt_for_tester(const std::string& s, int d) {
    if (d < s.length())
        return (unsigned char)s[d];
    else
        return -1; 
}

namespace {
    void standard_merge(std::vector<std::string>& arr, int l, int m, int r, long long& comparison_counter) {
        int n1 = m - l + 1;
        int n2 = r - m;
        std::vector<std::string> L(n1), R(n2);
        for (int i = 0; i < n1; i++) L[i] = arr[l + i];
        for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
        int i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
            if (StringSortTester::compareStringsAndCount(L[i], R[j], comparison_counter) <= 0) {
                arr[k++] = L[i++];
            } else {
                arr[k++] = R[j++];
            }
        }
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
    }

    void standard_merge_sort_recursive(std::vector<std::string>& arr, int l, int r, long long& comparison_counter) {
        if (l < r) {
            int m = l + (r - l) / 2;
            standard_merge_sort_recursive(arr, l, m, comparison_counter);
            standard_merge_sort_recursive(arr, m + 1, r, comparison_counter);
            standard_merge(arr, l, m, r, comparison_counter);
        }
    }

    int standard_partition(std::vector<std::string>& arr, int low, int high, long long& comparison_counter) {
        std::string pivot = arr[high];
        int i = (low - 1);
        for (int j = low; j <= high - 1; j++) {
            if (StringSortTester::compareStringsAndCount(arr[j], pivot, comparison_counter) < 0) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        return (i + 1);
    }

    void standard_quicksort_recursive(std::vector<std::string>& arr, int low, int high, long long& comparison_counter) {
        if (low < high) {
            int pi = standard_partition(arr, low, high, comparison_counter);
            standard_quicksort_recursive(arr, low, pi - 1, comparison_counter);
            standard_quicksort_recursive(arr, pi + 1, high, comparison_counter);
        }
    }
    
    int char_at(const std::string& s, int d, long long& comparison_counter) {
        comparison_counter++; 
        if (d < (int)s.length()) return static_cast<unsigned char>(s[d]);
        return -1; 
    }

    void exch(std::vector<std::string>& a, int i, int j) {
        std::string temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }

    void quicksort_3way_radix(std::vector<std::string>& a, int lo, int hi, int d, long long& comparison_counter) {
        if (hi <= lo) return;
        int lt = lo, gt = hi;
        int v = char_at(a[lo], d, comparison_counter);
        int i = lo + 1;
        while (i <= gt) {
            int t = char_at(a[i], d, comparison_counter);
            if (t < v) exch(a, lt++, i++);
            else if (t > v) exch(a, i, gt--);
            else i++;
        }
        quicksort_3way_radix(a, lo, lt - 1, d, comparison_counter);
        if (v >= 0) quicksort_3way_radix(a, lt, gt, d + 1, comparison_counter);
        quicksort_3way_radix(a, gt + 1, hi, d, comparison_counter);
    }

    const int R_adapted = 256; 
    const int CUTOFF_adapted = 74;

    void msd_radix_sort_recursive(std::vector<std::string>& a, int lo, int hi, int d, std::vector<std::string>& aux, long long& comparison_counter) {
        if (hi <= lo + CUTOFF_adapted) {
            quicksort_3way_radix(a, lo, hi, d, comparison_counter);
            return;
        }

        std::vector<int> count(R_adapted + 2, 0);
        for (int i = lo; i <= hi; i++) {
            count[char_at(a[i], d, comparison_counter) + 2]++;
        }
        for (int r = 0; r < R_adapted + 1; r++) {
            count[r + 1] += count[r];
        }
        for (int i = lo; i <= hi; i++) {
            aux[count[char_at(a[i], d, comparison_counter) + 1]++] = a[i];
        }
        for (int i = lo; i <= hi; i++) {
            a[i] = aux[i - lo];
        }
        for (int r = 0; r < R_adapted; r++) {
            msd_radix_sort_recursive(a, lo + count[r], lo + count[r+1] - 1, d + 1, aux, comparison_counter);
        }
    }

     void msd_radix_sort_pure_recursive(std::vector<std::string>& a, int lo, int hi, int d, std::vector<std::string>& aux, long long& comparison_counter) {
        if (hi <= lo) return;

        std::vector<int> count(R_adapted + 2, 0);
        for (int i = lo; i <= hi; i++) {
            count[char_at(a[i], d, comparison_counter) + 2]++;
        }
        for (int r = 0; r < R_adapted + 1; r++) {
            count[r + 1] += count[r];
        }
        for (int i = lo; i <= hi; i++) {
            aux[count[char_at(a[i], d, comparison_counter) + 1]++] = a[i];
        }
        for (int i = lo; i <= hi; i++) {
            a[i] = aux[i - lo];
        }
        for (int r = 0; r < R_adapted; r++) {
            msd_radix_sort_pure_recursive(a, lo + count[r], lo + count[r+1] - 1, d + 1, aux, comparison_counter);
        }
    }
}

}

#endif