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
    for (int i = 0; i < argc; i++) printf("argv[%d] = '%s'\n", i, argv[i]);
}
