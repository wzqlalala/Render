#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MXGEOCOMMAND_LIB)
#  define MXGEOCOMMAND_EXPORT Q_DECL_EXPORT
# else
#  define MXGEOCOMMAND_EXPORT Q_DECL_IMPORT
# endif
#else
# define MXGEOCOMMAND_EXPORT
#endif
