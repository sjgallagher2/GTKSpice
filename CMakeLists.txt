# GTKSpice CMakeLists.txt Root
cmake_minimum_required(VERSION 3.7.0)	# Set a minimum CMake version
project(gtk-spice)	# Set CMake project name

# Add GTKmm
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0) # Look into FindPkgConfig.cmake
pkg_check_modules(RSVG librsvg-2.0) # SVG support

# Add src/
add_subdirectory(src)

# Set build type
set(CMAKE_BUILD_TYPE Debug)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

