#include <functional>
#include <string>

namespace aoc 
{
    bool readInput(std::string const& fileName, std::function<void(const std::string&)> lineProcessor);
}