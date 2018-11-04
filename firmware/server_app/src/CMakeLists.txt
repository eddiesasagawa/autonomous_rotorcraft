project(arc_server)

# Dependencies
find_package( OpenCV REQUIRED )

# The main program
add_executable(arc_server main.cpp)
target_link_libraries(arc_server ${OpenCV_LIBS})
