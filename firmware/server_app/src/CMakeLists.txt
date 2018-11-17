project(arc_server)

# Dependencies
find_package( OpenCV REQUIRED )
find_package( cppzmq REQUIRED )

# library headers and sources
set(ARC_COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../common)
message(STATUS "Showing current source: ${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(
    ${ARC_COMMON_DIR}/interfaces
    ${ARC_COMMON_DIR}/msg
)
set(ARC_COMMON_SRC 
    ${ARC_COMMON_DIR}/interfaces/arc_host_interface.cc 
    ${ARC_COMMON_DIR}/msg/arc_host_msg.cc
)

# The main program
add_executable(arc_server main.cpp)
target_link_libraries(arc_server ${OpenCV_LIBS} cppzmq)
