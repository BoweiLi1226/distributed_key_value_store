cmake_minimum_required(VERSION 4.2.0)

project(distributed_key_value_store LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies
find_package(GTest CONFIG REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

# Paths
set(PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
set(PROTO_FILES "${PROTO_DIR}/raft.proto" "${PROTO_DIR}/kv_command.proto")
set(PROTO_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/proto_gen")
file(MAKE_DIRECTORY "${PROTO_GEN_DIR}")

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(KV_STORE_LIB_DIR "${SRC_DIR}/kv_store")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# libraries
add_library(proto_objects STATIC)

protobuf_generate(
    TARGET proto_objects
    LANGUAGE cpp
    PROTOS ${PROTO_FILES}
    IMPORT_DIRS ${PROTO_DIR}
    PROTOC_OUT_DIR ${PROTO_GEN_DIR}
)

target_include_directories(proto_objects PUBLIC "${PROTO_GEN_DIR}")

target_link_libraries(proto_objects PUBLIC protobuf::libprotobuf)

add_library(
    kv_store_lib
    "${KV_STORE_LIB_DIR}/kv_store.cpp"
    "${KV_STORE_LIB_DIR}/shared_kv_store.cpp"
)

target_include_directories(kv_store_lib PUBLIC "${INCLUDE_DIR}")

target_link_libraries(kv_store_lib PUBLIC proto_objects Boost::system)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE kv_store_lib)

enable_testing()

set(KV_STORE_TEST_DIR "tests/kv_store")
set(TEST_SOURCES
    "${KV_STORE_TEST_DIR}/kv_store_test.cpp"
    "${KV_STORE_TEST_DIR}/shared_kv_store_test.cpp"
)

if(NOT TEST_SOURCES)
    message(
        FATAL_ERROR
        "No test files found in tests/ subdirectory! Please add a .cpp file."
    )
endif()

add_executable(unit_tests ${TEST_SOURCES})

target_link_libraries(
    unit_tests
    PRIVATE kv_store_lib GTest::gtest_main GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
