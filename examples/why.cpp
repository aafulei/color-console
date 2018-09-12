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
    cout << "When in doubt, wear " << dye::red("red") << "." << endl;

    auto green = dye::green("green");
    cout << "I saw "<< green << " trees, " << green << " bushes." << endl;

    cout << "Take the " << dye::blue("Blue") << " Line and then "
         << "catch Bus " << dye::yellow(42 + 7 % 8) << "."<< endl;

    cout << dye::purple(DoubleVector{3.14, 2.72}) << endl;

    cout << dye::light_red('A') + dye::light_blue('B')
          + dye::light_green('C') << endl;

    const char ca[] = "ca";
    string str = "str";
    cout << "[ " + dye::aqua(ca) + " | " + dye::aqua(str) + " ]" << endl;

    double a = 88.88;
    cout << dye::colorize(a, a >= 0 ? "red" : "green").invert() << endl;

    return 0;
}
