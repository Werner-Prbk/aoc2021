#include <iostream>
#include <vector>
#include <tuple>
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

    bool is_within_rectangle(int x, int y) {
        if (x < xstart) return false;
        if (x > xend) return false;
        if (y < ystart) return false;
        if (y > yend) return false;
        return true;       
    }
};

void plot(rectangle area, rectangle target) {
    for (int y = area.yend; y >= area.ystart; y--) {
        for (int x = area.xstart; x <= area.xend; x++) {
            if (y == x && x == 0) cout << "S";
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


int main() {
    test_is_within_rectangle();

    pair<int, int> x;
    pair<int, int> y;
    readInput("test.txt", [&x,&y](auto const&l) {
        x = parseRange(l, "x");
        y = parseRange(l, "y");
    });

    plot(rectangle(rectangle(x,y), {{0,0}, {0,0}}), rectangle(x,y));

    return 0;
}