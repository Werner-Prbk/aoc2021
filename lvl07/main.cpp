#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <limits>
#include "../common/aoc.h"

using namespace std;

// inefficient but working
int calcFuelPart2(int distance)
{
    int sum = 0;
    while (distance)
    {
        sum += distance;
        distance--;
    }

    return sum;
}

int main(){
    vector<int> crabPositions;
    aoc::readInput("input.txt", [&crabPositions](auto const& line){crabPositions = aoc::splitString<int>(line, ',');});

    auto from = min_element(crabPositions.begin(), crabPositions.end());
    auto to = max_element(crabPositions.begin(), crabPositions.end());

    auto minFuel = std::numeric_limits<int>::max();
    auto minFuelPart2 = minFuel;

    for (int i = *from; i <= *to; ++i)
    {
        int tmpFuel = 0;
        int tmpFuelPart2 = 0;
        for (auto &&p : crabPositions)
        {
            auto dist = abs(i - p);
            tmpFuel += dist;
            tmpFuelPart2 += calcFuelPart2(dist);
        }
        minFuel = min(tmpFuel, minFuel);
        minFuelPart2 = min(tmpFuelPart2, minFuelPart2);
    }

    cout << "The minimum total fuel is (Part 1): " << minFuel << endl;
    cout << "The minimum total fuel is (Part 2): " << minFuelPart2 << endl;
    return 0;
}