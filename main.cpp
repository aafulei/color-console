#include "include/color.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

const set<string> TO_WATCH = {"google", "facebook", "microsoft", "twitter"};

bool is_highlight(string s, const set<string> & tw = TO_WATCH)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    if (!s.empty())
        if (std::ispunct(s.front())) {
            s.erase(s.begin());
        }
        if (std::ispunct(s.back()))
            s.pop_back();
    return tw.find(s) != tw.end();
}



auto mark(const string & str) -> decltype(dye::nothing(""))
{
    istringstream iss(str);
    string line;


    auto marked = dye::nothing("");

    while (getline(iss, line)) {
        istringstream liss(line);
        string s;
        while (liss >> s) {
            if (is_highlight(s)) {
                if (std::ispunct(s.back())) {
                    marked += dye::black_on_light_red(s.substr(0, s.size() - 1));
                    marked += dye::nothing(s.substr(s.size() - 1, 1));
                }
                else
                    marked += dye::black_on_light_red(s);
            }
            else
                marked += dye::nothing(s);
            marked += dye::nothing(" ");
        }
        marked += dye::nothing("\n");
    }

    return marked;
}


int main()
{
    auto str = "Today, Google, Facebook, Microsoft, and Twitter joined to announce a new standards initiative called the Data Transfer Project, designed as a new way to move data between platforms. In a blog post, Google described the project as letting users “transfer data directly from one service to another, without needing to download and re-upload it.” The current version of the system supports data transfer for photos, mail, contacts, calendars, and tasks, drawing from publicly available APIs from Google, Microsoft, Twitter, Flickr, Instagram, Remember the Milk, and SmugMug. Many of those transfers could already be accomplished through other means, but participants hope the project will grow into a more robust and flexible alternative to conventional APIs. In its own blog post, Microsoft called for more companies to sign onto the effort, adding that “portability and interoperability are central to cloud innovation and competition.” The existing code for the project is available open-source on GitHub, along with a white paper describing its scope. Much of the codebase consists of “adapters” that can translate proprietary APIs into an interoperable transfer, making Instagram data workable for Flickr and vice versa. Between those adapters, engineers have also built a system to encrypt the data in transit, issuing forward-secret keys for each transaction. Notably, that system is focused on one-time transfers rather than the continuous interoperability enabled by many APIs.";

    auto marked = mark(str);
    std::cout << marked << std::endl;


    return 0;
}