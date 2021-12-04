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

    // returns true if this was the winning call
    bool MarkIf(int num)
    {
        bool wasWinningCall = false;
        for (auto &&row : _board) 
        {
            for (auto &&field : row) 
            {
                if (get<0>(field) == num) 
                {
                    get<1>(field) = true;
                    
                    if (!_hasWon && CheckIfAnyColOrRowIsFull())
                    {
                        _hasWon = true;
                        wasWinningCall = true;
                    }
                }
            }
        }

        _lastNum = num;
        return wasWinningCall;
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
        return _hasWon;
    }

private:
    using tField = std::tuple<int, bool>;     

    bool CheckIfAnyColOrRowIsFull() const
    {
        array<int, N> sumCols {0};
        array<int, N> sumRows {0};

        for (size_t row = 0; row < N; ++row)
        {
            for (size_t col = 0; col < N; ++col)
            {
                if (get<1>(_board[row][col]))
                {
                    sumCols[row]++;
                    sumRows[col]++;
                }
            }
        }

        return any_of(sumCols.begin(), sumCols.end(), [](auto const& s){ return s == N; }) ||
            any_of(sumRows.begin(), sumRows.end(), [](auto const& s){ return s == N; });
    }

    array<array<tField, N>, N> _board;
    int _lastNum {};

    size_t _fillRow;
    size_t _fillCol;
    bool _hasWon;
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

template<size_t N>
int PlayGamePart1(vector<int> const& numbers, vector<Board<N>> boards)
{
    for (auto n : numbers)
    {
        for (auto &b : boards)
        {
            if (b.MarkIf(n))
            {
                return b.GetScore();
            }
        }
    }
    throw "Unexpected: No winner found.";
}

template<size_t N>
int PlayGamePart2(vector<int> const& numbers, vector<Board<N>> boards)
{
    int notYetWon = boards.size();
    for (auto n : numbers)
    {
        for (auto &b : boards)
        {
            if (b.MarkIf(n)) 
            {
                if (--notYetWon == 0) return b.GetScore();
            }
        }
    }
    throw "Unexpected: Not all boards were winning.";
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

    // Part 1
    auto result = PlayGamePart1(numbers, boards);
    cout << "Winning score (part 1): " << result << endl;

    // Part 2
    auto result2 = PlayGamePart2(numbers, boards);
    cout << "Score (part 2): " << result2 << endl;

    return 0;
}