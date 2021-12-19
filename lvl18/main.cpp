#include <iostream>
#include <vector>
#include <stack>
#include <tuple>
#include <memory>
#include <sstream>
#define DEBUG
#include <cassert>
#include "../common/aoc.h"

using namespace std;
using namespace aoc;

struct element;
using elementPair = pair<shared_ptr<element>, shared_ptr<element>>;

struct element
{
    element(int num) : isReal{true}, number{num} {}
    element(shared_ptr<elementPair> p) : isReal{false}, pair{p} {}

    void set(int num) {
        isReal = true;
        number = num;
        pair = nullptr;
    }

    void set(shared_ptr<elementPair> p) {
        isReal = false;
        number = -1;
        pair = p;
    }

    void print(ostream& os) {
        if (isReal) {
            os << number;
        } else {
            os << "[";
            pair->first->print(os);
            os << ",";
            pair->second->print(os);
            os << "]";
        }
    }

    void update_nesting_level(int lvl = 0) {
        nestingLevel = lvl;
        if (!isReal) {
            pair->first->update_nesting_level(lvl+1);
            pair->second->update_nesting_level(lvl+1);
        }
    }

    bool isReal;
    shared_ptr<elementPair> pair{};
    int number{-1};
    int nestingLevel {0};
};

shared_ptr<element> parse_line(string const &line)
{
    stack<shared_ptr<element>> opened;
    shared_ptr<element> root = nullptr;

    for (size_t i = 0; i < line.size(); ++i)
    {
        if (line[i] == '[')
        {
            auto p = make_shared<element>(make_shared<elementPair>());
            assert(p->isReal == false);
            assert(p->pair->first == nullptr);
            assert(p->pair->second == nullptr);

            if (opened.size() > 0) {
                assert(opened.top()->pair->first == nullptr || opened.top()->pair->second == nullptr);

                if (opened.top()->pair->first == nullptr) {
                    opened.top()->pair->first = p;
                }
                else if (opened.top()->pair->second == nullptr) {
                    opened.top()->pair->second = p;
                }
            }
            else {
                root = p;
            }
            opened.push(p);
        }
        else if (line[i] == ']')
        {   
            opened.pop();
        }
        else if (line[i] == ',')
        {
            // no action needed
        }
        else
        {
            // numbers may have more than 1 digit!
            size_t idx = i;
            while (isdigit(line[idx])){idx++;};
            string tmp = line.substr(i, idx - i);
            i += (tmp.length() - 1);
            auto num = stoi(tmp);

            assert(opened.top()->pair->first == nullptr || opened.top()->pair->second == nullptr);

            if (opened.top()->pair->first == nullptr) {
                opened.top()->pair->first = make_shared<element>(num);
            }
            else if (opened.top()->pair->second == nullptr) {
                opened.top()->pair->second = make_shared<element>(num);
            }
        }
    }
    
    root->update_nesting_level();
    return root;
}

bool find_prev_number(shared_ptr<element> current, shared_ptr<element> const tofind, shared_ptr<element>& prev) {
    if (current == tofind) {
        return true;
    }
    else if (current->isReal) {
        prev = current;
        return false;
    }
    else if (find_prev_number(current->pair->first, tofind, prev)) {
        return prev != nullptr;
    }
    else if (find_prev_number(current->pair->second, tofind, prev)) {
        return prev != nullptr;
    }
    return false;
}

bool find_next_number(shared_ptr<element> current, shared_ptr<element> const tofind, shared_ptr<element>& next) { 
    if (current == tofind) {
        return true;
    }
    else if (current->isReal) {
        next = current;
        return false;
    }
    else if (find_next_number(current->pair->second, tofind, next)) {
        return next != nullptr;
    }
    else if (find_next_number(current->pair->first, tofind, next)) {
        return next != nullptr;
    }
    return false;
}

bool explode_numbers(shared_ptr<element> n, shared_ptr<element> root = nullptr) {
    if (root == nullptr) root = n; 

    if (n->isReal) {
        return false;
    }
    else if (n->nestingLevel < 4) {
        if (explode_numbers(n->pair->first, root)) {
            return true;
        } 
        return explode_numbers(n->pair->second, root);
    }
    else {
        // left value is added to previous
        shared_ptr<element> prev = nullptr;
        if (find_prev_number(root, n, prev)) {
            prev->number += n->pair->first->number;
        }
        // right value is added to next
        shared_ptr<element> next = nullptr;
        if (find_next_number(root, n, next)) {
            next->number += n->pair->second->number;
        }
        // the exploding pair is replaced with 0
        n->set(0);
        root->update_nesting_level();
        return true;
    }
}

