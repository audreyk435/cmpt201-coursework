#define _DEFAULT_SOURCE 
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EXTRA_SIZE 256
#define BLOCK_SIZE 128
#define BUF_SIZE 256

struct header {
    uint64_t size;
    struct header *next;
};

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void print_out(char *format, void *data, size_t data_size) {
    char buf[BUF_SIZE];
    ssize_t len = snprintf(buf, BUF_SIZE, format, data_size == sizeof(uint64_t) 
                            ? *(uint64_t*)data : *(void **)data);
    if (len < 0) {
        handle_error("snprintf");
    }
    write(STDOUT_FILENO, buf, len);
}

int main () {
    void *start = sbrk(EXTRA_SIZE);
    if (start == (void *)-1) {
        handle_error("sbrk");
    }

    struct header *first = (struct header *)start;
    struct header *second = (struct header *)((char *)start + BLOCK_SIZE);
    
    first->size = BLOCK_SIZE;
    first->next = NULL;
    second->size = BLOCK_SIZE;
    second->next = first;

    size_t data_size = BLOCK_SIZE - sizeof(struct header);
    char *data1 = (char *)first + sizeof(struct header);
    char *data2 = (char *)second + sizeof(struct header);

    for (size_t i =0; i < data_size; i++) {
        data1[i]=0;
    }
    for (size_t i = 0; i < data_size; i++) {
        data2[i] = 1;
    }

    print_out ("first block:                %p\n", &first, sizeof(first));
    print_out ("second block:               %p\n", &second, sizeof(second));
    print_out ("first block size:           %lu\n", &first->size, sizeof(first->size));
    print_out ("first block next:           %p\n", &first->next, sizeof(first->next));
    print_out ("second block size:          %lu\n", &second->size, sizeof(second->size));
    print_out ("second block size:          %p\n", &second->next, sizeof(second->next));

    for (size_t i = 0; i < data_size; i++) {
        uint64_t value = data1[i];
        print_out("%lu\n", &value, sizeof(value));
    }
    for (size_t i = 0; i < data_size; i++) {
        uint64_t value = data2[i];
        print_out("%lu\n", &value, sizeof(value));
    }

    return 0;
}