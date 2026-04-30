#include "Lexer.h"

bool Lexer::isValidCodeId(const std::string& text) const {
    if (text.size() < 5) {
        return false;
    }

    if (!(text[0] >= 'A' && text[0] <= 'Z') ||
        !(text[1] >= 'A' && text[1] <= 'Z') ||
        !(text[2] >= 'A' && text[2] <= 'Z') ||
        text[3] != '-') {
        return false;
    }

    for (std::size_t i = 4; i < text.size(); ++i) {
        if (!isDigit(text[i])) {
            return false;
        }
    }

    return true;
}

bool Lexer::isCodeIdLike(const std::string& text) const {
    if (text.size() < 4) {
        return false;
    }

    std::size_t dashPos = text.find('-');
    if (dashPos == std::string::npos) {
        return false;
    }

    if (dashPos == 0 || dashPos == text.size() - 1) {
        return false;
    }

    bool hasLetter = false;
    bool hasDigit = false;
    for (std::size_t i = 0; i < text.size(); ++i) {
        const char c = text[i];
        if (i == dashPos) {
            continue;
        }

        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            hasLetter = true;
        } else if (isDigit(c)) {
            hasDigit = true;
        }
    }

    return hasLetter && hasDigit;
}

bool Lexer::isBloodTypeLike(const std::string& text) const {
    if (text.size() < 2 || text.size() > 3) {
        return false;
    }

    const char last = text[text.size() - 1];
    if (last != '+' && last != '-') {
        return false;
    }

    const std::string prefix = text.substr(0, text.size() - 1);
    if (prefix == "A" || prefix == "B" || prefix == "O" || prefix == "AB") {
        return true;
    }

    return false;
}

bool Lexer::isValidDate(const std::string& text) const {
    if (text.size() != 10 || text[4] != '-' || text[7] != '-') {
        return false;
    }

    const int month = (text[5] - '0') * 10 + (text[6] - '0');
    const int day = (text[8] - '0') * 10 + (text[9] - '0');

    if (month < 1 || month > 12) {
        return false;
    }

    if (day < 1 || day > 31) {
        return false;
    }

    return true;
}

bool Lexer::isValidHour(const std::string& text) const {
    if (text.size() != 5 || text[2] != ':') {
        return false;
    }

    const int hour = (text[0] - '0') * 10 + (text[1] - '0');
    const int minute = (text[3] - '0') * 10 + (text[4] - '0');

    if (hour < 0 || hour > 23) {
        return false;
    }

    if (minute < 0 || minute > 59) {
        return false;
    }

    return true;
}

bool Lexer::isLikelyMisspelledReservedWord(const std::string& text, std::string& suggestion) const {
    if (text.empty()) {
        return false;
    }

    const std::string upperText = toUpperAscii(text);
    for (std::unordered_set<std::string>::const_iterator it = m_reservedLexemes.begin(); it != m_reservedLexemes.end(); ++it) {
        const std::string candidate = *it;
        const std::string upperCandidate = toUpperAscii(candidate);

        if (upperText == upperCandidate && text != candidate) {
            suggestion = candidate;
            return true;
        }

        if (isOneEditAway(upperText, upperCandidate)) {
            suggestion = candidate;
            return true;
        }
    }

    return false;
}

std::string Lexer::toUpperAscii(const std::string& text) {
    std::string out = text;
    for (std::size_t i = 0; i < out.size(); ++i) {
        if (out[i] >= 'a' && out[i] <= 'z') {
            out[i] = static_cast<char>(out[i] - ('a' - 'A'));
        }
    }
    return out;
}

bool Lexer::isOneEditAway(const std::string& a, const std::string& b) {
    const std::size_t lenA = a.size();
    const std::size_t lenB = b.size();

    if (lenA > lenB + 1 || lenB > lenA + 1) {
        return false;
    }

    std::size_t i = 0;
    std::size_t j = 0;
    int edits = 0;

    while (i < lenA && j < lenB) {
        if (a[i] == b[j]) {
            ++i;
            ++j;
            continue;
        }

        ++edits;
        if (edits > 1) {
            return false;
        }

        if (lenA == lenB) {
            ++i;
            ++j;
        } else if (lenA > lenB) {
            ++i;
        } else {
            ++j;
        }
    }

    if (i < lenA || j < lenB) {
        ++edits;
    }

    return edits == 1;
}
