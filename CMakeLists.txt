cmake_minimum_required(VERSION 3.29)

message(STATUS "Hello from Root CMakeLists.txt!")
message(STATUS "[Step] Setting up configurations")

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Distribution")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# The version of CPM to install if it is not already installed
set(CPM_INSTALL_VERSION 0.40.2)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    message(STATUS "Enabling MSVC Debug Information Format")
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
        "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,Release>:EditAndContinue>,$<$<CONFIG:Debug,Release>:ProgramDatabase>>"
    )
endif()

#-----------------------------------------------------------------------------------
# Set Options for Atlas
#-----------------------------------------------------------------------------------

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

#-----------------------------------------------------------------------------------
# Global Dependencies
#-----------------------------------------------------------------------------------

message(STATUS "[Step] Handling dependencies...")

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

add_subdirectory("${PROJECT_SOURCE_DIR}/include/fastgltf")
find_package(boost REQUIRED)
find_package(eventpp REQUIRED)
find_package(SDL2 REQUIRED CONFIG)
find_package(fmt REQUIRED)
find_package(vulkan-memory-allocator REQUIRED)

message(STATUS "[Step] Setting up Vulkan")
message(STATUS "[Sub-Step] Attempting to find Vulkan SDK")

if (DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
  set(Vulkan_FOUND "True")

  message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
else()
  find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
  
endif()

if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()

message(STATUS "[Sub-Step] Finding glslangValidator")
find_program(GLSL_VALIDATOR glslangValidator $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)

message(STATUS "[Sub-Step] Finding Shaders")
file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/shaders/*.frag"
    "${PROJECT_SOURCE_DIR}/shaders/*.vert"
    "${PROJECT_SOURCE_DIR}/shaders/*.comp"
)

message(STATUS "[Sub-Step] Compiling Shaders")

# For whatever reason, this does not actually build the shaders. I dont know for sure why, but I think it might be due to CMake not being
# able to find the glslangValidator. I'm not sure.
foreach(GLSL ${GLSL_SOURCE_FILES})
  message(STATUS "Building Shader: ${GLSL}")
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
  message(STATUS ${GLSL})
  message(STATUS COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV})
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o 
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
)

if(${ATLAS_ENABLE_TESTING})
    find_package(GTest REQUIRED)
endif()

add_subdirectory("src")