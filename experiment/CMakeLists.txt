#
# Copyright (c) 2018 Vinnie Falco (vinnie dot falco at gmail dot com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/vinniefalco/p1134
#

source_group("" FILES hello_generator_1.cpp)
add_executable (hello-generator-1 hello_generator_1.cpp)
set_property(TARGET hello-generator-1 PROPERTY FOLDER "experiment")
