CMAKE_MINIMUM_REQUIRED(VERSION 3.28)

SET(CMAKE_C_COMPILER c:/mingw64/bin/gcc.exe)
SET(CMAKE_CXX_COMPILER c:/mingw64/bin/g++.exe)

SET(CMAKE_CXX_STANDARD          20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS       OFF)

project(
	OpenGl
	VERSION 1.0.0
	LANGUAGES C CXX)

# Add source files
file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
set( GLAD_GL "" )

add_subdirectory(include/glfw include/glm)

option( GLFW-CMAKE-STARTER-USE-GLFW-GLAD "Use GLAD from GLFW" ON )

if( GLFW-CMAKE-STARTER-USE-GLFW-GLAD )
    include_directories("${GLFW_SOURCE_DIR}/deps")
    set( GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h" )
endif()

add_executable(OpenGL ${SOURCE_FILES} ${HEADER_FILES} ${GLAD_GL})
target_link_libraries(OpenGL ${OPENGL_LIBRARIES} glfw )