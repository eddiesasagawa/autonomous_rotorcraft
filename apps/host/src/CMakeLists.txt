project(arc_host)

# Dependencies
find_package( OpenCV REQUIRED )
find_package( cppzmq REQUIRED )
find_package( Curses REQUIRED )

include_directories(${CURSES_INCLUDE_DIR})

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
set(ARC_HOST_SRCS 
    main.cc
    user_interface.cc
    host2arc_interface.cc
)

add_executable(arc_host ${ARC_HOST_SRCS} ${ARC_COMMON_SRC})
target_link_libraries(arc_host 
    ${OpenCV_LIBS} 
    cppzmq
    ${CURSES_LIBRARIES}
)
