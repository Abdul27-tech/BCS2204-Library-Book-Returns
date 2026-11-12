#include <iostream>
#include <string>
#include "DoublyLinkedList.h"
#include "Sorting.h"

struct IntLess { bool operator()(int a, int b) const { return a < b; } };
struct IntCompareKey { int operator()(int a, int key) const { return (a > key) - (a < key); } };

int main() {
    int passed = 0;
    auto check = [&](bool condition, const char* name) {
        std::cout << (condition ? "PASS: " : "FAIL: ") << name << '\n';
        if (condition) ++passed;
    };

    DoublyLinkedList<BookReturn> list;
    check(list.empty() && list.size() == 0, "empty structure boundary");

    BookReturn a{1, "Algorithms", "Amina", "2026-11-01"};
    BookReturn b{2, "Data Structures", "Brian", "2026-11-02"};
    BookReturn c{3, "Algorithms", "Carol", "2026-11-03"};
    check(list.append(a) && list.size() == 1, "one record insertion");
    check(!list.append(a), "duplicate return ID rejected");
    check(list.append(b) && list.append(c), "records including duplicate title inserted");
    check(list.findById(2) != nullptr, "search existing ID");
    check(list.findById(99) == nullptr, "search absent ID");

    DoublyLinkedList<BookReturn> copied(list);
    check(copied.size() == 3 && copied.findById(1) != list.findById(1), "deep copy uses separate nodes");
    copied.removeById(1);
    check(list.size() == 3 && copied.size() == 2, "copy can change without changing original");

    check(list.removeById(1) && list.size() == 2, "remove first record");
    check(list.removeById(3) && list.size() == 1, "remove last record");
    check(!list.removeById(999), "remove missing record safely");
    check(list.removeById(2) && list.empty(), "remove only remaining record");

    int sorted[] = {1, 2, 3, 4, 5};
    insertionSort(sorted, 5, IntLess{});
    check(sorted[0] == 1 && sorted[4] == 5, "insertion sort on already sorted input");
    int reversed[] = {5, 4, 3, 2, 1};
    randomizedQuickSort(reversed, 5, IntLess{}, 1234);
    check(reversed[0] == 1 && reversed[4] == 5, "random-pivot quicksort on reversed input");
    int values[] = {1, 2, 3, 4, 5};
    check(iterativeBinarySearch(values, 5, 4, IntCompareKey{}) == 3, "binary search existing key");
    check(iterativeBinarySearch(values, 5, 9, IntCompareKey{}) == -1, "binary search absent key");
    int emptyArraySearch = iterativeBinarySearch(values, 0, 1, IntCompareKey{});
    check(emptyArraySearch == -1, "binary search empty array");

    DoublyLinkedList<BookReturn> loaded;
    check(loaded.loadFromFile("book_returns.txt") && loaded.size() == 10, "load ten sample records");
    check(loaded.saveToFile("test_saved.txt"), "save records to text file");

    std::cout << "TOTAL: " << passed << "/19 checks passed\n";
    return passed == 19 ? 0 : 1;
}
