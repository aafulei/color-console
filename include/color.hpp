#ifndef COLOR_CONSOLE
#define COLOR_CONSOLE

#include <cstddef>
#include <cstdio>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

// ---------- ↓↓ hue namespace ↓↓ -------------------------------------------------------------------------------------
namespace hue
{
    const std::unordered_map<std::string, int> TABLE
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

    inline std::pair<int, int> parse(std::string tag)
    {
        auto it = TABLE.find(tag);
        if (it != TABLE.end())
            return {it->second, -1};
        else  {
            std::size_t pos = tag.find("on");
            if (pos == 0)
                return {-1, TABLE.find(tag.substr(3))->second};
            else
                return {TABLE.at(tag.substr(0, pos - 1)), TABLE.at(tag.substr(pos + 3))};
        }
    }

    // Windows-specific implementation
    #ifdef _WIN32
    #include <windows.h>

    inline int get();
    const int DEFAULT_COLOR = get();
    const int DEFAULT_FOREGROUND = DEFAULT_COLOR & 0xf;
    const int DEFAULT_BACKGROUND = DEFAULT_COLOR >> 4;
    static int CONSOLE_FOREGROUND = DEFAULT_FOREGROUND;
    static int CONSOLE_BACKGROUND = DEFAULT_BACKGROUND;

