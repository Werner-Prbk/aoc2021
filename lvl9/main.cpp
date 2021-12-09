#include <iostream>
#include <vector>
#include <locale>
#include <algorithm>
#include <numeric>
#include "../common/aoc.h"

using namespace std;
using namespace aoc;

struct Point {
    Point(int v) : value{v}, isLowPoint{false}, basinId{-1} 
    {}

    int value;
    bool isLowPoint;
    int basinId;
};

vector<Point*> GetAdjacents(vector<vector<Point>>& map, int row, int col)
{
    vector<Point*> p;
    if (row > 0) p.push_back(&map[row - 1][col]);
    if ((size_t)(row + 1) < map.size()) p.push_back(&map[row + 1][col]);
    if (col > 0) p.push_back(&map[row][col-1]);
    if ((size_t)(col+1) < map[0].size()) p.push_back(&map[row][col+1]);
    return p;
}

void FindLowPoints(vector<vector<Point>>& map)
{
    for(size_t r = 0; r < map.size(); ++r){
        for (size_t c = 0; c < map[0].size(); ++c){
            auto adj = GetAdjacents(map, r, c);
            auto min = min_element(adj.begin(), adj.end(), [](auto const&lhs, auto const&rhs) {return lhs->value < rhs->value;});
            if (map[r][c].value < (*min)->value)
            {
                map[r][c].isLowPoint = true;
            }
        }
    }
}

int main(){
    vector<vector<Point>> map;
    readInput("input.txt", [&map](auto const& line){
        vector<Point> v;
        for (auto &&c : line)
        {
            if (isdigit(c))
            {
                v.push_back(c - '0');
            }
        }
        map.push_back(v);
    });

    FindLowPoints(map);

    // for part 2
    auto basinId = 0;

    auto riskLevelSum = 0;

    for(size_t r = 0; r < map.size(); ++r){
        for (size_t c = 0; c < map[0].size(); ++c){
            if (map[r][c].isLowPoint)
            {
                riskLevelSum += (1 + map[r][c].value);

                // for part 2
                map[r][c].basinId = basinId++;
            }
        }
    }
    
    cout << "Risk level sum: " << riskLevelSum << endl; // 550
    
    // Part 2
    vector<int> basinIdSizes (basinId, 1);
    bool found = false;
    do
    {
        found = false;
        for(size_t r = 0; r < map.size(); ++r){
            for (size_t c = 0; c < map[0].size(); ++c){
                if (map[r][c].basinId < 0 && 
                    map[r][c].value < 9) {
                    auto adj = GetAdjacents(map, r, c);
                    for (auto &&a : adj)
                    {
                        if (a->basinId >= 0)
                        {
                            map[r][c].basinId = a->basinId;
                            basinIdSizes[a->basinId]++;
                            found = true;
                            break;
                        }
                    }
                }
            }
        }
    } while (found);
    
    sort(basinIdSizes.begin(), basinIdSizes.end(), greater<int>());
    auto result = accumulate(basinIdSizes.begin(), basinIdSizes.begin() + 3, 1, multiplies<int>());
    cout << "Product of 3 lagest basin sizes: " << result << endl;   // 1100682

    return 0;
}