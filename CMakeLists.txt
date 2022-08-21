cmake_minimum_required(VERSION 3.1)
project(myprojectdbscan VERSION 0.1.0)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -march=native")

find_package(OpenCL REQUIRED)
find_package(Boost REQUIRED)
find_package(PythonLibs REQUIRED)
#find_package(Eigen3 REQUIRED)

set(NUMPY_INCLUDE_DIRS "$ENV{HOME}/.local/lib/python3.8/site-packages/numpy/core/include")
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
#    ${OpenCL_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
#    ${EIGEN3_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
    ${NUMPY_INCLUDE_DIRS}
)

set(SRCS src/dbscan.cpp src/utils.cpp)
add_executable(dbscan ${SRCS})
target_link_libraries(dbscan ${PYTHON_LIBRARIES})

#enable_testing()
