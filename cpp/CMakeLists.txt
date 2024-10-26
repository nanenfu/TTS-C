cmake_minimum_required(VERSION 3.5)
project(ONNXTTS LANGUAGES CXX VERSION 0.0.1)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)

# Set ONNX Runtime paths
set(ONNXRUNTIME_DIR "../../onnxruntime")
include_directories(${ONNXRUNTIME_DIR}/include)
link_directories(${ONNXRUNTIME_DIR}/build/MacOS/RelWithDebInfo)

include(FetchContent)

# Fetch libraries
FetchContent_Declare(
  cnpy
  GIT_REPOSITORY https://github.com/rogersce/cnpy.git
  GIT_TAG        master  # or a specific version/tag
)
FetchContent_MakeAvailable(cnpy)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
)

FetchContent_MakeAvailable(Catch2)

# Find packages
find_package(SndFile REQUIRED)

# Tests
add_executable(tests
                text_preprocessor.cpp
                text_utils.cpp
                nlp.cpp
                tests/test_text_preprocessor.cpp
                tests/test_text_utils.cpp
                tests/test_nlp.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Automatic test registration
# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#automatic-test-registration
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests)

# Main executable
add_executable(tts_cpp
                text_utils.cpp
                nlp.cpp
                text_preprocessor.cpp
                main.cpp
)

target_link_libraries(tts_cpp
                    PRIVATE
                        onnxruntime
                        SndFile::sndfile
                        cnpy
                    )

target_include_directories(tts_cpp PRIVATE ${cnpy_SOURCE_DIR})

# Additional files
file(COPY onnx/ DESTINATION onnx)
file(COPY ssl_content.npy DESTINATION .)
