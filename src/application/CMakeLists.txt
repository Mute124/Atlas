﻿# Application: Main executable

if(WIN32)
	set(ATLAS_MAIN_APPLICATION_RC "Application.rc")
else()
	set(ATLAS_MAIN_APPLICATION_RC "")
endif()

add_executable(main_app 
	"main.cpp"
	"ApplicationUtils.h"
	"ApplicationUtils.cpp"
	"AtlasApp.h"
	"AngelScriptTest.h"
	"AngelScriptTest.cpp"
	"add_on/scriptstdstring/scriptstdstring.cpp"
	${ATLAS_MAIN_APPLICATION_RC}
	
    #"Shell.h"
    #"Shell.cpp"
)

# Link to the project shared library
target_link_libraries(main_app PRIVATE project_lib)

# Include directories
target_include_directories(main_app PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_include_directories(main_app PRIVATE "${PROJECT_SOURCE_DIR}/src/project")
target_include_directories(main_app PRIVATE "${PROJECT_SOURCE_DIR}/src/engine")