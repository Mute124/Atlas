cmake_minimum_required(VERSION 3.29)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Distribution")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
    "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,Release>:EditAndContinue>,$<$<CONFIG:Debug,Release>:ProgramDatabase>>"
  )
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# The version of CPM to install if it is not already installed
set(CPM_INSTALL_VERSION 0.40.2)
#set(CPM_SOURCE_CACHE "${CMAKE_BINARY_DIR}/pkgCache")


# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib") # Static libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # Shared libraries (.so/.dll)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # Executables (.out/.exe)

if (WIN32)
    message(STATUS "${CMAKE_SOURCE_DIR}/build-scripts/build-windows.bat")

    if (NOT EXISTS "${CMAKE_SOURCE_DIR}/cache/lock-configure.lock")
        
        #execute_process(
        #    # Add or remove arguments as needed. Use --debug for debug packages, --release for release packages, and --distribution for distribution packages
        #    COMMAND "${CMAKE_SOURCE_DIR}/build-windows.bat"
        #    RESULT_VARIABLE result
        #    OUTPUT_VARIABLE output
        #    ERROR_VARIABLE error_output
        #)
        #message(STATUS "${result}")
        #if(NOT ${result} EQUAL 0)
        #    message(FATAL_ERROR "The command failed with error: ${error_output}, ${output}")
        #endif()
       
        #        cmake -B ${{ steps.strings.outputs.build-output-dir }}
        #-DCMAKE_CXX_COMPILER=${{ matrix.cpp_compiler }}
        #-DCMAKE_C_COMPILER=${{ matrix.c_compiler }}
        #-DCMAKE_BUILD_TYPE=${{ matrix.build_type }}
        #-S ${{ github.workspace }}
    endif()
else()
   # execute_process(
   #     # Add or remove arguments as needed. Use --debug for debug packages, --release for release packages, and --distribution for distribution packages
   #     COMMAND "${CMAKE_SOURCE_DIR}/build-scripts/build-linux.sh" 
   #     RESULT_VARIABLE result
   #     OUTPUT_VARIABLE output
   #     ERROR_VARIABLE error_output
   # )
	#if(NOT ${result} EQUAL 0)
	#    message(FATAL_ERROR "The command failed with error: ${error_output}, ${output}")
	#endif()
endif()

# Platform-specific settings
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)  # To export symbols from DLLs
    set(CMAKE_RUNTIME_OUTPUT_NAME_SUFFIX ".exe")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
elseif(UNIX)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# Set project name
project(Atlas LANGUAGES CXX C)

include("cmake/cpm.cmake")
include("cmake/techstorm.cmake")
include("CMakeOptions.txt")

set(CPM_USE_LOCAL_PACKAGES ON)

if(CPM_SOURCE_CACHE)
  set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif(DEFINED ENV{CPM_SOURCE_CACHE})
  set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else()
  set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif()

# Set compilation definitions
if(CMAKE_BUILD_TYPE STREQUAL "Distribution")
    add_compile_definitions(ATLAS_ROOT_DIR="../") # Sets the TS_ROOT_DIR macro in a way that prevents the path on a development machine from being used in distribution.
    add_compile_definitions(ATLAS_TOP_LAYER="${CMAKE_SOURCE_DIR}")
    # Your code for the Distribution configuration

else()
   add_compile_definitions(ATLAS_ROOT_DIR="${CMAKE_BINARY_DIR}")
   add_compile_definitions(ATLAS_TOP_LAYER="${CMAKE_SOURCE_DIR}")

    # Your code for other configurations

endif()

if(${ATLAS_ENABLE_HAPTICS}) 
    add_compile_definitions(ATLAS_ENABLE_HAPTICS)
endif()

if(${ATLAS_ENABLE_EXTENSIONS}) 
    add_compile_definitions(ATLAS_ENABLE_EXTENSIONS)
    if(${ATLAS_ENABLE_DISCORD_RPC})
        add_compile_definitions(ATLAS_ENABLE_DISCORD_RPC)
    endif()
endif()

if(${ATLAS_ENABLE_MODDING})
    add_compile_definitions(ATLAS_ENABLE_MODDING)
    if(${ATLAS_ENABLE_ANGELSCRIPT})
        add_compile_definitions(ATLAS_ENABLE_ANGELSCRIPT)
    endif()
    
    if(${ATLAS_ENABLE_LUA})
        add_compile_definitions(ATLAS_ENABLE_LUA)
    endif()
endif()	



# Absolutely DO NOT remove this set command. It will break all rendering functionality
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE) 

#FetchContent_Declare(
#  raylib
#  GIT_REPOSITORY https://github.com/raysan5/raylib.git
#  GIT_TAG        5.5
#)

#FetchContent_MakeAvailable(raylib)

CPMAddPackage(
    NAME raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    OPTIONS
        "BUILD_SHARED_LIBS=ON"
)

#CPMAddPackage(
#    NAME raylib_nuklear
#    GIT_REPOSITORY https://github.com/RobLoach/raylib-nuklear.git
#    GIT_TAG v5.5.0
#)

CPMAddPackage(
    NAME spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.0
)

# find_package(ZLIB REQUIRED)
find_package(joltphysics REQUIRED)
find_package(boost REQUIRED)
find_package(libconfig REQUIRED)
find_package(Angelscript REQUIRED)

if(${ATLAS_ENABLE_MODDING})
    if(${ATLAS_ENABLE_ANGELSCRIPT})
       # find_package(Angelscript REQUIRED)
    endif()
    
    if(${ATLAS_ENABLE_LUA})
        find_package(sol2 REQUIRED)
    endif()
endif()


find_package(eventpp REQUIRED)
find_package(SDL2 REQUIRED CONFIG)
find_package(rmlui REQUIRED)
find_package(fmt REQUIRED)
find_package(nuklear REQUIRED)

find_package(simdjson REQUIRED)
find_package(tinyxml2 REQUIRED)
find_package(miniaudio REQUIRED)

#if(${ATLAS_ENABLE_TESTING})
    find_package(GTest REQUIRED)
#endif()

set(DOUBLE_PRECISION OFF)
set(GENERATE_DEBUG_SYMBOLS OFF)
set(CROSS_PLATFORM_DETERMINISTIC ON)
set(FLOATING_POINT_EXCEPTIONS_ENABLED OFF)
set(OBJECT_LAYER_BITS 32)
set(USE_SSE4_1 ON)
set(USE_SSE4_2 ON)
set(USE_AVX ON)
set(USE_AVX2 ON)
set(USE_AVX512 OFF)
set(USE_LZCNT ON)
set(USE_TZCNT ON)
set(USE_F16C ON)
set(USE_FMADD ON)

add_subdirectory("src/engine")

if (${ATLAS_CUSTOM_APPLICATION_HANDLING} EQUAL "OFF")
add_subdirectory("src/project")

# Currently, Techstorm only supports the default implementation, however, it is on the list of things to do.
if(${ATLAS_CUSTOM_APPLICATION_HANDLING} EQUAL "OFF")
	message(STATUS "Building application")
    add_subdirectory("src/application")
endif()

if(${ATLAS_BUILD_TOOLS} EQUAL "ON")
    message(STATUS "Building tools")
    add_subdirectory("src/tools")
endif()

if(${ATLAS_ENABLE_TESTING} EQUAL "ON")
    

	message(STATUS "Building tests")
	add_subdirectory("src/tests")

endif()