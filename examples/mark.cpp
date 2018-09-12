#include "../include/color.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <string>

using namespace std;

const set<string> TO_WATCH = {"google", "facebook", "microsoft", "twitter"};

void segment(const string & s, decltype(s.begin()) & a,  decltype(s.end()) & b)
{
    auto is_punct = [](char c) -> bool
                    {
                        auto punct = set<char>{',', '.', ':', ';', '?', '!',
                                               '\'', '\"', '(', ')'};
                        return punct.find(c) != punct.end();
                    };

    a = find_if_not(s.cbegin(), s.cend(), is_punct);
    b = find_if_not(s.crbegin(), s.crend(), is_punct).base();
    // if entire s is punct, this punct is deemed a post punct
    if (a == s.cend())
        a = s.cbegin();
}

void separate(const string & s, string & pre, string & word, string & post)
{
    auto a = s.cbegin(), b = s.cend();
    segment(s, a, b);
    pre = string(s.cbegin(), a);
    word = string(a, b);
    post = string(b, s.cend());
}

bool is_keyword(string s, const set<string> & names = TO_WATCH)
{
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return names.find(s) != names.end();
}

bool is_number(string s)
{
    if (s.empty())
        return false;
    auto a = s.cbegin(), b = s.cend();
    if (s.front() == '$')
        a = s.cbegin() + 1;
    if (s.back() == '%')
        b = s.cend() - 1;
    s = string(a, b);
    return !s.empty() && s.find_first_not_of(",.1234567890") == string::npos;
}

#if __cplusplus < 201402L
auto mark(const string & str, string color) -> decltype(dye::vanilla(""))
#else
auto mark(const string & str, string color)
#endif
{
    istringstream iss(str);
    auto marked = dye::vanilla("");
    for (string line; getline(iss, line); marked += "\n") {
        istringstream lineiss(line);
        for (string text; lineiss >> text; marked += " ") {
            string pre, word, post;
            // split a text into 3 parts: word in middle, and punctuations around it
            separate(text, pre, word, post);
            marked += pre;
            if (is_keyword(word))
                marked += dye::colorize(word, color).invert();
            else if (is_number(word))
                marked += dye::colorize(word, color);
            else
                marked += word;
            marked += post;
        }
    }
    return marked;
}

int main()
{
    auto tech_news = "Silicon Valley giants including Google, Facebook and "
                      "Twitter are headed to Capitol Hill. Here's what you "
                      "can expect from their hearings.";

    cout << mark(tech_news, "light_red") << endl;

    auto stock_news = "Shares in Twitter, Snap and Facebook all declined "
                      "significantly, dragging the Nasdaq down more than 1% "
                      "to below the 8,000 level.\n\nTwitter fell 6% to end the "
                      "session at $32.17. \n\nFacebook saw its shares fall more "
                      "than 2% to finish at $167.18.\n\nGoogle parent, "
                      "Alphabet, came in for a drubbing during the hearing "
                      "because it declined to accept the committes's "
                      "invitation to testify. The company's stock slipped "
                      "nearly 1% to close at 1186.48.\n\nMicrosoft fell almost "
                      "3% on the day, to $108.49.";

    cout << mark(stock_news, "yellow") << endl;

    return 0;
}
