#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

struct BookReturn {
    int returnId{};
    std::string bookTitle;
    std::string borrowerName;
    std::string returnDate;

    std::string toLine() const {
        return std::to_string(returnId) + "|" + bookTitle + "|" +
               borrowerName + "|" + returnDate;
    }

    static bool fromLine(const std::string& line, BookReturn& record) {
        std::stringstream input(line);
        std::string idText;
        if (!std::getline(input, idText, '|') ||
            !std::getline(input, record.bookTitle, '|') ||
            !std::getline(input, record.borrowerName, '|') ||
            !std::getline(input, record.returnDate)) {
            return false;
        }
        try {
            std::size_t used = 0;
            int id = std::stoi(idText, &used);
            if (used != idText.size() || id <= 0 ||
                record.bookTitle.empty() || record.borrowerName.empty() ||
                record.returnDate.empty()) return false;
            record.returnId = id;
            return true;
        } catch (...) {
            return false;
        }
    }
};

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* previous;
        Node* next;
        explicit Node(const T& value) : data(value), previous(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    std::size_t count;

    void clear() noexcept {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = tail = nullptr;
        count = 0;
    }

    void swap(DoublyLinkedList& other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(count, other.count);
    }

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    // Deep copy: create a new node for every node in the source list.
    DoublyLinkedList(const DoublyLinkedList& other)
        : head(nullptr), tail(nullptr), count(0) {
        try {
            for (Node* current = other.head; current != nullptr; current = current->next)
                append(current->data);
        } catch (...) {
            clear();
            throw;
        }
    }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            DoublyLinkedList temporary(other);
            swap(temporary);
        }
        return *this;
    }

    ~DoublyLinkedList() { clear(); }

    bool empty() const noexcept { return count == 0; }
    std::size_t size() const noexcept { return count; }

    bool containsId(int id) const {
        for (Node* current = head; current != nullptr; current = current->next)
            if (current->data.returnId == id) return true;
        return false;
    }

    bool append(const T& record) {
        if (record.returnId <= 0 || record.bookTitle.empty() ||
            record.borrowerName.empty() || record.returnDate.empty() ||
            record.bookTitle.find('|') != std::string::npos ||
            record.borrowerName.find('|') != std::string::npos ||
            record.returnDate.find('|') != std::string::npos ||
            containsId(record.returnId)) return false;

        Node* newNode = new Node(record);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            newNode->previous = tail;
            tail->next = newNode;
            tail = newNode;
        }
        ++count;
        return true;
    }

    bool removeById(int id) {
        for (Node* current = head; current != nullptr; current = current->next) {
            if (current->data.returnId == id) {
                if (current->previous != nullptr)
                    current->previous->next = current->next;
                else
                    head = current->next;

                if (current->next != nullptr)
                    current->next->previous = current->previous;
                else
                    tail = current->previous;

                delete current;
                --count;
                return true;
            }
        }
        return false;
    }

    T* findById(int id) {
        for (Node* current = head; current != nullptr; current = current->next)
            if (current->data.returnId == id) return &current->data;
        return nullptr;
    }

    const T* findById(int id) const {
        for (Node* current = head; current != nullptr; current = current->next)
            if (current->data.returnId == id) return &current->data;
        return nullptr;
    }

    bool copyToArray(T* destination, std::size_t capacity) const {
        if (capacity < count || (count > 0 && destination == nullptr)) return false;
        std::size_t index = 0;
        for (Node* current = head; current != nullptr; current = current->next)
            destination[index++] = current->data;
        return true;
    }

    void display() const {
        if (empty()) {
            std::cout << "No book return records are available.\n";
            return;
        }
        for (Node* current = head; current != nullptr; current = current->next) {
            std::cout << "ID: " << current->data.returnId
                      << " | Book: " << current->data.bookTitle
                      << " | Borrower: " << current->data.borrowerName
                      << " | Date: " << current->data.returnDate << '\n';
        }
    }

    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) return false;

        DoublyLinkedList<T> temporary;
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            T record{};
            if (!T::fromLine(line, record) || !temporary.append(record)) return false;
        }
        if (file.bad()) return false;
        swap(temporary); // Existing data is replaced only after a successful load.
        return true;
    }

    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) return false;
        for (Node* current = head; current != nullptr; current = current->next)
            file << current->data.toLine() << '\n';
        return file.good();
    }
};

#endif
