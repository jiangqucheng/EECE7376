/**
 * @file hw1pr3a.c
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
        int num;
        printf("Enter a number: ");
        scanf("%d", &num);
        exit(num);  // Child exits with the entered number as exit status
    }
    else if (pid > 0)
    {
        // Parent
        int status;
        wait(&status);  // Wait for child to finish
        printf("Child exited with status %d\n", WEXITSTATUS(status));
    }
    else
    {
        // Fork failed
        fprintf(stderr, "Fork failed\n");
        exit(1);  // Exit with error code 1;
    }

    return 0;
}
