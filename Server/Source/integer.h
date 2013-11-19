/*******************************************************************************
* Credits:  Stephan
******************************************************************************/
#ifndef DE_INTEGER_H
#define DE_INTEGER_H

#include <limits.h>
#include "bool.h"

#if 0
int8: -128 to 127
uint8: 0 to 255
int16: -32,768 to 32,767
Uint16: 0 to 65,535
int32: -2,147,483,648 to 2,147,483,647
Uint32: 0 to 4,294,967,295
int64: –9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
Uint64: 0 to 18,446,744,073,709,551,615
#endif

		/* Unsigned 8-bit integer. */
#ifndef UINT8_MAX
#define UINT8_MAX 0xFF
#endif /* UINT8_MAX */

#ifndef uint8
#ifdef UINT8_C
#undef UINT8_C
#endif /* UINT8_C */

#if (UCHAR_MAX == UINT8_MAX)
		typedef unsigned char                   uint8;
#define UINT8_C(v)                      ((uint8)v)
#else
#error No unsigned 8-bit integer defined for this platform.
#endif
#endif /* Uint8 */

		/* Signed 8-bit integer. */
#ifndef INT8_MAX
#define INT8_MAX 0x7F
#endif /* INT8_MAX */

#ifndef int8
#ifdef INT8_C
#undef INT8_C
#endif /* INT8_C */

#if (SCHAR_MAX == INT8_MAX)
		typedef signed char                     int8;
#define INT8_C(v)                       ((int8)v)
#else
#error No signed 8-bit integer defined for this platform.
#endif
#endif /* Int8 */

		/* Unsigned 16-bit */
#ifndef UINT16_MAX
#define UINT16_MAX 0xFFFF
#endif /* UINT16_MAX */

#ifndef uint16
#ifdef UINT16_C
#undef UINT16_C
#endif /* UINT16_C */

#if (UINT_MAX == UINT16_MAX)
		typedef unsigned int                    uint16;
#define UINT16_C(v)                     ((uint16)v)
#elif (USHRT_MAX == UINT16_MAX)
		typedef unsigned short                  uint16;
#define UINT16_C(v)                     ((uint16)v)
#else
#error No unsigned 16-bit integer defined for this platform.
#endif
#endif /* Uint16 */

		/* Signed 16-bit */
#ifndef INT16_MAX
#define INT16_MAX 0x7FFF
#endif /* INT16_MAX */

#ifndef int16
#ifdef INT16_C
#undef INT16_C
#endif /* INT16_C */

#if (INT_MAX == INT16_MAX)
		typedef signed int                      int16;
#define INT16_C(v)                      ((int16)v)
#elif (SHRT_MAX == INT16_MAX)
		typedef signed short                    int16;
#define INT16_C(v)                      ((int16)v)
#else
#error No signed 16-bit integer defined for this platform.
#endif
#endif /* Int16*/

		/* Unsigned 32-bit */
#ifndef UINT32_MAX
#define UINT32_MAX (0xFFFFFFFFUL)
#endif

#ifndef uint32
#ifdef UINT32_C
#undef UINT32_C
#endif /* UINT32_C */

#if (ULONG_MAX == UINT32_MAX)
		typedef unsigned long                   uint32;
#define UINT32_C(v)                     ((uint32)v)
#elif (UINT_MAX == UINT32_MAX)
		typedef unsigned int                    uint32;
#define UINT32_C(v)                     ((uint32)v)
#elif (USHRT_MAX == UINT32_MAX)
		typedef unsigned short                  uint32;
#define UINT32_C(v)                     ((uint32)v)
#else
#error No unsigned 32-bit integer defined for this platform.
#endif
#endif

		/* Signed 32-bit */
#ifndef INT32_MAX
#define INT32_MAX (0x7FFFFFFFUL)
#endif

#ifndef int32
#ifdef INT32_C
#undef INT32_C
#endif /* INT32_C */

#if (LONG_MAX == INT32_MAX)
		typedef signed long                     int32;
#define INT32_C(v)                      ((int32)v)
#elif (INT_MAX == INT32_MAX)
		typedef signed int                      int32;
#define INT32_C(v)                      ((int32)v)
#elif (SHRT_MAX == INT32_MAX)
		typedef signed short                    int32;
#define INT32_C(v)                      ((int32)v)
#else
#error No signed 32-bit integer defined for this platform.
#endif
#endif

		/* Unsigned 64-bit. */
#ifndef uint64
#ifdef UINT64_C
#undef UINT64_C
#endif /* UINT64_C */

#if defined(__GNUC__)
		__extension__ typedef unsigned long long uint64;
#define UINT64_C(v)                     ((uint64)v ## ULL)
#elif defined(__MWERKS__) || defined(__SUNPRO_C) || defined(__SUNPRO_CC) || \
	defined(__APPLE_CC__) || defined(_LONG_LONG) || defined(_CRAYC)
		typedef unsigned long long              uint64;
#define UINT64_C(v)                     ((uint64)v ## ULL)
#elif (defined(__WATCOMC__) && defined(__WATCOM_INT64__)) || \
	(defined(_MSC_VER) && _INTEGRAL_MAX_BITS >= 64) || \
	(defined (__BORLANDC__) && __BORLANDC__ > 0x460) || defined (__alpha) || \
	defined (__DECC)
		typedef unsigned __int64                uint64;
#define UINT64_C(v)                     ((uint64)v ## UI64)
#endif
#endif /* Uint64 */

	  /* Signed 64-bit. */
#ifndef int64
#ifdef INT64_C
#undef INT64_C
#endif /* INT64_C */

#if defined(__GNUC__)
	  __extension__ typedef long long         int64;
#define INT64_C(v)                      ((int64)v ## LL)
#elif defined(__MWERKS__) || defined(__SUNPRO_C) || defined(__SUNPRO_CC) || \
	defined(__APPLE_CC__) || defined(_LONG_LONG) || defined(_CRAYC)
	  typedef long long                       int64;
#define INT64_C(v)                      ((int64)v ## LL)
#elif (defined(__WATCOMC__) && defined(__WATCOM_INT64__)) || \
	(defined(_MSC_VER) && _INTEGRAL_MAX_BITS >= 64) || \
	(defined (__BORLANDC__) && __BORLANDC__ > 0x460) || defined (__alpha) || \
	defined (__DECC)
	  typedef __int64                         int64;
#define INT64_C(v)                      ((int64)v ## I64)
#endif
#endif /* Int64 */

#endif
