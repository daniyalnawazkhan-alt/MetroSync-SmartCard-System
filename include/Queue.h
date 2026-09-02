#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include "Card.h"


struct Passenger {
    std::string cardNumber;
    std::string gateId;
    std::string station;
    long long arrivalTimestamp; // epoch microseconds, set by the caller

    Passenger();
    Passenger(const std::string& cardNumberIn, const std::string& gateIdIn,
              const std::string& stationIn, long long arrivalTimestampIn);
};


class Queue {
private:
    struct Node {
        Passenger data;
        Node* next;
        explicit Node(const Passenger& p);
    };

    Node* frontPtr;
    Node* backPtr;
    size_t count;

    void copyFrom(const Queue& other);
    void destroy();

public:
    Queue();
    ~Queue();
    Queue(const Queue& other);
    Queue& operator=(const Queue& other);

    // O(1): appends to the back of the line.
    void enqueue(const Passenger& p, OperationStats& stats);

    // O(1): removes and returns the passenger at the front of the line.
    // Returns false (outPassenger untouched) if the queue is empty.
    bool dequeue(Passenger& outPassenger, OperationStats& stats);

    bool peekFront(Passenger& outPassenger) const;
    bool isEmpty() const;
    size_t size() const;
};

#endif 