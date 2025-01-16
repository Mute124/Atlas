# The sources are seperated into different variables for organization reasons
set(ATLAS_ENGINE_CORE_SOURCES
    "Common.h" 
    "Common.cpp"
    "engine.h" 
    "engine.cpp"
    "BasicCore.h" 

    "Project.h" 
    "Project.cpp" 
)

set(ATLAS_RENDERER_SOURCES
    "renderer/Window.h" 
    "renderer/Window.cpp" 

    "renderer/Renderer.h" 
    "renderer/Renderer.cpp"

    "renderer/Color.h" 

    "renderer/IRenderable.h"

    "objects/IGameObject.h" 
    "objects/GameObject.h"
    "objects/GameObject.cpp"
    "objects/PhysicsObject.h"
	"objects/PhysicsObject.cpp"

    "renderer/IEntity.h"
    "renderer/Entity.h"
	"renderer/Entity.cpp"

    "renderer/GameCamera.h" 
    "renderer/GameCamera.cpp" 

    "renderer/RenderLayer.h" 

    "objects/GameObjectRegistry.h"
    "objects/GameObjectRegistry.cpp" 

    "renderer/objects/PlaneObject.h"

    "objects/IGameObjectGate.h"

    "renderer/WindowDecorations.h"

    "renderer/GameUI.h"
    "renderer/GameUI.cpp"

    "renderer/GameModel.h"
    "renderer/GameModel.cpp"
)

set(ATLAS_STATE_SOURCES
    "states/IState.h"
    "states/IState.cpp"

	"states/State.h"
	"states/State.cpp"

    "states/IStateMachine.h"
	"states/IStateMachine.cpp"
)

set(ATLAS_SYSTEM_SOURCES
    "system/OS.h"
    "system/Memory.h"
)

set(ATLAS_UTIL_SOURCES
    "Utils/Singleton.h" 
    "utils/DoublyLinkedList.h" 
    "utils/Gate.h" 
    "utils/Globals.h"
    "utils/MiscUtils.h"

)

set(ATLAS_MATH_SOURCES
    "math/Vector.h" 
    "math/Vector.cpp" 
)

set(ATLAS_INPUT_SOURCES
    "input/Input.h" 
    "input/Input.cpp"
)

set(ATLAS_CONFIG_SOURCES
    "conf/Config.h"
    "conf/Config.cpp"

    "conf/JSONFile.h"
    "conf/JSONFile.cpp"

    "conf/XMLFile.h"
	"conf/XMLFile.cpp"
)

set(ATLAS_FILESYSTEM_SOURCES
    "fs/FileSystem.h"
    "fs/FileSystem.cpp"

    "fs/RegisteredFile.h"
	"fs/RegisteredFile.cpp"

    "fs/FileMeta.h"
	"fs/FileMeta.cpp"

    "fs/FileSystemRegistry.h"
	"fs/FileSystemRegistry.cpp"
)

set(ATLAS_PHYSICS_SOURCES
    "physics/Physics.h" 
    "physics/Physics.cpp"
    "physics/Listeners.h"
    "physics/Listeners.cpp"
    "physics/Layers.h"
    "physics/Layers.cpp"
)

set(ATLAS_MODDING_SOURCES
    "modding/ScriptingAPI.h"
    "modding/ScriptingAPI.cpp"
    "modding/ModManager.h"
    "modding/ModManager.cpp"
    "modding/Mod.h"
    "modding/Mod.cpp"    
)

set(ATLAS_DEBUGGING_SOURCES 
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

set(ATLAS_WORLD_SOURCES
    "world/world.h" 
    "world/tools/Terrain.h" 
)

set(ATLAS_LANGUAGE_SOURCES
    "language/Language.h"
    "language/LocalizationRegistry.h"
	"language/LocalizationRegistry.cpp"
)

set(ATLAS_ENGINE_SOURCES 
	# Engine Core
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
)



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
    
    simdjson::simdjson
    tinyxml2::tinyxml2
)

include("${CMAKE_SOURCE_DIR}/CMakeOptions.txt")

# Set modding related targets based on options
if(${ATLAS_ENABLE_MODDING})
    if(${ATLAS_ENABLE_ANGELSCRIPT})
        set(ATLAS_ENGINE_DEPENDENCIES
            ${ATLAS_ENGINE_DEPENDENCIES}
		    Angelscript::angelscript
        )
        
    endif()

    if(${ATLAS_ENABLE_LUA})
        set(ATLAS_ENGINE_DEPENDENCIES
            ${ATLAS_ENGINE_DEPENDENCIES}
			sol2::sol2
        )
    endif()
endif()

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
    set_target_properties(Atlas PROPERTIES OUTPUT_NAME "Techstorm.dll" )
elseif(UNIX)
    set_target_properties(Atlas PROPERTIES OUTPUT_NAME "Techstorm.so") 
endif()