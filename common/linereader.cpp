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