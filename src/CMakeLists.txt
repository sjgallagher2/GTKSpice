# CMakeLists.txt in src/
link_directories(
	${GTKMM_LIBRARY_DIRS} )

include_directories(
	${GTKMM_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} )

file(GLOB SOURCES "app/*.cpp" "gui/*.cpp" "app/*.h" "gui/*.h" "testing/*cpp" "testing/*.h")	# Set basic sources variable
add_executable(gtkspice ${SOURCES})

target_link_libraries(gtkspice
	${GTKMM_LIBRARIES} )

