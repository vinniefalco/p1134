//
// Copyright (c) 2018 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/vinniefalco/p1134
//

#include "generator.hpp"

#include <experimental/coroutine>
#include <cstdio>

cppcoro::generator<const char>
hello(const char* p)
{
    while (*p) {
        co_yield *p++;
    }
}

int main()
{
    for (const auto c : hello("Hello, world")) {
        std::putchar(c);
    }
}
