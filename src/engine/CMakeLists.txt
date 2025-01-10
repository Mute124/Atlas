

# The sources are seperated into different variables for organization reasons
set(TS_ENGINE_CORE_SOURCES
    "Common.h" 
    "Common.cpp"
    "engine.h" 
    "engine.cpp"
    "BasicCore.h" 

    "Project.h" 
    "Project.cpp" 
)

set(TS_ENGINE_RENDERER_SOURCES
    "renderer/Window.h" 
    "renderer/Window.cpp" 

    "renderer/Renderer.h" 
    "renderer/Renderer.cpp"

    "renderer/Color.h" 

    "renderer/IRenderable.h"

    "renderer/IGameObject.h" 
    "renderer/GameObject.h"
    "renderer/GameObject.cpp"
    "renderer/PhysicsObject.h"
	"renderer/PhysicsObject.cpp"

    "renderer/IEntity.h"
    "renderer/Entity.h"
	"renderer/Entity.cpp"

    "renderer/GameCamera.h" 
    "renderer/GameCamera.cpp" 

    "renderer/RenderLayer.h" 

    "renderer/GameObjectRegistry.h"
    "renderer/GameObjectRegistry.cpp" 

    "renderer/objects/PlaneObject.h"

    "renderer/IGameObjectGate.h"

    "renderer/WindowDecorations.h"

    "renderer/GameUI.h"
    "renderer/GameUI.cpp"


)

set(TS_UTIL_SOURCES
    "Utils/Singleton.h" 
    "utils/DoublyLinkedList.h" 
    "utils/Gate.h" 
    "utils/Globals.h"
    "utils/MiscUtils.h"
)

set(TS_MATH_SOURCES
    "math/Vector.h" 
    "math/Vector.cpp" 
)

set(TS_INPUT_SOURCES
    "input/Input.h" 
    "input/Input.cpp"
)

set(TS_CONFIG_SOURCES
    "conf/Config.h"
    "conf/Config.cpp"
)

set(TS_FILESYSTEM_SOURCES
    "fs/FileSystem.h"
    "fs/FileSystem.cpp"
)

set(TS_PHYSICS_SOURCES
    "physics/Physics.h" 
    "physics/Physics.cpp"
    "physics/Listeners.h"
    "physics/Listeners.cpp"
    "physics/Layers.h"
    "physics/Layers.cpp"
)

set(TS_MODDING_SOURCES
    "modding/ScriptingAPI.h"
    "modding/ScriptingAPI.cpp"
    "modding/ModManager.h"
    "modding/ModManager.cpp"
    "modding/Mod.h"
    "modding/Mod.cpp"    
)

set(TS_DEBUGGING_SOURCES 
    "EReturnCode.h"

    "dbg/Logging.h" 
    "dbg/Logging.cpp"

    "dbg/ELoggingMode.h"
    "dbg/ELogLevel.h"

    "dbg/Debugging.h"

    "dbg/Debugger.h"
	"dbg/Debugger.cpp"

    "dbg/Errors.h"

    "dbg/Console.h"
	"dbg/Console.cpp"
)

set(TS_WORLD_SOURCES
    "world/world.h" 
    "world/tools/Terrain.h" 
)

set(ENGINE_SOURCES 
	# Engine Core
    ${TS_ENGINE_CORE_SOURCES}
    ${TS_ENGINE_RENDERER_SOURCES}
    ${TS_UTIL_SOURCES}
	${TS_MATH_SOURCES}
    ${TS_INPUT_SOURCES}
    ${TS_CONFIG_SOURCES}
    ${TS_FILESYSTEM_SOURCES}
	${TS_PHYSICS_SOURCES}
	${TS_MODDING_SOURCES}
	${TS_DEBUGGING_SOURCES}
    ${TS_WORLD_SOURCES}
)

# Please note that this is NOT the final list of dependencies, some are toggled based on options
set(TS_ENGINE_DEPENDENCIES
    raylib 

    joltphysics::joltphysics 
    joltphysics_DEPS_TARGET

    libconfig::libconfig
    libconfig_libconfig_libconfig++_DEPS_TARGET

    eventpp::eventpp

    rmlui::rmlui
    rmlui_DEPS_TARGET

    spdlog::spdlog
    

    # fmt::fmt
    
)

include("${CMAKE_SOURCE_DIR}/CMakeOptions.txt")

# Set modding related targets based on options
if(${TS_ENABLE_MODDING})
    if(${TS_ENABLE_ANGELSCRIPT})
        set(TS_ENGINE_DEPENDENCIES
            ${TS_ENGINE_DEPENDENCIES}
		    Angelscript::angelscript
        )
        
    endif()

    if(${TS_ENABLE_LUA})
        set(TS_ENGINE_DEPENDENCIES
            ${TS_ENGINE_DEPENDENCIES}
			sol2::sol2
        )
    endif()
endif()


add_library(Atlas SHARED 
    ${ENGINE_SOURCES}
)

# Organize files in Visual Studio
foreach(source IN LISTS ENGINE_SOURCES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

target_include_directories(Atlas PRIVATE ${PROJECT_SOURCE_DIR}/include )

target_link_libraries(Atlas PUBLIC
    ${TS_ENGINE_DEPENDENCIES}
)

# Set output name for Windows/Linux
if(WIN32)
    set_target_properties(Atlas PROPERTIES OUTPUT_NAME "Techstorm.dll" )
elseif(UNIX)
    set_target_properties(Atlas PROPERTIES OUTPUT_NAME "Techstorm.so") 
endif()