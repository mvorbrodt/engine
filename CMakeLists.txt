cmake_minimum_required(VERSION 3.0.0)

project(engine VERSION 0.1
	DESCRIPTION "3D engine from https://vorbrodt.blog"
	LANGUAGES C CXX)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include_directories("/usr/local/include")
link_directories("/usr/local/lib")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLFW3 REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++2a")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -march=native")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

add_subdirectory(src)

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
