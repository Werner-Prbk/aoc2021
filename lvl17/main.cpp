#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#define DEBUG
#include <cassert>
#include "../common/aoc.h"

using namespace std;
using namespace aoc;

pair<int, int> parseRange(string const& str, string const& dim) {
    auto start = str.find(dim);
    auto end = str.find(",", start);
    auto part = str.substr(start+2, end-start);
    auto fromto = splitString<int>(part, '.');
    return {fromto[0], fromto[1]};
}

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

    int xstart;
    int xend;
    int ystart;
    int yend;

    bool is_within_rectangle(pair<int,int> pos) {
        return is_within_rectangle(pos.first, pos.second);
    }

    bool is_within_rectangle(int x, int y) {
        if (x < xstart) return false;
        if (x > xend) return false;
        if (y < ystart) return false;
        if (y > yend) return false;
        return true;       
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

void test_is_within_rectangle() {
    auto t1 = rectangle({10, 20}, {10, 20});
    assert(t1.is_within_rectangle(10, 10));
    assert(t1.is_within_rectangle(20, 20));
    assert(!t1.is_within_rectangle(21, 20));

    t1 = rectangle({-10, -20}, {10, 20});
    assert(t1.is_within_rectangle(-10, 10));
    assert(t1.is_within_rectangle(-20, 20));
    assert(!t1.is_within_rectangle(-20, 21));

    t1 = rectangle({-10, -20}, {-10, -20});
    assert(t1.is_within_rectangle(-10, -10));
    assert(t1.is_within_rectangle(-20, -20));
    assert(!t1.is_within_rectangle(-20, -1));

    t1 = rectangle({10, 20}, {-10, -20});
    assert(t1.is_within_rectangle(10, -10));
    assert(t1.is_within_rectangle(20, -20));
    assert(!t1.is_within_rectangle(1, -20));

    t1 = rectangle({-10, 10}, {-10, 10});
    assert(t1.is_within_rectangle(0, 0));
    assert(t1.is_within_rectangle(0, 0));
    assert(!t1.is_within_rectangle(11, 0));
    assert(!t1.is_within_rectangle(0, 11));
    assert(!t1.is_within_rectangle(0, -11));
    assert(!t1.is_within_rectangle(-11, 0));
    assert(!t1.is_within_rectangle(-11, -11));
    assert(!t1.is_within_rectangle(11, 11));
}

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

        //cout << "New velo: " << velocity.first << ", " << velocity.second << endl;
        //cout << "New posi:" << latestPos.first << ", " << latestPos.second << endl; 

        positions.push_back(latestPos);

        return latestPos;
    }

    rectangle get_area() {
        pair<int,int> xrange {0,0};
        pair<int,int> yrange {0,0};
        for (auto &&p : positions)
        {
            xrange.first = min(p.first, xrange.first);
            xrange.second = max(p.first, xrange.second);

            yrange.first = min(p.second, yrange.first);
            yrange.second = max(p.second, yrange.second);
        }
        return rectangle(xrange, yrange);
    }
};

int main() {
    test_is_within_rectangle();

    pair<int, int> x;
    pair<int, int> y;
    readInput("test.txt", [&x,&y](auto const&l) {
        x = parseRange(l, "x");
        y = parseRange(l, "y");
    });

//    auto target = rectangle(x,y);
//    probe p{{7,2}};
//    while (!target.is_within_rectangle(p.step()));
//    plot(rectangle(target, p.get_area()), target, p.positions);



    return 0;
}