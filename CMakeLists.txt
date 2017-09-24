cmake_minimum_required(VERSION 3.8)
project(TestProject)


set(SOURCE_FILES main.cpp glad.c shader.h shader.cpp stb_image.h stb_image.cpp camera.h)
add_executable(TestProject ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

target_link_libraries(${PROJECT_NAME} -L/usr/local/lib -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau)