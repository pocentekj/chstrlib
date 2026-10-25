#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistr.h>
#include <unitypes.h>
#include <uniwidth.h>

#include <klib/kvec.h>

typedef char *string;

/**
 * This function takes pointer to string - it's useful for `deferred` callback.
 */
static inline void free_string(string *s) {
    if (!s || !*s) return;
    free(*s);
    *s = NULL;
}

static inline size_t string_width(const string s) {
    return u8_strwidth((const uint8_t *)s, "UTF-8");
}

static inline bool string_utf8_valid(const string s) {
    const uint8_t *txt = (const uint8_t *)s;
    return u8_check(txt, u8_strlen(txt)) == NULL;
}

/**
 * Truncate in place string `s` to visible columns `maxcols`.
 * Returns 'true' if string has been modified, `false` otherwise.
 */
bool string_truncate(string s, size_t maxcols);

/**
 * Check if `s` starts with `search`.
 */
bool starts_with(const string s, const string search);

/**
 * Read entire file on `path` into string.
 *
 * If `max_size` is provided, it will be used as the upper size limits for files
 * that should be loaded. This function will reject bigger files and return NULL.
 * If `max_size` is NULL, the upper limit will be set to SIZE_MAX -1.
 *
 * Returns pointer to string on success, NULL otherwise. Note, that the pointer
 * returned must be released by calling `free` (or `free_string(&ptr)`).
 */
string string_from_file(const string path, size_t *max_size);

typedef struct {
    kvec_t(string) data;
} string_array;

string_array *string_array_new(void);

/**
 * This function takes pointer to pointer as an argument - to be used as
 * a callback for `deferred` macro.
 */
void string_array_destroy(string_array **arr);

static inline size_t string_array_len(const string_array *arr) {
    assert(arr);
    return arr ? kv_size(arr->data) : 0;
}

static inline string string_array_at(const string_array *arr, size_t i) {
    assert(arr);
    return (arr && i < kv_size(arr->data)) ? kv_A(arr->data, i) : NULL;
}

/**
 * Read entire file on `path` line by line, and store lines in `out` string_array.
 * Returns `true` on success, `false` on error. `errno` may be set by underlying
 * functions, such as `fopen`.
 */
bool read_lines(string path, string_array *out);

bool ch_parse_double(const string s, double *dest);
bool ch_parse_ll(const string s, long long *dest);
bool ch_parse_int(const string s, int *dest);
