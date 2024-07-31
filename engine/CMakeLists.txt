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

add_library("${ProjectName}-engine" STATIC
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
        src/glyph_renderer.cpp
        src/stb_rectPack.cpp
        src/misc_functions.cpp

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
        include/stb_rectPack.hpp
        include/rect.hpp
        include/render_window.hpp
        include/glyph_renderer.hpp
        include/misc_functions.hpp
)
target_link_libraries("${ProjectName}-engine" "${ProjectName}-dependencies" OpenGL::GL)
target_include_directories("${ProjectName}-engine" PUBLIC Engine/include)