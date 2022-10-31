#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mimalloc.h>
#include <string.h>

int main() {
    mi_option_set(mi_option_reset_delay, 1000);
    mi_option_set(mi_option_abandoned_page_reset, 1);

    printf("Large allocations: alloc all, free all\n");
    int size = 5000;
    int* p[size];
    for (int i = 0; i < size; i++) {
        p[i] = malloc(1 << 20);
        memset(p[i], 1, 1<<20);
        printf("p1 %p\n", p[i]);
    }
    sleep(2);
    for (int i = 0; i < size; i++) {
        printf("free %p\n", p[i]);
        free(p[i]);
    }
    sleep(2);
    printf("Large allocations: alternate alloc and free\n");
    for (int i = 0; i < size; i++) {
        p[i] = malloc(1 << 20);
        memset(p[i], 1, 1<<20);
        printf("p1 %p\n", p[i]);
        printf("free %p\n", p[i]);
        free(p[i]);
    }
    sleep(2);
}
