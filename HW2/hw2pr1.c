/**
 * @file hw2pr1.c
 * @author Qucheng Jiang
 * @author jiang.qu@northeastern.edu
 * @author NUID 001569593
 * @date 2024-02-01
 */

#include <stdio.h>  // printf(), fflush()
#include <stdlib.h>  // exit(), EXIT_FAILURE
#include <unistd.h>  // fork()
#include <sys/wait.h>  // wait()

// Prototype

// Main
int main()
{
    int pipefd_p2c[2], pipefd_c2p[2]; // Parent to child and child to parent pipes
    char buf; // Buffer for reading from pipe

    // Creating two pipes
    if (pipe(pipefd_p2c) == -1 || pipe(pipefd_c2p) == -1) 
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    int pid = fork();
    int i;
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child
        close(pipefd_p2c[1]); // Close unused write end
        close(pipefd_c2p[0]); // Close unused read end

        for (i = 0; i < 5; i++)
        {
            // Wait for parent to write
            read(pipefd_p2c[0], &buf, 1);

            printf("%d. Child\n", i + 1);
            fflush(stdout);
            
            // Signal parent
            write(pipefd_c2p[1], "!", 1);
        }

        close(pipefd_p2c[0]); // Close read end
        close(pipefd_c2p[1]); // Close write end
    }
    else
    {
        // Parent
        close(pipefd_p2c[0]); // Close unused read end
        close(pipefd_c2p[1]); // Close unused write end

        for (i = 0; i < 5; i++)
        {
            printf("%d. Parent\n", i + 1);
            fflush(stdout);
            
            // Signal child
            write(pipefd_p2c[1], "!", 1);

            // Let child process run once
            
            // Wait for child to write
            read(pipefd_c2p[0], &buf, 1);
        }
        
        close(pipefd_p2c[1]); // Close write end
        close(pipefd_c2p[0]); // Close read end

        wait(NULL);
    }
    return 0;
}

