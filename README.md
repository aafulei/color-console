# Color Console

A lightweight header-only C++ library to bring colors to your Windows console with a very-easy-to-use API that frees you from the burden of setting and resetting with colors every time you make a call.

<img src="image/tech.png" width="600"/>

## Contents

- [Installation](#user-content-instllation)
- [How to Use](#user-content-how-to-use)
- [Why Use It?](#user-content-why-use-it)
- [A Real Example](#user-content-a-real-example)

## Installation

Put [`color.hpp`](include/color.hpp) in the folder where you include headers.

## How to Use?

```c++
#include "../include/color.hpp"
#include <iostream>

int main()
{
    std::cout << dye::aqua("Hello, World!") << std::endl;
    return 0;
}
```

You are seeing `Hello, World!` in aqua.

<img src="image/hello.png" width="150"/> 

## Why Use It?

<img src="image/useng" width="400"/>

1. **No need to reset :**  most solutions on the market work like manipulators, which *constantly* require you to do a resetting whenever you make a setting. While this traditional approach is also offered in this library in the `hue` namespace

   ```c++
   cout << hue::red << "When in doubt, wear red." << hue::reset << endl;
   cout << hue::green << "When you're green, you're growing." << hue::reset << endl;
   ```

   it can be boring to do so. Why not just `dye` with

   ```c++
   cout << dye::red("When in doubt, wear red.") << endl;
   cout << dye::green("When you're green, you're growing.") << endl;
   ```

2. **Object-oriented :**  so that you may `dye` an object and save it for later output

   ```c++
   auto obj = dye::aqua("a light bluish-green color");
   cout << obj << endl;
   ```

3. **`dye` anything :**  

   ```c++
   cout << dye::blue(42 + 7 % 8) << endl;
   cout << dye::yellow(string("It shed a yellow light.")) << endl;
   ```

   In fact, you can `dye` any object for which `operator<<` is properly defined can be dyed.

   With properly defined and declared

   ```c++
   struct DoubleVector;
   ostream & operator<<(ostream &, const DoubleVector &);
   ```

    we are free to call with

    ```c++
   cout << dye::purple(DoubleVector{3.14, 2.72}) << endl;
    ```


4. **`+` supported, even with different colors :**

   ```c++
   cout << dye::light_red('A') + dye::light_blue('B') + dye::light_green('C') << endl;
   ```

5. **Extra support for strings :**  be it `std::string` or C-style strings, dyed or undyed, strings can link up easily.

   ```c++ 
   const char ca[] = "ca";
   string str = "str";
   cout << "[ " + dye::aqua(ca) + " | " + dye::aqua(str) + " ]" << endl;
   ```

6. **Convenient and extensible API :**  In the following code, `colorize` takes colors as parameters, while `inverse` method quickly gets you the inversed color.

   ```c++
   double a = 88.88;
   cout << dye::colorize(a, a >= 0 ? "red" : "green").inverse() << endl;
   ```

## A Real Example

With [Color Console](include/color.hpp), we implement an [auto marker](examples/mark.cpp) which highlights keywords given in a watch list and colorizes numbers as well. The key function is

```c++
using namespace std;

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

To mark the introductory paragraph of a tech news

```c++
cout << mark(tech_news, "light_red") << endl;
```

We are having

<img src="image/tech.png" width="600"/> 

As another example in which we mark both keywords and numbers

```c++
cout << mark(stock_news, "yellow") << endl;
```

We are having

<img src="image/stock.png" width="600"/> 


