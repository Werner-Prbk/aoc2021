#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <tuple>
#include "../common/aoc.h"

using namespace std;
using namespace aoc;

template<typename T1, typename T2>
void insertOrIncrement(map<T1, T2>& cc, T1 c, T2 inc){
    if (cc.find(c) == cc.end()) {
        cc.insert({c,inc});
    }
    else {
        cc[c] += inc;
    }
}

template<typename T>
void insertOrIncrement(vector<pair<string, T>>& pairs, string const& s, T cnt) {
    auto f = find_if(pairs.begin(), pairs.end(), [&s](auto const & cmp) { return s == cmp.first;});
    if (f == pairs.end()) {
        pairs.push_back({s, cnt});
    }
    else {
        f->second += cnt;
    }
}

template<typename T>
void pairInsertEfficient(vector<pair<string, T>>& pairs, map<string,string> const& rules, map<char, T>& charCnt) {
    vector<pair<string, T>> addLater;

    for (size_t i = 0; i < pairs.size(); ++i) {
        if ((pairs[i].second > 0) && 
            (rules.find(pairs[i].first) != rules.end())) {
            
            auto newChar = rules.at(pairs[i].first)[0];    
            auto times = pairs[i].second;
            pairs[i].second = 0;

            string n1 = pairs[i].first.substr(0, 1) + newChar;
            string n2 = newChar + pairs[i].first.substr(1, 1);

            addLater.push_back({n1, times});
            addLater.push_back({n2, times});

            insertOrIncrement(charCnt, newChar, times);
        }
    }
    
    for (auto &&i : addLater) {
        insertOrIncrement(pairs, i.first, i.second);
    } 
}

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

template<typename T>
map<char, T> countCharacters(string const& s) {
    map<char, T> charCount;
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

    // part 2
    auto patternCpy = pattern;

    int iter = 10;
    for(int i = 0; i < iter; ++i) {
        pattern = pairInsert(pattern, rules);
    }

    auto charCount = countCharacters<uint64_t>(pattern);
    auto leastCommon = min_element(charCount.begin(), charCount.end(), [](auto const& lhs, auto const& rhs) { return lhs.second < rhs.second; });
    auto mostCommon = max_element(charCount.begin(), charCount.end(), [](auto const& lhs, auto const& rhs) { return lhs.second < rhs.second; });

    cout << "Least common " << leastCommon->first << " - " << leastCommon->second << endl;
    cout << "Most common " << mostCommon->first << " - " << mostCommon->second << endl;
    cout << "Result (part 1): " << mostCommon->second - leastCommon->second << endl << endl;

    // Part 2 
    pattern = patternCpy;
    vector<pair<string, uint64_t>> pairs;
    charCount.clear();
    for (size_t i = 0; i < (pattern.size() - 1); ++i) {
        auto pair = pattern.substr(i, 2);
        insertOrIncrement<uint64_t>(pairs, pair, 1);
    }   

    charCount = countCharacters<uint64_t>(pattern);
    
    iter = 40;
    for(int i = 0; i < iter; ++i) {
        pairInsertEfficient<uint64_t>(pairs, rules, charCount);
    }
   
    leastCommon = min_element(charCount.begin(), charCount.end(), [](auto const& lhs, auto const& rhs) { return lhs.second < rhs.second; });
    mostCommon = max_element(charCount.begin(), charCount.end(), [](auto const& lhs, auto const& rhs) { return lhs.second < rhs.second; });

    cout << "Least common " << leastCommon->first << " - " << leastCommon->second << endl;
    cout << "Most common " << mostCommon->first << " - " << mostCommon->second << endl;
    cout << "Result (part 2): " << mostCommon->second - leastCommon->second << endl;

    return 0;
}