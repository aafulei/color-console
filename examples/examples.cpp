#include "color.hpp"
#include <iostream>
#include <vector>

struct my_vector : private std::vector<double>
{
    using vector<double>::vector;
    friend std::ostream & operator<<(std::ostream &, const my_vector &);
};

std::ostream & operator<<(std::ostream & os, const my_vector & v)
{
    for (const auto & e : v)
        os << e << "  ";
    return os;
}

int main()
{
    // colorize anything -- strings, numbers...
    std::cout << "Ride on the " << dye::on_blue("blue") << " line and then catch bus " << dye::on_red(49) << "." << std::endl;
    // ... and even user-defined clasess
    std::cout << dye::yellow(my_vector{1, 2, 3, 4, 5}) << std::endl;


    // colorize an object and reuse it
    auto green = dye::green("green");
    std::cout << "I saw "<< green << " trees, " << green << " bushes." << std::endl;
    // concatenate objects (colors may differ)
    std::cout << dye::light_red('A') + dye::light_blue('B') + dye::light_green('C') << std::endl;

    // invert colors...
    auto zebra = dye::black_on_white("zebra");
    std::cout << zebra << std::endl;
    zebra.invert();
    std::cout << zebra << std::endl;
    zebra.invert();

    // ... with a chainable method that does not alter the object
    std::cout << zebra << "\n"
              << zebra.reverse() << "\n"
              << zebra.reverse().reverse() << std::endl;

    // possible to concatenate different string types
    const char ca[] = "C-style char array";
    std::string str = "std::string";
    std::cout << "[ " + dye::aqua(ca) + " | " + dye::aqua(str) + " ]" << std::endl;

    for (int i = -2; i != 3; ++i)
        std::cout << dye::colorize(i, i > 0 ? "green" : i == 0 ? "yellow" : "red") << " ";

    return 0;
}
