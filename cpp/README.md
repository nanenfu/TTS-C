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
mkdir -p build/RelWithDebInfo
cd build/RelWithDebInfo
cmake ../..
make
./tts_cpp
```

3. Run tests
```bash
cd build/RelWithDebInfo
ctest # or ./tests -V for verbose output
```

## Linter

```bash
cpplint *.cpp *.h
```

## TODO

- [ ] Implement NLP::seg_text in text_preprocessor.cpp to segment different language segments
- [ ] NLP::g2p doesn't use the g2p model to detect phones that are not in the g2p_en_dict.json
- [ ] [Refactoring] Split preprocessor and tokenizer into two different classes

## Release

Compile onnxruntime in a Release mode:
```bash
./build.sh --config Release --build_shared_lib --parallel --compile_no_warning_as_error --skip_submodule_sync --cmake_extra_defines CMAKE_OSX_ARCHITECTURES=arm64
```


```bash
mkdir -p build/Release
cd build/Release
cmake -DCMAKE_BUILD_TYPE=Release ../..
make
```