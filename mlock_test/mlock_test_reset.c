#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mimalloc.h>
#include <string.h>

int main() {
    mi_option_set(mi_option_reset_delay, 1000);

    printf("Testing munlock with reset_delay\n");
    printf("Confirm that pages added to the reset queue with RESET_ADD that are not removed using RESET_REMOVE after each sleep are correctly freed\n");
    void* p1 = malloc(16*1024);
    void* p2 = malloc(16*1024);
    void* p3 = malloc(16*1024);
    void* p4 = malloc(16*1024);
    free(p1);
    printf("freed p1\n");
    free(p4);
    printf("freed p4\n");
    void* p_array[100];
    for (int i = 0; i < 12; i++) p_array[i] = malloc(10 * 1024);
    for (int i = 5; i < 12; i++) free(p_array[i]);
    printf("freed 7\n");
    printf("sleeping\n");
    sleep(2);
    void *p5 = malloc(50);
    free(p5);
    printf("sleeping\n");
    sleep(2);
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
