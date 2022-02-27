#ifndef COLOR_HPP
#define COLOR_HPP

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <type_traits>
#include <utility>
#include <windows.h>

namespace hue
{
    constexpr int DEFAULT_COLOR = 7;
    constexpr int BAD_COLOR = -256;

    static const std::map<std::string, int> CODES = {
        {"black",            0}, {"k",   0},
        {"blue",             1}, {"b",   1},
        {"green",            2}, {"g",   2},
        {"aqua",             3}, {"a",   3},
        {"red",              4}, {"r",   4},
        {"purple",           5}, {"p",   5},
        {"yellow",           6}, {"y",   6},
        {"white",            7}, {"w",   7},
        {"grey",             8}, {"e",   8},
        {"light blue",       9}, {"lb",  9},
        {"light green",     10}, {"lg", 10},
        {"light aqua",      11}, {"la", 11},
        {"light red",       12}, {"lr", 12},
        {"light purple",    13}, {"lp", 13},
        {"light yellow",    14}, {"ly", 14},
        {"bright white",    15}, {"bw", 15}
    };

    static const std::map<int, std::string> NAMES = {
        { 0, "black"},
        { 1, "blue"},
        { 2, "green"},
        { 3, "aqua"},
        { 4, "red"},
        { 5, "purple"},
        { 6, "yellow"},
        { 7, "white"},
        { 8, "grey"},
        { 9, "light blue"},
        {10, "light green"},
        {11, "light aqua"},
        {12, "light red"},
        {13, "light purple"},
        {14, "light yellow"},
        {15, "bright white"}
    };

    inline bool is_good(int c)
    {
        return 0 <= c && c < 256;
    }

    inline int itoc(int c)
    {
        return is_good(c) ? c : BAD_COLOR;
    }

    inline int itoc(int a, int b)
    {
        return itoc(a + b * 16);
    }

    // std::string to color
    static int stoc(std::string a)
    {
        // convert s to lowercase, and format variants like  "light_blue"
        std::transform(a.begin(), a.end(), a.begin(), [](char c)
                       {
                            if ('A' <= c && c <= 'Z')
                                c = c - 'A' + 'a';
                            else if (c == '_' || c == '-')
                                c = ' ';
                            return c;
                        });

        // operator[] on std::map is non-const, use std::map::at instead
        return (CODES.find(a) != CODES.end()) ? CODES.at(a) : BAD_COLOR;
    }

    static int stoc(std::string a, std::string b)
    {
        return itoc(stoc(a), stoc(b));
    }

    static std::string ctos(int c)
    {
        return (0 <= c && c < 256) ?
               "(text) " + NAMES.at(c % 16) + " + " +
               "(background) " + NAMES.at(c / 16) :
               "BAD COLOR";
    }

