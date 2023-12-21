#include "hashtable.h"
#include "set.h"
#include <stdlib.h>
#include <stdio.h>

// Hash function to determine the bin for a given key
static unsigned long hash_function(const char *str) {
    // Implementation of the hash function remains unchanged

    // This is a simple example; you might want to replace it with a more robust hash function
    // Ensure that the hash value is returned
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != '\0') {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}

hashtable_t *hashtable_new(const int num_slots) {
    hashtable_t *new_table = (hashtable_t *)malloc(sizeof(hashtable_t));
    if (new_table == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    new_table->num_bins = num_slots;
    new_table->bins = (set_t **)malloc(num_slots * sizeof(set_t *));
    if (new_table->bins == NULL) {
        free(new_table);
        return NULL;
    }

    for (int i = 0; i < num_slots; i++) {
        new_table->bins[i] = set_new();
        if (new_table->bins[i] == NULL) {
            for (int j = 0; j < i; j++) {
                set_delete(new_table->bins[j], NULL);
            }
            free(new_table->bins);
            free(new_table);
            return NULL;
        }
    }

    return new_table;
}

bool hashtable_insert(hashtable_t *ht, const char *key, void *item) {
    if (ht == NULL || key == NULL || item == NULL) {
        return false;
    }

    int bin_index = hash_function(key) % ht->num_bins;
    return set_insert(ht->bins[bin_index], key, item);
}

void *hashtable_find(hashtable_t *ht, const char *key) {
    if (ht == NULL || key == NULL) {
        return NULL;
    }

    int bin_index = hash_function(key) % ht->num_bins;
    return set_find(ht->bins[bin_index], key);
}

void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (fp == NULL) {
        return;
    }

    for (int i = 0; i < ht->num_bins; i++) {
        set_print(ht->bins[i], fp, itemprint);
    }
}

void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (ht == NULL || itemfunc == NULL) {
        return;
    }

    for (int i = 0; i < ht->num_bins; i++) {
        set_iterate(ht->bins[i], arg, itemfunc);
    }
}

void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item)) {
    if (ht == NULL) {
        return;
    }

    for (int i = 0; i < ht->num_bins; i++) {
        set_delete(ht->bins[i], itemdelete);
    }

    free(ht->bins);
    free(ht);
}

