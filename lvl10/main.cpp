#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <limits>
#include <algorithm>
#include "../common/aoc.h"

using namespace std;
using namespace aoc;

bool isOpening(char c) {
    return c == '(' || c == '[' || c == '{' || c == '<';
}

char getMatchingClosing(char c) {
    switch (c) {
        case '(': return ')';
        case '[': return ']';
        case '{': return '}';
        case '<': return '>';
        default: throw "Unexpected!";
    }
}

int getSyntaxErrorScore(char c) {
    switch (c) {
        case ')': return 3;
        case ']': return 57;
        case '}': return 1197;
        case '>': return 25137;
        default: throw "Unexpected!";
    }
}

int getAutoCompletionScore(char c) {
    switch (c) {
        case ')': return 1;
        case ']': return 2;
        case '}': return 3;
        case '>': return 4;
        default: throw "Unexpected!";
    }
}

struct syntaxError {
    size_t column {0};
    bool hasError {false};
    bool isComplete {false};
    vector<char> autoCompleted {};
};

syntaxError checkLineSyntax(string const& line) {
    stack<char> openingStack;

    for(size_t i = 0; i < line.size(); ++i) {
        if (isOpening(line[i])){
            openingStack.push(line[i]);
        }
        else {
            if (getMatchingClosing(openingStack.top()) != line[i]) {
                return syntaxError{i, true, false};
            }
            openingStack.pop();
        }
    }
    
    auto se = syntaxError{0, false, openingStack.size() == 0};

    // part 2
    while (openingStack.size() > 0) {
        se.autoCompleted.push_back(getMatchingClosing(openingStack.top()));
        openingStack.pop();
    }
    
    return se;
}

int main() {
    vector<string> lines;
    readInput("input.txt", [&lines](auto const& line){ lines.push_back(line); });

    uint syntaxErrorScore = 0;

    vector <syntaxError> syntaxErrors;

    for (auto &&l : lines) {
        auto se = checkLineSyntax(l);
        syntaxErrorScore += se.hasError ? getSyntaxErrorScore(l[se.column]) : 0;
        syntaxErrors.push_back(se);
    }

    cout << "Total syntax error score: " << syntaxErrorScore << endl;

    // part 2
    vector<uint64_t> autoCompletionScores;
    for (auto &&s : syntaxErrors) {
        if (!s.hasError) {
            uint64_t score = 0;
            for (auto &&c : s.autoCompleted) {
                score *= 5;
                score += getAutoCompletionScore(c);
            }
            autoCompletionScores.push_back(score);
        }
    }

    sort(autoCompletionScores.begin(), autoCompletionScores.end());

    cout << "Winner for auto completion is: " << autoCompletionScores[autoCompletionScores.size() / 2] << endl;

    return 0;
}