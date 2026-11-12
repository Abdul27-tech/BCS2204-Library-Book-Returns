#ifndef SORTING_H
#define SORTING_H

#include <random>
#include <utility>
#include "DoublyLinkedList.h"

// Comparator must return true when the first item should appear before the second.
template <typename T, typename Compare>
void insertionSort(T data[], int size, Compare before) {
    for (int i = 1; i < size; ++i) {
        T key = data[i];
        int j = i - 1;
        while (j >= 0 && before(key, data[j])) {
            data[j + 1] = data[j];
            --j;
        }
        data[j + 1] = key;
    }
}

template <typename T, typename Compare>
void quickSortRecursive(T data[], int low, int high, Compare before,
                        std::mt19937& generator) {
    if (low >= high) return;

    std::uniform_int_distribution<int> choose(low, high);
    int pivotIndex = choose(generator);
    T pivot = data[pivotIndex];

    // Three-way partition handles repeated keys without repeatedly splitting them.
    int smaller = low;
    int current = low;
    int larger = high;

    while (current <= larger) {
        if (before(data[current], pivot)) {
            std::swap(data[smaller], data[current]);
            ++smaller;
            ++current;
        } else if (before(pivot, data[current])) {
            std::swap(data[current], data[larger]);
            --larger;
        } else {
            ++current;
        }
    }

    quickSortRecursive(data, low, smaller - 1, before, generator);
    quickSortRecursive(data, larger + 1, high, before, generator);
}

template <typename T, typename Compare>
void randomizedQuickSort(T data[], int size, Compare before,
                         unsigned int seed = std::random_device{}()) {
    if (size < 2) return;
    std::mt19937 generator(seed);
    quickSortRecursive(data, 0, size - 1, before, generator);
}

template <typename T, typename Key, typename Compare>
int iterativeBinarySearch(const T data[], int size, const Key& key,
                          Compare compare) {
    int low = 0;
    int high = size - 1;
    while (low <= high) {
        int middle = low + (high - low) / 2;
        int result = compare(data[middle], key);
        if (result == 0) return middle;
        if (result < 0) low = middle + 1;
        else high = middle - 1;
    }
    return -1;
}

struct BookTitleOrder {
    bool operator()(const BookReturn& first, const BookReturn& second) const {
        return first.bookTitle < second.bookTitle;
    }
};

struct CompareBookTitleWithKey {
    // Negative: record title comes before key; positive: after key.
    int operator()(const BookReturn& record, const std::string& key) const {
        if (record.bookTitle < key) return -1;
        if (record.bookTitle > key) return 1;
        return 0;
    }
};

#endif
