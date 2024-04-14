/**
 * @author Qucheng Jiang
 * @author jiang.qu@northeastern.edu
 * @author NUID 001569593
 */

#pragma once

#include "unistd.h"

enum ERedirectStdio 
{
    REDIRECT_STDIN=STDIN_FILENO, 
    REDIRECT_STDOUT=STDOUT_FILENO, 
    REDIRECT_STDERR=STDERR_FILENO, 
};

typedef enum ERedirectStdio ERedirectStdio_t;
