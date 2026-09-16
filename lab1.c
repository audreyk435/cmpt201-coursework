#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char *buff = NULL;
  size_t size = 0;

  while (1) {
    printf("Please eneter some text: ");
    fflush(stdout);

    ssize_t num_char = getline(&buff, &size, stdin);
    if (num_char == -1) {
      break;
    }

    if (num_char > 0 && buff[num_char - 1] == '\n') {
      buff[num_char - 1] = '\0';
    }

    printf("Tokens:\n");

    char *saveptr;
    char *token = strtok_r(buff, " ", &saveptr);

    while (token != NULL) {
      printf("%s\n", token);
      token = strtok_r(NULL, " ", &saveptr);
    }
  }

  free(buff);
  return 0;
}
