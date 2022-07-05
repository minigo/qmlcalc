#include "calc.h"

#include <iostream>
#include <math.h>

double doIt (short op, double a, double b, short *error = nullptr)
{
    switch (op)
    {
    case Addition:
        return a + b;
    case Subtractions:
        return a - b;
    case Multiplication:
        return a * b;
    case Division:
        if (b < std::numeric_limits<double>::epsilon ()) {
            if (error)
                *error = -1;
            return 0;
        } else {
            return a/b;
        }
    case Unknown:
        if (error)
            *error = -2;
        return 0;
    default: {
        if (error)
            *error = -2;
        return 0;
    }
    }
}
