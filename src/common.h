///
///@file types.h
///@author Amrit Bhogal
///@version 1.0.0 - Amrit Bhogal on 2022-10-18
///

#pragma once

#include <stdint.h>
#include <stddef.h>

#include "api.h"

//Clion refuses to work with local functions, so we have to make it think its using clang blocks
#if defined(__CLION_IDE__)
#   define $(ret, args, ...) ((__typeof__(ret (*)args))^ret args __VA_ARGS__)
#else
//If this project used an actual good compiler (clang) I would use blocks (like a sane person!)

#   define $(ret, args, ...) ({ ret _fn_tmp args __VA_ARGS__; _fn_tmp; })

#endif

typedef uint8_t     uint8;
typedef uint16_t    uint16;
typedef uint32_t    uint32;
typedef uint64_t    uint64;

typedef int8_t      int8;
typedef int16_t     int16;
typedef int32_t     int32;
typedef int64_t     int64;

typedef float       float32;
typedef double      float64;
typedef long double float128;

typedef uint8_t Port_t;

enum State {
    on = true,
    off = false
};

typedef struct {
    uint32 x, y;
} Point_t;

__printflike(2, 3)
static void println(uint8 ln, const char *restrict fmt, ...)
{
    char buf[BUFSIZ] = {0};
    va_list l;
    va_start(l, fmt);
    int32   writtenc = vsnprintf(buf, BUFSIZ, fmt, l),
        siz = BUFSIZ - writtenc;
    va_end(l);

    if (siz != 0) {
        char spc[siz];
        memset(spc, ' ', siz);
        spc[siz] = '\0';
        strncat(buf, spc, siz);
    }

    controller_set_text(E_CONTROLLER_MASTER, ln, 0, buf);
}

#define print(...) println(0, __VA_ARGS__)


