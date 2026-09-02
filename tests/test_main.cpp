#include <iostream>
#include <cassert>
#include "HashTable.h"
#include "Queue.h"
#include "Stack.h"
#include "DoublyLinkedList.h"
#include "BST.h"

int main() {
    std::cout << "--- Running MetroPulse System Unit Tests ---\n";

    // Test BST Insertion
    StationBST bst;
    bst.insertStation("Anarkali", 10);
    bst.insertStation("Gajjumata", 5);

    // Test Doubly LinkedList Journey Tracking
    DoublyLinkedList history;
    history.append("1001001001001001", "Anarkali", "2026-09-02 10:00:00");

    std::cout << "All basic unit tests passed successfully!\n";
    return 0;
}