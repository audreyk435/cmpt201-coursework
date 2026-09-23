#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main (void) {
    char* line = NULL;
    size_t len = 0;

    while (1) {
        //Read input and print it back 
        printf ("Enter programs to run.\n>");
        fflush(stdout);

        ssize_t nread = getline (&line, &len, stdin);
        if (nread == -1) {
            break;
        }

        if (nread > 0 && line[nread-1] == '\n') {
            line[nread - 1] = '\0';
        }

        if (line [0] == '\0') {
            continue;
        }

        pid_t pid = fork ();
        if (pid == -1) {
            perror("fork");
            continue;
        }
        if (pid == 0) {
            execlp(line, line, (char *) NULL);

            printf("Exec failure\n");
            _exit(1);
        } else {
            if (waitpid(pid, NULL, 0) == -1) {
                perror ("waitpid");
                break;
            }
        }
    }
    free (line);
    return 0;
}