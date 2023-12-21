#include "crawler.h"
#include "bag.h"
#include <stdlib.h>
#include <stdbool.h>
struct bag {
    webpage_t **pages;
    int capacity;
    int size;
};

bag_t *bag_new(void) {
    bag_t *bag = (bag_t *)malloc(sizeof(bag_t));
    if (bag == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    bag->pages = NULL;
    bag->capacity = 0;
    bag->size = 0;

    return bag;
}

void bag_insert(bag_t *bag, webpage_t *page) {
    if (bag == NULL || page == NULL) {
        return;
    }

    if (bag->size >= bag->capacity) {
        bag->capacity = (bag->capacity == 0) ? 1 : bag->capacity * 2;
        bag->pages = realloc(bag->pages, bag->capacity * sizeof(webpage_t *));
        if (bag->pages == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
    }

    bag->pages[bag->size++] = page;
}

bool bag_is_empty(bag_t *bag) {
    return bag == NULL || bag->size == 0;
}

webpage_t *bag_extract(bag_t *bag) {
    if (bag_is_empty(bag)) {
        return NULL;
    }

    webpage_t *page = bag->pages[--bag->size];
    bag->pages[bag->size] = NULL;  // Remove reference from bag
    return page;
}

void bag_delete(bag_t *bag, void (*itemdelete)(void *item)) {
    if (bag == NULL) {
        return;
    }

    if (itemdelete != NULL) {
        for (int i = 0; i < bag->size; i++) {
            itemdelete(bag->pages[i]);
        }
    }

    free(bag->pages);
    free(bag);
}

