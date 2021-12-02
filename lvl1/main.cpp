#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include "../common/linereader.h"

using namespace std;

int getIncreasesWithSlidingWindow(vector<int> const& v, int const windowLen)
{
    auto cnt = 0;

    for (auto it = v.begin(); it < v.end() - windowLen; ++it)
    {   
        if (accumulate(it, it + windowLen, 0) < 
            accumulate(it + 1, it + 1 + windowLen, 0))
        {
            cnt++;
        }
    }

    return cnt;
}

int main()
{
    vector<int> v{};
    aoc::readInput("input.txt", [&v](auto const& s){ v.push_back(stoi(s)); });

    // part 1
    cout << "Result PART1: " << getIncreasesWithSlidingWindow(v, 1) << endl;

    // part 2
    cout << "Result PART2: " << getIncreasesWithSlidingWindow(v, 3) << endl;

    return 0;
}