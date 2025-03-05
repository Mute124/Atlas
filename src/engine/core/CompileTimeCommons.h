/*****************************************************************//**
 * @file   CompileTimeCommons.h
 * 
 * @brief  
 * 
 * @date   January 2025
 * 
 * @since v0.0.8
 *********************************************************************/
#pragma once

// ---------------------------------------------------------------
// Platform definitions
// ---------------------------------------------------------------

// find out what platform we are on
#ifdef _WIN32

/**
 * @brief The platform that the compiler is running on is
 * Windows.
 * 
 * @since v0.0.8
 */
#define ATLAS_PLATFORM_WINDOWS

#elif __APPLE__

/**
 * @brief The platform that the compiler is running on is
 * an Apple machine. If this is defined, it is an error.
 * See the warning below for why this is an error.
 * 
 * @warning Atlas does not yet (if ever) support Apple
 * platforms.
 * 
 * @since v0.0.8
 */
#define ATLAS_PLATFORM_MAC

#error "Mac support is not implemented and it may never be implemented. Please use Windows or Linux."

#elif __linux__

/**
 * @brief The platform that the compiler is running on is
 * a Linux machine. The distribution is unknown as this is
 * simply detected by checking if __linux__ is defined.
 * 
 * @since v0.0.8
 */
#define ATLAS_PLATFORM_LINUX

#else

/**
 * @brief The platform that the compiler is running on is
 * unknown and unsupported, therefore it is an error. See
 * below for why this is an error.
 * 
 * @warning This results in a compiler error as Atlas
 * does not yet (if ever) support this platform.
 * 
 * @since v0.0.8
 */
#define ATLAS_PLATFORM_UNKNOWN

#error "Unknown platform."

#endif

// find out processor type
#ifdef __x86_64__

/**
 * @brief The processor that the compiler is running on
 * is on x86_64 architecture.
 * 
 * @since v0.0.8
 */
#define ATLAS_PROCESSOR_X86_64

#elif __i386__

/**
 * @brief The processor that the compiler is running on is
 * on x86 architecture. 
 * 
 * @since v0.0.8
 */
#define ATLAS_PROCESSOR_X86

#endif

// find out compiler type
#ifdef _MSC_VER

/**
 * @brief The compiler being used is the
 * Microsoft Visual C++ compiler.
 * 
 * @since v0.0.8
 */
#define ATLAS_COMPILER_MSVC

#elif __clang__

/**
 * @brief The compiler being used is the Clang
 * compiler.
 * 
 * @since v0.0.8
 */
#define ATLAS_COMPILER_CLANG

#elif __GNUC__

/**
 * @brief The compiler being used is the GNU GCC
 * compiler.
 * 
 * @since v0.0.8
 */
#define ATLAS_COMPILER_GCC

#endif

/**
 * @brief This macro is used to mark a function as intentionally implicit.
 * 
 * @since v0.0.9
 */
#define ATLAS_IMPLICIT explicit(false)

/**
 * @brief A macro to define size_t.
 * 
 * @remarks In order to reduce the amount of times that size_t is written,
 * this macro is used. Furthermore, it exists to ensure that DRY (Don't
 * Repeat Yourself) is followed.
 * @since 
 */
#define ATLAS_SIZE_T size_t

#ifndef ATLAS_GENERATED_NULL_CHECK
	/**
	* @brief Macro to check if a pointer is null and return if it is. This is used to catch null pointer exceptions.
	* @since v0.0.9
	*/
#define ATLAS_GENERATED_NULL_CHECK(ptr) if(ptr == nullptr) { return; }
#endif

#ifndef ATLAS_GENERATED_NULL_CHECK_RETURN
	/**
	* @brief Macro to check if a pointer is null and return a null pointer if it is. This is used to catch null pointer exceptions.
	* @since v0.0.9
	*/
#define ATLAS_GENERATED_NULL_CHECK_RETURN(ptr) if(ptr == nullptr) { return nullptr; }
#endif

