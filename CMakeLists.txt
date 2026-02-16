cmake_minimum_required(VERSION 4.2.0)
project(distributed_key_value_store)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Protobuf REQUIRED)
set(PROTO_FILES proto/raft.proto)
set(PROTO_DIR "${CMAKE_CURRENT_BINARY_DIR}/proto")

protobuf_generate_cpp(
    PROTO_SRCS
    PROTO_HDRS
    ${PROTO_FILES}
    GENERATE_EXTENSIONS
    cpp
)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})

add_executable(main src/main.cpp ${PROTO_SRCS} ${PROTO_HDRS})

target_link_libraries(main PRIVATE protobuf::libprotobuf)

target_include_directories(main PRIVATE ${PROTO_DIR} ${INCLUDE_DIR})
