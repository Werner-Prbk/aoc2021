#include "linereader.h"
#include <fstream>

void aoc::readInput(std::string const& fileName, std::function<void(const std::string&)> lineProcessor)
{
    auto in = std::ifstream(fileName);

    if (!in.is_open()) throw "failed to open file!";

    std::string line;
    while (std::getline(in, line))
    {
        lineProcessor(line);
    }
}

std::vector<int> aoc::splitString(std::string const& str, char const sep)
{
    std::vector<int> v;
    size_t start = str.find_first_not_of(sep); // skip leading sep;
    size_t end = 0;
    
    do {
        end = str.find_first_of(sep, start);
        v.push_back(std::stoi(str.substr(start, end)));
        start = str.find_first_not_of(sep, end); // skip all sep
    } while (end != std::string::npos);

    return v;
}