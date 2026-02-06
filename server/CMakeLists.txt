cmake_minimum_required(VERSION 3.16)

project(UnityLobbyServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(unitylink
    src/unitylink.cpp
)

target_include_directories(unitylink PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(unitylink PRIVATE
    -Wall -Wextra -Wpedantic
)

add_executable(unitylobby_server
    src/main.cpp
)

target_link_libraries(unitylobby_server PRIVATE unitylink)

target_compile_options(unitylobby_server PRIVATE
    -Wall -Wextra -Wpedantic
)

add_executable(unitylobby_vector_replay
    src/vector_replay.cpp
)

target_link_libraries(unitylobby_vector_replay PRIVATE unitylink)

target_compile_options(unitylobby_vector_replay PRIVATE
    -Wall -Wextra -Wpedantic
)