    inline int get()
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info) == 0)
            throw std::runtime_error("Failed to get color attributes from console.");
        return info.wAttributes;
    }

    inline std::string set(int fg, int bg)
    {
        bool invert = fg == -2 || bg == -2;
        if (fg == -1 || fg == -2)
            fg = CONSOLE_FOREGROUND;
        if (bg == -1 || bg == -2)
            bg = CONSOLE_BACKGROUND;
        if (invert)
            std::swap(fg, bg);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg | bg << 4);
        CONSOLE_FOREGROUND = fg;
        CONSOLE_BACKGROUND = bg;
        return {};
    }

    inline std::string set(std::string tag)
    {
        std::pair<int, int> pair = parse(tag);
        return set(pair.first, pair.second);
    }

    inline std::string set_foreground(int fg)
    {
        return set(fg, CONSOLE_BACKGROUND);
    }

    inline std::string set_background(int bg)
    {
        return set(CONSOLE_FOREGROUND, bg);
    }

    inline std::string reset()
    {
        return set(DEFAULT_FOREGROUND, DEFAULT_BACKGROUND);
    }

    // Unix-specific implementation
    #elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))

    inline int win_to_unix(int win_color)
    {
        switch (win_color) {
            case  0: return 30;
            case  1: return 34;
            case  2: return 32;
            case  3: return 36;
            case  4: return 31;
            case  5: return 35;
            case  6: return 33;
            case  7: return 37;
            case  8: return 90;
            case  9: return 94;
            case 10: return 92;
            case 11: return 96;
            case 12: return 91;
            case 13: return 95;
            case 14: return 93;
            case 15: return 97;
            default: throw std::invalid_argument("Foreground and background color values must be a hex digit.");
        }
    }

    inline std::string set_foreground(int fg)
    {
        std::printf("\033[%dm", win_to_unix(fg));
        return {};
    }

    inline std::string set_background(int bg)
    {
        std::printf("\033[%dm", win_to_unix(bg) + 10);
        return {};
    }

    inline std::string set(int fg, int bg)
    {
        if (fg == -2 || bg == -2)
            printf("\033[7m");
        if (fg >> 4 == 0)
            set_foreground(fg);
        if (bg >> 4 == 0)
            set_background(bg);
        return {};
    }

    inline std::string set(std::string tag)
    {
        std::pair<int, int> pair = parse(tag);
        return set(pair.first, pair.second);
    }

    inline std::string reset()
    {
        std::printf("\033[0m");
        return {};
    }
    #endif

    // Windows-and-Unix-common implementation
    inline std::ostream & reset(std::ostream & os) { return os << reset(); }
    inline std::ostream & black(std::ostream & os) { return os << set_foreground(0); }
    inline std::ostream & blue(std::ostream & os) { return os << set_foreground(1); }
    inline std::ostream & green(std::ostream & os) { return os << set_foreground(2); }
    inline std::ostream & aqua(std::ostream & os) { return os << set_foreground(3); }
    inline std::ostream & cyan(std::ostream & os) { return os << set_foreground(3); }
    inline std::ostream & red(std::ostream & os) { return os << set_foreground(4); }
    inline std::ostream & purple(std::ostream & os) { return os << set_foreground(5); }
    inline std::ostream & magenta(std::ostream & os) { return os << set_foreground(5); }
    inline std::ostream & yellow(std::ostream & os) { return os << set_foreground(6); }
    inline std::ostream & white(std::ostream & os) { return os << set_foreground(7); }
    inline std::ostream & gray(std::ostream & os) { return os << set_foreground(8); }
    inline std::ostream & light_blue(std::ostream & os) { return os << set_foreground(9); }
    inline std::ostream & light_green(std::ostream & os) { return os << set_foreground(10); }
    inline std::ostream & light_aqua(std::ostream & os) { return os << set_foreground(11); }
    inline std::ostream & light_cyan(std::ostream & os) { return os << set_foreground(11); }
    inline std::ostream & light_red(std::ostream & os) { return os << set_foreground(12); }
    inline std::ostream & light_purple(std::ostream & os) { return os << set_foreground(13); }
    inline std::ostream & light_magenta(std::ostream & os) { return os << set_foreground(13); }
    inline std::ostream & light_yellow(std::ostream & os) { return os << set_foreground(14); }
    inline std::ostream & bright_white(std::ostream & os) { return os << set_foreground(15); }
    inline std::ostream & on_black(std::ostream & os) { return os << set_background(0); }
    inline std::ostream & on_blue(std::ostream & os) { return os << set_background(1); }
    inline std::ostream & on_green(std::ostream & os) { return os << set_background(2); }
    inline std::ostream & on_aqua(std::ostream & os) { return os << set_background(3); }
    inline std::ostream & on_cyan(std::ostream & os) { return os << set_background(3); }
    inline std::ostream & on_red(std::ostream & os) { return os << set_background(4); }
    inline std::ostream & on_purple(std::ostream & os) { return os << set_background(5); }
    inline std::ostream & on_magenta(std::ostream & os) { return os << set_background(5); }
    inline std::ostream & on_yellow(std::ostream & os) { return os << set_background(6); }
    inline std::ostream & on_white(std::ostream & os) { return os << set_background(7); }
    inline std::ostream & on_gray(std::ostream & os) { return os << set_background(8); }
    inline std::ostream & on_light_blue(std::ostream & os) { return os << set_background(9); }
    inline std::ostream & on_light_green(std::ostream & os) { return os << set_background(10); }
    inline std::ostream & on_light_aqua(std::ostream & os) { return os << set_background(11); }
    inline std::ostream & on_light_cyan(std::ostream & os) { return os << set_background(11); }
    inline std::ostream & on_light_red(std::ostream & os) { return os << set_background(12); }
    inline std::ostream & on_light_purple(std::ostream & os) { return os << set_background(13); }
    inline std::ostream & on_light_magenta(std::ostream & os) { return os << set_background(13); }
    inline std::ostream & on_light_yellow(std::ostream & os) { return os << set_background(14); }
    inline std::ostream & on_bright_white(std::ostream & os) { return os << set_background(15); }
    inline std::ostream & black_on_black(std::ostream & os) { return os << set(0, 0); }
    inline std::ostream & black_on_blue(std::ostream & os) { return os << set(0, 1); }
    inline std::ostream & black_on_green(std::ostream & os) { return os << set(0, 2); }
    inline std::ostream & black_on_aqua(std::ostream & os) { return os << set(0, 3); }
    inline std::ostream & black_on_cyan(std::ostream & os) { return os << set(0, 3); }
    inline std::ostream & black_on_red(std::ostream & os) { return os << set(0, 4); }
    inline std::ostream & black_on_purple(std::ostream & os) { return os << set(0, 5); }
    inline std::ostream & black_on_magenta(std::ostream & os) { return os << set(0, 5); }
    inline std::ostream & black_on_yellow(std::ostream & os) { return os << set(0, 6); }
    inline std::ostream & black_on_white(std::ostream & os) { return os << set(0, 7); }
    inline std::ostream & black_on_gray(std::ostream & os) { return os << set(0, 8); }
    inline std::ostream & black_on_light_blue(std::ostream & os) { return os << set(0, 9); }
    inline std::ostream & black_on_light_green(std::ostream & os) { return os << set(0, 10); }
    inline std::ostream & black_on_light_aqua(std::ostream & os) { return os << set(0, 11); }
    inline std::ostream & black_on_light_cyan(std::ostream & os) { return os << set(0, 11); }
    inline std::ostream & black_on_light_red(std::ostream & os) { return os << set(0, 12); }
    inline std::ostream & black_on_light_purple(std::ostream & os) { return os << set(0, 13); }
    inline std::ostream & black_on_light_magenta(std::ostream & os) { return os << set(0, 13); }
    inline std::ostream & black_on_light_yellow(std::ostream & os) { return os << set(0, 14); }
    inline std::ostream & black_on_bright_white(std::ostream & os) { return os << set(0, 15); }
    inline std::ostream & blue_on_black(std::ostream & os) { return os << set(1, 0); }
    inline std::ostream & blue_on_blue(std::ostream & os) { return os << set(1, 1); }
    inline std::ostream & blue_on_green(std::ostream & os) { return os << set(1, 2); }
    inline std::ostream & blue_on_aqua(std::ostream & os) { return os << set(1, 3); }
    inline std::ostream & blue_on_cyan(std::ostream & os) { return os << set(1, 3); }
    inline std::ostream & blue_on_red(std::ostream & os) { return os << set(1, 4); }
    inline std::ostream & blue_on_purple(std::ostream & os) { return os << set(1, 5); }
    inline std::ostream & blue_on_magenta(std::ostream & os) { return os << set(1, 5); }
    inline std::ostream & blue_on_yellow(std::ostream & os) { return os << set(1, 6); }
    inline std::ostream & blue_on_white(std::ostream & os) { return os << set(1, 7); }
    inline std::ostream & blue_on_gray(std::ostream & os) { return os << set(1, 8); }
    inline std::ostream & blue_on_light_blue(std::ostream & os) { return os << set(1, 9); }
    inline std::ostream & blue_on_light_green(std::ostream & os) { return os << set(1, 10); }
    inline std::ostream & blue_on_light_aqua(std::ostream & os) { return os << set(1, 11); }
    inline std::ostream & blue_on_light_cyan(std::ostream & os) { return os << set(1, 11); }
    inline std::ostream & blue_on_light_red(std::ostream & os) { return os << set(1, 12); }
    inline std::ostream & blue_on_light_purple(std::ostream & os) { return os << set(1, 13); }
    inline std::ostream & blue_on_light_magenta(std::ostream & os) { return os << set(1, 13); }
    inline std::ostream & blue_on_light_yellow(std::ostream & os) { return os << set(1, 14); }
    inline std::ostream & blue_on_bright_white(std::ostream & os) { return os << set(1, 15); }
    inline std::ostream & green_on_black(std::ostream & os) { return os << set(2, 0); }
    inline std::ostream & green_on_blue(std::ostream & os) { return os << set(2, 1); }
    inline std::ostream & green_on_green(std::ostream & os) { return os << set(2, 2); }
    inline std::ostream & green_on_aqua(std::ostream & os) { return os << set(2, 3); }
    inline std::ostream & green_on_cyan(std::ostream & os) { return os << set(2, 3); }
    inline std::ostream & green_on_red(std::ostream & os) { return os << set(2, 4); }
    inline std::ostream & green_on_purple(std::ostream & os) { return os << set(2, 5); }
    inline std::ostream & green_on_magenta(std::ostream & os) { return os << set(2, 5); }
    inline std::ostream & green_on_yellow(std::ostream & os) { return os << set(2, 6); }
    inline std::ostream & green_on_white(std::ostream & os) { return os << set(2, 7); }
    inline std::ostream & green_on_gray(std::ostream & os) { return os << set(2, 8); }
    inline std::ostream & green_on_light_blue(std::ostream & os) { return os << set(2, 9); }
    inline std::ostream & green_on_light_green(std::ostream & os) { return os << set(2, 10); }
    inline std::ostream & green_on_light_aqua(std::ostream & os) { return os << set(2, 11); }
    inline std::ostream & green_on_light_cyan(std::ostream & os) { return os << set(2, 11); }
    inline std::ostream & green_on_light_red(std::ostream & os) { return os << set(2, 12); }
    inline std::ostream & green_on_light_purple(std::ostream & os) { return os << set(2, 13); }
    inline std::ostream & green_on_light_magenta(std::ostream & os) { return os << set(2, 13); }
    inline std::ostream & green_on_light_yellow(std::ostream & os) { return os << set(2, 14); }
    inline std::ostream & green_on_bright_white(std::ostream & os) { return os << set(2, 15); }
    inline std::ostream & aqua_on_black(std::ostream & os) { return os << set(3, 0); }
    inline std::ostream & aqua_on_blue(std::ostream & os) { return os << set(3, 1); }
    inline std::ostream & aqua_on_green(std::ostream & os) { return os << set(3, 2); }
    inline std::ostream & aqua_on_aqua(std::ostream & os) { return os << set(3, 3); }
    inline std::ostream & aqua_on_cyan(std::ostream & os) { return os << set(3, 3); }
    inline std::ostream & aqua_on_red(std::ostream & os) { return os << set(3, 4); }
    inline std::ostream & aqua_on_purple(std::ostream & os) { return os << set(3, 5); }
    inline std::ostream & aqua_on_magenta(std::ostream & os) { return os << set(3, 5); }
    inline std::ostream & aqua_on_yellow(std::ostream & os) { return os << set(3, 6); }
    inline std::ostream & aqua_on_white(std::ostream & os) { return os << set(3, 7); }
    inline std::ostream & aqua_on_gray(std::ostream & os) { return os << set(3, 8); }
    inline std::ostream & aqua_on_light_blue(std::ostream & os) { return os << set(3, 9); }
    inline std::ostream & aqua_on_light_green(std::ostream & os) { return os << set(3, 10); }
    inline std::ostream & aqua_on_light_aqua(std::ostream & os) { return os << set(3, 11); }
    inline std::ostream & aqua_on_light_cyan(std::ostream & os) { return os << set(3, 11); }
    inline std::ostream & aqua_on_light_red(std::ostream & os) { return os << set(3, 12); }
    inline std::ostream & aqua_on_light_purple(std::ostream & os) { return os << set(3, 13); }
    inline std::ostream & aqua_on_light_magenta(std::ostream & os) { return os << set(3, 13); }
    inline std::ostream & aqua_on_light_yellow(std::ostream & os) { return os << set(3, 14); }
    inline std::ostream & aqua_on_bright_white(std::ostream & os) { return os << set(3, 15); }
    inline std::ostream & cyan_on_black(std::ostream & os) { return os << set(3, 0); }
    inline std::ostream & cyan_on_blue(std::ostream & os) { return os << set(3, 1); }
    inline std::ostream & cyan_on_green(std::ostream & os) { return os << set(3, 2); }
    inline std::ostream & cyan_on_aqua(std::ostream & os) { return os << set(3, 3); }
    inline std::ostream & cyan_on_cyan(std::ostream & os) { return os << set(3, 3); }
    inline std::ostream & cyan_on_red(std::ostream & os) { return os << set(3, 4); }
    inline std::ostream & cyan_on_purple(std::ostream & os) { return os << set(3, 5); }
    inline std::ostream & cyan_on_magenta(std::ostream & os) { return os << set(3, 5); }
    inline std::ostream & cyan_on_yellow(std::ostream & os) { return os << set(3, 6); }
    inline std::ostream & cyan_on_white(std::ostream & os) { return os << set(3, 7); }
    inline std::ostream & cyan_on_gray(std::ostream & os) { return os << set(3, 8); }
    inline std::ostream & cyan_on_light_blue(std::ostream & os) { return os << set(3, 9); }
    inline std::ostream & cyan_on_light_green(std::ostream & os) { return os << set(3, 10); }
    inline std::ostream & cyan_on_light_aqua(std::ostream & os) { return os << set(3, 11); }
    inline std::ostream & cyan_on_light_cyan(std::ostream & os) { return os << set(3, 11); }
    inline std::ostream & cyan_on_light_red(std::ostream & os) { return os << set(3, 12); }
    inline std::ostream & cyan_on_light_purple(std::ostream & os) { return os << set(3, 13); }
    inline std::ostream & cyan_on_light_magenta(std::ostream & os) { return os << set(3, 13); }
    inline std::ostream & cyan_on_light_yellow(std::ostream & os) { return os << set(3, 14); }
    inline std::ostream & cyan_on_bright_white(std::ostream & os) { return os << set(3, 15); }
    inline std::ostream & red_on_black(std::ostream & os) { return os << set(4, 0); }
    inline std::ostream & red_on_blue(std::ostream & os) { return os << set(4, 1); }
    inline std::ostream & red_on_green(std::ostream & os) { return os << set(4, 2); }
    inline std::ostream & red_on_aqua(std::ostream & os) { return os << set(4, 3); }
    inline std::ostream & red_on_cyan(std::ostream & os) { return os << set(4, 3); }
    inline std::ostream & red_on_red(std::ostream & os) { return os << set(4, 4); }
    inline std::ostream & red_on_purple(std::ostream & os) { return os << set(4, 5); }
    inline std::ostream & red_on_magenta(std::ostream & os) { return os << set(4, 5); }
    inline std::ostream & red_on_yellow(std::ostream & os) { return os << set(4, 6); }
    inline std::ostream & red_on_white(std::ostream & os) { return os << set(4, 7); }
    inline std::ostream & red_on_gray(std::ostream & os) { return os << set(4, 8); }
    inline std::ostream & red_on_light_blue(std::ostream & os) { return os << set(4, 9); }
    inline std::ostream & red_on_light_green(std::ostream & os) { return os << set(4, 10); }
    inline std::ostream & red_on_light_aqua(std::ostream & os) { return os << set(4, 11); }
    inline std::ostream & red_on_light_cyan(std::ostream & os) { return os << set(4, 11); }
    inline std::ostream & red_on_light_red(std::ostream & os) { return os << set(4, 12); }
    inline std::ostream & red_on_light_purple(std::ostream & os) { return os << set(4, 13); }
    inline std::ostream & red_on_light_magenta(std::ostream & os) { return os << set(4, 13); }
    inline std::ostream & red_on_light_yellow(std::ostream & os) { return os << set(4, 14); }
    inline std::ostream & red_on_bright_white(std::ostream & os) { return os << set(4, 15); }
    inline std::ostream & purple_on_black(std::ostream & os) { return os << set(5, 0); }
    inline std::ostream & purple_on_blue(std::ostream & os) { return os << set(5, 1); }
    inline std::ostream & purple_on_green(std::ostream & os) { return os << set(5, 2); }
    inline std::ostream & purple_on_aqua(std::ostream & os) { return os << set(5, 3); }
    inline std::ostream & purple_on_cyan(std::ostream & os) { return os << set(5, 3); }
    inline std::ostream & purple_on_red(std::ostream & os) { return os << set(5, 4); }
    inline std::ostream & purple_on_purple(std::ostream & os) { return os << set(5, 5); }
    inline std::ostream & purple_on_magenta(std::ostream & os) { return os << set(5, 5); }
    inline std::ostream & purple_on_yellow(std::ostream & os) { return os << set(5, 6); }
    inline std::ostream & purple_on_white(std::ostream & os) { return os << set(5, 7); }
    inline std::ostream & purple_on_gray(std::ostream & os) { return os << set(5, 8); }
    inline std::ostream & purple_on_light_blue(std::ostream & os) { return os << set(5, 9); }
    inline std::ostream & purple_on_light_green(std::ostream & os) { return os << set(5, 10); }
    inline std::ostream & purple_on_light_aqua(std::ostream & os) { return os << set(5, 11); }
    inline std::ostream & purple_on_light_cyan(std::ostream & os) { return os << set(5, 11); }
    inline std::ostream & purple_on_light_red(std::ostream & os) { return os << set(5, 12); }
    inline std::ostream & purple_on_light_purple(std::ostream & os) { return os << set(5, 13); }
    inline std::ostream & purple_on_light_magenta(std::ostream & os) { return os << set(5, 13); }
    inline std::ostream & purple_on_light_yellow(std::ostream & os) { return os << set(5, 14); }
    inline std::ostream & purple_on_bright_white(std::ostream & os) { return os << set(5, 15); }
    inline std::ostream & magenta_on_black(std::ostream & os) { return os << set(5, 0); }
    inline std::ostream & magenta_on_blue(std::ostream & os) { return os << set(5, 1); }
    inline std::ostream & magenta_on_green(std::ostream & os) { return os << set(5, 2); }
    inline std::ostream & magenta_on_aqua(std::ostream & os) { return os << set(5, 3); }
    inline std::ostream & magenta_on_cyan(std::ostream & os) { return os << set(5, 3); }
    inline std::ostream & magenta_on_red(std::ostream & os) { return os << set(5, 4); }
    inline std::ostream & magenta_on_purple(std::ostream & os) { return os << set(5, 5); }
    inline std::ostream & magenta_on_magenta(std::ostream & os) { return os << set(5, 5); }
    inline std::ostream & magenta_on_yellow(std::ostream & os) { return os << set(5, 6); }
    inline std::ostream & magenta_on_white(std::ostream & os) { return os << set(5, 7); }
    inline std::ostream & magenta_on_gray(std::ostream & os) { return os << set(5, 8); }
    inline std::ostream & magenta_on_light_blue(std::ostream & os) { return os << set(5, 9); }
    inline std::ostream & magenta_on_light_green(std::ostream & os) { return os << set(5, 10); }
    inline std::ostream & magenta_on_light_aqua(std::ostream & os) { return os << set(5, 11); }
    inline std::ostream & magenta_on_light_cyan(std::ostream & os) { return os << set(5, 11); }
    inline std::ostream & magenta_on_light_red(std::ostream & os) { return os << set(5, 12); }
    inline std::ostream & magenta_on_light_purple(std::ostream & os) { return os << set(5, 13); }
    inline std::ostream & magenta_on_light_magenta(std::ostream & os) { return os << set(5, 13); }
    inline std::ostream & magenta_on_light_yellow(std::ostream & os) { return os << set(5, 14); }
    inline std::ostream & magenta_on_bright_white(std::ostream & os) { return os << set(5, 15); }
    inline std::ostream & yellow_on_black(std::ostream & os) { return os << set(6, 0); }
    inline std::ostream & yellow_on_blue(std::ostream & os) { return os << set(6, 1); }
    inline std::ostream & yellow_on_green(std::ostream & os) { return os << set(6, 2); }
    inline std::ostream & yellow_on_aqua(std::ostream & os) { return os << set(6, 3); }
    inline std::ostream & yellow_on_cyan(std::ostream & os) { return os << set(6, 3); }
    inline std::ostream & yellow_on_red(std::ostream & os) { return os << set(6, 4); }
    inline std::ostream & yellow_on_purple(std::ostream & os) { return os << set(6, 5); }
    inline std::ostream & yellow_on_magenta(std::ostream & os) { return os << set(6, 5); }
    inline std::ostream & yellow_on_yellow(std::ostream & os) { return os << set(6, 6); }
    inline std::ostream & yellow_on_white(std::ostream & os) { return os << set(6, 7); }
    inline std::ostream & yellow_on_gray(std::ostream & os) { return os << set(6, 8); }
    inline std::ostream & yellow_on_light_blue(std::ostream & os) { return os << set(6, 9); }
    inline std::ostream & yellow_on_light_green(std::ostream & os) { return os << set(6, 10); }
    inline std::ostream & yellow_on_light_aqua(std::ostream & os) { return os << set(6, 11); }
    inline std::ostream & yellow_on_light_cyan(std::ostream & os) { return os << set(6, 11); }
    inline std::ostream & yellow_on_light_red(std::ostream & os) { return os << set(6, 12); }
    inline std::ostream & yellow_on_light_purple(std::ostream & os) { return os << set(6, 13); }
    inline std::ostream & yellow_on_light_magenta(std::ostream & os) { return os << set(6, 13); }
    inline std::ostream & yellow_on_light_yellow(std::ostream & os) { return os << set(6, 14); }
    inline std::ostream & yellow_on_bright_white(std::ostream & os) { return os << set(6, 15); }
    inline std::ostream & white_on_black(std::ostream & os) { return os << set(7, 0); }
    inline std::ostream & white_on_blue(std::ostream & os) { return os << set(7, 1); }
    inline std::ostream & white_on_green(std::ostream & os) { return os << set(7, 2); }
    inline std::ostream & white_on_aqua(std::ostream & os) { return os << set(7, 3); }
    inline std::ostream & white_on_cyan(std::ostream & os) { return os << set(7, 3); }
    inline std::ostream & white_on_red(std::ostream & os) { return os << set(7, 4); }
    inline std::ostream & white_on_purple(std::ostream & os) { return os << set(7, 5); }
    inline std::ostream & white_on_magenta(std::ostream & os) { return os << set(7, 5); }
    inline std::ostream & white_on_yellow(std::ostream & os) { return os << set(7, 6); }
    inline std::ostream & white_on_white(std::ostream & os) { return os << set(7, 7); }
    inline std::ostream & white_on_gray(std::ostream & os) { return os << set(7, 8); }
    inline std::ostream & white_on_light_blue(std::ostream & os) { return os << set(7, 9); }
    inline std::ostream & white_on_light_green(std::ostream & os) { return os << set(7, 10); }
    inline std::ostream & white_on_light_aqua(std::ostream & os) { return os << set(7, 11); }
    inline std::ostream & white_on_light_cyan(std::ostream & os) { return os << set(7, 11); }
    inline std::ostream & white_on_light_red(std::ostream & os) { return os << set(7, 12); }
    inline std::ostream & white_on_light_purple(std::ostream & os) { return os << set(7, 13); }
    inline std::ostream & white_on_light_magenta(std::ostream & os) { return os << set(7, 13); }
    inline std::ostream & white_on_light_yellow(std::ostream & os) { return os << set(7, 14); }
    inline std::ostream & white_on_bright_white(std::ostream & os) { return os << set(7, 15); }
    inline std::ostream & gray_on_black(std::ostream & os) { return os << set(8, 0); }
    inline std::ostream & gray_on_blue(std::ostream & os) { return os << set(8, 1); }
    inline std::ostream & gray_on_green(std::ostream & os) { return os << set(8, 2); }
    inline std::ostream & gray_on_aqua(std::ostream & os) { return os << set(8, 3); }
    inline std::ostream & gray_on_cyan(std::ostream & os) { return os << set(8, 3); }
    inline std::ostream & gray_on_red(std::ostream & os) { return os << set(8, 4); }
    inline std::ostream & gray_on_purple(std::ostream & os) { return os << set(8, 5); }
    inline std::ostream & gray_on_magenta(std::ostream & os) { return os << set(8, 5); }
    inline std::ostream & gray_on_yellow(std::ostream & os) { return os << set(8, 6); }
    inline std::ostream & gray_on_white(std::ostream & os) { return os << set(8, 7); }
    inline std::ostream & gray_on_gray(std::ostream & os) { return os << set(8, 8); }
    inline std::ostream & gray_on_light_blue(std::ostream & os) { return os << set(8, 9); }
    inline std::ostream & gray_on_light_green(std::ostream & os) { return os << set(8, 10); }
    inline std::ostream & gray_on_light_aqua(std::ostream & os) { return os << set(8, 11); }
    inline std::ostream & gray_on_light_cyan(std::ostream & os) { return os << set(8, 11); }
    inline std::ostream & gray_on_light_red(std::ostream & os) { return os << set(8, 12); }
    inline std::ostream & gray_on_light_purple(std::ostream & os) { return os << set(8, 13); }
    inline std::ostream & gray_on_light_magenta(std::ostream & os) { return os << set(8, 13); }
    inline std::ostream & gray_on_light_yellow(std::ostream & os) { return os << set(8, 14); }
    inline std::ostream & gray_on_bright_white(std::ostream & os) { return os << set(8, 15); }
    inline std::ostream & light_blue_on_black(std::ostream & os) { return os << set(9, 0); }
    inline std::ostream & light_blue_on_blue(std::ostream & os) { return os << set(9, 1); }
    inline std::ostream & light_blue_on_green(std::ostream & os) { return os << set(9, 2); }
    inline std::ostream & light_blue_on_aqua(std::ostream & os) { return os << set(9, 3); }
    inline std::ostream & light_blue_on_cyan(std::ostream & os) { return os << set(9, 3); }
    inline std::ostream & light_blue_on_red(std::ostream & os) { return os << set(9, 4); }
    inline std::ostream & light_blue_on_purple(std::ostream & os) { return os << set(9, 5); }
    inline std::ostream & light_blue_on_magenta(std::ostream & os) { return os << set(9, 5); }
    inline std::ostream & light_blue_on_yellow(std::ostream & os) { return os << set(9, 6); }
    inline std::ostream & light_blue_on_white(std::ostream & os) { return os << set(9, 7); }
    inline std::ostream & light_blue_on_gray(std::ostream & os) { return os << set(9, 8); }
    inline std::ostream & light_blue_on_light_blue(std::ostream & os) { return os << set(9, 9); }
    inline std::ostream & light_blue_on_light_green(std::ostream & os) { return os << set(9, 10); }
    inline std::ostream & light_blue_on_light_aqua(std::ostream & os) { return os << set(9, 11); }
    inline std::ostream & light_blue_on_light_cyan(std::ostream & os) { return os << set(9, 11); }
    inline std::ostream & light_blue_on_light_red(std::ostream & os) { return os << set(9, 12); }
    inline std::ostream & light_blue_on_light_purple(std::ostream & os) { return os << set(9, 13); }
    inline std::ostream & light_blue_on_light_magenta(std::ostream & os) { return os << set(9, 13); }
    inline std::ostream & light_blue_on_light_yellow(std::ostream & os) { return os << set(9, 14); }
    inline std::ostream & light_blue_on_bright_white(std::ostream & os) { return os << set(9, 15); }
    inline std::ostream & light_green_on_black(std::ostream & os) { return os << set(10, 0); }
    inline std::ostream & light_green_on_blue(std::ostream & os) { return os << set(10, 1); }
    inline std::ostream & light_green_on_green(std::ostream & os) { return os << set(10, 2); }
    inline std::ostream & light_green_on_aqua(std::ostream & os) { return os << set(10, 3); }
    inline std::ostream & light_green_on_cyan(std::ostream & os) { return os << set(10, 3); }
    inline std::ostream & light_green_on_red(std::ostream & os) { return os << set(10, 4); }
    inline std::ostream & light_green_on_purple(std::ostream & os) { return os << set(10, 5); }
    inline std::ostream & light_green_on_magenta(std::ostream & os) { return os << set(10, 5); }
    inline std::ostream & light_green_on_yellow(std::ostream & os) { return os << set(10, 6); }
    inline std::ostream & light_green_on_white(std::ostream & os) { return os << set(10, 7); }
    inline std::ostream & light_green_on_gray(std::ostream & os) { return os << set(10, 8); }
    inline std::ostream & light_green_on_light_blue(std::ostream & os) { return os << set(10, 9); }
    inline std::ostream & light_green_on_light_green(std::ostream & os) { return os << set(10, 10); }
    inline std::ostream & light_green_on_light_aqua(std::ostream & os) { return os << set(10, 11); }
    inline std::ostream & light_green_on_light_cyan(std::ostream & os) { return os << set(10, 11); }
    inline std::ostream & light_green_on_light_red(std::ostream & os) { return os << set(10, 12); }
    inline std::ostream & light_green_on_light_purple(std::ostream & os) { return os << set(10, 13); }
    inline std::ostream & light_green_on_light_magenta(std::ostream & os) { return os << set(10, 13); }
    inline std::ostream & light_green_on_light_yellow(std::ostream & os) { return os << set(10, 14); }
    inline std::ostream & light_green_on_bright_white(std::ostream & os) { return os << set(10, 15); }
    inline std::ostream & light_aqua_on_black(std::ostream & os) { return os << set(11, 0); }
    inline std::ostream & light_aqua_on_blue(std::ostream & os) { return os << set(11, 1); }
    inline std::ostream & light_aqua_on_green(std::ostream & os) { return os << set(11, 2); }
    inline std::ostream & light_aqua_on_aqua(std::ostream & os) { return os << set(11, 3); }
    inline std::ostream & light_aqua_on_cyan(std::ostream & os) { return os << set(11, 3); }
    inline std::ostream & light_aqua_on_red(std::ostream & os) { return os << set(11, 4); }
    inline std::ostream & light_aqua_on_purple(std::ostream & os) { return os << set(11, 5); }
    inline std::ostream & light_aqua_on_magenta(std::ostream & os) { return os << set(11, 5); }
    inline std::ostream & light_aqua_on_yellow(std::ostream & os) { return os << set(11, 6); }
    inline std::ostream & light_aqua_on_white(std::ostream & os) { return os << set(11, 7); }
    inline std::ostream & light_aqua_on_gray(std::ostream & os) { return os << set(11, 8); }
    inline std::ostream & light_aqua_on_light_blue(std::ostream & os) { return os << set(11, 9); }
    inline std::ostream & light_aqua_on_light_green(std::ostream & os) { return os << set(11, 10); }
    inline std::ostream & light_aqua_on_light_aqua(std::ostream & os) { return os << set(11, 11); }
    inline std::ostream & light_aqua_on_light_cyan(std::ostream & os) { return os << set(11, 11); }
    inline std::ostream & light_aqua_on_light_red(std::ostream & os) { return os << set(11, 12); }
    inline std::ostream & light_aqua_on_light_purple(std::ostream & os) { return os << set(11, 13); }
    inline std::ostream & light_aqua_on_light_magenta(std::ostream & os) { return os << set(11, 13); }
    inline std::ostream & light_aqua_on_light_yellow(std::ostream & os) { return os << set(11, 14); }
    inline std::ostream & light_aqua_on_bright_white(std::ostream & os) { return os << set(11, 15); }
    inline std::ostream & light_cyan_on_black(std::ostream & os) { return os << set(11, 0); }
    inline std::ostream & light_cyan_on_blue(std::ostream & os) { return os << set(11, 1); }
    inline std::ostream & light_cyan_on_green(std::ostream & os) { return os << set(11, 2); }
    inline std::ostream & light_cyan_on_aqua(std::ostream & os) { return os << set(11, 3); }
    inline std::ostream & light_cyan_on_cyan(std::ostream & os) { return os << set(11, 3); }
    inline std::ostream & light_cyan_on_red(std::ostream & os) { return os << set(11, 4); }
    inline std::ostream & light_cyan_on_purple(std::ostream & os) { return os << set(11, 5); }
    inline std::ostream & light_cyan_on_magenta(std::ostream & os) { return os << set(11, 5); }
    inline std::ostream & light_cyan_on_yellow(std::ostream & os) { return os << set(11, 6); }
    inline std::ostream & light_cyan_on_white(std::ostream & os) { return os << set(11, 7); }
    inline std::ostream & light_cyan_on_gray(std::ostream & os) { return os << set(11, 8); }
    inline std::ostream & light_cyan_on_light_blue(std::ostream & os) { return os << set(11, 9); }
    inline std::ostream & light_cyan_on_light_green(std::ostream & os) { return os << set(11, 10); }
    inline std::ostream & light_cyan_on_light_aqua(std::ostream & os) { return os << set(11, 11); }
    inline std::ostream & light_cyan_on_light_cyan(std::ostream & os) { return os << set(11, 11); }
    inline std::ostream & light_cyan_on_light_red(std::ostream & os) { return os << set(11, 12); }
    inline std::ostream & light_cyan_on_light_purple(std::ostream & os) { return os << set(11, 13); }
    inline std::ostream & light_cyan_on_light_magenta(std::ostream & os) { return os << set(11, 13); }
    inline std::ostream & light_cyan_on_light_yellow(std::ostream & os) { return os << set(11, 14); }
    inline std::ostream & light_cyan_on_bright_white(std::ostream & os) { return os << set(11, 15); }
    inline std::ostream & light_red_on_black(std::ostream & os) { return os << set(12, 0); }
    inline std::ostream & light_red_on_blue(std::ostream & os) { return os << set(12, 1); }
    inline std::ostream & light_red_on_green(std::ostream & os) { return os << set(12, 2); }
    inline std::ostream & light_red_on_aqua(std::ostream & os) { return os << set(12, 3); }
    inline std::ostream & light_red_on_cyan(std::ostream & os) { return os << set(12, 3); }
    inline std::ostream & light_red_on_red(std::ostream & os) { return os << set(12, 4); }
    inline std::ostream & light_red_on_purple(std::ostream & os) { return os << set(12, 5); }
    inline std::ostream & light_red_on_magenta(std::ostream & os) { return os << set(12, 5); }
    inline std::ostream & light_red_on_yellow(std::ostream & os) { return os << set(12, 6); }
    inline std::ostream & light_red_on_white(std::ostream & os) { return os << set(12, 7); }
    inline std::ostream & light_red_on_gray(std::ostream & os) { return os << set(12, 8); }
    inline std::ostream & light_red_on_light_blue(std::ostream & os) { return os << set(12, 9); }
    inline std::ostream & light_red_on_light_green(std::ostream & os) { return os << set(12, 10); }
    inline std::ostream & light_red_on_light_aqua(std::ostream & os) { return os << set(12, 11); }
    inline std::ostream & light_red_on_light_cyan(std::ostream & os) { return os << set(12, 11); }
    inline std::ostream & light_red_on_light_red(std::ostream & os) { return os << set(12, 12); }
    inline std::ostream & light_red_on_light_purple(std::ostream & os) { return os << set(12, 13); }
    inline std::ostream & light_red_on_light_magenta(std::ostream & os) { return os << set(12, 13); }
    inline std::ostream & light_red_on_light_yellow(std::ostream & os) { return os << set(12, 14); }
    inline std::ostream & light_red_on_bright_white(std::ostream & os) { return os << set(12, 15); }
    inline std::ostream & light_purple_on_black(std::ostream & os) { return os << set(13, 0); }
    inline std::ostream & light_purple_on_blue(std::ostream & os) { return os << set(13, 1); }
    inline std::ostream & light_purple_on_green(std::ostream & os) { return os << set(13, 2); }
    inline std::ostream & light_purple_on_aqua(std::ostream & os) { return os << set(13, 3); }
    inline std::ostream & light_purple_on_cyan(std::ostream & os) { return os << set(13, 3); }
    inline std::ostream & light_purple_on_red(std::ostream & os) { return os << set(13, 4); }
    inline std::ostream & light_purple_on_purple(std::ostream & os) { return os << set(13, 5); }
    inline std::ostream & light_purple_on_magenta(std::ostream & os) { return os << set(13, 5); }
    inline std::ostream & light_purple_on_yellow(std::ostream & os) { return os << set(13, 6); }
    inline std::ostream & light_purple_on_white(std::ostream & os) { return os << set(13, 7); }
    inline std::ostream & light_purple_on_gray(std::ostream & os) { return os << set(13, 8); }
    inline std::ostream & light_purple_on_light_blue(std::ostream & os) { return os << set(13, 9); }
    inline std::ostream & light_purple_on_light_green(std::ostream & os) { return os << set(13, 10); }
    inline std::ostream & light_purple_on_light_aqua(std::ostream & os) { return os << set(13, 11); }
    inline std::ostream & light_purple_on_light_cyan(std::ostream & os) { return os << set(13, 11); }
    inline std::ostream & light_purple_on_light_red(std::ostream & os) { return os << set(13, 12); }
    inline std::ostream & light_purple_on_light_purple(std::ostream & os) { return os << set(13, 13); }
    inline std::ostream & light_purple_on_light_magenta(std::ostream & os) { return os << set(13, 13); }
    inline std::ostream & light_purple_on_light_yellow(std::ostream & os) { return os << set(13, 14); }
    inline std::ostream & light_purple_on_bright_white(std::ostream & os) { return os << set(13, 15); }
    inline std::ostream & light_magenta_on_black(std::ostream & os) { return os << set(13, 0); }
    inline std::ostream & light_magenta_on_blue(std::ostream & os) { return os << set(13, 1); }
    inline std::ostream & light_magenta_on_green(std::ostream & os) { return os << set(13, 2); }
    inline std::ostream & light_magenta_on_aqua(std::ostream & os) { return os << set(13, 3); }
    inline std::ostream & light_magenta_on_cyan(std::ostream & os) { return os << set(13, 3); }
    inline std::ostream & light_magenta_on_red(std::ostream & os) { return os << set(13, 4); }
    inline std::ostream & light_magenta_on_purple(std::ostream & os) { return os << set(13, 5); }
    inline std::ostream & light_magenta_on_magenta(std::ostream & os) { return os << set(13, 5); }
    inline std::ostream & light_magenta_on_yellow(std::ostream & os) { return os << set(13, 6); }
    inline std::ostream & light_magenta_on_white(std::ostream & os) { return os << set(13, 7); }
    inline std::ostream & light_magenta_on_gray(std::ostream & os) { return os << set(13, 8); }
    inline std::ostream & light_magenta_on_light_blue(std::ostream & os) { return os << set(13, 9); }
    inline std::ostream & light_magenta_on_light_green(std::ostream & os) { return os << set(13, 10); }
    inline std::ostream & light_magenta_on_light_aqua(std::ostream & os) { return os << set(13, 11); }
    inline std::ostream & light_magenta_on_light_cyan(std::ostream & os) { return os << set(13, 11); }
    inline std::ostream & light_magenta_on_light_red(std::ostream & os) { return os << set(13, 12); }
    inline std::ostream & light_magenta_on_light_purple(std::ostream & os) { return os << set(13, 13); }
    inline std::ostream & light_magenta_on_light_magenta(std::ostream & os) { return os << set(13, 13); }
    inline std::ostream & light_magenta_on_light_yellow(std::ostream & os) { return os << set(13, 14); }
    inline std::ostream & light_magenta_on_bright_white(std::ostream & os) { return os << set(13, 15); }
    inline std::ostream & light_yellow_on_black(std::ostream & os) { return os << set(14, 0); }
    inline std::ostream & light_yellow_on_blue(std::ostream & os) { return os << set(14, 1); }
    inline std::ostream & light_yellow_on_green(std::ostream & os) { return os << set(14, 2); }
    inline std::ostream & light_yellow_on_aqua(std::ostream & os) { return os << set(14, 3); }
    inline std::ostream & light_yellow_on_cyan(std::ostream & os) { return os << set(14, 3); }
    inline std::ostream & light_yellow_on_red(std::ostream & os) { return os << set(14, 4); }
    inline std::ostream & light_yellow_on_purple(std::ostream & os) { return os << set(14, 5); }
    inline std::ostream & light_yellow_on_magenta(std::ostream & os) { return os << set(14, 5); }
    inline std::ostream & light_yellow_on_yellow(std::ostream & os) { return os << set(14, 6); }
    inline std::ostream & light_yellow_on_white(std::ostream & os) { return os << set(14, 7); }
    inline std::ostream & light_yellow_on_gray(std::ostream & os) { return os << set(14, 8); }
    inline std::ostream & light_yellow_on_light_blue(std::ostream & os) { return os << set(14, 9); }
    inline std::ostream & light_yellow_on_light_green(std::ostream & os) { return os << set(14, 10); }
    inline std::ostream & light_yellow_on_light_aqua(std::ostream & os) { return os << set(14, 11); }
    inline std::ostream & light_yellow_on_light_cyan(std::ostream & os) { return os << set(14, 11); }
    inline std::ostream & light_yellow_on_light_red(std::ostream & os) { return os << set(14, 12); }
    inline std::ostream & light_yellow_on_light_purple(std::ostream & os) { return os << set(14, 13); }
    inline std::ostream & light_yellow_on_light_magenta(std::ostream & os) { return os << set(14, 13); }
    inline std::ostream & light_yellow_on_light_yellow(std::ostream & os) { return os << set(14, 14); }
    inline std::ostream & light_yellow_on_bright_white(std::ostream & os) { return os << set(14, 15); }
    inline std::ostream & bright_white_on_black(std::ostream & os) { return os << set(15, 0); }
    inline std::ostream & bright_white_on_blue(std::ostream & os) { return os << set(15, 1); }
    inline std::ostream & bright_white_on_green(std::ostream & os) { return os << set(15, 2); }
    inline std::ostream & bright_white_on_aqua(std::ostream & os) { return os << set(15, 3); }
    inline std::ostream & bright_white_on_cyan(std::ostream & os) { return os << set(15, 3); }
    inline std::ostream & bright_white_on_red(std::ostream & os) { return os << set(15, 4); }
    inline std::ostream & bright_white_on_purple(std::ostream & os) { return os << set(15, 5); }
    inline std::ostream & bright_white_on_magenta(std::ostream & os) { return os << set(15, 5); }
    inline std::ostream & bright_white_on_yellow(std::ostream & os) { return os << set(15, 6); }
    inline std::ostream & bright_white_on_white(std::ostream & os) { return os << set(15, 7); }
    inline std::ostream & bright_white_on_gray(std::ostream & os) { return os << set(15, 8); }
    inline std::ostream & bright_white_on_light_blue(std::ostream & os) { return os << set(15, 9); }
    inline std::ostream & bright_white_on_light_green(std::ostream & os) { return os << set(15, 10); }
    inline std::ostream & bright_white_on_light_aqua(std::ostream & os) { return os << set(15, 11); }
    inline std::ostream & bright_white_on_light_cyan(std::ostream & os) { return os << set(15, 11); }
    inline std::ostream & bright_white_on_light_red(std::ostream & os) { return os << set(15, 12); }
    inline std::ostream & bright_white_on_light_purple(std::ostream & os) { return os << set(15, 13); }
    inline std::ostream & bright_white_on_light_magenta(std::ostream & os) { return os << set(15, 13); }
    inline std::ostream & bright_white_on_light_yellow(std::ostream & os) { return os << set(15, 14); }
    inline std::ostream & bright_white_on_bright_white(std::ostream & os) { return os << set(15, 15); }
}
// ---------- ↑↑ hue namespace ↑↑ -------------------------------------------------------------------------------------

