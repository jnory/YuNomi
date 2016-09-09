include_directories(${gtest_SOURCE_DIR}/include)

set(SOURCES
        array/test_bitslicearray.cpp
        array/test_fibonacciarray.cpp
        array/test_filter11array.cpp
        array/test_fixedlengtharray.cpp
        code/test_fibonacci.cpp
        core/test_filter11.cpp
        core/test_register.cpp
        core/test_succinct.cpp
        io/test_file.cpp
        io/test_hdf5.cpp
        serialize/test_hdf5.cpp
        succinct/test_rank.cpp
        succinct/test_select.cpp
        types/test_bigint.cpp
        )

add_executable(yunomi_tests ${SOURCES})
message("${HDF5_LIBRARIES}")
target_link_libraries(yunomi_tests gtest gtest_main ${HDF5_LIBRARIES})
