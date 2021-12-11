#include <iostream>
#include <vector>
#include <algorithm>
#include "../common/aoc.h"

using namespace std;
using namespace aoc;

struct Octopus {
    Octopus(int energyLevel) : energy{energyLevel}, hasFlashed{false} 
    {};

    int energy;
    bool hasFlashed;
};

void iterateOverOctopus(vector<vector<Octopus>> &oe, function<void(vector<vector<Octopus>> &oe, int row, int col)> op) {
    for (size_t r = 0; r < oe.size(); ++r) {
        for (size_t c = 0; c < oe[0].size(); ++c) {
            op(oe, (int)r, (int)c);
        }
    }
}

void increaseEnergy(vector<vector<Octopus>>& oe) {
    iterateOverOctopus(oe, [](auto& o, int r, int c) { o[r][c].energy++; });
}

void increaseEnergyOfAdjacent(vector<vector<Octopus>> & oe, int r, int c) {
    int const width = (int)oe[0].size();
    int const height = (int)oe.size();

    if (r > 0) oe[r-1][c].energy++;
    if ((r + 1) < height) oe[r+1][c].energy++;
    if (c > 0) oe[r][c-1].energy++;
    if ((c + 1) < width) oe[r][c+1].energy++;    
    if (r > 0 && c > 0) oe[r-1][c-1].energy++;
    if (r > 0 && (c + 1) < width) oe[r-1][c+1].energy++;
    if ((r + 1) < height && c > 0)  oe[r+1][c-1].energy++;
    if ((r + 1) < height && (c + 1) < width) oe[r+1][c+1].energy++;
}

bool checkFlashCriteria(Octopus const& o) {
    return o.energy > 9 && !o.hasFlashed; 
}

void flashLights(vector<vector<Octopus>> &oe) {
    bool repeat = false;
    do {
        repeat = false;
        iterateOverOctopus(oe, [&repeat](auto& o, int r, int c) {
            if (checkFlashCriteria(o[r][c])) {
                o[r][c].hasFlashed = true;
                increaseEnergyOfAdjacent(o, r, c);
                repeat = true;
            }
        });
    } while (repeat);
}

void simulateStep(vector<vector<Octopus>> &oe) {
    iterateOverOctopus(oe, [](auto& o, int r, int c) { o[r][c].hasFlashed = false; });
    increaseEnergy(oe);
    flashLights(oe);
    iterateOverOctopus(oe, [](auto& o, int r, int c) { if (o[r][c].hasFlashed) o[r][c].energy = 0; });
}

int main() {
    vector<vector<Octopus>> octopusEnergy;
    readInput("input.txt", [&octopusEnergy](auto const& line){
        vector<Octopus> row;
        for (auto &&c : line) {
            if (isdigit(c)) {
                row.push_back({c - '0'});
            }
        }
        octopusEnergy.push_back(row);
    });

    int totalSteps = 100;
    int flashes = 0;
    bool part2Solved = false;

    for (int i = 1; (i <= totalSteps) || !part2Solved; ++i) {
        
        bool notAllFlashed = false;
        simulateStep(octopusEnergy);
        iterateOverOctopus(octopusEnergy, [&flashes, &notAllFlashed](auto& oe, int r, int c) { 
            if (oe[r][c].hasFlashed) flashes++;
            else notAllFlashed = true;
        });  

        if (i == totalSteps) {
            cout << "Total flashes afer " << totalSteps << " steps: " << flashes << endl;
        }

        if (!notAllFlashed){
            part2Solved = true;
            cout << "All flashed in interation " << i << endl;
        }
    }

    return 0;
}