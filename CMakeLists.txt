cmake_minimum_required(VERSION 3.20)
project(pool)

set(CMAKE_CXX_STANDARD 17)

add_executable(pool
    src/main.cpp
    # IMPORTANT: list ALL your .cpp files here
    src/area.cpp
    src/ball.cpp
    src/button.cpp
    src/font.cpp
    src/math.cpp
    src/physics.cpp
    src/renderwindow.cpp
    src/texturemanager.cpp
)

target_include_directories(pool PRIVATE
    ${CMAKE_SOURCE_DIR}/src/include
)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)


target_link_libraries(pool PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)