/**
 * @file hw2pr3.c
 * @author Qucheng Jiang
 * @author jiang.qu@northeastern.edu
 * @author NUID 001569593
 * @date 2024-02-01
 */

#include <stdio.h>  // printf(), fflush(), fgets()
#include <stdlib.h>
#include <string.h>

// #define DEBUG

#ifdef DEBUG
#undef DEBUG
#define DEBUG(x) x
#define RELEASE(x) 
#else
#define DEBUG(x) 
#define RELEASE(x) x
#endif

#define MAX_INPUT_STR_LEN  1024
#define MAX_SUB_COMMANDS   5
#define MAX_ARGS           10
struct SubCommand
{
	char *line;
	char *argv[MAX_ARGS];
};                                                                        
struct Command
{
	struct SubCommand sub_commands[MAX_SUB_COMMANDS];
	int num_sub_commands;
    char *stdin_redirect;
    char *stdout_redirect;
    int background;
};
typedef struct Command SCommand;
typedef struct SubCommand SSubCommand;


///////////////////////////////////////////////////////////////////// Prototype
/**
 * @brief Read command
 * @details 
 * This function takes an entire command line (in the `char *line` argument), 
 * and populates the `Command` data structure, passed by reference in the 
 * second argument (`struct Command *command`). The function body has two 
 * parts: First, the line is split into sub-strings with strtok using the 
 * `|` delimiter, and each substring is duplicated and stored into the 
 * sub-command's line field. Second, all subcommands are processed, and their 
 * `argv` fields are populated by calling `ReadArgs`. 
 * @param line - The entire command line
 * @param command - The `Command` data structure
 */
void ReadCommand(char *line, struct Command *command);
/**
 * @brief Read redirects and background
 * This function populates fields `stdin_redirect`, `stdout_redirect`, and 
 * `background` for the `command` passed by reference in the function argument. 
 * The function assumes that all other fields of the command structure have 
 * already been populated, as a result to a previous invocation to `ReadCommand`. 
 * The function should internally scan the arguments from the last sub-command 
 * in reverse order, extracting trailing `&`, `> file`, or `< file` patterns 
 * in a loop. 
 * @param command - The `Command` data structure
 */
void ReadRedirectsAndBackground(struct Command *command);
/**
 * @brief Print command
 * @details
 * This function prints all arguments for each sub-command of the command 
 * passed by reference. The function invokes `PrintArgs` internally. 
 * @param command - The `Command` data structure
 */
void PrintCommand(struct Command *command);
/**
 * @brief Free command
 * @details
 * This function free all memory allocated by the `Command` data structure.
 * @param command - The `Command` data structure
 */
void FreeCommand(struct Command *command);
/**
 * @brief Read arguments
 * @details
 * Argument `size` represents the number of elements allocated for `argv` by 
 * the function caller. The function should guarantee that the array is 
 * null-terminated under any circumstances, even if the number of tokens in 
 * string `in` exceeds `size`. Notice that this function does not return the 
 * number of arguments extracted from `in` anymore. 
 * @param in - The entire command line
 * @param argv - The array of arguments
 * @param size  - The number of elements allocated for `argv`
 */
void ReadArgs(char *in, char **argv, int size);
/**
 * @brief Print arguments
 * @details
 * This function does not need the number of arguments to be passed to the 
 * function anymore. Instead, the function will stop printing arguments as 
 * soon as the `NULL` element is found. 
 * @param argv - The array of arguments
 */
void PrintArgs(char **argv);

////////////////////////////////////////////////////////////////////////// Main
int main()
{
    char s[MAX_INPUT_STR_LEN];
    SCommand cmd = {.num_sub_commands = 0};

    // Read a string from the user
    printf("Enter a string: ");
    fgets(s, sizeof s, stdin);

    // Extract arguments and print them
    ReadCommand(s, &cmd);
    ReadRedirectsAndBackground(&cmd);
    PrintCommand(&cmd);

    // Clean up memory and exit
    FreeCommand(&cmd);
    return 0;
}

