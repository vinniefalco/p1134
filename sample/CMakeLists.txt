#
# Copyright (c) 2018 Vinnie Falco (vinnie dot falco at gmail dot com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/vinniefalco/p1134
#

source_group("" FILES hello_co_yield_1.cpp)
add_executable (hello-co-yield-1 hello_co_yield_1.cpp)
set_property(TARGET hello-co-yield-1 PROPERTY FOLDER "sample")
