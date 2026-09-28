#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_SIZE 5

//Circular buffer holding the last 5 lines from input 
//Each entry is a pointer to a string that comes from getline()
typedef struct {
    char *lines[HISTORY_SIZE];
    int next; // index where next line is stored
    int count; // how many lines are stored
} History;

static void history_init(History *h) {
    for (int i = 0; i < HISTORY_SIZE; i++) {
        h->lines[i] = NULL;
    }
    h->next = 0;
    h->count = 0;
}

//Free the oldest line and replace it with the new line if buffer is full
static void history_add(History *h, char *line) {
    free(h->lines[h->next]);
    h->lines[h->next] = line;
    h->next = (h->next + 1) % HISTORY_SIZE;
    if (h->count < HISTORY_SIZE) {
        h->count++;
    }
}

//Prints stored lines from oldest to newest 
static void history_print(const History *h) {
    int start = (h->next - h->count + HISTORY_SIZE) % HISTORY_SIZE;
    for (int i =0; i < h->count; i++) {
        printf("%s\n", h->lines[(start + i) % HISTORY_SIZE]);
    }
}

static void history_free(History *h) {
    for (int i = 0; i < HISTORY_SIZE; i++) {
        free(h->lines[i]);
        h->lines[i] = NULL;
    }
}

//Reads one line and strips the trailing newline 
static char *read_line(void) {
    char *line = NULL;
    size_t cap = 0;

    printf("Enter input: ");
    if (getline(&line, &cap, stdin) == -1) {
        free(line);
        return NULL;
    }
    line[strcspn(line, "\n")] = '\0';
    return line;
}

int main (void) {
    History history;
    history_init(&history);

    char *line;
    while ((line = read_line()) != NULL) {
        int is_print = (strcmp(line, "print") == 0);
        history_add(&history, line);
        if (is_print) {
            history_print(&history);
        }
    }

    history_free(&history);
    return 0;
}