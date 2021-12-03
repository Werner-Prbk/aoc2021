#include <iostream>
#include <vector>
#include <algorithm>
#include "../common/linereader.h"

using namespace std;

vector<bool> DecodeDiagnosticReport(string const& code)
{
    vector<bool> v;
    for (auto &&i : code)
    {
        if (i == '0' || i == '1')
        {
            v.push_back(i == '1');
        }
    }
    return v;
}

int main()
{
    vector<vector<bool>> transformedTable;
 
    aoc::readInput("input.txt", [&transformedTable](auto const& line) 
    { 
        auto dec = DecodeDiagnosticReport(line); 
        
        transformedTable.resize(dec.size());

        for (size_t i = 0; i < dec.size(); ++i)
        {
            transformedTable[i].push_back(dec[i]);
        }        
    });

    auto const bitWidth = transformedTable.size();

    string gammaRateBin(bitWidth, '0');
    string epsilonBin(bitWidth, '0');

    for (size_t bitPos = 0; bitPos < bitWidth; ++bitPos)
    {
        auto ones = static_cast<size_t>(count_if(transformedTable[bitPos].begin(), transformedTable[bitPos].end(), [](auto b) { return b; }));
        auto zeroes = transformedTable[0].size() - ones;

        if (ones > zeroes)
        {
            gammaRateBin[bitPos] = '1';
        }
        else if (ones < zeroes)
        {
            epsilonBin[bitPos] = '1';
        }
        else
        {
            throw "Undefined!";
        }
    }
    
    auto gammaRateValue = stoi(gammaRateBin, 0, 2);
    auto epsilonValue = stoi(epsilonBin, 0, 2);

    cout << "GammaRate: " << gammaRateBin << " --> " << gammaRateValue << endl;
    cout << "Epsilon:   " << epsilonBin << " --> " << epsilonValue << endl;

    cout << "Power consumption: " << gammaRateValue * epsilonValue << endl; 

    return 0;
}