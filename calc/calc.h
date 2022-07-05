#ifndef CALC_H
#define CALC_H

//#include "calc_global.h"

#ifdef __cplusplus
extern "C" {
#endif

enum Operation {
    Addition,
    Subtractions,
    Division,
    Multiplication,
    Unknown
};

//!
//! Error
//! -1 - division by zero
//! -2 - Unknown operation
//! 0  - no error
//!
/*CALC_EXPORT*/ double doIt (Operation op, double a, double b, short *error = nullptr);
#ifdef __cplusplus
}
#endif

#endif