// ---------- ↓↓ dye namespace ↓↓ -------------------------------------------------------------------------------------
namespace dye
{
    template<typename T>
    class List;

    template<typename T>
    class Elem
    {
        T   content;
        int foreground;
        int background;

    public:
        Elem(T t) : content(t), foreground(-1), background(-1) {}
        Elem(T t, int fg, int bg) : content(t), foreground(fg), background(bg) {}
        Elem(T t, std::string tag) : content(t)
        {
            std::pair<int, int> pair = hue::parse(tag);
            foreground = pair.first;
            background = pair.second;
        }

        void invert()
        {
            if (foreground >= 0 && background >= 0)
                std::swap(foreground, background);
            else {
                if (foreground < 0)
                    foreground ^= 1;
                if (background < 0)
                    background ^= 1;
            }
        }

        Elem<T> reverse()
        {
            Elem<T> clone(*this);
            clone.invert();
            return clone;
        }

        void debug() const
        {
            std::printf("(%2d, %2d) ", this->foreground, this->background);
            std::cout << "[" << this->content << "]" << std::endl;
        }

        template<typename U>
        friend std::ostream & operator<<(std::ostream &, const Elem<U> &);

        template<typename U>
        friend std::ostream & operator<<(std::ostream &, const List<U> &);
    };

