#include "color.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <string>

const std::set<std::string> TO_WATCH = {"google", "facebook", "microsoft", "twitter"};

void segment(const std::string & s, decltype(s.begin()) & a,  decltype(s.end()) & b)
{
    auto is_punct = [](char c) -> bool
                    {
                        auto punct = std::set<char>{',', '.', ':', ';', '?', '!', '\'', '\"', '(', ')'};
                        return punct.find(c) != punct.end();
                    };
    a = std::find_if_not(s.cbegin(), s.cend(), is_punct);
    b = std::find_if_not(s.crbegin(), s.crend(), is_punct).base();
    // if entire s is punct, this punct is deemed a post punct
    if (a == s.cend())
        a = s.cbegin();
}

void split(const std::string & s, std::string & pre, std::string & mid, std::string & post)
{
    auto a = s.cbegin(), b = s.cend();
    segment(s, a, b);
    pre = std::string(s.cbegin(), a);
    mid = std::string(a, b);
    post = std::string(b, s.cend());
}

bool is_keyword(std::string s, const std::set<std::string> & names = TO_WATCH)
{
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return names.find(s) != names.end();
}

bool is_number(std::string s)
{
    if (s.empty())
        return false;
    auto a = s.cbegin(), b = s.cend();
    if (s.front() == '$')
        a = s.cbegin() + 1;
    if (s.back() == '%')
        b = s.cend() - 1;
    s = std::string(a, b);
    return !s.empty() && s.find_first_not_of(",.1234567890") == std::string::npos;
}

#if __cplusplus < 201402L
auto mark(const std::string & news, std::string color) -> decltype(dye::vanilla(""))
#else
auto mark(const std::string & news, std::string color)
#endif
{
    std::istringstream iss(news);
    auto marked = dye::vanilla("");
    for (std::string line; std::getline(iss, line); marked += "\n") {
        std::istringstream lineiss(line);
        for (std::string word; lineiss >> word; marked += " ") {
            std::string pre, mid, post;
            // split a word into 3 parts: letters/numbers in middle, and punctuations around them
            split(word, pre, mid, post);
            if (!pre.empty())
                marked += pre;
            if (is_keyword(mid))
                marked += dye::colorize(mid, color).reverse();
            else if (is_number(mid))
                marked += dye::colorize(mid, color);
            else
                marked += mid;
            if (!post.empty())
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
    std::cout << mark(tech_news, "light_red") << std::endl;
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
    std::cout << mark(stock_news, "yellow") << std::endl;
    return 0;
}
