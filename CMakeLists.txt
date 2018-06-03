cmake_minimum_required(VERSION 3.4.3)
project(yunomi)

find_package(Doxygen)
IF(DOXYGEN_FOUND)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
        add_custom_target(
                doc
                ${DOXYGEN_EXECUTABLE} 
                ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
ENDIF(DOXYGEN_FOUND)

find_package(HDF5 COMPONENTS CXX REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -fPIC -g")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto")

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${HDF5_INCLUDE_DIRS})

set(HEADERS
        ${PROJECT_SOURCE_DIR}/src/yunomi/array/bitslicearray.hpp
        ${PROJECT_SOURCE_DIR}/src/yunomi/array/fibonacciarray.hpp
        ${PROJECT_SOURCE_DIR}/src/yunomi/array/filter11array.hpp
        ${PROJECT_SOURCE_DIR}/src/yunomi/array/fixedlengtharray.hpp
        ${PROJECT_SOURCE_DIR}/src/yunomi/code/fibonacci.hpp
        ${PROJECT_SOURCE_DIR}/src/yunomi/core/defines.hpp
        ${PROJECT_SOURCE_DIR}/src/yunomi/core/filter11.hpp
        ${PROJECT_SOURCE_DIR}/src/yunomi/core/register.hpp
        ${PROJECT_SOURCE_DIR}/src/yunomi/core/succinct.hpp
        ${PROJECT_SOURCE_DIR}/src/yunomi/io/file.hpp
        ${PROJECT_SOURCE_DIR}/src/yunomi/io/hdf5.hpp
        ${PROJECT_SOURCE_DIR}/src/yunomi/serialize/hdf5.hpp
        ${PROJECT_SOURCE_DIR}/src/yunomi/succinct/dual_block_index.hpp
        ${PROJECT_SOURCE_DIR}/src/yunomi/succinct/rank.hpp
        ${PROJECT_SOURCE_DIR}/src/yunomi/succinct/select.hpp
        ${PROJECT_SOURCE_DIR}/src/yunomi/types/bigint.hpp
        )

add_subdirectory(tests)
add_subdirectory(examples)

