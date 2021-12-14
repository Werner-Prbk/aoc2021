#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include "../common/aoc.h"

using namespace std;
using namespace aoc;

string pairInsert(string const& pattern, map<string,string> const& rules) {
    string res;
    for (size_t i = 0; i < (pattern.size() - 1); ++i){
        res += pattern[i];
        string p = pattern.substr(i, 2);
        if (rules.find(p) != rules.end()) {
            res += rules.at(p);
        }
    }
    res += *pattern.rbegin();
    return res;
}

map<char, int> countCharacters(string const& s) {
    map<char, int> charCount;
    for (auto &&c : s) {
        if (charCount.find(c) != charCount.end()) {
            charCount[c]++;
        }
        else {
            charCount.insert({c, 1});
        }
    }
    return charCount;
}

int main() {
    string pattern = "";
    map<string, string> rules;

    readInput("input.txt", [&pattern, &rules](auto const&line) {
        if (pattern == "") {
            pattern = line;
        }
        else if (line.size() > 0){
            auto part = splitString<string>(line, ' ');
            rules.insert({part[0], part[2]});
        }
    });

    int iter = 10;
    for(int i = 0; i < iter; ++i) {
        pattern = pairInsert(pattern, rules);
    }

    auto charCount = countCharacters(pattern);
    auto leastCommon = min_element(charCount.begin(), charCount.end(), [](auto const& lhs, auto const& rhs) { return lhs.second < rhs.second; });
    auto mostCommon = max_element(charCount.begin(), charCount.end(), [](auto const& lhs, auto const& rhs) { return lhs.second < rhs.second; });

    cout << "Result (part 1): " << mostCommon->second - leastCommon->second << endl;

    return 0;
}