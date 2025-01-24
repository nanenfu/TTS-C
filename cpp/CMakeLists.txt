cmake_minimum_required(VERSION 3.5)
project(ONNXTTS LANGUAGES CXX VERSION 0.0.1)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)

# Set ONNX Runtime paths
set(ONNXRUNTIME_DIR "../../onnxruntime")
include_directories(${ONNXRUNTIME_DIR}/include)

# if CMAKE_BUILD_TYPE is Release, use the Release build of ONNX Runtime
# if CMAKE_BUILD_TYPE is Debug, use the Debug build of ONNX Runtime
# if CMAKE_BUILD_TYPE is not set, use the RelWithDebInfo build of ONNX Runtime
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin") # macOS
    link_directories(${ONNXRUNTIME_DIR}/build/MacOS/${CMAKE_BUILD_TYPE})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    link_directories(${ONNXRUNTIME_DIR}/build/Linux/RelWithDebInfo)
else()
    message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()

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

FetchContent_Declare(
  json URL
  https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
  libsndfile
  GIT_REPOSITORY https://github.com/libsndfile/libsndfile
  GIT_TAG        1.2.2)
FetchContent_MakeAvailable(libsndfile)

# Tests
add_executable(tests
                tts_engine.cpp
                vits.cpp
                encoder.cpp
                fsdecoder.cpp
                ssdecoder.cpp
                text_preprocessor.cpp
                string_utils.cpp
                onnx_utils.cpp
                nlp.cpp
                ssl_content.cpp
                g2p.cpp
                tests/test_text_preprocessor.cpp
                tests/test_string_utils.cpp
                tests/test_nlp.cpp
                tests/test_encoder.cpp
                tests/test_ssl_content.cpp
                tests/test_g2p.cpp
                tests/test_tts_engine.cpp
)

target_link_libraries(tests
                      PRIVATE
                        cnpy
                        onnxruntime
                        Catch2::Catch2WithMain
                        nlohmann_json::nlohmann_json
                      )
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(tests PRIVATE ${cnpy_SOURCE_DIR})

# Automatic test registration
# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#automatic-test-registration
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests)

# Main executable
add_executable( tts_cpp
                tts_engine.cpp
                vits.cpp
                encoder.cpp
                fsdecoder.cpp
                ssdecoder.cpp
                string_utils.cpp
                onnx_utils.cpp
                nlp.cpp
                text_preprocessor.cpp
                ssl_content.cpp
                g2p.cpp
                main.cpp
)


target_link_libraries(tts_cpp
                    PRIVATE
                        cnpy
                        onnxruntime
                        SndFile::sndfile
                        nlohmann_json::nlohmann_json
                    )

target_include_directories(tts_cpp PRIVATE ${cnpy_SOURCE_DIR})

# Library
add_library(sealtts
            SHARED
              tts_engine.cpp
              vits.cpp
              encoder.cpp
              fsdecoder.cpp
              ssdecoder.cpp
              string_utils.cpp
              onnx_utils.cpp
              nlp.cpp
              text_preprocessor.cpp
              ssl_content.cpp
              g2p.cpp
)

target_link_libraries(sealtts
                    PRIVATE
                        cnpy
                        onnxruntime
                        nlohmann_json::nlohmann_json
                    )

target_include_directories(sealtts PRIVATE ${cnpy_SOURCE_DIR})

# Additional files
file(COPY onnx/ DESTINATION onnx)
file(COPY ssl_content.npy DESTINATION .)
file(COPY g2p_en_dict.json DESTINATION .)
