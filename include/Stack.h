#ifndef STACK_H
#define STACK_H

#include <string>
#include "Card.h"


struct TopUpRecord {
    std::string cardNumber;
    double amount;
    double previousBalance;  // balance immediately before this top-up
    long long timestamp;     // epoch microseconds

    TopUpRecord();
    TopUpRecord(const std::string& cardNumberIn, double amountIn,
                double previousBalanceIn, long long timestampIn);
};


class Stack {
private:
    struct Node {
        TopUpRecord data;
        Node* next;
        explicit Node(const TopUpRecord& r);
    };

    Node* topPtr;
    size_t count;

    void copyFrom(const Stack& other);
    void destroy();

public:
    Stack();
    ~Stack();
    Stack(const Stack& other);
    Stack& operator=(const Stack& other);

    // O(1): pushes a new top-up record onto the top of the stack.
    void push(const TopUpRecord& r, OperationStats& stats);

    // O(1): pops and returns the most recently pushed record.
    // Returns false (outRecord untouched) if the stack is empty.
    bool pop(TopUpRecord& outRecord, OperationStats& stats);

    bool peek(TopUpRecord& outRecord) const;
    bool isEmpty() const;
    size_t size() const;
};

#endif 