bool split_numbers(shared_ptr<element> n) {
    if (n->isReal) {
        if (n->number >= 10) {
            auto p = make_shared<elementPair>();
            p->first = make_shared<element>(n->number / 2);
            p->second = make_shared<element>((n->number + 1) / 2);
            n->set(p);   
            n->update_nesting_level(n->nestingLevel);
            return true; 
        }
        return false;
    }
    else {
        if (split_numbers(n->pair->first)) {
            return true;
        }
        return split_numbers(n->pair->second);
    }
}

shared_ptr<element> reduce_numbers(shared_ptr<element> n) {
    bool repeat = true;   
    while (repeat) {
        repeat = false;
        if (explode_numbers(n)) repeat = true;
        else if (split_numbers(n)) repeat = true;
    }
    return n;
}

shared_ptr<element> concat_numbers(shared_ptr<element> lhs, shared_ptr<element> rhs) {
    auto root = make_shared<element>(make_shared<elementPair>());
    root->pair->first = lhs;
    root->pair->second = rhs;
    root->update_nesting_level();
    return root;
}

shared_ptr<element> add_numbers(shared_ptr<element> lhs, shared_ptr<element> rhs) {
    auto res = concat_numbers(lhs, rhs);
    return reduce_numbers(res);
}

int calc_magnitude(shared_ptr<element> n) {
    if (n->isReal) return n->number;
    return 3 * calc_magnitude(n->pair->first) + 2 * calc_magnitude(n->pair->second);    
}

vector<shared_ptr<element>> getNumbers(string const& filename) {
    vector<shared_ptr<element>> v;
    readInput(filename, [&v](auto const& line) { v.push_back(parse_line(line)); });
    return v;
}

// some testing
void test_parsing();
void test_concat_numbers();
void test_find_prev_next();
void test_explode();
void test_split();
void test_add();
void test_calc_magnitude();

int main()
{
    /*
    // some testing
    test_parsing();
    test_concat_numbers();
    test_find_prev_next();
    test_explode();
    test_split();
    test_add();
    test_calc_magnitude();
    */
    
    // Part 1
    auto numbers = getNumbers("input.txt");
    shared_ptr<element> res = numbers[0];
    for(size_t i = 1; i < numbers.size(); ++i) {
        res = add_numbers(res, numbers[i]);
    }
    cout << "Resulting magnitued is: " << calc_magnitude(res) << endl; 

    // Part 2
    int highest = 0;
    for (size_t i = 0; i < numbers.size(); ++i) {
        for (size_t j = 0; j < numbers.size(); ++j) {
            if (i == j) continue;
            
            // massive inefficient... but who cares!
            auto v = getNumbers("input.txt");
            auto mag = calc_magnitude(add_numbers(v[i], v[j]));
            highest = max(mag, highest);
       }
    }

    cout << "Highest magnitude (part 2): " << highest << endl;

    return 0;
}

void test_explode() {
    cout << "Explode Test 1" << endl;
    auto t = parse_line("[[[[[9,8],1],2],3],4]");
    assert(explode_numbers(t));
    stringstream ss;
    t->print(ss);
    assert(ss.str() == "[[[[0,9],2],3],4]");

    cout << "Explode Test 2" << endl;
    t = parse_line("[7,[6,[5,[4,[3,2]]]]]");
    assert(explode_numbers(t));
    ss = stringstream();
    t->print(ss);
    assert(ss.str() == "[7,[6,[5,[7,0]]]]");

    cout << "Explode Test 3" << endl;
    t = parse_line("[[6,[5,[4,[3,2]]]],1]");
    assert(explode_numbers(t));
    ss = stringstream();
    t->print(ss);
    assert(ss.str() == "[[6,[5,[7,0]]],3]");

    cout << "Explode Test 4" << endl;
    t = parse_line("[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]");
    assert(explode_numbers(t));
    ss = stringstream();
    t->print(ss);
    assert(ss.str() == "[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]");

    cout << "Explode Test 5" << endl;
    t = parse_line("[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]");
    assert(explode_numbers(t));
    ss = stringstream();
    t->print(ss);
    assert(ss.str() == "[[3,[2,[8,0]]],[9,[5,[7,0]]]]");
}

void test_split() {
    cout << "Split Test 1" << endl;
    auto t = parse_line("[[[[0,7],4],[15,[0,13]]],[1,1]]");
    assert(split_numbers(t));
    stringstream ss;
    t->print(ss);
    assert(ss.str() == "[[[[0,7],4],[[7,8],[0,13]]],[1,1]]");
    assert(split_numbers(t));
    ss = stringstream();
    t->print(ss);
    assert(ss.str() == "[[[[0,7],4],[[7,8],[0,[6,7]]]],[1,1]]");
    assert(split_numbers(t) == false);

}

