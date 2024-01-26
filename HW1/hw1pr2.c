#include <stdio.h>
#include <string.h>

// fgets() manual
// https://man7.org/linux/man-pages/man3/fgets.3p.html

int main() 
{
    char ipt_buff[16384];

    while (1) 
    {
        // Display prompt
        printf("$ "); fflush(stdout);

        // Read input from user
        if (fgets(ipt_buff, 16384, stdin) != NULL) 
        {
            // Replace newline character with null character
            ipt_buff[strlen(ipt_buff) - 1] = '\0';
            // Display the input back to the user
            printf("%s\n", ipt_buff);
        } 
        else
            // Error or EOF
            break;
    }

    return 0;
}
