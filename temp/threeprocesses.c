#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LEN 100

int main() {
    int fd[2][2];
    pipe(fd[0]); // this is for writing from parent to child1
    pipe(fd[1]); // this is for writing from child1 to child2

    int child1 = fork();
    if (child1 == 0) {
        close(fd[0][1]); // write pipe (used in parent)
        close(fd[1][0]); // read pipe  (used in child2)
        close(NULL);

        int x;
        read(fd[0][0], &x, sizeof(int)); // read from parent
        printf("Received int (child 1): %i\n", x);

        x *= 2;

        write(fd[1][1], &x, sizeof(int));

        close(fd[0][0]);
        close(fd[1][1]);

        return 0;
    }

    int child2 = fork();
    if (child2 == 0) {
        close(fd[0][0]); // read pipe  (used in child1)
        close(fd[1][1]); // write pipe (used in child1)
        close(fd[0][1]); // write pipe (used in parent)

        int x;
        read(fd[1][0], &x, sizeof(int)); // read from parent
        printf("Received int (child 2): %i\n", x);

        close(fd[1][0]);
        return 0;
    }

    // Parent
    close(fd[0][0]); // read pipe  (used in child1)
    close(fd[1][0]); // read pipe  (used in child2)
    close(fd[1][1]); // write pipe (used in child1)

    int x = 2;
    write(fd[0][1], &x, sizeof(int));

    close(fd[0][1]); // write pipe (used in parent)

    int stat_loc;
    int stat_loc2;
    waitpid(child1, &stat_loc, 0);
    waitpid(child2, &stat_loc2, 0);

    printf("Parent registers children %d and %d exit statuses: %i and %i\n", child1, child2, WEXITSTATUS(stat_loc), WEXITSTATUS(stat_loc2));
}