    template<typename T>
    std::ostream & operator<<(std::ostream & os, const Elem<T> & elem)
    {
        return os << hue::set(elem.foreground, elem.background) << elem.content << hue::reset;
    }

    template<typename T>
    class List : private std::list<Elem<T>>
    {
    public:
        using std::list<Elem<T>>::list;

        List<T> & operator+=(const List<T> & rhs)
        {
            this->insert(this->end(), rhs.begin(), rhs.end());
            return *this;
        }

        List<T> & operator+=(List<T> && rhs)
        {
            this->splice(this->end(), std::move(rhs));
            return *this;
        }

        List<T> & operator+=(T t)
        {
            this->push_back(std::move(t));
            return *this;
        }

        void push_back(T t)
        {
            this->std::list<Elem<T>>::push_back(Elem<T>(std::move(t)));
        }

        void push_front(T t)
        {
            this->std::list<Elem<T>>::push_front(Elem<T>(std::move(t)));
        }

        void invert()
        {
            for (Elem<T> & elem : *this)
                elem.invert();
        }

        List<T> reverse()
        {
            List<T> clone(*this);
            clone.invert();
            return clone;
        }

        void debug() const
        {
            for (const Elem<T> & elem: *this)
                elem.debug();
        }

        template<typename U>
        friend std::ostream & operator<<(std::ostream &, const List<U> &);
    };

    template<typename T>
    List<T> operator+(List<T> lhs, List<T> rhs)
    {
        List<T> res(std::move(lhs));
        return res += rhs;
    }

    template<typename T>
    List<T> operator+(List<T> lhs, std::string rhs)
    {
        List<T> res(std::move(lhs));
        res.push_back(std::move(rhs));
        return res;
    }

    template<typename T>
    List<T> operator+(std::string lhs, List<T> rhs)
    {
        List<T> res(std::move(rhs));
        res.push_front(std::move(lhs));
        return res;
    }

    template<typename T>
    std::ostream & operator<<(std::ostream & os, const List<T> & list)
    {
        for (auto it = list.cbegin(), prev = it; it != list.cend(); ++it) {
            if (prev == it || it->foreground != prev->foreground || it->background != prev->background) {
                os << hue::reset << hue::set(it->foreground, it->background);
            }
            os << it->content;
            prev = it;
        }
        return os << hue::reset;
    }

