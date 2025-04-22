
add_library(hybridsearch_client
        hybridsearch_client.cpp
        hybridsearch_client.h
        ${CMAKE_SOURCE_DIR}/src/network/hybridsearch_thrift/hybridsearchService.cpp
        ${CMAKE_SOURCE_DIR}/src/network/hybridsearch_thrift/hybridsearch_types.cpp
        table_info.h
)

target_include_directories(hybridsearch_client PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_include_directories(hybridsearch_client PUBLIC "${CMAKE_SOURCE_DIR}/src/network/hybridsearch_thrift")
target_include_directories(hybridsearch_client PUBLIC "${CMAKE_SOURCE_DIR}/third_party/thrift/lib/cpp/src")
target_include_directories(hybridsearch_client PUBLIC "${CMAKE_BINARY_DIR}/third_party/thrift/")

file(GLOB_RECURSE
        client_test_cpp
        CONFIGURE_DEPENDS
        test/*.cpp
)

add_executable(hybridsearch_client_test
        ${client_test_cpp}
)

target_link_libraries(
        hybridsearch_client_test
        hybridsearch_client
        thrift.a
        thriftnb.a
        c++.a
        c++abi.a
)

target_include_directories(hybridsearch_client_test PUBLIC "${CMAKE_SOURCE_DIR}/src/network/hybridsearch_thrift")
target_include_directories(hybridsearch_client_test PUBLIC "${CMAKE_SOURCE_DIR}/client/cpp")
target_link_directories(hybridsearch_client_test PUBLIC "${CMAKE_BINARY_DIR}/lib")

