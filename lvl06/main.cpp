#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

#include "../common/aoc.h"

using namespace std;

// the lazy way
void simulateDay(vector<int>& v, int normalFishTime, int const newFishTime)
{
    auto cnt = v.size();
    for (size_t i = 0; i < cnt; ++i)
    {
        if (v[i] == 0)
        {
            v[i] = normalFishTime;
            v.push_back(newFishTime);
        }
        else 
        {
            v[i]--;
        }
    }
}

template <typename T, size_t N>
void simulateDayEfficient(std::array<T, N>& arr, int const normalFishTime)
{
    auto forking = arr[0];
    for (size_t i = 0; i < (N - 1); ++i)
    {
        arr[i] = arr[i + 1];
    }

    arr[N - 1] = forking;
    arr[normalFishTime] += forking;
}

int main(){

    vector<int> fishtimer;

    aoc::readInput("input.txt", [&fishtimer](auto const& line) { fishtimer = aoc::splitString<int>(line, ','); });

    std::array<uint64_t, 9> timerCounter {};

    // initialize
    for (auto &&i : fishtimer)
    {
        timerCounter[i]++;
    }

    auto part1Days = 80;
    auto totalDays = 256;
    auto day = 0;

    // Part 1
    for (; day < part1Days; ++day)
    {
        simulateDayEfficient(timerCounter, 6);
    }

    cout << "fishes after " << totalDays << " days: " << 
        std::accumulate(timerCounter.begin(), timerCounter.end(), 0) << endl;

    // Part 2
    for (; day < totalDays; ++day)
    {
        simulateDayEfficient(timerCounter, 6);
    }

    cout << "fishes after " << totalDays << " days: " << 
        std::accumulate(timerCounter.begin(), timerCounter.end(), (uint64_t)0) << endl;

    return 0;
}