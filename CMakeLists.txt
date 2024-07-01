cmake_minimum_required(VERSION 3.8)

## enable hot reload for MSVC compilers if supported.
if(POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
		"$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
	)
endif()

project("Console Template Project")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## target binary file
set(TARGET_FILE
	"Cpp-Console-Application"
)

## header files
set(INC_FILES
	
)

## source files
set(SRC_FILES
	"src/main.cpp"
)

## resource files
set(RESOURCE_FILES
	
)

## add source and header files to this project's executable
add_executable(${TARGET_FILE} ${SRC_FILES} ${INC_FILES})

## add include directories
target_include_directories(${TARGET_FILE} PRIVATE "include")

## add libraries
# <lib>
#find_package(<lib> CONFIG REQUIRED)
#target_link_libraries(${TARGET_FILE} PRIVATE <lib>)

## copy resource files to build directory
add_custom_target(copy_resources ALL DEPENDS ${RESOURCE_FILES})
add_dependencies(${TARGET_FILE} copy_resources)
foreach(RESOURCE_FILE ${RESOURCE_FILES})
	add_custom_command(
		TARGET copy_resources
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_FILE}
		${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_FILE}
	)
endforeach()

## copy dynamic library and resource files from build to install directory
install(TARGETS ${TARGET_FILE} DESTINATION ${CMAKE_INSTALL_PREFIX})
if(NOT "${RESOURCE_FILES}" STREQUAL "")
	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resources DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()