void test_find_prev_next() {
    cout << "Find prev/next Test 1" << endl;
    auto t1 = parse_line("[1,2]");
    shared_ptr<element> found;
    assert(find_prev_number(t1, t1->pair->second, found) == true);
    assert(found == t1->pair->first);
    found = nullptr;
    assert(find_prev_number(t1, t1->pair->first, found) == false);
    found = nullptr;
    assert(find_next_number(t1, t1->pair->first, found) == true);
    assert(found == t1->pair->second);  
    found = nullptr;
    assert(find_next_number(t1, t1->pair->second, found) == false);

    cout << "Find prev/next Test 2" << endl;
    auto t2 = parse_line("[1,[2,3]]");
    found = nullptr;
    assert(find_prev_number(t2, t2->pair->second->pair->first, found) == true);
    assert(found == t2->pair->first);
    found = nullptr;
    assert(find_next_number(t2, t2->pair->second->pair->first, found) == true);
    assert(found == t2->pair->second->pair->second);  

    cout << "Find prev/next Test 3" << endl;
    auto t = parse_line("[7,[6,[5,[4,[3,2]]]]]");
    found = nullptr;
    assert(find_prev_number(t, t->pair->second->pair->first, found));
    assert(found == t->pair->first);
    found = nullptr;
    assert(find_prev_number(t, t->pair->second->pair->second->pair->second->pair->second->pair->first, found));
    assert(found->number == 4);
}

void test_concat_numbers() {
    auto term1 = parse_line("[1,2]");
    auto term2 = parse_line("[3,4]");
    auto term3 = parse_line("[[5,6],7]");

    cout << "Concat Test 1" << endl;
    auto r1 = concat_numbers(term1, term2);
    stringstream ss;
    r1->print(ss);
    assert("[[1,2],[3,4]]" == ss.str());

    cout << "Concat Test 2" << endl;
    auto r2 = concat_numbers(term1, term3);
    ss = stringstream();
    r2->print(ss);
    assert("[[1,2],[[5,6],7]]" == ss.str());

    cout << "Concat Test 3" << endl;
    auto r3 = concat_numbers(term1, term2);
    r3 = concat_numbers(r3, term3);
    ss = stringstream();
    r3->print(ss);
    assert("[[[1,2],[3,4]],[[5,6],7]]" == ss.str());
    assert(r3->pair->first->pair->first->nestingLevel == 2);
    assert(r3->pair->first->pair->second->nestingLevel == 2);
    assert(r3->pair->first->nestingLevel == 1);
    assert(r3->pair->second->pair->first->nestingLevel == 2);
    assert(r3->pair->second->pair->second->nestingLevel == 2);
    assert(r3->pair->second->nestingLevel == 1);
    assert(r3->nestingLevel == 0);

    cout << "Concat Test 4" << endl;
    auto r4 = concat_numbers(parse_line("[1,2]"), parse_line("[[3,4],5]"));
    ss = stringstream();
    r4->print(ss);
    assert("[[1,2],[[3,4],5]]" == ss.str());
}

void test_parsing()
{
    cout << "Parse Test 1" << endl;
    string t1 = "[1,2]";
    auto r1 = parse_line(t1);
    assert(r1->isReal == false);
    assert(r1->pair->first->isReal == true);
    assert(r1->pair->first->isReal == true);
    assert(r1->pair->first->number == 1);
    assert(r1->pair->second->number == 2);
    assert(r1->nestingLevel == 0);
    assert(r1->pair->first->nestingLevel == 1);
    assert(r1->pair->second->nestingLevel == 1);

    cout << "Parse Test 2" << endl;
    string t2 = "[1,[2,3]]";
    auto r2 = parse_line(t2);
    assert(r2->pair->first->isReal == true);
    assert(r2->pair->first->number == 1);
    assert(r2->pair->first->nestingLevel == 1);
    assert(r2->pair->second->isReal == false);
    assert(r2->pair->second->pair->first->number == 2);
    assert(r2->pair->second->pair->first->nestingLevel == 2);
    assert(r2->pair->second->pair->second->number == 3);
    assert(r2->pair->second->pair->second->nestingLevel == 2);

    cout << "Parse Test 3" << endl;
    string t3 = "[[10,20],30]";
    auto r3 = parse_line(t3);
    assert(r3->pair->first->isReal == false);
    assert(r3->pair->first->pair->first->number == 10);
    assert(r3->pair->first->pair->second->number == 20);
    assert(r3->pair->second->isReal == true);
    assert(r3->pair->second->number == 30);
    stringstream ss;
    r3->print(ss);
    assert(t3 == ss.str());
}

void test_add() {
    cout << "Add Test 1" << endl;
    auto lhs = parse_line("[[[[4,3],4],4],[7,[[8,4],9]]]");
    auto rhs = parse_line("[1,1]");
    auto res = add_numbers(lhs, rhs);
    stringstream ss;
    res->print(ss);
    assert(ss.str() == "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]");
}

void test_calc_magnitude() {
    cout << "Calc Magnitude Test 1" << endl;
    auto t = parse_line("[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]");
    assert(calc_magnitude(t) == 3488);
}