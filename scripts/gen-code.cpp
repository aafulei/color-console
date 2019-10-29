#include <cstdio>
#include <utility>
#include <vector>

const std::vector<std::pair<const char *, int>> TABLE
{
    {"black",            0},
    {"blue",             1},
    {"green",            2},
    {"aqua",             3},
    {"cyan",             3},
    {"red",              4},
    {"purple",           5},
    {"magenta",          5},
    {"yellow",           6},
    {"white",            7},
    {"gray",             8},
    {"light_blue",       9},
    {"light_green",     10},
    {"light_aqua",      11},
    {"light_cyan",      11},
    {"light_red",       12},
    {"light_purple",    13},
    {"light_magenta",   13},
    {"light_yellow",    14},
    {"bright_white",    15}
};

void gen_hue()
{
    for (auto & fg : TABLE)
        std::printf("inline std::ostream & %s(std::ostream & os) "
                    "{ return os << set_foreground(%d); }\n",
                    fg.first, fg.second);

    for (auto & bg : TABLE)
        std::printf("inline std::ostream & on_%s(std::ostream & os) "
                    "{ return os << set_background(%d); }\n",
                    bg.first, bg.second);

    for (auto & fg : TABLE)
        for (auto & bg : TABLE)
            std::printf("inline std::ostream & %s_on_%s(std::ostream & os) "
                        "{ return os << set(%d, %d); }\n",
                        fg.first, bg.first, fg.second, bg.second);
}

void gen_dye()
{
    for (auto & fg : TABLE)
        std::printf("template<typename T> L<T> %s(T t) "
                    "{ return L<T> { E<T>(t, %d, -1) }; }\n",
                    fg.first, fg.second);
    for (auto & bg : TABLE)
        std::printf("template<typename T> L<T> on_%s(T t) "
                    "{ return L<T> { E<T>(t, -1, %d) }; }\n",
                    bg.first, bg.second);
    for (auto & fg : TABLE)
        for (auto & bg : TABLE)
            std::printf("template<typename T> "
                        "L<T> %s_on_%s(T t) "
                        "{ return L<T> { E<T>(t, %d, %d) }; }\n",
                        fg.first, bg.first, fg.second, bg.second);
}

int main()
{
    gen_hue();
    gen_dye();
    return 0;
}
