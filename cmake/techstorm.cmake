
# Go through the list of files and, if the file is a .h, try to find a corrosponding .cpp file and add it to the list
function(complete_source_pairs SOURCES)
	message(STATUS "complete_source_pairs: ${SOURCES}")
	foreach(source IN LISTS SOURCES)
		message(STATUS "Completing Source Pair for: ${source}")
		if(source MATCHES "\\.h$")
			string(REPLACE ".h" ".cpp" source_cpp "${source}")
			list(APPEND SOURCES "${source_cpp}")
		endif()
	endforeach()
	set(${SOURCES} ${SOURCES} PARENT_SCOPE)
	message(STATUS "Completed Source Pairs: ${SOURCES}")
endfunction()

function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

function(setup_output_directories)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib") # Static libraries
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # Shared libraries (.so/.dll)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # Executables (.out/.exe)

endfunction()