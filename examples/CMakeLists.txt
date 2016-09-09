add_executable(bitslicearray bitslicearray.cpp ${HEADERS})
add_executable(fixedlengtharray fixedlengtharray.cpp ${HEADERS})
add_executable(fibonacciarray fibonacciarray.cpp ${HEADERS})
add_executable(filter11array filter11array.cpp ${HEADERS})
add_executable(serialize serialize.cpp ${HEADERS})
target_link_libraries(serialize ${HDF5_LIBRARIES})
add_executable(succinct succinct.cpp ${HEADERS})

