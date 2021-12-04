#include <iostream>
#include <algorithm>
#include <vector>
#include "../common/linereader.h"

using namespace std;

template<std::size_t N> 
class Board
{
public:
    bool IsFilled() const
    {
        return _fillRow == N && _fillCol == 0;
    }

    void Fill(int num)
    {
        SetAt(_fillRow, _fillCol++, num);

        if (_fillCol >= N)
        {
            _fillCol = 0;
            _fillRow++;
        }
    }

    int GetAt(size_t row, size_t col) const 
    {
        return get<0>(_board[row][col]);
    }
    
    void SetAt(size_t row, size_t col, int num)
    {
        get<0>(_board[row][col]) = num;
    }

    void MarkIf(int num)
    {
        for (auto &&row : _board) 
        {
            for (auto &&field : row) 
            {
                if (get<0>(field) == num) 
                {
                    get<1>(field) = true;
                }
            }
        }

        _lastNum = num;
    }
    
    int GetScore() const {
        int score = 0;
        for (auto &&row : _board) 
        {
            for (auto &&field : row) 
            {
                score += get<1>(field) ? 0 : get<0>(field);     
            }
        }
        
        return score * _lastNum;
    }

    bool HasWon() const
    {
        return any_of(_board.begin(), _board.end(), [](auto const& row) 
        {
            return all_of(row.begin(), row.end(), [](auto const& field) 
            { 
                return get<1>(field);
            });
        });
    }

private:
    using tField = std::tuple<int, bool>;     

    array<array<tField, N>, N> _board;
    int _lastNum {};

    size_t _fillRow;
    size_t _fillCol;
};

vector<int> Split(string const& s, char const sep)
{
    vector<int> v;
    size_t start = s.find_first_not_of(sep); // skip leading sep;
    size_t end = 0;
    
    do {
        end = s.find_first_of(sep, start);
        v.push_back(stoi(s.substr(start, end)));
        start = s.find_first_not_of(sep, end); // skip all sep
    } while (end != string::npos);

    return v;
}

int main() 
{
    vector<int> numbers;
    vector<Board<5>> boards;

    bool isFirstLine = true;

    aoc::readInput("input.txt", [&isFirstLine, &numbers, &boards](auto const& line)
    {        
        if (isFirstLine) 
        {
            isFirstLine = false;
            numbers = Split(line, ',');
            return;
        }

        if (line.size() == 0)
        {
            if (boards.size() > 0 && !boards.rbegin()->IsFilled()) 
            {
                throw "board was not filled correclty!";
            }

            boards.push_back(Board<5>());
        }
        else
        {
            auto fields = Split(line, ' ');
            for (auto &&i : fields) 
            { 
                boards.rbegin()->Fill(i); 
            };
        }
    });

    bool winnerFound = false;
    for (auto n : numbers)
    {
        for (auto &b : boards)
        {
            b.MarkIf(n);
            if (b.HasWon())
            {
                cout << "Winning score: " << b.GetScore() << endl;
                winnerFound = true;
                break;
            }
        }

        if (winnerFound) break;
    }

    return 0;
}