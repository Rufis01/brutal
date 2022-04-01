#pragma once

#include <bruwutal/base/count.h>
#include <bruwutal/base/macros.h>
#include <bruwutal/base/map.h>
#include <bruwutal/base/std.h>
#include <bruwutal/base/vaopt.h>
#include <bruwutal/text/str.h>

typedef enum
{
    ANY_INT,
    ANY_UINT,
    ANY_FLOAT,
    ANY_STR,
    ANY_PTR,
} AnyType;

typedef struct
{
    AnyType type;

    union
    {
        int64_t int_;
        uint64_t uint_;
#ifndef __freestanding__
        double float_;
#endif
        Str str_;
        void *ptr_;
    };
} Any;

static inline bool any_is(Any any, AnyType type)
{
    return any.type == type;
}

static inline Any any_int(int64_t i)
{
    return (Any){.type = ANY_INT, .int_ = i};
}

static inline Any any_uint(uint64_t u)
{
    return (Any){.type = ANY_UINT, .uint_ = u};
}

#ifndef __freestanding__

static inline Any any_float(double f)
{
    return (Any){.type = ANY_FLOAT, .float_ = f};
}

#endif

static inline Any any_str(Str s)
{
    return (Any){.type = ANY_STR, .str_ = s};
}

static inline Any any_cstr(char const *s)
{
    return any_str(str$(s));
}

static inline Any any_ptr(void *p)
{
    return (Any){.type = ANY_PTR, .ptr_ = p};
}

// clang-format off

#ifndef __freestanding__

#define any$(EXPR)                    \
    _Generic((EXPR),                  \
        _MatchSigned(any_int),        \
        _MatchUnsigned(any_uint),     \
        _MatchFloat(any_float),       \
                                      \
        char*: any_cstr,              \
        char const*: any_cstr,        \
        Str: any_str,                 \
        void*: any_ptr                \
    )(EXPR)

#else

#define any$(EXPR)                    \
    _Generic((EXPR),                  \
        _MatchSigned(any_int),        \
        _MatchUnsigned(any_uint),     \
                                      \
        char*: any_cstr,              \
        char const*: any_cstr,        \
        Str: any_str,                 \
        void*: any_ptr                \
    )(EXPR)

#endif

// clang-format on

typedef struct
{
    Any *buf;
    size_t len;
} AnyVa;

#define __any_va_empty$(...) \
    (AnyVa) { .buf = nullptr, .len = 0 }

#define __any_va_impl$(...) \
    (AnyVa) { (Any[]){MAP_LIST(any$, __VA_ARGS__)}, COUNT(__VA_ARGS__) }

#define any_va$(...)  \
    IFNE(__VA_ARGS__) \
    (__any_va_impl$, __any_va_empty$)(__VA_ARGS__)
