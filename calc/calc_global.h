#ifndef CALC_GLOBAL_H
#define CALC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CALC_LIBRARY)
#  define CALC_EXPORT Q_DECL_EXPORT
#else
#  define CALC_EXPORT Q_DECL_IMPORT
#endif

#endif // CALC_GLOBAL_H
