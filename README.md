# Color Console

A lightweight header-only C++ library to bring colors to your Windows console with a very-easy-to-use API that frees you from the burden of setting and resetting with colors every time you make a call.

## Installation

Put [`color.hpp`](include/color.hpp) in the folder where you include headers.

## How to Use?

```c++
#include "include/color.hpp"
#include <iostream>

int main()
{
    std::cout << dye::aqua("Hello, World!") << std::endl;
    return 0;
}
```

You are seeing `Hello, World!` in aqua.

<img src="image/hello.png" width="150"/> 

## Why Use it?

1. **Object-oriented, not console-oriented:** most current solutions work like manipulators which *constantly* requires you to do a resetting after a setting. While this traditional approach is also offered in this library (in the `hue` namespace)

   ```c++
   std::cout << hue::red << "When in doubt, wear red." << hue::reset << std::endl;
   std::cout << hue::blue << "Blue jeans. Coca-Cola." << hue::reset << std::endl;
   std::cout << hue::green << "When you're green, you're growing." << hue::reset << std::endl;
   ```

   it can be boring to do so. Why not just

   ```c++
   std::cout << dye::red("When in doubt, wear red.") << std::endl;
   std::cout << dye::blue("Blue jeans. Coca-Cola.") << std::endl;
   std::cout << dye::green("When you're green, you're growing.") << std::endl;
   ```

2. **You can `dye` anything that can be output:** `std::string`, C-style strings, `int`, `double`, you name it ...

   ```c++
   std::cout << dye::light_red(3.14) << std::endl;
   std::cout << dye::light_blue(42 + 7 % 8) << std::endl;
   
   const char ca[] = "To Gatsby, the green light represents his dream.";
   std::cout << dye::light_green(ca) << std::endl
   ```

3. **Concatenation supported between dyed objects, even with different colors**

4. **Extra support for strings**

## More Examples

Consider a marker that takes in a `std::string` and marks the keywords and numbers with the given `color`.

```c++
auto mark(const string & str, string color)
{
    istringstream iss(str);
    auto marked = dye::vanilla("");
    for (string line; getline(iss, line); marked += "\n\n") {
        istringstream lineiss(line);
        for (string text; lineiss >> text; marked += " ") {
            string pre, word, post;
            // split a text into 3 parts: word in middle, and punctuations around it
            separate(text, pre, word, post);
            marked += pre;
            if (is_keyword(word))
                marked += dye::colorize(word, color).inverse();
            else if (is_number(word))
                marked += dye::colorize(word, color);
            else
                marked += word;
            marked += post;
        }
    }
    return marked;
}
```

Suppose we call with

```c++
auto tech_news_marked = mark(tech_news, "light_red");
cout << endl << tech_news_marked << endl;
```

Then we have

<img src="image/tech_news_marked.png" width="600"/> 

As another example which we mark both keywords and numbers

```c++
auto stock_news_marked = mark(stock_news, "yellow");
cout << endl << stock_news_marked << endl;
```

We are having

<img src="image/stock_news_marked.png" width="600"/> 


