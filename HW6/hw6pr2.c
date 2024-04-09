/**
 * @file hw6pr2.c
 * @author Qucheng Jiang
 * @author jiang.qu@northeastern.edu
 * @author NUID 001569593
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd;
    pid_t pid;

    // Open a file (or create if it doesn't exist) with read/write permissions
    fd = open("shared_file.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Fork a child process
    pid = fork();
    if (pid == -1) { perror("Fork failed"); close(fd); return 1; }  // fork get err

    if (pid == 0) { 
        // Child process
        for (int i = 0; i < 5; ++i) { write(fd, "Child\n", 6); sleep(1); }
    } else { 
        // Parent process
        for (int i = 0; i < 5; ++i) { write(fd, "Parent\n", 7); sleep(1); }
    }

    close(fd);
    return 0;
}
