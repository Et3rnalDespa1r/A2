#include <iostream>
#include <vector>

const int INSERTION_THRESHOLD = 15;

void insertion_sort(std::vector<long long> &a, int l, int r) {
    for (int i = l + 1; i < r; ++i) {
        long long key = a[i];
        int j = i - 1;
        while (j >= l && a[j] > key) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = key;
    }
}
void merge_sort(std::vector<long long> &a, std::vector<long long> &tmp, int l, int r) {
    int len = r - l;
    if (len <= 1) {
        return;
    }
    if (len <= INSERTION_THRESHOLD) {
        insertion_sort(a, l, r);
        return;
    }
    int m = l + (r - l) / 2;
    merge_sort(a, tmp, l, m);
    merge_sort(a, tmp, m, r);
    int i = l;
    int j = m;
    int k = 0;
    while (i < m && j < r) {
        if (a[i] <= a[j]) {
            tmp[k++] = a[i++];
        } else {
            tmp[k++] = a[j++];
        }
    }
    while (i < m) {
        tmp[k++] = a[i++];
    }
    while (j < r) {
        tmp[k++] = a[j++];
    }
    for (int t = 0; t < k; ++t) {
        a[l + t] = tmp[t];
    }
}
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    if (!(std::cin >> n)) {
        return 0;
    }
    std::vector<long long> a;
    a.reserve(n);
    for (int i = 0; i < n; ++i) {
        long long x;
        std::cin >> x;
        a.push_back(x);
    }
    if (n > 0) {
        std::vector<long long> tmp(n);
        merge_sort(a, tmp, 0, n);
    }
    for (int i = 0; i < n; ++i) {
        if (i) std::cout << ' ';
        std::cout << a[i];
    }
    return 0;
}
