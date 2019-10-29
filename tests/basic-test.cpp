#include "color.hpp"
#include <iostream>
#include <string>

void hue_test()
{
    hue::set("red");
    std::cout << "Now it should be red." << std::endl;
    hue::reset();
    std::cout << "Now it should be back to normal." << std::endl;
    hue::set("on_green");
    std::cout << "Now it should be on green." << std::endl;
    hue::reset();
    std::cout << "Now it should be back to normal." << std::endl;
    std::cout << hue::black_on_bright_white
              << "Now it should be black on bright white." << std::endl
              << hue::reset
              << "Now it should be back to normal." << std::endl;
    hue::set("aqua");
    hue::set("on_light_yellow");
    std::cout << "Now it should be aqua on light yellow." << std::endl;
    hue::reset();
}

void dye_test()
{
    std::cout << dye::vanilla("Now it should be back to normal.") << std::endl;
}

template<typename T>
void dye_invert_test_helper(T && t)
{
    std::cout << t << std::endl;
    t.invert();
    std::cout << t << std::endl;
    t.invert();
    std::cout << t << std::endl;
    std::cout << t.reverse() << std::endl;
    std::cout << t.reverse().reverse() << std::endl;
}

void dye_invert_test()
{
    dye_invert_test_helper(dye::vanilla("vanilla"));
    dye_invert_test_helper(dye::blue("blue"));
    dye_invert_test_helper(dye::on_aqua("on_aqua"));
    dye_invert_test_helper(dye::light_red_on_bright_white("a lovely color"));
}

int main()
{
    hue_test();
    dye_test();
    dye_invert_test();
    return 0;
}
