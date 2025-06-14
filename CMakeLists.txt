cmake_minimum_required(VERSION 3.10)
project(projek-akhir2)

cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)
set(GLAD_SRC ./glad/src/glad.c)
set(GLAD_INC ./glad/include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/**/*.cpp)

set(IMGUI_SOURCES
    ./imgui/imgui.cpp
    ./imgui/imgui_demo.cpp
    ./imgui/imgui_draw.cpp
    ./imgui/imgui_tables.cpp
    ./imgui/imgui_widgets.cpp
    ./imgui/backends/imgui_impl_glfw.cpp
    ./imgui/backends/imgui_impl_opengl3.cpp
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(SDL2 REQUIRED)
find_library(SDL2_MIXER_LIBRARY SDL2_mixer)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${IMGUI_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
	./include/
	./imgui/
	./imgui/backends
    ./tmp/rapidjson/include/
	${GLAD_INC}
)
target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw SDL2::SDL2 ${SDL2_MIXER_LIBRARY})
target_sources(${PROJECT_NAME} PRIVATE ${GLAD_SRC})

