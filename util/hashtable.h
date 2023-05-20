/*
 * MIT License
 *
 * Copyright (c) 2023 Ricardo Fares
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>
#include "../semantic/semantic.h"

/**
 * A singly-linked hash table node.
 * <p>
 * This hash table node stores the key-value pair
 * as well as the next node in the list.
 */
struct node {
    const char              *key;
    struct var_descriptor_t  descriptor;
    struct node             *next;
};

/**
 * A hash table.
 * <p>
 * This hash table implementation use <b>chaining</b>
 * to resolve the <em>collisions</em>.
 * <p>
 * Moreover, this hash table has been implemented using an
 * aggressive way to handle exceptional cases, in any of
 * these cases the application is immediately terminated.
 * For instance, an exceptional case can be either not
 * enough memory to allocate a hash table or not enough
 * memory to resize it.
 * <p>
 * Finally, it must be said that there is no need for
 * the hash table users to access or modify directly the
 * fields of the <em>hashtable</em> structure. Therefore, it
 * is recommended to use only the functions provided for access
 * and modification.
 */
struct hashtable {
    struct node  **table;
    size_t         size;
    size_t         capacity;
    float          lf;
};

/* Hash Table Function Declaration */

/**
 * It allocates a <em>hashtable</em> structure and returns a pointer
 * to the allocated memory.
 * <p>
 * If there is not enough memory to allocate the <em>hashtable</em>
 * structure or allocate the nodes to satisfy the <em>initial capacity</em>,
 * then the application is immediately terminated.
 *
 * @param initial_capacity the initial capacity
 *
 * @return a pointer to the memory allocated for the <em>hashtable</em>
 */
struct hashtable *
hashtable_new(size_t initial_capacity);

/**
 * It inserts a key-value pair into the hash table.
 *
 * @param ht    the hash table
 * @param key   the key
 * @param value the value
 */
void
hashtable_insert(struct hashtable *ht, const char *key, struct var_descriptor_t *value);

/**
 * It returns a pointer to the value mapped by key in this hash table.
 * <p>
 * If the pointer is NULL, then the mapping does not exist in the
 * specified hash table.
 *
 * @param ht  the hash table
 * @param key the key
 *
 * @return a pointer to the mapped value by the key or NULL if there
 *         is no mapping
 */
struct var_descriptor_t *
hashtable_find(struct hashtable *ht, const char *key);

/**
 * It removes the key-value pair associated with the specified key
 * in the specified hash table. If the mapping does exist, then
 * the value is placed in the specified <em>placeholder</em>.
 *
 * @param ht          the hash table
 * @param key         the key
 * @param placeholder the placeholder
 */
void
hashtable_remove(struct hashtable *ht, const char *key, struct var_descriptor_t *placeholder);

#endif // HASHTABLE_H
