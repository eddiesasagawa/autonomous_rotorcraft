project(arc_host)

# Dependencies
# get_directory_property(_vars_before VARIABLES)

find_package( OpenCV REQUIRED )
find_package( cppzmq REQUIRED )
# find_package( Curses REQUIRED )
# find_package( spdlog REQUIRED )

# get_directory_property(_vars VARIABLES)

# list(REMOVE_ITEM _vars _vars_before ${_vars_before})
# foreach(_var IN LISTS _vars)
#     message(STATUS "${_var} = ${${_var}}")
# endforeach()

# include_directories(include ${CURSES_INCLUDE_DIR})

### SPDLOG
set(SPDLOG_PATH $ENV{HOME}/support/spdlog)
set(SPDLOG_BUILD_BENCH OFF)
include_directories(include ${SPDLOG_PATH}/include/spdlog)
add_subdirectory(${SPDLOG_PATH} ${CMAKE_CURRENT_BINARY_DIR}/spdlog)

# library headers and sources
set(ARC_COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../common)
message(STATUS "Showing current source: ${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(
  ${ARC_COMMON_DIR}/interfaces
  ${ARC_COMMON_DIR}/msg
)
set(ARC_COMMON_SRC
  ${ARC_COMMON_DIR}/interfaces/arc_host_interface.cc
  ${ARC_COMMON_DIR}/interfaces/arc_errors.cc
  ${ARC_COMMON_DIR}/msg/arc_host_msg.cc
)

# The main program
set(ARC_HOST_APP_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/app)
set(ARC_HOST_APP_SRC
  ${ARC_HOST_APP_SRC_DIR}/arc_host.cc
  ${ARC_HOST_APP_SRC_DIR}/host2arc_interface.cc
  ${ARC_HOST_APP_SRC_DIR}/user_interface.cc
)

include_directories(
  ${ARC_HOST_APP_SRC_DIR}
)

set(ARC_HOST_SRCS
  main.cc
  ${ARC_HOST_APP_SRC}
)

add_executable(arc_host ${ARC_HOST_SRCS} ${ARC_COMMON_SRC})
target_link_libraries(arc_host
  ${OpenCV_LIBS}
  cppzmq
  # ${CURSES_LIBRARIES}
  spdlog
)
