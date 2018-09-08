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
    using ostream = std::ostream;
    using string = std::string;

    constexpr int DEFAULT_COLOR = 7;
    constexpr int BAD_COLOR = -256;

    const std::map<string, int> CODES = {
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

    const std::map<int, string>  NAMES = {
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

    // string to color
    int stoc(string a)
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

    int itoc(int c)
    {
        return is_good(c) ? c : BAD_COLOR;
    }

    int itoc(int a, int b)
    {
        return itoc(a + b * 16);
    }

    int stoc(string a, string b)
    {
        return itoc(stoc(a), stoc(b));
    }

    string ctos(int c)
    {
        return (0 <= c && c < 256) ?
               "(text) " + NAMES.at(c % 16) + " + " +
               "(background) " + NAMES.at(c / 16) :
               "BAD COLOR";
    }

    int get()
    {
        CONSOLE_SCREEN_BUFFER_INFO i;
        return GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &i) ?
               i.wAttributes : BAD_COLOR;
    }

    int get_text()
    {
        return (get() != BAD_COLOR) ? get() % 16 : BAD_COLOR;
    }

    int get_background()
    {
        return (get() != BAD_COLOR) ? get() / 16 : BAD_COLOR;
    }

    void set(int c)
    {
        if (is_good(c))
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
    }

    void set(int a, int b)
    {
        set(a + b * 16);
    }

    void set(string a, string b)
    {
        set(stoc(a) + stoc(b) * 16);
    }

    void set_text(string a)
    {
        set(stoc(a), get_background());
    }

    void set_background(string b)
    {
        set(get_text(), stoc(b));
    }

    void reset()
    {
        set(DEFAULT_COLOR);
    }

    int inverse(int c)
    {
        if (is_good(c)) {
            int a = c % 16;
            int b = c / 16;
            return b + a * 16;
        }
        else
            return BAD_COLOR;
    }

    ostream & reset(ostream & os)           { reset();              return os; }
    ostream & black(ostream & os)           { set_text("k");        return os; }
    ostream & blue(ostream & os)            { set_text("b");        return os; }
    ostream & green(ostream & os)           { set_text("g");        return os; }
    ostream & aqua(ostream & os)            { set_text("a");        return os; }
    ostream & red(ostream & os)             { set_text("r");        return os; }
    ostream & purple(ostream & os)          { set_text("p");        return os; }
    ostream & yellow(ostream & os)          { set_text("y");        return os; }
    ostream & white(ostream & os)           { set_text("w");        return os; }
    ostream & grey(ostream & os)            { set_text("e");        return os; }
    ostream & light_blue(ostream & os)      { set_text("lb");       return os; }
    ostream & light_green(ostream & os)     { set_text("lg");       return os; }
    ostream & light_aqua(ostream & os)      { set_text("la");       return os; }
    ostream & light_red(ostream & os)       { set_text("lr");       return os; }
    ostream & light_purple(ostream & os)    { set_text("lp");       return os; }
    ostream & light_yellow(ostream & os)    { set_text("ly");       return os; }
    ostream & bright_white(ostream & os)    { set_text("bw");       return os; }
    ostream & on_black(ostream & os)        { set_background("k");  return os; }
    ostream & on_blue(ostream & os)         { set_background("b");  return os; }
    ostream & on_green(ostream & os)        { set_background("g");  return os; }
    ostream & on_aqua(ostream & os)         { set_background("a");  return os; }
    ostream & on_red(ostream & os)          { set_background("r");  return os; }
    ostream & on_purple(ostream & os)       { set_background("p");  return os; }
    ostream & on_yellow(ostream & os)       { set_background("y");  return os; }
    ostream & on_white(ostream & os)        { set_background("w");  return os; }
    ostream & on_grey(ostream & os)         { set_background("e");  return os; }
    ostream & on_light_blue(ostream & os)   { set_background("lb"); return os; }
    ostream & on_light_green(ostream & os)  { set_background("lg"); return os; }
    ostream & on_light_aqua(ostream & os)   { set_background("la"); return os; }
    ostream & on_light_red(ostream & os)    { set_background("lr"); return os; }
    ostream & on_light_purple(ostream & os) { set_background("lp"); return os; }
    ostream & on_light_yellow(ostream & os) { set_background("ly"); return os; }
    ostream & on_bright_white(ostream & os) { set_background("bw"); return os; }
}


namespace dye
{
    using ostream = std::ostream;
    using string = std::string;

    template<typename T> class colored;
    template<typename T> class colorful;

    template<typename T>
    class colorful : public std::list<colored<T>>
    {
    public:
        using std::list<colored<T>>::list;

        colorful<T> & operator+=(const colorful<T> & rhs)
        {
            this->insert(this->end(), rhs.begin(), rhs.end());
            return *this;
        }

        colorful<T> & operator+=(colorful<T> && rhs)
        {
            this->splice(this->end(), std::move(rhs));
            return *this;
        }
    };

    template<typename T>
    colorful<T> operator+(const colorful<T> & lhs, const colorful<T> & rhs)
    {
        colorful<T> res(lhs);
        return res += rhs;
    }

