#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void error(const char *msg, ...)
{
    printf("Fatal error: ");
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
    printf("\n");
    exit(EXIT_FAILURE);
}