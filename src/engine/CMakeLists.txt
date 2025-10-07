set(ATLAS_ENGINE_SOURCES

	#-----------------------------------
	# Engine Core
	#-----------------------------------
    "core/Core.h"
    "core/Version.h"
    
    "core/MakeNonCopyable.h"

    "core/Common.h"
    
    "core/Math.h"

    "core/Device.h"
	"core/Device.cpp"

    "core/Module.h"
    #"common.cpp"

    "core/Timer.h"
    "core/Timer.cpp"

    "core/MemoryCounter.h"
    "core/MemoryCounter.cpp"

    "core/MemoryAllocator.h"
    "core/MemoryAllocator.cpp"
    
    "core/IAtlasEngine.h"
    "core/AtlasEngine.h"
    "core/AtlasEngine.cpp"

    "core/threading/GameThread.h"
    "core/threading/GameThread.cpp"

    "core/threading/GameThreadScheduler.h"
    "core/threading/GameThreadScheduler.cpp"

    "core/Task.h"

    "core/threading/GameThreader.h"
	"core/threading/GameThreader.cpp"

    "core/threading/ThreadSafeVariable.h"
    "core/threading/ThreadSafeVariable.cpp"

	#-----------------------------------
	# Graphics
	#-----------------------------------

    "graphics/Frame.h"
    "graphics/Frame.cpp"

    "graphics/DeletionQueue.h"
	"graphics/DeletionQueue.cpp"

    "graphics/VulkanInstance.h"
	"graphics/VulkanInstance.cpp"

    "graphics/PhysicalDevice.h"
	"graphics/PhysicalDevice.cpp"

    "graphics/Renderer.h"
    "graphics/Renderer.cpp"

    "graphics/VulkanRenderer.h"
	"graphics/VulkanRenderer.cpp"

    "graphics/VulkanSwapchain.h"
	"graphics/VulkanSwapchain.cpp"

    "graphics/backend/RenderingBackend.h"
    "graphics/backend/RenderingBackend.cpp"

    "graphics/DescriptorLayoutBuilder.h"
	"graphics/DescriptorLayoutBuilder.cpp"

    "graphics/backend/VKDevice.h"
    "graphics/backend/VKDevice.cpp"

    "graphics/window/Window.h" 
    "graphics/window/Window.cpp" 

    "graphics/Renderer.h" 
    "graphics/Renderer.cpp"

    "graphics/AllocatedImage.h"

    "graphics/Color.h"

	#-----------------------------------
    # IO
	#-----------------------------------

    "io/IOCommon.h"

    "io/File.h"
    "io/File.cpp"

    "io/PathLocation.h"

    "io/PathSearcher.h"
	"io/PathSearcher.cpp"

    "io/FileSystemMap.h"
	"io/FileSystemMap.cpp"

    "io/IOManager.h"
	"io/IOManager.cpp"

	#-----------------------------------
    # Scripting
    #-----------------------------------

    "scripting/ScriptedObject.h"
	"scripting/ScriptedObject.cpp"

    #-----------------------------------
	# Debugging
    #-----------------------------------

    "debugging/Logging.h"
    "debugging/Logging.cpp"

    "debugging/Debugger.h"
    "debugging/Debugger.cpp"
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

    eventpp::eventpp
    glfw
    spdlog
    
    # TODO: these two need to be toggled based on options
    vk-bootstrap::vk-bootstrap
    Vulkan::Vulkan
    vulkan-memory-allocator::vulkan-memory-allocator

    SDL2::SDL2main

    simdjson::simdjson
    tinyxml2::tinyxml2
    boost::boost
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