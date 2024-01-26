/**
 * @file hw1pr1.c
 * @author Qucheng Jiang
 * @author jiang.qu@northeastern.edu
 * @author NUID 001569593
 * @date 2024-01-23
 */

#include <stdio.h>

// Prototype
void print_args(int argc, char **argv);

int main(int argc, char **argv)
{
    print_args(argc, argv);
    return 0;
}

void print_args(int argc, char **argv)
{
    // Print arguments
    for (int i = 0; i < argc; i++) printf("argv[%d] = '%s'\n", i, argv[i]);
}