    template<typename T> using C = typename std::conditional<std::is_same<T, const char *>::value, std::string, T>::type;
    template<typename T> using L = List<C<T>>;
    template<typename T> using E = Elem<C<T>>;
    template<typename T> L<T> colorize(T t, std::string tag) { return L<T> { E<T>(t, tag) }; }
    template<typename T> L<T> vanilla(T t) { return L<T> { E<T>(t) }; }
    template<typename T> L<T> black(T t) { return L<T> { E<T>(t, 0, -1) }; }
    template<typename T> L<T> blue(T t) { return L<T> { E<T>(t, 1, -1) }; }
    template<typename T> L<T> green(T t) { return L<T> { E<T>(t, 2, -1) }; }
    template<typename T> L<T> aqua(T t) { return L<T> { E<T>(t, 3, -1) }; }
    template<typename T> L<T> cyan(T t) { return L<T> { E<T>(t, 3, -1) }; }
    template<typename T> L<T> red(T t) { return L<T> { E<T>(t, 4, -1) }; }
    template<typename T> L<T> purple(T t) { return L<T> { E<T>(t, 5, -1) }; }
    template<typename T> L<T> magenta(T t) { return L<T> { E<T>(t, 5, -1) }; }
    template<typename T> L<T> yellow(T t) { return L<T> { E<T>(t, 6, -1) }; }
    template<typename T> L<T> white(T t) { return L<T> { E<T>(t, 7, -1) }; }
    template<typename T> L<T> gray(T t) { return L<T> { E<T>(t, 8, -1) }; }
    template<typename T> L<T> light_blue(T t) { return L<T> { E<T>(t, 9, -1) }; }
    template<typename T> L<T> light_green(T t) { return L<T> { E<T>(t, 10, -1) }; }
    template<typename T> L<T> light_aqua(T t) { return L<T> { E<T>(t, 11, -1) }; }
    template<typename T> L<T> light_cyan(T t) { return L<T> { E<T>(t, 11, -1) }; }
    template<typename T> L<T> light_red(T t) { return L<T> { E<T>(t, 12, -1) }; }
    template<typename T> L<T> light_purple(T t) { return L<T> { E<T>(t, 13, -1) }; }
    template<typename T> L<T> light_magenta(T t) { return L<T> { E<T>(t, 13, -1) }; }
    template<typename T> L<T> light_yellow(T t) { return L<T> { E<T>(t, 14, -1) }; }
    template<typename T> L<T> bright_white(T t) { return L<T> { E<T>(t, 15, -1) }; }
    template<typename T> L<T> on_black(T t) { return L<T> { E<T>(t, -1, 0) }; }
    template<typename T> L<T> on_blue(T t) { return L<T> { E<T>(t, -1, 1) }; }
    template<typename T> L<T> on_green(T t) { return L<T> { E<T>(t, -1, 2) }; }
    template<typename T> L<T> on_aqua(T t) { return L<T> { E<T>(t, -1, 3) }; }
    template<typename T> L<T> on_cyan(T t) { return L<T> { E<T>(t, -1, 3) }; }
    template<typename T> L<T> on_red(T t) { return L<T> { E<T>(t, -1, 4) }; }
    template<typename T> L<T> on_purple(T t) { return L<T> { E<T>(t, -1, 5) }; }
    template<typename T> L<T> on_magenta(T t) { return L<T> { E<T>(t, -1, 5) }; }
    template<typename T> L<T> on_yellow(T t) { return L<T> { E<T>(t, -1, 6) }; }
    template<typename T> L<T> on_white(T t) { return L<T> { E<T>(t, -1, 7) }; }
    template<typename T> L<T> on_gray(T t) { return L<T> { E<T>(t, -1, 8) }; }
    template<typename T> L<T> on_light_blue(T t) { return L<T> { E<T>(t, -1, 9) }; }
    template<typename T> L<T> on_light_green(T t) { return L<T> { E<T>(t, -1, 10) }; }
    template<typename T> L<T> on_light_aqua(T t) { return L<T> { E<T>(t, -1, 11) }; }
    template<typename T> L<T> on_light_cyan(T t) { return L<T> { E<T>(t, -1, 11) }; }
    template<typename T> L<T> on_light_red(T t) { return L<T> { E<T>(t, -1, 12) }; }
    template<typename T> L<T> on_light_purple(T t) { return L<T> { E<T>(t, -1, 13) }; }
    template<typename T> L<T> on_light_magenta(T t) { return L<T> { E<T>(t, -1, 13) }; }
    template<typename T> L<T> on_light_yellow(T t) { return L<T> { E<T>(t, -1, 14) }; }
    template<typename T> L<T> on_bright_white(T t) { return L<T> { E<T>(t, -1, 15) }; }
    template<typename T> L<T> black_on_black(T t) { return L<T> { E<T>(t, 0, 0) }; }
    template<typename T> L<T> black_on_blue(T t) { return L<T> { E<T>(t, 0, 1) }; }
    template<typename T> L<T> black_on_green(T t) { return L<T> { E<T>(t, 0, 2) }; }
    template<typename T> L<T> black_on_aqua(T t) { return L<T> { E<T>(t, 0, 3) }; }
    template<typename T> L<T> black_on_cyan(T t) { return L<T> { E<T>(t, 0, 3) }; }
    template<typename T> L<T> black_on_red(T t) { return L<T> { E<T>(t, 0, 4) }; }
    template<typename T> L<T> black_on_purple(T t) { return L<T> { E<T>(t, 0, 5) }; }
    template<typename T> L<T> black_on_magenta(T t) { return L<T> { E<T>(t, 0, 5) }; }
    template<typename T> L<T> black_on_yellow(T t) { return L<T> { E<T>(t, 0, 6) }; }
    template<typename T> L<T> black_on_white(T t) { return L<T> { E<T>(t, 0, 7) }; }
    template<typename T> L<T> black_on_gray(T t) { return L<T> { E<T>(t, 0, 8) }; }
    template<typename T> L<T> black_on_light_blue(T t) { return L<T> { E<T>(t, 0, 9) }; }
    template<typename T> L<T> black_on_light_green(T t) { return L<T> { E<T>(t, 0, 10) }; }
    template<typename T> L<T> black_on_light_aqua(T t) { return L<T> { E<T>(t, 0, 11) }; }
    template<typename T> L<T> black_on_light_cyan(T t) { return L<T> { E<T>(t, 0, 11) }; }
    template<typename T> L<T> black_on_light_red(T t) { return L<T> { E<T>(t, 0, 12) }; }
    template<typename T> L<T> black_on_light_purple(T t) { return L<T> { E<T>(t, 0, 13) }; }
    template<typename T> L<T> black_on_light_magenta(T t) { return L<T> { E<T>(t, 0, 13) }; }
    template<typename T> L<T> black_on_light_yellow(T t) { return L<T> { E<T>(t, 0, 14) }; }
    template<typename T> L<T> black_on_bright_white(T t) { return L<T> { E<T>(t, 0, 15) }; }
    template<typename T> L<T> blue_on_black(T t) { return L<T> { E<T>(t, 1, 0) }; }
    template<typename T> L<T> blue_on_blue(T t) { return L<T> { E<T>(t, 1, 1) }; }
    template<typename T> L<T> blue_on_green(T t) { return L<T> { E<T>(t, 1, 2) }; }
    template<typename T> L<T> blue_on_aqua(T t) { return L<T> { E<T>(t, 1, 3) }; }
    template<typename T> L<T> blue_on_cyan(T t) { return L<T> { E<T>(t, 1, 3) }; }
    template<typename T> L<T> blue_on_red(T t) { return L<T> { E<T>(t, 1, 4) }; }
    template<typename T> L<T> blue_on_purple(T t) { return L<T> { E<T>(t, 1, 5) }; }
    template<typename T> L<T> blue_on_magenta(T t) { return L<T> { E<T>(t, 1, 5) }; }
    template<typename T> L<T> blue_on_yellow(T t) { return L<T> { E<T>(t, 1, 6) }; }
    template<typename T> L<T> blue_on_white(T t) { return L<T> { E<T>(t, 1, 7) }; }
    template<typename T> L<T> blue_on_gray(T t) { return L<T> { E<T>(t, 1, 8) }; }
    template<typename T> L<T> blue_on_light_blue(T t) { return L<T> { E<T>(t, 1, 9) }; }
    template<typename T> L<T> blue_on_light_green(T t) { return L<T> { E<T>(t, 1, 10) }; }
    template<typename T> L<T> blue_on_light_aqua(T t) { return L<T> { E<T>(t, 1, 11) }; }
    template<typename T> L<T> blue_on_light_cyan(T t) { return L<T> { E<T>(t, 1, 11) }; }
    template<typename T> L<T> blue_on_light_red(T t) { return L<T> { E<T>(t, 1, 12) }; }
    template<typename T> L<T> blue_on_light_purple(T t) { return L<T> { E<T>(t, 1, 13) }; }
    template<typename T> L<T> blue_on_light_magenta(T t) { return L<T> { E<T>(t, 1, 13) }; }
    template<typename T> L<T> blue_on_light_yellow(T t) { return L<T> { E<T>(t, 1, 14) }; }
    template<typename T> L<T> blue_on_bright_white(T t) { return L<T> { E<T>(t, 1, 15) }; }
    template<typename T> L<T> green_on_black(T t) { return L<T> { E<T>(t, 2, 0) }; }
    template<typename T> L<T> green_on_blue(T t) { return L<T> { E<T>(t, 2, 1) }; }
    template<typename T> L<T> green_on_green(T t) { return L<T> { E<T>(t, 2, 2) }; }
    template<typename T> L<T> green_on_aqua(T t) { return L<T> { E<T>(t, 2, 3) }; }
    template<typename T> L<T> green_on_cyan(T t) { return L<T> { E<T>(t, 2, 3) }; }
    template<typename T> L<T> green_on_red(T t) { return L<T> { E<T>(t, 2, 4) }; }
    template<typename T> L<T> green_on_purple(T t) { return L<T> { E<T>(t, 2, 5) }; }
    template<typename T> L<T> green_on_magenta(T t) { return L<T> { E<T>(t, 2, 5) }; }
    template<typename T> L<T> green_on_yellow(T t) { return L<T> { E<T>(t, 2, 6) }; }
    template<typename T> L<T> green_on_white(T t) { return L<T> { E<T>(t, 2, 7) }; }
    template<typename T> L<T> green_on_gray(T t) { return L<T> { E<T>(t, 2, 8) }; }
    template<typename T> L<T> green_on_light_blue(T t) { return L<T> { E<T>(t, 2, 9) }; }
    template<typename T> L<T> green_on_light_green(T t) { return L<T> { E<T>(t, 2, 10) }; }
    template<typename T> L<T> green_on_light_aqua(T t) { return L<T> { E<T>(t, 2, 11) }; }
    template<typename T> L<T> green_on_light_cyan(T t) { return L<T> { E<T>(t, 2, 11) }; }
    template<typename T> L<T> green_on_light_red(T t) { return L<T> { E<T>(t, 2, 12) }; }
    template<typename T> L<T> green_on_light_purple(T t) { return L<T> { E<T>(t, 2, 13) }; }
    template<typename T> L<T> green_on_light_magenta(T t) { return L<T> { E<T>(t, 2, 13) }; }
    template<typename T> L<T> green_on_light_yellow(T t) { return L<T> { E<T>(t, 2, 14) }; }
    template<typename T> L<T> green_on_bright_white(T t) { return L<T> { E<T>(t, 2, 15) }; }
    template<typename T> L<T> aqua_on_black(T t) { return L<T> { E<T>(t, 3, 0) }; }
    template<typename T> L<T> aqua_on_blue(T t) { return L<T> { E<T>(t, 3, 1) }; }
    template<typename T> L<T> aqua_on_green(T t) { return L<T> { E<T>(t, 3, 2) }; }
    template<typename T> L<T> aqua_on_aqua(T t) { return L<T> { E<T>(t, 3, 3) }; }
    template<typename T> L<T> aqua_on_cyan(T t) { return L<T> { E<T>(t, 3, 3) }; }
    template<typename T> L<T> aqua_on_red(T t) { return L<T> { E<T>(t, 3, 4) }; }
    template<typename T> L<T> aqua_on_purple(T t) { return L<T> { E<T>(t, 3, 5) }; }
    template<typename T> L<T> aqua_on_magenta(T t) { return L<T> { E<T>(t, 3, 5) }; }
    template<typename T> L<T> aqua_on_yellow(T t) { return L<T> { E<T>(t, 3, 6) }; }
    template<typename T> L<T> aqua_on_white(T t) { return L<T> { E<T>(t, 3, 7) }; }
    template<typename T> L<T> aqua_on_gray(T t) { return L<T> { E<T>(t, 3, 8) }; }
    template<typename T> L<T> aqua_on_light_blue(T t) { return L<T> { E<T>(t, 3, 9) }; }
    template<typename T> L<T> aqua_on_light_green(T t) { return L<T> { E<T>(t, 3, 10) }; }
    template<typename T> L<T> aqua_on_light_aqua(T t) { return L<T> { E<T>(t, 3, 11) }; }
    template<typename T> L<T> aqua_on_light_cyan(T t) { return L<T> { E<T>(t, 3, 11) }; }
    template<typename T> L<T> aqua_on_light_red(T t) { return L<T> { E<T>(t, 3, 12) }; }
    template<typename T> L<T> aqua_on_light_purple(T t) { return L<T> { E<T>(t, 3, 13) }; }
    template<typename T> L<T> aqua_on_light_magenta(T t) { return L<T> { E<T>(t, 3, 13) }; }
    template<typename T> L<T> aqua_on_light_yellow(T t) { return L<T> { E<T>(t, 3, 14) }; }
    template<typename T> L<T> aqua_on_bright_white(T t) { return L<T> { E<T>(t, 3, 15) }; }
    template<typename T> L<T> cyan_on_black(T t) { return L<T> { E<T>(t, 3, 0) }; }
    template<typename T> L<T> cyan_on_blue(T t) { return L<T> { E<T>(t, 3, 1) }; }
    template<typename T> L<T> cyan_on_green(T t) { return L<T> { E<T>(t, 3, 2) }; }
    template<typename T> L<T> cyan_on_aqua(T t) { return L<T> { E<T>(t, 3, 3) }; }
    template<typename T> L<T> cyan_on_cyan(T t) { return L<T> { E<T>(t, 3, 3) }; }
    template<typename T> L<T> cyan_on_red(T t) { return L<T> { E<T>(t, 3, 4) }; }
    template<typename T> L<T> cyan_on_purple(T t) { return L<T> { E<T>(t, 3, 5) }; }
    template<typename T> L<T> cyan_on_magenta(T t) { return L<T> { E<T>(t, 3, 5) }; }
    template<typename T> L<T> cyan_on_yellow(T t) { return L<T> { E<T>(t, 3, 6) }; }
    template<typename T> L<T> cyan_on_white(T t) { return L<T> { E<T>(t, 3, 7) }; }
    template<typename T> L<T> cyan_on_gray(T t) { return L<T> { E<T>(t, 3, 8) }; }
    template<typename T> L<T> cyan_on_light_blue(T t) { return L<T> { E<T>(t, 3, 9) }; }
    template<typename T> L<T> cyan_on_light_green(T t) { return L<T> { E<T>(t, 3, 10) }; }
    template<typename T> L<T> cyan_on_light_aqua(T t) { return L<T> { E<T>(t, 3, 11) }; }
    template<typename T> L<T> cyan_on_light_cyan(T t) { return L<T> { E<T>(t, 3, 11) }; }
    template<typename T> L<T> cyan_on_light_red(T t) { return L<T> { E<T>(t, 3, 12) }; }
    template<typename T> L<T> cyan_on_light_purple(T t) { return L<T> { E<T>(t, 3, 13) }; }
    template<typename T> L<T> cyan_on_light_magenta(T t) { return L<T> { E<T>(t, 3, 13) }; }
    template<typename T> L<T> cyan_on_light_yellow(T t) { return L<T> { E<T>(t, 3, 14) }; }
    template<typename T> L<T> cyan_on_bright_white(T t) { return L<T> { E<T>(t, 3, 15) }; }
    template<typename T> L<T> red_on_black(T t) { return L<T> { E<T>(t, 4, 0) }; }
    template<typename T> L<T> red_on_blue(T t) { return L<T> { E<T>(t, 4, 1) }; }
    template<typename T> L<T> red_on_green(T t) { return L<T> { E<T>(t, 4, 2) }; }
    template<typename T> L<T> red_on_aqua(T t) { return L<T> { E<T>(t, 4, 3) }; }
    template<typename T> L<T> red_on_cyan(T t) { return L<T> { E<T>(t, 4, 3) }; }
    template<typename T> L<T> red_on_red(T t) { return L<T> { E<T>(t, 4, 4) }; }
    template<typename T> L<T> red_on_purple(T t) { return L<T> { E<T>(t, 4, 5) }; }
    template<typename T> L<T> red_on_magenta(T t) { return L<T> { E<T>(t, 4, 5) }; }
    template<typename T> L<T> red_on_yellow(T t) { return L<T> { E<T>(t, 4, 6) }; }
    template<typename T> L<T> red_on_white(T t) { return L<T> { E<T>(t, 4, 7) }; }
    template<typename T> L<T> red_on_gray(T t) { return L<T> { E<T>(t, 4, 8) }; }
    template<typename T> L<T> red_on_light_blue(T t) { return L<T> { E<T>(t, 4, 9) }; }
    template<typename T> L<T> red_on_light_green(T t) { return L<T> { E<T>(t, 4, 10) }; }
    template<typename T> L<T> red_on_light_aqua(T t) { return L<T> { E<T>(t, 4, 11) }; }
    template<typename T> L<T> red_on_light_cyan(T t) { return L<T> { E<T>(t, 4, 11) }; }
    template<typename T> L<T> red_on_light_red(T t) { return L<T> { E<T>(t, 4, 12) }; }
    template<typename T> L<T> red_on_light_purple(T t) { return L<T> { E<T>(t, 4, 13) }; }
    template<typename T> L<T> red_on_light_magenta(T t) { return L<T> { E<T>(t, 4, 13) }; }
    template<typename T> L<T> red_on_light_yellow(T t) { return L<T> { E<T>(t, 4, 14) }; }
    template<typename T> L<T> red_on_bright_white(T t) { return L<T> { E<T>(t, 4, 15) }; }
    template<typename T> L<T> purple_on_black(T t) { return L<T> { E<T>(t, 5, 0) }; }
    template<typename T> L<T> purple_on_blue(T t) { return L<T> { E<T>(t, 5, 1) }; }
    template<typename T> L<T> purple_on_green(T t) { return L<T> { E<T>(t, 5, 2) }; }
    template<typename T> L<T> purple_on_aqua(T t) { return L<T> { E<T>(t, 5, 3) }; }
    template<typename T> L<T> purple_on_cyan(T t) { return L<T> { E<T>(t, 5, 3) }; }
    template<typename T> L<T> purple_on_red(T t) { return L<T> { E<T>(t, 5, 4) }; }
    template<typename T> L<T> purple_on_purple(T t) { return L<T> { E<T>(t, 5, 5) }; }
    template<typename T> L<T> purple_on_magenta(T t) { return L<T> { E<T>(t, 5, 5) }; }
    template<typename T> L<T> purple_on_yellow(T t) { return L<T> { E<T>(t, 5, 6) }; }
    template<typename T> L<T> purple_on_white(T t) { return L<T> { E<T>(t, 5, 7) }; }
    template<typename T> L<T> purple_on_gray(T t) { return L<T> { E<T>(t, 5, 8) }; }
    template<typename T> L<T> purple_on_light_blue(T t) { return L<T> { E<T>(t, 5, 9) }; }
    template<typename T> L<T> purple_on_light_green(T t) { return L<T> { E<T>(t, 5, 10) }; }
    template<typename T> L<T> purple_on_light_aqua(T t) { return L<T> { E<T>(t, 5, 11) }; }
    template<typename T> L<T> purple_on_light_cyan(T t) { return L<T> { E<T>(t, 5, 11) }; }
    template<typename T> L<T> purple_on_light_red(T t) { return L<T> { E<T>(t, 5, 12) }; }
    template<typename T> L<T> purple_on_light_purple(T t) { return L<T> { E<T>(t, 5, 13) }; }
    template<typename T> L<T> purple_on_light_magenta(T t) { return L<T> { E<T>(t, 5, 13) }; }
    template<typename T> L<T> purple_on_light_yellow(T t) { return L<T> { E<T>(t, 5, 14) }; }
    template<typename T> L<T> purple_on_bright_white(T t) { return L<T> { E<T>(t, 5, 15) }; }
    template<typename T> L<T> magenta_on_black(T t) { return L<T> { E<T>(t, 5, 0) }; }
    template<typename T> L<T> magenta_on_blue(T t) { return L<T> { E<T>(t, 5, 1) }; }
    template<typename T> L<T> magenta_on_green(T t) { return L<T> { E<T>(t, 5, 2) }; }
    template<typename T> L<T> magenta_on_aqua(T t) { return L<T> { E<T>(t, 5, 3) }; }
    template<typename T> L<T> magenta_on_cyan(T t) { return L<T> { E<T>(t, 5, 3) }; }
    template<typename T> L<T> magenta_on_red(T t) { return L<T> { E<T>(t, 5, 4) }; }
    template<typename T> L<T> magenta_on_purple(T t) { return L<T> { E<T>(t, 5, 5) }; }
    template<typename T> L<T> magenta_on_magenta(T t) { return L<T> { E<T>(t, 5, 5) }; }
    template<typename T> L<T> magenta_on_yellow(T t) { return L<T> { E<T>(t, 5, 6) }; }
    template<typename T> L<T> magenta_on_white(T t) { return L<T> { E<T>(t, 5, 7) }; }
    template<typename T> L<T> magenta_on_gray(T t) { return L<T> { E<T>(t, 5, 8) }; }
    template<typename T> L<T> magenta_on_light_blue(T t) { return L<T> { E<T>(t, 5, 9) }; }
    template<typename T> L<T> magenta_on_light_green(T t) { return L<T> { E<T>(t, 5, 10) }; }
    template<typename T> L<T> magenta_on_light_aqua(T t) { return L<T> { E<T>(t, 5, 11) }; }
    template<typename T> L<T> magenta_on_light_cyan(T t) { return L<T> { E<T>(t, 5, 11) }; }
    template<typename T> L<T> magenta_on_light_red(T t) { return L<T> { E<T>(t, 5, 12) }; }
    template<typename T> L<T> magenta_on_light_purple(T t) { return L<T> { E<T>(t, 5, 13) }; }
    template<typename T> L<T> magenta_on_light_magenta(T t) { return L<T> { E<T>(t, 5, 13) }; }
    template<typename T> L<T> magenta_on_light_yellow(T t) { return L<T> { E<T>(t, 5, 14) }; }
    template<typename T> L<T> magenta_on_bright_white(T t) { return L<T> { E<T>(t, 5, 15) }; }
    template<typename T> L<T> yellow_on_black(T t) { return L<T> { E<T>(t, 6, 0) }; }
    template<typename T> L<T> yellow_on_blue(T t) { return L<T> { E<T>(t, 6, 1) }; }
    template<typename T> L<T> yellow_on_green(T t) { return L<T> { E<T>(t, 6, 2) }; }
    template<typename T> L<T> yellow_on_aqua(T t) { return L<T> { E<T>(t, 6, 3) }; }
    template<typename T> L<T> yellow_on_cyan(T t) { return L<T> { E<T>(t, 6, 3) }; }
    template<typename T> L<T> yellow_on_red(T t) { return L<T> { E<T>(t, 6, 4) }; }
    template<typename T> L<T> yellow_on_purple(T t) { return L<T> { E<T>(t, 6, 5) }; }
    template<typename T> L<T> yellow_on_magenta(T t) { return L<T> { E<T>(t, 6, 5) }; }
    template<typename T> L<T> yellow_on_yellow(T t) { return L<T> { E<T>(t, 6, 6) }; }
    template<typename T> L<T> yellow_on_white(T t) { return L<T> { E<T>(t, 6, 7) }; }
    template<typename T> L<T> yellow_on_gray(T t) { return L<T> { E<T>(t, 6, 8) }; }
    template<typename T> L<T> yellow_on_light_blue(T t) { return L<T> { E<T>(t, 6, 9) }; }
    template<typename T> L<T> yellow_on_light_green(T t) { return L<T> { E<T>(t, 6, 10) }; }
    template<typename T> L<T> yellow_on_light_aqua(T t) { return L<T> { E<T>(t, 6, 11) }; }
    template<typename T> L<T> yellow_on_light_cyan(T t) { return L<T> { E<T>(t, 6, 11) }; }
    template<typename T> L<T> yellow_on_light_red(T t) { return L<T> { E<T>(t, 6, 12) }; }
    template<typename T> L<T> yellow_on_light_purple(T t) { return L<T> { E<T>(t, 6, 13) }; }
    template<typename T> L<T> yellow_on_light_magenta(T t) { return L<T> { E<T>(t, 6, 13) }; }
    template<typename T> L<T> yellow_on_light_yellow(T t) { return L<T> { E<T>(t, 6, 14) }; }
    template<typename T> L<T> yellow_on_bright_white(T t) { return L<T> { E<T>(t, 6, 15) }; }
    template<typename T> L<T> white_on_black(T t) { return L<T> { E<T>(t, 7, 0) }; }
    template<typename T> L<T> white_on_blue(T t) { return L<T> { E<T>(t, 7, 1) }; }
    template<typename T> L<T> white_on_green(T t) { return L<T> { E<T>(t, 7, 2) }; }
    template<typename T> L<T> white_on_aqua(T t) { return L<T> { E<T>(t, 7, 3) }; }
    template<typename T> L<T> white_on_cyan(T t) { return L<T> { E<T>(t, 7, 3) }; }
    template<typename T> L<T> white_on_red(T t) { return L<T> { E<T>(t, 7, 4) }; }
    template<typename T> L<T> white_on_purple(T t) { return L<T> { E<T>(t, 7, 5) }; }
    template<typename T> L<T> white_on_magenta(T t) { return L<T> { E<T>(t, 7, 5) }; }
    template<typename T> L<T> white_on_yellow(T t) { return L<T> { E<T>(t, 7, 6) }; }
    template<typename T> L<T> white_on_white(T t) { return L<T> { E<T>(t, 7, 7) }; }
    template<typename T> L<T> white_on_gray(T t) { return L<T> { E<T>(t, 7, 8) }; }
    template<typename T> L<T> white_on_light_blue(T t) { return L<T> { E<T>(t, 7, 9) }; }
    template<typename T> L<T> white_on_light_green(T t) { return L<T> { E<T>(t, 7, 10) }; }
    template<typename T> L<T> white_on_light_aqua(T t) { return L<T> { E<T>(t, 7, 11) }; }
    template<typename T> L<T> white_on_light_cyan(T t) { return L<T> { E<T>(t, 7, 11) }; }
    template<typename T> L<T> white_on_light_red(T t) { return L<T> { E<T>(t, 7, 12) }; }
    template<typename T> L<T> white_on_light_purple(T t) { return L<T> { E<T>(t, 7, 13) }; }
    template<typename T> L<T> white_on_light_magenta(T t) { return L<T> { E<T>(t, 7, 13) }; }
    template<typename T> L<T> white_on_light_yellow(T t) { return L<T> { E<T>(t, 7, 14) }; }
    template<typename T> L<T> white_on_bright_white(T t) { return L<T> { E<T>(t, 7, 15) }; }
    template<typename T> L<T> gray_on_black(T t) { return L<T> { E<T>(t, 8, 0) }; }
    template<typename T> L<T> gray_on_blue(T t) { return L<T> { E<T>(t, 8, 1) }; }
    template<typename T> L<T> gray_on_green(T t) { return L<T> { E<T>(t, 8, 2) }; }
    template<typename T> L<T> gray_on_aqua(T t) { return L<T> { E<T>(t, 8, 3) }; }
    template<typename T> L<T> gray_on_cyan(T t) { return L<T> { E<T>(t, 8, 3) }; }
    template<typename T> L<T> gray_on_red(T t) { return L<T> { E<T>(t, 8, 4) }; }
    template<typename T> L<T> gray_on_purple(T t) { return L<T> { E<T>(t, 8, 5) }; }
    template<typename T> L<T> gray_on_magenta(T t) { return L<T> { E<T>(t, 8, 5) }; }
    template<typename T> L<T> gray_on_yellow(T t) { return L<T> { E<T>(t, 8, 6) }; }
    template<typename T> L<T> gray_on_white(T t) { return L<T> { E<T>(t, 8, 7) }; }
    template<typename T> L<T> gray_on_gray(T t) { return L<T> { E<T>(t, 8, 8) }; }
    template<typename T> L<T> gray_on_light_blue(T t) { return L<T> { E<T>(t, 8, 9) }; }
    template<typename T> L<T> gray_on_light_green(T t) { return L<T> { E<T>(t, 8, 10) }; }
    template<typename T> L<T> gray_on_light_aqua(T t) { return L<T> { E<T>(t, 8, 11) }; }
    template<typename T> L<T> gray_on_light_cyan(T t) { return L<T> { E<T>(t, 8, 11) }; }
    template<typename T> L<T> gray_on_light_red(T t) { return L<T> { E<T>(t, 8, 12) }; }
    template<typename T> L<T> gray_on_light_purple(T t) { return L<T> { E<T>(t, 8, 13) }; }
    template<typename T> L<T> gray_on_light_magenta(T t) { return L<T> { E<T>(t, 8, 13) }; }
    template<typename T> L<T> gray_on_light_yellow(T t) { return L<T> { E<T>(t, 8, 14) }; }
    template<typename T> L<T> gray_on_bright_white(T t) { return L<T> { E<T>(t, 8, 15) }; }
    template<typename T> L<T> light_blue_on_black(T t) { return L<T> { E<T>(t, 9, 0) }; }
    template<typename T> L<T> light_blue_on_blue(T t) { return L<T> { E<T>(t, 9, 1) }; }
    template<typename T> L<T> light_blue_on_green(T t) { return L<T> { E<T>(t, 9, 2) }; }
    template<typename T> L<T> light_blue_on_aqua(T t) { return L<T> { E<T>(t, 9, 3) }; }
    template<typename T> L<T> light_blue_on_cyan(T t) { return L<T> { E<T>(t, 9, 3) }; }
    template<typename T> L<T> light_blue_on_red(T t) { return L<T> { E<T>(t, 9, 4) }; }
    template<typename T> L<T> light_blue_on_purple(T t) { return L<T> { E<T>(t, 9, 5) }; }
    template<typename T> L<T> light_blue_on_magenta(T t) { return L<T> { E<T>(t, 9, 5) }; }
    template<typename T> L<T> light_blue_on_yellow(T t) { return L<T> { E<T>(t, 9, 6) }; }
    template<typename T> L<T> light_blue_on_white(T t) { return L<T> { E<T>(t, 9, 7) }; }
    template<typename T> L<T> light_blue_on_gray(T t) { return L<T> { E<T>(t, 9, 8) }; }
    template<typename T> L<T> light_blue_on_light_blue(T t) { return L<T> { E<T>(t, 9, 9) }; }
    template<typename T> L<T> light_blue_on_light_green(T t) { return L<T> { E<T>(t, 9, 10) }; }
    template<typename T> L<T> light_blue_on_light_aqua(T t) { return L<T> { E<T>(t, 9, 11) }; }
    template<typename T> L<T> light_blue_on_light_cyan(T t) { return L<T> { E<T>(t, 9, 11) }; }
    template<typename T> L<T> light_blue_on_light_red(T t) { return L<T> { E<T>(t, 9, 12) }; }
    template<typename T> L<T> light_blue_on_light_purple(T t) { return L<T> { E<T>(t, 9, 13) }; }
    template<typename T> L<T> light_blue_on_light_magenta(T t) { return L<T> { E<T>(t, 9, 13) }; }
    template<typename T> L<T> light_blue_on_light_yellow(T t) { return L<T> { E<T>(t, 9, 14) }; }
    template<typename T> L<T> light_blue_on_bright_white(T t) { return L<T> { E<T>(t, 9, 15) }; }
    template<typename T> L<T> light_green_on_black(T t) { return L<T> { E<T>(t, 10, 0) }; }
    template<typename T> L<T> light_green_on_blue(T t) { return L<T> { E<T>(t, 10, 1) }; }
    template<typename T> L<T> light_green_on_green(T t) { return L<T> { E<T>(t, 10, 2) }; }
    template<typename T> L<T> light_green_on_aqua(T t) { return L<T> { E<T>(t, 10, 3) }; }
    template<typename T> L<T> light_green_on_cyan(T t) { return L<T> { E<T>(t, 10, 3) }; }
    template<typename T> L<T> light_green_on_red(T t) { return L<T> { E<T>(t, 10, 4) }; }
    template<typename T> L<T> light_green_on_purple(T t) { return L<T> { E<T>(t, 10, 5) }; }
    template<typename T> L<T> light_green_on_magenta(T t) { return L<T> { E<T>(t, 10, 5) }; }
    template<typename T> L<T> light_green_on_yellow(T t) { return L<T> { E<T>(t, 10, 6) }; }
    template<typename T> L<T> light_green_on_white(T t) { return L<T> { E<T>(t, 10, 7) }; }
    template<typename T> L<T> light_green_on_gray(T t) { return L<T> { E<T>(t, 10, 8) }; }
    template<typename T> L<T> light_green_on_light_blue(T t) { return L<T> { E<T>(t, 10, 9) }; }
    template<typename T> L<T> light_green_on_light_green(T t) { return L<T> { E<T>(t, 10, 10) }; }
    template<typename T> L<T> light_green_on_light_aqua(T t) { return L<T> { E<T>(t, 10, 11) }; }
    template<typename T> L<T> light_green_on_light_cyan(T t) { return L<T> { E<T>(t, 10, 11) }; }
    template<typename T> L<T> light_green_on_light_red(T t) { return L<T> { E<T>(t, 10, 12) }; }
    template<typename T> L<T> light_green_on_light_purple(T t) { return L<T> { E<T>(t, 10, 13) }; }
    template<typename T> L<T> light_green_on_light_magenta(T t) { return L<T> { E<T>(t, 10, 13) }; }
    template<typename T> L<T> light_green_on_light_yellow(T t) { return L<T> { E<T>(t, 10, 14) }; }
    template<typename T> L<T> light_green_on_bright_white(T t) { return L<T> { E<T>(t, 10, 15) }; }
    template<typename T> L<T> light_aqua_on_black(T t) { return L<T> { E<T>(t, 11, 0) }; }
    template<typename T> L<T> light_aqua_on_blue(T t) { return L<T> { E<T>(t, 11, 1) }; }
    template<typename T> L<T> light_aqua_on_green(T t) { return L<T> { E<T>(t, 11, 2) }; }
    template<typename T> L<T> light_aqua_on_aqua(T t) { return L<T> { E<T>(t, 11, 3) }; }
    template<typename T> L<T> light_aqua_on_cyan(T t) { return L<T> { E<T>(t, 11, 3) }; }
    template<typename T> L<T> light_aqua_on_red(T t) { return L<T> { E<T>(t, 11, 4) }; }
    template<typename T> L<T> light_aqua_on_purple(T t) { return L<T> { E<T>(t, 11, 5) }; }
    template<typename T> L<T> light_aqua_on_magenta(T t) { return L<T> { E<T>(t, 11, 5) }; }
    template<typename T> L<T> light_aqua_on_yellow(T t) { return L<T> { E<T>(t, 11, 6) }; }
    template<typename T> L<T> light_aqua_on_white(T t) { return L<T> { E<T>(t, 11, 7) }; }
    template<typename T> L<T> light_aqua_on_gray(T t) { return L<T> { E<T>(t, 11, 8) }; }
    template<typename T> L<T> light_aqua_on_light_blue(T t) { return L<T> { E<T>(t, 11, 9) }; }
    template<typename T> L<T> light_aqua_on_light_green(T t) { return L<T> { E<T>(t, 11, 10) }; }
    template<typename T> L<T> light_aqua_on_light_aqua(T t) { return L<T> { E<T>(t, 11, 11) }; }
    template<typename T> L<T> light_aqua_on_light_cyan(T t) { return L<T> { E<T>(t, 11, 11) }; }
    template<typename T> L<T> light_aqua_on_light_red(T t) { return L<T> { E<T>(t, 11, 12) }; }
    template<typename T> L<T> light_aqua_on_light_purple(T t) { return L<T> { E<T>(t, 11, 13) }; }
    template<typename T> L<T> light_aqua_on_light_magenta(T t) { return L<T> { E<T>(t, 11, 13) }; }
    template<typename T> L<T> light_aqua_on_light_yellow(T t) { return L<T> { E<T>(t, 11, 14) }; }
    template<typename T> L<T> light_aqua_on_bright_white(T t) { return L<T> { E<T>(t, 11, 15) }; }
    template<typename T> L<T> light_cyan_on_black(T t) { return L<T> { E<T>(t, 11, 0) }; }
    template<typename T> L<T> light_cyan_on_blue(T t) { return L<T> { E<T>(t, 11, 1) }; }
    template<typename T> L<T> light_cyan_on_green(T t) { return L<T> { E<T>(t, 11, 2) }; }
    template<typename T> L<T> light_cyan_on_aqua(T t) { return L<T> { E<T>(t, 11, 3) }; }
    template<typename T> L<T> light_cyan_on_cyan(T t) { return L<T> { E<T>(t, 11, 3) }; }
    template<typename T> L<T> light_cyan_on_red(T t) { return L<T> { E<T>(t, 11, 4) }; }
    template<typename T> L<T> light_cyan_on_purple(T t) { return L<T> { E<T>(t, 11, 5) }; }
    template<typename T> L<T> light_cyan_on_magenta(T t) { return L<T> { E<T>(t, 11, 5) }; }
    template<typename T> L<T> light_cyan_on_yellow(T t) { return L<T> { E<T>(t, 11, 6) }; }
    template<typename T> L<T> light_cyan_on_white(T t) { return L<T> { E<T>(t, 11, 7) }; }
    template<typename T> L<T> light_cyan_on_gray(T t) { return L<T> { E<T>(t, 11, 8) }; }
    template<typename T> L<T> light_cyan_on_light_blue(T t) { return L<T> { E<T>(t, 11, 9) }; }
    template<typename T> L<T> light_cyan_on_light_green(T t) { return L<T> { E<T>(t, 11, 10) }; }
    template<typename T> L<T> light_cyan_on_light_aqua(T t) { return L<T> { E<T>(t, 11, 11) }; }
    template<typename T> L<T> light_cyan_on_light_cyan(T t) { return L<T> { E<T>(t, 11, 11) }; }
    template<typename T> L<T> light_cyan_on_light_red(T t) { return L<T> { E<T>(t, 11, 12) }; }
    template<typename T> L<T> light_cyan_on_light_purple(T t) { return L<T> { E<T>(t, 11, 13) }; }
    template<typename T> L<T> light_cyan_on_light_magenta(T t) { return L<T> { E<T>(t, 11, 13) }; }
    template<typename T> L<T> light_cyan_on_light_yellow(T t) { return L<T> { E<T>(t, 11, 14) }; }
    template<typename T> L<T> light_cyan_on_bright_white(T t) { return L<T> { E<T>(t, 11, 15) }; }
    template<typename T> L<T> light_red_on_black(T t) { return L<T> { E<T>(t, 12, 0) }; }
    template<typename T> L<T> light_red_on_blue(T t) { return L<T> { E<T>(t, 12, 1) }; }
    template<typename T> L<T> light_red_on_green(T t) { return L<T> { E<T>(t, 12, 2) }; }
    template<typename T> L<T> light_red_on_aqua(T t) { return L<T> { E<T>(t, 12, 3) }; }
    template<typename T> L<T> light_red_on_cyan(T t) { return L<T> { E<T>(t, 12, 3) }; }
    template<typename T> L<T> light_red_on_red(T t) { return L<T> { E<T>(t, 12, 4) }; }
    template<typename T> L<T> light_red_on_purple(T t) { return L<T> { E<T>(t, 12, 5) }; }
    template<typename T> L<T> light_red_on_magenta(T t) { return L<T> { E<T>(t, 12, 5) }; }
    template<typename T> L<T> light_red_on_yellow(T t) { return L<T> { E<T>(t, 12, 6) }; }
    template<typename T> L<T> light_red_on_white(T t) { return L<T> { E<T>(t, 12, 7) }; }
    template<typename T> L<T> light_red_on_gray(T t) { return L<T> { E<T>(t, 12, 8) }; }
    template<typename T> L<T> light_red_on_light_blue(T t) { return L<T> { E<T>(t, 12, 9) }; }
    template<typename T> L<T> light_red_on_light_green(T t) { return L<T> { E<T>(t, 12, 10) }; }
    template<typename T> L<T> light_red_on_light_aqua(T t) { return L<T> { E<T>(t, 12, 11) }; }
    template<typename T> L<T> light_red_on_light_cyan(T t) { return L<T> { E<T>(t, 12, 11) }; }
    template<typename T> L<T> light_red_on_light_red(T t) { return L<T> { E<T>(t, 12, 12) }; }
    template<typename T> L<T> light_red_on_light_purple(T t) { return L<T> { E<T>(t, 12, 13) }; }
    template<typename T> L<T> light_red_on_light_magenta(T t) { return L<T> { E<T>(t, 12, 13) }; }
    template<typename T> L<T> light_red_on_light_yellow(T t) { return L<T> { E<T>(t, 12, 14) }; }
    template<typename T> L<T> light_red_on_bright_white(T t) { return L<T> { E<T>(t, 12, 15) }; }
    template<typename T> L<T> light_purple_on_black(T t) { return L<T> { E<T>(t, 13, 0) }; }
    template<typename T> L<T> light_purple_on_blue(T t) { return L<T> { E<T>(t, 13, 1) }; }
    template<typename T> L<T> light_purple_on_green(T t) { return L<T> { E<T>(t, 13, 2) }; }
    template<typename T> L<T> light_purple_on_aqua(T t) { return L<T> { E<T>(t, 13, 3) }; }
    template<typename T> L<T> light_purple_on_cyan(T t) { return L<T> { E<T>(t, 13, 3) }; }
    template<typename T> L<T> light_purple_on_red(T t) { return L<T> { E<T>(t, 13, 4) }; }
    template<typename T> L<T> light_purple_on_purple(T t) { return L<T> { E<T>(t, 13, 5) }; }
    template<typename T> L<T> light_purple_on_magenta(T t) { return L<T> { E<T>(t, 13, 5) }; }
    template<typename T> L<T> light_purple_on_yellow(T t) { return L<T> { E<T>(t, 13, 6) }; }
    template<typename T> L<T> light_purple_on_white(T t) { return L<T> { E<T>(t, 13, 7) }; }
    template<typename T> L<T> light_purple_on_gray(T t) { return L<T> { E<T>(t, 13, 8) }; }
    template<typename T> L<T> light_purple_on_light_blue(T t) { return L<T> { E<T>(t, 13, 9) }; }
    template<typename T> L<T> light_purple_on_light_green(T t) { return L<T> { E<T>(t, 13, 10) }; }
    template<typename T> L<T> light_purple_on_light_aqua(T t) { return L<T> { E<T>(t, 13, 11) }; }
    template<typename T> L<T> light_purple_on_light_cyan(T t) { return L<T> { E<T>(t, 13, 11) }; }
    template<typename T> L<T> light_purple_on_light_red(T t) { return L<T> { E<T>(t, 13, 12) }; }
    template<typename T> L<T> light_purple_on_light_purple(T t) { return L<T> { E<T>(t, 13, 13) }; }
    template<typename T> L<T> light_purple_on_light_magenta(T t) { return L<T> { E<T>(t, 13, 13) }; }
    template<typename T> L<T> light_purple_on_light_yellow(T t) { return L<T> { E<T>(t, 13, 14) }; }
    template<typename T> L<T> light_purple_on_bright_white(T t) { return L<T> { E<T>(t, 13, 15) }; }
    template<typename T> L<T> light_magenta_on_black(T t) { return L<T> { E<T>(t, 13, 0) }; }
    template<typename T> L<T> light_magenta_on_blue(T t) { return L<T> { E<T>(t, 13, 1) }; }
    template<typename T> L<T> light_magenta_on_green(T t) { return L<T> { E<T>(t, 13, 2) }; }
    template<typename T> L<T> light_magenta_on_aqua(T t) { return L<T> { E<T>(t, 13, 3) }; }
    template<typename T> L<T> light_magenta_on_cyan(T t) { return L<T> { E<T>(t, 13, 3) }; }
    template<typename T> L<T> light_magenta_on_red(T t) { return L<T> { E<T>(t, 13, 4) }; }
    template<typename T> L<T> light_magenta_on_purple(T t) { return L<T> { E<T>(t, 13, 5) }; }
    template<typename T> L<T> light_magenta_on_magenta(T t) { return L<T> { E<T>(t, 13, 5) }; }
    template<typename T> L<T> light_magenta_on_yellow(T t) { return L<T> { E<T>(t, 13, 6) }; }
    template<typename T> L<T> light_magenta_on_white(T t) { return L<T> { E<T>(t, 13, 7) }; }
    template<typename T> L<T> light_magenta_on_gray(T t) { return L<T> { E<T>(t, 13, 8) }; }
    template<typename T> L<T> light_magenta_on_light_blue(T t) { return L<T> { E<T>(t, 13, 9) }; }
    template<typename T> L<T> light_magenta_on_light_green(T t) { return L<T> { E<T>(t, 13, 10) }; }
    template<typename T> L<T> light_magenta_on_light_aqua(T t) { return L<T> { E<T>(t, 13, 11) }; }
    template<typename T> L<T> light_magenta_on_light_cyan(T t) { return L<T> { E<T>(t, 13, 11) }; }
    template<typename T> L<T> light_magenta_on_light_red(T t) { return L<T> { E<T>(t, 13, 12) }; }
    template<typename T> L<T> light_magenta_on_light_purple(T t) { return L<T> { E<T>(t, 13, 13) }; }
    template<typename T> L<T> light_magenta_on_light_magenta(T t) { return L<T> { E<T>(t, 13, 13) }; }
    template<typename T> L<T> light_magenta_on_light_yellow(T t) { return L<T> { E<T>(t, 13, 14) }; }
    template<typename T> L<T> light_magenta_on_bright_white(T t) { return L<T> { E<T>(t, 13, 15) }; }
    template<typename T> L<T> light_yellow_on_black(T t) { return L<T> { E<T>(t, 14, 0) }; }
    template<typename T> L<T> light_yellow_on_blue(T t) { return L<T> { E<T>(t, 14, 1) }; }
    template<typename T> L<T> light_yellow_on_green(T t) { return L<T> { E<T>(t, 14, 2) }; }
    template<typename T> L<T> light_yellow_on_aqua(T t) { return L<T> { E<T>(t, 14, 3) }; }
    template<typename T> L<T> light_yellow_on_cyan(T t) { return L<T> { E<T>(t, 14, 3) }; }
    template<typename T> L<T> light_yellow_on_red(T t) { return L<T> { E<T>(t, 14, 4) }; }
    template<typename T> L<T> light_yellow_on_purple(T t) { return L<T> { E<T>(t, 14, 5) }; }
    template<typename T> L<T> light_yellow_on_magenta(T t) { return L<T> { E<T>(t, 14, 5) }; }
    template<typename T> L<T> light_yellow_on_yellow(T t) { return L<T> { E<T>(t, 14, 6) }; }
    template<typename T> L<T> light_yellow_on_white(T t) { return L<T> { E<T>(t, 14, 7) }; }
    template<typename T> L<T> light_yellow_on_gray(T t) { return L<T> { E<T>(t, 14, 8) }; }
    template<typename T> L<T> light_yellow_on_light_blue(T t) { return L<T> { E<T>(t, 14, 9) }; }
    template<typename T> L<T> light_yellow_on_light_green(T t) { return L<T> { E<T>(t, 14, 10) }; }
    template<typename T> L<T> light_yellow_on_light_aqua(T t) { return L<T> { E<T>(t, 14, 11) }; }
    template<typename T> L<T> light_yellow_on_light_cyan(T t) { return L<T> { E<T>(t, 14, 11) }; }
    template<typename T> L<T> light_yellow_on_light_red(T t) { return L<T> { E<T>(t, 14, 12) }; }
    template<typename T> L<T> light_yellow_on_light_purple(T t) { return L<T> { E<T>(t, 14, 13) }; }
    template<typename T> L<T> light_yellow_on_light_magenta(T t) { return L<T> { E<T>(t, 14, 13) }; }
    template<typename T> L<T> light_yellow_on_light_yellow(T t) { return L<T> { E<T>(t, 14, 14) }; }
    template<typename T> L<T> light_yellow_on_bright_white(T t) { return L<T> { E<T>(t, 14, 15) }; }
    template<typename T> L<T> bright_white_on_black(T t) { return L<T> { E<T>(t, 15, 0) }; }
    template<typename T> L<T> bright_white_on_blue(T t) { return L<T> { E<T>(t, 15, 1) }; }
    template<typename T> L<T> bright_white_on_green(T t) { return L<T> { E<T>(t, 15, 2) }; }
    template<typename T> L<T> bright_white_on_aqua(T t) { return L<T> { E<T>(t, 15, 3) }; }
    template<typename T> L<T> bright_white_on_cyan(T t) { return L<T> { E<T>(t, 15, 3) }; }
    template<typename T> L<T> bright_white_on_red(T t) { return L<T> { E<T>(t, 15, 4) }; }
    template<typename T> L<T> bright_white_on_purple(T t) { return L<T> { E<T>(t, 15, 5) }; }
    template<typename T> L<T> bright_white_on_magenta(T t) { return L<T> { E<T>(t, 15, 5) }; }
    template<typename T> L<T> bright_white_on_yellow(T t) { return L<T> { E<T>(t, 15, 6) }; }
    template<typename T> L<T> bright_white_on_white(T t) { return L<T> { E<T>(t, 15, 7) }; }
    template<typename T> L<T> bright_white_on_gray(T t) { return L<T> { E<T>(t, 15, 8) }; }
    template<typename T> L<T> bright_white_on_light_blue(T t) { return L<T> { E<T>(t, 15, 9) }; }
    template<typename T> L<T> bright_white_on_light_green(T t) { return L<T> { E<T>(t, 15, 10) }; }
    template<typename T> L<T> bright_white_on_light_aqua(T t) { return L<T> { E<T>(t, 15, 11) }; }
    template<typename T> L<T> bright_white_on_light_cyan(T t) { return L<T> { E<T>(t, 15, 11) }; }
    template<typename T> L<T> bright_white_on_light_red(T t) { return L<T> { E<T>(t, 15, 12) }; }
    template<typename T> L<T> bright_white_on_light_purple(T t) { return L<T> { E<T>(t, 15, 13) }; }
    template<typename T> L<T> bright_white_on_light_magenta(T t) { return L<T> { E<T>(t, 15, 13) }; }
    template<typename T> L<T> bright_white_on_light_yellow(T t) { return L<T> { E<T>(t, 15, 14) }; }
    template<typename T> L<T> bright_white_on_bright_white(T t) { return L<T> { E<T>(t, 15, 15) }; }
}
// ---------- ↑↑ dye namespace ↑↑ -------------------------------------------------------------------------------------
#endif
