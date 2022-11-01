#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mimalloc.h>
#include <string.h>

int main() {
    mi_option_set(mi_option_reset_delay, 1000);

    printf("Testing munlock with reset_delay\n");
    // make 20 64kb allocations
    // free 8 at a time with 1s delays inbetween
    // should see the pages resetting one by one
    void* allocs[24];
    for (int i = 0; i < 24; i++) {
        void* p6 = malloc(1 << 16); // 64kb
        allocs[i] = p6;
        printf("p6 %p\n", p6);
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 8; j++) {
            printf("free %p\n", allocs[8 * i + j]);
            free(allocs[8 * i + j]);
        }
        printf("sleeping\n");
        sleep(1);
    }
}
