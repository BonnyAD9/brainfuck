#include "vec.h" // Vec

#include <stdlib.h>  // size_t, realloc, free, NULL
#include <stdbool.h> // bool, true, false
#include <string.h>  // memcpy

#include "err.h" // set_err_msg, FAILED_ALLOC

static bool vec_err(void) {
    set_err_msg(FAILED_ALLOC, "Vec failed to allocate memory.");
    return false;
}

Vec vec_new(size_t item_size) {
    return (Vec) {
        .item_size = item_size,
    };
}

bool vec_push(Vec *vec, const void *item) {
    if (!vec_reserve(vec, 1)) {
        return false;
    }

    memcpy(vec_at(*vec, vec->len), item, vec->item_size);
    return true;
}

void *vec_at(const Vec vec, size_t index) {
    return vec.data + vec.item_size * index;
}

void vec_clear(Vec *vec) {
    vec->len = 0;
}

bool vec_reserve(Vec *vec, size_t count) {
    size_t min_alloc = vec->len + count;
    size_t new_alloc = vec->alloc;
    if (min_alloc <= new_alloc) {
        return true;
    }

    if (new_alloc == 0 && min_alloc != 0) {
        ++new_alloc;
    }

    while (new_alloc < min_alloc) {
        new_alloc *= 2;
    }

    char *new_data = realloc(vec->data, new_alloc);
    if (!new_data) {
        return vec_err();
    }

    vec->data = new_data;
    vec->alloc = new_alloc;

    return true;
}

bool vec_resize_exact(Vec *vec, size_t len) {
    char *new_data = realloc(vec->data, vec->item_size * len);
    if (!new_data) {
        return vec_err();
    }

    vec->data = new_data;
    vec->alloc = len;

    if (len < vec->len) {
        vec->len = len;
    }

    return true;
}

bool vec_extend_exact(Vec *vec, size_t len, void *item) {
    if (!vec_resize_exact(vec, len)) {
        return false;
    }

    while (vec->len < vec->alloc) {
        vec_push(vec, item);
    }

    return true;
}

void vec_free(Vec *vec) {
    free(vec->data);
    vec->data = NULL;
    vec->alloc = 0;
    vec->len = 0;
}
