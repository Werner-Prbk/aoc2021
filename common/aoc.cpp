#include "aoc.h"
#include <fstream>
#include <functional>
#include <algorithm>

namespace aoc {
    template<typename T>
    std::vector<T> splitStringGeneric(std::string const& str, char const sep, std::function<T(std::string const& in)> converter)
    {
        std::vector<T> v;
        size_t start = str.find_first_not_of(sep); // skip leading sep;
        size_t end = 0;
        
        do {
            end = str.find_first_of(sep, start);
            v.push_back(converter(str.substr(start, end - start)));
            start = str.find_first_not_of(sep, end); // skip all sep
        } while ((end != std::string::npos) && (start != std::string::npos));

        return v;
    }
}

size_t aoc::countMatchingCharsInString(std::string const& s, std::string const& chars)
{
    size_t cnt = 0;
    for (auto &&c : chars)
    {
        if (std::find(s.begin(), s.end(), c) != s.end()) cnt++;
    }
    return cnt;
}

bool aoc::stringContainsAll(std::string const& s, std::string const& chars)
{
    return countMatchingCharsInString(s, chars) == chars.size();
}

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

template<>
std::vector<std::string> aoc::splitString(std::string const& str, char const sep)
{
    return splitStringGeneric<std::string>(str, sep, [](auto const& s){return s;});
}

template<>
std::vector<int> aoc::splitString(std::string const& str, char const sep)
{
    return splitStringGeneric<int>(str, sep, [](auto const& s) {return std::stoi(s);});
}