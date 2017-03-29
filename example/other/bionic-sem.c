#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define  NUM_SEMAPHORES  32
#define  MAX_FAILS       10

int  main(void)
{
    int   counter = 0;
    int   fails   = 0;

    if (counter == IPC_PRIVATE)
        counter++;

    printf( "%d (NUM_SEMAPHORES=%d)\n", counter, NUM_SEMAPHORES);

    for (;;) {
        int  ret = fork();
        int  status;

        if (ret < 0) {
            perror("fork:");
            break;
        }
        if (ret == 0) {
            /* in the child */
            ret = semget( (key_t)counter, NUM_SEMAPHORES, IPC_CREAT );
            if (ret < 0) {
                return errno;
            }
            return 0;
        }
        else {
            /* in the parent */
            ret = wait(&status);
            if (ret < 0) {
                perror("waitpid:");
                break;
            }
            if (status != 0) {
                status = WEXITSTATUS(status);
                fprintf(stderr, "child %d FAIL at counter=%d: %d\n", ret,
                                counter, status);
                if (++fails >= MAX_FAILS)
                    break;
            }
        }

        counter++;
        if ((counter % 1000) == 0) {
            printf("%d\n", counter);
        }
        if (counter == IPC_PRIVATE)
            counter++;
    }
    return 0;
}
