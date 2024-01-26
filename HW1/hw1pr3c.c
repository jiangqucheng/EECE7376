/**
 * @file hw1pr3c.c
 * @author Qucheng Jiang
 * @author jiang.qu@northeastern.edu
 * @author NUID 001569593
 * @date 2024-01-23
 */

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
        printf("[Child]: exiting\n");
        exit(0);  // Child exits immediately
    }
    else if (pid > 0)
    {
        // Parent process creates a zombie child
        while (1)
        {
            printf("[Parent]: running\n");
            sleep(2);  // Sleep for 2 seconds
        }
    }
    else
    {
        // Fork failed
        fprintf(stderr, "Fork failed\n");
        exit(1);  // Exit with error code 1;
    }

    return 0;
}
