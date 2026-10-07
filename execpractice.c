#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  pid_t pid = fork();

  if (pid < 0) {
    perror("fork");
    exit(1);
  }

  if (pid == 0) {
    execl("/bin/ls", "ls", "-a", "-l", "-h", (char *)NULL);
    perror("execl");
    exit(1);
  } else {
    execlp ("ls, "ls", (char*) NULL);
    perror ("execlp");
    exit (1);
  }

  printf("%d\n", getpid());
  return 0;
}
