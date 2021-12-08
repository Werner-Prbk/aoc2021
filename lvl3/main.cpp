#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
// #include <ranges> // no compiler support for now -.-
#include "../common/aoc.h"

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

int BinaryToInt(vector<bool> bv)
{
    string s(bv.size(), '0');
    transform(bv.begin(), bv.end(), s.begin(), [](auto b)
    {
        return b ? '1' : '0' ;
    });

    return stoi(s, 0, 2);
}

bool GetDominantBitValue(vector<bool> line, bool winningValue = true)
{
    auto ones = static_cast<size_t>(count_if(line.begin(), line.end(), [](auto b) { return b; }));
    auto zeroes = line.size() - ones;
    
    if (ones == zeroes) 
    {
        return winningValue;
    }
    return (ones > zeroes);
}

vector<bool> FindMatchingWord(vector<vector<bool>> tt, bool matchingBit)
{
    for (size_t bitPos = 0; bitPos < tt.size(); ++bitPos)
    {
        auto dominantBit = GetDominantBitValue(tt[bitPos]) == matchingBit;

        vector<int> idxToDelete;        
        
        for (size_t i = 0; i < tt[bitPos].size(); ++i)
        {
            if (tt[bitPos][i] != dominantBit)
            {
                idxToDelete.push_back(i);
            }
        }
        
        // delete columns
        for_each(idxToDelete.rbegin(), idxToDelete.rend(),[&tt](auto i){
            for_each(tt.begin(), tt.end(), [i](auto & line) { line.erase(line.begin() + i); });
        });
    }

    vector<bool> res;
    for_each(tt.begin(), tt.end(),[&res](auto const& v){ res.push_back(v[0]); });
    return res;
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

    auto const wordWidth = transformedTable.size();
    vector<bool> gammaRateWord(wordWidth, false);
    vector<bool> epsilonWord(wordWidth, false);

    for (size_t bitPos = 0; bitPos < wordWidth; ++bitPos)
    {
        gammaRateWord[bitPos] = GetDominantBitValue(transformedTable[bitPos]);
        epsilonWord[bitPos] = !gammaRateWord[bitPos];
    }

    auto gammaRateValue = BinaryToInt(gammaRateWord);
    auto epsilonValue = BinaryToInt(epsilonWord);

    cout << "PART 1 ------------" << endl;
    cout << "GammaRate: " << gammaRateValue << endl;
    cout << "Epsilon:   " << epsilonValue << endl;
    cout << "Power consumption: " << gammaRateValue * epsilonValue << endl; 
    
    auto oxyWord = FindMatchingWord(transformedTable, true);
    auto co2Word = FindMatchingWord(transformedTable, false);
    auto oxyValue = BinaryToInt(oxyWord);
    auto co2Value = BinaryToInt(co2Word);

    cout << "PART 2 ------------" << endl;
    cout << "OxygenGeneratorRating: " << oxyValue << endl;
    cout << "Co2ScrubberRating::   " << co2Value << endl;
    cout << "Life support rating: " << oxyValue * co2Value << endl; 

    return 0;
}