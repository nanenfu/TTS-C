#pragma once

#include <vector>
#include <string>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include "fsdecoder.h"

class SSDecoder {
public:
    SSDecoder(const std::string& onnx_model_path, Ort::Env& env, Ort::MemoryInfo& memory_info);
    std::vector<int64_t> run(FSDecoderResult& fsdecoder_result, int early_stop_num, int prefix_len) const;
private:
    Ort::MemoryInfo& memory_info;
    std::unique_ptr<Ort::Session> session;
};
