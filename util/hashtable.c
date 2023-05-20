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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./hashtable.h"

/**
 * It calculates a hash code for the specified key.
 * <p>
 * <b>Implementation Notes:</b>
 * This function implements the DJB2 Hash Algorithm.
 *
 * @param ht_cap the hash table capacity
 * @param key    the key to be hashed
 *
 * @return the key hash code
 */
static size_t
hash(const size_t ht_cap, const char *key) {
    size_t hash = 5381;
    char c;
    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;
    return hash % ht_cap;
}

/**
 * It resizes the specified hash table.
 * <p>
 * If the new hash table capacity exceeds the maximum size
 * or if there is not enough memory to resize the hash table,
 * then the application is immediately terminated.
 *
 * @param ht the hash table to be resized
 */
static void
resize(struct hashtable *ht) {
    size_t old_cap = ht->capacity;
    size_t new_cap = old_cap << 1; /* newCap = 2 oldCap */
    /* It checks if the resizing exceeds the maximum size */
    if (new_cap < old_cap) {
        printf("Hash table maximum size exceeded.\n");
        exit(EXIT_FAILURE);
    }
    struct node **new_table;
    /* It checks if the hash table nodes could not be allocated for the new capacity */
    if (!(new_table = malloc(sizeof(struct node *) * new_cap))) {
        printf("Hash table resizing could not allocate %zu elements.\n", new_cap);
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < old_cap; i++) {
        struct node *curr = ht->table[i];
        while (curr) {
            ht->table[i] = curr->next;
            size_t new_h = hash(new_cap, curr->key);
            curr->next = new_table[new_h];
            new_table[new_h] = curr;
            curr = ht->table[i];
        }
    }
    free(ht->table);
    ht->table    = new_table;
    ht->capacity = new_cap;
    ht->lf       = (float)ht->size / ht->capacity;
}

struct hashtable *
hashtable_new(size_t initial_capacity) {
    struct hashtable *ht = malloc(sizeof(struct hashtable));
    /* It checks if there is not enough memory to allocate a hash table */
    if (!ht) {
        printf("A hash table could not be allocated.\n");
        exit(EXIT_FAILURE);
    }
    ht->size     = 0;
    ht->capacity = initial_capacity;
    ht->lf       = 0.0f;
    /* It checks if the hash table nodes could not be allocated */
    if (!(ht->table = malloc(sizeof(struct node *) * initial_capacity))) {
        printf("A hash table with initial capacity %zu could not be allocated.\n", initial_capacity);
        free(ht);
        exit(EXIT_FAILURE);
    }
    return ht;
}

void
hashtable_insert(struct hashtable *ht, const char *key, struct var_descriptor_t *value) {
    struct node *new_node = malloc(sizeof(struct node));
    /* It checks if there is not enough memory to allocate a hash table node */
    if (!new_node) {
        printf("A hashtable node could not be allocated.\n");
        exit(EXIT_FAILURE);
    }
    if (ht->lf >= 0.75f)
        resize(ht);
    size_t h = hash(ht->capacity, key);
    new_node->key              = key;
    memcpy(&new_node->descriptor, value, sizeof(struct var_descriptor_t));
    new_node->next             = ht->table[h];
    ht->table[h]               = new_node;
    ht->size++;
    ht->lf                     = (float)ht->size / ht->capacity;
}

struct var_descriptor_t *
hashtable_find(struct hashtable *ht, const char *key) {
    size_t h          = hash(ht->capacity, key);
    struct node *curr = ht->table[h];
    while (curr) {
        if (0 == strcmp(curr->key, key))
            return &curr->descriptor;
        curr = curr->next;
    }
    return NULL;
}

void
hashtable_remove(struct hashtable *ht, const char *key, struct var_descriptor_t *placeholder) {
    size_t h          = hash(ht->capacity, key);
    struct node *prev = NULL, *curr = ht->table[h];
    while  (curr) {
        if (0 == strcmp(curr->key, key)) {
            *placeholder = curr->descriptor;
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    if (curr) {
        if (prev)
            prev->next = curr->next;
        else ht->table[h] = curr->next;
        ht->size--;
        ht->lf = (float)ht->size / ht->capacity;
        free(curr);
    }
}
