#include <iostream>
#include <sstream>
#include <vector>
#include "../common/linereader.h"

using namespace std;


struct Command
{
    Command(string str)
    {
        auto ss = stringstream(str);
        
        string dirStr;
        ss >> dirStr;

        switch (dirStr[0])
        {
        case 'f': direction = Direction::forward; break;
        case 'u': direction = Direction::up; break;
        case 'd': direction = Direction::down; break;
        default:
            throw "unable to parse direcion string";
        }

        ss >> this->units;
    }

    enum Direction {forward, down, up};

    Direction direction;
    int units;
};

struct Position
{
    int x;
    int y;
};

void ProcessCommand(Command const& c, Position& p)
{
    switch (c.direction)
    {
        case Command::Direction::down: 
            p.y += c.units;
            break;
        case Command::Direction::up: 
            p.y -= c.units;
            break;
        case Command::Direction::forward:
            p.x += c.units;
            break;    
        default:
            throw "unexpected direction";
    }
}

int main()
{
    vector<Command> v;
    aoc::readInput("input.txt", [&v](auto const&line) { v.push_back(Command(line)); });

    Position pos {0,0};

    for (auto &&c : v)
    {
        ProcessCommand(c, pos);
    }

    cout << "Position: " << pos.x << "/" << pos.y << endl;
    cout << "Result is: " << pos.x * pos.y << endl;

}