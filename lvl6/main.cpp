#include <iostream>
#include <vector>
#include "../common/linereader.h"

using namespace std;


void simulateDay(vector<int>& v, int normalFishTime, int newFishTime)
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

int main(){

    vector<int> fishtimer;

    aoc::readInput("input.txt", [&fishtimer](auto const& line) { fishtimer = aoc::splitString(line, ','); });

    auto totalDays = 80;
    for (int d = 0; d < totalDays; ++d)
    {
        simulateDay(fishtimer, 6, 8);
    }

    cout << "fishes after " << totalDays << " days: " << fishtimer.size() << endl;

    return 0;
}