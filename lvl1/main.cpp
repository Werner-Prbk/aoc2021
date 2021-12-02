#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>

using namespace std;

vector<int> readInput(string const& fileName)
{
    auto v = vector<int>();
    auto in = std::ifstream(fileName);

    while (!in.eof())
    {
        int num = 0;
        in >> num;
        v.push_back(num);
    }

    return v;
}

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
    auto v = readInput("input.txt");

    // part 1
    cout << "Result PART1: " << getIncreasesWithSlidingWindow(v, 1) << endl;

    // part 2
    cout << "Result PART2: " << getIncreasesWithSlidingWindow(v, 3) << endl;

    return 0;
}