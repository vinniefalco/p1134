//
// Copyright (c) 2018 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/vinniefalco/p1134
//

#include <cassert>
#include <type_traits>
#include <utility>
#include <boost/asio/coroutine.hpp>

//
// Based on work written by Christopher Kohlhoff
//

//------------------------------------------------------------------------------
// Emulate resumable expressions using Asio's stackless coroutine.

template <class Body>
class resumable_t
{
public:
    static_assert(std::is_base_of_v<boost::asio::coroutine, Body>);

    using result_type = void;

    template <class... Args>
    explicit resumable_t(Args&&... args)
        : body_(std::forward<Args>(args)...)
    {
    }

    resumable_t(const resumable_t&) = delete;
    resumable_t& operator=(const resumable_t&) = delete;

    void resume()
    {
        body_();
    }

    bool ready() const noexcept
    {
        return body_.is_complete();
    }

    void result()
    {
    }

private:
    Body body_;
};

//------------------------------------------------------------------------------
// Adapt an emulated resumable expression as a generator.

void*& generated_result()
{
    /* resumable_local */ thread_local void* ptr;
    return ptr;
}

template <class T>
inline const int generated_type_marker{};

const int*& generated_type()
{
    /* resumable_local */ thread_local const int* ptr;
    return ptr;
}

template <class T, class Body>
class generator_t
{
public:
    static_assert(std::is_base_of_v<boost::asio::coroutine, Body>);

    generator_t(Body b)
        : resumable_(result_, std::move(b))
    {
        resumable_.resume();
    }

    class end_t {};

    class iterator
    {
    public:
        T operator*()
        {
            return *static_cast<T*>(*self_.result_);
        }

        void operator++()
        {
            self_.resumable_.resume();
        }

        bool operator==(end_t) const
        {
            return self_.resumable_.ready();
        }

        bool operator!=(end_t) const
        {
            return !self_.resumable_.ready();
        }

    private:
        friend class generator_t;

        iterator(generator_t& self)
            : self_(self)
        {
        }

        generator_t& self_;
    };

    iterator begin()
    {
        return *this;
    }

    end_t end()
    {
        return {};
    }

private:
    // Emulates a resumable function that does some initial setup, executed as
    // part of the resumable expression so it gets access to resumable-local
    // storage, before calling the user-supplied body.
    class entry_point : public boost::asio::coroutine
    {
    public:
        entry_point(void**& result, Body b)
            : result_(result),
            body_(std::move(b))
        {
        }

        void operator()()
        {
            BOOST_ASIO_CORO_REENTER(this)
            {
                result_ = &generated_result();
                generated_type() = &generated_type_marker<T>;

                // This loop emulates a nested call to a resumable function.
                for (body_(); !body_.is_complete(); body_()) BOOST_ASIO_CORO_YIELD;
            }
        }

    private:
        void**& result_;
        Body body_;
    };

    void** result_;
    resumable_t<entry_point> resumable_;
};

template<class T, class Body>
generator_t<T, Body> generator(Body b)
{
    return generator_t<T, Body>(std::move(b));
}

// yield() is a resumable function.
template<class T>
struct yield_t
{
    // "Stack" storage for the result. When we update the resumable-local pointer
    // to this location, the generator automatically sees the result.
    T value_;

    void operator()(T value)
    {
        assert(generated_type() == &generated_type_marker<T>);
        value_ = std::move(value);
        generated_result() = &value_;
    }
};

//------------------------------------------------------------------------------

#include <cstdio>

auto hello(const char* p)
{
    struct hello_t : boost::asio::coroutine
    {
        const char* p;
        yield_t<char> yield;

        hello_t(const char* captured_p)
            : p(captured_p)
        {
        }

        void operator()()
        {
            BOOST_ASIO_CORO_REENTER(this)
            {
                while (*p)
                    BOOST_ASIO_CORO_YIELD yield(*p++);
            }
        }
    };

    return generator<char>(hello_t(p));
}

int main()
{
    for (auto c : hello("Hello, world\n"))
        putchar(c);
}

