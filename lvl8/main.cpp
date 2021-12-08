#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "../common/aoc.h"

using namespace std;
using namespace aoc;

class SevenSegment {
    public:
    static SevenSegment Create(string const& input)
    {
        auto tmp = splitString<string>(input, '|');

        SevenSegment ss;
        ss._learningPattern = splitString<string>(tmp[0], ' ');
        ss._outputPattern = splitString<string>(tmp[1], ' ');
        return ss;
    }

    int CountUniqueDigits() const
    {
        int sum = 0;
        for (auto &&i : _outputPattern)
        {
            if ((i.size() == 2) ||  // 1
                (i.size() == 3) ||  // 7
                (i.size() == 4) ||  // 4
                (i.size() == 7))    // 8
                {
                    sum++;
                }
        }
        return sum;        
    }

    int GetNumber()
    {
        BuildPatternMap();

        int number = 0;
        int pos = 1000; // 4 digits

        for (auto &&p : _outputPattern)
        {
            sort(p.begin(), p.end());
            auto digit = find_if(_patternMap.begin(), _patternMap.end(), [p](auto const& s) { return p.compare(s) == 0; }) - _patternMap.begin();
            number += digit * pos;
            pos /= 10;
        }

        return number;
    } 

    private:
    SevenSegment(){}

    void BuildPatternMap()
    {
        if (_learningPattern.size() == 0) return;

        // take well known patterns first
        _patternMap[1] = TakeFromLearningPattern([](auto const& s) {return s.size() == 2;});
        _patternMap[4] = TakeFromLearningPattern([](auto const& s) {return s.size() == 4;});
        _patternMap[7] = TakeFromLearningPattern([](auto const& s) {return s.size() == 3;});
        _patternMap[8] = TakeFromLearningPattern([](auto const& s) {return s.size() == 7;});

         //only 9 contains 4
        _patternMap[9] = TakeFromLearningPattern([this](auto const& s) { return stringContainsAll(s, _patternMap[4]); });
        
        // only 3 contains 1 and has 5 segments
        _patternMap[3] = TakeFromLearningPattern([this](auto const& s) { return s.size() == 5 && stringContainsAll(s, _patternMap[1]); });
    
        // only 0 contains 1 and has 6 segments
        _patternMap[0] = TakeFromLearningPattern([this](auto const& s) { return s.size() == 6 && stringContainsAll(s, _patternMap[1]); });

        // now only 6 has 6 segments in the unresolved learning patterns
        _patternMap[6] = TakeFromLearningPattern([](auto const& s) {return s.size() == 6;});

        // only 5 contains 3/4 segments of 4
        _patternMap[5] = TakeFromLearningPattern([this](auto const& s) {return countMatchingCharsInString(s, _patternMap[4]) == 3;});

        // 2 is the last one in the list
        _patternMap[2] = TakeFromLearningPattern([](auto const& s) {return s.size() > 0;});

        if  (_learningPattern.size() > 0) throw "Something went wrong!";

        // allows easy comparison
        for (auto &&p : _patternMap)
        {
            sort(p.begin(), p.end());
        }
    }

    string TakeFromLearningPattern(function<bool(string const& s)> matcher)
    {
        auto matchIt = std::find_if(_learningPattern.begin(), _learningPattern.end(), [&matcher](auto const& s) {return matcher(s);});
        auto match = *matchIt;
        _learningPattern.erase(matchIt);
        return match;
    }

    vector<string> _learningPattern;
    vector<string> _outputPattern;
    array<string, 10> _patternMap;
};

int main() {
    vector<SevenSegment> sevenSegments;
    readInput("input.txt", [&sevenSegments](auto const& line) { sevenSegments.push_back(SevenSegment::Create(line)); });

    // Part 1
    int sum = 0;
    for (auto &&s : sevenSegments)
    {
        sum += s.CountUniqueDigits();
    }
    
    cout << "Unique digits: " << sum << endl;

    // Part 2
    uint sumNum = 0;
    for (auto &&s : sevenSegments)
    {
        sumNum += s.GetNumber();
    }

    cout << "Sum of all numbers: " << sumNum << endl;

    return 0;
}