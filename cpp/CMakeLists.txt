cmake_minimum_required(VERSION 3.26)
project(ONNXTTS)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)

# Set ONNX Runtime paths
set(ONNXRUNTIME_DIR "../../onnxruntime")
include_directories(${ONNXRUNTIME_DIR}/include)
link_directories(${ONNXRUNTIME_DIR}/build/MacOS/RelWithDebInfo)

include(FetchContent)

# Fetch cnpy from GitHub
FetchContent_Declare(
  cnpy
  GIT_REPOSITORY https://github.com/rogersce/cnpy.git
  GIT_TAG        master  # or a specific version/tag
)
FetchContent_MakeAvailable(cnpy)

# Find libsndfile
find_package(SndFile REQUIRED)

add_executable(tts_cpp main.cpp text_preprocessor.cpp)

target_link_libraries(tts_cpp
                    PRIVATE
                        onnxruntime
                        SndFile::sndfile
                        cnpy
                    )

target_include_directories(tts_cpp PRIVATE ${cnpy_SOURCE_DIR})

file(COPY onnx/ DESTINATION onnx)
file(COPY ssl_content.npy DESTINATION .)

message(STATUS "Include directories for MyExecutable: ${CMAKE_CURRENT_SOURCE_DIR}")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "Include dir='${dir}'")
endforeach()
