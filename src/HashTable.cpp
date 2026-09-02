#include "../include/HashTable.h"

const double HashTable::MAX_LOAD_FACTOR = 0.75;

HashTable::Node::Node(const Card& card) : data(card), next(nullptr) {}




unsigned long HashTable::hashKey(const std::string& key) const {
    unsigned long h = 5381;
    for (size_t i = 0; i < key.length(); ++i) {
        h = ((h << 5) + h) + static_cast<unsigned char>(key[i]); // h*33 + c
    }
    return h % capacity;
}

bool HashTable::isPrime(size_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (size_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

size_t HashTable::nextPrime(size_t n) {
    size_t candidate = (n % 2 == 0) ? n + 1 : n;
    while (!isPrime(candidate)) {
        candidate += 2;
    }
    return candidate;
}

HashTable::HashTable(size_t initialCapacity)
    : buckets(nullptr), capacity(0), itemCount(0) {
    capacity = nextPrime(initialCapacity < 8 ? 8 : initialCapacity);
    buckets = new Node*[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        buckets[i] = nullptr;
    }
}

void HashTable::destroy() {
    if (buckets == nullptr) return;
    for (size_t i = 0; i < capacity; ++i) {
        Node* cur = buckets[i];
        while (cur != nullptr) {
            Node* toDelete = cur;
            cur = cur->next;
            delete toDelete;
        }
    }
    delete[] buckets;
    buckets = nullptr;
    capacity = 0;
    itemCount = 0;
}

HashTable::~HashTable() {
    destroy();
}

void HashTable::copyFrom(const HashTable& other) {
    capacity = other.capacity;
    itemCount = 0;
    buckets = new Node*[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        buckets[i] = nullptr;
    }
    // Deep-copy every chain, preserving bucket order.
    for (size_t i = 0; i < other.capacity; ++i) {
        Node* srcNode = other.buckets[i];
        Node* prevCopy = nullptr;
        while (srcNode != nullptr) {
            Node* newNode = new Node(srcNode->data);
            if (prevCopy == nullptr) {
                buckets[i] = newNode;
            } else {
                prevCopy->next = newNode;
            }
            prevCopy = newNode;
            srcNode = srcNode->next;
            ++itemCount;
        }
    }
}

HashTable::HashTable(const HashTable& other) : buckets(nullptr), capacity(0), itemCount(0) {
    copyFrom(other);
}

HashTable& HashTable::operator=(const HashTable& other) {
    if (this != &other) {
        destroy();
        copyFrom(other);
    }
    return *this;
}

void HashTable::rehash(long long& comparisonsAccumulator) {
    size_t oldCapacity = capacity;
    Node** oldBuckets = buckets;

    capacity = nextPrime(oldCapacity * 2);
    buckets = new Node*[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        buckets[i] = nullptr;
    }
    itemCount = 0;

    for (size_t i = 0; i < oldCapacity; ++i) {
        Node* cur = oldBuckets[i];
        while (cur != nullptr) {
            Node* next = cur->next;
            unsigned long idx = hashKey(cur->data.cardNumber);
            ++comparisonsAccumulator;
            cur->next = buckets[idx];
            buckets[idx] = cur;
            ++itemCount;
            cur = next;
        }
    }
    delete[] oldBuckets;
}

bool HashTable::insert(const Card& card, OperationStats& stats) {
    stats.comparisons = 0;

    if ((static_cast<double>(itemCount + 1) / static_cast<double>(capacity)) > MAX_LOAD_FACTOR) {
        rehash(stats.comparisons);
    }

    unsigned long idx = hashKey(card.cardNumber);
    Node* cur = buckets[idx];
    while (cur != nullptr) {
        ++stats.comparisons;
        if (cur->data.cardNumber == card.cardNumber) {
            cur->data = card; // overwrite existing record
            return false;
        }
        cur = cur->next;
    }

    Node* newNode = new Node(card);
    newNode->next = buckets[idx];
    buckets[idx] = newNode;
    ++itemCount;
    return true;
}

bool HashTable::search(const std::string& cardNumber, Card& outCard, OperationStats& stats) const {
    stats.comparisons = 0;
    unsigned long idx = hashKey(cardNumber);
    Node* cur = buckets[idx];
    while (cur != nullptr) {
        ++stats.comparisons;
        if (cur->data.cardNumber == cardNumber) {
            outCard = cur->data;
            return true;
        }
        cur = cur->next;
    }
    return false;
}

Card* HashTable::find(const std::string& cardNumber, OperationStats& stats) {
    stats.comparisons = 0;
    unsigned long idx = hashKey(cardNumber);
    Node* cur = buckets[idx];
    while (cur != nullptr) {
        ++stats.comparisons;
        if (cur->data.cardNumber == cardNumber) {
            return &(cur->data);
        }
        cur = cur->next;
    }
    return nullptr;
}

bool HashTable::updateBalance(const std::string& cardNumber, double newBalance, OperationStats& stats) {
    Card* card = find(cardNumber, stats);
    if (card == nullptr) {
        return false;
    }
    card->balance = newBalance;
    return true;
}

bool HashTable::setBlocked(const std::string& cardNumber, bool blocked, OperationStats& stats) {
    Card* card = find(cardNumber, stats);
    if (card == nullptr) {
        return false;
    }
    card->isBlocked = blocked;
    return true;
}

bool HashTable::remove(const std::string& cardNumber, OperationStats& stats) {
    stats.comparisons = 0;
    unsigned long idx = hashKey(cardNumber);
    Node* cur = buckets[idx];
    Node* prev = nullptr;
    while (cur != nullptr) {
        ++stats.comparisons;
        if (cur->data.cardNumber == cardNumber) {
            if (prev == nullptr) {
                buckets[idx] = cur->next;
            } else {
                prev->next = cur->next;
            }
            delete cur;
            --itemCount;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

size_t HashTable::size() const {
    return itemCount;
}

size_t HashTable::bucketCount() const {
    return capacity;
}

double HashTable::loadFactor() const {
    return static_cast<double>(itemCount) / static_cast<double>(capacity);
}

void HashTable::forEach(void (*visitor)(const Card&)) const {
    if (visitor == nullptr) return;
    for (size_t i = 0; i < capacity; ++i) {
        Node* cur = buckets[i];
        while (cur != nullptr) {
            visitor(cur->data);
            cur = cur->next;
        }
    }
}