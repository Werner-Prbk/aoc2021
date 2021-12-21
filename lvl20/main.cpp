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
    image(size_t w, size_t h) {
        for (size_t i = 0; i < h; ++i) {
            _image.push_back(vector<pixel_t>(w, pixel_t::dark));
        }
    }

    void add_row(string const& row) {
        vector<pixel_t> r;
        for (auto &&c : row) {
            r.push_back(char_to_pixel(c));
        }
        _image.push_back(r);
    }

    void print(ostream& os) const {
        for (auto &&r : _image) {
            for(auto &&c: r) {
                os << pixel_to_char(c);
            }
            os << endl;
        }
    }

    size_t get_width() const {return _image[0].size();}
    size_t get_height() const {return _image.size();}

    void expand_by(size_t const w, size_t const h) {
        if (w > 0 || h > 0) {
            auto i = image(get_width() + w, get_height() + h);
            for (size_t r = 0; r < get_height(); ++r) {
                for (size_t c = 0; c < get_width(); ++c) {
                    i._image[r + h/2][c + w/2] = _image[r][c];
                }
            }
            _image = i._image;
        }
    }

    vector<vector<pixel_t>> _image;
};

template<size_t N>
array<array<pixel_t, N>, N> get_filter(image const& img, size_t const row, size_t const col) {
    array<array<pixel_t, N>, N> f;
    for (size_t r = 0; r < N; r++) {
        for (size_t c = 0; c < N; ++c) {
            f[r][c] = img._image[r + row][c + col];
        }
    }
    return f;
}

template<size_t N>
int filter_to_decimal(array<array<pixel_t, N>, N> const& filter) {
    string s = "0";
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

void test_filter_to_decimal() {
    
    array<array<pixel_t, 3>, 3> f0 {pixel_t::dark};
    array<array<pixel_t, 3>, 3> f511 {pixel_t::light};
    
    for (int r = 0; r < 3; ++r){
        for (int c = 0; c < 3; ++c) {
            f0[r][c] = pixel_t::dark;
            f511[r][c] = pixel_t::light;
        }
    }
    
    assert(filter_to_decimal(f0) == 0);    
    assert(filter_to_decimal(f511) == 511);
}

image enhance_image(image& input, vector<pixel_t> const& enh) {
    size_t const filter_size = 3;
    input.expand_by(2 * filter_size, 2 * filter_size);
    image output(input.get_width(), input.get_height());
/*
#######
#######
##...##
##...##
##...##
#######
#######
*/
    for(size_t r = 0; r <= (input.get_height() - filter_size); ++r) {
        for (size_t c = 0; c <= (input.get_width() - filter_size); ++c) {
            auto f = get_filter<filter_size>(input, r, c);
            auto num = filter_to_decimal<filter_size>(f);
            output._image[r][c] = enh[num]; 
        }
    }
    
    return output;
}

size_t count_lit(image const& img) {
    size_t sum = 0;
    for (size_t r = 0; r < img.get_height(); ++r) {
        for (size_t c = 0; c < img.get_width(); ++c) {
            if (img._image[r][c] == pixel_t::light) sum++;
        }
    }
    return sum;
}

int main() {
    vector<pixel_t> enhancement;
    image img;

    readInput("test.txt", [&enhancement, &img](auto const&line) {
        if (enhancement.size() == 0) {
            for (auto &&c : line) {
                enhancement.push_back(char_to_pixel(c));
            }
        }
        else if (line.size() > 0) {
            img.add_row(line);
        }
    });

    img = enhance_image(img, enhancement);
    img = enhance_image(img, enhancement);
    img.print(cout);
    cout << endl;
    cout << "Sum of lit pixels: " << count_lit(img) << endl;
    return 0;
}