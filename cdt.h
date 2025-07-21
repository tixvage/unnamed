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

//TODO: implement string builder fully
typedef struct String_View {
    const char *bytes;
    usize count;
} String_View;

#define sv(x) sv_from_parts(x, strlen(x))

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

#endif
