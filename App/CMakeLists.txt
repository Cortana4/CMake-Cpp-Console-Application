cmake_minimum_required(VERSION 3.22)

project("App Project")

## target binary file
set(TARGET_NAME "app")

## header files
file(GLOB_RECURSE INC_FILES "include/*.h")

## source files
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

## add source and header files to this project's executable
add_executable("${TARGET_NAME}" ${SRC_FILES} ${INC_FILES})

## add include directories
target_include_directories("${TARGET_NAME}" PRIVATE "include")

## add libraries
# <lib>
#find_package(<lib> CONFIG REQUIRED)
#target_link_libraries("${TARGET_NAME}" PRIVATE <lib>)

## make assets available in the build directory via symlink
#util_add_post_build_create_symlink("${TARGET_NAME}"
#	"${CMAKE_CURRENT_SOURCE_DIR}/resources"
#	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources"
#)

## install exetutable, shared and static libraries
install(TARGETS "${TARGET_NAME}" RUNTIME DESTINATION "${RUNTIME_INSTALL_DIRECTORY}")
install(TARGETS "${TARGET_NAME}" LIBRARY DESTINATION "${LIBRARY_INSTALL_DIRECTORY}")
install(TARGETS "${TARGET_NAME}" ARCHIVE DESTINATION "${ARCHIVE_INSTALL_DIRECTORY}")

util_install_dependencies(
	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}"
	"${LIBRARY_INSTALL_DIRECTORY}"
	"${INSTALL_SYSTEM_LIBRARIES}"
)

## install assets
#install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION "${RUNTIME_INSTALL_DIRECTORY}")
