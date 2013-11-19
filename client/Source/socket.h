#ifndef DE_SOCKET_H
#define DE_SOCKET_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#include "winsocket.h"
#define DE_WINDOWS_PLATFORM 1
#else
#include "linsocket.h"
#endif
#endif