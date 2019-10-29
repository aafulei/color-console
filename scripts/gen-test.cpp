#include <cstdio>
#include <utility>
#include <vector>

const std::vector<const char *> TABLE
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

void gen_hue_test()
{
    for (auto & fg : TABLE)
        std::printf("std::cout << hue::%s << \"hue::%s\" << "
                    "hue::reset << std::endl;\n", fg, fg);

    for (auto & bg : TABLE)
        std::printf("std::cout << hue::on_%s << \"hue::on_%s\" << "
                    "hue::reset << std::endl;\n", bg, bg);

    for (auto & fg : TABLE)
        for (auto & bg : TABLE)
            std::printf("std::cout << hue::%s_on_%s << ""\"hue::%s_on_%s\" << "
                        "hue::reset << std::endl;\n", fg, bg, fg, bg);
}

void gen_dye_test()
{
    for (auto & fg : TABLE)
        std::printf("std::cout << dye::%s(\"dye::%s\") << "
                    "std::endl;\n", fg, fg);

    for (auto & bg : TABLE)
        std::printf("std::cout << dye::on_%s(\"dye::on_%s\") << "
                    "std::endl;\n", bg, bg);

    for (auto & fg : TABLE)
        for (auto & bg : TABLE)
            std::printf("std::cout << dye::%s_on_%s(\"dye::%s_on_%s\") << "
                        "std::endl;\n", fg, bg, fg, bg);
}

int main()
{
    gen_hue_test();
    gen_dye_test();
    return 0;
}
