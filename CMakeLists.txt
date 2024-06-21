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
	src/object.cpp
	src/GL_assets.cpp
	src/qrk_debug.cpp
	src/draw.cpp
	src/app_entry_point.cpp
	src/image.cpp
	src/texture.cpp

	#header files
	include/window.hpp
	include/vector.hpp
	include/matrix.hpp
	include/color.hpp
	include/object.hpp
	include/GL_assets.hpp
	include/qrk_debug.hpp
	include/draw.hpp
	include/constants.hpp
	include/units.hpp
	include/app_entry_point.hpp
	include/image.hpp
	include/texture.hpp

	#dependencies
	dependencies/OpenGL.h
	dependencies/wglext.h
	dependencies/KHR/khrplatform.h
	dependencies/glad/glad.c
	dependencies/STB/stb_image.h
)
target_link_libraries("${ProjectName}" OpenGL::GL)
target_include_directories("${ProjectName}" PUBLIC include dependencies dependencies/KHR)

add_executable("${ProjectName}-test"
	#src files
	src/main.cpp
	#header files
)
add_custom_command(TARGET "${ProjectName}-test" POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory 
		${PROJECT_SOURCE_DIR}/objects
		${CMAKE_CURRENT_BINARY_DIR}/objects
	COMMAND ${CMAKE_COMMAND} -E copy_directory 
		${PROJECT_SOURCE_DIR}/shaders
		${CMAKE_CURRENT_BINARY_DIR}/shaders
			COMMAND ${CMAKE_COMMAND} -E copy_directory 
		${PROJECT_SOURCE_DIR}/textures
		${CMAKE_CURRENT_BINARY_DIR}/textures
)
target_link_libraries("${ProjectName}-test" "${ProjectName}")