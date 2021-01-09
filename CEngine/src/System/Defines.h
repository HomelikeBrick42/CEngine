#pragma once

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;

typedef signed char			s8;
typedef signed short		s16;
typedef signed int			s32;
typedef signed long long	s64;

typedef float				f32;
typedef double				f64;

typedef u8					b8;

_Static_assert(sizeof(u8) == 1, "Expected sizeof u8 to be 1 byte.");
_Static_assert(sizeof(u16) == 2, "Expected sizeof u16 to be 2 bytes.");
_Static_assert(sizeof(u32) == 4, "Expected sizeof u32 to be 4 bytes.");
_Static_assert(sizeof(u64) == 8, "Expected sizeof u64 to be 8 bytes.");

_Static_assert(sizeof(s8) == 1, "Expected sizeof s8 to be 1 byte.");
_Static_assert(sizeof(s16) == 2, "Expected sizeof s16 to be 2 bytes.");
_Static_assert(sizeof(s32) == 4, "Expected sizeof s32 to be 4 bytes.");
_Static_assert(sizeof(s64) == 8, "Expected sizeof s64 to be 8 bytes.");

_Static_assert(sizeof(f32) == 4, "Expected sizeof f32 to be 4 bytes.");
_Static_assert(sizeof(f64) == 8, "Expected sizeof f64 to be 8 bytes.");

#if !defined(TRUE)
	#define TRUE 1
#endif

#if !defined(FALSE)
	#define FALSE 0
#endif

#if !defined(NULL)
	#define NULL ((void*)0)
#endif

#if !defined(CLAMP)
	#define CLAMP(value, min, max) (value <= min) ? min : (value >= max) ? max : value
#endif

#define DECLARE_TYPE(type) typedef struct type type
#define DECLARE_ENUM(type) typedef enum type type

#if _WIN32 || _WIN64
	#define PLATFORM_WINDOWS 1
#else
	#error "Unsupported platform!"
#endif
