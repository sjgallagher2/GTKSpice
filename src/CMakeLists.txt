# CMakeLists.txt in src/
link_directories(
	${GTKMM_LIBRARY_DIRS} ${RSVG_LIBRARY_DIRS})

include_directories(
	${GTKMM_INCLUDE_DIRS} ${RSVG_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} )

file(GLOB SOURCES "app/*.cpp" "gui/*.cpp" "app/*.h" "gui/*.h" "tools/*.cpp" "tools/*.h" "spice/*.cpp" "spice/*.h" "testing/*cpp" "testing/*.h")	# Set basic sources variable

add_executable(gtkspice ${SOURCES})

target_link_libraries(gtkspice
	${GTKMM_LIBRARIES} ${RSVG_LIBRARIES})

# Set build type
set(CMAKE_BUILD_TYPE Debug)
