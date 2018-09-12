#include "../include/color.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct DoubleVector : private vector<double>
{
    using vector<double>::vector;
    friend ostream & operator<<(ostream &, const DoubleVector &);
};

ostream & operator<<(ostream & os, const DoubleVector & v)
{
    for (const auto & e : v)
        os << e << "  ";
    return os;
}

int main()
{
    auto a = dye::on_yellow(42);
    cout << a << endl;

    using vec = DoubleVector;

    auto b = dye::red(vec{1, 2, 3});
    b = b + dye::blue(vec{4, 5, 6});
    b += dye::green(vec{7, 8, 9});
    cout <<  b << endl;

    cout << dye::on_white(string("strings")) + " are " +
            dye::on_white("more") + string(" flexible") << endl;

    cout << dye::colorize("grape", "purple") << endl;

    cout << dye::invert(dye::red("red")) << endl;

    auto contrast = dye::vanilla("contrast");
    cout << contrast.invert() << endl;

    return 0;
}