void ReadCommand(char *line, struct Command *command)
{
    // Split the line into sub-commands
    int subcnt = 0;
    char * pSepStr = 0x00;
    line[strcspn(line, "\n")] = 0x00; // Remove the newline character at the end
    pSepStr = strtok(line, "|");

    while (pSepStr != NULL && subcnt < MAX_SUB_COMMANDS)
    {
        // Get object ptr
        SSubCommand* p_subcmd = &command->sub_commands[subcnt];
        for (size_t i = 0; i < MAX_ARGS; i++) p_subcmd->argv[i] = NULL;
        // Duplicate the string
        p_subcmd->line = strdup(pSepStr);
        // Get next sub-command
        pSepStr = strtok(NULL, "|");
        // Next sub-command idx
        subcnt++;
    }
    command->num_sub_commands = subcnt;
    
    // Read arguments
    for (size_t i = 0; i < subcnt; i++) 
    {
        SSubCommand* p_subcmd = &command->sub_commands[i];
        ReadArgs(p_subcmd->line, p_subcmd->argv, MAX_ARGS);
    }
}


// int get_args(char *in, char **argv, int max_args)
void ReadArgs(char *in, char **argv, int size)
{
    int argc = 0;
    char * pSepStr = 0x00;
    in[strcspn(in, "\n")] = 0x00;  // Remove the newline character at the end
    char* t_dup_in = strdup(in);  // Duplicate the input string, for modify
    pSepStr = strtok(t_dup_in, " ");

    for (size_t i = 0; i < size; i++) argv[i] = NULL;

    while (pSepStr != NULL && argc < size-1)
    {
        // Duplicate the string
        argv[argc] = strdup(pSepStr);
        // Get next argument
        pSepStr = strtok(NULL, " ");
        // Next argument idx
        argc++;
    }

    free(t_dup_in);  // Free the duplicated string
    // return argc;
}

void ReadRedirectsAndBackground(struct Command *command) {
    command->stdin_redirect = NULL;
    command->stdout_redirect = NULL;
    command->background = 0;

    struct SubCommand *lastCmd = &command->sub_commands[command->num_sub_commands - 1];
    for (int i = MAX_ARGS - 2; i >= 0; i--) 
    {
        if (lastCmd->argv[i] == NULL) continue;
        if (strcmp(lastCmd->argv[i], "&") == 0) {
            command->background = 1;
            lastCmd->argv[i] = NULL; // Remove '&' from arguments
        } else if (strcmp(lastCmd->argv[i], ">") == 0 && lastCmd->argv[i + 1] != NULL) {
            command->stdout_redirect = lastCmd->argv[i + 1];
            lastCmd->argv[i] = NULL; // Remove '>' and filename from arguments
            lastCmd->argv[i + 1] = NULL;
        } else if (strcmp(lastCmd->argv[i], "<") == 0 && lastCmd->argv[i + 1] != NULL) {
            command->stdin_redirect = lastCmd->argv[i + 1];
            lastCmd->argv[i] = NULL; // Remove '<' and filename from arguments
            lastCmd->argv[i + 1] = NULL;
        }
    }
}

void PrintCommand(struct Command *command)
{
    // Print sub-commands
    for (size_t i = 0; i < command->num_sub_commands; i++)
    {
        SSubCommand* p_subcmd = &command->sub_commands[i];
        RELEASE(printf("Command %lu\n", i);)
        DEBUG(printf("Command %lu : '%s'\n", i, p_subcmd->line);)
        PrintArgs(p_subcmd->argv);
    }

    // Print redirects
    if (command->stdin_redirect != NULL) {
        printf("Redirect stdin: %s\n", command->stdin_redirect);
    }
    if (command->stdout_redirect != NULL) {
        printf("Redirect stdout: %s\n", command->stdout_redirect);
    }
    if (command->background) {
        printf("Background: yes\n");
    }
    else
    {
        printf("Background: no\n");
    }
}

// void print_args(int argc, char **argv)
void PrintArgs(char **argv)
{
    // Print arguments
    int i = 0;
    while (*argv != NULL) printf( DEBUG("  ") "argv[%d] = '%s'\n", i++, *(argv++));
    RELEASE(printf("\n");)
}

// void free_args(int argc, char **argv)
void FreeCommand(struct Command *command)
{
    // Free allocated memory
    for (size_t i = 0; i < command->num_sub_commands; i++)
    {
        SSubCommand* p_subcmd = &command->sub_commands[i];
        free(p_subcmd->line);
        for (size_t j = 0; j < MAX_ARGS; j++) if (p_subcmd->argv[j] != NULL) free(p_subcmd->argv[j]);
    }
}