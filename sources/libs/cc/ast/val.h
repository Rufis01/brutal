#pragma once

#include <brutal/base.h>
#include <cc/ast/ref.h>

typedef int CSigned;
typedef unsigned int CUnsigned;

typedef enum
{
    CVAL_INVALID,

    CVAL_SIGNED,
    CVAL_UNSIGNED,
    CVAL_FLOAT,
    CVAL_STRING,
    CVAL_COUNT,
} CValType;

typedef struct
{
    CREF;
    CValType type;

    union
    {
        CSigned signed_;
        CUnsigned unsigned_;
        double float_;
        Str string_;
    };
} CVal;

Str cval_type_to_str(CValType type);
