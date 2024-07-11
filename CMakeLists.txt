cmake_minimum_required(VERSION 3.10)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ProjectName "Quark")

project("${ProjectName}")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
find_package(OpenGL REQUIRED)

add_custom_target(q_start_clock ALL
	COMMENT "Starting console clock"
	COMMAND "${PROJECT_SOURCE_DIR}/console_clock/bin/run.bat"
)
add_custom_target(q_copy_resources ALL
	COMMENT "Copying resources directory"
	COMMAND ${CMAKE_COMMAND} -E copy_directory 
			${PROJECT_SOURCE_DIR}/resources
			${CMAKE_CURRENT_BINARY_DIR}/resources
)

add_library("${ProjectName}-dependencies" STATIC
	dependencies/OpenGL.h
	dependencies/wglext.h
	dependencies/KHR/khrplatform.h
	dependencies/glad/glad.c
	dependencies/STB/stb_image.h
	dependencies/STB/stb_truetype.h
)

target_include_directories("${ProjectName}-dependencies" PUBLIC 
	dependencies 
	dependencies/KHR
)

add_library("${ProjectName}" STATIC
	#src files
	src/window.cpp
	src/object.cpp
	src/GL_assets.cpp
	src/qrk_debug.cpp
	src/draw.cpp	
	src/image.cpp
	src/texture.cpp
	src/event.cpp
	src/stb_truetype.cpp
	src/rect.cpp
	src/render_window.cpp

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
	include/image.hpp
	include/texture.hpp
	include/event.hpp
	include/stb_truetype.hpp
	include/rect.hpp
	include/render_window.hpp
)
target_link_libraries("${ProjectName}" "${ProjectName}-dependencies" OpenGL::GL)
target_include_directories("${ProjectName}" PUBLIC include)

add_executable("${ProjectName}-test"
	#src files
	src/main.cpp
	#header files
)

add_dependencies("${ProjectName}-test" q_start_clock q_copy_resources)
target_link_libraries("${ProjectName}-test" "${ProjectName}")