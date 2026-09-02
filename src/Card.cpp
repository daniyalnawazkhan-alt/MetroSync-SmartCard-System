#include "../include/Card.h"
#include <sstream>
#include <cstdlib>

Card::Card()
    : cardNumber(""), holderName(""), cnic(""), balance(0.0),
      isBlocked(false), entryStation(""), hasTappedIn(false) {}

Card::Card(const std::string& cardNumberIn,
           const std::string& holderNameIn,
           const std::string& cnicIn,
           double balanceIn,
           bool isBlockedIn,
           const std::string& entryStationIn,
           bool hasTappedInIn)
    : cardNumber(cardNumberIn), holderName(holderNameIn), cnic(cnicIn),
      balance(balanceIn), isBlocked(isBlockedIn),
      entryStation(entryStationIn), hasTappedIn(hasTappedInIn) {}

OperationStats::OperationStats() : comparisons(0), microseconds(0) {}

// Manual digit check -- <cctype>'s isdigit() is intentionally avoided since
// it is outside the permitted header list.
static bool isAsciiDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isValidCardNumber(const std::string& cardNumber) {
    if (cardNumber.length() != 16) {
        return false;
    }
    for (size_t i = 0; i < cardNumber.length(); ++i) {
        if (!isAsciiDigit(cardNumber[i])) {
            return false;
        }
    }
    return true;
}

// Splits `line` on `delimiter`, appending each token into `outTokens`, an
// array supplied (and owned) by the caller. Returns the number of tokens
// written, or -1 if `line` contains more than `maxTokens` fields (guards
// against unbounded allocation without needing an STL container).
static int splitCSVLine(const std::string& line, char delimiter,
                         std::string* outTokens, int maxTokens) {
    std::stringstream ss(line);
    std::string token;
    int count = 0;
    while (std::getline(ss, token, delimiter)) {
        if (count >= maxTokens) {
            return -1;
        }
        outTokens[count] = token;
        ++count;
    }
    return count;
}

bool parseCardFromCSVLine(const std::string& line, Card& outCard) {
    if (line.empty()) {
        return false;
    }

    const int FIELD_COUNT = 7;
    std::string fields[FIELD_COUNT];
    int found = splitCSVLine(line, ',', fields, FIELD_COUNT);
    if (found != FIELD_COUNT) {
        return false;
    }

    const std::string& cardNumber = fields[0];
    if (!isValidCardNumber(cardNumber)) {
        return false;
    }

    double balance = std::strtod(fields[3].c_str(), nullptr);
    bool blocked = (fields[4] == "1" || fields[4] == "true" || fields[4] == "TRUE");
    bool tappedIn = (fields[6] == "1" || fields[6] == "true" || fields[6] == "TRUE");

    outCard = Card(cardNumber, fields[1], fields[2], balance, blocked, fields[5], tappedIn);
    return true;
}

std::string cardToCSVLine(const Card& card) {
    std::ostringstream oss;
    oss << card.cardNumber << ','
        << card.holderName << ','
        << card.cnic << ','
        << card.balance << ','
        << (card.isBlocked ? "1" : "0") << ','
        << card.entryStation << ','
        << (card.hasTappedIn ? "1" : "0");
    return oss.str();
}