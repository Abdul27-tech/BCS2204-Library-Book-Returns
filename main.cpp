#include <iostream>
#include <limits>
#include <string>
#include "DoublyLinkedList.h"
#include "Sorting.h"

using namespace std;

void showArray(const BookReturn* records, int size) {
    for (int i = 0; i < size; ++i)
        cout << records[i].returnId << " | " << records[i].bookTitle
             << " | " << records[i].borrowerName << " | "
             << records[i].returnDate << '\n';
}

int main() {
    DoublyLinkedList<BookReturn> library;
    if (!library.loadFromFile("book_returns.txt")) {
        cerr << "Could not load book_returns.txt. Check the file and its format.\n";
        return 1;
    }

    int choice = -1;
    do {
        cout << "\n===== LIBRARY BOOK RETURNS REGISTER =====\n"
             << "1. Display all records\n"
             << "2. Add a returned book\n"
             << "3. Search by return ID\n"
             << "4. Remove a return record\n"
             << "5. Sort records by book title (Insertion Sort)\n"
             << "6. Sort records by book title (Random-Pivot Quick Sort)\n"
             << "7. Binary search by book title (sort first)\n"
             << "8. Save records\n"
             << "0. Exit\nChoice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a number from the menu.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            library.display();
        } else if (choice == 2) {
            BookReturn record{};
            cout << "Return ID: ";
            if (!(cin >> record.returnId)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid ID.\n"; continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Book title: "; getline(cin, record.bookTitle);
            cout << "Borrower name: "; getline(cin, record.borrowerName);
            cout << "Return date (YYYY-MM-DD): "; getline(cin, record.returnDate);
            cout << (library.append(record) ? "Record added.\n" :
                     "Record rejected. Check fields and make sure the ID is unique.\n");
        } else if (choice == 3) {
            int id;
            cout << "Enter return ID: ";
            if (!(cin >> id)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid ID.\n"; continue;
            }
            const BookReturn* found = library.findById(id);
            if (found) cout << "Found: " << found->bookTitle << " returned by "
                            << found->borrowerName << " on " << found->returnDate << '\n';
            else cout << "No record has that ID.\n";
        } else if (choice == 4) {
            int id;
            cout << "Enter return ID to remove: ";
            if (!(cin >> id)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid ID.\n"; continue;
            }
            cout << (library.removeById(id) ? "Record removed.\n" : "Record not found.\n");
        } else if (choice == 5 || choice == 6 || choice == 7) {
            int size = static_cast<int>(library.size());
            if (size == 0) { cout << "There are no records to sort or search.\n"; continue; }
            BookReturn* records = new BookReturn[size];
            library.copyToArray(records, library.size());
            if (choice == 5) {
                insertionSort(records, size, BookTitleOrder{});
                cout << "Sorted using insertion sort:\n";
                showArray(records, size);
            } else {
                randomizedQuickSort(records, size, BookTitleOrder{});
                if (choice == 6) {
                    cout << "Sorted using random-pivot quicksort:\n";
                    showArray(records, size);
                } else {
                    string title;
                    cout << "Enter exact book title to search: ";
                    getline(cin, title);
                    int position = iterativeBinarySearch(records, size, title,
                                                          CompareBookTitleWithKey{});
                    if (position >= 0)
                        cout << "Found: " << records[position].bookTitle
                             << " (Return ID " << records[position].returnId << ")\n";
                    else cout << "That title was not found.\n";
                }
            }
            delete[] records;
        } else if (choice == 8) {
            cout << (library.saveToFile("book_returns.txt") ?
                     "Records saved.\n" : "Could not save records.\n");
        } else if (choice != 0) {
            cout << "Invalid menu option.\n";
        }
    } while (choice != 0);

    if (!library.saveToFile("book_returns.txt"))
        cerr << "Warning: records could not be saved on exit.\n";
    cout << "Program ended.\n";
    return 0;
}
