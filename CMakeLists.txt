cmake_minimum_required(VERSION 4.2.0)
project(distributed_key_value_store)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

add_executable(main src/main.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_include_directories(main PRIVATE ${PROTO_DIR})
target_link_libraries(main PRIVATE protobuf::libprotobuf)
