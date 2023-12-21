#ifndef VEC_H_INCLUDED
#define VEC_H_INCLUDED

#include <stddef.h>  // size_t
#include <stdbool.h> // bool

/// Creates new vector for the given type
#define VEC_NEW(type) ((Vec) { .item_size = sizeof(type) })

/// l-value of vector at the given index
#define VEC_AT(type, vec, index) (((type *)(vec).data)[index])

/// Pushes new value to the vector
#define VEC_PUSH(type, vec, value) do { \
    Vec *__vec = (vec); \
    if (vec_reserve(__vec, 1)) { \
        VEC_AT(type, *__vec, __vec->len) = value; \
        ++__vec->len; \
    } \
} while (0)

/// Returns the last item from the vector, and removes it.
#define VEC_POP(type, vec) (*(type *)vec_pop(vec))

/// Makes the vector size so that it can hold exactly the given number of items
/// and fills the new items (if any) with the value of `item`
#define VEC_EXTEND_EXACT(type, vec, size, value) do { \
    Vec *__vec = (vec); \
    vec_resize_exact(__vec, (size)); \
    while (__vec->len < __vec->alloc) { \
        VEC_AT(type, *__vec, __vec->len) = value; \
        ++__vec->len; \
    } \
} while (0)

#define VEC_FOR_EACH(vec, type, name) for ( \
    struct { \
        Vec *__vec; \
        size_t i; \
        type *v; \
    } name = { .__vec = (Vec *)(vec), .i = 0 }; \
    name.i < name.__vec->len \
        && (name.v = &VEC_AT(type, *name.__vec, name.i), true); \
    ++name.i \
)

typedef struct {
    size_t item_size;
    size_t alloc;
    size_t len;
    char *data;
} Vec;

/// Creates new vector
Vec vec_new(size_t item_size);

/// Appends value to the vector
bool vec_push(Vec *vec, const void *item);

/// Returns the last item from the vector, and removes it.
/// Returns null if vector is empty.
void *vec_pop(Vec *vec);

/// Gets pointer to value at the given index
void *vec_at(const Vec vec, size_t index);

/// Removes all items from the vector
void vec_clear(Vec *vec);

/// Makes sure that at least `count` elements can be inserted before
/// reallocation.
bool vec_reserve(Vec *vec, size_t count);

/// Makes the vector size so that it can hold exactly the given number of items
bool vec_resize_exact(Vec *vec, size_t len);

/// Makes the vector size so that it can hold exactly the given number of items
/// and fills the new items (if any) with the value of `item`
bool vec_extend_exact(Vec *vec, size_t len, void *item);

/// Frees all data allocated by the vector
void vec_free(Vec *vec);

#endif // VEC_H_INCLUDED