    template<typename T>
    colorful<T> operator+(const colorful<T> & lhs, colorful<T> && rhs)
    {
        colorful<T> res(lhs);
        return res += std::move(rhs);
    }

    template<typename T>
    colorful<T> operator+(colorful<T> && lhs, const colorful<T> & rhs)
    {
        colorful<T> res(std::move(lhs));
        return res += rhs;
    }

    template<typename T>
    colorful<T> operator+(colorful<T> && lhs, colorful<T> && rhs)
    {
        colorful<T> res(std::move(lhs));
        return res += std::move(rhs);
    }

    template<typename T>
    ostream & operator<<(ostream & os, const colorful<T> & colorful)
    {
         for (const auto & elem : colorful)
             os << elem;
         return os;
    }

    template<typename T>
    class colored
    {
        T thing;
        int color;

    public:
        colored(T t)                     :  thing(t), color(hue::get()) {}
        colored(T t, int a)              :  thing(t), color(hue::itoc(a)) {}
        colored(T t, int a, int b)       :  thing(t), color(hue::itoc(a, b)) {}
        colored(T t, string a)           :  thing(t), color(hue::stoc(a)) {}
        colored(T t, string a, string b) :  thing(t), color(hue::stoc(a, b)) {}

        template<typename U> friend class colorful;
        template<typename U> friend ostream & operator<<(ostream & os, const colored<U> & at);
    };

    template<typename T>
    ostream & operator<<(ostream & os, const colored<T> & at)
    {
        hue::set(at.color);
        os << at.thing;
        hue::reset();
        return os;
    }

    auto inverse = hue::inverse;

    template<typename T>
    using bar = typename std::conditional<std::is_same<T, const char *>::value, string, T>::type;
    template<typename T> using R = colorful<bar<T>>;
    template<typename T> using S = colored<bar<T>>;

