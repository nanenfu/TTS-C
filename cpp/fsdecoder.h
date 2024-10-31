#pragma once

#include <string>
#include <vector>
#include <memory>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include "encoder.h"

struct FSDecoderResult {
    std::vector<int64_t > y;
    std::vector<int64_t> y_shape;
    std::vector<float> k;
    std::vector<int64_t> k_shape;
    std::vector<float> v;
    std::vector<int64_t> v_shape;
    std::vector<float> y_emb;
    std::vector<int64_t> y_emb_shape;
    std::vector<float> x_example;
    std::vector<int64_t> x_example_shape;
};

class FSDecoder {
public:
    FSDecoder(const std::string& onnx_model_path, Ort::Env& env, Ort::MemoryInfo& memory_info);
    FSDecoderResult run(EncoderResult& encoder_result) const;

private:
    Ort::MemoryInfo& memory_info;
    std::unique_ptr<Ort::Session> session;
};
