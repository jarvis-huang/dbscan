cmake_minimum_required(VERSION 3.1)
project(myprojectdbscan VERSION 0.1.0)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -march=native")

add_definitions(-DCL_TARGET_OPENCL_VERSION=120)
set(OpenCL_INCLUDE_DIRS /usr/include)
set(OpenCL_LIBRARY /opt/intel/opencl-1.2-6.4.0.37/lib64/libOpenCL.so)
set(PYTHON_LIBRARIES /usr/lib/x86_64-linux-gnu/libpython3.8.so)
set(PYTHON_INCLUDE_DIRS /usr/include/python3.8)
find_package(Boost REQUIRED COMPONENTS system filesystem chrono timer REQUIRED)
find_package(Eigen3 REQUIRED)

set(NUMPY_INCLUDE_DIRS "/usr/local/lib/python3.8/dist-packages/numpy/core/include/")
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCL_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
    ${NUMPY_INCLUDE_DIRS}
)

message(STATUS "OpenCL_INCLUDE_DIRS: ${OpenCL_INCLUDE_DIRS}")
message(STATUS "OpenCL_LIBRARY: ${OpenCL_LIBRARY}")

# seq version
#set(SRCS src/dbscan.cpp src/utils.cpp src/partgen.cpp)
#add_executable(dbscan ${SRCS})
#target_link_libraries(dbscan ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})

# par version
set(SRCS2 src/dbscan-par.cpp src/utils.cpp src/partgen.cpp src/myocl.cpp)
add_executable(dbscan-par ${SRCS2})
target_link_libraries(dbscan-par ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} ${OpenCL_LIBRARY})
 

#enable_testing()
