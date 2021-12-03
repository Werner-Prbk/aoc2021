#include <functional>
#include <string>

namespace aoc 
{
    void readInput(std::string const& fileName, std::function<void(const std::string&)> lineProcessor);
}