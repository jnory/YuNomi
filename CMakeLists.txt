cmake_minimum_required(VERSION 2.8.4)
project(yunomi)

set(CMAKE_MACOSX_RPATH FALSE)

find_package(Boost COMPONENTS python REQUIRED)
find_package(PythonLibs REQUIRED)

add_definitions(-DDALM_MAX_ORDER=6)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto")
include_directories(${PROJECT_BINARY_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})

set(HEADERS
    include/array_value_reader.h
    include/fib_array.h
    include/fib_coder.h
    include/file_value_reader.h
    include/gjc_array.h
    include/value_reader.h
    include/y_bitarray.h
    include/y_defines.h
    include/y_rank11.h
    include/y_select11.h
)


add_library(_yunomi SHARED
    python/_yunomi.cpp
    ${HEADERS}
    )

target_link_libraries(
    _yunomi
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARY}
)
set_target_properties(_yunomi PROPERTIES PREFIX "")
set_target_properties(_yunomi PROPERTIES SUFFIX ".so")

INSTALL(FILES ${HEADERS} DESTINATION include)
INSTALL(TARGETS _yunomi LIBRARY DESTINATION python)
INSTALL(FILES python/yunomi.py DESTINATION python)