# The sources are seperated into different variables for organization reasons

set(ATLAS_COMPONENT_SOURCES

)

set(ATLAS_ENGINE_CORE_SOURCES
    "core/Core.h"
    "core/Common.h" 

    "core/Module.h"
    #"common.cpp"

    "core/ThreadSafeVariable.h"
    "core/ThreadSafeVariable.cpp"

    "core/MemoryAllocator.h"
    "core/MemoryAllocator.cpp"
    
    "core/AtlasEngine.h"
    "core/AtlasEngine.cpp"


)

set(ATLAS_RENDERER_SOURCES
    "renderer/backend/RenderingBackend.h"

    "renderer/backend/VulkanRenderingBackend.h"
    "renderer/backend/VulkanRenderingBackend.cpp"


    "renderer/window/Window.h" 
    "renderer/window/Window.cpp" 

    "renderer/Renderer.h" 
    "renderer/Renderer.cpp"
)

set(ATLAS_STATE_SOURCES

)

set(ATLAS_SYSTEM_SOURCES

)

set(ATLAS_UTIL_SOURCES


)

set(ATLAS_MATH_SOURCES

)

set(ATLAS_INPUT_SOURCES

)

set(ATLAS_CONFIG_SOURCES

)

set(ATLAS_FILESYSTEM_SOURCES

)

set(ATLAS_PHYSICS_SOURCES

)

set(ATLAS_MODDING_SOURCES

)

set(ATLAS_DEBUGGING_SOURCES 
 
)

set(ATLAS_WORLD_SOURCES

)

set(ATLAS_LANGUAGE_SOURCES

)

set(ATLAS_PROJECT_SOURCES 

)

set(ATLAS_AUDIO_SOURCES

)

set(ATLAS_ENGINE_SOURCES 
	# Engine Core
    ${ATLAS_COMPONENT_SOURCES}
    ${ATLAS_ENGINE_CORE_SOURCES}
    ${ATLAS_RENDERER_SOURCES}
    ${ATLAS_STATE_SOURCES}
    ${ATLAS_SYSTEM_SOURCES}
    ${ATLAS_UTIL_SOURCES}
	${ATLAS_MATH_SOURCES}
    ${ATLAS_INPUT_SOURCES}
    ${ATLAS_CONFIG_SOURCES}
    ${ATLAS_FILESYSTEM_SOURCES}
	${ATLAS_PHYSICS_SOURCES}
	${ATLAS_MODDING_SOURCES}
	${ATLAS_DEBUGGING_SOURCES}
    ${ATLAS_WORLD_SOURCES}
    ${ATLAS_LANGUAGE_SOURCES}
    ${ATLAS_PROJECT_SOURCES}
    ${ATLAS_AUDIO_SOURCES}
)

if (DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
  set(Vulkan_FOUND "True")

else()
  find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
  message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()

# Please note that this is NOT the final list of dependencies, some are toggled based on options
set(ATLAS_ENGINE_DEPENDENCIES
    raylib 

    joltphysics::joltphysics 
    joltphysics_DEPS_TARGET

    libconfig::libconfig
    libconfig_libconfig_libconfig++_DEPS_TARGET

    eventpp::eventpp

    rmlui::rmlui
    rmlui_DEPS_TARGET

    spdlog::spdlog
    Vulkan::Vulkan
    simdjson::simdjson
    tinyxml2::tinyxml2
    Angelscript::angelscript
    boost::boost
    miniaudio::miniaudio
)

include("${CMAKE_SOURCE_DIR}/CMakeOptions.txt")

# Set modding related targets based on options
if(${ATLAS_ENABLE_MODDING})
    if(${ATLAS_ENABLE_ANGELSCRIPT})
        set(ATLAS_ENGINE_DEPENDENCIES
            ${ATLAS_ENGINE_DEPENDENCIES}
		   # Angelscript::angelscript
        )
        
    endif()

    if(${ATLAS_ENABLE_LUA})
        set(ATLAS_ENGINE_DEPENDENCIES
            ${ATLAS_ENGINE_DEPENDENCIES}
			sol2::sol2
        )
    endif()
endif()

#if(${ATLAS_ENABLE_TESTING}) 
    set(ATLAS_ENGINE_DEPENDENCIES 
		${ATLAS_ENGINE_DEPENDENCIES}
    #    gtest::gtest
    )
#endif()

add_library(Atlas SHARED 
    ${ATLAS_ENGINE_SOURCES}
)

# Organize files in Visual Studio
foreach(source IN LISTS ATLAS_ENGINE_SOURCES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

target_include_directories(Atlas PRIVATE ${PROJECT_SOURCE_DIR}/include )

target_link_libraries(Atlas PUBLIC
    ${ATLAS_ENGINE_DEPENDENCIES}
)

# Set output name for Windows/Linux
if(WIN32)
    set_target_properties(Atlas PROPERTIES OUTPUT_NAME "Atlas")
elseif(UNIX)
    set_target_properties(Atlas PROPERTIES OUTPUT_NAME "Atlas") 
endif()