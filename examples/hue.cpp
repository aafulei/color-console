#include "../include/color.hpp"
#include <iostream>

int main()
{
    std::cout << hue::light_red << hue::on_bright_white
              << "Hello, World" << hue::reset << std::endl;
    return 0;
}