set(ATLAS_VULKAN_API_SOURCES
    "graphics/vulkan/DeletionQueue.h"
	"graphics/vulkan/DeletionQueue.cpp"

    "graphics/vulkan/VulkanInstance.h"
	"graphics/vulkan/VulkanInstance.cpp"

    "graphics/vulkan/PhysicalDevice.h"
	"graphics/vulkan/PhysicalDevice.cpp"

    "graphics/vulkan/VulkanRenderer.h"
	"graphics/vulkan/VulkanRenderer.cpp"

    "graphics/vulkan/VulkanSwapchain.h"
	"graphics/vulkan/VulkanSwapchain.cpp"

    "graphics/vulkan/DescriptorLayoutBuilder.h"
	"graphics/vulkan/DescriptorLayoutBuilder.cpp"

    "graphics/vulkan/PipelineBuilder.h"
	"graphics/vulkan/PipelineBuilder.cpp"

    "graphics/vulkan/VKDevice.h"
    "graphics/vulkan/VKDevice.cpp"

    "graphics/vulkan/AllocatedImage.h"
)

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

    "core/Timer.h"
    "core/Timer.cpp"

    "core/MemoryCounter.h"
    "core/MemoryCounter.cpp"

    "core/MemoryAllocator.h"
    "core/MemoryAllocator.cpp"
    
    "core/platform/Platform.h"
	"core/platform/Platform.cpp"

    "core/IAtlasEngine.h"
    "core/AtlasEngine.h"
    "core/AtlasEngine.cpp"

    "core/IFactory.h"

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
    # Input
	#-----------------------------------

    "input/Input.h"

    "input/InputAction.h"
    "input/InputAction.cpp"

    #-----------------------------------
    # Haptics
	#-----------------------------------

    "haptics/HapticDevice.h"
    "haptics/HapticDevice.cpp"

    "haptics/HapticDeviceEffectManager.h"
	"haptics/HapticDeviceEffectManager.cpp"

    "haptics/HapticEffect.h"
	"haptics/HapticEffect.cpp"

	#-----------------------------------
	# Graphics
	#-----------------------------------

    "graphics/test.cpp"

    "graphics/RenderCommon.h"
	"graphics/RenderCommon.cpp"

    "graphics/GraphicsUtils.h"
    "graphics/GraphicsUtils.cpp"

    "graphics/Frame.h"
    "graphics/Frame.cpp"

    ${ATLAS_VULKAN_API_SOURCES}


    "graphics/Renderer.h"
    "graphics/Renderer.cpp"

    "graphics/backend/RenderingBackend.h"
    "graphics/backend/RenderingBackend.cpp"

    "graphics/drawing/Effect.h"
	"graphics/drawing/Effect.cpp"

    "graphics/drawing/EffectManager.h"
	"graphics/drawing/EffectManager.cpp"

    "graphics/window/Window.h" 
    "graphics/window/Window.cpp" 

    "graphics/Renderer.h" 
    "graphics/Renderer.cpp"

    "graphics/Color.h"

	#-----------------------------------
    # IO
	#-----------------------------------

    "io/IOCommon.h"
    "io/IOCommon.cpp"

    "io/FileHandle.h"
    "io/FileHandle.cpp"

    "io/FileRecord.h"
    "io/FileRecord.cpp"

    "io/FileData.h"
    "io/FileData.cpp"

    #"io/PathLocation.h"

    #"io/PathSearcher.h"
	#"io/PathSearcher.cpp"

    #"io/FileSystemMap.h"
	#"io/FileSystemMap.cpp"

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

add_library(imgui STATIC)
target_include_directories(imgui PUBLIC "${PROJECT_SOURCE_DIR}/include/imgui")
target_sources(imgui PUBLIC
  "${PROJECT_SOURCE_DIR}/include/imgui/imgui.h"
  "${PROJECT_SOURCE_DIR}/include/imgui/imgui.cpp"

  "${PROJECT_SOURCE_DIR}/include/imgui/imgui_demo.cpp"
  "${PROJECT_SOURCE_DIR}/include/imgui/imgui_draw.cpp"
  "${PROJECT_SOURCE_DIR}/include/imgui/imgui_widgets.cpp"
  "${PROJECT_SOURCE_DIR}/include/imgui/imgui_tables.cpp"
  "${PROJECT_SOURCE_DIR}/include/imgui/backends/imgui_impl_vulkan.cpp"
  "${PROJECT_SOURCE_DIR}/include/imgui/backends/imgui_impl_sdl2.cpp"
)
# This is requires so IMGUI can use these libraries
target_link_libraries(imgui PUBLIC Vulkan::Vulkan SDL2::SDL2)

# Please note that this is NOT the final list of dependencies, some are toggled based on options
set(ATLAS_ENGINE_DEPENDENCIES
    imgui
    eventpp::eventpp
    #glfw
    spdlog
    
    # TODO: these two need to be toggled based on options
    vk-bootstrap::vk-bootstrap
    Vulkan::Vulkan
    vulkan-memory-allocator::vulkan-memory-allocator
    

    SDL2::SDL2main
    fastgltf::fastgltf
    #simdjson::simdjson
    #tinyxml2::tinyxml2
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