# TTS CPP

## Compilation

0. Install prerequisites

on macOS:
```bash
brew install cmake zlib
```

on Ubuntu:
```bash
sudo apt-get install build-essential
```

1. Compile the onnxruntime library
```bash
git clone https://github.com/microsoft/onnxruntime.git
cd onnxruntime
git checkout v1.19.2 # or the version you want to use

# For macOS arm64 or Linux arm64 run the following command:
./build.sh --config RelWithDebInfo --build_shared_lib --parallel --compile_no_warning_as_error --skip_submodule_sync --cmake_extra_defines CMAKE_OSX_ARCHITECTURES=arm64
```


2. Compile the TTS library
```bash
mkdir -p build/Release
cd build/Release
cmake -DCMAKE_BUILD_TYPE=Release ../..
make
```

```bash
mkdir -p build/Debug
cd build/Debug
cmake -DCMAKE_BUILD_TYPE=Debug ../..
make
```

3. Run tests
```bash
ctest # or ./tests -V for verbose output
```

4. Run benchmarks
```bash
./tests --benchmark-samples 5
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