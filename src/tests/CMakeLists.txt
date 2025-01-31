set(ATLAS_RENDERER_TESTS
    "renderer/RendererTests.cpp"
)
set(ATLAS_COMPONENTS_TESTS
	"components/ComponentsTests.cpp"
)

set(ATLAS_CREATE_IF_MISSING TRUE)

if(${ATLAS_CREATE_IF_MISSING})
    # Ensure that each file exists. If not, create it as an empty file.
    foreach(source IN LISTS ATLAS_RENDERER_TESTS)
	    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
		    message(STATUS "Creating empty file: ${CMAKE_CURRENT_SOURCE_DIR}/${source}")
		    file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/${source}" "#include <gmock/gmock.h>\n#include <gtest/gtest.h>\n\nnamespace Atlas::Tests {\n\n}\n")
	    endif()
    endforeach()

    foreach(source IN LISTS ATLAS_RENDERER_TESTS)
	    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
		    message(STATUS "Creating empty file: ${CMAKE_CURRENT_SOURCE_DIR}/${source}")
		    file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/${source}" "#include <gmock/gmock.h>\n#include <gtest/gtest.h>\n\nnamespace Atlas::Tests {\n\n}\n")
	    endif()
    endforeach()

endif()

add_executable(AtlasUnitTests 
	"AtlasUnitTests.cpp"
    
    ${ATLAS_RENDERER_TESTS}
)
target_include_directories(project_lib PRIVATE ${PROJECT_SOURCE_DIR}/src/engine)
target_link_libraries(AtlasUnitTests Atlas)

# Organize files in Visual Studio
foreach(source IN LISTS ATLAS_ENGINE_SOURCES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
#include(CTest)

#add_test(NAME AtlasUnitTests
#	COMMAND AtlasUnitTests
#)
# define tests
