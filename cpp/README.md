# TTS CPP

## Compilation

0. Install prerequisites
```bash
brew install cmake libsndfile zlib
```

1. Compile the onnxruntime library
```bash
git clone https://github.com/microsoft/onnxruntime.git
cd onnxruntime
git checkout v1.19.2 # or the version you want to use

# For macOS arm64 run the following command
./build.sh --config RelWithDebInfo --build_shared_lib --parallel --compile_no_warning_as_error --skip_submodule_sync --cmake_extra_defines CMAKE_OSX_ARCHITECTURES=arm64
```

2. Compile the TTS library
```bash
mkdir build
cd build
cmake ..
make
./tts_cpp
```

3. Run tests
```bash
cd build
ctest
```

## Linter

```bash
cppcheck -x c++ --enable=all --inconclusive --library=posix text_utils.cpp
cpplint *.cpp *.h
```