

#if !defined RELATIVE_HEADER_NAME
#error "RELATIVE_HEADER_NAME is not defined"
#endif

#if defined(TARGET_ARCH)
#undef TARGET_ARCH
#endif

#if defined(__ICC430__) || defined(__MSP430__)
#define TARGET_ARCH MSP430/
#elif defined(__ICCAVR__) || defined(__AVR__)
#define TARGET_ARCH Avr/
#elif defined(__ICCARM__) || defined ( __CC_ARM   ) || defined(__arm__)
#define TARGET_ARCH ARM/
#else

#if defined(DEFAULT_INCLUDE)
#include DEFAULT_INCLUDE
#undef DEFAULT_INCLUDE
#else
#error "Not supported arch."
#endif

#endif

#define STR_CONCAT2(First, Second) First/**/Second
#define STR_CONCAT(First, Second) STR_CONCAT2(First,Second)

#define INCLUDE_FILE_NAME(First, Second) <STR_CONCAT(First,Second)>

#include INCLUDE_FILE_NAME(TARGET_ARCH, RELATIVE_HEADER_NAME)

