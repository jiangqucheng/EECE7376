/**
 * @file hw1pr4.c
 * @author Qucheng Jiang
 * @author jiang.qu@northeastern.edu
 * @author NUID 001569593
 * @date 2024-01-23
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototype
int get_args(char *in, char **argv, int max_args);
void print_args(int argc, char **argv);
void free_args(int argc, char **argv);

int main()
{
    char s[200];  
    char *argv[10]; 
    int argc; 

    // Read a string from the user 
    printf("Enter a string: ");  
    fgets(s, sizeof s, stdin); 

    // Extract arguments and print them 
    argc = get_args(s, argv, 10);  
    print_args(argc, argv); 

    // Clean up memory and exit
    free_args(argc, argv); 
    return 0;
}

int get_args(char *in, char **argv, int max_args)
{
    int argc = 0;
    char * pSepStr = 0x00;
    in[strcspn(in, "\n")] = 0x00; // Remove the newline character at the end
    pSepStr = strtok(in, " ");

    while (pSepStr != NULL && argc < max_args)
    {
        // Duplicate the string
        argv[argc] = strdup(pSepStr);
        // Get next argument
        pSepStr = strtok(NULL, " ");
        // Next argument idx
        argc++;
    }
    return argc;
}

void print_args(int argc, char **argv)
{
    // Print arguments
    for (int i = 0; i < argc; i++) printf("argv[%d] = '%s'\n", i, argv[i]);
}

void free_args(int argc, char **argv)
{
    // Free allocated memory
    for (int i = 0; i < argc; i++) free(argv[i]);
}