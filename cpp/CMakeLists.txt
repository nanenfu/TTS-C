cmake_minimum_required(VERSION 3.26)
project(ONNXTTS)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)

# Set ONNX Runtime paths
set(ONNXRUNTIME_DIR "../../onnxruntime")
include_directories(${ONNXRUNTIME_DIR}/include)
link_directories(${ONNXRUNTIME_DIR}/build/MacOS/RelWithDebInfo)

# Find libsndfile
find_package(SndFile REQUIRED)

add_executable(tts_cpp main.cpp)

target_link_libraries(tts_cpp
                    PRIVATE
                        onnxruntime
                        SndFile::sndfile
                    )