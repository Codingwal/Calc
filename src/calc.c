#include "calc.h"
#include <stdlib.h>
#include <ctype.h>
#include "error.h"
#include <math.h>

// Algorithm from
// https://eli.thegreenplace.net/2012/08/02/parsing-expressions-by-precedence-climbing

static const char *it;

int64_t parseExpr(int minPrec); // Forward declaration

int64_t parseAtom()
{
    int64_t result;

    if (*it == '(')
    {
        it++;
        result = parseExpr(1);
        it++;
        return result;
    }

    if (it[1] == 'x')
    {
        it += 2; // Skip "0x"
        result = strtoll(it, NULL, 16);
    }
    else if (it[1] == 'b')
    {
        it += 2; // Skip "0b"
        result = strtoll(it, NULL, 2);
    }
    else
        result = strtoll(it, NULL, 10);

    while (isdigit(*it) || ('a' <= *it && *it <= 'f') || ('A' <= *it && *it <= 'F'))
        it++;

    if (isalpha(*it))
    {
        if (it[1] == 'i' && it[2] == 'B')
        {
            switch (it[0])
            {
            case 'K': // Kibibytes
                it += 3;
                return result * 1024;
            case 'M': // Mebibytes
                it += 3;
                return result * 1024 * 1024;
            case 'G': // Gibibytes
                it += 3;
                return result * 1024 * 1024 * 1024;
            default:
                error("Invalid unit (\"%s\")", it);
            }
        }
    }

    return result;
}

int precedence(char operator)
{
    switch (operator)
    {
    case ')':  // Special case required for subexpressions
    case '\0': // Special case for end of string
        return 0;
    case '<':
    case '>':
        return 1;
    case '+':
    case '-':
        return 2;
    case '*':
    case '/':
        return 3;
    case '^':
        return 4;
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
    case '^':
        return RIGHT;
    case '+':
    case '-':
    case '*':
    case '/':
    case '<':
    case '>':
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
    case '^':
        lhs = (int64_t)pow(lhs, rhs);
        break;
    case '<':
        lhs <<= rhs;
        break;
    case '>':
        lhs >>= rhs;
        break;
    default:
        error("Invalid operator '%c'", operator);
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

    while (precedence(*it) >= minPrec)
    {
        char op = *it;
        int prec = precedence(op);
        Associativity assoc = associativity(op);

        int nextMinPrec;
        if (assoc == LEFT)
            nextMinPrec = prec + 1;
        else
            nextMinPrec = prec;

        it++; // Advance from operator to value

        int64_t rhs = parseExpr(nextMinPrec);
        result = calcValue(result, op, rhs);

        consumeSpaces();
    }

    return result;
}

int64_t calc(const char *str)
{
    it = str;
    return parseExpr(1);
}