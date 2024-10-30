#include <iostream>
#include <vector>
#include <string>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include "ssdecoder.h"
#include "onnx_utils.h"

SSDecoder::SSDecoder(const std::string& onnx_model_path, Ort::Env& env, Ort::MemoryInfo& _memory_info)
                    : memory_info(_memory_info)
{
    std::cout << "Loading ONNX models from " << onnx_model_path << std::endl;
    Ort::SessionOptions encoder_session_options;
    session = std::make_unique<Ort::Session>(env, onnx_model_path.c_str(), encoder_session_options);

    std::cout << "SSDecoder model info: " << std::endl;
    display_model_info(*session);
}

std::vector<int64_t> SSDecoder::run(FSDecoderResult& fsdecoder_result, int early_stop_num) const
{
    return std::vector<int64_t>();
}
