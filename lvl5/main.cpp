#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <numeric>
#include <locale>
#include "../common/linereader.h"

using namespace std;

struct Coordinate {

    Coordinate(int xinit = 0, int yinit = 0) : x{xinit}, y{yinit}
    {}

    int x;
    int y;
};

class Line {
public:
    Line(Coordinate startinit = Coordinate(), Coordinate endinit = Coordinate()) 
        : start{startinit}, end{endinit}
        {}

    int GetMaxX() const {
        return max(start.x, end.x);
    }

    int GetMaxY() const {
        return max(start.y, end.y);
    }

    vector<Coordinate> GetPath(bool skipDiagonales = false) const {
        vector<Coordinate> path;

        auto direction = [](int const diff) {
            if (diff < 0) return -1;
            if (diff > 0) return 1;
            return 0;
        };

        auto incX = direction(end.x - start.x);
        auto incY = direction(end.y - start.y);

        if (skipDiagonales && (incX != 0) && (incY != 0))
        {
            return path;
        } 

        Coordinate tmp(start);

        while ((tmp.x != end.x) || (tmp.y != end.y))
        {
            path.push_back(tmp);
            tmp.x += incX;
            tmp.y += incY;
        }

        path.push_back(end); 
        return path;
    }

    Coordinate start;
    Coordinate end;
};

Line ParseLine(string const& l)
{  
    vector<int> nums;
    string tmp;
    for (auto &&c : l)
    {
        if (isdigit(c)) 
        {
            tmp.append({c});
        }
        else if (tmp.size() > 0)
        {
            nums.push_back(stoi(tmp));
            tmp.clear();
        }
    }

    if (tmp.size() > 0) {
        nums.push_back(stoi(tmp));
    }

    return Line(
        Coordinate(nums[0], nums[1]),
        Coordinate(nums[2], nums[3]));    
}

vector<vector<int>> BuildEmptyDiagram(vector<Line> const& lines)
{
    vector<vector<int>> diagram {0};
    
    int xmax = max_element(lines.begin(), lines.end(), [](auto const& lhs, auto const& rhs) {
            return lhs.GetMaxX() < rhs.GetMaxX();
        })->GetMaxX();

    int ymax = max_element(lines.begin(), lines.end(), [](auto const& lhs, auto const& rhs) {
            return lhs.GetMaxY() < rhs.GetMaxY();
        })->GetMaxY();

    // dont foget the interval is [0, max]
    for (int rows = 0; rows <= ymax; ++rows)
    {
        diagram.push_back(vector<int>(xmax + 1, 0));
    }
    return diagram;
}

int main() {
    vector<Line> lines;

    aoc::readInput("input.txt", [&lines](auto const& l) {
        lines.push_back(ParseLine(l));
    });


    // Part 1
    auto diag = BuildEmptyDiagram(lines);

    for (auto &&l : lines)
    {
        for (auto &&c : l.GetPath(true))
        {
            diag[c.y][c.x]++;
        }
    }
    
    int overlaps = 0;
    for (auto &&cols : diag)
    {
        for (auto &&elem : cols)
        {
            if (elem > 1)
            {
                overlaps++;
            }
        }
    }
    
    cout << "Overlapping lines (ignore diagonales): " << overlaps << endl;

    // Part 2
    diag = BuildEmptyDiagram(lines);

    for (auto &&l : lines)
    {
        for (auto &&c : l.GetPath(false))
        {
            diag[c.y][c.x]++;
        }
    }
    
    overlaps = 0;
    for (auto &&cols : diag)
    {
        for (auto &&elem : cols)
        {
            if (elem > 1)
            {
                overlaps++;
            }
        }
    }
    
    cout << "Overlapping lines (with diagonales): " << overlaps << endl;

    return 0;
}