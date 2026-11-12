BCS 2204 - Library Book Returns Register
Registration number: VU-BCS-2603-2525-DAY
Scenario: Library Book Returns Register
Primary data structure: Doubly linked list

Files:
- DoublyLinkedList.h: BookReturn structure and doubly linked list template
- Sorting.h: template insertion sort, random-pivot quicksort and iterative binary search
- main.cpp: menu-driven application
- book_returns.txt: ten starting records

Compile (Linux/macOS):
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o library
./library

Compile on Windows with MinGW:
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o library.exe
library.exe

For memory checking on a compiler that supports AddressSanitizer:
g++ -std=c++17 -Wall -Wextra -pedantic -fsanitize=address -fno-omit-frame-pointer main.cpp -o library_asan
./library_asan

Keep book_returns.txt in the program's working directory. The program loads it at start
and saves changes when option 8 is selected or when the program exits.

The sorting routines work on a temporary dynamic array copied from the linked list.
The linked list remains the primary storage structure. The array is not an STL container.
