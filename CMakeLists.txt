cmake_minimum_required(VERSION 3.22)

project("Console Template Project")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## target binary file
set(TARGET_FILE "Cpp-Console-Application")

## header files
file(GLOB_RECURSE INC_FILES "include/*.h")

## source files
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

## set build directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${RUNTIME_SUBDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${LIBRARY_SUBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ARCHIVE_SUBDIR}")

## set install directories
set(RUNTIME_INSTALL_DIRECTORY "${CMAKE_INSTALL_PREFIX}/${RUNTIME_SUBDIR}")
set(LIBRARY_INSTALL_DIRECTORY "${CMAKE_INSTALL_PREFIX}/${LIBRARY_SUBDIR}")
set(ARCHIVE_INSTALL_DIRECTORY "${CMAKE_INSTALL_PREFIX}/${ARCHIVE_SUBDIR}")

## add source and header files to this project's executable
add_executable(${TARGET_FILE} ${SRC_FILES} ${INC_FILES})

## add include directories
target_include_directories(${TARGET_FILE} PRIVATE "include")

## add libraries
# <lib>
#find_package(<lib> CONFIG REQUIRED)
#target_link_libraries(${TARGET_FILE} PRIVATE <lib>)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	## On Windows, vcpkg automatically copies dependent shared library files
	## to the build and install directory. Unfortunately, this functionality
	## is not available on linux. Instead, vcpkg sets RPATH/RUNPATH, so that
	## dependent .so files are found at runtime. But the paths set by vcpkg
	## are absolute, which means they are only correct for the build directory.
	## In order to have a self contained install directory, we have to find
	## and install all dependent .so files manually.

	function(add_install_dependencies BINARY OUTPUT_DIRECTORY INCLUDE_SYSTEM_LIBRARIES)
		if(NOT BUILD_SHARED_LIBS)
			return()
		endif()

		## only CMAKE_INSTALL_PREFIX is available in the generated install script, so
		## we have to pass all other variables we want to use manually
		install(CODE "set(BINARY \"${BINARY}\")")
		install(CODE "set(OUTPUT_DIRECTORY \"${OUTPUT_DIRECTORY}\")")
		install(CODE "set(INCLUDE_SYSTEM_LIBRARIES \"${INCLUDE_SYSTEM_LIBRARIES}\")")
		install(CODE "set(CMAKE_BINARY_DIR \"${CMAKE_BINARY_DIR}\")")

		## this adds code to the install script, that recursively gets dependencies and
		## dependencies of dependencies and installs them to OUTPUT_DIRECTORY
		install(CODE [[
			cmake_policy(VERSION 3.22)

			function(install_dependencies LIBRARY)
				## Skip already installed libraries
				get_property(INSTALLED_LIBS GLOBAL PROPERTY INSTALLED_LIBRARIES)
				if(LIBRARY IN_LIST INSTALLED_LIBS)
					return()
				endif()

				## Skip system libraries if disabled
				string(FIND "${LIBRARY}" "${CMAKE_BINARY_DIR}" POSITION)
				if(NOT INCLUDE_SYSTEM_LIBRARIES AND POSITION EQUAL -1)
					return()
				endif()

				## Install library
				file(INSTALL
					DESTINATION "${OUTPUT_DIRECTORY}"
					TYPE SHARED_LIBRARY
					FOLLOW_SYMLINK_CHAIN
					FILES "${LIBRARY}"
				)

				## Mark library as installed
				set_property(GLOBAL APPEND PROPERTY INSTALLED_LIBRARIES "${LIBRARY}")

				## Get dependencies of this library
				file(GET_RUNTIME_DEPENDENCIES
					LIBRARIES ${LIBRARY}
					RESOLVED_DEPENDENCIES_VAR RESOLVED_DEPS
					UNRESOLVED_DEPENDENCIES_VAR UNRESOLVED_DEPS
				)

				## Recursively install dependencies
				foreach(FILE ${RESOLVED_DEPS})
					if(NOT IS_SYMLINK ${FILE})
						install_dependencies(${FILE})
					endif()
				endforeach()

				## Report unresolved dependencies
				foreach(FILE ${UNRESOLVED_DEPS})
					message(STATUS "Unresolved from ${LIBRARY}: ${FILE}")
				endforeach()
			endfunction()

			## Initialize a global list to store installed libraries
			set_property(GLOBAL PROPERTY INSTALLED_LIBRARIES "")

			## Get runtime dependencies of the main executable
			file(GET_RUNTIME_DEPENDENCIES
				EXECUTABLES ${BINARY}
				RESOLVED_DEPENDENCIES_VAR RESOLVED_DEPS
				UNRESOLVED_DEPENDENCIES_VAR UNRESOLVED_DEPS
			)

			## Install all resolved dependencies
			foreach(FILE ${RESOLVED_DEPS})
				install_dependencies(${FILE})
			endforeach()

			## Report unresolved dependencies
			foreach(FILE ${UNRESOLVED_DEPS})
				message(STATUS "Unresolved: ${FILE}")
			endforeach()
		]])
	endfunction()

	add_install_dependencies(
		${RUNTIME_SUBDIR}/${TARGET_FILE}
		${LIBRARY_INSTALL_DIRECTORY}
		${INSTALL_SYSTEM_LIBRARIES}
	)
endif()

## install exetutable, shared and static libraries
install(TARGETS ${TARGET_FILE} RUNTIME DESTINATION ${RUNTIME_INSTALL_DIRECTORY})
install(TARGETS ${TARGET_FILE} LIBRARY DESTINATION ${LIBRARY_INSTALL_DIRECTORY})
install(TARGETS ${TARGET_FILE} ARCHIVE DESTINATION ${ARCHIVE_INSTALL_DIRECTORY})

## install resources folder
#install(DIRECTORY resources DESTINATION ${RUNTIME_INSTALL_DIRECTORY})