    static int get()
    {
        CONSOLE_SCREEN_BUFFER_INFO i;
        return GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &i) ?
               i.wAttributes : BAD_COLOR;
    }

    static int get_text()
    {
        return (get() != BAD_COLOR) ? get() % 16 : BAD_COLOR;
    }

    static int get_background()
    {
        return (get() != BAD_COLOR) ? get() / 16 : BAD_COLOR;
    }

    static void set(int c)
    {
        if (is_good(c))
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
    }

    static void set(int a, int b)
    {
        set(a + b * 16);
    }

    static void set(std::string a, std::string b)
    {
        set(stoc(a) + stoc(b) * 16);
    }

    static void set_text(std::string a)
    {
        set(stoc(a), get_background());
    }

    static void set_background(std::string b)
    {
        set(get_text(), stoc(b));
    }

    static void reset()
    {
        set(DEFAULT_COLOR);
    }

    static int invert(int c)
    {
        if (is_good(c)) {
            int a = c % 16;
            int b = c / 16;
            return b + a * 16;
        }
        else
            return BAD_COLOR;
    }

    inline std::ostream & reset(std::ostream & os)                 { reset();                return os; }
    inline std::ostream & black(std::ostream & os)                 { set_text("k");          return os; }
    inline std::ostream & blue(std::ostream & os)                  { set_text("b");          return os; }
    inline std::ostream & green(std::ostream & os)                 { set_text("g");          return os; }
    inline std::ostream & aqua(std::ostream & os)                  { set_text("a");          return os; }
    inline std::ostream & red(std::ostream & os)                   { set_text("r");          return os; }
    inline std::ostream & purple(std::ostream & os)                { set_text("p");          return os; }
    inline std::ostream & yellow(std::ostream & os)                { set_text("y");          return os; }
    inline std::ostream & white(std::ostream & os)                 { set_text("w");          return os; }
    inline std::ostream & grey(std::ostream & os)                  { set_text("e");          return os; }
    inline std::ostream & light_blue(std::ostream & os)            { set_text("lb");         return os; }
    inline std::ostream & light_green(std::ostream & os)           { set_text("lg");         return os; }
    inline std::ostream & light_aqua(std::ostream & os)            { set_text("la");         return os; }
    inline std::ostream & light_red(std::ostream & os)             { set_text("lr");         return os; }
    inline std::ostream & light_purple(std::ostream & os)          { set_text("lp");         return os; }
    inline std::ostream & light_yellow(std::ostream & os)          { set_text("ly");         return os; }
    inline std::ostream & bright_white(std::ostream & os)          { set_text("bw");         return os; }
    inline std::ostream & on_black(std::ostream & os)              { set_background("k");    return os; }
    inline std::ostream & on_blue(std::ostream & os)               { set_background("b");    return os; }
    inline std::ostream & on_green(std::ostream & os)              { set_background("g");    return os; }
    inline std::ostream & on_aqua(std::ostream & os)               { set_background("a");    return os; }
    inline std::ostream & on_red(std::ostream & os)                { set_background("r");    return os; }
    inline std::ostream & on_purple(std::ostream & os)             { set_background("p");    return os; }
    inline std::ostream & on_yellow(std::ostream & os)             { set_background("y");    return os; }
    inline std::ostream & on_white(std::ostream & os)              { set_background("w");    return os; }
    inline std::ostream & on_grey(std::ostream & os)               { set_background("e");    return os; }
    inline std::ostream & on_light_blue(std::ostream & os)         { set_background("lb");   return os; }
    inline std::ostream & on_light_green(std::ostream & os)        { set_background("lg");   return os; }
    inline std::ostream & on_light_aqua(std::ostream & os)         { set_background("la");   return os; }
    inline std::ostream & on_light_red(std::ostream & os)          { set_background("lr");   return os; }
    inline std::ostream & on_light_purple(std::ostream & os)       { set_background("lp");   return os; }
    inline std::ostream & on_light_yellow(std::ostream & os)       { set_background("ly");   return os; }
    inline std::ostream & on_bright_white(std::ostream & os)       { set_background("bw");   return os; }
    inline std::ostream & black_on_black(std::ostream & os)               { set("k", "k");   return os; }
    inline std::ostream & black_on_blue(std::ostream & os)                { set("k", "b");   return os; }
    inline std::ostream & black_on_green(std::ostream & os)               { set("k", "g");   return os; }
    inline std::ostream & black_on_aqua(std::ostream & os)                { set("k", "a");   return os; }
    inline std::ostream & black_on_red(std::ostream & os)                 { set("k", "r");   return os; }
    inline std::ostream & black_on_purple(std::ostream & os)              { set("k", "p");   return os; }
    inline std::ostream & black_on_yellow(std::ostream & os)              { set("k", "y");   return os; }
    inline std::ostream & black_on_white(std::ostream & os)               { set("k", "w");   return os; }
    inline std::ostream & black_on_grey(std::ostream & os)                { set("k", "e");   return os; }
    inline std::ostream & black_on_light_blue(std::ostream & os)          { set("k", "lb");  return os; }
    inline std::ostream & black_on_light_green(std::ostream & os)         { set("k", "lg");  return os; }
    inline std::ostream & black_on_light_aqua(std::ostream & os)          { set("k", "la");  return os; }
    inline std::ostream & black_on_light_red(std::ostream & os)           { set("k", "lr");  return os; }
    inline std::ostream & black_on_light_purple(std::ostream & os)        { set("k", "lp");  return os; }
    inline std::ostream & black_on_light_yellow(std::ostream & os)        { set("k", "ly");  return os; }
    inline std::ostream & black_on_bright_white(std::ostream & os)        { set("k", "bw");  return os; }
    inline std::ostream & blue_on_black(std::ostream & os)                { set("b", "k");   return os; }
    inline std::ostream & blue_on_blue(std::ostream & os)                 { set("b", "b");   return os; }
    inline std::ostream & blue_on_green(std::ostream & os)                { set("b", "g");   return os; }
    inline std::ostream & blue_on_aqua(std::ostream & os)                 { set("b", "a");   return os; }
    inline std::ostream & blue_on_red(std::ostream & os)                  { set("b", "r");   return os; }
    inline std::ostream & blue_on_purple(std::ostream & os)               { set("b", "p");   return os; }
    inline std::ostream & blue_on_yellow(std::ostream & os)               { set("b", "y");   return os; }
    inline std::ostream & blue_on_white(std::ostream & os)                { set("b", "w");   return os; }
    inline std::ostream & blue_on_grey(std::ostream & os)                 { set("b", "e");   return os; }
    inline std::ostream & blue_on_light_blue(std::ostream & os)           { set("b", "lb");  return os; }
    inline std::ostream & blue_on_light_green(std::ostream & os)          { set("b", "lg");  return os; }
    inline std::ostream & blue_on_light_aqua(std::ostream & os)           { set("b", "la");  return os; }
    inline std::ostream & blue_on_light_red(std::ostream & os)            { set("b", "lr");  return os; }
    inline std::ostream & blue_on_light_purple(std::ostream & os)         { set("b", "lp");  return os; }
    inline std::ostream & blue_on_light_yellow(std::ostream & os)         { set("b", "ly");  return os; }
    inline std::ostream & blue_on_bright_white(std::ostream & os)         { set("b", "bw");  return os; }
    inline std::ostream & green_on_black(std::ostream & os)               { set("g", "k");   return os; }
    inline std::ostream & green_on_blue(std::ostream & os)                { set("g", "b");   return os; }
    inline std::ostream & green_on_green(std::ostream & os)               { set("g", "g");   return os; }
    inline std::ostream & green_on_aqua(std::ostream & os)                { set("g", "a");   return os; }
    inline std::ostream & green_on_red(std::ostream & os)                 { set("g", "r");   return os; }
    inline std::ostream & green_on_purple(std::ostream & os)              { set("g", "p");   return os; }
    inline std::ostream & green_on_yellow(std::ostream & os)              { set("g", "y");   return os; }
    inline std::ostream & green_on_white(std::ostream & os)               { set("g", "w");   return os; }
    inline std::ostream & green_on_grey(std::ostream & os)                { set("g", "e");   return os; }
    inline std::ostream & green_on_light_blue(std::ostream & os)          { set("g", "lb");  return os; }
    inline std::ostream & green_on_light_green(std::ostream & os)         { set("g", "lg");  return os; }
    inline std::ostream & green_on_light_aqua(std::ostream & os)          { set("g", "la");  return os; }
    inline std::ostream & green_on_light_red(std::ostream & os)           { set("g", "lr");  return os; }
    inline std::ostream & green_on_light_purple(std::ostream & os)        { set("g", "lp");  return os; }
    inline std::ostream & green_on_light_yellow(std::ostream & os)        { set("g", "ly");  return os; }
    inline std::ostream & green_on_bright_white(std::ostream & os)        { set("g", "bw");  return os; }
    inline std::ostream & aqua_on_black(std::ostream & os)                { set("a", "k");   return os; }
    inline std::ostream & aqua_on_blue(std::ostream & os)                 { set("a", "b");   return os; }
    inline std::ostream & aqua_on_green(std::ostream & os)                { set("a", "g");   return os; }
    inline std::ostream & aqua_on_aqua(std::ostream & os)                 { set("a", "a");   return os; }
    inline std::ostream & aqua_on_red(std::ostream & os)                  { set("a", "r");   return os; }
    inline std::ostream & aqua_on_purple(std::ostream & os)               { set("a", "p");   return os; }
    inline std::ostream & aqua_on_yellow(std::ostream & os)               { set("a", "y");   return os; }
    inline std::ostream & aqua_on_white(std::ostream & os)                { set("a", "w");   return os; }
    inline std::ostream & aqua_on_grey(std::ostream & os)                 { set("a", "e");   return os; }
    inline std::ostream & aqua_on_light_blue(std::ostream & os)           { set("a", "lb");  return os; }
    inline std::ostream & aqua_on_light_green(std::ostream & os)          { set("a", "lg");  return os; }
    inline std::ostream & aqua_on_light_aqua(std::ostream & os)           { set("a", "la");  return os; }
    inline std::ostream & aqua_on_light_red(std::ostream & os)            { set("a", "lr");  return os; }
    inline std::ostream & aqua_on_light_purple(std::ostream & os)         { set("a", "lp");  return os; }
    inline std::ostream & aqua_on_light_yellow(std::ostream & os)         { set("a", "ly");  return os; }
    inline std::ostream & aqua_on_bright_white(std::ostream & os)         { set("a", "bw");  return os; }
    inline std::ostream & red_on_black(std::ostream & os)                 { set("r", "k");   return os; }
    inline std::ostream & red_on_blue(std::ostream & os)                  { set("r", "b");   return os; }
    inline std::ostream & red_on_green(std::ostream & os)                 { set("r", "g");   return os; }
    inline std::ostream & red_on_aqua(std::ostream & os)                  { set("r", "a");   return os; }
    inline std::ostream & red_on_red(std::ostream & os)                   { set("r", "r");   return os; }
    inline std::ostream & red_on_purple(std::ostream & os)                { set("r", "p");   return os; }
    inline std::ostream & red_on_yellow(std::ostream & os)                { set("r", "y");   return os; }
    inline std::ostream & red_on_white(std::ostream & os)                 { set("r", "w");   return os; }
    inline std::ostream & red_on_grey(std::ostream & os)                  { set("r", "e");   return os; }
    inline std::ostream & red_on_light_blue(std::ostream & os)            { set("r", "lb");  return os; }
    inline std::ostream & red_on_light_green(std::ostream & os)           { set("r", "lg");  return os; }
    inline std::ostream & red_on_light_aqua(std::ostream & os)            { set("r", "la");  return os; }
    inline std::ostream & red_on_light_red(std::ostream & os)             { set("r", "lr");  return os; }
    inline std::ostream & red_on_light_purple(std::ostream & os)          { set("r", "lp");  return os; }
    inline std::ostream & red_on_light_yellow(std::ostream & os)          { set("r", "ly");  return os; }
    inline std::ostream & red_on_bright_white(std::ostream & os)          { set("r", "bw");  return os; }
    inline std::ostream & purple_on_black(std::ostream & os)              { set("p", "k");   return os; }
    inline std::ostream & purple_on_blue(std::ostream & os)               { set("p", "b");   return os; }
    inline std::ostream & purple_on_green(std::ostream & os)              { set("p", "g");   return os; }
    inline std::ostream & purple_on_aqua(std::ostream & os)               { set("p", "a");   return os; }
    inline std::ostream & purple_on_red(std::ostream & os)                { set("p", "r");   return os; }
    inline std::ostream & purple_on_purple(std::ostream & os)             { set("p", "p");   return os; }
    inline std::ostream & purple_on_yellow(std::ostream & os)             { set("p", "y");   return os; }
    inline std::ostream & purple_on_white(std::ostream & os)              { set("p", "w");   return os; }
    inline std::ostream & purple_on_grey(std::ostream & os)               { set("p", "e");   return os; }
    inline std::ostream & purple_on_light_blue(std::ostream & os)         { set("p", "lb");  return os; }
    inline std::ostream & purple_on_light_green(std::ostream & os)        { set("p", "lg");  return os; }
    inline std::ostream & purple_on_light_aqua(std::ostream & os)         { set("p", "la");  return os; }
    inline std::ostream & purple_on_light_red(std::ostream & os)          { set("p", "lr");  return os; }
    inline std::ostream & purple_on_light_purple(std::ostream & os)       { set("p", "lp");  return os; }
    inline std::ostream & purple_on_light_yellow(std::ostream & os)       { set("p", "ly");  return os; }
    inline std::ostream & purple_on_bright_white(std::ostream & os)       { set("p", "bw");  return os; }
    inline std::ostream & yellow_on_black(std::ostream & os)              { set("y", "k");   return os; }
    inline std::ostream & yellow_on_blue(std::ostream & os)               { set("y", "b");   return os; }
    inline std::ostream & yellow_on_green(std::ostream & os)              { set("y", "g");   return os; }
    inline std::ostream & yellow_on_aqua(std::ostream & os)               { set("y", "a");   return os; }
    inline std::ostream & yellow_on_red(std::ostream & os)                { set("y", "r");   return os; }
    inline std::ostream & yellow_on_purple(std::ostream & os)             { set("y", "p");   return os; }
    inline std::ostream & yellow_on_yellow(std::ostream & os)             { set("y", "y");   return os; }
    inline std::ostream & yellow_on_white(std::ostream & os)              { set("y", "w");   return os; }
    inline std::ostream & yellow_on_grey(std::ostream & os)               { set("y", "e");   return os; }
    inline std::ostream & yellow_on_light_blue(std::ostream & os)         { set("y", "lb");  return os; }
    inline std::ostream & yellow_on_light_green(std::ostream & os)        { set("y", "lg");  return os; }
    inline std::ostream & yellow_on_light_aqua(std::ostream & os)         { set("y", "la");  return os; }
    inline std::ostream & yellow_on_light_red(std::ostream & os)          { set("y", "lr");  return os; }
    inline std::ostream & yellow_on_light_purple(std::ostream & os)       { set("y", "lp");  return os; }
    inline std::ostream & yellow_on_light_yellow(std::ostream & os)       { set("y", "ly");  return os; }
    inline std::ostream & yellow_on_bright_white(std::ostream & os)       { set("y", "bw");  return os; }
    inline std::ostream & white_on_black(std::ostream & os)               { set("w", "k");   return os; }
    inline std::ostream & white_on_blue(std::ostream & os)                { set("w", "b");   return os; }
    inline std::ostream & white_on_green(std::ostream & os)               { set("w", "g");   return os; }
    inline std::ostream & white_on_aqua(std::ostream & os)                { set("w", "a");   return os; }
    inline std::ostream & white_on_red(std::ostream & os)                 { set("w", "r");   return os; }
    inline std::ostream & white_on_purple(std::ostream & os)              { set("w", "p");   return os; }
    inline std::ostream & white_on_yellow(std::ostream & os)              { set("w", "y");   return os; }
    inline std::ostream & white_on_white(std::ostream & os)               { set("w", "w");   return os; }
    inline std::ostream & white_on_grey(std::ostream & os)                { set("w", "e");   return os; }
    inline std::ostream & white_on_light_blue(std::ostream & os)          { set("w", "lb");  return os; }
    inline std::ostream & white_on_light_green(std::ostream & os)         { set("w", "lg");  return os; }
    inline std::ostream & white_on_light_aqua(std::ostream & os)          { set("w", "la");  return os; }
    inline std::ostream & white_on_light_red(std::ostream & os)           { set("w", "lr");  return os; }
    inline std::ostream & white_on_light_purple(std::ostream & os)        { set("w", "lp");  return os; }
    inline std::ostream & white_on_light_yellow(std::ostream & os)        { set("w", "ly");  return os; }
    inline std::ostream & white_on_bright_white(std::ostream & os)        { set("w", "bw");  return os; }
    inline std::ostream & grey_on_black(std::ostream & os)                { set("e", "k");   return os; }
    inline std::ostream & grey_on_blue(std::ostream & os)                 { set("e", "b");   return os; }
    inline std::ostream & grey_on_green(std::ostream & os)                { set("e", "g");   return os; }
    inline std::ostream & grey_on_aqua(std::ostream & os)                 { set("e", "a");   return os; }
    inline std::ostream & grey_on_red(std::ostream & os)                  { set("e", "r");   return os; }
    inline std::ostream & grey_on_purple(std::ostream & os)               { set("e", "p");   return os; }
    inline std::ostream & grey_on_yellow(std::ostream & os)               { set("e", "y");   return os; }
    inline std::ostream & grey_on_white(std::ostream & os)                { set("e", "w");   return os; }
    inline std::ostream & grey_on_grey(std::ostream & os)                 { set("e", "e");   return os; }
    inline std::ostream & grey_on_light_blue(std::ostream & os)           { set("e", "lb");  return os; }
    inline std::ostream & grey_on_light_green(std::ostream & os)          { set("e", "lg");  return os; }
    inline std::ostream & grey_on_light_aqua(std::ostream & os)           { set("e", "la");  return os; }
    inline std::ostream & grey_on_light_red(std::ostream & os)            { set("e", "lr");  return os; }
    inline std::ostream & grey_on_light_purple(std::ostream & os)         { set("e", "lp");  return os; }
    inline std::ostream & grey_on_light_yellow(std::ostream & os)         { set("e", "ly");  return os; }
    inline std::ostream & grey_on_bright_white(std::ostream & os)         { set("e", "bw");  return os; }
    inline std::ostream & light_blue_on_black(std::ostream & os)          { set("lb", "k");  return os; }
    inline std::ostream & light_blue_on_blue(std::ostream & os)           { set("lb", "b");  return os; }
    inline std::ostream & light_blue_on_green(std::ostream & os)          { set("lb", "g");  return os; }
    inline std::ostream & light_blue_on_aqua(std::ostream & os)           { set("lb", "a");  return os; }
    inline std::ostream & light_blue_on_red(std::ostream & os)            { set("lb", "r");  return os; }
    inline std::ostream & light_blue_on_purple(std::ostream & os)         { set("lb", "p");  return os; }
    inline std::ostream & light_blue_on_yellow(std::ostream & os)         { set("lb", "y");  return os; }
    inline std::ostream & light_blue_on_white(std::ostream & os)          { set("lb", "w");  return os; }
    inline std::ostream & light_blue_on_grey(std::ostream & os)           { set("lb", "e");  return os; }
    inline std::ostream & light_blue_on_light_blue(std::ostream & os)     { set("lb", "lb"); return os; }
    inline std::ostream & light_blue_on_light_green(std::ostream & os)    { set("lb", "lg"); return os; }
    inline std::ostream & light_blue_on_light_aqua(std::ostream & os)     { set("lb", "la"); return os; }
    inline std::ostream & light_blue_on_light_red(std::ostream & os)      { set("lb", "lr"); return os; }
    inline std::ostream & light_blue_on_light_purple(std::ostream & os)   { set("lb", "lp"); return os; }
    inline std::ostream & light_blue_on_light_yellow(std::ostream & os)   { set("lb", "ly"); return os; }
    inline std::ostream & light_blue_on_bright_white(std::ostream & os)   { set("lb", "bw"); return os; }
    inline std::ostream & light_green_on_black(std::ostream & os)         { set("lg", "k");  return os; }
    inline std::ostream & light_green_on_blue(std::ostream & os)          { set("lg", "b");  return os; }
    inline std::ostream & light_green_on_green(std::ostream & os)         { set("lg", "g");  return os; }
    inline std::ostream & light_green_on_aqua(std::ostream & os)          { set("lg", "a");  return os; }
    inline std::ostream & light_green_on_red(std::ostream & os)           { set("lg", "r");  return os; }
    inline std::ostream & light_green_on_purple(std::ostream & os)        { set("lg", "p");  return os; }
    inline std::ostream & light_green_on_yellow(std::ostream & os)        { set("lg", "y");  return os; }
    inline std::ostream & light_green_on_white(std::ostream & os)         { set("lg", "w");  return os; }
    inline std::ostream & light_green_on_grey(std::ostream & os)          { set("lg", "e");  return os; }
    inline std::ostream & light_green_on_light_blue(std::ostream & os)    { set("lg", "lb"); return os; }
    inline std::ostream & light_green_on_light_green(std::ostream & os)   { set("lg", "lg"); return os; }
    inline std::ostream & light_green_on_light_aqua(std::ostream & os)    { set("lg", "la"); return os; }
    inline std::ostream & light_green_on_light_red(std::ostream & os)     { set("lg", "lr"); return os; }
    inline std::ostream & light_green_on_light_purple(std::ostream & os)  { set("lg", "lp"); return os; }
    inline std::ostream & light_green_on_light_yellow(std::ostream & os)  { set("lg", "ly"); return os; }
    inline std::ostream & light_green_on_bright_white(std::ostream & os)  { set("lg", "bw"); return os; }
    inline std::ostream & light_aqua_on_black(std::ostream & os)          { set("la", "k");  return os; }
    inline std::ostream & light_aqua_on_blue(std::ostream & os)           { set("la", "b");  return os; }
    inline std::ostream & light_aqua_on_green(std::ostream & os)          { set("la", "g");  return os; }
    inline std::ostream & light_aqua_on_aqua(std::ostream & os)           { set("la", "a");  return os; }
    inline std::ostream & light_aqua_on_red(std::ostream & os)            { set("la", "r");  return os; }
    inline std::ostream & light_aqua_on_purple(std::ostream & os)         { set("la", "p");  return os; }
    inline std::ostream & light_aqua_on_yellow(std::ostream & os)         { set("la", "y");  return os; }
    inline std::ostream & light_aqua_on_white(std::ostream & os)          { set("la", "w");  return os; }
    inline std::ostream & light_aqua_on_grey(std::ostream & os)           { set("la", "e");  return os; }
    inline std::ostream & light_aqua_on_light_blue(std::ostream & os)     { set("la", "lb"); return os; }
    inline std::ostream & light_aqua_on_light_green(std::ostream & os)    { set("la", "lg"); return os; }
    inline std::ostream & light_aqua_on_light_aqua(std::ostream & os)     { set("la", "la"); return os; }
    inline std::ostream & light_aqua_on_light_red(std::ostream & os)      { set("la", "lr"); return os; }
    inline std::ostream & light_aqua_on_light_purple(std::ostream & os)   { set("la", "lp"); return os; }
    inline std::ostream & light_aqua_on_light_yellow(std::ostream & os)   { set("la", "ly"); return os; }
    inline std::ostream & light_aqua_on_bright_white(std::ostream & os)   { set("la", "bw"); return os; }
    inline std::ostream & light_red_on_black(std::ostream & os)           { set("lr", "k");  return os; }
    inline std::ostream & light_red_on_blue(std::ostream & os)            { set("lr", "b");  return os; }
    inline std::ostream & light_red_on_green(std::ostream & os)           { set("lr", "g");  return os; }
    inline std::ostream & light_red_on_aqua(std::ostream & os)            { set("lr", "a");  return os; }
    inline std::ostream & light_red_on_red(std::ostream & os)             { set("lr", "r");  return os; }
    inline std::ostream & light_red_on_purple(std::ostream & os)          { set("lr", "p");  return os; }
    inline std::ostream & light_red_on_yellow(std::ostream & os)          { set("lr", "y");  return os; }
    inline std::ostream & light_red_on_white(std::ostream & os)           { set("lr", "w");  return os; }
    inline std::ostream & light_red_on_grey(std::ostream & os)            { set("lr", "e");  return os; }
    inline std::ostream & light_red_on_light_blue(std::ostream & os)      { set("lr", "lb"); return os; }
    inline std::ostream & light_red_on_light_green(std::ostream & os)     { set("lr", "lg"); return os; }
    inline std::ostream & light_red_on_light_aqua(std::ostream & os)      { set("lr", "la"); return os; }
    inline std::ostream & light_red_on_light_red(std::ostream & os)       { set("lr", "lr"); return os; }
    inline std::ostream & light_red_on_light_purple(std::ostream & os)    { set("lr", "lp"); return os; }
    inline std::ostream & light_red_on_light_yellow(std::ostream & os)    { set("lr", "ly"); return os; }
    inline std::ostream & light_red_on_bright_white(std::ostream & os)    { set("lr", "bw"); return os; }
    inline std::ostream & light_purple_on_black(std::ostream & os)        { set("lp", "k");  return os; }
    inline std::ostream & light_purple_on_blue(std::ostream & os)         { set("lp", "b");  return os; }
    inline std::ostream & light_purple_on_green(std::ostream & os)        { set("lp", "g");  return os; }
    inline std::ostream & light_purple_on_aqua(std::ostream & os)         { set("lp", "a");  return os; }
    inline std::ostream & light_purple_on_red(std::ostream & os)          { set("lp", "r");  return os; }
    inline std::ostream & light_purple_on_purple(std::ostream & os)       { set("lp", "p");  return os; }
    inline std::ostream & light_purple_on_yellow(std::ostream & os)       { set("lp", "y");  return os; }
    inline std::ostream & light_purple_on_white(std::ostream & os)        { set("lp", "w");  return os; }
    inline std::ostream & light_purple_on_grey(std::ostream & os)         { set("lp", "e");  return os; }
    inline std::ostream & light_purple_on_light_blue(std::ostream & os)   { set("lp", "lb"); return os; }
    inline std::ostream & light_purple_on_light_green(std::ostream & os)  { set("lp", "lg"); return os; }
    inline std::ostream & light_purple_on_light_aqua(std::ostream & os)   { set("lp", "la"); return os; }
    inline std::ostream & light_purple_on_light_red(std::ostream & os)    { set("lp", "lr"); return os; }
    inline std::ostream & light_purple_on_light_purple(std::ostream & os) { set("lp", "lp"); return os; }
    inline std::ostream & light_purple_on_light_yellow(std::ostream & os) { set("lp", "ly"); return os; }
    inline std::ostream & light_purple_on_bright_white(std::ostream & os) { set("lp", "bw"); return os; }
    inline std::ostream & light_yellow_on_black(std::ostream & os)        { set("ly", "k");  return os; }
    inline std::ostream & light_yellow_on_blue(std::ostream & os)         { set("ly", "b");  return os; }
    inline std::ostream & light_yellow_on_green(std::ostream & os)        { set("ly", "g");  return os; }
    inline std::ostream & light_yellow_on_aqua(std::ostream & os)         { set("ly", "a");  return os; }
    inline std::ostream & light_yellow_on_red(std::ostream & os)          { set("ly", "r");  return os; }
    inline std::ostream & light_yellow_on_purple(std::ostream & os)       { set("ly", "p");  return os; }
    inline std::ostream & light_yellow_on_yellow(std::ostream & os)       { set("ly", "y");  return os; }
    inline std::ostream & light_yellow_on_white(std::ostream & os)        { set("ly", "w");  return os; }
    inline std::ostream & light_yellow_on_grey(std::ostream & os)         { set("ly", "e");  return os; }
    inline std::ostream & light_yellow_on_light_blue(std::ostream & os)   { set("ly", "lb"); return os; }
    inline std::ostream & light_yellow_on_light_green(std::ostream & os)  { set("ly", "lg"); return os; }
    inline std::ostream & light_yellow_on_light_aqua(std::ostream & os)   { set("ly", "la"); return os; }
    inline std::ostream & light_yellow_on_light_red(std::ostream & os)    { set("ly", "lr"); return os; }
    inline std::ostream & light_yellow_on_light_purple(std::ostream & os) { set("ly", "lp"); return os; }
    inline std::ostream & light_yellow_on_light_yellow(std::ostream & os) { set("ly", "ly"); return os; }
    inline std::ostream & light_yellow_on_bright_white(std::ostream & os) { set("ly", "bw"); return os; }
    inline std::ostream & bright_white_on_black(std::ostream & os)        { set("bw", "k");  return os; }
    inline std::ostream & bright_white_on_blue(std::ostream & os)         { set("bw", "b");  return os; }
    inline std::ostream & bright_white_on_green(std::ostream & os)        { set("bw", "g");  return os; }
    inline std::ostream & bright_white_on_aqua(std::ostream & os)         { set("bw", "a");  return os; }
    inline std::ostream & bright_white_on_red(std::ostream & os)          { set("bw", "r");  return os; }
    inline std::ostream & bright_white_on_purple(std::ostream & os)       { set("bw", "p");  return os; }
    inline std::ostream & bright_white_on_yellow(std::ostream & os)       { set("bw", "y");  return os; }
    inline std::ostream & bright_white_on_white(std::ostream & os)        { set("bw", "w");  return os; }
    inline std::ostream & bright_white_on_grey(std::ostream & os)         { set("bw", "e");  return os; }
    inline std::ostream & bright_white_on_light_blue(std::ostream & os)   { set("bw", "lb"); return os; }
    inline std::ostream & bright_white_on_light_green(std::ostream & os)  { set("bw", "lg"); return os; }
    inline std::ostream & bright_white_on_light_aqua(std::ostream & os)   { set("bw", "la"); return os; }
    inline std::ostream & bright_white_on_light_red(std::ostream & os)    { set("bw", "lr"); return os; }
    inline std::ostream & bright_white_on_light_purple(std::ostream & os) { set("bw", "lp"); return os; }
    inline std::ostream & bright_white_on_light_yellow(std::ostream & os) { set("bw", "ly"); return os; }
    inline std::ostream & bright_white_on_bright_white(std::ostream & os) { set("bw", "bw"); return os; }
}


