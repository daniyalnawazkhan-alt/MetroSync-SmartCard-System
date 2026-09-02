#ifndef CARD_H
#define CARD_H

#include <string>


struct Card {
    std::string cardNumber;    // 16-digit unique card number (hash table key)
    std::string holderName;
    std::string cnic;
    double balance;
    bool isBlocked;
    std::string entryStation;  // station where the card currently has an open tap-in
    bool hasTappedIn;

    Card();

    Card(const std::string& cardNumberIn,
         const std::string& holderNameIn,
         const std::string& cnicIn,
         double balanceIn,
         bool isBlockedIn = false,
         const std::string& entryStationIn = "",
         bool hasTappedInIn = false);
};


struct OperationStats {
    long long comparisons;
    long long microseconds;

    OperationStats();
};


bool parseCardFromCSVLine(const std::string& line, Card& outCard);

// Serialises a Card back into a single CSV line (used for persistence,
// end-of-day dumps, and logging).
std::string cardToCSVLine(const Card& card);

// Validates that a card number is exactly 16 numeric digits. Implemented
// manually (no <cctype>) since the header allow-list does not include it.
bool isValidCardNumber(const std::string& cardNumber);

#endif 