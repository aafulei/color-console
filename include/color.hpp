#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
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
        return itoc(stoc(a), stoc(a));
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
        using std::list<colored<T>>::list;

    public:
        colorful operator+(const colorful<T> rhs) const
        {
            colorful res(*this);
            res.insert(res.end(), rhs.begin(), rhs.end());
            return res;
        }

        template<typename U>
        friend ostream & operator<<(ostream & os, const colorful<U> & colorful);
    };

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

    template<typename T> colorful<T> nothing(T t)                       { return colorful<T> { colored<T> (t) }; }
    template<typename T> colorful<T> black(T t)                         { return colorful<T> { colored<T>(t, "k") }; }
    template<typename T> colorful<T> blue(T t)                          { return colorful<T> { colored<T> (t, "b") }; }
    template<typename T> colorful<T> green(T t)                         { return colorful<T> { colored<T> (t, "g") }; }
    template<typename T> colorful<T> aqua(T t)                          { return colorful<T> { colored<T> (t, "a") }; }
    template<typename T> colorful<T> red(T t)                           { return colorful<T> { colored<T> (t, "r") }; }
    template<typename T> colorful<T> purple(T t)                        { return colorful<T> { colored<T> (t, "p") }; }
    template<typename T> colorful<T> yellow(T t)                        { return colorful<T> { colored<T> (t, "y") }; }
    template<typename T> colorful<T> white(T t)                         { return colorful<T> { colored<T> (t, "w") }; }
    template<typename T> colorful<T> grey(T t)                          { return colorful<T> { colored<T> (t, "e") }; }
    template<typename T> colorful<T> light_blue(T t)                    { return colorful<T> { colored<T> (t, "lb") }; }
    template<typename T> colorful<T> light_green(T t)                   { return colorful<T> { colored<T> (t, "lg") }; }
    template<typename T> colorful<T> light_aqua(T t)                    { return colorful<T> { colored<T> (t, "lq") }; }
    template<typename T> colorful<T> light_red(T t)                     { return colorful<T> { colored<T> (t, "lr") }; }
    template<typename T> colorful<T> light_purple(T t)                  { return colorful<T> { colored<T> (t, "lp") }; }
    template<typename T> colorful<T> light_yellow(T t)                  { return colorful<T> { colored<T> (t, "ly") }; }
    template<typename T> colorful<T> bright_white(T t)                  { return colorful<T> { colored<T> (t, "bw") }; }
    template<typename T> colorful<T> black_on_black(T t)                { return colorful<T> { colored<T> (t, "k", "k") }; }
    template<typename T> colorful<T> black_on_blue(T t)                 { return colorful<T> { colored<T> (t, "k", "b") }; }
    template<typename T> colorful<T> black_on_green(T t)                { return colorful<T> { colored<T> (t, "k", "g") }; }
    template<typename T> colorful<T> black_on_aqua(T t)                 { return colorful<T> { colored<T> (t, "k", "a") }; }
    template<typename T> colorful<T> black_on_red(T t)                  { return colorful<T> { colored<T> (t, "k", "r") }; }
    template<typename T> colorful<T> black_on_purple(T t)               { return colorful<T> { colored<T> (t, "k", "p") }; }
    template<typename T> colorful<T> black_on_yellow(T t)               { return colorful<T> { colored<T> (t, "k", "y") }; }
    template<typename T> colorful<T> black_on_white(T t)                { return colorful<T> { colored<T> (t, "k", "w") }; }
    template<typename T> colorful<T> black_on_grey(T t)                 { return colorful<T> { colored<T> (t, "k", "e") }; }
    template<typename T> colorful<T> black_on_light_blue(T t)           { return colorful<T> { colored<T> (t, "k", "lb") }; }
    template<typename T> colorful<T> black_on_light_green(T t)          { return colorful<T> { colored<T> (t, "k", "lg") }; }
    template<typename T> colorful<T> black_on_light_aqua(T t)           { return colorful<T> { colored<T> (t, "k", "lq") }; }
    template<typename T> colorful<T> black_on_light_red(T t)            { return colorful<T> { colored<T> (t, "k", "lr") }; }
    template<typename T> colorful<T> black_on_light_purple(T t)         { return colorful<T> { colored<T> (t, "k", "lp") }; }
    template<typename T> colorful<T> black_on_light_yellow(T t)         { return colorful<T> { colored<T> (t, "k", "ly") }; }
    template<typename T> colorful<T> black_on_bright_white(T t)         { return colorful<T> { colored<T> (t, "k", "bw") }; }
    template<typename T> colorful<T> blue_on_black(T t)                 { return colorful<T> { colored<T> (t, "b", "k") }; }
    template<typename T> colorful<T> blue_on_blue(T t)                  { return colorful<T> { colored<T> (t, "b", "b") }; }
    template<typename T> colorful<T> blue_on_green(T t)                 { return colorful<T> { colored<T> (t, "b", "g") }; }
    template<typename T> colorful<T> blue_on_aqua(T t)                  { return colorful<T> { colored<T> (t, "b", "a") }; }
    template<typename T> colorful<T> blue_on_red(T t)                   { return colorful<T> { colored<T> (t, "b", "r") }; }
    template<typename T> colorful<T> blue_on_purple(T t)                { return colorful<T> { colored<T> (t, "b", "p") }; }
    template<typename T> colorful<T> blue_on_yellow(T t)                { return colorful<T> { colored<T> (t, "b", "y") }; }
    template<typename T> colorful<T> blue_on_white(T t)                 { return colorful<T> { colored<T> (t, "b", "w") }; }
    template<typename T> colorful<T> blue_on_grey(T t)                  { return colorful<T> { colored<T> (t, "b", "e") }; }
    template<typename T> colorful<T> blue_on_light_blue(T t)            { return colorful<T> { colored<T> (t, "b", "lb") }; }
    template<typename T> colorful<T> blue_on_light_green(T t)           { return colorful<T> { colored<T> (t, "b", "lg") }; }
    template<typename T> colorful<T> blue_on_light_aqua(T t)            { return colorful<T> { colored<T> (t, "b", "lq") }; }
    template<typename T> colorful<T> blue_on_light_red(T t)             { return colorful<T> { colored<T> (t, "b", "lr") }; }
    template<typename T> colorful<T> blue_on_light_purple(T t)          { return colorful<T> { colored<T> (t, "b", "lp") }; }
    template<typename T> colorful<T> blue_on_light_yellow(T t)          { return colorful<T> { colored<T> (t, "b", "ly") }; }
    template<typename T> colorful<T> blue_on_bright_white(T t)          { return colorful<T> { colored<T> (t, "b", "bw") }; }
    template<typename T> colorful<T> green_on_black(T t)                { return colorful<T> { colored<T> (t, "g", "k") }; }
    template<typename T> colorful<T> green_on_blue(T t)                 { return colorful<T> { colored<T> (t, "g", "b") }; }
    template<typename T> colorful<T> green_on_green(T t)                { return colorful<T> { colored<T> (t, "g", "g") }; }
    template<typename T> colorful<T> green_on_aqua(T t)                 { return colorful<T> { colored<T> (t, "g", "a") }; }
    template<typename T> colorful<T> green_on_red(T t)                  { return colorful<T> { colored<T> (t, "g", "r") }; }
    template<typename T> colorful<T> green_on_purple(T t)               { return colorful<T> { colored<T> (t, "g", "p") }; }
    template<typename T> colorful<T> green_on_yellow(T t)               { return colorful<T> { colored<T> (t, "g", "y") }; }
    template<typename T> colorful<T> green_on_white(T t)                { return colorful<T> { colored<T> (t, "g", "w") }; }
    template<typename T> colorful<T> green_on_grey(T t)                 { return colorful<T> { colored<T> (t, "g", "e") }; }
    template<typename T> colorful<T> green_on_light_blue(T t)           { return colorful<T> { colored<T> (t, "g", "lb") }; }
    template<typename T> colorful<T> green_on_light_green(T t)          { return colorful<T> { colored<T> (t, "g", "lg") }; }
    template<typename T> colorful<T> green_on_light_aqua(T t)           { return colorful<T> { colored<T> (t, "g", "lq") }; }
    template<typename T> colorful<T> green_on_light_red(T t)            { return colorful<T> { colored<T> (t, "g", "lr") }; }
    template<typename T> colorful<T> green_on_light_purple(T t)         { return colorful<T> { colored<T> (t, "g", "lp") }; }
    template<typename T> colorful<T> green_on_light_yellow(T t)         { return colorful<T> { colored<T> (t, "g", "ly") }; }
    template<typename T> colorful<T> green_on_bright_white(T t)         { return colorful<T> { colored<T> (t, "g", "bw") }; }
    template<typename T> colorful<T> aqua_on_black(T t)                 { return colorful<T> { colored<T> (t, "a", "k") }; }
    template<typename T> colorful<T> aqua_on_blue(T t)                  { return colorful<T> { colored<T> (t, "a", "b") }; }
    template<typename T> colorful<T> aqua_on_green(T t)                 { return colorful<T> { colored<T> (t, "a", "g") }; }
    template<typename T> colorful<T> aqua_on_aqua(T t)                  { return colorful<T> { colored<T> (t, "a", "a") }; }
    template<typename T> colorful<T> aqua_on_red(T t)                   { return colorful<T> { colored<T> (t, "a", "r") }; }
    template<typename T> colorful<T> aqua_on_purple(T t)                { return colorful<T> { colored<T> (t, "a", "p") }; }
    template<typename T> colorful<T> aqua_on_yellow(T t)                { return colorful<T> { colored<T> (t, "a", "y") }; }
    template<typename T> colorful<T> aqua_on_white(T t)                 { return colorful<T> { colored<T> (t, "a", "w") }; }
    template<typename T> colorful<T> aqua_on_grey(T t)                  { return colorful<T> { colored<T> (t, "a", "e") }; }
    template<typename T> colorful<T> aqua_on_light_blue(T t)            { return colorful<T> { colored<T> (t, "a", "lb") }; }
    template<typename T> colorful<T> aqua_on_light_green(T t)           { return colorful<T> { colored<T> (t, "a", "lg") }; }
    template<typename T> colorful<T> aqua_on_light_aqua(T t)            { return colorful<T> { colored<T> (t, "a", "lq") }; }
    template<typename T> colorful<T> aqua_on_light_red(T t)             { return colorful<T> { colored<T> (t, "a", "lr") }; }
    template<typename T> colorful<T> aqua_on_light_purple(T t)          { return colorful<T> { colored<T> (t, "a", "lp") }; }
    template<typename T> colorful<T> aqua_on_light_yellow(T t)          { return colorful<T> { colored<T> (t, "a", "ly") }; }
    template<typename T> colorful<T> aqua_on_bright_white(T t)          { return colorful<T> { colored<T> (t, "a", "bw") }; }
    template<typename T> colorful<T> red_on_black(T t)                  { return colorful<T> { colored<T> (t, "r", "k") }; }
    template<typename T> colorful<T> red_on_blue(T t)                   { return colorful<T> { colored<T> (t, "r", "b") }; }
    template<typename T> colorful<T> red_on_green(T t)                  { return colorful<T> { colored<T> (t, "r", "g") }; }
    template<typename T> colorful<T> red_on_aqua(T t)                   { return colorful<T> { colored<T> (t, "r", "a") }; }
    template<typename T> colorful<T> red_on_red(T t)                    { return colorful<T> { colored<T> (t, "r", "r") }; }
    template<typename T> colorful<T> red_on_purple(T t)                 { return colorful<T> { colored<T> (t, "r", "p") }; }
    template<typename T> colorful<T> red_on_yellow(T t)                 { return colorful<T> { colored<T> (t, "r", "y") }; }
    template<typename T> colorful<T> red_on_white(T t)                  { return colorful<T> { colored<T> (t, "r", "w") }; }
    template<typename T> colorful<T> red_on_grey(T t)                   { return colorful<T> { colored<T> (t, "r", "e") }; }
    template<typename T> colorful<T> red_on_light_blue(T t)             { return colorful<T> { colored<T> (t, "r", "lb") }; }
    template<typename T> colorful<T> red_on_light_green(T t)            { return colorful<T> { colored<T> (t, "r", "lg") }; }
    template<typename T> colorful<T> red_on_light_aqua(T t)             { return colorful<T> { colored<T> (t, "r", "lq") }; }
    template<typename T> colorful<T> red_on_light_red(T t)              { return colorful<T> { colored<T> (t, "r", "lr") }; }
    template<typename T> colorful<T> red_on_light_purple(T t)           { return colorful<T> { colored<T> (t, "r", "lp") }; }
    template<typename T> colorful<T> red_on_light_yellow(T t)           { return colorful<T> { colored<T> (t, "r", "ly") }; }
    template<typename T> colorful<T> red_on_bright_white(T t)           { return colorful<T> { colored<T> (t, "r", "bw") }; }
    template<typename T> colorful<T> purple_on_black(T t)               { return colorful<T> { colored<T> (t, "p", "k") }; }
    template<typename T> colorful<T> purple_on_blue(T t)                { return colorful<T> { colored<T> (t, "p", "b") }; }
    template<typename T> colorful<T> purple_on_green(T t)               { return colorful<T> { colored<T> (t, "p", "g") }; }
    template<typename T> colorful<T> purple_on_aqua(T t)                { return colorful<T> { colored<T> (t, "p", "a") }; }
    template<typename T> colorful<T> purple_on_red(T t)                 { return colorful<T> { colored<T> (t, "p", "r") }; }
    template<typename T> colorful<T> purple_on_purple(T t)              { return colorful<T> { colored<T> (t, "p", "p") }; }
    template<typename T> colorful<T> purple_on_yellow(T t)              { return colorful<T> { colored<T> (t, "p", "y") }; }
    template<typename T> colorful<T> purple_on_white(T t)               { return colorful<T> { colored<T> (t, "p", "w") }; }
    template<typename T> colorful<T> purple_on_grey(T t)                { return colorful<T> { colored<T> (t, "p", "e") }; }
    template<typename T> colorful<T> purple_on_light_blue(T t)          { return colorful<T> { colored<T> (t, "p", "lb") }; }
    template<typename T> colorful<T> purple_on_light_green(T t)         { return colorful<T> { colored<T> (t, "p", "lg") }; }
    template<typename T> colorful<T> purple_on_light_aqua(T t)          { return colorful<T> { colored<T> (t, "p", "lq") }; }
    template<typename T> colorful<T> purple_on_light_red(T t)           { return colorful<T> { colored<T> (t, "p", "lr") }; }
    template<typename T> colorful<T> purple_on_light_purple(T t)        { return colorful<T> { colored<T> (t, "p", "lp") }; }
    template<typename T> colorful<T> purple_on_light_yellow(T t)        { return colorful<T> { colored<T> (t, "p", "ly") }; }
    template<typename T> colorful<T> purple_on_bright_white(T t)        { return colorful<T> { colored<T> (t, "p", "bw") }; }
    template<typename T> colorful<T> yellow_on_black(T t)               { return colorful<T> { colored<T> (t, "y", "k") }; }
    template<typename T> colorful<T> yellow_on_blue(T t)                { return colorful<T> { colored<T> (t, "y", "b") }; }
    template<typename T> colorful<T> yellow_on_green(T t)               { return colorful<T> { colored<T> (t, "y", "g") }; }
    template<typename T> colorful<T> yellow_on_aqua(T t)                { return colorful<T> { colored<T> (t, "y", "a") }; }
    template<typename T> colorful<T> yellow_on_red(T t)                 { return colorful<T> { colored<T> (t, "y", "r") }; }
    template<typename T> colorful<T> yellow_on_purple(T t)              { return colorful<T> { colored<T> (t, "y", "p") }; }
    template<typename T> colorful<T> yellow_on_yellow(T t)              { return colorful<T> { colored<T> (t, "y", "y") }; }
    template<typename T> colorful<T> yellow_on_white(T t)               { return colorful<T> { colored<T> (t, "y", "w") }; }
    template<typename T> colorful<T> yellow_on_grey(T t)                { return colorful<T> { colored<T> (t, "y", "e") }; }
    template<typename T> colorful<T> yellow_on_light_blue(T t)          { return colorful<T> { colored<T> (t, "y", "lb") }; }
    template<typename T> colorful<T> yellow_on_light_green(T t)         { return colorful<T> { colored<T> (t, "y", "lg") }; }
    template<typename T> colorful<T> yellow_on_light_aqua(T t)          { return colorful<T> { colored<T> (t, "y", "lq") }; }
    template<typename T> colorful<T> yellow_on_light_red(T t)           { return colorful<T> { colored<T> (t, "y", "lr") }; }
    template<typename T> colorful<T> yellow_on_light_purple(T t)        { return colorful<T> { colored<T> (t, "y", "lp") }; }
    template<typename T> colorful<T> yellow_on_light_yellow(T t)        { return colorful<T> { colored<T> (t, "y", "ly") }; }
    template<typename T> colorful<T> yellow_on_bright_white(T t)        { return colorful<T> { colored<T> (t, "y", "bw") }; }
    template<typename T> colorful<T> white_on_black(T t)                { return colorful<T> { colored<T> (t, "w", "k") }; }
    template<typename T> colorful<T> white_on_blue(T t)                 { return colorful<T> { colored<T> (t, "w", "b") }; }
    template<typename T> colorful<T> white_on_green(T t)                { return colorful<T> { colored<T> (t, "w", "g") }; }
    template<typename T> colorful<T> white_on_aqua(T t)                 { return colorful<T> { colored<T> (t, "w", "a") }; }
    template<typename T> colorful<T> white_on_red(T t)                  { return colorful<T> { colored<T> (t, "w", "r") }; }
    template<typename T> colorful<T> white_on_purple(T t)               { return colorful<T> { colored<T> (t, "w", "p") }; }
    template<typename T> colorful<T> white_on_yellow(T t)               { return colorful<T> { colored<T> (t, "w", "y") }; }
    template<typename T> colorful<T> white_on_white(T t)                { return colorful<T> { colored<T> (t, "w", "w") }; }
    template<typename T> colorful<T> white_on_grey(T t)                 { return colorful<T> { colored<T> (t, "w", "e") }; }
    template<typename T> colorful<T> white_on_light_blue(T t)           { return colorful<T> { colored<T> (t, "w", "lb") }; }
    template<typename T> colorful<T> white_on_light_green(T t)          { return colorful<T> { colored<T> (t, "w", "lg") }; }
    template<typename T> colorful<T> white_on_light_aqua(T t)           { return colorful<T> { colored<T> (t, "w", "lq") }; }
    template<typename T> colorful<T> white_on_light_red(T t)            { return colorful<T> { colored<T> (t, "w", "lr") }; }
    template<typename T> colorful<T> white_on_light_purple(T t)         { return colorful<T> { colored<T> (t, "w", "lp") }; }
    template<typename T> colorful<T> white_on_light_yellow(T t)         { return colorful<T> { colored<T> (t, "w", "ly") }; }
    template<typename T> colorful<T> white_on_bright_white(T t)         { return colorful<T> { colored<T> (t, "w", "bw") }; }
    template<typename T> colorful<T> grey_on_black(T t)                 { return colorful<T> { colored<T> (t, "e", "k") }; }
    template<typename T> colorful<T> grey_on_blue(T t)                  { return colorful<T> { colored<T> (t, "e", "b") }; }
    template<typename T> colorful<T> grey_on_green(T t)                 { return colorful<T> { colored<T> (t, "e", "g") }; }
    template<typename T> colorful<T> grey_on_aqua(T t)                  { return colorful<T> { colored<T> (t, "e", "a") }; }
    template<typename T> colorful<T> grey_on_red(T t)                   { return colorful<T> { colored<T> (t, "e", "r") }; }
    template<typename T> colorful<T> grey_on_purple(T t)                { return colorful<T> { colored<T> (t, "e", "p") }; }
    template<typename T> colorful<T> grey_on_yellow(T t)                { return colorful<T> { colored<T> (t, "e", "y") }; }
    template<typename T> colorful<T> grey_on_white(T t)                 { return colorful<T> { colored<T> (t, "e", "w") }; }
    template<typename T> colorful<T> grey_on_grey(T t)                  { return colorful<T> { colored<T> (t, "e", "e") }; }
    template<typename T> colorful<T> grey_on_light_blue(T t)            { return colorful<T> { colored<T> (t, "e", "lb") }; }
    template<typename T> colorful<T> grey_on_light_green(T t)           { return colorful<T> { colored<T> (t, "e", "lg") }; }
    template<typename T> colorful<T> grey_on_light_aqua(T t)            { return colorful<T> { colored<T> (t, "e", "lq") }; }
    template<typename T> colorful<T> grey_on_light_red(T t)             { return colorful<T> { colored<T> (t, "e", "lr") }; }
    template<typename T> colorful<T> grey_on_light_purple(T t)          { return colorful<T> { colored<T> (t, "e", "lp") }; }
    template<typename T> colorful<T> grey_on_light_yellow(T t)          { return colorful<T> { colored<T> (t, "e", "ly") }; }
    template<typename T> colorful<T> grey_on_bright_white(T t)          { return colorful<T> { colored<T> (t, "e", "bw") }; }
    template<typename T> colorful<T> light_blue_on_black(T t)           { return colorful<T> { colored<T> (t, "lb", "k") }; }
    template<typename T> colorful<T> light_blue_on_blue(T t)            { return colorful<T> { colored<T> (t, "lb", "b") }; }
    template<typename T> colorful<T> light_blue_on_green(T t)           { return colorful<T> { colored<T> (t, "lb", "g") }; }
    template<typename T> colorful<T> light_blue_on_aqua(T t)            { return colorful<T> { colored<T> (t, "lb", "a") }; }
    template<typename T> colorful<T> light_blue_on_red(T t)             { return colorful<T> { colored<T> (t, "lb", "r") }; }
    template<typename T> colorful<T> light_blue_on_purple(T t)          { return colorful<T> { colored<T> (t, "lb", "p") }; }
    template<typename T> colorful<T> light_blue_on_yellow(T t)          { return colorful<T> { colored<T> (t, "lb", "y") }; }
    template<typename T> colorful<T> light_blue_on_white(T t)           { return colorful<T> { colored<T> (t, "lb", "w") }; }
    template<typename T> colorful<T> light_blue_on_grey(T t)            { return colorful<T> { colored<T> (t, "lb", "e") }; }
    template<typename T> colorful<T> light_blue_on_light_blue(T t)      { return colorful<T> { colored<T> (t, "lb", "lb") }; }
    template<typename T> colorful<T> light_blue_on_light_green(T t)     { return colorful<T> { colored<T> (t, "lb", "lg") }; }
    template<typename T> colorful<T> light_blue_on_light_aqua(T t)      { return colorful<T> { colored<T> (t, "lb", "lq") }; }
    template<typename T> colorful<T> light_blue_on_light_red(T t)       { return colorful<T> { colored<T> (t, "lb", "lr") }; }
    template<typename T> colorful<T> light_blue_on_light_purple(T t)    { return colorful<T> { colored<T> (t, "lb", "lp") }; }
    template<typename T> colorful<T> light_blue_on_light_yellow(T t)    { return colorful<T> { colored<T> (t, "lb", "ly") }; }
    template<typename T> colorful<T> light_blue_on_bright_white(T t)    { return colorful<T> { colored<T> (t, "lb", "bw") }; }
    template<typename T> colorful<T> light_green_on_black(T t)          { return colorful<T> { colored<T> (t, "lg", "k") }; }
    template<typename T> colorful<T> light_green_on_blue(T t)           { return colorful<T> { colored<T> (t, "lg", "b") }; }
    template<typename T> colorful<T> light_green_on_green(T t)          { return colorful<T> { colored<T> (t, "lg", "g") }; }
    template<typename T> colorful<T> light_green_on_aqua(T t)           { return colorful<T> { colored<T> (t, "lg", "a") }; }
    template<typename T> colorful<T> light_green_on_red(T t)            { return colorful<T> { colored<T> (t, "lg", "r") }; }
    template<typename T> colorful<T> light_green_on_purple(T t)         { return colorful<T> { colored<T> (t, "lg", "p") }; }
    template<typename T> colorful<T> light_green_on_yellow(T t)         { return colorful<T> { colored<T> (t, "lg", "y") }; }
    template<typename T> colorful<T> light_green_on_white(T t)          { return colorful<T> { colored<T> (t, "lg", "w") }; }
    template<typename T> colorful<T> light_green_on_grey(T t)           { return colorful<T> { colored<T> (t, "lg", "e") }; }
    template<typename T> colorful<T> light_green_on_light_blue(T t)     { return colorful<T> { colored<T> (t, "lg", "lb") }; }
    template<typename T> colorful<T> light_green_on_light_green(T t)    { return colorful<T> { colored<T> (t, "lg", "lg") }; }
    template<typename T> colorful<T> light_green_on_light_aqua(T t)     { return colorful<T> { colored<T> (t, "lg", "lq") }; }
    template<typename T> colorful<T> light_green_on_light_red(T t)      { return colorful<T> { colored<T> (t, "lg", "lr") }; }
    template<typename T> colorful<T> light_green_on_light_purple(T t)   { return colorful<T> { colored<T> (t, "lg", "lp") }; }
    template<typename T> colorful<T> light_green_on_light_yellow(T t)   { return colorful<T> { colored<T> (t, "lg", "ly") }; }
    template<typename T> colorful<T> light_green_on_bright_white(T t)   { return colorful<T> { colored<T> (t, "lg", "bw") }; }
    template<typename T> colorful<T> light_aqua_on_black(T t)           { return colorful<T> { colored<T> (t, "lq", "k") }; }
    template<typename T> colorful<T> light_aqua_on_blue(T t)            { return colorful<T> { colored<T> (t, "lq", "b") }; }
    template<typename T> colorful<T> light_aqua_on_green(T t)           { return colorful<T> { colored<T> (t, "lq", "g") }; }
    template<typename T> colorful<T> light_aqua_on_aqua(T t)            { return colorful<T> { colored<T> (t, "lq", "a") }; }
    template<typename T> colorful<T> light_aqua_on_red(T t)             { return colorful<T> { colored<T> (t, "lq", "r") }; }
    template<typename T> colorful<T> light_aqua_on_purple(T t)          { return colorful<T> { colored<T> (t, "lq", "p") }; }
    template<typename T> colorful<T> light_aqua_on_yellow(T t)          { return colorful<T> { colored<T> (t, "lq", "y") }; }
    template<typename T> colorful<T> light_aqua_on_white(T t)           { return colorful<T> { colored<T> (t, "lq", "w") }; }
    template<typename T> colorful<T> light_aqua_on_grey(T t)            { return colorful<T> { colored<T> (t, "lq", "e") }; }
    template<typename T> colorful<T> light_aqua_on_light_blue(T t)      { return colorful<T> { colored<T> (t, "lq", "lb") }; }
    template<typename T> colorful<T> light_aqua_on_light_green(T t)     { return colorful<T> { colored<T> (t, "lq", "lg") }; }
    template<typename T> colorful<T> light_aqua_on_light_aqua(T t)      { return colorful<T> { colored<T> (t, "lq", "lq") }; }
    template<typename T> colorful<T> light_aqua_on_light_red(T t)       { return colorful<T> { colored<T> (t, "lq", "lr") }; }
    template<typename T> colorful<T> light_aqua_on_light_purple(T t)    { return colorful<T> { colored<T> (t, "lq", "lp") }; }
    template<typename T> colorful<T> light_aqua_on_light_yellow(T t)    { return colorful<T> { colored<T> (t, "lq", "ly") }; }
    template<typename T> colorful<T> light_aqua_on_bright_white(T t)    { return colorful<T> { colored<T> (t, "lq", "bw") }; }
    template<typename T> colorful<T> light_red_on_black(T t)            { return colorful<T> { colored<T> (t, "lr", "k") }; }
    template<typename T> colorful<T> light_red_on_blue(T t)             { return colorful<T> { colored<T> (t, "lr", "b") }; }
    template<typename T> colorful<T> light_red_on_green(T t)            { return colorful<T> { colored<T> (t, "lr", "g") }; }
    template<typename T> colorful<T> light_red_on_aqua(T t)             { return colorful<T> { colored<T> (t, "lr", "a") }; }
    template<typename T> colorful<T> light_red_on_red(T t)              { return colorful<T> { colored<T> (t, "lr", "r") }; }
    template<typename T> colorful<T> light_red_on_purple(T t)           { return colorful<T> { colored<T> (t, "lr", "p") }; }
    template<typename T> colorful<T> light_red_on_yellow(T t)           { return colorful<T> { colored<T> (t, "lr", "y") }; }
    template<typename T> colorful<T> light_red_on_white(T t)            { return colorful<T> { colored<T> (t, "lr", "w") }; }
    template<typename T> colorful<T> light_red_on_grey(T t)             { return colorful<T> { colored<T> (t, "lr", "e") }; }
    template<typename T> colorful<T> light_red_on_light_blue(T t)       { return colorful<T> { colored<T> (t, "lr", "lb") }; }
    template<typename T> colorful<T> light_red_on_light_green(T t)      { return colorful<T> { colored<T> (t, "lr", "lg") }; }
    template<typename T> colorful<T> light_red_on_light_aqua(T t)       { return colorful<T> { colored<T> (t, "lr", "lq") }; }
    template<typename T> colorful<T> light_red_on_light_red(T t)        { return colorful<T> { colored<T> (t, "lr", "lr") }; }
    template<typename T> colorful<T> light_red_on_light_purple(T t)     { return colorful<T> { colored<T> (t, "lr", "lp") }; }
    template<typename T> colorful<T> light_red_on_light_yellow(T t)     { return colorful<T> { colored<T> (t, "lr", "ly") }; }
    template<typename T> colorful<T> light_red_on_bright_white(T t)     { return colorful<T> { colored<T> (t, "lr", "bw") }; }
    template<typename T> colorful<T> light_purple_on_black(T t)         { return colorful<T> { colored<T> (t, "lp", "k") }; }
    template<typename T> colorful<T> light_purple_on_blue(T t)          { return colorful<T> { colored<T> (t, "lp", "b") }; }
    template<typename T> colorful<T> light_purple_on_green(T t)         { return colorful<T> { colored<T> (t, "lp", "g") }; }
    template<typename T> colorful<T> light_purple_on_aqua(T t)          { return colorful<T> { colored<T> (t, "lp", "a") }; }
    template<typename T> colorful<T> light_purple_on_red(T t)           { return colorful<T> { colored<T> (t, "lp", "r") }; }
    template<typename T> colorful<T> light_purple_on_purple(T t)        { return colorful<T> { colored<T> (t, "lp", "p") }; }
    template<typename T> colorful<T> light_purple_on_yellow(T t)        { return colorful<T> { colored<T> (t, "lp", "y") }; }
    template<typename T> colorful<T> light_purple_on_white(T t)         { return colorful<T> { colored<T> (t, "lp", "w") }; }
    template<typename T> colorful<T> light_purple_on_grey(T t)          { return colorful<T> { colored<T> (t, "lp", "e") }; }
    template<typename T> colorful<T> light_purple_on_light_blue(T t)    { return colorful<T> { colored<T> (t, "lp", "lb") }; }
    template<typename T> colorful<T> light_purple_on_light_green(T t)   { return colorful<T> { colored<T> (t, "lp", "lg") }; }
    template<typename T> colorful<T> light_purple_on_light_aqua(T t)    { return colorful<T> { colored<T> (t, "lp", "lq") }; }
    template<typename T> colorful<T> light_purple_on_light_red(T t)     { return colorful<T> { colored<T> (t, "lp", "lr") }; }
    template<typename T> colorful<T> light_purple_on_light_purple(T t)  { return colorful<T> { colored<T> (t, "lp", "lp") }; }
    template<typename T> colorful<T> light_purple_on_light_yellow(T t)  { return colorful<T> { colored<T> (t, "lp", "ly") }; }
    template<typename T> colorful<T> light_purple_on_bright_white(T t)  { return colorful<T> { colored<T> (t, "lp", "bw") }; }
    template<typename T> colorful<T> light_yellow_on_black(T t)         { return colorful<T> { colored<T> (t, "ly", "k") }; }
    template<typename T> colorful<T> light_yellow_on_blue(T t)          { return colorful<T> { colored<T> (t, "ly", "b") }; }
    template<typename T> colorful<T> light_yellow_on_green(T t)         { return colorful<T> { colored<T> (t, "ly", "g") }; }
    template<typename T> colorful<T> light_yellow_on_aqua(T t)          { return colorful<T> { colored<T> (t, "ly", "a") }; }
    template<typename T> colorful<T> light_yellow_on_red(T t)           { return colorful<T> { colored<T> (t, "ly", "r") }; }
    template<typename T> colorful<T> light_yellow_on_purple(T t)        { return colorful<T> { colored<T> (t, "ly", "p") }; }
    template<typename T> colorful<T> light_yellow_on_yellow(T t)        { return colorful<T> { colored<T> (t, "ly", "y") }; }
    template<typename T> colorful<T> light_yellow_on_white(T t)         { return colorful<T> { colored<T> (t, "ly", "w") }; }
    template<typename T> colorful<T> light_yellow_on_grey(T t)          { return colorful<T> { colored<T> (t, "ly", "e") }; }
    template<typename T> colorful<T> light_yellow_on_light_blue(T t)    { return colorful<T> { colored<T> (t, "ly", "lb") }; }
    template<typename T> colorful<T> light_yellow_on_light_green(T t)   { return colorful<T> { colored<T> (t, "ly", "lg") }; }
    template<typename T> colorful<T> light_yellow_on_light_aqua(T t)    { return colorful<T> { colored<T> (t, "ly", "lq") }; }
    template<typename T> colorful<T> light_yellow_on_light_red(T t)     { return colorful<T> { colored<T> (t, "ly", "lr") }; }
    template<typename T> colorful<T> light_yellow_on_light_purple(T t)  { return colorful<T> { colored<T> (t, "ly", "lp") }; }
    template<typename T> colorful<T> light_yellow_on_light_yellow(T t)  { return colorful<T> { colored<T> (t, "ly", "ly") }; }
    template<typename T> colorful<T> light_yellow_on_bright_white(T t)  { return colorful<T> { colored<T> (t, "ly", "bw") }; }
    template<typename T> colorful<T> bright_white_on_black(T t)         { return colorful<T> { colored<T> (t, "bw", "k") }; }
    template<typename T> colorful<T> bright_white_on_blue(T t)          { return colorful<T> { colored<T> (t, "bw", "b") }; }
    template<typename T> colorful<T> bright_white_on_green(T t)         { return colorful<T> { colored<T> (t, "bw", "g") }; }
    template<typename T> colorful<T> bright_white_on_aqua(T t)          { return colorful<T> { colored<T> (t, "bw", "a") }; }
    template<typename T> colorful<T> bright_white_on_red(T t)           { return colorful<T> { colored<T> (t, "bw", "r") }; }
    template<typename T> colorful<T> bright_white_on_purple(T t)        { return colorful<T> { colored<T> (t, "bw", "p") }; }
    template<typename T> colorful<T> bright_white_on_yellow(T t)        { return colorful<T> { colored<T> (t, "bw", "y") }; }
    template<typename T> colorful<T> bright_white_on_white(T t)         { return colorful<T> { colored<T> (t, "bw", "w") }; }
    template<typename T> colorful<T> bright_white_on_grey(T t)          { return colorful<T> { colored<T> (t, "bw", "e") }; }
    template<typename T> colorful<T> bright_white_on_light_blue(T t)    { return colorful<T> { colored<T> (t, "bw", "lb") }; }
    template<typename T> colorful<T> bright_white_on_light_green(T t)   { return colorful<T> { colored<T> (t, "bw", "lg") }; }
    template<typename T> colorful<T> bright_white_on_light_aqua(T t)    { return colorful<T> { colored<T> (t, "bw", "lq") }; }
    template<typename T> colorful<T> bright_white_on_light_red(T t)     { return colorful<T> { colored<T> (t, "bw", "lr") }; }
    template<typename T> colorful<T> bright_white_on_light_purple(T t)  { return colorful<T> { colored<T> (t, "bw", "lp") }; }
    template<typename T> colorful<T> bright_white_on_light_yellow(T t)  { return colorful<T> { colored<T> (t, "bw", "ly") }; }
    template<typename T> colorful<T> bright_white_on_bright_white(T t)  { return colorful<T> { colored<T> (t, "bw", "bw") }; }
}
