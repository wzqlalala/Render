#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(RENDLABEL_LIB)
#  define RENDLABEL_EXPORT Q_DECL_EXPORT
# else
#  define RENDLABEL_EXPORT Q_DECL_IMPORT
# endif
#else
# define RENDLABEL_EXPORT
#endif
