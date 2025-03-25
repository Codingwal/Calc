#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "error.h"
#include "calc.h"

typedef unsigned char byte;

bool string_compare(const char *a, const char *b)
{
    return strcmp(a, b) == 0;
}

void printBinary(int64_t value)
{
    byte *arr = (byte *)&value;
    bool oneFound = false;

    for (int i = 7; i >= 0; i--)
    {
        byte b = arr[i];
        for (size_t j = 0; j < 8; j++)
        {
            if (b & 0b10000000)
            {
                oneFound = true;
                putchar('1');
            }
            else if (oneFound)
                putchar('0');

            b <<= 1;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        error("Expected second argument with expression to parse");

    // Parse arguments (last argument is the expression to calculate)
    int outputBase = 0;
    for (size_t i = 1; i < (size_t)argc - 1; i++)
    {
        char *str = argv[i];
        if (string_compare(str, "--dec"))
        {
            if (outputBase != 0)
                error("Multiple output base specifiers");
            outputBase = 10;
        }
        else if (string_compare(str, "--hex"))
        {
            if (outputBase != 0)
                error("Multiple output base specifiers");
            outputBase = 16;
        }
        else if (string_compare(str, "--bin"))
        {
            if (outputBase != 0)
                error("Multiple output base specifiers");
            outputBase = 2;
        }
        else
        {
            error("Invalid argument \"%s\"", str);
        }
    }
    if (outputBase == 0)
        outputBase = 10;

    int64_t res = calc(argv[argc - 1]);

    puts("\n");

    if (outputBase == 10)
        printf("Result: %lld\n", res);
    else if (outputBase == 16)
        printf("Result: 0x%llx\n", res);
    else if (outputBase == 2)
    {
        printf("Result: 0b");
        printBinary(res);
        printf("\n");
    }
    else
        error("WTF");
}