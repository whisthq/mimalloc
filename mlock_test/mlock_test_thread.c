#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mimalloc.h>
#include <string.h>
#include <pthread.h>

// make sure that we don't double mlock anything extraneous (e.g. a page used by both parent and child)
// make sure that when the segment is abandoned, unused pages are munlock'ed (for medium allocations, the first two pages used)

void* common_small;
void* common_medium;
void* common_large;

void *thread(void* vargp) {
    // child
    printf("Child allocating...\n");
    int size = 20;
    void* smalls[size];
    void* mediums[size];
    void* larges[size];
    // small allocations
    for (int i = 0; i < size; i++) {
        smalls[i] = malloc(100);
        mediums[i] = malloc(1 << 16);
        larges[i] = malloc(1 << 20);
    }
    // The child should abandon 
    for (int i = 0; i < size - 1; i++) {
        free(smalls[i]);
        free(mediums[i]);
        free(larges[i]);
    }
    printf("Child finished freeing\n");
    common_small = smalls[size - 1];
    common_medium = mediums[size - 1];
    common_large = larges[size - 1];
    printf("small %p medium %p large %p\n", common_small, common_medium, common_large);
    printf("Child exiting...\n");
    pthread_exit(0);
}

int main() {
    mi_option_set(mi_option_reset_delay, 1000);

    printf("Parent allocate some stuff\n");
    for (int i = 0; i < 5; i++) {
        void* p1 = malloc(100);
    }
    pthread_t thread_id;
    int i;
    pthread_create(&thread_id, NULL, thread, (void*)i);
    // parent
    // sleep to wait for child
    printf("waiting for thread...\n");
    pthread_join(thread_id, NULL);
    printf("Parent allocating..\n");
    memset(common_small, 1, 10);
    memset(common_medium, 1, 10);
    memset(common_large, 1, 10);
    // now try some allocations
    // small allocations
    for (int i = 0; i < 5; i++) {
        void* p1 = malloc(100);
    }
    // medium allocations
    for (int i = 0; i < 5; i++) {
        void* p1 = malloc(1 << 16);
    }
    // large allocations
    for (int i = 0; i < 5; i++) {
        void* p1 = malloc(1 << 20);
    }
    printf("small %p medium %p large %p\n", common_small, common_medium, common_large);
    printf("Freeing commons\n");
    free(common_small);
    free(common_medium);
    free(common_medium);
}
