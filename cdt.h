#ifndef _CYC_CDT_H
#define _CYC_CDT_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
typedef size_t usize;
typedef ptrdiff_t isize;
typedef float  f32;
typedef double f64;

#define U8_MIN 0u
#define U8_MAX 0xffu
#define I8_MIN (-0x7f - 1)
#define I8_MAX 0x7f

#define U16_MIN 0u
#define U16_MAX 0xffffu
#define I16_MIN (-0x7fff - 1)
#define I16_MAX 0x7fff

#define U32_MIN 0u
#define U32_MAX 0xffffffffu
#define I32_MIN (-0x7fffffff - 1)
#define I32_MAX 0x7fffffff

#define U64_MIN 0ull
#define U64_MAX 0xffffffffffffffffull
#define I64_MIN (-0x7fffffffffffffffll - 1)
#define I64_MAX 0x7fffffffffffffffll

#define F32_MIN 1.17549435e-38f
#define F32_MAX 3.40282347e+38f

#define F64_MIN 2.2250738585072014e-308
#define F64_MAX 1.7976931348623157e+308

void cdt_error_handler(const char *file, i32 line, const char *pre_msg, const char *msg, ...);

#define CDT_ASSERT assert
#define CDT_TRAP() abort()

#define cast(T) (T)
#define unused (void)

#define panic(msg, ...) do { \
    cdt_error_handler(__FILE__, __LINE__, NULL, msg, ##__VA_ARGS__); \
    CDT_TRAP(); \
} while(0)

#define todo(msg, ...) do { \
    cdt_error_handler(__FILE__, __LINE__, "not yet implemented", msg, ##__VA_ARGS__); \
    CDT_TRAP(); \
} while(0)

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define clamp(x, lower, upper) min(max((x), (lower)), (upper))

//TODO: implement proper allocator
#define create(T) malloc(sizeof(T))
#define destroy(x) free(x)

//TODO: implement string builder fully
typedef struct String_Builder {

} String_Builder;

//TODO: implement string view fully
typedef struct String_View {
    const char *bytes;
    usize count;
} String_View;

//TODO: add formatter
#define sv(x) sv_from_parts(x, strlen(x))
#define sv_arg(x) cast(i32)(x.count), (x.bytes)

String_View sv_from_parts(const char *bytes, usize count);

#endif


#ifdef CDT_IMPL

void cdt_error_handler(const char *file, i32 line, const char *pre_msg, const char *msg, ...) {
    fprintf(stderr, "thread panicked at %s:%d:\n", file, line);
    if (pre_msg) {
        fprintf(stderr, "%s: ", pre_msg);
    }
    if (msg) {
        va_list va;
        va_start(va, msg);
        vfprintf(stderr, msg, va);
        va_end(va);
    } else {
        fprintf(stderr, "error!");
    }
    fprintf(stderr, "\n");
}

String_View sv_from_parts(const char *bytes, usize count) {
    String_View sv = {
        bytes,
        count,
    };
    return sv;
}
//NOTE: do i really need that for include order?
#undef CDT_IMPL
#endif
