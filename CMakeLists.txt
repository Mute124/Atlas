cmake_minimum_required(VERSION 3.29)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Distribution")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    message(STATUS "Enabling MSVC Debug Information Format")
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

#-----------------------------------------------------------------------------------
# Set Options for Atlas
#-----------------------------------------------------------------------------------

# Modules
option(ATLAS_ENABLE_HAPTICS "Should haptic experiences be enabled" OFF)
option(ATLAS_ENABLE_EXTENSIONS "Should extension functionality be enabled" OFF)
option(ATLAS_ENABLE_DISCORD_RPC "Should discord rich presense be enabled" OFF)

# Modding options
option(ATLAS_ENABLE_MODDING "Should modding be enabled" ON)
option(ATLAS_ENABLE_ANGELSCRIPT "Should AngelScript be enabled for modding" ON) # Currently, this is not yet implemented
option(ATLAS_ENABLE_LUA "Should Lua be enabled for modding" ON)

# Architecture options
option(ATLAS_LIBRARY_MODE "Should Atlas' run application automatically handle things" OFF)
option(ATLAS_BARE_BONES "Should Atlas only build the bare minimum" OFF)
option(ATLAS_SEPERATE_OUTPUT "Should Atlas put libraries in the lib folder and executables in the bin folder" ON)

# Rendering backend options
option(ATLAS_USE_RAYLIB "Should Atlas use raylib" OFF) # this will satisfy the backend and window API requirements
option(ATLAS_USE_VULKAN "Should Atlas use Vulkan (You still need to have Vulkan SDK installed)" ON)

# Window API options
option(ATLAS_USE_GLFW "Should Atlas use GLFW" OFF)
option(ATLAS_USE_SDL2 "Should Atlas use SDL2" ON)

# Build target options
option(ATLAS_BUILD_EXAMPLES "Build Examples For Atlas" ON) # Not implemented yet
option(ATLAS_BUILD_TOOLS "Build Tools For Atlas" OFF)
option(ATLAS_ENABLE_TESTING "Enable Unit Tests For Atlas" OFF) # Not implimented yet

# Output directories
if(${ATLAS_SEPERATE_OUTPUT})
    message("Atlas will put libraries in the lib folder and executables in the bin folder")

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib") # Static libraries
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # Shared libraries (.so/.dll)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # Executables (.out/.exe)
endif()

# Platform-specific settings
if(WIN32)
    message("Atlas is building on Windows")
	set(ATLAS_PLATFORM_WINDOWS TRUE)

    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)  # To export symbols from DLLs
    set(CMAKE_RUNTIME_OUTPUT_NAME_SUFFIX ".exe")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
elseif(UNIX)
    message("Atlas is building on a Unix system")
    set(ATLAS_PLATFORM_UNIX TRUE)

    # Ensure that shared libraries are named .so since that is what it is on unix.
    # TODO: Check to see if this is the best way to do this
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
else()
    # since the platform is unknown, throw an error
    # TODO: throw an error

endif()

# This should remain here because it fixed some compiler errors regarding missing symbols
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set project name
project(Atlas LANGUAGES CXX C)

include("cmake/cpm.cmake")
include("cmake/techstorm.cmake")

#-----------------------------------------------------------------------------------
# Code Definitions
#-----------------------------------------------------------------------------------
message(STATUS "Setting up code definitions...")

# Set compilation definitions. This is heavily used by the file system
if(CMAKE_BUILD_TYPE STREQUAL "Distribution")
    add_compile_definitions(ATLAS_ROOT_DIR="../") # Sets the TS_ROOT_DIR macro in a way that prevents the path on a development machine from being used in distribution.
    add_compile_definitions(ATLAS_TOP_LAYER="${CMAKE_SOURCE_DIR}")
    # Your code for the Distribution configuration

else()
   add_compile_definitions(ATLAS_ROOT_DIR="${CMAKE_BINARY_DIR}")
   add_compile_definitions(ATLAS_TOP_LAYER="${CMAKE_SOURCE_DIR}")
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

add_compile_definitions(ATLAS_ENABLE_TESTING=${ATLAS_ENABLE_TESTING})
add_compile_definitions(ATLAS_TESTING)

message(STATUS "Finished setting up code definitions")

#-----------------------------------------------------------------------------------
# Global Dependencies
#-----------------------------------------------------------------------------------

message(STATUS "Handling dependencies...")


if(${ATLAS_USE_RAYLIB})
    # TODO: Make raylib usage optional.
    # Absolutely DO NOT remove this if using raylib. It will break all rendering functionality
    set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE) 
    CPMAddPackage(
        NAME raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
        OPTIONS
            "BUILD_SHARED_LIBS=ON"
    )
endif()

CPMAddPackage(
    NAME glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

CPMAddPackage(
    NAME vk-bootstrap
    GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap.git
    GIT_TAG v1.4.312
)

CPMAddPackage(
    NAME spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.0
)

find_package(joltphysics REQUIRED)
find_package(boost REQUIRED)
find_package(libconfig REQUIRED)

if(${ATLAS_ENABLE_MODDING})
    if(${ATLAS_ENABLE_ANGELSCRIPT})
        find_package(Angelscript REQUIRED)
    endif()
    
    if(${ATLAS_ENABLE_LUA})
        find_package(sol2 REQUIRED)
    endif()
endif()


find_package(eventpp REQUIRED)

find_package(SDL2 REQUIRED CONFIG)
find_package(rmlui REQUIRED)
find_package(fmt REQUIRED)

find_package(simdjson REQUIRED)
find_package(tinyxml2 REQUIRED)

find_package(miniaudio REQUIRED)

if(${ATLAS_ENABLE_TESTING})
    find_package(GTest REQUIRED)
endif()


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

# Currently, Techstorm only supports the default implementation, however, it is on the list of things to do.
if(NOT ${ATLAS_LIBRARY_MODE})
    add_subdirectory("src/project")

	message(STATUS "Building application")
    add_subdirectory("src/application")
endif()

if(${ATLAS_BUILD_TOOLS})
    message(STATUS "Building tools")
    add_subdirectory("src/tools")
endif()

if(${ATLAS_ENABLE_TESTING})
    

	message(STATUS "Building tests")
	add_subdirectory("src/tests")
endif()