namespace dye
{
    template<typename T>
    using bar = typename std::conditional<std::is_same<T, const char *>::value, std::string, T>::type;

    template<typename T> class colorful;
    template<typename T> class item;

    template<typename T>
    class colorful : private std::list<item<T>>
    {
    public:
        using std::list<item<T>>::list;

        inline colorful<T> & operator+=(const colorful<T> & rhs)
        {
            this->insert(this->end(), rhs.begin(), rhs.end());
            return *this;
        }

        inline colorful<T> & operator+=(colorful<T> && rhs)
        {
            this->splice(this->end(), std::move(rhs));
            return *this;
        }

        inline colorful<T> & operator+=(T t)
        {
            this->push_back(std::move(t));
            return *this;
        }

        inline void push_front(T t)
        {
            this->std::list<item<T>>::push_front(item<T>(std::move(t)));
        }

        inline void push_back(T t)
        {
            this->std::list<item<T>>::push_back(item<T>(std::move(t)));
        }

        inline colorful<T> & invert()
        {
            for (auto & elem : *this)
                elem.invert();
            return *this;
        }

        template<typename U>
        friend std::ostream & operator<<(std::ostream &, const colorful<U> &);

        template<typename U>
        friend colorful<U> invert(colorful<U> col);
    };

    template<typename T>
    inline colorful<T> operator+(colorful<T> lhs, colorful<T> rhs)
    {
        colorful<T> res(std::move(lhs));
        return res += rhs;
    }

