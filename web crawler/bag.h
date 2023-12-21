#ifndef BAG_H
#define BAG_H

#include <stdbool.h>

// Forward declaration of webpage_t
typedef struct webpage webpage_t;

typedef struct bag bag_t;

/**
 * Create a new bag.
 */
bag_t *bag_new(void);

/**
 * Insert a webpage into the bag.
 */
void bag_insert(bag_t *bag, webpage_t *page);

/**
 * Check if the bag is empty.
 */
bool bag_is_empty(bag_t *bag);

/**
 * Extract a webpage from the bag.
 */
webpage_t *bag_extract(bag_t *bag);

/**
 * Delete a bag.
 */
void bag_delete(bag_t *bag, void (*itemdelete)(void *item));

#endif

