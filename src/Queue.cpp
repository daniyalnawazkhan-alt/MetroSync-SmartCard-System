#include "../include/Queue.h"

Passenger::Passenger()
    : cardNumber(""), gateId(""), station(""), arrivalTimestamp(0) {}

Passenger::Passenger(const std::string& cardNumberIn, const std::string& gateIdIn,
                      const std::string& stationIn, long long arrivalTimestampIn)
    : cardNumber(cardNumberIn), gateId(gateIdIn), station(stationIn),
      arrivalTimestamp(arrivalTimestampIn) {}

Queue::Node::Node(const Passenger& p) : data(p), next(nullptr) {}

Queue::Queue() : frontPtr(nullptr), backPtr(nullptr), count(0) {}

void Queue::destroy() {
    Node* cur = frontPtr;
    while (cur != nullptr) {
        Node* toDelete = cur;
        cur = cur->next;
        delete toDelete;
    }
    frontPtr = nullptr;
    backPtr = nullptr;
    count = 0;
}

Queue::~Queue() {
    destroy();
}

void Queue::copyFrom(const Queue& other) {
    frontPtr = nullptr;
    backPtr = nullptr;
    count = 0;
    Node* cur = other.frontPtr;
    while (cur != nullptr) {
        Node* newNode = new Node(cur->data);
        if (backPtr == nullptr) {
            frontPtr = newNode;
            backPtr = newNode;
        } else {
            backPtr->next = newNode;
            backPtr = newNode;
        }
        ++count;
        cur = cur->next;
    }
}

Queue::Queue(const Queue& other) : frontPtr(nullptr), backPtr(nullptr), count(0) {
    copyFrom(other);
}

Queue& Queue::operator=(const Queue& other) {
    if (this != &other) {
        destroy();
        copyFrom(other);
    }
    return *this;
}

void Queue::enqueue(const Passenger& p, OperationStats& stats) {
    stats.comparisons = 1; // O(1) pointer append -- one bookkeeping step
    Node* newNode = new Node(p);
    if (backPtr == nullptr) {
        frontPtr = newNode;
        backPtr = newNode;
    } else {
        backPtr->next = newNode;
        backPtr = newNode;
    }
    ++count;
}

bool Queue::dequeue(Passenger& outPassenger, OperationStats& stats) {
    stats.comparisons = 1; // O(1) pointer detach
    if (frontPtr == nullptr) {
        return false;
    }
    Node* toDelete = frontPtr;
    outPassenger = toDelete->data;
    frontPtr = frontPtr->next;
    if (frontPtr == nullptr) {
        backPtr = nullptr;
    }
    delete toDelete;
    --count;
    return true;
}

bool Queue::peekFront(Passenger& outPassenger) const {
    if (frontPtr == nullptr) {
        return false;
    }
    outPassenger = frontPtr->data;
    return true;
}

bool Queue::isEmpty() const {
    return frontPtr == nullptr;
}

size_t Queue::size() const {
    return count;
}