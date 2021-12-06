#include <functional>
#include <string>
#include <vector>
namespace aoc 
{
    void readInput(std::string const& fileName, std::function<void(const std::string&)> lineProcessor);
    
    std::vector<int> splitString(std::string const& str, char const sep);
}