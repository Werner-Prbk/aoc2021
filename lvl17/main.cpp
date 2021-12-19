#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <set>
#define DEBUG
#include <cassert>
#include "../common/aoc.h"

using namespace std;
using namespace aoc;

struct rectangle {
    rectangle(rectangle r1, rectangle r2) {
        xstart = min(r1.xstart, r2.xstart);
        xend = max(r1.xend, r2.xend);
        ystart = min(r1.ystart, r2.ystart);
        yend = max(r1.yend, r2.yend);
    }

    rectangle(pair<int, int> x, pair<int, int> y) {
        xstart = min(x.first, x.second);        
        xend = max(x.first, x.second); 
        ystart = min(y.first, y.second);       
        yend = max(y.first, y.second);       
    }

    bool is_within_rectangle(pair<int,int> pos) const {
        return is_within_rectangle(pos.first, pos.second);
    }

    bool is_within_rectangle(int x, int y) const {
        if (x < xstart) return false;
        if (x > xend) return false;
        if (y < ystart) return false;
        if (y > yend) return false;
        return true;       
    }

    int xstart;
    int xend;
    int ystart;
    int yend;
};

struct probe {
    pair<int,int> velocity;
    probe (pair<int,int> velo) : velocity{velo} {} 
    vector<pair<int,int>> positions {{0,0}};
    pair<int,int> step() {
        auto latestPos = *positions.rbegin();
        
        if (velocity.first < 0) {
            latestPos.first += velocity.first;
            velocity.first += 1;
        } else if (velocity.first > 0) {
            latestPos.first += velocity.first;
            velocity.first -= 1;
        }

        latestPos.second += velocity.second;
        velocity.second -= 1;

        positions.push_back(latestPos);
        return latestPos;
    }

    rectangle get_area() {
        pair<int,int> xrange {0,0};
        pair<int,int> yrange {0,0};
        for (auto &&p : positions) {
            xrange.first = min(p.first, xrange.first);
            xrange.second = max(p.first, xrange.second);
            yrange.first = min(p.second, yrange.first);
            yrange.second = max(p.second, yrange.second);
        }
        return rectangle(xrange, yrange);
    }
};

void plot(rectangle area, rectangle target, vector<pair<int,int>> positions) {
    for (int y = area.yend; y >= area.ystart; y--) {
        for (int x = area.xstart; x <= area.xend; x++) {
            if (y == x && x == 0) cout << "S";
            else if (find(positions.begin(), positions.end(), pair<int,int>{x,y}) != positions.end()) {
                cout << "#";
            }
            else if (target.is_within_rectangle(x,y)) cout << "T";
            else cout << ".";
        }
        cout << endl;
    }
}

pair<int, int> parse_range(string const& str, string const& dim) {
    auto start = str.find(dim);
    auto end = str.find(",", start);
    auto part = str.substr(start+2, end-start);
    auto fromto = splitString<int>(part, '.');
    return {fromto[0], fromto[1]};
}

bool is_overshooted(rectangle const& t, pair<int,int> pos) {
    assert(t.xstart >= 0);  // assumption, that target is on the right hand side!
    if (pos.second < t.ystart) return true;
    if (pos.first > t.xend) return true;
    return false;
}

int find_highest_position(rectangle const& t, set<pair<int,int>>& distinctVelocities) {
    int yMax = 0;

    for (int vx = 1; vx <= t.xend; vx++) {
        for (int vy = -abs(t.ystart); vy <= abs(t.ystart); ++vy) {
            pair<int,int> pos;
            pair<int,int> currentVelo {vx,vy};
            probe p{currentVelo};
            do {
                pos = p.step();
                if (t.is_within_rectangle(pos)){
                    distinctVelocities.insert(currentVelo);
                    auto m = max_element(p.positions.begin(), p.positions.end(), [](auto const& lhs, auto const& rhs) { return lhs.second < rhs.second;});
                    yMax = max(yMax, m->second);
                }
            } while(!is_overshooted(t, pos));
        }
    }
    return yMax;
}

int main() {
    pair<int, int> x;
    pair<int, int> y;
    readInput("input.txt", [&x,&y](auto const&l) {
        x = parse_range(l, "x");
        y = parse_range(l, "y");
    });

    auto target = rectangle(x,y);
    set<pair<int,int>> distinctVelo;
    auto result = find_highest_position(target, distinctVelo);
    cout << "Highest position (part 1): " << result << endl;
    cout << "Distinct velocities (part 2): " << distinctVelo.size() << endl;

    return 0;
}