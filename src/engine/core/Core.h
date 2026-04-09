/**************************************************************************************************
 * @file Core.h
 * 
 * @brief The core macro and preprocessor definitions for the engine. This file is responsible for
 * the following things:
 * - Defining the version of the engine.
 * - Defining the target platform.
 * - Defining the build type of the engine.
 * - Defining the compiler being used.
 * - Defining the system's architecture.
 * - Translating various macros into Atlas' code convention compliant names.
 * - Universalizes the explicit keywords to be used for the engine. This means that the compiler
 * will not complain about the keywords not being supported.
 * 
 * @note This file should not contain any functional code (classes, functions, etc) nor should it
 * contain any includes as this is intended to be a file with configuration definitions and macros.
 * 
 * @date April 2025
 * 
 * @since v0.0.1
 * 
 *  Copyright 2024 Mute124
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License. 
 * ***************************************************************************************************/
#pragma once

// ---------------------------------------------------------------
// Configuration definitions
// ---------------------------------------------------------------

// These should eventually be undefined when the engine is released to the public. For now, they are
// defined because the engine is still in development.
#define ATLAS_USE_SDL2
#define ATLAS_USE_VULKAN
#define ATLAS_RENDERER_3D

// This MUST be changed if the engine version is changed.
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
	/**
	 * @brief When defined, the detected platform is Win32 (Windows).
	 * 
	 * @since v0.0.1
	 */
	#define ATLAS_PLATFORM_WINDOWS

#elif __APPLE__

	/**
	 * @brief When defined, the detected platform is MacOS. Currently, this platform is NOT supported
	 * as it is a low-priority feature to implement, and a compiler error is thrown to warn the user
	 * of this.
	 * 
	 * @since v0.0.1
	 */
	#define ATLAS_PLATFORM_MAC

	#error "Mac support is not yet implemented."
#elif __linux__

	/**
	* @brief When defined, the detected platform is Linux.
	* 
	* @since v0.0.1
	*/
	#define ATLAS_PLATFORM_LINUX

// This else statement shouldn't be necessary, but it is here for the sake of completeness.
#else
	
	/**
	 * @brief When defined, the detected platform is unknown. Usually, this should not happen unless
	 * the platform being used is not the main platforms listed above. If this is the case, please
	 * check your build settings and make sure that the platform is supported by the engine before
	 * submitting an issue on GitHub. When defined, this will throw an error to warn the user.
	 * 
	 * @since v0.0.1
	 */
	#define ATLAS_PLATFORM_UNKNOWN
	#error "The detected platform is unknown and is not supported by the engine. Please check your build settings and make sure that the platform is supported by the engine before submitting an issue on GitHub."
#endif

// find out processor type
#ifdef __x86_64__
	/**
	 * @brief When defined, the detected target processor architecture is x86_64 (64-bit).
	 * 
	 * @since v0.0.1
	 */
	#define ATLAS_PROCESSOR_X86_64
#elif __i386__

	/**
	 * @brief When defined, the detected target processor architecture is x86 (32-bit). 
	 * 
	 * @since v0.0.1
	 */
	#define ATLAS_PROCESSOR_X86
#endif

// Find out the compiler being used to build the engine.
#ifdef _MSC_VER

	/**
	 * @brief When defined, the detected compiler is Microsoft Visual Studio.
	 * 
	 * @since v0.0.1
	 */
	#define ATLAS_COMPILER_MSVC
#elif __clang__
	
	/**
	 * @brief When defined, the detected compiler is Clang.
	 * 
	 * @since v0.0.1
	 */
	#define ATLAS_COMPILER_CLANG
#elif __GNUC__

	/**
	 * @brief When defined, the detected compiler is GCC.
	 * 
	 * @since v0.0.1
	 */
	#define ATLAS_COMPILER_GCC
#endif

// TODO: Why isnt this all one preprocessing statement? Additionally, why isnt this with the above preprocessing statements?
#ifdef __MINGW32__
	/**
	 * @brief When defined, the detected compiler is MinGW.
	 * 
	 * @since v0.0.1
	 */
	#define ATLAS_COMPILER_MINGW
#elif defined(__MINGW64__)
	
	/**
	 * @brief When defined, the detected compiler is MinGW.
	 * 
	 * @since v0.0.1
	 */
	#define ATLAS_COMPILER_MINGW
#endif

// Check if conditional explicit is supported by the compiler
#ifdef __cpp_conditional_explicit

	/**
	 * @brief The compiler supports conditional explicit when the __cpp_conditional_explicit macro is defined.
	 * In terms of the changes this does to the code when defined, this means that the ATLAS_EXPLICT and the
	 * ATLAS_IMPLICIT macros will be defined with the explicit and the explicit(false) keyword.
	 * 
	 * @since v0.0.1
	 */
	#define ATLAS_CONDITIONAL_EXPLICIT_SUPPORTED
#endif // __cpp_conditional_explicit

// Check if the compiler supports the [[nodiscard]] attribute, and if it does, define the macro ATLAS_HAS_NODISCARD
#ifndef ATLAS_HAS_NODISCARD
	#ifndef __has_cpp_attribute
		#define ATLAS_HAS_NODISCARD 0
	#elif __has_cpp_attribute(nodiscard) >= 201603L
		#define ATLAS_HAS_NODISCARD 1
	#else
		#define ATLAS_HAS_NODISCARD 0
	#endif
#endif 

// If the compiler supports the [[nodiscard]] attribute, define the macro ATLAS_NODISCARD to [[nodiscard]], otherwise
// define it to nothing
#if ATLAS_HAS_NODISCARD
	#define ATLAS_NODISCARD [[nodiscard]]
#else
	#define ATLAS_NODISCARD
#endif

// If conditional explicit is supported, define the macros ATLAS_IMPLICIT and ATLAS_EXPLICIT to explicit(false)
// and explicit(true), otherwise define them to nothing
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

// This is obsolete.
#ifdef ATLAS_USE_GLFW3
	#ifdef ATLAS_USE_VULKAN

		// Define this here so that GLFW knows to include vulkan header files
		
		#define GLFW_INCLUDE_VULKAN	
	#endif
#endif

// ---------------------------------------------------------------
// Macro definitions (helpers)
// ---------------------------------------------------------------

// Macros to insert null checks
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