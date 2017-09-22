# GTKSpice CMakeLists.txt Root
cmake_minimum_required(VERSION 3.7.0)	# Set a minimum CMake version
project(gtk-spice)	# Set CMake project name

file(GLOB SOURCES "src/*.cpp")	# Set basic sources variable
add_executable(gtkspice ${SOURCES})

# Add GTK-2.0 
find_package(GTK2 REQUIRED)
include_directories(src/ ${GTK2_INCLUDE_DIRS})
target_link_libraries(gtkspice ${GTK2_LIBRARIES})
