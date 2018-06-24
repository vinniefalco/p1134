#
# Copyright (c) 2016-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/boostorg/beast
#

cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)
project(p1134 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_definitions(-D_WIN32_WINNT=0x0601)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS=1)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
  add_definitions(-D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING)
  add_compile_options(
      /bigobj       # large object file format
      #/permissive-  # strict C++
      #/wd4503      # decorated name length exceeded, name was truncated
      /W4           # enable all warnings
      /MP           # Multi-processor compilation
      )
  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ob2 /Oi /Ot /GL /MT")
  set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Oi /Ot /MT")

  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
  set (CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER build)

#
# boost
#

set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME ON)
add_definitions (-DBOOST_ALL_STATIC_LINK=1)
add_definitions (-DBOOST_COROUTINES_NO_DEPRECATION_WARNING=1)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

#
# common
#

include_directories(./include)

file(GLOB APP_FILES
  main.cpp
  README.md)

file(GLOB REXP_FILES
  lib/waiter.cpp)

source_group("" FILES ${APP_FILES})

add_executable(main ${APP_FILES} ${REXP_FILES})

if(NOT WIN32)
  find_package(Threads)
  target_link_libraries(main PRIVATE Threads::Threads)
endif()

set_target_properties(main PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY
  VS_STARTUP_PROJECT main)
