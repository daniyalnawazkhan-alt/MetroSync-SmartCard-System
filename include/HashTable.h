#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include "Card.h"


class HashTable {
private:
    struct Node {
        Card data;
        Node* next;
        explicit Node(const Card& card);
    };

    Node** buckets;     // array of bucket head pointers, size == capacity
    size_t capacity;    // number of buckets (kept prime to reduce clustering)
    size_t itemCount;   // number of cards currently stored

    static const double MAX_LOAD_FACTOR;

    unsigned long hashKey(const std::string& key) const;
    static bool isPrime(size_t n);
    static size_t nextPrime(size_t n);
    void rehash(long long& comparisonsAccumulator);
    void copyFrom(const HashTable& other);
    void destroy();

public:
    explicit HashTable(size_t initialCapacity = 1031);
    ~HashTable();
    HashTable(const HashTable& other);
    HashTable& operator=(const HashTable& other);

    // Inserts a new card. If a card with the same cardNumber already exists,
    // its record is overwritten in place. Returns true if a *new* card was
    // added, false if an existing one was overwritten.
    bool insert(const Card& card, OperationStats& stats);

    // Looks up a card by number; copies the stored record into outCard.
    // Returns false (outCard untouched) if not found.
    bool search(const std::string& cardNumber, Card& outCard, OperationStats& stats) const;

    // Returns a mutable pointer to the stored card (nullptr if absent).
    // Used internally to mutate balance / block flags / tap state in place
    // without a copy-out-modify-copy-back round trip.
    Card* find(const std::string& cardNumber, OperationStats& stats);

    bool updateBalance(const std::string& cardNumber, double newBalance, OperationStats& stats);
    bool setBlocked(const std::string& cardNumber, bool blocked, OperationStats& stats);
    bool remove(const std::string& cardNumber, OperationStats& stats);

    size_t size() const;
    size_t bucketCount() const;
    double loadFactor() const;

    // Visits every stored card via a plain function pointer 
    // Used for end-of-day dumps / full-registry reports.
    void forEach(void (*visitor)(const Card&)) const;
};

#endif 