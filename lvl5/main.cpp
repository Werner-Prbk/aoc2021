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

struct Line {
    Line(Coordinate startinit = Coordinate(), Coordinate endinit = Coordinate()) 
        : start{startinit}, end{endinit}
        {}

    int GetMaxX() const {
        return max(start.x, end.x);
    }

    int GetMaxY() const {
        return max(start.y, end.y);
    }

    vector<Coordinate> GetPath() const {
        vector<Coordinate> path;

        if (end.x == start.x)
        {
            auto const from = min(start.y, end.y);
            auto const to = max(start.y, end.y);
            
            for (int i = from; i <= to; ++i)
            {
                path.push_back({start.x, i});
            }
        }
        else if (end.y == start.y)
        {
            auto const from = min(start.x, end.x);
            auto const to = max(start.x, end.x);
            
            for (int i = from; i <= to; ++i)
            {
                path.push_back({i, start.y});
            }
        }
        else 
        {
            auto const dx = end.x - start.x;
            auto const dy = end.y - start.y;

            auto incX = dx > 0 ? 1 : -1;
            auto incY = dy > 0 ? 1 : -1;

            Coordinate tmp(start);

            while (tmp.x != end.x)
            {
                path.push_back(tmp);
                tmp.x += incX;
                tmp.y += incY;
            }

            path.push_back(end);
        }

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

    auto diag = BuildEmptyDiagram(lines);

    for (auto &&l : lines)
    {
        for (auto &&c : l.GetPath())
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
    
    cout << "Overlapping lines: " << overlaps << endl;

    return 0;
}