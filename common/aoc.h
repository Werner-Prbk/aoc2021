#include <functional>
#include <string>
#include <vector>

namespace aoc 
{
    void readInput(std::string const& fileName, std::function<void(const std::string&)> lineProcessor);
    
    template <typename T>
    std::vector<T> splitString(std::string const& str, char const sep);

    size_t countMatchingCharsInString(std::string const& s, std::string const& chars);
    bool stringContainsAll(std::string const& s, std::string const& chars);
}