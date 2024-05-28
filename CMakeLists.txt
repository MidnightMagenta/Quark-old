cmake_minimum_required(VERSION 3.10)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ProjectName "Quark")

project("${ProjectName}")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
find_package(OpenGL REQUIRED)

add_library("${ProjectName}" STATIC
	#src files
	src/window.cpp
	
	#header files
	include/window.hpp
	include/vector.hpp
	include/matrix.hpp

	dependencies/OpenGL.h
	dependencies/wglext.h
	dependencies/glext.h
	dependencies/KHR/khrplatform.h
)
target_link_libraries("${ProjectName}" OpenGL::GL)
target_include_directories("${ProjectName}" PUBLIC include dependencies dependencies/KHR)

add_executable("${ProjectName}-test"
	#src files
	src/main.cpp
	#header files
)
target_link_libraries("${ProjectName}-test" "${ProjectName}")