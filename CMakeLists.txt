cmake_minimum_required(VERSION 4.2.0)

project(distributed_key_value_store LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies
find_package(Protobuf REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)

set(PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
set(PROTO_FILES "${PROTO_DIR}/raft.proto" "${PROTO_DIR}/kv_command.proto")
set(PROTO_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/proto_gen")
file(MAKE_DIRECTORY "${PROTO_GEN_DIR}")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(main src/main.cpp)

protobuf_generate(
    TARGET main
    LANGUAGE cpp
    PROTOS ${PROTO_FILES}
    IMPORT_DIRS ${PROTO_DIR}
    PROTOC_OUT_DIR ${PROTO_GEN_DIR}
)

target_include_directories(main PRIVATE "${PROTO_GEN_DIR}" "${INCLUDE_DIR}")

target_link_libraries(
    main
    PRIVATE protobuf::libprotobuf Boost::system Boost::thread
)
