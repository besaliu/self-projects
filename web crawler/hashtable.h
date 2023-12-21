#ifndef HASH_H_
#define HASH_H_

#include <stdbool.h>
#include <stdio.h>  // Include this line for FILE

// Declare set_t if it's not declared in this file
typedef struct set set_t;

// Declare hashtable_t
typedef struct hashtable {
    int num_bins;
    set_t **bins;
} hashtable_t;

hashtable_t *hashtable_new(const int num_slots);
bool hashtable_insert(hashtable_t *ht, const char *key, void *item);
void *hashtable_find(hashtable_t *ht, const char *key);
void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item));
void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item));
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item));

#endif  // HASH_H_

