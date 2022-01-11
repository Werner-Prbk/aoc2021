#include <iostream>
#include <algorithm>
#include <vector>
#define DEBUG
#include <cassert>
#include "../common/aoc.h"

using namespace std;
using namespace aoc;

enum class pixel_t {
    light,
    dark
};

pixel_t char_to_pixel(char c) {
    if (c == '#') return pixel_t::light;
    if (c == '.') return pixel_t::dark;
    throw "Character is no pixel!";
};

char pixel_to_char(pixel_t p) {
    if (p == pixel_t::light) return '#';
    return '.';
};

class image {
    public:
    image() {}
    image(size_t w, size_t h, pixel_t init = pixel_t::dark) {
        for (size_t i = 0; i < h; ++i) {
            img.push_back(vector<pixel_t>(w, init));
        }
    }

    void add_row(string const& row) {
        vector<pixel_t> r;
        for (auto &&c : row) {
            r.push_back(char_to_pixel(c));
        }
        img.push_back(r);
    }

    void print(ostream& os) const {
        for (auto &&r : img) {
            for(auto &&c: r) {
                os << pixel_to_char(c);
            }
            os << endl;
        }
    }

    size_t get_width() const {return img[0].size();}
    size_t get_height() const {return img.size();}

    void expand_by(size_t const w, size_t const h, pixel_t const expander) {
        if (w > 0 || h > 0) {
            auto i = image(get_width() + w, get_height() + h, expander);
            for (size_t r = 0; r < get_height(); ++r) {
                for (size_t c = 0; c < get_width(); ++c) {
                    i.img[r + h/2][c + w/2] = img[r][c];
                }
            }
            img = i.img;
        }
    }

    vector<vector<pixel_t>> img;
};

template<size_t N>
array<array<pixel_t, N>, N> get_filter(image const& img, size_t const row_center, size_t const col_center) {
    array<array<pixel_t, N>, N> f;
    for (size_t r = 0; r < N; r++) {
        for (size_t c = 0; c < N; ++c) {
            f[r][c] = img.img[r-1 + row_center][c-1 + col_center];
        }
    }
    return f;
}

template<size_t N>
int filter_to_decimal(array<array<pixel_t, N>, N> const& filter) {
    string s = "";
    for(size_t r = 0; r < N; ++r) {
        for (size_t c = 0; c < N; ++c) {
            if (filter[r][c] == pixel_t::light) {
                s += "1";
            }
            else {
                s+= "0";
            }
        }
    }
    return stoi(s, 0, 2);
} 

image enhance_image(image& input, vector<pixel_t> const& enh, pixel_t expander) {
    image output(input.get_width() + 2, input.get_height() + 2);
    input.expand_by(4, 4, expander);
    
    for(size_t r = 1; r < (input.get_height() - 1); ++r) {
        for (size_t c = 1; c < (input.get_width() - 1); ++c) {
            auto f = get_filter<3>(input, r, c);
            auto num = filter_to_decimal<3>(f);
            output.img[r-1][c-1] = enh[num]; 
        }
    }
    
    return output;
}

size_t count_lit(image const& img) {
    size_t sum = 0;
    for (size_t r = 0; r < img.get_height(); ++r) {
        for (size_t c = 0; c < img.get_width(); ++c) {
            if (img.img[r][c] == pixel_t::light) sum++;
        }
    }
    return sum;
}

int main() {
    vector<pixel_t> enhancement;
    image input;

    readInput("input.txt", [&enhancement, &input](auto const&line) {
        if (enhancement.size() == 0) {
            for (auto &&c : line) {
                enhancement.push_back(char_to_pixel(c));
            }
        }
        else if (line.size() > 0) {
            input.add_row(line);
        }
    });

    int times = 50;

    input = enhance_image(input, enhancement, pixel_t::dark);
    for (int i = 1; i < times; ++i) {
        input = enhance_image(input, enhancement, enhancement[0]);
    }
    input.print(cout);
    cout << endl;
    cout << "Sum of lit pixels: " << count_lit(input) << endl;
    return 0;
}