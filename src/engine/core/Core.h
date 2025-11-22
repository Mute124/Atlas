/**
* @file Core.h
* 
* @brief Contains alot of core preprocessing and macro definitions
* 
*/
#pragma once

#define ATLAS_USE_SDL2
#define ATLAS_USE_VULKAN
#define ATLAS_RENDERER_3D

#define ATLAS_VERSION_MAJOR 0
#define ATLAS_VERSION_MINOR 0
#define ATLAS_VERSION_PATCH 1

#ifdef _DEBUG
	#define ATLAS_DEBUG
#endif

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

#ifdef __MINGW32__
	#define ATLAS_COMPILER_MINGW
#elif defined(__MINGW64__)
	
	#define ATLAS_COMPILER_MINGW
#endif

#ifdef __cpp_conditional_explicit
	#define ATLAS_CONDITIONAL_EXPLICIT_SUPPORTED
#endif

#ifdef ATLAS_CONDITIONAL_EXPLICIT_SUPPORTED
	#define ATLAS_IMPLICIT explicit(false)
	#define ATLAS_EXPLICIT explicit(true)
#else 
	#define ATLAS_IMPLICIT
	#define ATLAS_EXPLICIT
#endif

// ---------------------------------------------------------------
// Library definitions
// ---------------------------------------------------------------

// ---------------------------------------------------------------
// Renderer Macros
// ---------------------------------------------------------------

// Ensure that raylib is not being used.
#ifdef ATLAS_USE_RAYLIB
	#error "Atlas does not yet support raylib."
#endif

#ifdef ATLAS_USE_GLFW3
	#ifdef ATLAS_USE_VULKAN

		// Define this here so that GLFW knows to include vulkan header files
		
		#define GLFW_INCLUDE_VULKAN	
	#endif
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


// ---------------------------------------------------------------
// Macro definitions (reminders)
// ---------------------------------------------------------------

#define ATLAS_STRINGIZE( L )     #L
#define ATLAS_MAKE_STRING( M, L ) M(L)
#define $Line ATLAS_MAKE_STRING( ATLAS_STRINGIZE, __LINE__ )
#define ATLAS_REMINDER __FILE__ "(" $Line ") : Reminder: "