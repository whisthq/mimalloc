#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mimalloc.h>
#include <string.h>
#include <pthread.h>

// Run this while turning on MLOCK_LOG, compare the printfs here with the regions of memory MLOCK'ed and MUNLOCK'ed.
void* common_small;
void* common_medium;
void* common_large;

void *thread(void* vargp) {
    // child
    printf("Child allocating...\n");
    int size = 5;
    void* smalls[size];
    void* mediums[size];
    void* larges[size];
    // small allocations
    for (int i = 0; i < size; i++) {
        smalls[i] = malloc(100);
        mediums[i] = malloc(1 << 16);
        larges[i] = malloc(1 << 20);
    }
    for (int i = 0; i < size; i++) {
        free(smalls[i]);
        free(mediums[i]);
        free(larges[i]);
    }
    common_small = malloc(10);
    common_medium = malloc(100 * 1000);
    common_large = malloc(1000 * 1000 * 2);
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
