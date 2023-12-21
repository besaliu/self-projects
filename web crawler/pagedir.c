#include "pagedir.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

bool pagedir_init(const char *pageDirectory) {
    // Create the directory if it doesn't exist
    int status = mkdir(pageDirectory, 0777);
    if (status != 0) {
        fprintf(stderr, "Error creating the directory: %s\n", pageDirectory);
        return false;
    }
    return true;
}

void pagedir_save(const webpage_t *page, const char *pageDirectory, const int documentID) {
    char filename[50];
    snprintf(filename, sizeof(filename), "%s/%d", pageDirectory, documentID);  // Use a simple number as the filename

    FILE *file = fopen(filename, "wb");  // Open the file in binary write mode
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    }

    // Write the URL and depth as text, followed by the HTML content as binary data
    fprintf(file, "%s\n%d\n", page->url, page->depth);
    fwrite(page->html, 1, page->length, file);

    fclose(file);

    // Free the HTML content after writing to the file
    free(page->html);
}

