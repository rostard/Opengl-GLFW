cmake_minimum_required(VERSION 3.5)
project(Opengl-GLFW)

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
set(SOURCE_FILES main.cpp glad.c shader.h shader.cpp stb_image.h stb_image.cpp camera.h mesh.h mesh.cpp)
add_executable(Opengl-GLFW ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

target_link_libraries(${PROJECT_NAME} -L/usr/local/lib
         -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lassimp -lstdc++ -lz )
