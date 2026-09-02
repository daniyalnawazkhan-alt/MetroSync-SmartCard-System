#include "../include/Stack.h"

TopUpRecord::TopUpRecord()
    : cardNumber(""), amount(0.0), previousBalance(0.0), timestamp(0) {}

TopUpRecord::TopUpRecord(const std::string& cardNumberIn, double amountIn,
                          double previousBalanceIn, long long timestampIn)
    : cardNumber(cardNumberIn), amount(amountIn),
      previousBalance(previousBalanceIn), timestamp(timestampIn) {}

Stack::Node::Node(const TopUpRecord& r) : data(r), next(nullptr) {}

Stack::Stack() : topPtr(nullptr), count(0) {}

void Stack::destroy() {
    Node* cur = topPtr;
    while (cur != nullptr) {
        Node* toDelete = cur;
        cur = cur->next;
        delete toDelete;
    }
    topPtr = nullptr;
    count = 0;
}

Stack::~Stack() {
    destroy();
}

void Stack::copyFrom(const Stack& other) {

    topPtr = nullptr;
    count = 0;

    // Step 1: build a reversed copy (bottom-of-other becomes top-of-reversed).
    Node* reversedHead = nullptr;
    Node* cur = other.topPtr;
    while (cur != nullptr) {
        Node* newNode = new Node(cur->data);
        newNode->next = reversedHead;
        reversedHead = newNode;
        cur = cur->next;
    }

    // Step 2: re-reverse in place to restore original top-to-bottom order.
    Node* prev = nullptr;
    cur = reversedHead;
    while (cur != nullptr) {
        Node* next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    topPtr = prev;
    count = other.count;
}

Stack::Stack(const Stack& other) : topPtr(nullptr), count(0) {
    copyFrom(other);
}

Stack& Stack::operator=(const Stack& other) {
    if (this != &other) {
        destroy();
        copyFrom(other);
    }
    return *this;
}

void Stack::push(const TopUpRecord& r, OperationStats& stats) {
    stats.comparisons = 1; // O(1) pointer prepend
    Node* newNode = new Node(r);
    newNode->next = topPtr;
    topPtr = newNode;
    ++count;
}

bool Stack::pop(TopUpRecord& outRecord, OperationStats& stats) {
    stats.comparisons = 1; // O(1) pointer detach
    if (topPtr == nullptr) {
        return false;
    }
    Node* toDelete = topPtr;
    outRecord = toDelete->data;
    topPtr = topPtr->next;
    delete toDelete;
    --count;
    return true;
}

bool Stack::peek(TopUpRecord& outRecord) const {
    if (topPtr == nullptr) {
        return false;
    }
    outRecord = topPtr->data;
    return true;
}

bool Stack::isEmpty() const {
    return topPtr == nullptr;
}

size_t Stack::size() const {
    return count;
}