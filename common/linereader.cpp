#include "linereader.h"
#include <fstream>

bool aoc::readInput(std::string const& fileName, std::function<void(const std::string&)> lineProcessor)
{
    auto in = std::ifstream(fileName);

    if (!in.is_open()) return false;

    std::string line;
    while (std::getline(in, line))
    {
        lineProcessor(line);
    }

    return true;
}