#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
#include "Sorting.h"

struct IntegerLess { bool operator()(int a, int b) const { return a < b; } };

int main() {
    const int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000};
    std::mt19937 generator(20261112);
    std::cout << "n,insertion_average_ms,quicksort_average_ms\n";
    for (int n : sizes) {
        double insertionTotal = 0.0, quickTotal = 0.0;
        for (int repetition = 0; repetition < 3; ++repetition) {
            std::vector<int> original(n);
            for (int i = 0; i < n; ++i) original[i] = i;
            std::shuffle(original.begin(), original.end(), generator);
            int* work = new int[n];

            std::copy(original.begin(), original.end(), work);
            auto start = std::chrono::steady_clock::now();
            insertionSort(work, n, IntegerLess{});
            auto stop = std::chrono::steady_clock::now();
            insertionTotal += std::chrono::duration<double, std::milli>(stop - start).count();

            std::copy(original.begin(), original.end(), work);
            start = std::chrono::steady_clock::now();
            randomizedQuickSort(work, n, IntegerLess{},
                                static_cast<unsigned int>(7000 + n + repetition));
            stop = std::chrono::steady_clock::now();
            quickTotal += std::chrono::duration<double, std::milli>(stop - start).count();
            delete[] work;
        }
        std::cout << n << ',' << std::fixed << std::setprecision(3)
                  << insertionTotal / 3.0 << ',' << quickTotal / 3.0 << '\n';
    }
}
