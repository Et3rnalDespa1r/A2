#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <string>

struct ArrayGenerator {
    std::mt19937_64 rng;
    int max_value;

    explicit ArrayGenerator(int max_val = 6000)
            : rng(123456789u), max_value(max_val) {}

    std::vector<long long> generate_random(int n) {
        std::uniform_int_distribution<int> dist(0, max_value);
        std::vector<long long> a(n);
        for (int i = 0; i < n; ++i) {
            a[i] = dist(rng);
        }
        return a;
    }

    std::vector<long long> generate_reversed(int n) {
        auto a = generate_random(n);
        std::sort(a.begin(), a.end());
        std::reverse(a.begin(), a.end());
        return a;
    }

    std::vector<long long> generate_almost_sorted(int n, int swaps = 10) {
        auto a = generate_random(n);
        std::sort(a.begin(), a.end());
        if (n <= 1) return a;
        std::uniform_int_distribution<int> pos(0, n - 1);
        for (int k = 0; k < swaps; ++k) {
            int i = pos(rng);
            int j = pos(rng);
            std::swap(a[i], a[j]);
        }
        return a;
    }
};

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

void merge_core(std::vector<long long> &a,
                std::vector<long long> &tmp,
                int l, int r) {
    int len = r - l;
    if (len <= 1) return;
    int m = l + len / 2;
    merge_core(a, tmp, l, m);
    merge_core(a, tmp, m, r);
    int i = l, j = m, k = 0;
    while (i < m && j < r) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else tmp[k++] = a[j++];
    }
    while (i < m) tmp[k++] = a[i++];
    while (j < r) tmp[k++] = a[j++];
    for (int t = 0; t < k; ++t) a[l + t] = tmp[t];
}

void merge_hybrid(std::vector<long long> &a,
                  std::vector<long long> &tmp,
                  int l, int r,
                  int threshold) {
    int len = r - l;
    if (len <= 1) return;
    if (len <= threshold) {
        insertion_sort(a, l, r);
        return;
    }
    int m = l + len / 2;
    merge_hybrid(a, tmp, l, m, threshold);
    merge_hybrid(a, tmp, m, r, threshold);
    int i = l, j = m, k = 0;
    while (i < m && j < r) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else tmp[k++] = a[j++];
    }
    while (i < m) tmp[k++] = a[i++];
    while (j < r) tmp[k++] = a[j++];
    for (int t = 0; t < k; ++t) a[l + t] = tmp[t];
}

long long measure_merge(std::vector<long long> a) {
    int n = (int)a.size();
    std::vector<long long> tmp(n);
    auto start = std::chrono::high_resolution_clock::now();
    merge_core(a, tmp, 0, n);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}

long long measure_hybrid(std::vector<long long> a, int threshold) {
    int n = (int)a.size();
    std::vector<long long> tmp(n);
    auto start = std::chrono::high_resolution_clock::now();
    merge_hybrid(a, tmp, 0, n, threshold);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    ArrayGenerator gen(6000);

    std::ofstream out("results_a2.csv");
    out << "тип_массива,размер,алгоритм,threshold,время_мкс\n";

    const int trials = 5;
    std::vector<int> thresholds = {5, 10, 20, 30, 50};

    for (int n = 500; n <= 100000; n += 100) {
        auto base_random = gen.generate_random(n);
        auto base_reversed = gen.generate_reversed(n);
        auto base_almost = gen.generate_almost_sorted(n, std::max(1, n / 100));

        auto test_types = {
                std::make_pair(std::string("random"), base_random),
                std::make_pair(std::string("reversed"), base_reversed),
                std::make_pair(std::string("almost"), base_almost)
        };

        for (auto &p : test_types) {
            const std::string &type_name = p.first;
            const std::vector<long long> &base = p.second;

            long long sum_merge = 0;
            for (int t = 0; t < trials; ++t) {
                sum_merge += measure_merge(base);
            }
            long long avg_merge = sum_merge / trials;
            out << type_name << "," << n << ",merge,0," << avg_merge << "\n";

            for (int thr : thresholds) {
                long long sum_h = 0;
                for (int t = 0; t < trials; ++t) {
                    sum_h += measure_hybrid(base, thr);
                }
                long long avg_h = sum_h / trials;
                out << type_name << "," << n << ",hybrid," << thr << "," << avg_h << "\n";
            }
        }
    }

    return 0;
}