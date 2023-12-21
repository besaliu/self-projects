#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "crawler.h"
#include "pagedir.h"
#include "url.h"
#include "curl.h"
#include <errno.h>


// Entry point
int main(const int argc, char *argv[]) {
    char *seedURL;
    char *pageDirectory;
    int maxDepth;

    // Parse command-line arguments
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);

    // Perform crawling
    crawl(seedURL, pageDirectory, maxDepth);

    return 0;
}

// Duplicate a string and handle errors
char *mystrdup(const char *str) {
    size_t len = strlen(str) + 1;
    char *dup = malloc(len);
    if (dup == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    return memcpy(dup, str, len);
}


// Free memory occupied by a webpage
void webpage_delete(void *item) {
    if (item == NULL) {
        return;
    }

    webpage_t *page = (webpage_t *)item;
    free(page->url);
    free(page->html);
    free(page);
}

// Parse command-line arguments
void parseArgs(int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s seedURL pageDirectory maxDepth\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    *seedURL = argv[1];
    *pageDirectory = argv[2];
    *maxDepth = atoi(argv[3]);
}



void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen) {
    const char *searchStr = "href=\"";
    const char *html = page->html;
    char *pos = (char *)html;

    while ((pos = strstr(pos, searchStr)) != NULL) {
        pos += strlen(searchStr);
        char *start = pos;
        char *end = strchr(start, '"');

        if (end == NULL) {
            fprintf(stderr, "Error: Malformed URL in pageScan\n");
            break;
        }

        int urlLen = end - start;
        char *url = strndup(start, urlLen);

        if (isInternalURL(url, page->url)) {
            if (hashtable_insert(pagesSeen, url, "")) {
                if (!hashtable_find(pagesSeen, url)) {
                    // Create a new webpage for the found URL and add it to the bag
                    webpage_t *newPage = (webpage_t *)malloc(sizeof(webpage_t));
                    newPage->url = strdup(url);
                    newPage->depth = page->depth;
                    newPage->html = download(url, &(newPage->length));  // Use download to fetch HTML content
                    bag_insert(pagesToCrawl, newPage);
                } else {
                    printf("%d    IgnDupl: %s\n", page->depth, url);

                    // Free the URL only if it's not a duplicate
                    free(url);
                    return;  // Exit the function after encountering a duplicate
                }
            } else {
                printf("Failed to insert into hashtable: %s\n", url);
            }
        } else {
            printf("%d   IgnExtrn: %s\n", page->depth, url);
            free(url);  // Free the URL if it's external
        }

        pos = end; // Move to the end of the found URL
    }
}

void crawl(const char *seedURL, const char *pageDirectory, const int maxDepth) {
    hashtable_t *pagesSeen = hashtable_new(200);
    bag_t *pagesToCrawl = bag_new();

    webpage_t *seedPage = (webpage_t *)malloc(sizeof(webpage_t));
    seedPage->url = strdup(seedURL);
    seedPage->depth = 0;
    seedPage->html = download(seedURL, &(seedPage->length));
    webpage_t *current = (webpage_t *)malloc(sizeof(webpage_t));

    // Check if download failed for the seed page
    if (seedPage->html == NULL) {
        fprintf(stderr, "Error: HTML content is NULL for seed URL %s\n", seedPage->url);
        free(seedPage->url);
        free(seedPage);
        hashtable_delete(pagesSeen, NULL);
        bag_delete(pagesToCrawl, webpage_delete);
        return;
    }

    // Add seedURL to hashtable and bag
    hashtable_insert(pagesSeen, seedURL, "");
    bag_insert(pagesToCrawl, seedPage);

    int documentID = 1;

    while (!bag_is_empty(pagesToCrawl)) {
        webpage_t *current = bag_extract(pagesToCrawl);
        sleep(1); // Pause for one second between page fetches

        printf("%d  Fetching URL: %s\n", current->depth, current->url);

        char *html = download(current->url, &(current->length));

        if (html != NULL) {
            printf("%d  Fetched: %s\n", current->depth, current->url);

            // Save HTML content to file
            pagedir_save(current, pageDirectory, documentID++);

            if (current->depth < maxDepth) {
                // Create a new webpage for scanning and add it to the bag
                webpage_t *page = (webpage_t *)malloc(sizeof(webpage_t));
                page->url = strdup(current->url);
                page->depth = current->depth + 1;
                page->html = strdup(html);
                page->length = strlen(html);

                printf("%d  Scanning: %s\n", page->depth, page->url);
                pageScan(page, pagesToCrawl, pagesSeen);

                // Free the new webpage structure
                free(page->url);  // Free URL before freeing the structure
                free(page);
            }

            // Free the HTML content after it has been used
            free(html);
        } else {
            fprintf(stderr, "%d  Error: HTML content is NULL for %s\n", current->depth, current->url);
            printf("HTML is NULL for %s\n", current->url);
        }

        // Free the current webpage after it has been used
        free(current->url);  // Free URL before freeing the structure
        free(current);
    }

    // Free the seedPage structure
    free(seedPage->url);
    free(seedPage);

    // Free the hashtable and bag
    hashtable_delete(pagesSeen, NULL);
    bag_delete(pagesToCrawl, webpage_delete);
}


