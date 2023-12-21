// crawler.h

#ifndef CRAWLER_H
#define CRAWLER_H

#include "bag.h"
#include "hashtable.h"


// Define the webpage_t type
typedef struct webpage {
    char *url;
    char *html;
    size_t length;
    int depth;
} webpage_t;

// Function prototypes for webpage_t
webpage_t *webpage_new(const char *url, const char *html, size_t length, int depth);

void webpage_delete(void *item);

// Function prototypes for crawler
int main(const int argc, char *argv[]);
void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth);
// Add this prototype
void crawl(const char *seedURL, const char *pageDirectory, const int maxDepth);

void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen);

#endif