    template<typename T>
    inline colorful<T> operator+(colorful<T> lhs, std::string rhs)
    {
        colorful<T> res(std::move(lhs));
        res.push_back(std::move(rhs));
        return res;
    }

    template<typename T>
    inline colorful<T> operator+(const std::string & lhs, colorful<T> rhs)
    {
        colorful<T> res(std::move(rhs));
        res.push_front(std::move(lhs));
        return res;
    }

    template<typename T>
    inline std::ostream & operator<<(std::ostream & os, const colorful<T> & colorful)
    {
         for (const auto & elem : colorful)
             os << elem;
         return os;
    }

    template<typename T>
    inline colorful<T> invert(colorful<T> col)
    {
        colorful<T> res(std::move(col));
        for (auto & elem : res)
            elem.invert();
        return res;
    }

    template<typename T>
    class item
    {
        T thing;
        int color;

    public:
        item(T t)                               :  thing(std::move(t)), color(hue::get())       {}
        item(T t, int a)                        :  thing(std::move(t)), color(hue::itoc(a))     {}
        item(T t, int a, int b)                 :  thing(std::move(t)), color(hue::itoc(a, b))  {}
        item(T t, std::string a)                :  thing(std::move(t)), color(hue::stoc(a))     {}
        item(T t, std::string a, std::string b) :  thing(std::move(t)), color(hue::stoc(a, b))  {}

        inline item<T> & invert()
        {
            color = hue::invert(color);
            return *this;
        }

        template<typename U>
        friend class colorful;

        template<typename U>
        friend std::ostream & operator<<(std::ostream &, const item<U> &);
    };

