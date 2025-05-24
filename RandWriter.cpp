// Copyright 2025 Cameron Gagne

#include <set>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <numeric>
#include "RandWriter.hpp"

RandWriter::RandWriter(const std::string& text, size_t k)
    : order(k), sourceText(text) {

    if (text.length() < k)
        throw std::invalid_argument("Text must be at least length k.");

    // Build alphabet
    std::set<char> alphabetSet;
    for (char c : text)
        alphabetSet.insert(c);
    alphabet.assign(alphabetSet.begin(), alphabetSet.end());
    std::sort(alphabet.begin(), alphabet.end());

    // Make text circular
    std::string circularText = text + text.substr(0, k);

    // Populate map
    for (size_t i = 0; i < text.size(); ++i) {
        std::string kgram = circularText.substr(i, k);
        char nextChar = circularText[i + k];

        kgramMap[kgram][nextChar]++;
    }

    unsigned int seed = static_cast<unsigned int>(
        std::chrono::steady_clock::now().time_since_epoch().count());
    rng.seed(seed);
}

size_t RandWriter::orderK() const {
    return order;
}

int RandWriter::freq(const std::string& kgram) const {
    if (kgram.length() != order)
        throw std::invalid_argument("kgram must be of length k");

    auto it = kgramMap.find(kgram);
    if (it == kgramMap.end())
        return 0;

    // Use lambda in std::accumulate
    return std::accumulate(it->second.begin(), it->second.end(), 0,
        [](int sum, const std::pair<const char, int>& p) {
            return sum + p.second;
        });
}

int RandWriter::freq(const std::string& kgram, char c) const {
    if (kgram.length() != order)
        throw std::invalid_argument("kgram must be of length k");

    auto it = kgramMap.find(kgram);
    if (it == kgramMap.end())
        return 0;

    auto charIt = it->second.find(c);
    if (charIt == it->second.end())
        return 0;

    return charIt->second;
}

char RandWriter::kRand(const std::string& kgram) {
    if (kgram.length() != order)
        throw std::invalid_argument("kgram must be of length k");

    auto it = kgramMap.find(kgram);
    if (it == kgramMap.end())
        throw std::invalid_argument("kgram not found in model");

    const auto& charFreqMap = it->second;

    std::vector<char> chars;
    std::vector<int> weights;

    for (const auto& pair : charFreqMap) {
        chars.push_back(pair.first);
        weights.push_back(pair.second);
    }

    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    int index = dist(rng);

    return chars[index];
}

std::string RandWriter::generate(const std::string& kgram, size_t L) {
    if (kgram.length() != order)
        throw std::invalid_argument("kgram must be of length k");

    std::string result = kgram;

    while (result.length() < L) {
        std::string currentKgram = result.substr(result.length() - order, order);
        char nextChar = kRand(currentKgram);
        result += nextChar;
    }

    return result;
}

std::ostream& operator<<(std::ostream& os, const RandWriter& rw) {
    os << "Order: " << rw.order << '\n';
    os << "Alphabet: " << rw.alphabet << '\n';

    for (const auto& [kgram, followMap] : rw.kgramMap) {
        os << "kgram: " << kgram << '\n';
        for (const auto& [ch, count] : followMap) {
            os << "  " << ch << ": " << count << '\n';
        }
    }

    return os;
}
