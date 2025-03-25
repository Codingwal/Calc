#include "calc.h"
#include <stdlib.h>
#include <ctype.h>
#include "error.h"

#include <stdio.h>

static const char *it;

int64_t parseExpr(int minPrec); // Forward declaration

int64_t parseAtom()
{
    int64_t result;

    if (*it == '(')
    {
        result = parseExpr(1);
        it++;
        return result;
    }

    if (it[1] == 'x')
        result = strtol(it + 2, NULL, 16); // Skip 0x
    else if (it[1] == 'b')
        result = strtol(it + 2, NULL, 2); // Skip 0b
    else
        result = strtol(it, NULL, 10);

    while (isalnum(*it))
        it++;

    return result;
}

int precedence(char operator)
{
    switch (operator)
    {
    case ')':  // Special case required for subexpressions
    case '\0': // Special case for end of string
        return 0;
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    default:
        error("Invalid operator '%c' (%d)", operator,(int) operator);
        return -1; // Never reached
    }
}
typedef enum Associativity
{
    LEFT,
    RIGHT
} Associativity;
int associativity(char operator)
{
    switch (operator)
    {
    case '+':
    case '-':
    case '*':
    case '/':
        return LEFT;
    default:
        error("Invalid operator '%c'", operator);
        return -1; // Never reached
    }
}

int64_t calcValue(int64_t lhs, char operator, int64_t rhs)
{
    switch (operator)
    {
    case '+':
        lhs += rhs;
        break;
    case '-':
        lhs -= rhs;
        break;
    case '*':
        lhs *= rhs;
        break;
    case '/':
        lhs /= rhs;
        break;
    default:
        error("Invalid operator '%c'!!!", operator);
        break;
    }
    return lhs;
}

void consumeSpaces()
{
    while (isspace(*it))
        it++;
}

int64_t parseExpr(int minPrec)
{
    consumeSpaces();
    int64_t result = parseAtom(it);
    consumeSpaces();

    printf("parseExpr: result: %lld, *it: '%c' (%d)\n", result, *it, (int)*it);

    while (precedence(*it) >= minPrec)
    {
        printf("!\n");
        char op = *it;
        int prec = precedence(op);
        Associativity assoc = associativity(op);

        printf("1 prec: %d, assoc: %d, *it: '%c' (%d)\n", prec, (int)assoc, *it, (int)*it);

        int nextMinPrec;
        if (assoc == LEFT)
            nextMinPrec = prec + 1;
        else
            nextMinPrec = prec;

        it++; // Advance from operator to value

        printf("2 prec: %d, assoc: %d, *it: '%c' (%d)\n", prec, (int)assoc, *it, (int)*it);

        int64_t rhs = parseExpr(nextMinPrec);
        result = calcValue(result, op, rhs);

        consumeSpaces();
    }

    printf("res: %lld\n", result);
    return result;
}

int64_t calc(const char *str)
{
    it = str;
    return parseExpr(1);
}