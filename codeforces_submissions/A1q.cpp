#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int charAt(const std::string& s, int d) {
    if (d < s.length())
        return s[d];
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

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;
    std::vector<std::string> strings(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> strings[i];
    }

    if (n > 0) {
        quickSort3Way(strings, 0, n - 1, 0);
    }

    for (int i = 0; i < n; ++i) {
        std::cout << strings[i] << std::endl;
    }

    return 0;
} 