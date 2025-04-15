#pragma once
#define ATLAS_RENDERER_3D
// ---------------------------------------------------------------
// Platform definitions
// ---------------------------------------------------------------

// find out what platform we are on
#ifdef _WIN32
	#define ATLAS_PLATFORM_WINDOWS

#elif __APPLE__
	#define ATLAS_PLATFORM_MAC
	#error "Mac support is not yet implemented."
#elif __linux__
	#define ATLAS_PLATFORM_LINUX

#else
	#define ATLAS_PLATFORM_UNKNOWN
	#error "Unknown platform."

#endif

// find out processor type
#ifdef __x86_64__
	#define ATLAS_PROCESSOR_X86_64
#elif __i386__
	#define ATLAS_PROCESSOR_X86

#endif

// find out compiler type
#ifdef _MSC_VER
	#define ATLAS_COMPILER_MSVC
#elif __clang__
	#define ATLAS_COMPILER_CLANG
#elif __GNUC__
	#define ATLAS_COMPILER_GCC
#endif

// ---------------------------------------------------------------
// Macro definitions (helpers)
// ---------------------------------------------------------------

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

