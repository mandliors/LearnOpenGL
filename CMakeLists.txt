cmake_minimum_required(VERSION 3.11)

project(opengl_test)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

add_executable(opengl_test)

target_include_directories(opengl_test PRIVATE
    src vendor/include
)
target_sources(opengl_test PRIVATE
    vendor/src/glad.c
    src/main.cpp
)

target_compile_options(opengl_test PRIVATE
    -fsanitize=address -fno-omit-frame-pointer
)

target_link_directories(opengl_test PRIVATE
    vendor/lib
)
target_link_options(opengl_test PRIVATE
    -fsanitize=address
)
target_link_libraries(opengl_test PRIVATE
    glfw3 GL X11 pthread Xrandr Xi dl m ${CMAKE_DL_LIBS}
)