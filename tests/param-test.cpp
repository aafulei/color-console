#include "color.hpp"
#include <iostream>
#include <string>
#include <vector>

const std::vector<std::string> TABLE
{
    "black",
    "blue",
    "green",
    "aqua",
    "cyan",
    "red",
    "purple",
    "magenta",
    "yellow",
    "white",
    "gray",
    "light_blue",
    "light_green",
    "light_aqua",
    "light_cyan",
    "light_red",
    "light_purple",
    "light_magenta",
    "light_yellow",
    "bright_white"
};

void exhaustive_hue_param_test()
{
    for (auto & tag : TABLE)
        std::cout << hue::set(tag) << "hue::" + tag << hue::reset << std::endl;

    for (auto & tag : TABLE)
        std::cout << hue::set("on_" + tag) << "hue::on_" + tag << hue::reset << std::endl;

    for (auto & fg : TABLE)
        for (auto & bg : TABLE)
            std::cout << hue::set(fg + "_on_" + bg) << "hue::" + fg + "_on_" + bg << hue::reset << std::endl;
}

void exhaustive_dye_param_test()
{
    for (auto & tag : TABLE)
        std::cout << dye::colorize("dye::" + tag, tag) << std::endl;

    for (auto & tag : TABLE)
        std::cout << dye::colorize("dye::on_" + tag, "on_" + tag) << std::endl;

    for (auto & fg : TABLE)
        for (auto & bg : TABLE)
            std::cout << dye::colorize("dye::" + fg + "_on_" + bg, fg + "_on_" + bg) << std::endl;
}

int main()
{
    exhaustive_hue_param_test();
    exhaustive_dye_param_test();
    return 0;
}
