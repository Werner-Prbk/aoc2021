#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <limits>
#include "../common/aoc.h"

using namespace std;
using namespace aoc;

class SyntaxChecker 
{
    public:
    bool Process(char c)
    {
        switch (c)
        {
            case '(': _counter['(']++; break;
            case ')': _counter['(']--; break;
            case '[': _counter['[']++; break;
            case ']': _counter['[']--; break;
            case '{': _counter['{']++; break;
            case '}': _counter['{']--; break;
            case '<': _counter['<']++; break;
            case '>': _counter['<']--; break;
            default: throw "Unexpected token";
        }

        _processCnt++;


    }

    private:



    int _processCnt {0};
    map<char, int> _counter {{'(', 0}, {'[', 0}, {'<', 0}, {'{', 0}};
};


bool isOpening(char c)
{
    return 
        c == '(' ||
        c == '[' ||
        c == '{' ||
        c == '<';
}

bool isClosing(char c)
{
    return 
        c == ')' ||
        c == ']' ||
        c == '}' ||
        c == '>';
}

char getMatchingClosing(char c)
{
    switch (c)
    {
        case '(': return ')';
        case '[': return ']';
        case '{': return '}';
        case '<': return '>';
        default: throw "Unexpected!";
    }
}

struct syntaxError {
    size_t column {0};
    bool hasError {false};
    bool isComplete {false};
};

syntaxError checkLineSyntax(string const& line)
{
    stack<char> openingStack;

    for(size_t i = 0; i < line.size(); ++i)
    {
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

    return syntaxError{0, false, openingStack.size() == 0};
}

int getScore(char c)
{
    switch (c)
    {
        case ')': return 3;
        case ']': return 57;
        case '}': return 1197;
        case '>': return 25137;
        default: throw "Unexpected!";
    }
}

int main(){
    vector<string> lines;
    readInput("input.txt", [&lines](auto const& line){ lines.push_back(line); });

    uint score = 0;

    for (auto &&l : lines)
    {
        auto se = checkLineSyntax(l);

        if (se.hasError)
        {
            //cout << l[se.column] << " on column " << se.column << endl;
            score += getScore(l[se.column]);
        }
    }

    cout << "Total score: " << score << endl;
    


    return 0;
}