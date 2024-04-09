/**
 * @file hw6pr3.c
 * @author Qucheng Jiang
 * @author jiang.qu@northeastern.edu
 * @author NUID 001569593
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


int printFileInfo(char *filePath) {
    struct stat fileInfo;
    // Get file info
    if (stat(filePath, &fileInfo) == -1) { perror("Error: No such file or directory"); return -1; }
    // Print Inode & Size
    printf("Inode: %ld\n", fileInfo.st_ino);
    printf("Size: %ld\n", fileInfo.st_size);
    // Print Permissions
    printf("Permissions: ");
    printf((fileInfo.st_mode & S_IRUSR) ? "r" : "-");printf((fileInfo.st_mode & S_IWUSR) ? "w" : "-");printf((fileInfo.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileInfo.st_mode & S_IRGRP) ? "r" : "-");printf((fileInfo.st_mode & S_IWGRP) ? "w" : "-");printf((fileInfo.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileInfo.st_mode & S_IROTH) ? "r" : "-");printf((fileInfo.st_mode & S_IWOTH) ? "w" : "-");printf((fileInfo.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");
    return 0;
}

int createLink(char *src, char *dest, int symlinkFlag) {
    // Swith here between soft(0) and hard(1)
    int result = symlinkFlag ? symlink(src, dest) : link(src, dest);
    if (result == -1) { perror("Error: creating link failed"); return -1; }
    return 0;
}

int removeFile(char *filePath) {
    struct stat fileInfo;
    if (stat(filePath, &fileInfo) == -1) { perror("Error: Cannot getting file info"); return -1; }
    if (S_ISREG(fileInfo.st_mode)) { if (unlink(filePath) == -1) { perror("Error: Cannot unlink file"); return -1; }} else { fprintf(stderr, "Error: Not a regular file\n"); return -1; }
    return 0;
}

int main(int argc, char *argv[]) {
    // first stage filter
    if (argc < 3) { fprintf(stderr, "Usage: ./files action [args]\n"); return 1; }

    // check with each action
    if (strcmp(argv[1], "info") == 0 && argc == 3) {printFileInfo(argv[2]);} 
    else if (strcmp(argv[1], "link") == 0 && argc == 4) {createLink(argv[2], argv[3], 0);} 
    else if (strcmp(argv[1], "symlink") == 0 && argc == 4) {createLink(argv[2], argv[3], 1);} 
    else if (strcmp(argv[1], "rm") == 0 && argc == 3) {removeFile(argv[2]);} 
    else {
        // not fits to any action
        fprintf(stderr, "Invalid action or arguments.\n");
        fprintf(stderr, "Usage: ./files action [args]\n");
        fprintf(stderr, "Action: [info(1),link(2),symlink(2),rm(1)]\n");
        return 1;
    }

    return 0;
}
