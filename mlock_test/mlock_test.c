#include <stdlib.h>
#include <stdio.h>

// Run this while turning on MLOCK_LOG, compare the printfs here with the regions of memory MLOCK'ed and MUNLOCK'ed.

int main() {
    void* p1 = malloc(1 << 20); // 1mb I think
    printf("p1 %p\n", p1);
    void* p2 = malloc(1 << 19); // 0.5mb -> 1.5mb
    printf("p2 %p\n", p2);
    void* p3 = realloc(p1, 1 << 21); // realloc 2 mb -> 2.5 mb
    printf("p3 %p (realloc p1)\n", p3);
    void* p4 = malloc(3 << 20); // 3mb -> 5.5 mb
    printf("p4 %p\n", p4);
    void* p6 = malloc(1 << 16); // 64kb
    printf("p6 %p\n", p6);
    void* p7 = malloc(2 << 16); // 128 kb
    printf("p7 %p\n", p7);
    void* p8 = malloc(3 << 16); // 192 kb
    printf("p8 %p\n", p8);
    // alternate small and big-ish allocations
    for (int i = 0; i < 10; i++) {
        void* small1 = malloc(50); // 50b
        printf("small1 %p\n", small1);
        free(small1);
        void* p5 = malloc(1 << 12); // 4kb
        printf("p5 %p\n", p5);
    }
    void* small2 = calloc(4, 24); // 96b
    void* medium1 = malloc(4000); // about 4kb
    printf("small2 %p, medium1 %p\n", small2, medium1);
    free(p4);

    free(p2);
    free(p3);
    free(small2);
    free(medium1);
}