    template<typename T>
    inline std::ostream & operator<<(std::ostream & os, const item<T> & it)
    {
        hue::set(it.color);
        os << it.thing;
        hue::reset();
        return os;
    }

    template<typename T> using R = colorful<bar<T>>;
    template<typename T> using S = item<bar<T>>;

    inline template<typename T> R<T> colorize(T t, std::string a)      { return R<T> { S<T>(t, a) }; }
    inline template<typename T> R<T> vanilla(T t)                      { return R<T> { S<T>(t) }; }
    inline template<typename T> R<T> black(T t)                        { return R<T> { S<T>(t, "k") }; }
    inline template<typename T> R<T> blue(T t)                         { return R<T> { S<T>(t, "b") }; }
    inline template<typename T> R<T> green(T t)                        { return R<T> { S<T>(t, "g") }; }
    inline template<typename T> R<T> aqua(T t)                         { return R<T> { S<T>(t, "a") }; }
    inline template<typename T> R<T> red(T t)                          { return R<T> { S<T>(t, "r") }; }
    inline template<typename T> R<T> purple(T t)                       { return R<T> { S<T>(t, "p") }; }
    inline template<typename T> R<T> yellow(T t)                       { return R<T> { S<T>(t, "y") }; }
    inline template<typename T> R<T> white(T t)                        { return R<T> { S<T>(t, "w") }; }
    inline template<typename T> R<T> grey(T t)                         { return R<T> { S<T>(t, "e") }; }
    inline template<typename T> R<T> light_blue(T t)                   { return R<T> { S<T>(t, "lb") }; }
    inline template<typename T> R<T> light_green(T t)                  { return R<T> { S<T>(t, "lg") }; }
    inline template<typename T> R<T> light_aqua(T t)                   { return R<T> { S<T>(t, "la") }; }
    inline template<typename T> R<T> light_red(T t)                    { return R<T> { S<T>(t, "lr") }; }
    inline template<typename T> R<T> light_purple(T t)                 { return R<T> { S<T>(t, "lp") }; }
    inline template<typename T> R<T> light_yellow(T t)                 { return R<T> { S<T>(t, "ly") }; }
    inline template<typename T> R<T> bright_white(T t)                 { return R<T> { S<T>(t, "bw") }; }
    inline template<typename T> R<T> on_black(T t)                     { return R<T> { S<T>(t, "k", "k") }; }
    inline template<typename T> R<T> on_blue(T t)                      { return R<T> { S<T>(t, "k", "b") }; }
    inline template<typename T> R<T> on_green(T t)                     { return R<T> { S<T>(t, "k", "g") }; }
    inline template<typename T> R<T> on_aqua(T t)                      { return R<T> { S<T>(t, "k", "a") }; }
    inline template<typename T> R<T> on_red(T t)                       { return R<T> { S<T>(t, "k", "r") }; }
    inline template<typename T> R<T> on_purple(T t)                    { return R<T> { S<T>(t, "k", "p") }; }
    inline template<typename T> R<T> on_yellow(T t)                    { return R<T> { S<T>(t, "k", "y") }; }
    inline template<typename T> R<T> on_white(T t)                     { return R<T> { S<T>(t, "k", "w") }; }
    inline template<typename T> R<T> on_grey(T t)                      { return R<T> { S<T>(t, "k", "e") }; }
    inline template<typename T> R<T> on_light_blue(T t)                { return R<T> { S<T>(t, "k", "lb") }; }
    inline template<typename T> R<T> on_light_green(T t)               { return R<T> { S<T>(t, "k", "lg") }; }
    inline template<typename T> R<T> on_light_aqua(T t)                { return R<T> { S<T>(t, "k", "la") }; }
    inline template<typename T> R<T> on_light_red(T t)                 { return R<T> { S<T>(t, "k", "lr") }; }
    inline template<typename T> R<T> on_light_purple(T t)              { return R<T> { S<T>(t, "k", "lp") }; }
    inline template<typename T> R<T> on_light_yellow(T t)              { return R<T> { S<T>(t, "k", "ly") }; }
    inline template<typename T> R<T> on_bright_white(T t)              { return R<T> { S<T>(t, "k", "bw") }; }
    inline template<typename T> R<T> black_on_black(T t)               { return R<T> { S<T>(t, "k", "k") }; }
    inline template<typename T> R<T> black_on_blue(T t)                { return R<T> { S<T>(t, "k", "b") }; }
    inline template<typename T> R<T> black_on_green(T t)               { return R<T> { S<T>(t, "k", "g") }; }
    inline template<typename T> R<T> black_on_aqua(T t)                { return R<T> { S<T>(t, "k", "a") }; }
    inline template<typename T> R<T> black_on_red(T t)                 { return R<T> { S<T>(t, "k", "r") }; }
    inline template<typename T> R<T> black_on_purple(T t)              { return R<T> { S<T>(t, "k", "p") }; }
    inline template<typename T> R<T> black_on_yellow(T t)              { return R<T> { S<T>(t, "k", "y") }; }
    inline template<typename T> R<T> black_on_white(T t)               { return R<T> { S<T>(t, "k", "w") }; }
    inline template<typename T> R<T> black_on_grey(T t)                { return R<T> { S<T>(t, "k", "e") }; }
    inline template<typename T> R<T> black_on_light_blue(T t)          { return R<T> { S<T>(t, "k", "lb") }; }
    inline template<typename T> R<T> black_on_light_green(T t)         { return R<T> { S<T>(t, "k", "lg") }; }
    inline template<typename T> R<T> black_on_light_aqua(T t)          { return R<T> { S<T>(t, "k", "la") }; }
    inline template<typename T> R<T> black_on_light_red(T t)           { return R<T> { S<T>(t, "k", "lr") }; }
    inline template<typename T> R<T> black_on_light_purple(T t)        { return R<T> { S<T>(t, "k", "lp") }; }
    inline template<typename T> R<T> black_on_light_yellow(T t)        { return R<T> { S<T>(t, "k", "ly") }; }
    inline template<typename T> R<T> black_on_bright_white(T t)        { return R<T> { S<T>(t, "k", "bw") }; }
    inline template<typename T> R<T> blue_on_black(T t)                { return R<T> { S<T>(t, "b", "k") }; }
    inline template<typename T> R<T> blue_on_blue(T t)                 { return R<T> { S<T>(t, "b", "b") }; }
    inline template<typename T> R<T> blue_on_green(T t)                { return R<T> { S<T>(t, "b", "g") }; }
    inline template<typename T> R<T> blue_on_aqua(T t)                 { return R<T> { S<T>(t, "b", "a") }; }
    inline template<typename T> R<T> blue_on_red(T t)                  { return R<T> { S<T>(t, "b", "r") }; }
    inline template<typename T> R<T> blue_on_purple(T t)               { return R<T> { S<T>(t, "b", "p") }; }
    inline template<typename T> R<T> blue_on_yellow(T t)               { return R<T> { S<T>(t, "b", "y") }; }
    inline template<typename T> R<T> blue_on_white(T t)                { return R<T> { S<T>(t, "b", "w") }; }
    inline template<typename T> R<T> blue_on_grey(T t)                 { return R<T> { S<T>(t, "b", "e") }; }
    inline template<typename T> R<T> blue_on_light_blue(T t)           { return R<T> { S<T>(t, "b", "lb") }; }
    inline template<typename T> R<T> blue_on_light_green(T t)          { return R<T> { S<T>(t, "b", "lg") }; }
    inline template<typename T> R<T> blue_on_light_aqua(T t)           { return R<T> { S<T>(t, "b", "la") }; }
    inline template<typename T> R<T> blue_on_light_red(T t)            { return R<T> { S<T>(t, "b", "lr") }; }
    inline template<typename T> R<T> blue_on_light_purple(T t)         { return R<T> { S<T>(t, "b", "lp") }; }
    inline template<typename T> R<T> blue_on_light_yellow(T t)         { return R<T> { S<T>(t, "b", "ly") }; }
    inline template<typename T> R<T> blue_on_bright_white(T t)         { return R<T> { S<T>(t, "b", "bw") }; }
    inline template<typename T> R<T> green_on_black(T t)               { return R<T> { S<T>(t, "g", "k") }; }
    inline template<typename T> R<T> green_on_blue(T t)                { return R<T> { S<T>(t, "g", "b") }; }
    inline template<typename T> R<T> green_on_green(T t)               { return R<T> { S<T>(t, "g", "g") }; }
    inline template<typename T> R<T> green_on_aqua(T t)                { return R<T> { S<T>(t, "g", "a") }; }
    inline template<typename T> R<T> green_on_red(T t)                 { return R<T> { S<T>(t, "g", "r") }; }
    inline template<typename T> R<T> green_on_purple(T t)              { return R<T> { S<T>(t, "g", "p") }; }
    inline template<typename T> R<T> green_on_yellow(T t)              { return R<T> { S<T>(t, "g", "y") }; }
    inline template<typename T> R<T> green_on_white(T t)               { return R<T> { S<T>(t, "g", "w") }; }
    inline template<typename T> R<T> green_on_grey(T t)                { return R<T> { S<T>(t, "g", "e") }; }
    inline template<typename T> R<T> green_on_light_blue(T t)          { return R<T> { S<T>(t, "g", "lb") }; }
    inline template<typename T> R<T> green_on_light_green(T t)         { return R<T> { S<T>(t, "g", "lg") }; }
    inline template<typename T> R<T> green_on_light_aqua(T t)          { return R<T> { S<T>(t, "g", "la") }; }
    inline template<typename T> R<T> green_on_light_red(T t)           { return R<T> { S<T>(t, "g", "lr") }; }
    inline template<typename T> R<T> green_on_light_purple(T t)        { return R<T> { S<T>(t, "g", "lp") }; }
    inline template<typename T> R<T> green_on_light_yellow(T t)        { return R<T> { S<T>(t, "g", "ly") }; }
    inline template<typename T> R<T> green_on_bright_white(T t)        { return R<T> { S<T>(t, "g", "bw") }; }
    inline template<typename T> R<T> aqua_on_black(T t)                { return R<T> { S<T>(t, "a", "k") }; }
    inline template<typename T> R<T> aqua_on_blue(T t)                 { return R<T> { S<T>(t, "a", "b") }; }
    inline template<typename T> R<T> aqua_on_green(T t)                { return R<T> { S<T>(t, "a", "g") }; }
    inline template<typename T> R<T> aqua_on_aqua(T t)                 { return R<T> { S<T>(t, "a", "a") }; }
    inline template<typename T> R<T> aqua_on_red(T t)                  { return R<T> { S<T>(t, "a", "r") }; }
    inline template<typename T> R<T> aqua_on_purple(T t)               { return R<T> { S<T>(t, "a", "p") }; }
    inline template<typename T> R<T> aqua_on_yellow(T t)               { return R<T> { S<T>(t, "a", "y") }; }
    inline template<typename T> R<T> aqua_on_white(T t)                { return R<T> { S<T>(t, "a", "w") }; }
    inline template<typename T> R<T> aqua_on_grey(T t)                 { return R<T> { S<T>(t, "a", "e") }; }
    inline template<typename T> R<T> aqua_on_light_blue(T t)           { return R<T> { S<T>(t, "a", "lb") }; }
    inline template<typename T> R<T> aqua_on_light_green(T t)          { return R<T> { S<T>(t, "a", "lg") }; }
    inline template<typename T> R<T> aqua_on_light_aqua(T t)           { return R<T> { S<T>(t, "a", "la") }; }
    inline template<typename T> R<T> aqua_on_light_red(T t)            { return R<T> { S<T>(t, "a", "lr") }; }
    inline template<typename T> R<T> aqua_on_light_purple(T t)         { return R<T> { S<T>(t, "a", "lp") }; }
    inline template<typename T> R<T> aqua_on_light_yellow(T t)         { return R<T> { S<T>(t, "a", "ly") }; }
    inline template<typename T> R<T> aqua_on_bright_white(T t)         { return R<T> { S<T>(t, "a", "bw") }; }
    inline template<typename T> R<T> red_on_black(T t)                 { return R<T> { S<T>(t, "r", "k") }; }
    inline template<typename T> R<T> red_on_blue(T t)                  { return R<T> { S<T>(t, "r", "b") }; }
    inline template<typename T> R<T> red_on_green(T t)                 { return R<T> { S<T>(t, "r", "g") }; }
    inline template<typename T> R<T> red_on_aqua(T t)                  { return R<T> { S<T>(t, "r", "a") }; }
    inline template<typename T> R<T> red_on_red(T t)                   { return R<T> { S<T>(t, "r", "r") }; }
    inline template<typename T> R<T> red_on_purple(T t)                { return R<T> { S<T>(t, "r", "p") }; }
    inline template<typename T> R<T> red_on_yellow(T t)                { return R<T> { S<T>(t, "r", "y") }; }
    inline template<typename T> R<T> red_on_white(T t)                 { return R<T> { S<T>(t, "r", "w") }; }
    inline template<typename T> R<T> red_on_grey(T t)                  { return R<T> { S<T>(t, "r", "e") }; }
    inline template<typename T> R<T> red_on_light_blue(T t)            { return R<T> { S<T>(t, "r", "lb") }; }
    inline template<typename T> R<T> red_on_light_green(T t)           { return R<T> { S<T>(t, "r", "lg") }; }
    inline template<typename T> R<T> red_on_light_aqua(T t)            { return R<T> { S<T>(t, "r", "la") }; }
    inline template<typename T> R<T> red_on_light_red(T t)             { return R<T> { S<T>(t, "r", "lr") }; }
    inline template<typename T> R<T> red_on_light_purple(T t)          { return R<T> { S<T>(t, "r", "lp") }; }
    inline template<typename T> R<T> red_on_light_yellow(T t)          { return R<T> { S<T>(t, "r", "ly") }; }
    inline template<typename T> R<T> red_on_bright_white(T t)          { return R<T> { S<T>(t, "r", "bw") }; }
    inline template<typename T> R<T> purple_on_black(T t)              { return R<T> { S<T>(t, "p", "k") }; }
    inline template<typename T> R<T> purple_on_blue(T t)               { return R<T> { S<T>(t, "p", "b") }; }
    inline template<typename T> R<T> purple_on_green(T t)              { return R<T> { S<T>(t, "p", "g") }; }
    inline template<typename T> R<T> purple_on_aqua(T t)               { return R<T> { S<T>(t, "p", "a") }; }
    inline template<typename T> R<T> purple_on_red(T t)                { return R<T> { S<T>(t, "p", "r") }; }
    inline template<typename T> R<T> purple_on_purple(T t)             { return R<T> { S<T>(t, "p", "p") }; }
    inline template<typename T> R<T> purple_on_yellow(T t)             { return R<T> { S<T>(t, "p", "y") }; }
    inline template<typename T> R<T> purple_on_white(T t)              { return R<T> { S<T>(t, "p", "w") }; }
    inline template<typename T> R<T> purple_on_grey(T t)               { return R<T> { S<T>(t, "p", "e") }; }
    inline template<typename T> R<T> purple_on_light_blue(T t)         { return R<T> { S<T>(t, "p", "lb") }; }
    inline template<typename T> R<T> purple_on_light_green(T t)        { return R<T> { S<T>(t, "p", "lg") }; }
    inline template<typename T> R<T> purple_on_light_aqua(T t)         { return R<T> { S<T>(t, "p", "la") }; }
    inline template<typename T> R<T> purple_on_light_red(T t)          { return R<T> { S<T>(t, "p", "lr") }; }
    inline template<typename T> R<T> purple_on_light_purple(T t)       { return R<T> { S<T>(t, "p", "lp") }; }
    inline template<typename T> R<T> purple_on_light_yellow(T t)       { return R<T> { S<T>(t, "p", "ly") }; }
    inline template<typename T> R<T> purple_on_bright_white(T t)       { return R<T> { S<T>(t, "p", "bw") }; }
    inline template<typename T> R<T> yellow_on_black(T t)              { return R<T> { S<T>(t, "y", "k") }; }
    inline template<typename T> R<T> yellow_on_blue(T t)               { return R<T> { S<T>(t, "y", "b") }; }
    inline template<typename T> R<T> yellow_on_green(T t)              { return R<T> { S<T>(t, "y", "g") }; }
    inline template<typename T> R<T> yellow_on_aqua(T t)               { return R<T> { S<T>(t, "y", "a") }; }
    inline template<typename T> R<T> yellow_on_red(T t)                { return R<T> { S<T>(t, "y", "r") }; }
    inline template<typename T> R<T> yellow_on_purple(T t)             { return R<T> { S<T>(t, "y", "p") }; }
    inline template<typename T> R<T> yellow_on_yellow(T t)             { return R<T> { S<T>(t, "y", "y") }; }
    inline template<typename T> R<T> yellow_on_white(T t)              { return R<T> { S<T>(t, "y", "w") }; }
    inline template<typename T> R<T> yellow_on_grey(T t)               { return R<T> { S<T>(t, "y", "e") }; }
    inline template<typename T> R<T> yellow_on_light_blue(T t)         { return R<T> { S<T>(t, "y", "lb") }; }
    inline template<typename T> R<T> yellow_on_light_green(T t)        { return R<T> { S<T>(t, "y", "lg") }; }
    inline template<typename T> R<T> yellow_on_light_aqua(T t)         { return R<T> { S<T>(t, "y", "la") }; }
    inline template<typename T> R<T> yellow_on_light_red(T t)          { return R<T> { S<T>(t, "y", "lr") }; }
    inline template<typename T> R<T> yellow_on_light_purple(T t)       { return R<T> { S<T>(t, "y", "lp") }; }
    inline template<typename T> R<T> yellow_on_light_yellow(T t)       { return R<T> { S<T>(t, "y", "ly") }; }
    inline template<typename T> R<T> yellow_on_bright_white(T t)       { return R<T> { S<T>(t, "y", "bw") }; }
    inline template<typename T> R<T> white_on_black(T t)               { return R<T> { S<T>(t, "w", "k") }; }
    inline template<typename T> R<T> white_on_blue(T t)                { return R<T> { S<T>(t, "w", "b") }; }
    inline template<typename T> R<T> white_on_green(T t)               { return R<T> { S<T>(t, "w", "g") }; }
    inline template<typename T> R<T> white_on_aqua(T t)                { return R<T> { S<T>(t, "w", "a") }; }
    inline template<typename T> R<T> white_on_red(T t)                 { return R<T> { S<T>(t, "w", "r") }; }
    inline template<typename T> R<T> white_on_purple(T t)              { return R<T> { S<T>(t, "w", "p") }; }
    inline template<typename T> R<T> white_on_yellow(T t)              { return R<T> { S<T>(t, "w", "y") }; }
    inline template<typename T> R<T> white_on_white(T t)               { return R<T> { S<T>(t, "w", "w") }; }
    inline template<typename T> R<T> white_on_grey(T t)                { return R<T> { S<T>(t, "w", "e") }; }
    inline template<typename T> R<T> white_on_light_blue(T t)          { return R<T> { S<T>(t, "w", "lb") }; }
    inline template<typename T> R<T> white_on_light_green(T t)         { return R<T> { S<T>(t, "w", "lg") }; }
    inline template<typename T> R<T> white_on_light_aqua(T t)          { return R<T> { S<T>(t, "w", "la") }; }
    inline template<typename T> R<T> white_on_light_red(T t)           { return R<T> { S<T>(t, "w", "lr") }; }
    inline template<typename T> R<T> white_on_light_purple(T t)        { return R<T> { S<T>(t, "w", "lp") }; }
    inline template<typename T> R<T> white_on_light_yellow(T t)        { return R<T> { S<T>(t, "w", "ly") }; }
    inline template<typename T> R<T> white_on_bright_white(T t)        { return R<T> { S<T>(t, "w", "bw") }; }
    inline template<typename T> R<T> grey_on_black(T t)                { return R<T> { S<T>(t, "e", "k") }; }
    inline template<typename T> R<T> grey_on_blue(T t)                 { return R<T> { S<T>(t, "e", "b") }; }
    inline template<typename T> R<T> grey_on_green(T t)                { return R<T> { S<T>(t, "e", "g") }; }
    inline template<typename T> R<T> grey_on_aqua(T t)                 { return R<T> { S<T>(t, "e", "a") }; }
    inline template<typename T> R<T> grey_on_red(T t)                  { return R<T> { S<T>(t, "e", "r") }; }
    inline template<typename T> R<T> grey_on_purple(T t)               { return R<T> { S<T>(t, "e", "p") }; }
    inline template<typename T> R<T> grey_on_yellow(T t)               { return R<T> { S<T>(t, "e", "y") }; }
    inline template<typename T> R<T> grey_on_white(T t)                { return R<T> { S<T>(t, "e", "w") }; }
    inline template<typename T> R<T> grey_on_grey(T t)                 { return R<T> { S<T>(t, "e", "e") }; }
    inline template<typename T> R<T> grey_on_light_blue(T t)           { return R<T> { S<T>(t, "e", "lb") }; }
    inline template<typename T> R<T> grey_on_light_green(T t)          { return R<T> { S<T>(t, "e", "lg") }; }
    inline template<typename T> R<T> grey_on_light_aqua(T t)           { return R<T> { S<T>(t, "e", "la") }; }
    inline template<typename T> R<T> grey_on_light_red(T t)            { return R<T> { S<T>(t, "e", "lr") }; }
    inline template<typename T> R<T> grey_on_light_purple(T t)         { return R<T> { S<T>(t, "e", "lp") }; }
    inline template<typename T> R<T> grey_on_light_yellow(T t)         { return R<T> { S<T>(t, "e", "ly") }; }
    inline template<typename T> R<T> grey_on_bright_white(T t)         { return R<T> { S<T>(t, "e", "bw") }; }
    inline template<typename T> R<T> light_blue_on_black(T t)          { return R<T> { S<T>(t, "lb", "k") }; }
    inline template<typename T> R<T> light_blue_on_blue(T t)           { return R<T> { S<T>(t, "lb", "b") }; }
    inline template<typename T> R<T> light_blue_on_green(T t)          { return R<T> { S<T>(t, "lb", "g") }; }
    inline template<typename T> R<T> light_blue_on_aqua(T t)           { return R<T> { S<T>(t, "lb", "a") }; }
    inline template<typename T> R<T> light_blue_on_red(T t)            { return R<T> { S<T>(t, "lb", "r") }; }
    inline template<typename T> R<T> light_blue_on_purple(T t)         { return R<T> { S<T>(t, "lb", "p") }; }
    inline template<typename T> R<T> light_blue_on_yellow(T t)         { return R<T> { S<T>(t, "lb", "y") }; }
    inline template<typename T> R<T> light_blue_on_white(T t)          { return R<T> { S<T>(t, "lb", "w") }; }
    inline template<typename T> R<T> light_blue_on_grey(T t)           { return R<T> { S<T>(t, "lb", "e") }; }
    inline template<typename T> R<T> light_blue_on_light_blue(T t)     { return R<T> { S<T>(t, "lb", "lb") }; }
    inline template<typename T> R<T> light_blue_on_light_green(T t)    { return R<T> { S<T>(t, "lb", "lg") }; }
    inline template<typename T> R<T> light_blue_on_light_aqua(T t)     { return R<T> { S<T>(t, "lb", "la") }; }
    inline template<typename T> R<T> light_blue_on_light_red(T t)      { return R<T> { S<T>(t, "lb", "lr") }; }
    inline template<typename T> R<T> light_blue_on_light_purple(T t)   { return R<T> { S<T>(t, "lb", "lp") }; }
    inline template<typename T> R<T> light_blue_on_light_yellow(T t)   { return R<T> { S<T>(t, "lb", "ly") }; }
    inline template<typename T> R<T> light_blue_on_bright_white(T t)   { return R<T> { S<T>(t, "lb", "bw") }; }
    inline template<typename T> R<T> light_green_on_black(T t)         { return R<T> { S<T>(t, "lg", "k") }; }
    inline template<typename T> R<T> light_green_on_blue(T t)          { return R<T> { S<T>(t, "lg", "b") }; }
    inline template<typename T> R<T> light_green_on_green(T t)         { return R<T> { S<T>(t, "lg", "g") }; }
    inline template<typename T> R<T> light_green_on_aqua(T t)          { return R<T> { S<T>(t, "lg", "a") }; }
    inline template<typename T> R<T> light_green_on_red(T t)           { return R<T> { S<T>(t, "lg", "r") }; }
    inline template<typename T> R<T> light_green_on_purple(T t)        { return R<T> { S<T>(t, "lg", "p") }; }
    inline template<typename T> R<T> light_green_on_yellow(T t)        { return R<T> { S<T>(t, "lg", "y") }; }
    inline template<typename T> R<T> light_green_on_white(T t)         { return R<T> { S<T>(t, "lg", "w") }; }
    inline template<typename T> R<T> light_green_on_grey(T t)          { return R<T> { S<T>(t, "lg", "e") }; }
    inline template<typename T> R<T> light_green_on_light_blue(T t)    { return R<T> { S<T>(t, "lg", "lb") }; }
    inline template<typename T> R<T> light_green_on_light_green(T t)   { return R<T> { S<T>(t, "lg", "lg") }; }
    inline template<typename T> R<T> light_green_on_light_aqua(T t)    { return R<T> { S<T>(t, "lg", "la") }; }
    inline template<typename T> R<T> light_green_on_light_red(T t)     { return R<T> { S<T>(t, "lg", "lr") }; }
    inline template<typename T> R<T> light_green_on_light_purple(T t)  { return R<T> { S<T>(t, "lg", "lp") }; }
    inline template<typename T> R<T> light_green_on_light_yellow(T t)  { return R<T> { S<T>(t, "lg", "ly") }; }
    inline template<typename T> R<T> light_green_on_bright_white(T t)  { return R<T> { S<T>(t, "lg", "bw") }; }
    inline template<typename T> R<T> light_aqua_on_black(T t)          { return R<T> { S<T>(t, "la", "k") }; }
    inline template<typename T> R<T> light_aqua_on_blue(T t)           { return R<T> { S<T>(t, "la", "b") }; }
    inline template<typename T> R<T> light_aqua_on_green(T t)          { return R<T> { S<T>(t, "la", "g") }; }
    inline template<typename T> R<T> light_aqua_on_aqua(T t)           { return R<T> { S<T>(t, "la", "a") }; }
    inline template<typename T> R<T> light_aqua_on_red(T t)            { return R<T> { S<T>(t, "la", "r") }; }
    inline template<typename T> R<T> light_aqua_on_purple(T t)         { return R<T> { S<T>(t, "la", "p") }; }
    inline template<typename T> R<T> light_aqua_on_yellow(T t)         { return R<T> { S<T>(t, "la", "y") }; }
    inline template<typename T> R<T> light_aqua_on_white(T t)          { return R<T> { S<T>(t, "la", "w") }; }
    inline template<typename T> R<T> light_aqua_on_grey(T t)           { return R<T> { S<T>(t, "la", "e") }; }
    inline template<typename T> R<T> light_aqua_on_light_blue(T t)     { return R<T> { S<T>(t, "la", "lb") }; }
    inline template<typename T> R<T> light_aqua_on_light_green(T t)    { return R<T> { S<T>(t, "la", "lg") }; }
    inline template<typename T> R<T> light_aqua_on_light_aqua(T t)     { return R<T> { S<T>(t, "la", "la") }; }
    inline template<typename T> R<T> light_aqua_on_light_red(T t)      { return R<T> { S<T>(t, "la", "lr") }; }
    inline template<typename T> R<T> light_aqua_on_light_purple(T t)   { return R<T> { S<T>(t, "la", "lp") }; }
    inline template<typename T> R<T> light_aqua_on_light_yellow(T t)   { return R<T> { S<T>(t, "la", "ly") }; }
    inline template<typename T> R<T> light_aqua_on_bright_white(T t)   { return R<T> { S<T>(t, "la", "bw") }; }
    inline template<typename T> R<T> light_red_on_black(T t)           { return R<T> { S<T>(t, "lr", "k") }; }
    inline template<typename T> R<T> light_red_on_blue(T t)            { return R<T> { S<T>(t, "lr", "b") }; }
    inline template<typename T> R<T> light_red_on_green(T t)           { return R<T> { S<T>(t, "lr", "g") }; }
    inline template<typename T> R<T> light_red_on_aqua(T t)            { return R<T> { S<T>(t, "lr", "a") }; }
    inline template<typename T> R<T> light_red_on_red(T t)             { return R<T> { S<T>(t, "lr", "r") }; }
    inline template<typename T> R<T> light_red_on_purple(T t)          { return R<T> { S<T>(t, "lr", "p") }; }
    inline template<typename T> R<T> light_red_on_yellow(T t)          { return R<T> { S<T>(t, "lr", "y") }; }
    inline template<typename T> R<T> light_red_on_white(T t)           { return R<T> { S<T>(t, "lr", "w") }; }
    inline template<typename T> R<T> light_red_on_grey(T t)            { return R<T> { S<T>(t, "lr", "e") }; }
    inline template<typename T> R<T> light_red_on_light_blue(T t)      { return R<T> { S<T>(t, "lr", "lb") }; }
    inline template<typename T> R<T> light_red_on_light_green(T t)     { return R<T> { S<T>(t, "lr", "lg") }; }
    inline template<typename T> R<T> light_red_on_light_aqua(T t)      { return R<T> { S<T>(t, "lr", "la") }; }
    inline template<typename T> R<T> light_red_on_light_red(T t)       { return R<T> { S<T>(t, "lr", "lr") }; }
    inline template<typename T> R<T> light_red_on_light_purple(T t)    { return R<T> { S<T>(t, "lr", "lp") }; }
    inline template<typename T> R<T> light_red_on_light_yellow(T t)    { return R<T> { S<T>(t, "lr", "ly") }; }
    inline template<typename T> R<T> light_red_on_bright_white(T t)    { return R<T> { S<T>(t, "lr", "bw") }; }
    inline template<typename T> R<T> light_purple_on_black(T t)        { return R<T> { S<T>(t, "lp", "k") }; }
    inline template<typename T> R<T> light_purple_on_blue(T t)         { return R<T> { S<T>(t, "lp", "b") }; }
    inline template<typename T> R<T> light_purple_on_green(T t)        { return R<T> { S<T>(t, "lp", "g") }; }
    inline template<typename T> R<T> light_purple_on_aqua(T t)         { return R<T> { S<T>(t, "lp", "a") }; }
    inline template<typename T> R<T> light_purple_on_red(T t)          { return R<T> { S<T>(t, "lp", "r") }; }
    inline template<typename T> R<T> light_purple_on_purple(T t)       { return R<T> { S<T>(t, "lp", "p") }; }
    inline template<typename T> R<T> light_purple_on_yellow(T t)       { return R<T> { S<T>(t, "lp", "y") }; }
    inline template<typename T> R<T> light_purple_on_white(T t)        { return R<T> { S<T>(t, "lp", "w") }; }
    inline template<typename T> R<T> light_purple_on_grey(T t)         { return R<T> { S<T>(t, "lp", "e") }; }
    inline template<typename T> R<T> light_purple_on_light_blue(T t)   { return R<T> { S<T>(t, "lp", "lb") }; }
    inline template<typename T> R<T> light_purple_on_light_green(T t)  { return R<T> { S<T>(t, "lp", "lg") }; }
    inline template<typename T> R<T> light_purple_on_light_aqua(T t)   { return R<T> { S<T>(t, "lp", "la") }; }
    inline template<typename T> R<T> light_purple_on_light_red(T t)    { return R<T> { S<T>(t, "lp", "lr") }; }
    inline template<typename T> R<T> light_purple_on_light_purple(T t) { return R<T> { S<T>(t, "lp", "lp") }; }
    inline template<typename T> R<T> light_purple_on_light_yellow(T t) { return R<T> { S<T>(t, "lp", "ly") }; }
    inline template<typename T> R<T> light_purple_on_bright_white(T t) { return R<T> { S<T>(t, "lp", "bw") }; }
    inline template<typename T> R<T> light_yellow_on_black(T t)        { return R<T> { S<T>(t, "ly", "k") }; }
    inline template<typename T> R<T> light_yellow_on_blue(T t)         { return R<T> { S<T>(t, "ly", "b") }; }
    inline template<typename T> R<T> light_yellow_on_green(T t)        { return R<T> { S<T>(t, "ly", "g") }; }
    inline template<typename T> R<T> light_yellow_on_aqua(T t)         { return R<T> { S<T>(t, "ly", "a") }; }
    inline template<typename T> R<T> light_yellow_on_red(T t)          { return R<T> { S<T>(t, "ly", "r") }; }
    inline template<typename T> R<T> light_yellow_on_purple(T t)       { return R<T> { S<T>(t, "ly", "p") }; }
    inline template<typename T> R<T> light_yellow_on_yellow(T t)       { return R<T> { S<T>(t, "ly", "y") }; }
    inline template<typename T> R<T> light_yellow_on_white(T t)        { return R<T> { S<T>(t, "ly", "w") }; }
    inline template<typename T> R<T> light_yellow_on_grey(T t)         { return R<T> { S<T>(t, "ly", "e") }; }
    inline template<typename T> R<T> light_yellow_on_light_blue(T t)   { return R<T> { S<T>(t, "ly", "lb") }; }
    inline template<typename T> R<T> light_yellow_on_light_green(T t)  { return R<T> { S<T>(t, "ly", "lg") }; }
    inline template<typename T> R<T> light_yellow_on_light_aqua(T t)   { return R<T> { S<T>(t, "ly", "la") }; }
    inline template<typename T> R<T> light_yellow_on_light_red(T t)    { return R<T> { S<T>(t, "ly", "lr") }; }
    inline template<typename T> R<T> light_yellow_on_light_purple(T t) { return R<T> { S<T>(t, "ly", "lp") }; }
    inline template<typename T> R<T> light_yellow_on_light_yellow(T t) { return R<T> { S<T>(t, "ly", "ly") }; }
    inline template<typename T> R<T> light_yellow_on_bright_white(T t) { return R<T> { S<T>(t, "ly", "bw") }; }
    inline template<typename T> R<T> bright_white_on_black(T t)        { return R<T> { S<T>(t, "bw", "k") }; }
    inline template<typename T> R<T> bright_white_on_blue(T t)         { return R<T> { S<T>(t, "bw", "b") }; }
    inline template<typename T> R<T> bright_white_on_green(T t)        { return R<T> { S<T>(t, "bw", "g") }; }
    inline template<typename T> R<T> bright_white_on_aqua(T t)         { return R<T> { S<T>(t, "bw", "a") }; }
    inline template<typename T> R<T> bright_white_on_red(T t)          { return R<T> { S<T>(t, "bw", "r") }; }
    inline template<typename T> R<T> bright_white_on_purple(T t)       { return R<T> { S<T>(t, "bw", "p") }; }
    inline template<typename T> R<T> bright_white_on_yellow(T t)       { return R<T> { S<T>(t, "bw", "y") }; }
    inline template<typename T> R<T> bright_white_on_white(T t)        { return R<T> { S<T>(t, "bw", "w") }; }
    inline template<typename T> R<T> bright_white_on_grey(T t)         { return R<T> { S<T>(t, "bw", "e") }; }
    inline template<typename T> R<T> bright_white_on_light_blue(T t)   { return R<T> { S<T>(t, "bw", "lb") }; }
    inline template<typename T> R<T> bright_white_on_light_green(T t)  { return R<T> { S<T>(t, "bw", "lg") }; }
    inline template<typename T> R<T> bright_white_on_light_aqua(T t)   { return R<T> { S<T>(t, "bw", "la") }; }
    inline template<typename T> R<T> bright_white_on_light_red(T t)    { return R<T> { S<T>(t, "bw", "lr") }; }
    inline template<typename T> R<T> bright_white_on_light_purple(T t) { return R<T> { S<T>(t, "bw", "lp") }; }
    inline template<typename T> R<T> bright_white_on_light_yellow(T t) { return R<T> { S<T>(t, "bw", "ly") }; }
    inline template<typename T> R<T> bright_white_on_bright_white(T t) { return R<T> { S<T>(t, "bw", "bw") }; }
}

#endif
