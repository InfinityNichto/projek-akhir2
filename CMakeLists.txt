cmake_minimum_required(VERSION 3.10)
project(projek-akhir2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/**/*.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
	./include/
)
target_link_libraries(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE)


