#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mimalloc.h>
#include <string.h>

// Run this while turning on MLOCK_LOG, compare the printfs here with the regions of memory MLOCK'ed and MUNLOCK'ed.

int main() {
    mi_option_set(mi_option_reset_delay, 1000);

    printf("Large allocations\n");
    void* p1 = malloc(1 << 20); // 1mb I think
    printf("p1 %p\n", p1);
    void* p2 = malloc(1 << 19); // 0.5mb -> 1.5mb
    printf("p2 %p\n", p2);
    void* p3 = realloc(p1, 1 << 21); // realloc 2 mb -> 2.5 mb
    printf("p3 %p (realloc p1)\n", p3);
    printf("Huge page allocation\n");
    void* p4 = malloc(3 << 20); // 3mb -> 5.5 mb
    printf("p4 %p\n", p4);
    free(p4); // this should cause an immediate unreset because it was the only thing on the segment
    printf("medium allocations\n");
    void* p7 = malloc(2 << 16); // 128 kb
    printf("p7 %p\n", p7);
    void* p8 = malloc(3 << 16); // 192 kb
    printf("p8 %p\n", p8);
    // alternate small and big-ish allocations
    printf("alternate between small and medium allocations\n");
    for (int i = 0; i < 10; i++) {
        void* small1 = malloc(0x60); // 96b
        printf("small1 %p\n", small1);
        free(small1);
        void* p5 = malloc(1 << 12); // 4kb
        printf("p5 %p\n", p5);
    }
}
