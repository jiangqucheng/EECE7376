#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() 
{
    pid_t pid = fork();

    if (pid == 0)
    {
        // Child
        while (1)
        {
            printf("[Child]: alive\n");
            sleep(2);  // Sleep for 2 seconds
        }
    }
    else if (pid > 0)
    {
        // Parent
        printf("[Parent]: exiting\n");
        exit(0);
    }
    else
    {
        // Fork failed
        fprintf(stderr, "Fork failed\n");
        exit(1);  // Exit with error code 1;
    }

    return 0;
}
