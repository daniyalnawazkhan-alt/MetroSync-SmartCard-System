#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <iostream>
#include <string>

struct JourneyNode {
    std::string cardNum;
    std::string station;
    std::string timestamp;
    JourneyNode* prev;
    JourneyNode* next;

    JourneyNode(std::string c, std::string s, std::string t)
        : cardNum(c), station(s), timestamp(t), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    JourneyNode* head;
    JourneyNode* tail;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    ~DoublyLinkedList() {
        JourneyNode* current = head;
        while (current != nullptr) {
            JourneyNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void append(std::string cardNum, std::string station, std::string timestamp) {
        JourneyNode* newNode = new JourneyNode(cardNum, station, timestamp);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void printForward() const {
        JourneyNode* curr = head;
        while (curr) {
            std::cout << "[" << curr->timestamp << "] Card: " << curr->cardNum 
                      << " @ Station: " << curr->station << "\n";
            curr = curr->next;
        }
    }
};

#endif