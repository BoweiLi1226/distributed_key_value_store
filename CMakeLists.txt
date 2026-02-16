cmake_minimum_required(VERSION 4.2.0)
project(distributed_key_value_store)

set(CMAKE_CXX_STANDARD, 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  protobuf
  GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
  GIT_TAG        v33.4
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   TRUE
)
set(protobuf_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(protobuf_INSTALL OFF CACHE BOOL "" FORCE)
set(protobuf_LOCAL_DEPENDENCIES ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(protobuf)

set(protobuf_MODULE_PATH "${protobuf_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${protobuf_MODULE_PATH}")
include(${protobuf_SOURCE_DIR}/cmake/protobuf-macros.cmake)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS raft.proto)

add_executable(main src/main.cpp ${PROTO_SRCS} ${PROTO_HDRS})
