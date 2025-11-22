add_subdirectory("engine")

# Currently, Techstorm only supports the default implementation, however, it is on the list of things to do.
if(NOT ${ATLAS_LIBRARY_MODE})
    add_subdirectory("project")

	message(STATUS "Building application")
    add_subdirectory("application")
endif()

if(${ATLAS_BUILD_TOOLS})
    message(STATUS "Building tools")
    add_subdirectory("tools")
endif()

if(${ATLAS_ENABLE_TESTING})
    

	message(STATUS "Building tests")
	add_subdirectory("tests")
endif()