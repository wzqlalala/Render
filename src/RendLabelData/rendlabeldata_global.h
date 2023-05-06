#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(RENDLABELDATA_LIB)
#  define RENDLABELDATA_EXPORT Q_DECL_EXPORT
# else
#  define RENDLABELDATA_EXPORT Q_DECL_IMPORT
# endif
#else
# define RENDLABELDATA_EXPORT
#endif