    template<typename T> R<T> nothing(T t)                      { return R<T> { S<T>(t) }; }
    template<typename T> R<T> black(T t)                        { return R<T> { S<T>(t, "k") }; }
    template<typename T> R<T> blue(T t)                         { return R<T> { S<T>(t, "b") }; }
    template<typename T> R<T> green(T t)                        { return R<T> { S<T>(t, "g") }; }
    template<typename T> R<T> aqua(T t)                         { return R<T> { S<T>(t, "a") }; }
    template<typename T> R<T> red(T t)                          { return R<T> { S<T>(t, "r") }; }
    template<typename T> R<T> purple(T t)                       { return R<T> { S<T>(t, "p") }; }
    template<typename T> R<T> yellow(T t)                       { return R<T> { S<T>(t, "y") }; }
    template<typename T> R<T> white(T t)                        { return R<T> { S<T>(t, "w") }; }
    template<typename T> R<T> grey(T t)                         { return R<T> { S<T>(t, "e") }; }
    template<typename T> R<T> light_blue(T t)                   { return R<T> { S<T>(t, "lb") }; }
    template<typename T> R<T> light_green(T t)                  { return R<T> { S<T>(t, "lg") }; }
    template<typename T> R<T> light_aqua(T t)                   { return R<T> { S<T>(t, "lq") }; }
    template<typename T> R<T> light_red(T t)                    { return R<T> { S<T>(t, "lr") }; }
    template<typename T> R<T> light_purple(T t)                 { return R<T> { S<T>(t, "lp") }; }
    template<typename T> R<T> light_yellow(T t)                 { return R<T> { S<T>(t, "ly") }; }
    template<typename T> R<T> bright_white(T t)                 { return R<T> { S<T>(t, "bw") }; }
    template<typename T> R<T> black_on_black(T t)               { return R<T> { S<T>(t, "k", "k") }; }
    template<typename T> R<T> black_on_blue(T t)                { return R<T> { S<T>(t, "k", "b") }; }
    template<typename T> R<T> black_on_green(T t)               { return R<T> { S<T>(t, "k", "g") }; }
    template<typename T> R<T> black_on_aqua(T t)                { return R<T> { S<T>(t, "k", "a") }; }
    template<typename T> R<T> black_on_red(T t)                 { return R<T> { S<T>(t, "k", "r") }; }
    template<typename T> R<T> black_on_purple(T t)              { return R<T> { S<T>(t, "k", "p") }; }
    template<typename T> R<T> black_on_yellow(T t)              { return R<T> { S<T>(t, "k", "y") }; }
    template<typename T> R<T> black_on_white(T t)               { return R<T> { S<T>(t, "k", "w") }; }
    template<typename T> R<T> black_on_grey(T t)                { return R<T> { S<T>(t, "k", "e") }; }
    template<typename T> R<T> black_on_light_blue(T t)          { return R<T> { S<T>(t, "k", "lb") }; }
    template<typename T> R<T> black_on_light_green(T t)         { return R<T> { S<T>(t, "k", "lg") }; }
    template<typename T> R<T> black_on_light_aqua(T t)          { return R<T> { S<T>(t, "k", "lq") }; }
    template<typename T> R<T> black_on_light_red(T t)           { return R<T> { S<T>(t, "k", "lr") }; }
    template<typename T> R<T> black_on_light_purple(T t)        { return R<T> { S<T>(t, "k", "lp") }; }
    template<typename T> R<T> black_on_light_yellow(T t)        { return R<T> { S<T>(t, "k", "ly") }; }
    template<typename T> R<T> black_on_bright_white(T t)        { return R<T> { S<T>(t, "k", "bw") }; }
    template<typename T> R<T> blue_on_black(T t)                { return R<T> { S<T>(t, "b", "k") }; }
    template<typename T> R<T> blue_on_blue(T t)                 { return R<T> { S<T>(t, "b", "b") }; }
    template<typename T> R<T> blue_on_green(T t)                { return R<T> { S<T>(t, "b", "g") }; }
    template<typename T> R<T> blue_on_aqua(T t)                 { return R<T> { S<T>(t, "b", "a") }; }
    template<typename T> R<T> blue_on_red(T t)                  { return R<T> { S<T>(t, "b", "r") }; }
    template<typename T> R<T> blue_on_purple(T t)               { return R<T> { S<T>(t, "b", "p") }; }
    template<typename T> R<T> blue_on_yellow(T t)               { return R<T> { S<T>(t, "b", "y") }; }
    template<typename T> R<T> blue_on_white(T t)                { return R<T> { S<T>(t, "b", "w") }; }
    template<typename T> R<T> blue_on_grey(T t)                 { return R<T> { S<T>(t, "b", "e") }; }
    template<typename T> R<T> blue_on_light_blue(T t)           { return R<T> { S<T>(t, "b", "lb") }; }
    template<typename T> R<T> blue_on_light_green(T t)          { return R<T> { S<T>(t, "b", "lg") }; }
    template<typename T> R<T> blue_on_light_aqua(T t)           { return R<T> { S<T>(t, "b", "lq") }; }
    template<typename T> R<T> blue_on_light_red(T t)            { return R<T> { S<T>(t, "b", "lr") }; }
    template<typename T> R<T> blue_on_light_purple(T t)         { return R<T> { S<T>(t, "b", "lp") }; }
    template<typename T> R<T> blue_on_light_yellow(T t)         { return R<T> { S<T>(t, "b", "ly") }; }
    template<typename T> R<T> blue_on_bright_white(T t)         { return R<T> { S<T>(t, "b", "bw") }; }
    template<typename T> R<T> green_on_black(T t)               { return R<T> { S<T>(t, "g", "k") }; }
    template<typename T> R<T> green_on_blue(T t)                { return R<T> { S<T>(t, "g", "b") }; }
    template<typename T> R<T> green_on_green(T t)               { return R<T> { S<T>(t, "g", "g") }; }
    template<typename T> R<T> green_on_aqua(T t)                { return R<T> { S<T>(t, "g", "a") }; }
    template<typename T> R<T> green_on_red(T t)                 { return R<T> { S<T>(t, "g", "r") }; }
    template<typename T> R<T> green_on_purple(T t)              { return R<T> { S<T>(t, "g", "p") }; }
    template<typename T> R<T> green_on_yellow(T t)              { return R<T> { S<T>(t, "g", "y") }; }
    template<typename T> R<T> green_on_white(T t)               { return R<T> { S<T>(t, "g", "w") }; }
    template<typename T> R<T> green_on_grey(T t)                { return R<T> { S<T>(t, "g", "e") }; }
    template<typename T> R<T> green_on_light_blue(T t)          { return R<T> { S<T>(t, "g", "lb") }; }
    template<typename T> R<T> green_on_light_green(T t)         { return R<T> { S<T>(t, "g", "lg") }; }
    template<typename T> R<T> green_on_light_aqua(T t)          { return R<T> { S<T>(t, "g", "lq") }; }
    template<typename T> R<T> green_on_light_red(T t)           { return R<T> { S<T>(t, "g", "lr") }; }
    template<typename T> R<T> green_on_light_purple(T t)        { return R<T> { S<T>(t, "g", "lp") }; }
    template<typename T> R<T> green_on_light_yellow(T t)        { return R<T> { S<T>(t, "g", "ly") }; }
    template<typename T> R<T> green_on_bright_white(T t)        { return R<T> { S<T>(t, "g", "bw") }; }
    template<typename T> R<T> aqua_on_black(T t)                { return R<T> { S<T>(t, "a", "k") }; }
    template<typename T> R<T> aqua_on_blue(T t)                 { return R<T> { S<T>(t, "a", "b") }; }
    template<typename T> R<T> aqua_on_green(T t)                { return R<T> { S<T>(t, "a", "g") }; }
    template<typename T> R<T> aqua_on_aqua(T t)                 { return R<T> { S<T>(t, "a", "a") }; }
    template<typename T> R<T> aqua_on_red(T t)                  { return R<T> { S<T>(t, "a", "r") }; }
    template<typename T> R<T> aqua_on_purple(T t)               { return R<T> { S<T>(t, "a", "p") }; }
    template<typename T> R<T> aqua_on_yellow(T t)               { return R<T> { S<T>(t, "a", "y") }; }
    template<typename T> R<T> aqua_on_white(T t)                { return R<T> { S<T>(t, "a", "w") }; }
    template<typename T> R<T> aqua_on_grey(T t)                 { return R<T> { S<T>(t, "a", "e") }; }
    template<typename T> R<T> aqua_on_light_blue(T t)           { return R<T> { S<T>(t, "a", "lb") }; }
    template<typename T> R<T> aqua_on_light_green(T t)          { return R<T> { S<T>(t, "a", "lg") }; }
    template<typename T> R<T> aqua_on_light_aqua(T t)           { return R<T> { S<T>(t, "a", "lq") }; }
    template<typename T> R<T> aqua_on_light_red(T t)            { return R<T> { S<T>(t, "a", "lr") }; }
    template<typename T> R<T> aqua_on_light_purple(T t)         { return R<T> { S<T>(t, "a", "lp") }; }
    template<typename T> R<T> aqua_on_light_yellow(T t)         { return R<T> { S<T>(t, "a", "ly") }; }
    template<typename T> R<T> aqua_on_bright_white(T t)         { return R<T> { S<T>(t, "a", "bw") }; }
    template<typename T> R<T> red_on_black(T t)                 { return R<T> { S<T>(t, "r", "k") }; }
    template<typename T> R<T> red_on_blue(T t)                  { return R<T> { S<T>(t, "r", "b") }; }
    template<typename T> R<T> red_on_green(T t)                 { return R<T> { S<T>(t, "r", "g") }; }
    template<typename T> R<T> red_on_aqua(T t)                  { return R<T> { S<T>(t, "r", "a") }; }
    template<typename T> R<T> red_on_red(T t)                   { return R<T> { S<T>(t, "r", "r") }; }
    template<typename T> R<T> red_on_purple(T t)                { return R<T> { S<T>(t, "r", "p") }; }
    template<typename T> R<T> red_on_yellow(T t)                { return R<T> { S<T>(t, "r", "y") }; }
    template<typename T> R<T> red_on_white(T t)                 { return R<T> { S<T>(t, "r", "w") }; }
    template<typename T> R<T> red_on_grey(T t)                  { return R<T> { S<T>(t, "r", "e") }; }
    template<typename T> R<T> red_on_light_blue(T t)            { return R<T> { S<T>(t, "r", "lb") }; }
    template<typename T> R<T> red_on_light_green(T t)           { return R<T> { S<T>(t, "r", "lg") }; }
    template<typename T> R<T> red_on_light_aqua(T t)            { return R<T> { S<T>(t, "r", "lq") }; }
    template<typename T> R<T> red_on_light_red(T t)             { return R<T> { S<T>(t, "r", "lr") }; }
    template<typename T> R<T> red_on_light_purple(T t)          { return R<T> { S<T>(t, "r", "lp") }; }
    template<typename T> R<T> red_on_light_yellow(T t)          { return R<T> { S<T>(t, "r", "ly") }; }
    template<typename T> R<T> red_on_bright_white(T t)          { return R<T> { S<T>(t, "r", "bw") }; }
    template<typename T> R<T> purple_on_black(T t)              { return R<T> { S<T>(t, "p", "k") }; }
    template<typename T> R<T> purple_on_blue(T t)               { return R<T> { S<T>(t, "p", "b") }; }
    template<typename T> R<T> purple_on_green(T t)              { return R<T> { S<T>(t, "p", "g") }; }
    template<typename T> R<T> purple_on_aqua(T t)               { return R<T> { S<T>(t, "p", "a") }; }
    template<typename T> R<T> purple_on_red(T t)                { return R<T> { S<T>(t, "p", "r") }; }
    template<typename T> R<T> purple_on_purple(T t)             { return R<T> { S<T>(t, "p", "p") }; }
    template<typename T> R<T> purple_on_yellow(T t)             { return R<T> { S<T>(t, "p", "y") }; }
    template<typename T> R<T> purple_on_white(T t)              { return R<T> { S<T>(t, "p", "w") }; }
    template<typename T> R<T> purple_on_grey(T t)               { return R<T> { S<T>(t, "p", "e") }; }
    template<typename T> R<T> purple_on_light_blue(T t)         { return R<T> { S<T>(t, "p", "lb") }; }
    template<typename T> R<T> purple_on_light_green(T t)        { return R<T> { S<T>(t, "p", "lg") }; }
    template<typename T> R<T> purple_on_light_aqua(T t)         { return R<T> { S<T>(t, "p", "lq") }; }
    template<typename T> R<T> purple_on_light_red(T t)          { return R<T> { S<T>(t, "p", "lr") }; }
    template<typename T> R<T> purple_on_light_purple(T t)       { return R<T> { S<T>(t, "p", "lp") }; }
    template<typename T> R<T> purple_on_light_yellow(T t)       { return R<T> { S<T>(t, "p", "ly") }; }
    template<typename T> R<T> purple_on_bright_white(T t)       { return R<T> { S<T>(t, "p", "bw") }; }
    template<typename T> R<T> yellow_on_black(T t)              { return R<T> { S<T>(t, "y", "k") }; }
    template<typename T> R<T> yellow_on_blue(T t)               { return R<T> { S<T>(t, "y", "b") }; }
    template<typename T> R<T> yellow_on_green(T t)              { return R<T> { S<T>(t, "y", "g") }; }
    template<typename T> R<T> yellow_on_aqua(T t)               { return R<T> { S<T>(t, "y", "a") }; }
    template<typename T> R<T> yellow_on_red(T t)                { return R<T> { S<T>(t, "y", "r") }; }
    template<typename T> R<T> yellow_on_purple(T t)             { return R<T> { S<T>(t, "y", "p") }; }
    template<typename T> R<T> yellow_on_yellow(T t)             { return R<T> { S<T>(t, "y", "y") }; }
    template<typename T> R<T> yellow_on_white(T t)              { return R<T> { S<T>(t, "y", "w") }; }
    template<typename T> R<T> yellow_on_grey(T t)               { return R<T> { S<T>(t, "y", "e") }; }
    template<typename T> R<T> yellow_on_light_blue(T t)         { return R<T> { S<T>(t, "y", "lb") }; }
    template<typename T> R<T> yellow_on_light_green(T t)        { return R<T> { S<T>(t, "y", "lg") }; }
    template<typename T> R<T> yellow_on_light_aqua(T t)         { return R<T> { S<T>(t, "y", "lq") }; }
    template<typename T> R<T> yellow_on_light_red(T t)          { return R<T> { S<T>(t, "y", "lr") }; }
    template<typename T> R<T> yellow_on_light_purple(T t)       { return R<T> { S<T>(t, "y", "lp") }; }
    template<typename T> R<T> yellow_on_light_yellow(T t)       { return R<T> { S<T>(t, "y", "ly") }; }
    template<typename T> R<T> yellow_on_bright_white(T t)       { return R<T> { S<T>(t, "y", "bw") }; }
    template<typename T> R<T> white_on_black(T t)               { return R<T> { S<T>(t, "w", "k") }; }
    template<typename T> R<T> white_on_blue(T t)                { return R<T> { S<T>(t, "w", "b") }; }
    template<typename T> R<T> white_on_green(T t)               { return R<T> { S<T>(t, "w", "g") }; }
    template<typename T> R<T> white_on_aqua(T t)                { return R<T> { S<T>(t, "w", "a") }; }
    template<typename T> R<T> white_on_red(T t)                 { return R<T> { S<T>(t, "w", "r") }; }
    template<typename T> R<T> white_on_purple(T t)              { return R<T> { S<T>(t, "w", "p") }; }
    template<typename T> R<T> white_on_yellow(T t)              { return R<T> { S<T>(t, "w", "y") }; }
    template<typename T> R<T> white_on_white(T t)               { return R<T> { S<T>(t, "w", "w") }; }
    template<typename T> R<T> white_on_grey(T t)                { return R<T> { S<T>(t, "w", "e") }; }
    template<typename T> R<T> white_on_light_blue(T t)          { return R<T> { S<T>(t, "w", "lb") }; }
    template<typename T> R<T> white_on_light_green(T t)         { return R<T> { S<T>(t, "w", "lg") }; }
    template<typename T> R<T> white_on_light_aqua(T t)          { return R<T> { S<T>(t, "w", "lq") }; }
    template<typename T> R<T> white_on_light_red(T t)           { return R<T> { S<T>(t, "w", "lr") }; }
    template<typename T> R<T> white_on_light_purple(T t)        { return R<T> { S<T>(t, "w", "lp") }; }
    template<typename T> R<T> white_on_light_yellow(T t)        { return R<T> { S<T>(t, "w", "ly") }; }
    template<typename T> R<T> white_on_bright_white(T t)        { return R<T> { S<T>(t, "w", "bw") }; }
    template<typename T> R<T> grey_on_black(T t)                { return R<T> { S<T>(t, "e", "k") }; }
    template<typename T> R<T> grey_on_blue(T t)                 { return R<T> { S<T>(t, "e", "b") }; }
    template<typename T> R<T> grey_on_green(T t)                { return R<T> { S<T>(t, "e", "g") }; }
    template<typename T> R<T> grey_on_aqua(T t)                 { return R<T> { S<T>(t, "e", "a") }; }
    template<typename T> R<T> grey_on_red(T t)                  { return R<T> { S<T>(t, "e", "r") }; }
    template<typename T> R<T> grey_on_purple(T t)               { return R<T> { S<T>(t, "e", "p") }; }
    template<typename T> R<T> grey_on_yellow(T t)               { return R<T> { S<T>(t, "e", "y") }; }
    template<typename T> R<T> grey_on_white(T t)                { return R<T> { S<T>(t, "e", "w") }; }
    template<typename T> R<T> grey_on_grey(T t)                 { return R<T> { S<T>(t, "e", "e") }; }
    template<typename T> R<T> grey_on_light_blue(T t)           { return R<T> { S<T>(t, "e", "lb") }; }
    template<typename T> R<T> grey_on_light_green(T t)          { return R<T> { S<T>(t, "e", "lg") }; }
    template<typename T> R<T> grey_on_light_aqua(T t)           { return R<T> { S<T>(t, "e", "lq") }; }
    template<typename T> R<T> grey_on_light_red(T t)            { return R<T> { S<T>(t, "e", "lr") }; }
    template<typename T> R<T> grey_on_light_purple(T t)         { return R<T> { S<T>(t, "e", "lp") }; }
    template<typename T> R<T> grey_on_light_yellow(T t)         { return R<T> { S<T>(t, "e", "ly") }; }
    template<typename T> R<T> grey_on_bright_white(T t)         { return R<T> { S<T>(t, "e", "bw") }; }
    template<typename T> R<T> light_blue_on_black(T t)          { return R<T> { S<T>(t, "lb", "k") }; }
    template<typename T> R<T> light_blue_on_blue(T t)           { return R<T> { S<T>(t, "lb", "b") }; }
    template<typename T> R<T> light_blue_on_green(T t)          { return R<T> { S<T>(t, "lb", "g") }; }
    template<typename T> R<T> light_blue_on_aqua(T t)           { return R<T> { S<T>(t, "lb", "a") }; }
    template<typename T> R<T> light_blue_on_red(T t)            { return R<T> { S<T>(t, "lb", "r") }; }
    template<typename T> R<T> light_blue_on_purple(T t)         { return R<T> { S<T>(t, "lb", "p") }; }
    template<typename T> R<T> light_blue_on_yellow(T t)         { return R<T> { S<T>(t, "lb", "y") }; }
    template<typename T> R<T> light_blue_on_white(T t)          { return R<T> { S<T>(t, "lb", "w") }; }
    template<typename T> R<T> light_blue_on_grey(T t)           { return R<T> { S<T>(t, "lb", "e") }; }
    template<typename T> R<T> light_blue_on_light_blue(T t)     { return R<T> { S<T>(t, "lb", "lb") }; }
    template<typename T> R<T> light_blue_on_light_green(T t)    { return R<T> { S<T>(t, "lb", "lg") }; }
    template<typename T> R<T> light_blue_on_light_aqua(T t)     { return R<T> { S<T>(t, "lb", "lq") }; }
    template<typename T> R<T> light_blue_on_light_red(T t)      { return R<T> { S<T>(t, "lb", "lr") }; }
    template<typename T> R<T> light_blue_on_light_purple(T t)   { return R<T> { S<T>(t, "lb", "lp") }; }
    template<typename T> R<T> light_blue_on_light_yellow(T t)   { return R<T> { S<T>(t, "lb", "ly") }; }
    template<typename T> R<T> light_blue_on_bright_white(T t)   { return R<T> { S<T>(t, "lb", "bw") }; }
    template<typename T> R<T> light_green_on_black(T t)         { return R<T> { S<T>(t, "lg", "k") }; }
    template<typename T> R<T> light_green_on_blue(T t)          { return R<T> { S<T>(t, "lg", "b") }; }
    template<typename T> R<T> light_green_on_green(T t)         { return R<T> { S<T>(t, "lg", "g") }; }
    template<typename T> R<T> light_green_on_aqua(T t)          { return R<T> { S<T>(t, "lg", "a") }; }
    template<typename T> R<T> light_green_on_red(T t)           { return R<T> { S<T>(t, "lg", "r") }; }
    template<typename T> R<T> light_green_on_purple(T t)        { return R<T> { S<T>(t, "lg", "p") }; }
    template<typename T> R<T> light_green_on_yellow(T t)        { return R<T> { S<T>(t, "lg", "y") }; }
    template<typename T> R<T> light_green_on_white(T t)         { return R<T> { S<T>(t, "lg", "w") }; }
    template<typename T> R<T> light_green_on_grey(T t)          { return R<T> { S<T>(t, "lg", "e") }; }
    template<typename T> R<T> light_green_on_light_blue(T t)    { return R<T> { S<T>(t, "lg", "lb") }; }
    template<typename T> R<T> light_green_on_light_green(T t)   { return R<T> { S<T>(t, "lg", "lg") }; }
    template<typename T> R<T> light_green_on_light_aqua(T t)    { return R<T> { S<T>(t, "lg", "lq") }; }
    template<typename T> R<T> light_green_on_light_red(T t)     { return R<T> { S<T>(t, "lg", "lr") }; }
    template<typename T> R<T> light_green_on_light_purple(T t)  { return R<T> { S<T>(t, "lg", "lp") }; }
    template<typename T> R<T> light_green_on_light_yellow(T t)  { return R<T> { S<T>(t, "lg", "ly") }; }
    template<typename T> R<T> light_green_on_bright_white(T t)  { return R<T> { S<T>(t, "lg", "bw") }; }
    template<typename T> R<T> light_aqua_on_black(T t)          { return R<T> { S<T>(t, "lq", "k") }; }
    template<typename T> R<T> light_aqua_on_blue(T t)           { return R<T> { S<T>(t, "lq", "b") }; }
    template<typename T> R<T> light_aqua_on_green(T t)          { return R<T> { S<T>(t, "lq", "g") }; }
    template<typename T> R<T> light_aqua_on_aqua(T t)           { return R<T> { S<T>(t, "lq", "a") }; }
    template<typename T> R<T> light_aqua_on_red(T t)            { return R<T> { S<T>(t, "lq", "r") }; }
    template<typename T> R<T> light_aqua_on_purple(T t)         { return R<T> { S<T>(t, "lq", "p") }; }
    template<typename T> R<T> light_aqua_on_yellow(T t)         { return R<T> { S<T>(t, "lq", "y") }; }
    template<typename T> R<T> light_aqua_on_white(T t)          { return R<T> { S<T>(t, "lq", "w") }; }
    template<typename T> R<T> light_aqua_on_grey(T t)           { return R<T> { S<T>(t, "lq", "e") }; }
    template<typename T> R<T> light_aqua_on_light_blue(T t)     { return R<T> { S<T>(t, "lq", "lb") }; }
    template<typename T> R<T> light_aqua_on_light_green(T t)    { return R<T> { S<T>(t, "lq", "lg") }; }
    template<typename T> R<T> light_aqua_on_light_aqua(T t)     { return R<T> { S<T>(t, "lq", "lq") }; }
    template<typename T> R<T> light_aqua_on_light_red(T t)      { return R<T> { S<T>(t, "lq", "lr") }; }
    template<typename T> R<T> light_aqua_on_light_purple(T t)   { return R<T> { S<T>(t, "lq", "lp") }; }
    template<typename T> R<T> light_aqua_on_light_yellow(T t)   { return R<T> { S<T>(t, "lq", "ly") }; }
    template<typename T> R<T> light_aqua_on_bright_white(T t)   { return R<T> { S<T>(t, "lq", "bw") }; }
    template<typename T> R<T> light_red_on_black(T t)           { return R<T> { S<T>(t, "lr", "k") }; }
    template<typename T> R<T> light_red_on_blue(T t)            { return R<T> { S<T>(t, "lr", "b") }; }
    template<typename T> R<T> light_red_on_green(T t)           { return R<T> { S<T>(t, "lr", "g") }; }
    template<typename T> R<T> light_red_on_aqua(T t)            { return R<T> { S<T>(t, "lr", "a") }; }
    template<typename T> R<T> light_red_on_red(T t)             { return R<T> { S<T>(t, "lr", "r") }; }
    template<typename T> R<T> light_red_on_purple(T t)          { return R<T> { S<T>(t, "lr", "p") }; }
    template<typename T> R<T> light_red_on_yellow(T t)          { return R<T> { S<T>(t, "lr", "y") }; }
    template<typename T> R<T> light_red_on_white(T t)           { return R<T> { S<T>(t, "lr", "w") }; }
    template<typename T> R<T> light_red_on_grey(T t)            { return R<T> { S<T>(t, "lr", "e") }; }
    template<typename T> R<T> light_red_on_light_blue(T t)      { return R<T> { S<T>(t, "lr", "lb") }; }
    template<typename T> R<T> light_red_on_light_green(T t)     { return R<T> { S<T>(t, "lr", "lg") }; }
    template<typename T> R<T> light_red_on_light_aqua(T t)      { return R<T> { S<T>(t, "lr", "lq") }; }
    template<typename T> R<T> light_red_on_light_red(T t)       { return R<T> { S<T>(t, "lr", "lr") }; }
    template<typename T> R<T> light_red_on_light_purple(T t)    { return R<T> { S<T>(t, "lr", "lp") }; }
    template<typename T> R<T> light_red_on_light_yellow(T t)    { return R<T> { S<T>(t, "lr", "ly") }; }
    template<typename T> R<T> light_red_on_bright_white(T t)    { return R<T> { S<T>(t, "lr", "bw") }; }
    template<typename T> R<T> light_purple_on_black(T t)        { return R<T> { S<T>(t, "lp", "k") }; }
    template<typename T> R<T> light_purple_on_blue(T t)         { return R<T> { S<T>(t, "lp", "b") }; }
    template<typename T> R<T> light_purple_on_green(T t)        { return R<T> { S<T>(t, "lp", "g") }; }
    template<typename T> R<T> light_purple_on_aqua(T t)         { return R<T> { S<T>(t, "lp", "a") }; }
    template<typename T> R<T> light_purple_on_red(T t)          { return R<T> { S<T>(t, "lp", "r") }; }
    template<typename T> R<T> light_purple_on_purple(T t)       { return R<T> { S<T>(t, "lp", "p") }; }
    template<typename T> R<T> light_purple_on_yellow(T t)       { return R<T> { S<T>(t, "lp", "y") }; }
    template<typename T> R<T> light_purple_on_white(T t)        { return R<T> { S<T>(t, "lp", "w") }; }
    template<typename T> R<T> light_purple_on_grey(T t)         { return R<T> { S<T>(t, "lp", "e") }; }
    template<typename T> R<T> light_purple_on_light_blue(T t)   { return R<T> { S<T>(t, "lp", "lb") }; }
    template<typename T> R<T> light_purple_on_light_green(T t)  { return R<T> { S<T>(t, "lp", "lg") }; }
    template<typename T> R<T> light_purple_on_light_aqua(T t)   { return R<T> { S<T>(t, "lp", "lq") }; }
    template<typename T> R<T> light_purple_on_light_red(T t)    { return R<T> { S<T>(t, "lp", "lr") }; }
    template<typename T> R<T> light_purple_on_light_purple(T t) { return R<T> { S<T>(t, "lp", "lp") }; }
    template<typename T> R<T> light_purple_on_light_yellow(T t) { return R<T> { S<T>(t, "lp", "ly") }; }
    template<typename T> R<T> light_purple_on_bright_white(T t) { return R<T> { S<T>(t, "lp", "bw") }; }
    template<typename T> R<T> light_yellow_on_black(T t)        { return R<T> { S<T>(t, "ly", "k") }; }
    template<typename T> R<T> light_yellow_on_blue(T t)         { return R<T> { S<T>(t, "ly", "b") }; }
    template<typename T> R<T> light_yellow_on_green(T t)        { return R<T> { S<T>(t, "ly", "g") }; }
    template<typename T> R<T> light_yellow_on_aqua(T t)         { return R<T> { S<T>(t, "ly", "a") }; }
    template<typename T> R<T> light_yellow_on_red(T t)          { return R<T> { S<T>(t, "ly", "r") }; }
    template<typename T> R<T> light_yellow_on_purple(T t)       { return R<T> { S<T>(t, "ly", "p") }; }
    template<typename T> R<T> light_yellow_on_yellow(T t)       { return R<T> { S<T>(t, "ly", "y") }; }
    template<typename T> R<T> light_yellow_on_white(T t)        { return R<T> { S<T>(t, "ly", "w") }; }
    template<typename T> R<T> light_yellow_on_grey(T t)         { return R<T> { S<T>(t, "ly", "e") }; }
    template<typename T> R<T> light_yellow_on_light_blue(T t)   { return R<T> { S<T>(t, "ly", "lb") }; }
    template<typename T> R<T> light_yellow_on_light_green(T t)  { return R<T> { S<T>(t, "ly", "lg") }; }
    template<typename T> R<T> light_yellow_on_light_aqua(T t)   { return R<T> { S<T>(t, "ly", "lq") }; }
    template<typename T> R<T> light_yellow_on_light_red(T t)    { return R<T> { S<T>(t, "ly", "lr") }; }
    template<typename T> R<T> light_yellow_on_light_purple(T t) { return R<T> { S<T>(t, "ly", "lp") }; }
    template<typename T> R<T> light_yellow_on_light_yellow(T t) { return R<T> { S<T>(t, "ly", "ly") }; }
    template<typename T> R<T> light_yellow_on_bright_white(T t) { return R<T> { S<T>(t, "ly", "bw") }; }
    template<typename T> R<T> bright_white_on_black(T t)        { return R<T> { S<T>(t, "bw", "k") }; }
    template<typename T> R<T> bright_white_on_blue(T t)         { return R<T> { S<T>(t, "bw", "b") }; }
    template<typename T> R<T> bright_white_on_green(T t)        { return R<T> { S<T>(t, "bw", "g") }; }
    template<typename T> R<T> bright_white_on_aqua(T t)         { return R<T> { S<T>(t, "bw", "a") }; }
    template<typename T> R<T> bright_white_on_red(T t)          { return R<T> { S<T>(t, "bw", "r") }; }
    template<typename T> R<T> bright_white_on_purple(T t)       { return R<T> { S<T>(t, "bw", "p") }; }
    template<typename T> R<T> bright_white_on_yellow(T t)       { return R<T> { S<T>(t, "bw", "y") }; }
    template<typename T> R<T> bright_white_on_white(T t)        { return R<T> { S<T>(t, "bw", "w") }; }
    template<typename T> R<T> bright_white_on_grey(T t)         { return R<T> { S<T>(t, "bw", "e") }; }
    template<typename T> R<T> bright_white_on_light_blue(T t)   { return R<T> { S<T>(t, "bw", "lb") }; }
    template<typename T> R<T> bright_white_on_light_green(T t)  { return R<T> { S<T>(t, "bw", "lg") }; }
    template<typename T> R<T> bright_white_on_light_aqua(T t)   { return R<T> { S<T>(t, "bw", "lq") }; }
    template<typename T> R<T> bright_white_on_light_red(T t)    { return R<T> { S<T>(t, "bw", "lr") }; }
    template<typename T> R<T> bright_white_on_light_purple(T t) { return R<T> { S<T>(t, "bw", "lp") }; }
    template<typename T> R<T> bright_white_on_light_yellow(T t) { return R<T> { S<T>(t, "bw", "ly") }; }
    template<typename T> R<T> bright_white_on_bright_white(T t) { return R<T> { S<T>(t, "bw", "bw") }; }
}
